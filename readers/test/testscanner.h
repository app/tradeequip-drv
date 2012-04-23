#include <tereaderbarcode.h>
#include <iostream>
//#include <config.h>
#include <qobject.h>
#include <qapplication.h>
#include <qstring.h>

using namespace std;

class Receiver : public QObject
{
    Q_OBJECT
    TEReaderBase *m_pS;
public:
    Receiver(TEReaderBase * pS):m_pS(pS){};
public slots:
    void onSignal(int sig)
    {
        cout << "Signal N" << sig << " received." << endl;
		if (!m_pS->errorText().isEmpty())
			cout << "Error text:" << m_pS->errorText().ascii() << endl;
		if (!m_pS->text().isEmpty())
			cout << "Barcode:" << m_pS->text().ascii() << endl;
		m_pS->next();
    }
}; 
