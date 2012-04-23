#ifndef _DISPLAYEPSON_H_
#define _DISPLAYEPSON_H_

#include <qobject.h>
#include <qstring.h>
#include <qapplication.h>
#include <qiodevice.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qcstring.h>
#include "tedisplaybase.h"
#include "estring.h"

#ifdef EPSON_BUILD
#  define EPSON_EXPORT LIB_DLLEXPORT
#  define EPSON_TEMPLATE_EXTERN
#else
#  define EPSON_EXPORT LIB_DLLIMPORT
#  define EPSON_TEMPLATE_EXTERN TEMPLATE_EXTERN
#endif

class EPSON_EXPORT TEDisplayEpson : public TEDisplayBase
{
	Q_OBJECT

	typedef TEDisplayBase superclass;

public:
	TEDisplayEpson(int pn=1);
	~TEDisplayEpson();

	int displayWidth();

	virtual QString modelInfo();
	QCString utf8ToDevice( const QString &text );
	QString deviceToUtf8( const QCString &text );

	virtual int scrollSpeed();
	virtual int brightness()
	{
		return iBrightness;
	};
public slots:
	virtual void setScrollSpeed(int);

	// Method sets display brightness in range 0..255
	void setBrightness(int);

	// Methos sets display update interval in ms
	void setUpdateInterval(int interval);

	virtual void setPortBaudRate(int);

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

