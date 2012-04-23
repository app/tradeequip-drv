#ifndef _TEDISPLAYVF2029_H_
#define _TEDISPLAYVF2029_H_

#include <qobject.h>
#include <qstring.h>
#include <qapplication.h>
#include <qiodevice.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qcstring.h>
#include "tedisplaybase.h"
#include "estring.h"

#ifdef VF_BUILD
#  define VF_EXPORT LIB_DLLEXPORT
#  define VF_TEMPLATE_EXTERN
#else
#  define VF_EXPORT LIB_DLLIMPORT
#  define VF_TEMPLATE_EXTERN TEMPLATE_EXTERN
#endif

class VF_EXPORT TEDisplayVF2029 : public TEDisplayBase
{
	Q_OBJECT

	typedef TEDisplayBase superclass;

public:
	TEDisplayVF2029(int pn=1);
	~TEDisplayVF2029();

	int displayWidth();

	virtual QString modelInfo();
	QCString utf8ToDevice( const QString &text );
	QString deviceToUtf8( const QCString &text );

	virtual int scrollSpeed();
	virtual int brightness()
	{
		return iBrightness;
	};
  virtual bool isCommandSetSupported(int);
public slots:
  virtual QValueList<int> supportedBaudRates() {return QValueList<int>() << 9600 << 19200;}
	virtual void setScrollSpeed(int);

	// Method sets display brightness in range 0..255
	void setBrightness(int);

	// Method sets display update interval in ms
	void setUpdateInterval(int interval);

	virtual void setPortBaudRate(int);

  // sets command set (one of TEDisplayBase::CommandSet), returns non-zero value on error
  virtual int setCommandSet(int);

	virtual void initDisplay();
	virtual void setDisplayMode(const QString & mode);
	virtual void showText(const QString & text, 
		const QString & category="default", 
		const QString & mode="scroll-left");
	virtual void loadUserDefChars(int _firstChar, int _lastChar, const QByteArray & arr);
	virtual void processEvent( int Code );
//	virtual QStringList execCommand(const QString & cmd);
protected slots:
	void updateDisplay();
	void onTimer();

protected:
	virtual void initPort();
	void setBrightnessInternal();
	void loadUserDefCharsInternal();
	void initDisplayInternal();

	static QTextCodec *ibm866;
	EString l1,l2;
	int updInterval;

	bool bInitPending;

	bool bBrightnessPending;
	int iBrightness;

	bool bLoadCharsPending;
	int firstChar;
	int lastChar;
	QByteArray charBitmaps;

	int iInitCounter;
};

#endif

