#ifndef _TEREADERCARD_H_
#define _TEREADERCARD_H_

#include <qtimer.h>
#include <tereaderbase.h>

#ifdef TRCARD_BUILD
#  define TRCARD_EXPORT LIB_DLLEXPORT
#  define TRCARD_TEMPLATE_EXTERN
#else
#  define TRCARD_EXPORT LIB_DLLIMPORT
#  define TRCARD_TEMPLATE_EXTERN TEMPLATE_EXTERN
#endif

class TRCARD_EXPORT TEReaderCard : public TEReaderBase
{
    Q_OBJECT
public:
  typedef TEReaderBase superclass;
	TEReaderCard();

  virtual QString text(int iTrackNumber=0) const;
	virtual bool setParameter(const QString &, const QString &); // set Scanner parameter for configurable scanners
	virtual int setConnectionType( int eCt );

  virtual int readerType() const {return RT_CARD;}

  virtual int startDriver();
  virtual int stopDriver();

  virtual QValueList<int> supportedBaudRates()
  {
    QValueList<int> ret;
    ret<<2400<<4800<<7200<<9600<<14400<<19200<<38400<<57600<<115200;
    return ret;
  }

protected:
	bool eventFilter( QObject *obj, QEvent *e );
	virtual int openPort(); // opens port
	virtual int closePort(); // closes port
	virtual int readBarcode(); // tries to read barcode
  void clearKeyBuffer();
private:
	int m_iCount;
	bool m_bSuffix;
	bool m_bCodeStarted;
  bool m_bDriverStarted;
	QString m_qsReadBuffer;
};

TRCARD_EXPORT TEReaderBase * createObject(const QString &);
TRCARD_EXPORT void destroyObject(TEReaderBase *);

#endif
