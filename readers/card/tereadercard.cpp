#include <qapplication.h>
#include "tereadercard.h"
#include <vector>


TEReaderCard::TEReaderCard() : TEReaderBase()
{
    setName("TEReaderCard");
    m_bCodeStarted = false;
    m_qsReadBuffer = "";
    m_iCount = 0;
    m_bDriverStarted=false;
}


int
TEReaderCard::setConnectionType(int ct)
{
  bool bEvFilterIsInstalled=false;
  clearKeyBuffer();
	if (ct==ECT_KEYBOARD)
  {
    if (m_bDriverStarted)
    {
      qApp->removeEventFilter(this);
 		  qApp->installEventFilter(this);
      bEvFilterIsInstalled=true;
    }
	} 
  else
  {
 		qApp->removeEventFilter(this);
	}
	int ret=superclass::setConnectionType(ct);
  if (ret)
  { // failure
    if (bEvFilterIsInstalled)
      qApp->removeEventFilter(this);
  }
  return ret;
}



bool
TEReaderCard::eventFilter( QObject *, QEvent *e )
{
	QKeyEvent *k;
	if ( !e->spontaneous() ) return false;
	if ( e->type() == QEvent::KeyPress ) 
  {
		k = ( QKeyEvent *) e;
    if (k->text().isEmpty())
      return FALSE;
		m_qsReadBuffer += k->text();
    if (!m_bCodeStarted)
    { // We haven't encountered startSequence() yet.
      // Zero-lenght start sequence!
      m_qsReadBuffer=m_qsReadBuffer.right(startSequence().length()+1);
      if (m_qsReadBuffer==startSequence())
        m_bCodeStarted=true;
    }
    else
    { // We are waiting for stopSequence
			if (m_qsReadBuffer.right(stopSequence().length()) == stopSequence())
      {
        pushText(m_qsReadBuffer);
        clearKeyBuffer();
			}
      return TRUE;
    }
  }
	return FALSE;
}

void TEReaderCard::clearKeyBuffer()
{
  m_bCodeStarted=false;
  m_iCount=1;
  m_qsReadBuffer="";
}

int TEReaderCard::openPort() // opens port
{
  if (connectionType()==ECT_KEYBOARD)
  {
    clearKeyBuffer();
    qApp->installEventFilter(this);
    return 1; // success.
  }
  else
	  return open();
}

int TEReaderCard::closePort() // closes port
{
  if (connectionType()==ECT_KEYBOARD)
  {
    qApp->removeEventFilter(this);
    clearKeyBuffer();
    return 1;
  }
  else
	  return isOpen()?close():0;
}



int TEReaderCard::readBarcode() // tries to read barcode
{
	if (!isOpen())
		return 0;
	if (readCount()==0)
		return 0;

	QString barcode;

	while (true)
	{
		char ch=(char)getch();
		if (isTimeout())
		{
			if (useStopByte())
			{
				// we haven't received stop byte. Drop that byte sequence
				reportError(tr("Timeout while waiting for barcode stop byte"));
				return 0;
			}
			else
			{ // we don't use stop byte, so it is end of barcode
				barcode=barcode.mid(dropBegCnt(),barcode.length()-dropBegCnt()-dropEndCnt());
				pushText(barcode);
				return 1;
			}
		}
		if (useStopByte() && ch==(char)stopByte())
		{
			if (!dropStopByte()) barcode.append(QChar(ch));
			barcode=barcode.mid(dropBegCnt(),barcode.length()-dropBegCnt()-dropEndCnt());
			pushText(barcode);
			return 1;
		}
		barcode.append(QChar(ch));
		if ((int)barcode.length()>maxLength())
		{
			reportError(tr("Barcode is too long (more than %1 byte(s) lenght)").arg(maxLength()));
			return 0;
		}
	}
}


bool TEReaderCard::setParameter(const QString &, const QString &) // set Scanner parameter for configurable scanners
{
	setErrorText(tr("Generic scanner driver doesn't support scanner configuration."));
	return false;
}

int TEReaderCard::startDriver()
{
  if (m_bDriverStarted)
  {
    setErrorText(tr("Driver is running already."));
    return CRERROR_INVALIDCALL;
  }
  int ret=superclass::startDriver();
  if (ret)
    return ret; // error
  return CRERROR_OK;
}

int TEReaderCard::stopDriver()
{
  if (!m_bDriverStarted)
  {
    setErrorText(tr("Driver is stopped already."));
    return CRERROR_INVALIDCALL;
  }
  int ret=superclass::stopDriver();
  if (ret)
    return ret; // error
  return CRERROR_OK;
}

QString TEReaderCard::text(int iTrackNumber) const
{
  QString txt=superclass::text(0);
  switch(iTrackNumber)
  {
  case 0:
    return txt;
  case 1:
    return txt.section(' ',0,0); // Magnetic card track can not contain space symbols.
  case 2:
    return txt.section(' ',1,1);
  case 3:
    return txt.section(' ',2,2);
  }
  return txt;
}

TEReaderBase * createObject(const QString &)
{
	return new TEReaderCard;
}

void destroyObject(TEReaderBase * pScan)
{
	delete pScan;
}


#include "teplugin.h"
typedef TEPlugin<TEReaderCard> TEReaderCardPlugin;
TE_EXPORT_PLUGIN( TEReaderCardPlugin )
