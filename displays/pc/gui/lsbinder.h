#ifndef LSBINDER_H
#define LSBINDER_H
#include <qstring.h>
#include <qstringlist.h>
#include <qfont.h>
#include <tdatafile.h>
#include <vector>

using std::vector;

/*
Usage pattern:
T::T()
{
	m_Binder.addBinding("BueName_bkcolor",BueName,paletteBackgroundColor,setPaletteBackgroundColor);
	m_Binder.addBinding("BueName_fgcolor",BueName,paletteForegroundColor,setPaletteForegroundColor);
}

T::load()
{
	m_Binder.load();
}

*/

// Type converter
template <class T, class GetVType>
class DefGetAccessor
{
public:
	typedef GetVType (T::*Type)()const;
};

//Type converter
template <class T, class SetVType>
class DefSetAccessor
{
public:
	typedef void (T::*Type)(SetVType);
};

//Interface to load/save stream
class LSStream
{
public:
	virtual bool Load(const QString & sName, int & iVal)=0;
	virtual bool Load(const QString & sName, double & dVal)=0;
	virtual bool Load(const QString & sName, bool & bVal)=0;
	virtual bool Load(const QString & sName, QColor & clVal)=0;
	virtual bool Load(const QString & sName, QString & sVal)=0;
	virtual bool Save(const QString & sName, int iVal)=0;
	virtual bool Save(const QString & sName, const double & dVal)=0;
	virtual bool Save(const QString & sName, bool bVal)=0;
	virtual bool Save(const QString & sName, const QColor & clVal)=0;
	virtual bool Save(const QString & sName, const QString & sVal)=0;
	virtual ~LSStream(){}
};

class TDataFileStream : public LSStream
{
private:
	TDataFile * m_pTDF;
public:
	TDataFileStream(TDataFile * pTDF)
	{
		m_pTDF=pTDF;
	}
	virtual bool Load(const QString & sName, int & iVal)
	{
		QString val=m_pTDF->readString(sName);
		if (val.isEmpty())
			return false;
		iVal=val.stripWhiteSpace().toInt();
		return true;
	}

	virtual bool Load(const QString & sName, double & dVal)
	{
		QString val=m_pTDF->readString(sName);
		if (val.isEmpty())
			return false;
		dVal=val.stripWhiteSpace().toDouble();
		return true;
	}

	virtual bool Load(const QString & sName, bool & bVal)
	{
		QString val=m_pTDF->readString(sName);
		if (val.isEmpty())
			return false;
		bVal=(bool)val.stripWhiteSpace().toInt();
		return true;
	}

	virtual bool Load(const QString & sName, QColor & clVal)
	{
		QColor val=m_pTDF->readColor(sName);
		if (!val.isValid())
			return false;
		clVal=val;
		return true;
	}

	virtual bool Load(const QString & sName, QString & sVal)
	{
		QString val=m_pTDF->readString(sName);
		if (val.isEmpty())
			return false;
		sVal=val;
		return true;
	}

	virtual bool Save(const QString & sName, int iVal)
	{
		m_pTDF->writeNumber(sName,iVal,10);
		return true;
	}
	virtual bool Save(const QString & sName, const double & dVal)
	{
		m_pTDF->writeString(sName,QString("%1").arg(dVal));
		return true;
	}
	virtual bool Save(const QString & sName, bool bVal)
	{
		m_pTDF->writeNumber(sName,bVal);
		return true;
	}
	virtual bool Save(const QString & sName, const QColor & clVal)
	{
		m_pTDF->writeColor(sName,clVal);
		return true;
	}
	virtual bool Save(const QString & sName, const QString & sVal)
	{
		m_pTDF->writeString(sName,sVal);
		return true;
	}
};


//Interface to object's property loading/saving
struct LoaderSaverBase
{
	LoaderSaverBase(const QString & name):m_sName(name){}
	virtual ~LoaderSaverBase(){};
	virtual bool Load(LSStream * pStream)=0;
	virtual bool Save(LSStream * pStream)=0;
	QString m_sName;
};

//Implementation of object's property loading/saving
template <class T, class GetVType, class SetVType, template <class , class > class GetAccessor=DefGetAccessor, template <class , class> class SetAccessor=DefSetAccessor>
struct LoaderSaverImpl : public LoaderSaverBase
{
	LoaderSaverImpl(const QString & name,T * pObj, GetVType DefVal, typename GetAccessor<T,GetVType>::Type pGet, typename SetAccessor<T,SetVType>::Type pSet) : LoaderSaverBase(name)
	{
		m_pObj=pObj;
		m_pGet=pGet;
		m_pSet=pSet;
		m_DefVal=DefVal;
	}
	virtual bool Load(LSStream * pStream)
	{
		GetVType val;
		if (pStream->Load(m_sName,val))
			(m_pObj->*m_pSet)(val);
		else
			return true; //(m_pObj->*m_pSet)(m_DefVal);
		return true;
	}

	virtual bool Save(LSStream * pStream)
	{
		GetVType val=(m_pObj->*m_pGet)();
		return pStream->Save(m_sName,val);
	}

	T * m_pObj;
	typename GetAccessor<T,GetVType>::Type m_pGet;
	typename SetAccessor<T,SetVType>::Type m_pSet;
	GetVType m_DefVal;
};

template <class T>
struct LoaderSaverFontSizeImpl : public LoaderSaverBase
{
	LoaderSaverFontSizeImpl(const QString & name,T * pObj, int DefVal) : LoaderSaverBase(name)
	{
		m_pObj=pObj;
		m_DefVal=DefVal;
	}
	virtual bool Load(LSStream * pStream)
	{
		int val;
		if (!pStream->Load(m_sName,val))
			return true;
//			val=m_DefVal;
		QFont fnt=m_pObj->font();
		fnt.setPixelSize(val);
		m_pObj->setFont(fnt);
		return true;
	}

	virtual bool Save(LSStream * pStream)
	{
		int val=m_pObj->font().pixelSize();
		return pStream->Save(m_sName,val);
	}

	T * m_pObj;
	int m_DefVal;
};


class Serializer
{
public:
	Serializer()
	{
	}
	~Serializer()
	{
		uint i;
		for(i=0;i<m_vLS.size();i++)
			delete m_vLS[i];
	}

//	void setConfigName(const QString &);
	bool LoadSettings(LSStream * pLSS)
	{
		uint i;
		bool ret=true;
		for(i=0;i<m_vLS.size();i++)
			ret&=m_vLS[i]->Load(pLSS);
		return ret;
	}

	bool SaveSettings(LSStream * pLSS)
	{
		uint i;
		bool ret=true;
		for(i=0;i<m_vLS.size();i++)
			ret&=m_vLS[i]->Save(pLSS);
		return ret;
	}

	void addBinding(LoaderSaverBase * pLSB)
	{
		m_vLS.push_back(pLSB);
	}

	QString lastError() const;
private:
	vector<LoaderSaverBase *> m_vLS;
};

#endif
