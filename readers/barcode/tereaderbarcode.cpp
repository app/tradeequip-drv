#include "tereaderbarcode.h"
#include <teplugin.h>
#include <vector>

TEReaderBarcode::TEReaderBarcode() : superclass()
{
    setName("TEReaderBarcode");
}

int TEReaderBarcode::openPort() // opens port
{
	return open();
}

int TEReaderBarcode::closePort() // closes port
{
	return isOpen()?close():0;
}

int TEReaderBarcode::readBarcode() // tries to read barcode
{
	if (!isOpen())
		return 0;
	if (readCount()==0)
		return 0;

	QString barcode;

	while (true)
	{
		int ich=getch();
		char ch=(char)ich;
		if (ich==-1)
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
			if (!dropStopByte())
				barcode.append(QChar(ch));
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

bool TEReaderBarcode::setParameter(const QString &, const QString &) // set Scanner parameter for configurable scanners
{
	setErrorText(tr("Generic scanner driver doesn't support scanner configuration."));
	return false;
}

TEReaderBase * createObject(const QString &)
{
	return new TEReaderBarcode;
}

void destroyObject(TEReaderBase * pScan)
{
	delete pScan;
}

typedef TEPlugin<TEReaderBarcode> TEReaderBarcodePlugin;
TE_EXPORT_PLUGIN( TEReaderBarcodePlugin )
