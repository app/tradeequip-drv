#include "shtrih-fr.h"
#include <qdatetime.h>
#include <qtextcodec.h>

//using ShtrihFR::Result;

template <class T>
class OClHelper
{
public:
  OClHelper(T * that)
  {
    m_that=that;
    m_res=CRERROR_OK;
    if (m_that->captureMode()==TE::CM_NonExclusive)
      m_res=m_that->open()!=0?CRERROR_OK:CRERROR_DEVICEERROR;
  }
  ~OClHelper()
  {
	  if (m_that->captureMode()==TE::CM_NonExclusive)
      m_res=m_that->close();
  }
  int result() {return m_res;}
private:
  T * m_that;
  int m_res;
};

#define makeHelper if (!isDriverRunning()) {setErrorText(tr("Driver isn't started."));return CRERROR_INVALIDCALL;}\
  OClHelper<ShtrihFR> localOClHelper(this);\
 int loclhres; if ((loclhres=localOClHelper.result())!=CRERROR_OK) return loclhres

Q_ULONG from4Bytes(ShtrihFR::Byte *pBytes /* Byte[4] */)
{
	return ((Q_ULONG)pBytes[0])|(((Q_ULONG)pBytes[1])<<8)|(((Q_ULONG)pBytes[2])<<16)|(((Q_ULONG)pBytes[3])<<24);
}

ShtrihFR::Byte ShtrihFR::m_bTmpOpNum;

ShtrihFR::ShtrihFR() : TECashRegisterBase(1,0)
{
	init();
}

ShtrihFR::ShtrihFR(int pn, Q_ULONG mn) : TECashRegisterBase(pn,mn)
{
	init();
	setPortNumber(pn);
	setMachineNumber(mn);
}

ShtrihFR::~ShtrihFR()
{
}

void ShtrihFR::init()
{
	setName("TECashRegisterShtrihFR");
// MSVC 7.0 doesn't support argument type selection based on more/less specialization.
//	addFuncBinding<ShtrihFR,int>(m_ee,this,&ShtrihFR::open,"open");
//	addFuncBinding<ShtrihFR,int>(m_ee,this,&ShtrihFR::close,"close");
//	addFuncBinding(m_ee,this,&ShtrihFR::internalCutCheck,"internalCutCheck");
//	addFuncBinding(m_ee,this,&ShtrihFR::internalTapeAdvance,"internalTapeAdvance");
//	m_ee.addFuncBinding0<ShtrihFR,Q_ULONG,ConstFunc0<ShtrihFR,Q_ULONG> >(this,&ShtrihFR::readMachineNumber,"readMachineNumber");
	m_ee.addFuncBinding(this,&ShtrihFR::readMachineNumber,"readMachineNumber");
	m_ee.addFuncBinding<ShtrihFR,int>(this,&ShtrihFR::open,"open");
	m_ee.addFuncBinding<ShtrihFR,int>(this,&ShtrihFR::close,"close");
	m_ee.addFuncBinding<ShtrihFR,int,int,int&>(this,&ShtrihFR::openCheck,"openCheck");
	m_ee.addFuncBinding(this,&ShtrihFR::cancelPrint,"cancelPrint");
	m_ee.addFuncBinding(this,&ShtrihFR::bueAdd,"addBuy");
	m_ee.addFuncBinding(this,&ShtrihFR::printCheck,"printCheck");
	m_ee.addFuncBinding(this,&ShtrihFR::internalOpenCashbox,"openCashbox");
	m_ee.addFuncBinding<ShtrihFR,QString>(this,&ShtrihFR::errorText,"errorText");
	m_ee.addFuncBinding<ShtrihFR,int,int>(this,&ShtrihFR::setPortNumber,"setPortNumber");
	m_ee.addProcBinding1<ShtrihFR,int>(this,&ShtrihFR::setPortBaudRate,"setBaudRate");
	m_ee.addFuncBinding(this,&ShtrihFR::setPassword,"setPassword");
	m_ee.addFuncBinding(this,&ShtrihFR::internalReadOptions,"readOptions");
	m_ee.addFuncBinding(this,&ShtrihFR::internalDailyReport,"dailyReport");
	m_ee.addFuncBinding(this,&ShtrihFR::internalDailyReportAndClear,"dailyReportAndClear");
	m_ee.addFuncBinding(this,&ShtrihFR::internalClearCheck,"internalClearCheck");
	m_ee.addFuncBinding<ShtrihFR,QVariant,const QString &>(this,&ShtrihFR::value,"value");
	setCaptureMode(CM_NonExclusive);
	setPortBaudRate(4800);
	setPortDataBits(DATA_8);
	setPortFlowControl(FLOW_OFF);
	setPortNumber(1);
	setPortParity(PAR_NONE);
	setPortStopBits(STOP_1);
	setTimeout(byteTimeout());
	m_bOptionsRead=false;
	m_bCheckIsOpen=false;
	m_bOnlinePrintMode=false;
	m_bCrItProcessed=false;
	m_bCrItBAdd=false;
	m_bCrItBAddImplicit=false;
	m_bCrItCompleted=false;
	m_ullQuantityMultiplier=1000;
	m_ullPriceMultiplier=100;
	m_ullPercentMultiplier=100;
}

ShtrihFR::Result ShtrihFR::sendCommand(Byte const * pCommand,
	int szCmdSize, Byte * pResponce, int szRespBufSize,
	int *pszWrittenBytes, int *pszReadBytes, bool ignorePendingData)
{
	if (szCmdSize<=0)
	{
		setErrorText(tr("szCmdSize must be greater then zero."));
		return RSP_INVALIDPARAMETER2;
	}
	if (szCmdSize>255)
	{
		setErrorText(tr("szCmdSize must be less then 256."));
		return RSP_INVALIDPARAMETER2;
	}
	if (szCmdSize>0 && pCommand==0)
	{
		setErrorText(tr("pCommand must not be NULL, while szCmdSize>0."));
		return RSP_INVALIDPARAMETERS;
	}
	if (szRespBufSize<0)
	{
		setErrorText(tr("szRespBufSize<0"));
		return RSP_INVALIDPARAMETER4;
	}
	//defaulting read/write byte count to zero
	if (pszWrittenBytes) // pointer can be null
		*pszWrittenBytes=0;
	if (pszReadBytes)
		*pszReadBytes=0;
	if (!port()->isOpen())
	{
		setErrorText(tr("Port is closed."));
		return RSP_PORTCLOSED;
	}
	Result result=RSP_TIMEOUT;
	for(int j=0;j<retryCount();j++)
	{
//		port()->setTimeout(0,byteTimeout());
		if (putch(ENQ)==-1)
		{	// Timeout or hardware error
			setErrorText(tr("Timeout while sending ENQ byte."));
			setOptionsRead(false); //Device is offline. We should read device setting next time.
			return RSP_TIMEOUT;
		}
		// We should wait for byte more then 10 seconds.
		setTimeout(reqTimeout());
		QTime timer;
//		timer.start();
		int ichr;
//		do
//		{
		ichr=getch();
		setTimeout(byteTimeout());
//		}while(ichr==-1 && timer.elapsed()<10000);
//		port()->setTimeout(0,byteTimeout());
		if (ichr==-1) // There's no response, exiting
		{
			setErrorText(tr("Timeout while waiting confirmation byte.\nCheck FR's connectivity, please.\nFR could be powered off, as well."));
			setOptionsRead(false); //Device is offline. We should read device setting next time.
			return RSP_TIMEOUT;
		}
		if (((Byte)ichr)==ACK)
		{ // Device is going to send data.
			if (ignorePendingData)
			{ // Just read and ignore it, then retry.
				readData(0,0,0);
				continue;
			}
			else
			{ // Notify caller
				setErrorText(tr("Device is going to send data."));
				return RSP_DATAPENDING;
			}
		} /* End of ACK byte processing*/
		else if (((Byte)ichr)!=NAK)
		{ // Device is sending some data, just ignore it.
			int szBytesWaiting;
			while ((szBytesWaiting=port()->bytesWaiting())!=0)
			{
				if (szBytesWaiting==-1)
				{ // Unfortunate occurence - we cannot read size of input queue.
					break; // let's retry
				}
				getch();
			}
			if (szBytesWaiting==-1) continue;
			// Input queue is cleared, retrying.
			continue;
		} /* End of non-matching byte processing */
		// Ok. We can finally send data to device.
		result=sendData(pCommand,szCmdSize,pszWrittenBytes);
		if (result!=RSP_OK)
		{
//			printf("Send data failed.\n");
			continue; // sendData failed, so we should repeat entire IO sequence, or fail if retry count is exceeded.
		}
		result=readData(pResponce,szRespBufSize,pszReadBytes);
//		printf("readData result %d\n",result);
		if (result==RSP_OK)
		{
			setErrorText(tr("No error"));
			return result;
		}
		else if (result==RSP_BUFFEROVERRUN)
			return result;
		else if (result==RSP_COMMANDERROR)
			return result;
		else if (result==RSP_RESPONCETOOSHORT)
			return result;
		// Otherwise we make another attempt.
	} /* End of retry cycle*/
	if (result==RSP_TIMEOUT)
	{
		setOptionsRead(false); //Device is offline. We should read device setting next time.
		setErrorText(tr("All retries failed. Device link is down."));
	}
	return result;
}

ShtrihFR::Result ShtrihFR::sendData(Byte const * pCommand, int szCmdSize, int * pszBytesWritten)
{
//	port()->setTimeout(0,byteTimeout());
	for(int j=0;j<retryCount();j++)
	{
		Byte bCheckSum=(Byte)szCmdSize; // Lenght of a command is included in checksum.
		// There's no checking of successfullness of sending each byte, according to protocol diagram.
		putch(STX);
		putch((Byte)szCmdSize);
		for(int i=0;i<szCmdSize;i++)
		{
			putch(pCommand[i]);
			bCheckSum=bCheckSum^pCommand[i];
		}
		putch(bCheckSum);
//		port()->setTimeout(0,respTimeout());
		int iResp=getch();
//		port()->setTimeout(0,byteTimeout());
		if (((Byte)iResp)!=ACK)
			continue; // error occured, retrying.
		if (pszBytesWritten)
			*pszBytesWritten=szCmdSize;
		return RSP_OK;
	}
	if (pszBytesWritten)
		*pszBytesWritten=0;
	return RSP_TIMEOUT;
}

ShtrihFR::Result ShtrihFR::readData(Byte * pResponce, int szRespBufSize, int *pszReadBytes)
{
	if (pszReadBytes)
		*pszReadBytes=0;
//	port()->setTimeout(0,respTimeout());
	setTimeout(respTimeout());
	if (((Byte)getch())!=STX)
	{
		putch(NAK);
		setTimeout(byteTimeout());
		setErrorText(tr("readData: Timeout while waiting for STX."));
		return RSP_TIMEOUT;
	}
	setTimeout(byteTimeout());
	int iszLen=getch();
	if (iszLen==-1)
	{
		putch(NAK);
		setErrorText(tr("readData: Timeout while waiting for length byte."));
		return RSP_TIMEOUT;
	}
	iszLen=(Byte)iszLen;
	Byte checkSum=(Byte)iszLen;
	Byte errorCode=0;
	for(int i=0;i<iszLen;i++)
	{
		int iByte=getch();
		if (iByte==-1)
		{
			putch(NAK);
			if (pszReadBytes)
				*pszReadBytes=i;
			setErrorText(tr("readData: Timeout while waiting for message byte."));
			return RSP_TIMEOUT;
		}
		if (pResponce && i<szRespBufSize)
			pResponce[i]=(Byte)iByte;
		if (i==1)
			errorCode=(Byte)iByte;
		checkSum=checkSum^(Byte)iByte;
	}
	if (pszReadBytes)
		*pszReadBytes=iszLen;
	int iByte=getch();
	if (iByte==-1)
	{
		putch(NAK);
		setErrorText(tr("readData: Timeout while waiting for checksum byte."));
		return RSP_TIMEOUT;
	}
	// !!! Insert msleep(respTimeout()) here !!!
	if (((Byte)iByte)!=checkSum)
	{
		putch(NAK);
		setErrorText(tr(QString("readData: Checksum error. 0x%1 is expected, 0x%2 is received.")
			.arg((int)checkSum,0,16).arg(iByte&0xff,0,16)));
		return RSP_CHECKSUMERROR;
	}
	putch(ACK);
	if (iszLen>=2)
	{	// Checking command error code
		if (errorCode!=0)
		{
			setErrorText(tr("Device returns error code 0x%1. %2").arg((int)errorCode,0,16).arg(devErrorText(errorCode)));
			setDeviceError(errorCode);
			return RSP_COMMANDERROR;
		}
	}
	else
	{
		setErrorText(tr("Too short responce - less then 2 bytes."));
		return RSP_RESPONCETOOSHORT;
	} /* End too short responce processing */
	if (iszLen>szRespBufSize && pResponce!=NULL)
	{ // We report buffer overruns only if caller provides buffer space.
		setErrorText(tr("Insufficient buffer size. Device responce was truncated."));
		return RSP_BUFFEROVERRUN;
	}
	return RSP_OK;
}

ShtrihFR::Result ShtrihFR::internalAddOperation(int oOperation, Byte const * baOperpass /* Byte [4] */,
		Decimal dQuantity, Decimal dPrice, Byte Section, Byte const * baTax /* Byte [4] */,
		const QString & sText, Byte & OpNum)
{
	struct
	{ // Check the Standard to ensure that this struct is packed !!!
		Byte cmd;
		Byte opPwd[4];
        Byte qty[5];
		Byte price[5];
		Byte sect;
		Byte tax[4];
		Byte text[40];
	} lCmd;
// Sanity checks.
	if (baOperpass==NULL)
	{
		setErrorText(tr("NULL pointer was passed for operator's password"));
		return RSP_INVALIDPARAMETER2;
	}
	// NULL for baTax mean no taxes
	//else if (baTax==NULL)
	//{
	//	setErrorText(tr("NULL pointer was passed for taxes' array."));
	//	return RSP_INVALIDPARAMETER6;
	//}
	lCmd.cmd=0;
	switch(oOperation)
	{
	case FOP_BUY:
		lCmd.cmd=0x81;
		break;
	case FOP_BUYRET:
		lCmd.cmd=0x83;
		break;
	case FOP_SELL:
		lCmd.cmd=0x80;
		break;
	case FOP_SELLRET:
		lCmd.cmd=0x82;
		break;
	case FOP_STORNO:
		lCmd.cmd=0x84;
		break;
	default:
		setErrorText(tr("Invalid operation number %1").arg(oOperation));
		return RSP_INVALIDPARAMETER1;
	}
	memcpy((void*)lCmd.opPwd,(void*)baOperpass,sizeof(lCmd.opPwd));
	Result res;
	if ((res=fromDecimal(dQuantity,lCmd.qty,sizeof(lCmd.qty),DTQuantity))!=RSP_OK) // There can be overflow or something.
		return RSP_INTERNALERROR;
	if ((res=fromDecimal(dPrice,lCmd.price,sizeof(lCmd.price),DTPrice))!=RSP_OK)
		return RSP_INTERNALERROR;
	lCmd.sect=Section;
	if (baTax)
		memcpy((void*)lCmd.tax,(void*)baTax,sizeof(lCmd.tax));
	else
		memset((void*)lCmd.tax,0,sizeof(lCmd.tax));
	memset((void*)lCmd.text,0,sizeof(lCmd.text));
	QCString scText=utf8ToDevice(sText);
	memcpy((void*)lCmd.text,(const void*)((const char *)scText),min(sizeof(lCmd.text)-4,scText.length()));
	Byte respbuf[4];
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),respbuf,sizeof(respbuf));
	if (res==RSP_OK)
	{
		OpNum=respbuf[2];
	}
	else if (res==RSP_BUFFEROVERRUN)
	{
		op_warning(tr("internalAddOperation: Unexpected buffer overrun."));
		res=RSP_OK;
	}
	return res;
}

ShtrihFR::Result ShtrihFR::fromDecimal(Decimal dVal,Byte * pBuf,int szSize, DecType type)
{ // Sanity checking
	if (dVal<0)
	{
		setErrorText(tr("fromDecimal: Conversion of negative values isn't supported."));
		return RSP_INVALIDPARAMETER1;
	}
	if (pBuf==0)
	{
		setErrorText(tr("fromDecimal: pBuf is NULL."));
		return RSP_INVALIDPARAMETER2;
	}
	// DTInteger type doesn't require device options to convert to.
	if (DTInteger!=type && !optionsRead())
	{
		setErrorText(tr("fromDecimal: Cannot convert number into device representation. Device settings weren't read."));
		return RSP_NODEVICEINFO;
	}
	Q_ULLONG ulVal=0;
	switch(type)
	{
	case DTPrice:
		ulVal=(Q_ULLONG)round(dVal*priceMultiplier());
		break;
	case DTQuantity:
		ulVal=(Q_ULLONG)round(dVal*quantityMultiplier());
		break;
	case DTPercent:
		ulVal=(Q_ULLONG)round(dVal*percentMultiplier());
		break;
	case DTInteger:
		ulVal=(Q_ULLONG)round(dVal);
		break;
	default:
		setErrorText(tr("fromDecimal: Invalid decimal type parameter value."));
		return RSP_INVALIDPARAMETER4;
	}
	for(int i=0;i<szSize;i++)
	{
		pBuf[i]=(Byte)(ulVal & 0xFF);
		ulVal>>=8;
	}
	if (ulVal!=0)
	{
		setErrorText(tr("fromDecimal: Overflow. Supplied buffer size is too small."));
		return RSP_INVALIDPARAMETER3;
	}
	return RSP_OK;
}

ShtrihFR::Result ShtrihFR::toDecimal(Byte const * const pBuf,int szSize, Decimal * pdVal, DecType type)
{ // Sanity checking
	if (pdVal==0)
	{
		setErrorText(tr("toDecimal: pdVal==0"));
		return RSP_INVALIDPARAMETER3;
	}
	if (pBuf==0)
	{
		setErrorText(tr("toDecimal: pBuf is NULL."));
		return RSP_INVALIDPARAMETER1;
	}
	if (szSize<=0)
	{
		setErrorText(tr("toDecimal: szSize should be greater than zero"));
		return RSP_INVALIDPARAMETER1;
	}
	if (DTInteger!=type && !optionsRead())
	{
		setErrorText(tr("toDecimal: Cannot convert number from device representation. Device settings weren't read."));
		return RSP_NODEVICEINFO;
	}
	Q_ULLONG ulVal=0;
	Q_ULLONG ulDivisor;
	switch(type)
	{
	case DTPrice:
		ulDivisor=priceMultiplier();
		break;
	case DTQuantity:
		ulDivisor=quantityMultiplier();
		break;
	case DTPercent:
		ulDivisor=percentMultiplier();
		break;
	case DTInteger:
		ulDivisor=1;
		break;
	default:
		setErrorText(tr("toDecimal: Invalid decimal type parameter value."));
		return RSP_INVALIDPARAMETER4;
	}
	for(int i=szSize-1;i>=0;--i)
	{
		ulVal<<=8;
		ulVal|=(Q_ULLONG)pBuf[i];
	}
	if (ulDivisor==0)
	{
		setErrorText(tr("toDecimal: ulDivisor==0"));
		return RSP_INTERNALERROR;
	}
	*pdVal=ulVal/(Decimal)ulDivisor;
	return RSP_OK;
}

QCString ShtrihFR::utf8ToDevice( const QString &text )
{
	QTextCodec * cp1251=QTextCodec::codecForName("CP1251");
	return cp1251->fromUnicode(text);
}

QString ShtrihFR::deviceToUtf8( const QCString &text )
{
	QTextCodec * cp1251=QTextCodec::codecForName("CP1251");
	return cp1251->toUnicode(text);
}

ShtrihFR::Result ShtrihFR::internalReadOptions(int /*reserved*/)
{
	Result res;
	Byte cmd[1]={0xfc}; // Команда запроса типа устройства
	union{
		struct{
			Byte cmd;
			Byte errorByte;
			Byte deviceType;
			Byte deviceSubtype;
			Byte protocolVersionMajor;
			Byte protocolVersionMinor;
			Byte deviceModel;
			Byte deviceLanguage;
			Byte deviceName[1];
		} la;
		Byte ba[256];
	} resp;
	int readBytes;
	// Clear all values
	setValue("Model",QVariant());
	setValue("SoftVersion",QVariant());
	setValue("SoftBuild",QVariant());
	setValue("SoftBuildDate",QVariant());
	setValue("CurrentDocumentNumber",QVariant());
	setValue("DateTime",QVariant());
	setValue("Mode",QVariant());
	setValue("Submode",QVariant());
	setValue("FiscalizationsCount",QVariant());
	setValue("FiscalizationsRemaining",QVariant());
	setValue("FRFlags",QVariant());
	setValue("FMFlags",QVariant());
	setValue("SerialNumber",QVariant());
	setValue("LastClosedChangeNumber",QVariant());
	setValue("FreeFMRecordCount",QVariant());
	setValue("INN",QVariant());

	res=sendCommand(cmd,sizeof(cmd),resp.ba,sizeof(resp),0,&readBytes);
	if (res==RSP_OK)
	{
		int szModLen=readBytes-8; // !!! Magic number !!!
		if (szModLen<=0)
			setValue("Model","Unknown");
		else
		{
			QCString csModStr((char*)(resp.la.deviceName),szModLen+1); // +1 for null byte
			setValue("Model",deviceToUtf8(csModStr));
		}
		if (resp.la.deviceType==0 && resp.la.deviceSubtype==0)
		{ // It's a fiscal register.
			FRState frs;
			if (internalReadState(&frs)==RSP_OK)
			{
				ushort frflags=frs.frFlags[0]|(frs.frFlags[1]<<8);
				setPriceMultiplier((frflags&FR_TwoDigitsAfterDot)?100:1);
				setValue("SoftVersion",QString("%1.%2").arg((int)frs.fmSoftwareVersion[0]).arg((int)frs.fmSoftwareVersion[1]));
				setValue("SoftBuild",(uint)(frs.fmBuildNumber[0]+(uint)frs.fmBuildNumber[1]*256));
				setValue("SoftBuildDate",QDateTime(QDate(frs.frBuildDate[2]+2000,frs.frBuildDate[1],frs.frBuildDate[0])));
				setValue("CurrentDocumentNumber",(uint)(frs.currentDocumentNumber[0]+(uint)frs.currentDocumentNumber[1]*256));
				setValue("DateTime",QDateTime(QDate(frs.date[2]+2000,frs.date[1],frs.date[0]),QTime(frs.time[0],frs.time[1],frs.time[2])));
				setValue("Mode",(uint)frs.frMode);
				setValue("Submode",(uint)frs.frSubmode);
				setValue("FiscalizationsCount",(uint)frs.fiscalizationsCount);
				setValue("FiscalizationsRemaining",(uint)frs.fiscalizationsRemaining);
				setValue("FRFlags",(uint)(frs.frFlags[0]+(uint)frs.frFlags[1]*256));
				setValue("FMFlags",(uint)(frs.fmFlags));
				setValue("SerialNumber",((uint)(frs.serialNumber[0]))+(((uint)(frs.serialNumber[1]))<<8)+
					(((uint)(frs.serialNumber[2]))<<16)+(((uint)(frs.serialNumber[3]))<<24));
				setValue("LastClosedChangeNumber",(uint)(frs.lastClosedChangeNumber[0]+(uint)frs.lastClosedChangeNumber[1]*256));
				setValue("FreeFMRecordCount",(uint)(frs.freeFMRecordsCount[0]+(uint)frs.freeFMRecordsCount[1]*256));
				char innbuf[13];
				sprintf(innbuf,"%02x%02x%02x%02x%02x%02x",frs.INN[0],frs.INN[1],frs.INN[2],frs.INN[3],frs.INN[4],frs.INN[5]);
				setValue("INN",QString(innbuf));
				setOptionsRead(true);
				return RSP_OK;
			}
//			else
//			    setErrorText(tr(""));

		}
	}
	else if (res==RSP_COMMANDERROR)
	{ // Device doesn't support this command. Let's try another variant.
		setValue("Model","Unknown");
	}
	setOptionsRead(false);
	return RSP_NODEVICEINFO;
}

// Operator password in m_baOperPass should be set.
ShtrihFR::Result ShtrihFR::internalReadState(FRState * pState)
{
	Byte cmd[5]={0x11,m_baOperPass[0],m_baOperPass[1],m_baOperPass[2],m_baOperPass[3]};
	return sendCommand(cmd,sizeof(cmd),(Byte*)pState,sizeof(*pState));
}

ShtrihFR::Result ShtrihFR::internalReadShortState(FRShortState * pState)
{
	Byte cmd[5]={0x10,m_baOperPass[0],m_baOperPass[1],m_baOperPass[2],m_baOperPass[3]};
	return sendCommand(cmd,sizeof(cmd),(Byte*)pState,sizeof(*pState));
}

ShtrihFR::Result ShtrihFR::setPassword(int pt,const QString & pass)
{
	bool ok;
	ulong ulPass=pass.toULong(&ok);
	if (!ok)
	{
		setErrorText(tr("Invalid password format. Enter digits only, please."));
		return CRERROR_INVALIDCALL;
	}
	Byte * pPass;
	switch(pt)
	{
	case PTOperator:
		pPass=m_baOperPass;
		break;
	case PTAdmin:
		pPass=m_baAdminPass;
		break;
	case PTFiscal:
		pPass=m_baFiscalPass;
		break;
	default:
		setErrorText(tr("Invalid password type."));
		return CRERROR_INVALIDCALL;
	}

	for(int i=0;i<4;i++)
	{
		pPass[i]=(Byte)(ulPass & 0xff);
		ulPass>>=8;
	}
	return CRERROR_OK;
}

ShtrihFR::Result ShtrihFR::internalSetCommConfig(int portNumber, int baudRate, int timeOut)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte portNumber;
		Byte baudRateCode;
		Byte timeOut;
	} lCmd;
	// sanity checks
	if (portNumber<0 || portNumber>255)
	{
		setErrorText(tr("Invalid port number."));
		return RSP_INVALIDPARAMETER1;
	}
	if (timeOut<=0 || timeOut>256)
	{
		setErrorText(tr("Invalid port number."));
		return RSP_INVALIDPARAMETER3;
	}
	lCmd.cmd=0x14;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(m_baAdminPass));
	lCmd.portNumber=(Byte)portNumber;
	lCmd.timeOut=(timeOut==256)?0:timeOut;
	switch(baudRate)
	{
	case 2400:
		lCmd.baudRateCode=0;
		break;
	case 4800:
		lCmd.baudRateCode=1;
		break;
	case 9600:
		lCmd.baudRateCode=2;
		break;
	case 19200:
		lCmd.baudRateCode=3;
		break;
	case 38400:
		lCmd.baudRateCode=4;
		break;
	case 57600:
		lCmd.baudRateCode=5;
		break;
	case 115200:
		lCmd.baudRateCode=6;
		break;
	default:
		setErrorText(tr("Unsupported baud rate %1").arg(baudRate));
		return RSP_INVALIDPARAMETER2;
	}
	Result ret=sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
	return ret;
}

ShtrihFR::Result ShtrihFR::internalReadCommConfig(int portNumber, int *pBaudRate, int *pTimeOut)
{
	Result ret=0;
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte portNumber;
	} lCmd;
	if (portNumber<0 || portNumber>255)
	{
		setErrorText(tr("Invalid port number."));
		return RSP_INVALIDPARAMETER1;
	}
	lCmd.cmd=0x15;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(m_baAdminPass));
	lCmd.portNumber=portNumber;
	struct
	{
		Byte cmd;
		Byte err;
		Byte baudRateCode;
		Byte timeOut;
	} lResp;
	int baudratetbl[7]={2400,4800,9600,19200,38400,57600,115200};
	ret=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (ret==RSP_OK)
	{
		if (pBaudRate)
			*pBaudRate=(lResp.baudRateCode>6)?lResp.baudRateCode:baudratetbl[lResp.baudRateCode];
		if (pTimeOut)
			*pTimeOut=lResp.timeOut==0?256:lResp.timeOut;
	}
	return ret;
}

ShtrihFR::Result ShtrihFR::internalQueryTableInfo(int tableNum, QString & name, int *pRowCount, int *pColumnCount)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte tableNum;
	}lCmd;
	lCmd.cmd=0x2d;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(m_baAdminPass));
	lCmd.tableNum=(Byte)tableNum;
	struct
	{
		Byte cmd;
		Byte err;
		Byte name[40];
		Byte rowCount[2];
		Byte columnCount;
	}lResp;
	Result res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		name=deviceToUtf8(QCString((char*)lResp.name,sizeof(lResp.name)+1));
		if (pRowCount)
			*pRowCount=lResp.rowCount[0]+256*(int)lResp.rowCount[1];
		if (pColumnCount)
			*pColumnCount=lResp.columnCount;
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalQueryColumnInfo(int tableNum, int columnNum, QString & name, int *pType, int *pSize, void *pMin, void *pMax)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte tableNum;
		Byte columnNum;
	}lCmd;
	lCmd.cmd=0x2e;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(m_baAdminPass));
	lCmd.tableNum=(Byte)tableNum;
	lCmd.columnNum=(Byte)columnNum;
	struct
	{
		Byte cmd;
		Byte err;
		Byte name[40];
		Byte type;
		Byte size;
		Byte minmax[256];
	}lResp;
	int br;
	Result res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp),0,&br);
	if (res==RSP_OK)
	{
		name=deviceToUtf8(QCString((char*)lResp.name,sizeof(lResp.name)+1));
		if (pType)
			*pType=lResp.type;
		if (pSize)
			*pSize=lResp.size;
		if (lResp.size>128) lResp.size=128;
		if (pMin)
			memcpy(pMin,(void*)(lResp.minmax),lResp.size);
		if (pMax)
			memcpy(pMax,(void*)(lResp.minmax+lResp.size),lResp.size);
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalReadField(int tableNum, int rowNum, int columnNum, void *pValue, int *pSize)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte tableNum;
		Byte rowNum[2];
		Byte columnNum;
	}lCmd;
	lCmd.cmd=0x1f;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(m_baAdminPass));
	lCmd.tableNum=(Byte)tableNum;
	lCmd.rowNum[0]=(Byte)(rowNum&0xff);
	lCmd.rowNum[1]=(Byte)((rowNum>>8)&0xff);
	lCmd.columnNum=(Byte)columnNum;
	struct
	{
		Byte cmd;
		Byte err;
		Byte value[40];
	}lResp;
	int br;
	int maxsize=pSize?(*pSize):40;
	Result res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp),0,&br);
	if (res==RSP_OK)
	{
		maxsize=(br-2>maxsize)?maxsize:br-2;
		if (pValue)
			memcpy(pValue,(void*)(lResp.value),maxsize);
		if (pSize)
			*pSize=maxsize;
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalWriteField(int tableNum, int rowNum, int columnNum, void *pValue, int size)
{
	if (pValue==0)
	{
		setErrorText(tr("internalWriteField: pValue==NULL"));
		return RSP_INVALIDPARAMETER4;
	}
	if (size>40)
	{
		setErrorText(tr("internalWriteField: value size greater than 40"));
		return RSP_INVALIDPARAMETER5;
	}
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte tableNum;
		Byte rowNum[2];
		Byte columnNum;
		Byte value[40];
	}lCmd;
	lCmd.cmd=0x1e;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(m_baAdminPass));
	lCmd.tableNum=(Byte)tableNum;
	lCmd.rowNum[0]=(Byte)(rowNum&0xff);
	lCmd.rowNum[1]=(Byte)((rowNum>>8)&0xff);
	lCmd.columnNum=(Byte)columnNum;
	memcpy((void*)lCmd.value,pValue,size);
	struct
	{
		Byte cmd;
		Byte err;
	}lResp;
	int br;
	Result res=sendCommand((Byte*)&lCmd,sizeof(lCmd)-sizeof(lCmd.value)+size,(Byte*)&lResp,sizeof(lResp),0,&br);
	return res;
}

ShtrihFR::Result ShtrihFR::internalCloseCheck(Decimal dCashsum, Decimal dSumPT2, Decimal dSumPT3, Decimal dSumPT4,
	Decimal dDiscount, Byte const * baTax /* Byte [4] */,
	const QString sText, Decimal * pdChangeSum, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
    Byte cashSum[5];
		Byte type2Sum[5];
		Byte type3Sum[5];
		Byte type4Sum[5];
		Byte rebate[2];
		Byte tax[4];
		Byte text[40];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
		Byte changeSum[5];
	}lResp;
	//sanity checking
	//if (baTax==0)
	//{
	//	setErrorText(tr("internalCloseCheck: baTax==0"));
	//	return RSP_INVALIDPARAMETER6;
	//}
	lCmd.cmd=0x85;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(m_baOperPass));
	Result res;
	res=fromDecimal(dCashsum,lCmd.cashSum,sizeof(lCmd.cashSum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dCashsum into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=fromDecimal(dSumPT2,lCmd.type2Sum,sizeof(lCmd.type2Sum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dSumPT2 into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=fromDecimal(dSumPT3,lCmd.type3Sum,sizeof(lCmd.type3Sum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dSumPT3 into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=fromDecimal(dSumPT4,lCmd.type4Sum,sizeof(lCmd.type4Sum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dSumPT4 into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=fromDecimal(dDiscount,lCmd.rebate,sizeof(lCmd.rebate),DTPercent);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dDiscount into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	if (baTax)
		memcpy((void*)lCmd.tax,(void*)baTax,sizeof(lCmd.tax));
	else
		memset((void*)lCmd.tax,0,sizeof(lCmd.tax));
	res=stringToByteArray(sText,lCmd.text,sizeof(lCmd.text));
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting sText into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res!=RSP_OK)
	{
		return res;
	}
	if (pdChangeSum)
		res=toDecimal(lResp.changeSum,sizeof(lResp.changeSum),pdChangeSum,DTPrice);
	OpNum=lResp.operNum;
	return res;
}

ShtrihFR::Result ShtrihFR::internalAddDiscount(Decimal dSum, Byte const * baTax /* Byte [4] */,
	const QString & sText, Byte & OpNum)
{
	return internalCommandSumTaxText(0x86,dSum,baTax,sText,OpNum);
}

ShtrihFR::Result ShtrihFR::internalAddExtra(Decimal dSum, Byte const * baTax /* Byte [4] */,
	const QString & sText, Byte & OpNum)
{
	return internalCommandSumTaxText(0x87,dSum,baTax,sText,OpNum);
}

ShtrihFR::Result ShtrihFR::internalClearCheck(Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte opNum;
	}lResp;
	lCmd.cmd=0x88;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	Result res;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
		OpNum=lResp.opNum;
	return res;
}

ShtrihFR::Result ShtrihFR::internalCheckSubtotal(Decimal * pdSubtotal, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte opNum;
		Byte subtotal[5];
	}lResp;
	lCmd.cmd=0x89;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	Result res;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		OpNum=lResp.opNum;
		if (pdSubtotal)
			res=toDecimal((Byte*)&lResp.subtotal,sizeof(lResp.subtotal),pdSubtotal,DTPrice);
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalPayingIn(Decimal dSum, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
        Byte sum[5];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte opNum;
		Byte docnum[2];
	}lResp;
	Result res;
	lCmd.cmd=0x50;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	res=fromDecimal(dSum,lCmd.sum,sizeof(lCmd.sum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dSum into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		OpNum=lResp.opNum;
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalPayment(Decimal dSum, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
        Byte sum[5];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte opNum;
		Byte docnum[2];
	}lResp;
	Result res;
	lCmd.cmd=0x51;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	res=fromDecimal(dSum,lCmd.sum,sizeof(lCmd.sum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dSum into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		OpNum=lResp.opNum;
	}
	return res;
}


ShtrihFR::Result ShtrihFR::internalCommandSumTaxText(Byte cmd,Decimal dSum,  Byte const * baTax /* Byte [4] */,
		const QString & sText, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte sum[5];
		Byte tax[4];
		Byte text[40];
	}lCmd;
	lCmd.cmd=cmd;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(m_baOperPass));
	Result res;
	res=fromDecimal(dSum,lCmd.sum,sizeof(lCmd.sum),DTPrice);
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting dSum into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	memcpy((void*)lCmd.tax,baTax,sizeof(lCmd.tax));
	res=stringToByteArray(sText,lCmd.text,sizeof(lCmd.text));
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting sText into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0,0,0,OpNum);
}

ShtrihFR::Result ShtrihFR::internalAddDiscountStorno(Decimal dSum,  Byte const * baTax /* Byte [4] */,
		const QString & sText, Byte & OpNum)
{
	return internalCommandSumTaxText(0x8A,dSum,baTax,sText,OpNum);
}

ShtrihFR::Result ShtrihFR::internalAddExtraStorno(Decimal dSum,  Byte const * baTax /* Byte [4] */,
		const QString & sText, Byte & OpNum)
{
	return internalCommandSumTaxText(0x8B,dSum,baTax,sText,OpNum);
}

ShtrihFR::Result ShtrihFR::internalRing()
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
	}lCmd;
	lCmd.cmd=0x13;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalRepeatDocument(Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
	}lResp;
	lCmd.cmd=0x8c;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	Result res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK) OpNum=lResp.operNum;
	return res;
}

ShtrihFR::Result ShtrihFR::internalOpenCheck(int iFDT, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte operation;
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
	}lResp;
	lCmd.cmd=0x8d;
	lCmd.operation=iFDT;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	Result res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK) OpNum=lResp.operNum;
	return res;
}

ShtrihFR::Result ShtrihFR::internalPrintLine(const QString & line, bool bOnCheckTape, bool bOnJournalTape, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte flags;
		Byte line[40];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
	}lResp;
	Result res;
	lCmd.cmd=0x17;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.flags=(bOnCheckTape?2:0)|(bOnJournalTape?1:0);
	res=stringToByteArray(line,lCmd.line,sizeof(lCmd.line));
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting 'line' into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK) OpNum=lResp.operNum;
	return res;
}

ShtrihFR::Result ShtrihFR::internalPrintBoldLine(const QString & line, bool bOnCheckTape, bool bOnJournalTape, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte flags;
		Byte line[20];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
	}lResp;
	Result res;
	lCmd.cmd=0x12;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.flags=(bOnCheckTape?2:0)|(bOnJournalTape?1:0);
	res=stringToByteArray(line,lCmd.line,sizeof(lCmd.line));
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting 'line' into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK) OpNum=lResp.operNum;
	return res;
}

ShtrihFR::Result ShtrihFR::internalPrintHeader(const QString & header, int iDocNumber, int *piSeqDocNumber, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte header[30];
		Byte docNum[2];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
		Byte seqNum[2];
	}lResp;
// Sanity checks
	Result res;
	if (iDocNumber<=0 || iDocNumber>=65536)
	{
		setErrorText(tr("Document number is out of range (1..65535)."));
		return RSP_INVALIDPARAMETER2;
	}
	lCmd.cmd=0x18;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	res=stringToByteArray(header,lCmd.header,sizeof(lCmd.header));
	if (res!=RSP_OK)
	{
		QString error=errorText();
		setErrorText(tr("Error while converting 'line' into device format. %1").arg(error));
		return RSP_INTERNALERROR;
	}
	lCmd.docNum[0]=(Byte)(iDocNumber & 0xff);
	lCmd.docNum[1]=(Byte)((iDocNumber>>8) & 0xff);
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		if (piSeqDocNumber!=0)
		{
			*piSeqDocNumber=lResp.seqNum[0]|(((int)lResp.seqNum[1])<<8);
			OpNum=lResp.operNum;
		}
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalGetMoneyAccumValue(int accumNum, Decimal *pdValue, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte accumNum;
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
		Byte value[6];
	}lResp;
// Sanity checks
	Result res;
	if (accumNum<0 || accumNum>255)
	{
		setErrorText(tr("Register number is out of range (0..255)."));
		return RSP_INVALIDPARAMETER1;
	}
	lCmd.cmd=0x1a;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.accumNum=accumNum;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		if (pdValue!=0)
		{
			res=toDecimal(lResp.value,sizeof(lResp.value),pdValue,DTPrice);
			if (res!=RSP_OK)
			{
				QString err=errorText();
				setErrorText(tr("Error while converting 'value' to double. %1").arg(err));
				return RSP_INTERNALERROR;
			}
		}
		OpNum=lResp.operNum;
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalGetOperRegisterValue(int regNum, int *piValue, Byte & OpNum)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte regNum;
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte operNum;
		Byte value[2];
	}lResp;
// Sanity checks
	Result res;
	if (regNum<0 || regNum>255)
	{
		setErrorText(tr("Register number is out of range (0..255)."));
		return RSP_INVALIDPARAMETER1;
	}
	lCmd.cmd=0x1b;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.regNum=regNum;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		if (piValue!=0)
		{
			*piValue=lResp.value[0]|(((int)lResp.value[1])<<8);
		}
		OpNum=lResp.operNum;
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalWriteLicense(const Byte * pLic /* Byte [5] */)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte license[5];
	}lCmd;
	// sanity checking
	if (pLic==0)
	{
		setErrorText(tr("Pointer to license shouldn't be NULL."));
		return RSP_INVALIDPARAMETER1;
	}
	lCmd.cmd=0x1c;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	memcpy((void*)lCmd.license,(void*)pLic,sizeof(lCmd.license));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalReadLicense(Byte * pLic /* Byte [5] */)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte license[5];
	}lResp;
	lCmd.cmd=0x1d;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	Result res;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		if (pLic!=0)
		{
			memcpy((void*)pLic,(void*)&lResp.license,sizeof(lResp.license));
		}
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalSetTime(int hour, int min, int sec)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte time[3]; // hour,min,sec
	}lCmd;
// Sanity check
	if (hour<0 || hour>23)
	{
		setErrorText(tr("Hour value (%1) is out of range (0..23)").arg(hour));
		return RSP_INVALIDPARAMETER1;
	}
	if (min<0 || min>59)
	{
		setErrorText(tr("Minute value (%1) is out of range (0..59)").arg(min));
		return RSP_INVALIDPARAMETER2;
	}
	if (sec<0 || sec>59)
	{
		setErrorText(tr("Second's value (%1) is out of range (0..59)").arg(sec));
		return RSP_INVALIDPARAMETER3;
	}
	lCmd.cmd=0x21;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	lCmd.time[0]=(Byte)hour;
	lCmd.time[1]=(Byte)min;
	lCmd.time[2]=(Byte)sec;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalSetDate(int day, int month, int year)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte date[3]; // day, month, year(0..255)
	}lCmd;
	if (year<0 || (year>255 && year<2000) || year>2255)
	{
		setErrorText(tr("Year's value (%1) is out of range 2000..2255 or 0..255.").arg(year));
		return RSP_INVALIDPARAMETER3;
	}
	if (!QDate::isValid((year<=255 && year>=0)?year+2000:year,month,day))
	{
		setErrorText(tr("Invalid date %1-%2-%3.").arg(day).arg(month).arg(((year<=255)&&(year>=0))?year+2000:year));
		return RSP_INVALIDPARAMETERS;
	}
	year=(year>=2000 && year<=2255)?year-2000:year;
	lCmd.cmd=0x22;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	lCmd.date[0]=(Byte)day;
	lCmd.date[1]=(Byte)month;
	lCmd.date[2]=(Byte)year;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalCommitDate(int day, int month, int year)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte date[3]; // day, month, year(0..255)
	}lCmd;
	if (year<0 || (year>255 && year<2000) || year>2255)
	{
		setErrorText(tr("Year's value (%1) is out of range 2000..2255 or 0..255.").arg(year));
		return RSP_INVALIDPARAMETER3;
	}
	if (!QDate::isValid((year<=255 && year>=0)?year+2000:year,month,day))
	{
		setErrorText(tr("Invalid date %1-%2-%3.").arg(day).arg(month).arg(((year<=255)&&(year>=0))?year+2000:year));
		return RSP_INVALIDPARAMETERS;
	}
	year=(year>=2000 && year<=2255)?year-2000:year;
	lCmd.cmd=0x23;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	lCmd.date[0]=(Byte)day;
	lCmd.date[1]=(Byte)month;
	lCmd.date[2]=(Byte)year;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalBeginDump(int devNum, int * pBlockCount)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte devNum;
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte blockCount[2];
	}lResp;
// Sanity check
	if (devNum<0 || devNum>255)
	{
		setErrorText(tr("devNum value (%1) is out of range 0..255.").arg(devNum));
		return RSP_INVALIDPARAMETER1;
	}
	lCmd.cmd=0x01;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	lCmd.devNum=(Byte)devNum;
	Result res;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		if (pBlockCount!=0)
			*pBlockCount=lResp.blockCount[0]|(((int)lResp.blockCount[1])<<8);
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalGetData(int *pDevNum, int *pBlockNum, Byte * pData, int szBufSize)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	struct
	{
		Byte cmd;
		Byte err;
		Byte devNum;
		Byte blockNum[2];
		Byte data[32];
	}lResp;
	lCmd.cmd=0x02;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	Result res;
	res=sendCommand((Byte*)&lCmd,sizeof(lCmd),(Byte*)&lResp,sizeof(lResp));
	if (res==RSP_OK)
	{
		if (pDevNum!=0)
			*pDevNum=lResp.devNum;
		if (pBlockNum!=0)
			*pBlockNum=lResp.blockNum[0]|(((int)lResp.blockNum[1])<<8);
		if (pData!=0)
			memcpy((void*)pData,(void*)lResp.data,(unsigned int)szBufSize<sizeof(lResp.data)?szBufSize:sizeof(lResp.data));
	}
	return res;
}

ShtrihFR::Result ShtrihFR::internalEndDump()
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	lCmd.cmd=0x03;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalDailyReport()
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	lCmd.cmd=0x40;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalDailyReportAndClear()
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	lCmd.cmd=0x41;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalSetDecimalPointPos(int iFlag)
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
		Byte posFlag;
	}lCmd;
	lCmd.cmd=0x20;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	lCmd.posFlag=iFlag;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalResetTables()
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	lCmd.cmd=0x024;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalCutCheck(int iCutType)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte cutType;
	}lCmd;
	lCmd.cmd=0x025;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.cutType=(Byte)iCutType;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalTotalClear()
{
	struct
	{
		Byte cmd;
		Byte admPass[4];
	}lCmd;
	lCmd.cmd=0x027;
	memcpy((void*)lCmd.admPass,(void*)m_baAdminPass,sizeof(lCmd.admPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalOpenCashbox(int iCashboxNumber)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte cashboxNumber;
	}lCmd;
	lCmd.cmd=0x028;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.cashboxNumber=(Byte)iCashboxNumber;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalContinuePrint()
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
	}lCmd;
	lCmd.cmd=0xb0;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

/*! /en	Operator password required. /_en
	/ru Тестовый прогон. /_ru */
ShtrihFR::Result ShtrihFR::internalTestRun(int iPeriodInMin)
{
  // Sanity checks
  if (iPeriodInMin<1 || iPeriodInMin>99)
  {
    setErrorText(tr("internalTestRun: iPeriodInMin should be in 1..99 range."));
    return RSP_INVALIDPARAMETER1;
  }
	struct
	{
		Byte cmd;
		Byte operPass[4];
    Byte period;
	}lCmd;
	lCmd.cmd=0x19;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
  lCmd.period=(Byte)iPeriodInMin;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

/*! /en	Operator password required. /_en
	/ru Остановить тестовый прогон. /_ru */
ShtrihFR::Result ShtrihFR::internalStopTestRun()
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
	}lCmd;
	lCmd.cmd=0x2b;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::internalTapeAdvance(int flags, int iLineCount)
{
	struct
	{
		Byte cmd;
		Byte operPass[4];
		Byte flags;
		Byte lineCount;
	}lCmd;
	lCmd.cmd=0x029;
	memcpy((void*)lCmd.operPass,(void*)m_baOperPass,sizeof(lCmd.operPass));
	lCmd.flags=(Byte)flags;
	lCmd.lineCount=(Byte)iLineCount;
	return sendCommand((Byte*)&lCmd,sizeof(lCmd),0,0);
}

ShtrihFR::Result ShtrihFR::stringToByteArray(const QString & text, Byte * pBuf, int szSize)
{
	if (pBuf==0)
	{
		setErrorText(tr("stringToByteArray: pBuf==0"));
		return RSP_INVALIDPARAMETER2;
	}
	if (szSize<=0)
	{
		setErrorText(tr("stringToByteArray: szSize<=0"));
		return RSP_INVALIDPARAMETER3;
	}
	QCString devStr=utf8ToDevice(text);
	memset((void*)pBuf,0,szSize);
	memcpy((void*)pBuf,(const void*)((const char*)devStr),min((uint)szSize,devStr.length()));
	return RSP_OK;
}

QString ShtrihFR::devErrorText(Byte err)
{
	QString et=tr("Unknown error 0x%1").arg((int)err,0,16);;
	switch(err)
	{
	case 0x0:
		et=trUtf8("Ошибок нет");
		break;
	case 0x1:
		et=trUtf8("Неисправен накопитель ФП1, ФП2 или часы");
		break;
	case 0x2:
		et=trUtf8("Отсутствует ФП1");
		break;
	case 0x3:
		et=trUtf8("Отсутствует ФП2");
		break;
	case 0x4:
		et=trUtf8("Некорректные параметры в команде обращения к ФП");
		break;
	case 0x5:
		et=trUtf8("Нет запрошенных данных");
		break;
	case 0x6:
		et=trUtf8("ФП в режиме вывода данных");
		break;
	case 0x7:
		et=trUtf8("Некорректные параметры в команде для данной реализации ФП");
		break;
	case 0x8:
		et=trUtf8("Команда не поддерживается в данной реализации ФП");
		break;
	case 0x9:
		et=trUtf8("Некорректная длина команды");
		break;
	case 0xa:
	case 0xb:
	case 0xc:
	case 0xd:
	case 0xe:
	case 0xf:
	case 0x10:
		break;
	case 0x11:
		et=trUtf8("Не введена лицензия");
		break;
	case 0x12:
		et=trUtf8("Заводской номер уже введен");
		break;
	case 0x13:
		et=trUtf8("Текущая дата меньше даты последней записи в ФП");
		break;
	case 0x14:
		et=trUtf8("Область сменных итогов ФП переполнена");
		break;
	case 0x15:
		et=trUtf8("Смена уже открыта");
		break;
	case 0x16:
		et=trUtf8("Смена не открыта");
		break;
	case 0x17:
		et=trUtf8("Номер первой смены больше номера последней смены");
		break;
	case 0x18:
		et=trUtf8("Дата первой смены больше даты последней смены");
		break;
	case 0x19:
		et=trUtf8("Нет данных в ФП");
		break;
	case 0x1a:
		et=trUtf8("Область перерегистраций в ФП переполнена");
		break;
	case 0x1b:
		et=trUtf8("Заводской номер не введён");
		break;
	case 0x1c:
		et=trUtf8("В заданном диапазоне есть поврежденная запись");
		break;
	case 0x1d:
		et=trUtf8("Повреждена последняя запись сменных итогов");
		break;
	case 0x1e:
		et=trUtf8("Область перерегистраций в ФП переполнена");
		break;
	case 0x1f:
		break;
	case 0x20:
		et=trUtf8("Переполнение денежного регистра при добавлении");
		break;
	case 0x21:
		et=trUtf8("Вычитаемая сумма больще содержимого денежного регистра");
		break;
	case 0x22:
		et=trUtf8("Неверная дата");
		break;
	case 0x23:
		break;
	case 0x24:
		break;
	case 0x25:
		break;
	case 0x26:
		et=trUtf8("Вносимая клиентом сумма меньше суммы чека");
		break;
	case 0x27:
		break;
	case 0x28:
		break;
	case 0x29:
		break;
	case 0x2a:
		break;
	case 0x2b:
		et=trUtf8("Невозможно отменить предыдущую команду");
		break;
	case 0x2c:
		et=trUtf8("Обнуленная касса (повторное гашение невозможно)");
		break;
	case 0x2d:
		et=trUtf8("Сумма чека по секции меньше суммы сторно.");
		break;
	case 0x2e:
		et=trUtf8("В ФР нет денег для выплаты");
		break;
	case 0x2f:
		break;
	case 0x30:
		et=trUtf8("ФР заблокирован, ждет ввода пароля налогового инспектора");
		break;
	case 0x31:
		break;
	case 0x32:
		et=trUtf8("Требуется выполнение общего гашения");
		break;
	case 0x33:
		et=trUtf8("Некорректный параметр в команде");
		break;
	case 0x34:
		et=trUtf8("Нет данных");
		break;
	case 0x35:
		et=trUtf8("Некорректный параметр при данных настройках");
		break;
	case 0x36:
		et=trUtf8("Некорректные параметры в команде для данной реализации ФР");
		break;
	case 0x37:
		et=trUtf8("Команда не поддерживается в данной реализации ФР");
		break;
	case 0x38:
		et=trUtf8("Ошибка в ПЗУ");
		break;
	case 0x39:
		et=trUtf8("Внутренняя ошибка ПО ФР");
		break;
	case 0x3a:
		et=trUtf8("Переполнение накопления по надбавкам в смене");
		break;
	case 0x3b:
		et=trUtf8("Переполнение накопления в смене");
		break;
	case 0x3c:
		et=trUtf8("Смена открыта - операция невозможна");
		break;
	case 0x3d:
		et=trUtf8("Смена не открыта - операция невозможна");
		break;
	case 0x3e:
		et=trUtf8("Переполнение накопления по секциям в смене");
		break;
	case 0x3f:
		et=trUtf8("Переполнение накопления по скидкам в смене");
		break;
	case 0x40:
		et=trUtf8("Переполнение диапазона скидок");
		break;
	case 0x41:
		et=trUtf8("Переполнение диапазона наличными");
		break;
	case 0x42:
		et=trUtf8("Переполнение диапазона тип 2");
		break;
	case 0x43:
		et=trUtf8("Переполнение диапазона тип 3");
		break;
	case 0x44:
		et=trUtf8("Переполнение диапазона тип 4");
		break;
	case 0x45:
		et=trUtf8("Сумма всех типов оплаты меньше итога чека");
		break;
	case 0x46:
		et=trUtf8("Не хватает наличности в кассе");
		break;
	case 0x47:
		et=trUtf8("Переполнение накопления по налогам в смене");
		break;
	case 0x48:
		et=trUtf8("Переполнение итога чека");
		break;
	case 0x49:
		et=trUtf8("Операция невозможна в открытом чеке данного типа");
		break;
	case 0x4a:
		et=trUtf8("Открыт чек - операция невозможна");
		break;
	case 0x4b:
		et=trUtf8("Буфер чека переполнен");
		break;
	case 0x4c:
		et=trUtf8("Переполнение накопления по обороту налогов в смене");
		break;
	case 0x4d:
		et=trUtf8("Вносимая безналичной оплатой сумма больше суммы чека");
		break;
	case 0x4e:
		et=trUtf8("Смена превысила 24 часа");
		break;
	case 0x4f:
		et=trUtf8("Неверный пароль");
		break;
	case 0x50:
		et=trUtf8("Идет печать предыдущей команды");
		break;
	case 0x51:
		et=trUtf8("Переполнение накоплений наличными в смене");
		break;
	case 0x52:
		et=trUtf8("Переполнение накоплений по типу оплаты 2 в смене");
		break;
	case 0x53:
		et=trUtf8("Переполнение накоплений по типу оплаты 3 в смене");
		break;
	case 0x54:
		et=trUtf8("Переполнение накоплений по типу оплаты 4 в смене");
		break;
	case 0x55:
		et=trUtf8("Чек закрыт - операция невозможна");
		break;
	case 0x56:
		et=trUtf8("Нет документа для повтора");
		break;
	case 0x57:
		break;
	case 0x58:
		et=trUtf8("Ожидание команды продолжения печати");
		break;
	case 0x59:
		et=trUtf8("Документ открыт другим оператором");
		break;
	case 0x5a:
		et=trUtf8("Скидка превышает накопления в чеке");
		break;
	case 0x5b:
		et=trUtf8("Переполнение диапазона надбавок");
		break;
	case 0x5c:
		break;
	case 0x5d:
		et=trUtf8("Таблица не определена");
		break;
	case 0x5e:
		et=trUtf8("Некорректная операция");
		break;
	case 0x5f:
		et=trUtf8("Отрицательный итог чека");
		break;
	case 0x60:
		et=trUtf8("Переполнение при умножении");
		break;
	case 0x61:
		et=trUtf8("Переполнение диапазона цены");
		break;
	case 0x62:
		et=trUtf8("Переполнение диапазона количества");
		break;
	case 0x63:
		et=trUtf8("Переполнение диапазона отдела");
		break;
	case 0x64:
		et=trUtf8("ФП отсутствует");
		break;
	case 0x65:
		et=trUtf8("Не хватает денег в секции");
		break;
	case 0x66:
		et=trUtf8("Переполнение денег в секции");
		break;
	case 0x68:
		et=trUtf8("Не хватает денег по обороту налогов");
		break;
	case 0x69:
		et=trUtf8("Переполнение денег по обороту налогов");
		break;
	case 0x6b:
		et=trUtf8("Нет чековой ленты");
		break;
	case 0x6c:
		et=trUtf8("Нет контрольной ленты");
		break;
	case 0x6d:
		et=trUtf8("Не хватает денег по налогу");
		break;
	case 0x6e:
		et=trUtf8("Переполнение денег по налогу");
		break;
	case 0x6f:
		et=trUtf8("Переполнение по выплате в смене");
		break;
	case 0x70:
		et=trUtf8("Переполнение ФП");
		break;
	case 0x71:
		et=trUtf8("Ошибка отрезчика");
		break;
	case 0x72:
		et=trUtf8("Команда не поддерживается в данном подрежиме");
		break;
	case 0x73:
		et=trUtf8("Команда не поддерживается в данном режиме");
		break;
	case 0x74:
		et=trUtf8("Ошибка ОЗУ");
		break;
	case 0x75:
		et=trUtf8("Ошибка питания");
		break;
	case 0x76:
		et=trUtf8("Ошибка принтера: нет импульсов с тахогенератора");
		break;
	case 0x77:
		et=trUtf8("Ошибка принтера: нет сигналов с датчика");
		break;
	case 0x78:
		et=trUtf8("Замена ПО");
		break;
	case 0x79:
		et=trUtf8("Замена ФП");
		break;
	case 0x7a:
		et=trUtf8("Поле не редактируется");
		break;
	case 0x7b:
		et=trUtf8("Ошибка оборудования");
		break;
	case 0x7c:
		et=trUtf8("Не совпадает дата");
		break;
	case 0x7d:
		et=trUtf8("Неверный формат даты");
		break;
	case 0x7e:
		et=trUtf8("Неверное значение в поле длины");
		break;
	case 0x7f:
		et=trUtf8("Переполнение диапазона итога чека");
		break;
	case 0x80:
	case 0x81:
	case 0x82:
	case 0x83:
		et=trUtf8("Ошибка связи с ФП");
		break;
	case 0x84:
		et=trUtf8("Переполнение наличности");
		break;
	case 0x85:
		et=trUtf8("Переполнение по продажам в смене");
		break;
	case 0x86:
		et=trUtf8("Переполнение по покупкам в смене");
		break;
	case 0x87:
		et=trUtf8("Переполнение по возвратам продаж в смене");
		break;
	case 0x88:
		et=trUtf8("Переполнение по возвратам покупок в смене");
		break;
	case 0x89:
		et=trUtf8("Переполнение по внесению в смене");
		break;
	case 0x8a:
		et=trUtf8("Переполнение по надбавкам в чеке");
		break;
	case 0x8b:
		et=trUtf8("Переполнение по скидкам в чеке");
		break;
	case 0x8c:
		et=trUtf8("Отрицательный итог по надбавкам в чеке");
		break;
	case 0x8d:
		et=trUtf8("Отрицательный итог по скидкам в чеке");
		break;
	case 0x8e:
		et=trUtf8("Нулевой итог чека");
		break;
	}
	return et;
}

bool ShtrihFR::openCheck()
{
	if (m_vCheckBuffer.size()==0)
	{
		return true;
	}
	else
	{
		setErrorText("Current check isn't empty.");
		return false;
	}
}

bool ShtrihFR::cancelPrint()
{
	if (m_vCheckBuffer.size()==0)
	{
		setErrorText("Current check is empty.");
		return false;
	}
	m_vCheckBuffer.clear();
	return true;
}

int ShtrihFR::bueAdd( const QString &name, int dep, double summa, double quantity, int qdec )
{
	PurchaseInfo pi(name,dep,summa,quantity,qdec);
	m_vCheckBuffer.push_back(pi);
	return 1;
}

bool ShtrihFR::printCheck(bool bRet)
{
	if (m_vCheckBuffer.size()==0)
	{
		setErrorText("Current check is empty.");
		return false;
	}
	Byte tax[4]={0,0,0,0};
	if (internalOpenCheck(bRet?FDT_SELLRET:FDT_SELL)!=RSP_OK)
	{
		goto errorexit;
	}
	for(uint i=0;i<m_vCheckBuffer.size();++i)
	{
		if (internalAddOperation(bRet?FOP_SELLRET:FOP_SELL,m_baOperPass,(Decimal)m_vCheckBuffer[i].m_quantity,(Decimal)m_vCheckBuffer[i].m_price,(Byte)m_vCheckBuffer[i].m_dep,tax,m_vCheckBuffer[i].m_name)!=RSP_OK)
			goto errorexit;
	}
	Decimal subTotal;
	if (internalCheckSubtotal(&subTotal)!=RSP_OK)
		goto errorexit;
	if (internalCloseCheck(subTotal,0,0,0,0,tax,"",0)!=RSP_OK)
		goto errorexit;
	m_vCheckBuffer.clear();
	return true;
errorexit:
	m_vCheckBuffer.clear();
	return false;
}

Q_ULONG ShtrihFR::readMachineNumber()
{
	FRState st;
	if (internalReadState(&st)==RSP_OK)
	{
		setMachineNumber(from4Bytes(st.serialNumber));
		return machineNumber();
	}
	else
	{
		setMachineNumber(0);
		return 0;
	}
}

bool ShtrihFR::isCheckOpened()
{
	return m_bCheckIsOpen; //
}

bool ShtrihFR::onlinePrintMode()
{
	return false; // !!! Заглушка !!! Исправить
}

void ShtrihFR::setOnlinePrintMode(bool bOnlinePM)
{
// We cannot change print mode while check is opened!!! This issue should be resolved.
// Maybe we should postpone change until current check has been closed or cleared.
	// !!! Заглушка !!! Исправить
    Q_UNUSED(bOnlinePM);
}

bool ShtrihFR::isOnlinePrintModeSupported()
{
	return false; // I don't support online print mode yet.
}

int ShtrihFR::openCheck(int eDocumentType, int & /*iReserved*/)
{
  makeHelper;
	int paymenttype=eDocumentType & CRO_OPMASK;
	if ((eDocumentType & ~CRO_OPMASK)!=0 || paymenttype==CRO_BUYSTORNO || paymenttype==CRO_CASHIN || paymenttype==CRO_CASHOUT || paymenttype==CRO_SELLSTORNO)
	{
		setErrorText(tr("Unsupported document type."));
		return CRERROR_INVALIDCALL;
	}
	int fdt=0;
	int fop=0;
	switch(eDocumentType)
	{
	case CRO_BUY:
		fdt=FDT_BUY;
		fop=FOP_BUY;
		break;
	case CRO_SELL:
		fdt=FDT_SELL;
		fop=FOP_SELL;
		break;
	case CRO_BUYRET:
		fdt=FDT_BUYRET;
		fop=FOP_BUYRET;
		break;
	case CRO_SELLRET:
		fdt=FDT_SELLRET;
		fop=FOP_SELLRET;
		break;
	default:
		setErrorText(tr("Unsupported document type."));
		return CRERROR_INVALIDCALL;
	}
	m_CheckInfo.m_CheckOperation=fdt;
	m_CheckInfo.m_DefaultOperation=fop;
	if (onlinePrintMode())
	{
		return CRERROR_NOTSUPPORTED;
		int res=internalOpenCheck(fdt);
		if (res==RSP_OK)
		{
			m_bCheckIsOpen=true;
			return CRERROR_OK;
		}
		else
			return CRERROR_DEVICEERROR;
	}
	else
	{
		m_bCheckIsOpen=true;
		return CRERROR_OK;
	}
}

int ShtrihFR::beginAdd(int /*iReserved*/)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	if (!isCheckOpened())
	{
		setErrorText(tr("Check isn't opened. I cannot beginAdd()."));
		return CRERROR_INVALIDCALL;
	}
	if (m_bCrItBAdd)
	{
		setErrorText(tr("Previous call to beginAdd() was not matched with endAdd()."));
		return CRERROR_INVALIDCALL;
	}
	m_CurrentItem.clear();
	m_bCrItCompleted=false;
	m_bCrItProcessed=false;
	m_bCrItBAdd=true;
	m_bCrItBAddImplicit=false;
	return CRERROR_OK;
}

int ShtrihFR::addTax(int iTax)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	if (m_CurrentItem.m_taxes.size()>=4)
	{
		setErrorText(tr("ShtrihFR cannot use more then 4 taxes per item."));
		return CRERROR_INVALIDCALL;
	}
	for(uint i=0;i<m_CurrentItem.m_taxes.size();++i)
	{
		if (m_CurrentItem.m_taxes[i]==iTax)
			return CRERROR_OK;
	}
	m_CurrentItem.m_taxes.push_back(iTax);
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setDiscount(double dDiscount)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	m_CurrentItem.m_dDiscount=dDiscount;
	m_CurrentItem.m_isDiscountInPercent=false;
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setDiscountPercent(double dDisPer)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	m_CurrentItem.m_dDiscount=dDisPer;
	m_CurrentItem.m_isDiscountInPercent=true;
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setItemSection(int iSection)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	m_CurrentItem.m_dep=iSection;
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setOperation(int eOperationType)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	int fdt=0;
	switch(eOperationType)
	{
	case CRO_BUY:
		fdt=FDT_BUY;
		break;
	case CRO_SELL:
		fdt=FDT_SELL;
		break;
	case CRO_BUYRET:
		fdt=FDT_BUYRET;
		break;
	case CRO_SELLRET:
		fdt=FDT_SELLRET;
		break;
	default:
		setErrorText(tr("Unsupported operation type."));
		return CRERROR_INVALIDCALL;
	}
	m_CurrentItem.m_iOperation=fdt;
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setUnit(const QString & sUnit)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	m_CurrentItem.m_sUnit=sUnit;
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setComment(const QString & sComment)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	m_CurrentItem.m_sUnit=sComment;
	m_CurrentItem.m_isEmpty=false;
    return CRERROR_OK;
}

int ShtrihFR::setItem(const QString & sName, double dPrice, double dQuantity)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=checkForBeginAdd();
	if (ret) return ret;// CRERROR_OK == 0, so...
	m_CurrentItem.m_name=sName;
	m_CurrentItem.m_price=dPrice;
	m_CurrentItem.m_quantity=dQuantity;
	m_CurrentItem.m_isEmpty=false;
	m_bCrItCompleted=true; //Disable attributes addition
	if (m_bCrItBAddImplicit)
		return endAdd(0);
    return CRERROR_OK;
}

int ShtrihFR::endAdd(int /*iReserved*/)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int ret=CRERROR_OK;
	if (!m_bCrItBAdd)
	{
		setErrorText(tr("There was no matching beginAdd() call."));
		return CRERROR_INVALIDCALL;
	}
	if (!m_bCrItCompleted)
	{
		setErrorText(tr("There was no setItem() call. Item isn't complete."));
		return CRERROR_INVALIDCALL;
	}
	if (m_bOnlinePrintMode)
	{
		ret=printItemOut(m_CurrentItem);
	}
	else
		m_vCheckBuffer.push_back(m_CurrentItem);
	m_bCrItBAdd=false;
	m_bCrItProcessed=true;
	m_bCrItCompleted=false;
	m_CurrentItem.clear();
	return ret;
}

int ShtrihFR::cancelAdd(int /*iReserved*/)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	if (m_bCrItBAdd)
	{
		setErrorText(tr("There was no matching beginAdd() call."));
		return CRERROR_INVALIDCALL;
	}
	if (m_bCrItProcessed)
	{
		setErrorText(tr("This item was processed already. I cannot cancel it."));
		return CRERROR_INVALIDCALL;
	}
	m_bCrItBAdd=false;
	m_bCrItProcessed=false;
	m_bCrItCompleted=false;
	m_CurrentItem.clear();
	return CRERROR_OK;
}

int ShtrihFR::setPayment(double dSum, int iType)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	if (!m_bCheckIsOpen)
	{
		setErrorText(tr("Check isn't open. I cannot setPayment(%1,%2).").arg(dSum).arg(iType));
		return CRERROR_INVALIDCALL;
	}
	if (iType<1 || iType>4)
	{
		setErrorText(tr("Unsupported payment type %1. Valid range 1..4.").arg(iType));
		return CRERROR_INVALIDCALL;
	}
	m_CheckInfo.m_mPayment[iType]=dSum;
	return CRERROR_OK;
}

int ShtrihFR::addCheckTax(int iTax)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	if (!m_bCheckIsOpen)
	{
		setErrorText(tr("Check isn't open. I cannot addCheckTax(%1).").arg(iTax));
		return CRERROR_INVALIDCALL;
	}
	if (m_CheckInfo.m_vTaxes.size()>=4)
	{
		setErrorText(tr("ShtrihFR does not support more then 4 taxes per item. I cannot addCheckTax(%1).").arg(iTax));
		return CRERROR_INVALIDCALL;
	}
	for(uint i=0;i<m_CheckInfo.m_vTaxes.size();++i)
	{
		if (m_CheckInfo.m_vTaxes[i]==iTax)
			return CRERROR_OK;
	}
	m_CheckInfo.m_vTaxes.push_back(iTax);
	return CRERROR_OK;
}

int ShtrihFR::cancelCheck(int /*iReserved*/)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	if (!m_bCheckIsOpen)
	{
		setErrorText(tr("Check isn't open. I cannot cancelCheck()."));
		return CRERROR_INVALIDCALL;
	}
	if (m_bCrItBAdd)
	{
		int ret=cancelAdd(0); // Открыта позиция, отменяем её.
		if (ret)
			return ret;
	}
	clearCheckInfo();
	m_bCheckIsOpen=false;
	return CRERROR_OK;
}

//!!!TODO: Complete this function.
int ShtrihFR::closeCheck(double &dChange, int /*iReserved*/)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int res;
	if (m_bCrItBAdd)
	{
		setErrorText(tr("There's currently open check position, endAdd() or setItem() wasn't called. I cannot close check."));
		return CRERROR_INVALIDCALL;
	}
	if (onlinePrintMode())
	{ //
		clearCheckInfo();
		m_bCheckIsOpen=false;
		return CRERROR_INVALIDCALL;
	}
	else
	{
		Byte taxes[4]={0,0,0,0};
    FRState state;
    res=internalReadState(&state);
    if (res)
    {
      return CRERROR_DEVICEERROR;
    }
    if (state.frSubmode==3)
    { //CR waits for continue print command
      res=internalContinuePrint();
      if (res)
        return CRERROR_DEVICEERROR;
    }
		res=internalOpenCheck(m_CheckInfo.m_CheckOperation);
		if (res==RSP_COMMANDERROR)
		{
			explainCheckPrintError();
			return CRERROR_DEVICEERROR;
		}
		else if (res)
			return CRERROR_DEVICEERROR;
		for(uint i=0;i<m_vCheckBuffer.size();++i)
		{
			PurchaseInfo & ci=m_vCheckBuffer[i];
			for(uint j=0;j<ci.m_taxes.size();++j)
				taxes[j]=ci.m_taxes[j];
			int op=(ci.m_iOperation==-1)?m_CheckInfo.m_DefaultOperation:ci.m_iOperation;
			res=internalAddOperation(op,m_baOperPass,ci.m_quantity,ci.m_price,ci.m_dep,taxes,ci.m_name);
			if (res)
				goto checkerror;
			if (ci.m_dDiscount!=0)
			{
				bool extra=ci.m_dDiscount<0;
				double discount=fabs(ci.m_dDiscount);
                if (ci.m_isDiscountInPercent)
					discount=ci.m_price*ci.m_quantity*discount/100;
				discount=floor(discount*100+0.5)/100;
				if (extra)
					res=internalAddExtra(discount,taxes,"");
				else
					res=internalAddDiscount(discount,taxes,"");
				if (res)
					goto checkerror;
			}
		}
		double payment[4]={0,0,0,0};
		for(int i=0;i<4;++i)
			if (m_CheckInfo.m_mPayment.find(i+1)!=m_CheckInfo.m_mPayment.end())
				payment[i]=m_CheckInfo.m_mPayment[i+1];
		for(uint j=0;j<m_CheckInfo.m_vTaxes.size();++j)
			taxes[j]=m_CheckInfo.m_vTaxes[j];
		res=internalCloseCheck(payment[0],payment[1],payment[2],payment[3],0,taxes,"",&dChange);
		if (res==RSP_COMMANDERROR)
		{ // Let's make error message more user friendly.
			explainCheckPrintError();
			goto checkerror;
		}
		else if (res)
			goto checkerror;
	}
	clearCheckInfo();
	m_bCheckIsOpen=false;
	return CRERROR_OK;
checkerror:
	QString error=errorText();
	clearCheckInfo();
	m_bCheckIsOpen=false;
	res=internalClearCheck();
	if (res)
	{
		setErrorText(tr("!!!There was error while adding check positions: %1. And I cannot cancel check due to error: %1.").arg(error).arg(errorText()));
		return CRERROR_FATALDEVICEERROR;
	}
	else
		setErrorText(error);
	return CRERROR_DEVICEERROR;
}

void ShtrihFR::explainCheckPrintError()
{
	Result res=0;
	QString checkError=errorText();
	if (deviceError()==0x73)
	{
		FRState frState;
		res=internalReadState(&frState);
		if (res)
		{// Well, we cannot read FR state even, so let's make user aware of situation.
			setErrorText(checkError+tr("\nI cannot determine current FR mode however.\nPlease, call support for further information."));
		}
		else if ((frState.frMode&SHMODE_MASK)==SHMODE_24HOURSHIFT) // Shift begun more then 24 hours ago.
		{
			setErrorText(tr("Shift cannot be longer than 24 hours.\nTake Z-report, please."));
		}
		else if ((frState.frMode&SHMODE_MASK)==SHMODE_BADFISCALPASSWORD)
		{
			setErrorText(tr("FR is locked by wrong fiscal password.\nContact tax officer to resolve situation."));
		}
		else if ((frState.frMode&SHMODE_MASK)==SHMODE_OPENDOC)
		{
			setErrorText(tr("There's open document in the FR.\n"
				"First, make sure You have one copy of Open-POS running.\n"
				"If no other copy is running,\n"
				"then use FR control utility to cancel document."));
		}
	}
	else if (deviceError()==0x58)
	{ // Device waits for "continue print" command. Let's give it to it.
		QString err=errorText();
		internalContinuePrint();
		setErrorText(err+tr("\nContinue print command was issued.\nTry to print check again."));
	}
}

int ShtrihFR::checkForBeginAdd()
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	if (m_bCrItCompleted)
	{
		setErrorText(tr("Item was closed by setItem(). Cannot add attribute"));
		return CRERROR_INVALIDCALL;
	}
	if (!m_bCrItBAdd) // beginAdd() wasn't called we should call it implicitly
	{
		int ret=beginAdd(0);
		if (ret)
			return ret;
	}
	m_bCrItBAddImplicit=true;
	return CRERROR_OK;
}

int ShtrihFR::open()
{
    return super::open();
}

/*! \fn int ShtrihFR::payment(double dSum)
/ru Выплата денег из кассы.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int ShtrihFR::payment(double dSum)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
//Sanity checks
  if (dSum<=0)
  {
      setErrorText(tr("payment(): Sum must be positive."));
      return CRERROR_INVALIDCALL;
  }
  if (internalPayment(dSum)!=RSP_OK)
      return CRERROR_DEVICEERROR;
  else
      return CRERROR_OK;
}

/*! \fn int ShtrihFR::payingin(double dSum)
/ru Внесение денег в кассу.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int ShtrihFR::payingin(double dSum)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
//Sanity checks
    if (dSum<=0)
    {
        setErrorText(tr("payingin(): Sum must be positive."));
        return CRERROR_INVALIDCALL;
    }
    if (internalPayingIn(dSum)!=RSP_OK)
        return CRERROR_DEVICEERROR;
    else
        return CRERROR_OK;
}

int ShtrihFR::curDocumentNumber(int & iDocNum)
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
  int res=0;
  int counter=retryCount();
  FRState fs;
  while(true)
  { // If device is printing document we should wait for it to complete.
//    res=internalReadState(&fs);
    res=internalGetOperRegisterValue(152,&iDocNum);
    if (res!=RSP_OK)
      return CRERROR_DEVICEERROR;
    break;
    if (fs.frSubmode==0)
      break;
    Sleep(500);
    counter--;
    if (counter<=0)
    {
      setErrorText(tr("Device prints some document. I cannot determine current document number exactly."));
      return CRERROR_DEVICEERROR;
    }
  };
  if (res!=RSP_OK)
    return CRERROR_DEVICEERROR;

//  iDocNum=fs.currentDocumentNumber[0]+256*fs.currentDocumentNumber[1];
  return CRERROR_OK;
}


int ShtrihFR::curControlLineNumber(int & iLineNum)
{
	makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int res=0;
	int counter=retryCount();
	FRState fs;
	while(true)
	{ // If device is printing document we should wait for it to complete.
//    res=internalReadState(&fs);
		res=internalGetOperRegisterValue(159,&iLineNum);
		if (res!=RSP_OK)
			return CRERROR_DEVICEERROR;
		break;
		if (fs.frSubmode==0)
			break;
		Sleep(500);
		counter--;
		if (counter<=0)
		{
			setErrorText(tr("Device prints some document. I cannot determine current control line number exactly."));
			return CRERROR_DEVICEERROR;
		}
	};
	if (res!=RSP_OK)
		return CRERROR_DEVICEERROR;

//  iDocNum=fs.currentDocumentNumber[0]+256*fs.currentDocumentNumber[1];
	return CRERROR_OK;
}



int ShtrihFR::checkDeviceReady()
{
  makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
  int ret=CRERROR_OK;
  FRState fs;
  ret=internalReadState(&fs);
  if (ret==CRERROR_OK)
  {
    int mode=fs.frMode&SHMODE_MASK;
    if (fs.frSubmode==3)
    { // Device waits for continue print command
      ret=internalContinuePrint();
      if (ret)
      { // Something goes wrong
        // Just return standard error message
        ret=CRERROR_DEVICEERROR;
      }
      else
      { // Device is printing document now. We suggest user to wait and try again.
        setErrorText(tr("Please, wait for cash register to print document. And try again."));
        ret=CRERROR_DEVICEERROR;
      }
    }
    else if (fs.frSubmode==1 || fs.frSubmode==2)
    { // Device is out of paper.
      setErrorText(tr("Cash register is out of paper.\nPlease, insert tape and try again."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (fs.frSubmode!=0)
    { // Device is printing something
      setErrorText(tr("Cash register is printing something now.\n Please, wait for it to stop, then try again."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_OPENSHIFT || mode==SHMODE_CLOSEDSHIFT)
    { // Ok.
      ret=CRERROR_OK;
    }
    else if (mode==SHMODE_24HOURSHIFT)
    {
			setErrorText(tr("Shift cannot be longer than 24 hours.\nTake Z-report, please."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_BADFISCALPASSWORD)
    {
			setErrorText(tr("FR is locked by wrong fiscal password.\nContact tax officer to resolve situation."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_DATECONFIRM)
    {
			setErrorText(tr("Cash register waits for date confirmation.\nPlease, use utility program to set date."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_DOTPOSITION_CHANGEENABLED)
    {
			setErrorText(tr("Cash register waits for dot position change confirmation.\nPlease, use utility program to set dot position."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_OPENDOC)
    {
			setErrorText(tr("Cash register has open document in its memory.\nPlease, use utility program to cancel or print document."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_TECHRESET)
    {
			setErrorText(tr("ERAM content error.\nPlease, contact support center or use utility proram to reset device."));
      ret=CRERROR_DEVICEERROR;
    }
    else if (mode==SHMODE_TESTRUN)
    {
      ret=internalStopTestRun();
      if (ret==RSP_OK)
      {
			  setErrorText(tr("Test run is running on cash register.\nStop test run command was issued.\nPlease, try again."));
        ret=CRERROR_DEVICEERROR;
      }
    }
    else if (mode==SHMODE_FULLFISCALREPORT)
    {
			setErrorText(tr("Full fiscal report is being printed on cash register.\nPlease, wait and then try again."));
      ret=CRERROR_DEVICEERROR;
    }
    else
    {
      // nothing above matches, so it is something unexpected.
      setErrorText(tr("Device is in unknown state.\nThere's a possibility that driver doesn't support that device.\nPlease, contact support center."));
      ret=CRERROR_FATALDEVICEERROR;
    }
  }
  return ret;
}

int ShtrihFR::startDriver()
{
  printf("Before m_bDriverStarted check.\n");
  if (m_bDriverStarted)
  {
    printf("Driver is running already.\n");
    setErrorText(tr("Driver is running already."));
    return CRERROR_INVALIDCALL;
  }
  printf("Before open.\n");
  if (open()!=0)
  {
    printf("Before makeHelper.\n");
    setErrorText(tr("No error."));
    m_bDriverStarted=true;
    printf("Before internalReadOptions().\n");
    int res=CRERROR_OK;
    if (internalReadOptions(0)!=RSP_OK)
      res=CRERROR_DEVICEERROR;
    if (captureMode()==CM_NonExclusive)
	close();
    return res;
  }
  printf("open failure.\n");
  return CRERROR_DEVICEERROR;
}

int ShtrihFR::stopDriver()
{
  if (!m_bDriverStarted)
  {
    return CRERROR_INVALIDCALL;
  }
  close();
  return CRERROR_OK;
}

int ShtrihFR::setCaptureMode(int iMode)
{
  //if (m_bDriverStarted)
  //{
  //  setErrorText(tr("Do not change capture mode while driver is running."));
  //  return CRERROR_INVALIDCALL;
  //}
  m_iCaptureMode=iMode;
  return CRERROR_OK;
}

/*! \fn int ShtrihFR::openCashbox(int n)
/ru Открыть денежный ящик номер n, нумерация ящиков начинается с 1.
  При исполнении этой функции возможно обращение к устройству.
	\param n - номер денежного ящика (начиная с 1)
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int ShtrihFR::openCashbox(int n)
{
  makeHelper;
  Result res=internalOpenCashbox(n-1);
  return res==RSP_OK?CRERROR_OK:CRERROR_DEVICEERROR;
}

/*! \fn int ShtrihFR::ZReport()
/ru Запускает вывод Z-отчёта на устройстве.
  При исполнении этой функции возможно обращение к устройству.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int ShtrihFR::ZReport()
{
  makeHelper;
  Result res=internalDailyReportAndClear();
  return res==RSP_OK?CRERROR_OK:CRERROR_DEVICEERROR;
}

/*! \fn int ShtrihFR::XReport()
/ru Запускает вывод X-отчёта на устройстве.
  При исполнении этой функции возможно обращение к устройству.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int ShtrihFR::XReport()
{
  makeHelper;
  Result res=internalDailyReport();
  return res==RSP_OK?CRERROR_OK:CRERROR_DEVICEERROR;
}



/*! \fn int ShtrihFR::XReport()
/ru Запускает вывод X-отчёта на устройстве.
  При исполнении этой функции возможно обращение к устройству.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
double ShtrihFR::summaTotal()
{
	makeHelper; // This macro incorporates OClHelper, which automagically opens/closes port as needed.
	int res=0;
	int counter=retryCount();
	FRState fs;
	double sum = 0.0;
	double sumr[4] = {0.0, 0.0, 0.0, 0.0 } ;
	int i;
//	Decimal dSum;

//	clearError();
	setErrorCode( CRERROR_OK );
	while(true)
	{ // If device is printing document we should wait for it to complete.
//    res=internalReadState(&fs);
		for ( i = 0; i <4;i++) {
		    res = internalGetMoneyAccumValue( 193+i, &sumr[i] );
		    if (res!=RSP_OK) break;
		}
//		if ((res=fromDecimal( sum, dSum, sizeof(dSum),DTPrice))!=RSP_OK)
//			return sum; //RSP_INTERNALERROR;
//		res=internalGetOperRegisterValue(159,&iLineNum);
		if (res!=RSP_OK)
			return sum; //CRERROR_DEVICEERROR;
		break;
		if (fs.frSubmode==0)
			break;
		Sleep(500);
		counter--;
		if (counter<=0)
		{
			setErrorText(tr("Device prints some document. I cannot determine current control line number exactly."));
			return sum;//CRERROR_DEVICEERROR;
		}
	};
	if (res!=RSP_OK)
		return sum; //CRERROR_DEVICEERROR;
//  iDocNum=fs.currentDocumentNumber[0]+256*fs.currentDocumentNumber[1];
	sum = sumr[0]-sumr[1]+sumr[2]-sumr[3];
	return sum;

}


#include "teplugin.h"
//template TEPlugin<ShtrihFR>;
typedef TEPlugin<ShtrihFR> TECashRegisterShtrihFRPlugin;
TE_EXPORT_PLUGIN( TECashRegisterShtrihFRPlugin )


