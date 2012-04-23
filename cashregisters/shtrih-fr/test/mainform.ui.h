/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qtextedit.h>
#include <qtable.h>
#include <qstatusbar.h>
#include <qapplication.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qlistbox.h>
#include <qmessagebox.h>

#include <shtrih-fr.h>

QString resText(int res)
{
  QString restxt;
  switch(res)
  {
  case RSP_BUFFEROVERRUN:
    restxt=QObject::tr("Buffer overrun");
    break;
  case RSP_CHECKSUMERROR:
    restxt=QObject::tr("Checksum error");
    break;
  case RSP_COMMANDERROR:
    restxt=QObject::tr("Device returns error");
    break;
  case RSP_INVALIDPARAMETERS:
  case RSP_INVALIDPARAMETER1:
  case RSP_INVALIDPARAMETER2:
  case RSP_INVALIDPARAMETER3:
  case RSP_INVALIDPARAMETER4:
  case RSP_INVALIDPARAMETER5:
  case RSP_INVALIDPARAMETER6:
  case RSP_INVALIDPARAMETER7:
  case RSP_INVALIDPARAMETER8:
    restxt=QObject::tr("Invalid parameters");
    break;
  case RSP_OK:
    restxt=QObject::tr("OK");
    break;
  case RSP_PORTCLOSED:
    restxt=QObject::tr("Port closed");
    break;
  case RSP_RESPONCETOOSHORT:
    restxt=QObject::tr("Responce too short");
    break;
  case RSP_TIMEOUT:
    restxt=QObject::tr("Timeout");
    break;
  default:
    restxt=QObject::tr("Error code %1").arg(res);
  }
  return restxt;
}

#define FR_NAME "TECashRegisterShtrihFR"
void MainForm::init()
{
  m_Shtrih=0;
  if (te_driverExists( FR_NAME )) {
    m_Shtrih = ( ShtrihFR *) te_createDriverObject( FR_NAME );
  } else {
    QMessageBox::critical(0, tr("Error"), tr("Driver Shtrih-FR not installed"), QMessageBox::Ok,0);
    close();
  }
}

void MainForm::open()
{
  m_Shtrih->setDebugLevel(1);
  m_Shtrih->setPassword(ShtrihFR::PTOperator,QString("30"));
  m_Shtrih->setPassword(ShtrihFR::PTAdmin,QString("30"));
  int baudrate=cb_BaudRate->currentText().toInt();
  m_Shtrih->setPortBaudRate(baudrate);
  int res=m_Shtrih->open();
  te_Log->append(tr("open() returns %1").arg(res));
}


void MainForm::close()
{
  if (m_Shtrih)
  {
    int res=m_Shtrih->close();
    te_Log->append(tr("close() returns %1").arg(res));
  }
}


void MainForm::issueCommand()
{
  QString command=le_Command->text();
  int len=command.length();
  ShtrihFR::Byte * cmd=new ShtrihFR::Byte[len/2];
  for(int i=0;i<len/2;++i)
  {
    int byte;
    sscanf(command.ascii()+i*2,"%02x",&byte);
    cmd[i]=(ShtrihFR::Byte)byte;
  }
  ShtrihFR::Byte resp[256];
  int wb=0,rb=0;
  ShtrihFR::Result res=m_Shtrih->sendCommand(cmd,len/2,resp,256,&wb,&rb,true);
  delete [] cmd;

  QString restxt=resText(res);
  te_Log->append(tr("sendCommand returns '%1'. Error text: '%2'. Returned bytes:").arg(restxt).arg(res==RSP_OK?QString(""):m_Shtrih->errorText()));
  QString resptxt;
  for(int j=0;j<rb;j++)
    resptxt+=QString("%1").arg((int)resp[j],2,16);
  te_Log->append(resptxt);
  resptxt="";
  for(int k=0;k<rb;k++)
  {
    if (resp[k])
    {
      QCString onechar((char*)(resp+k),2);
      QString qs=m_Shtrih->deviceToUtf8(onechar);
      if (qs.at(0).isPrint())
        resptxt+=qs;
      else
        resptxt+=" ";
    }
  }
  te_Log->append(resptxt);
}


void MainForm::readState()
{
  int res;
  statusBar()->message(tr("Communicating..."));
  showResult(res=m_Shtrih->internalReadOptions(0));
  te_Log->append(tr("Calling internalReadOptions... Result: '%1'").arg(resText(res)));
  if (res!=RSP_OK)
    te_Log->append(tr("Error text: '%1'").arg(m_Shtrih->errorText()));
  // table1->setEnabled(false);
  QStringList vsl=m_Shtrih->valueNamesList();
  table1->setNumRows(vsl.size());
  for(uint i=0;i<vsl.size();i++)
  {
    //  table1->insertRows(table1->numRows());
    table1->setText(i,0,vsl[i]);
    table1->setText(i,1,m_Shtrih->value(vsl[i]).toString());
  }
  table1->adjustColumn(0);
  table1->adjustColumn(1);
  // table1->setEnabled(true);
  // table1->
}


void MainForm::burstRead()
{
  int baudrate[]={2400,4800,9600,19200,38400,57600,115200};
  int res;
  for(int i=0;i<7;i++)
  {
    m_Shtrih->setPortBaudRate(baudrate[i]);
    te_Log->append(tr("Checking baud rate %1").arg(baudrate[i]));
    statusBar()->message(tr("Communicating..."));
    showResult(res=m_Shtrih->internalReadOptions(0));
    if (res==RSP_OK)
    {
      for(int j=0;j<20;++j)
      {
        QApplication::sendPostedEvents();
        readState();
      }
      break;
    }
    QApplication::sendPostedEvents();
  }
}


void MainForm::printString()
{
  int res;
  statusBar()->message(tr("Communicating..."));
  showResult(res=m_Shtrih->internalPrintLine(le_Command->text(),true,true));
  if (res!=RSP_OK)
    te_Log->append(tr("Error text: '%1'").arg(m_Shtrih->errorText()));
}


void MainForm::readTableList()
{
  int res;
  QString name;
  lb_Tables->clear();
  for(int i=1;i<100;++i)
  {
    statusBar()->message(tr("Communicating..."));
    showResult(res=m_Shtrih->internalQueryTableInfo(i,name,0,0));
    if (res!=RSP_OK)
      return;
    lb_Tables->insertItem(name);
  }
}

void MainForm::tableSelected(int num)
{
  num=num+1;
  int cols,rows;
  int res;
  QString name;
  statusBar()->message(tr("Communicating..."));
  showResult(res=m_Shtrih->internalQueryTableInfo(num,name,&rows,&cols));
  if (res!=RSP_OK)
    return;
  table2->setNumRows(cols);
  table2->setNumCols(rows);
  QStringList columnnames;
  int ctype[100];
  int csize[100];
  int i,j;
  for(i=1;i<=cols;++i)
  {
    statusBar()->message(tr("Communicating..."));
    showResult(res=m_Shtrih->internalQueryColumnInfo(num,i,name,ctype+i,csize+i));
    if (res!=RSP_OK)
      return;
    if (ctype[i]==0)
    {
      ShtrihFR::Byte *pMin=new ShtrihFR::Byte[csize[i]];
      ShtrihFR::Byte *pMax=new ShtrihFR::Byte[csize[i]];
      statusBar()->message(tr("Communicating..."));
      showResult(res=m_Shtrih->internalQueryColumnInfo(num,i,name,0,0,(void*)pMin,(void*)pMax));
      if (res==RSP_OK)
      {
        ShtrihFR::Decimal dMin,dMax;
        if (m_Shtrih->toDecimal(pMin,csize[i],&dMin,ShtrihFR::DTInteger)==RSP_OK &&
          m_Shtrih->toDecimal(pMax,csize[i],&dMax,ShtrihFR::DTInteger)==RSP_OK)
          name=QString("%1 (%2-%3)").arg(name).arg(dMin,0,'f',0).arg(dMax,0,'f',0);
      }
      delete []pMin;
      delete []pMax;
    }
    columnnames+=name;
  }
  table2->setRowLabels(columnnames);
  //table2->verticalHeader()->adjustHeaderSize();
  for(i=1;i<=cols;++i)
  {
    for(j=1;j<=rows;j++)
    {
      ShtrihFR::Byte buf[256];
      int size=csize[i];
      statusBar()->message(tr("Communicating..."));
      showResult(res=m_Shtrih->internalReadField(num,j,i,(void*)buf,&size));
      if (res!=RSP_OK)
        return;
      QString val;
      if (ctype[i]==1)
      { // string
        QCString cstr((char*)buf,size+1);
        val=m_Shtrih->deviceToUtf8(cstr);
      }
      else
      {
        ShtrihFR::Decimal dec;
        m_Shtrih->toDecimal(buf,size,&dec,ShtrihFR::DTInteger);
        val=QString("%1").arg(dec);
      }
      table2->setText(i-1,j-1,val);
    }
  }
  for(j=1;j<=rows;j++)
    table2->adjustColumn(j-1);
  table2->verticalHeader()->setResizeEnabled(false);
  // table2->verticalHeader()->setStretchEnabled(true);
}


void MainForm::showResult( int res )
{
  if (res==RSP_OK)
  {
    statusBar()->message(tr("OK"));
    return;
  }
  QString msg=resText(res);
  if (res==RSP_COMMANDERROR)
    msg+=QString(" ")+m_Shtrih->devErrorText(m_Shtrih->deviceError());
  else
    msg+=QString(" ")+m_Shtrih->errorText();
  statusBar()->message(msg);
}


void MainForm::setPortConfig()
{
  int portnum=cb_Port->currentText().toInt();
  int baudrate=cb_BaudRate->currentText().toInt();
  int res;
  //    m_Shtrih->setPortNumber(portnum);
  statusBar()->message(tr("Communicating..."));    
  showResult(res=m_Shtrih->internalSetCommConfig(0,baudrate,100));
  if (res==RSP_OK)
    m_Shtrih->setPortBaudRate(baudrate);
}


void MainForm::saveDump()
{
  int devNum=le_devNumber->text().toInt();
  int blocks=0;
  int res;
  m_Shtrih->internalEndDump();
  statusBar()->message(tr("Sending command internalBeginDump..."));    
  showResult(res=m_Shtrih->internalBeginDump(devNum,&blocks));
  if (res!=RSP_OK)
    return;
  int retDevNum=0;
  int blockNum=0;
  ShtrihFR::Byte *data=new ShtrihFR::Byte[32*blocks];
  pb_Load->setTotalSteps(blocks-1);
  pb_Load->setProgress(0);
  statusBar()->message(tr("Aquiring dump..."));    
  for(int i=0;i<blocks;i++)
  {
    //  statusBar()->message(tr("Sending command internalGetData..."));    
    /* showResult */(res=m_Shtrih->internalGetData(&retDevNum,&blockNum,data+32*i,32));
    if (res!=RSP_OK)
      goto errexit;
    if (retDevNum!=devNum)
    {
      statusBar()->message(tr("Wrong device number in device responce"));
      goto errexit;
    }
    if (blockNum!=i)
    {
      statusBar()->message(tr("Block sequence error. %1 was expected, %2 is received.").arg(i).arg(blockNum));
      goto errexit;
    }
    pb_Load->setProgress(i);
    QApplication::sendPostedEvents();
  }
  {
    QString sfn=QFileDialog::getSaveFileName();
    QFile file(sfn);
    if (file.open(IO_WriteOnly | IO_Truncate))
    {
      file.writeBlock((const char*)data,32*blocks);
      file.close();
      statusBar()->message(tr("Dump was saved to file '%1'.").arg(sfn));
    }
    else
    {
      statusBar()->message(tr("Cannot open file '%1' for writing.").arg(sfn));
    }
  }
errexit:
  m_Shtrih->internalEndDump();
  delete []data;
  return;
}


void MainForm::chopper()
{
  for(int i=0;i<10;i++)
  {
    m_Shtrih->internalTapeAdvance(2,1);
    m_Shtrih->internalCutCheck(1);
  }
}


void MainForm::commitAction()
{
  int actNum=lb_Ops->currentItem();
  QString name=le_ItName->text();
  double sum=le_Sum->text().toDouble();
  int res=0;
  ShtrihFR::Byte operpass[4]={30,0,0,0};
  double subtotal=0,change=0;
  switch(actNum)
  {
  case 0: // Open check for sell
    showResult(res=m_Shtrih->internalOpenCheck(FDT_SELL));
    break;
  case 1: // Open check for sell return
    showResult(res=m_Shtrih->internalOpenCheck(FDT_SELLRET));
    break;
  case 2: // Add sell op
    showResult(res=m_Shtrih->internalAddOperation(FOP_SELL,operpass,1,sum,1,0,name));
    break;
  case 3: // Add sell ret op
    showResult(res=m_Shtrih->internalAddOperation(FOP_SELLRET,operpass,1,sum,1,0,name));
    break;
  case 4: // Close check
    showResult(res=m_Shtrih->internalCheckSubtotal(&subtotal));
    if (res==RSP_OK)
      showResult(res=m_Shtrih->internalCloseCheck(subtotal,0,0,0,0,0,"Check closed",&change));
    break;
  case 5: // New mode check
    {
      int i;
      double change=0;
      showResult(res=m_Shtrih->openCheck(CRO_SELL,i));
      if (res) break;
      showResult(res=m_Shtrih->setItem(QString::fromUtf8("Тест"),50.5,1));
      if (res) { m_Shtrih->cancelCheck(0);break;}
      showResult(res=m_Shtrih->addTax(1));
      if (res) { m_Shtrih->cancelCheck(0);break;}
      showResult(res=m_Shtrih->setDiscount(-2));
      if (res) { m_Shtrih->cancelCheck(0);break;}
      showResult(res=m_Shtrih->addTax(2));
      if (res) { m_Shtrih->cancelCheck(0);break;}
      showResult(res=m_Shtrih->setItem(QString::fromUtf8("Тест2"),20.5,1.5));
      if (res) { m_Shtrih->cancelCheck(0);break;}
      showResult(res=m_Shtrih->setPayment(50,1));
      showResult(res=m_Shtrih->setPayment(50,2));
      if (res) { m_Shtrih->cancelCheck(0);break;}
      showResult(res=m_Shtrih->closeCheck(change,0));
      if (res) { m_Shtrih->cancelCheck(0);break;}
    }
    break;
  case 6: // dailyReportAndClear
    {
      showResult(res=m_Shtrih->internalDailyReportAndClear());
    }
    break;
  }
}


void MainForm::commitTableChanges()
{
  int num=lb_Tables->currentItem()+1; 
  int res;
  int rc,cc;
  QString name;
  showResult(res=m_Shtrih->internalQueryTableInfo(num,name,&rc,&cc));
  if (res) return;
  for(int i=1;i<=cc;++i)
  {
    int tp,sz;
    showResult(res=m_Shtrih->internalQueryColumnInfo(num,i,name,&tp,&sz));
    if (res) return;
    for(int j=1;j<=rc;++j)
    {
      int tcol=j-1;
      int trow=i-1;
      QString text=table2->text(trow,tcol);
      union
      {
        ShtrihFR::Byte data[40];
        Q_LLONG longv;
      }data;
      memset((void*)&data,0,sizeof(data));
      if (tp==1)
        m_Shtrih->stringToByteArray(text,data.data,sizeof(data.data));
      else
        data.longv=text.toLongLong();
      showResult(res=m_Shtrih->internalWriteField(num,j,i,(void*)&data,sz));
      if (res) te_Log->append(tr("Error at (%1,%2): %3").arg(i).arg(j).arg(m_Shtrih->errorText()));
    }
  }
}


bool MainForm::success()
{
  return (bool)m_Shtrih;
}


void MainForm::onOperRegs()
{
  for(int i=0;i<256;++i)
  {
    int val;
    if (m_Shtrih->internalGetOperRegisterValue(i,&val)==0)
    {
      te_Log->append(QString("%1 - %2").arg(i).arg(val));
    }
    else
    {
      te_Log->append(QString("%1 - error").arg(i).arg(val));
    }
  }
}


void MainForm::onMoneyRegs()
{
    double val;
  for(int i=0;i<256;++i)
  {
   val = 0.0;
    if (m_Shtrih->internalGetMoneyAccumValue(i,&val)==0)
    {
      te_Log->append(QString("%1 - %2").arg(i).arg(val));
    }
    else
    {
      te_Log->append(QString("%1 - error").arg(i).arg(val));
    }
  }
}
