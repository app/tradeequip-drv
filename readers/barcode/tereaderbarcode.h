#ifndef _TEREADERBARCODE_H_
#define _TEREADERBARCODE_H_
#include <tereaderbase.h>

#ifdef TRBARCODE_BUILD
#  define TRBARCODE_EXPORT LIB_DLLEXPORT
#  define TRBARCODE_TEMPLATE_EXTERN
#else
#  define TRBARCODE_EXPORT LIB_DLLIMPORT
#  define TRBARCODE_TEMPLATE_EXTERN TEMPLATE_EXTERN
#endif

class TRBARCODE_EXPORT TEReaderBarcode : public TEReaderBase
{
    Q_OBJECT
    typedef TEReaderBase superclass;
public:
	TEReaderBarcode();
	virtual bool setParameter(const QString &, const QString &); // set Scanner parameter for configurable scanners
  virtual int readerType() const {return RT_BARCODE;}

  virtual QValueList<int> supportedBaudRates()
  {
    QValueList<int> ret;
    ret<<2400<<4800<<7200<<9600<<14400<<19200<<38400<<57600<<115200;
    return ret;
  }


protected:
    virtual int openPort(); // opens port
    virtual int closePort(); // closes port
    virtual int readBarcode(); // tries to read barcode
};

TRBARCODE_EXPORT TEReaderBase * createObject(const QString &);
TRBARCODE_EXPORT void destroyObject(TEReaderBase *);

#endif
