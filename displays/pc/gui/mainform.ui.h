/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qstatusbar.h>
#include <qtextcodec.h>
#include <qcolordialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include "dconfig.h"

template <class T, class V>
class CRGetAccessor
{
public:
	typedef const V & (T::*Type)() const;
};

void MainForm::init()
{
#define BGCOLORQLABEL(Item) m_Serializer.addBinding(new LoaderSaverImpl<QLabel,QColor,const QColor&,CRGetAccessor >\
	( #Item "_bkcolor",Item,QColor(0,0,0),&QLabel::paletteBackgroundColor,&QLabel::setPaletteBackgroundColor))
#define FGCOLORQLABEL(Item) m_Serializer.addBinding(new LoaderSaverImpl<QLabel,QColor,const QColor&,CRGetAccessor >\
	( #Item "_fgcolor",Item,QColor(0,0,0),&QLabel::paletteForegroundColor,&QLabel::setPaletteForegroundColor))
#define FONTSIZE(Item) m_Serializer.addBinding(new LoaderSaverFontSizeImpl<QLabel>( #Item "_fontsize",Item,20))

#define ADDTOSERIALIZER(Item) BGCOLORQLABEL(Item);FGCOLORQLABEL(Item);FONTSIZE(Item)

	Header->installEventFilter(this);
	m_Serializer.addBinding(new LoaderSaverImpl<MainForm,QString,const QString&>
		( "HostName",this,"",&MainForm::hostName,&MainForm::setHostName));
	m_Serializer.addBinding(new LoaderSaverImpl<MainForm,int,int>
		( "PortNumber",this,8200,&MainForm::portNumber,&MainForm::setPort));
	m_Serializer.addBinding(new LoaderSaverImpl<QLabel,QString,const QString& > 
		( "Header_text",Header,"",&QLabel::text,&QLabel::setText));
	ADDTOSERIALIZER(Header);
	ADDTOSERIALIZER(Discount);
	ADDTOSERIALIZER(tDiscount);
	ADDTOSERIALIZER(Quantity);
	ADDTOSERIALIZER(tQuantity);
	ADDTOSERIALIZER(Summa);
	ADDTOSERIALIZER(tSum);
	ADDTOSERIALIZER(Price);
	ADDTOSERIALIZER(tPrice);
	ADDTOSERIALIZER(BueName);
	ADDTOSERIALIZER(tBueName);
	ADDTOSERIALIZER(TDiscount);
	ADDTOSERIALIZER(Change);
	ADDTOSERIALIZER(tTotal);
	ADDTOSERIALIZER(tCache);
	ADDTOSERIALIZER(Total);
	ADDTOSERIALIZER(tChange);
	ADDTOSERIALIZER(Cache);
	m_Serializer.addBinding(new LoaderSaverImpl<QWidget,QColor,const QColor&,CRGetAccessor >
		( "Window_bkcolor",centralWidget(),QColor(0,0,0),&QWidget::paletteBackgroundColor,&QWidget::setPaletteBackgroundColor));

    int w, h;
    host ="localhost";
    port = 8200;
    termname = "20416041";
    
    delete statusBar();
    w = width();
    h = height();
    qWarning("w=%i h=%i tw=%i\n", w, h, BuesList->width());
//    BuesList->setColumnWidth(0, BuesList->width() - 330);
    BuesList->setColumnStretchable( 0, true );
    BuesList->setColumnWidth(1,70);
    BuesList->setColumnWidth(2,90);
    BuesList->setColumnWidth(3,70);
    socket = new QSocket( this );
    connect( socket, SIGNAL(connected()),
		SLOT(socketConnected()) );
    connect( socket, SIGNAL(connectionClosed()),
		SLOT(socketConnectionClosed()) );
    connect( socket, SIGNAL(readyRead()),
		SLOT(socketReadyRead()) );
    connect( socket, SIGNAL(error(int)),
		SLOT(socketError(int)) );

    // connect to the server
//    infoText->append( tr("Trying to connect to the server\n") );
//    socket->connectToHost( host, port );        
    startTimer(1000);
	reset();
	m_bChClosed=true;
}

void MainForm::closeConnection()
{
	socket->close();
	if ( socket->state() == QSocket::Closing ) {
	    // We have a delayed close.
	    connect( socket, SIGNAL(delayedCloseFinished()),
		    SLOT(socketClosed()) );
	} else {
	    // The socket is closed.
	    socketClosed();
	}
}

void MainForm::sendToServer()
{
	// write to the server
//	QTextStream os(socket);
//	os << inputText->text() << "\n";
//	inputText->setText( "" );
}

void MainForm::socketReadyRead()
{
    QString info, cmd, sname, name, name2;
    QString iKL, iBueN, iBueNC, iBarCode, iDep, iPrice, iQuantity, iSumma, iDiscount, iTotal;
    QString iCache, iChange;
	
//    QTextCodec *wcodec = QTextCodec::codecForName("cp1251");

    QTextCodec *dcodec = QTextCodec::codecForName("IBM 866");
//    QCString locallyEncoded = codec->fromUnicode( unicodeString );
 
	char buf[1024];
	// read from the server
//	Change->setText("read");
	int l = socket->readBlock( buf, sizeof( buf ) );
//	if ( l ) info = buf; else info = "no data";
//	info = socket->readLine();
//	BueName->setText(info);
  	if ( l ) {
		buf[ l ] =0;
		info = QString::fromUtf8( buf ).simplifyWhiteSpace();
//	while ( socket->canReadLine() ) {
//	    info = socket->readLine();//.section("\n",0,0);
	    printf("%s\n", (const char *) dcodec->fromUnicode( info ) );
	    cmd = info.section( " ",0,0 );
	    sname = info.section( " ",1,1 );
	    if (true) { // termname == sname ) {
		name = info.section( "|", 1, 1 );
		name2 = info.section( "|", 2, 2 );
		info = info.section( "|", 0, 0 );
		if ( cmd == "add" ){
			if (m_bChClosed)
			{
				reset();
				m_bChClosed=false;
			}
			iKL = info.section( " ",2,2 );
			iBueN = info.section( " ",3,3 );
			iBueNC = info.section( " ",4,4 );
			iBarCode = info.section( " ",5,5 );
			iDep = info.section( " ",6,6 );
			iPrice = info.section( " ",7,7 );
			iQuantity = info.section( " ",8,8 );
			iSumma = info.section( " ",9,9 );
			iDiscount = info.section( " ",10,10 );
			iTotal = info.section( " ",11,11 );
			BueName->setText( " "+name );
		 	Price->setText( " "+iPrice );
			Quantity->setText( " "+iQuantity  );
			Summa->setText( " "+iSumma );
		 	Discount->setText( " "+iDiscount );
		 	//Total->setText( " "+iTotal );
			double total=Total->text().stripWhiteSpace().toDouble();
			total+=Summa->text().stripWhiteSpace().toDouble();
			Total->setText(QString(" %1").arg(total,14,'f',2));
		    	Cache->setText("");
		    	Change->setText("");
		 	int row = BuesList->numRows();
		  	BuesList->setNumRows( row+1 );
		    	BuesList->setText( row, 0, name );
		    	BuesList->setText( row, 1, iPrice );
		    	BuesList->setText( row, 2, iQuantity );
		    	BuesList->setText( row, 3, iSumma );
		    	BuesList->selectRow( row );
		}
		if ( cmd == "reset" ) reset();
/*		{
			BueName->setText("");
		    	Price->setText("");
		    	Quantity->setText("");
		    	Summa->setText("");
		    	Discount->setText("");
		    	Total->setText("");
		    	Cache->setText("");
		    	Change->setText("");
		    	BuesList->setNumRows( 0 );
		}
*/
		if ( cmd == "close" ){
			double cashSum=info.section(" ",3,3).toDouble();
			if (cashSum!=0)
			{
				Cache->setText(QString(" %1").arg(cashSum,14,'f',2));
				double total=Total->text().stripWhiteSpace().toDouble();
				Change->setText(QString(" %1").arg(cashSum-total,14,'f',2));
			}
//			reset();
			m_bChClosed=true;
		}
		if ( cmd == "notfound" ){
			BueName->setText( iBarCode+tr(" - not found") );
			Price->setText("");
			Quantity->setText("");
			Summa->setText("");
			Discount->setText("");
		}
		if ( cmd == "itog" ){
			iTotal = info.section( " ",2,2 );
			iCache = iBueNC = info.section( " ",3,3 );
			iChange = iBarCode = info.section( " ",4,4 );
			Total->setText( " "+iTotal );
			Cache->setText( " "+iCache );
			Change->setText( " "+iChange );
		}
		if ( cmd == "discount" ){
			BueName->setText( name );
			TDiscount->setText( name2 );
		 	Price->setText( " " );
			Quantity->setText( " " );
			Summa->setText( " " );
		 	Discount->setText( " " );
		}
//		if ( !bueName.isNull() ) BueName->setText( buename );
	    }
	}
/*	
      char cmd[20]="",snm[20]="", kl[10]="", cbue[10]="", otd[3]="",
           bue[10]="", code[20]="", price[20]="", q[20]="",
           sum[20]="", disc[20]="", itog[20]="", nal[20]="", change[20]="";

      int pc, i;
      AnsiString S, Name="";
      S=Socket->ReceiveText();
      sscanf(S.c_str(),"%s %s",cmd, snm);
      if (strcmpi(snm, TermName)==0) {
          pc=S.Pos("|");
          if (pc>0) {
              Name=S.SubString(pc+1,S.Length());
          }
          if (strcmpi(cmd, "add")==0) {
              sscanf(S.c_str(),"%s %s %s %s %s %s %s %s %s %s %s %s",
              cmd, snm, kl, bue, cbue, code,
              otd, price, q, sum, disc, itog);
              tName->Caption=" "+Name;
              tPrice->Caption=price;
              tQuantity->Caption=q;
              tSum->Caption=sum;
              tItog->Caption=itog;
              tDiscount->Caption=disc;

              i=sgList->RowCount-1;
              sgList->Cells[0][i]=cbue;
              sgList->Cells[1][i]=Name;
              sgList->Cells[2][i]=price;
              sgList->Cells[3][i]=q;
              sgList->Cells[4][i]=sum;
              sgList->Cells[5][i]=disc;
              sgList->RowCount++;
              sgList->Row=i+1;
              sgList->Col=1;
              for (i=0;i<6;i++) sgList->Cells[i][sgList->RowCount-1]=" ";
              if (MPlayer->Mode==mpPlaying) MPlayer->Stop();
          }
          if (strcmpi(cmd, "reset")==0) {
              sgList->RowCount=2;
              tName->Caption=" ";
              tPrice->Caption=" ";
              tQuantity->Caption=" ";
              tSum->Caption=" ";
              tItog->Caption=" ";
              tNal->Caption=" ";
              tChange->Caption=" ";
              tDiscount->Caption=" ";
              for (i=0;i<6;i++) sgList->Cells[i][1]=" ";
              if (MPlayer->Mode==mpPlaying) MPlayer->Stop();
          }
          if (strcmpi(cmd, "closecheck")==0) {

          }
          if (strcmpi(cmd, "notfound")==0) {
              sscanf(S.c_str(),"%s %s %s",
              cmd, snm, code);
              S.printf("%s-не найден", code);
              tName->Caption=S;
              tPrice->Caption=" ";
              tQuantity->Caption=" ";
              tSum->Caption=" ";
              tDiscount->Caption=" ";
              if (FNotFound!="") {
                  MPlayer->FileName=FNotFound;
                  MPlayer->Open();
                  MPlayer->Rewind();
                  MPlayer->Play();
              }
          }
          if (strcmpi(cmd, "itog")==0) {
              sscanf(S.c_str(),"%s %s %s %s %s",
              cmd, snm, itog, nal, change);
              tItog->Caption=itog;
              tNal->Caption=nal;
              tChange->Caption=change;
          }
          if (strcmpi(cmd, "discount")==0) {
              tName->Caption=" "+Name;
              tPrice->Caption=" ";
              tQuantity->Caption=" ";
              tSum->Caption=" ";
              tDiscount->Caption=" ";
              if (MPlayer->Mode==mpPlaying) MPlayer->Stop();
          }
      }
*/	
    qWarning("w=%i h=%i tw=%i\n", width(), height(), BuesList->width());
    BuesList->setColumnWidth(0, BuesList->width() - 240);

}

void MainForm::socketConnected()
{
//	infoText->append( tr("Connected to server\n") );
}

void MainForm::socketConnectionClosed()
{
//	infoText->append( tr("Connection closed by the server\n") );
}

void MainForm::socketClosed()
{
//	infoText->append( tr("Connection closed\n") );
}

void MainForm::socketError( int  )
{
//	infoText->append( tr("Error number %1 occurred\n").arg(e) );
}

void MainForm::timerEvent( QTimerEvent * )
{
    if ( socket->state() == QSocket::Idle ) {
	printf(".\n");
	socket->connectToHost( host, port );        
    }
//	infoText->append( tr("Error number %1 occurred\n").arg(e) );
}


void MainForm::config()
{
	dConfig d;
	int res =  d.exec();
	if ( res ) {
	}
}


void MainForm::reset()
{
	BueName->setText(" ");
    	Price->setText(" ");
    	Quantity->setText("");
    	Summa->setText("");
    	Discount->setText("");
    	Total->setText("");
    	Cache->setText("");
    	Change->setText("");
    	BuesList->setNumRows( 0 );
}


void MainForm::mousePressEvent( QMouseEvent *e )
{
	if ( e->button() == RightButton )
	{
		QWidget *w = childAt( e->pos() );
		QColor color = w->backgroundColor();
		color = QColorDialog::getColor( color,
		this, "color dialog" );
		if ( color.isValid() ) {
			w->setBackgroundColor( color );
		}
	}
	else if ( e->button() == LeftButton )
	{
		QWidget *w = childAt( e->pos() );
		QColor color = w->paletteForegroundColor();
		color = QColorDialog::getColor( color,
		this, "color dialog" );
		if ( color.isValid() ) {
			w->setPaletteForegroundColor( color );
		}
	}
}

/*
void StringListEdit::setList(const QStringList &list)
{
    listBox->clear();
    listBox->insertStringList(list);
    QFontMetrics fm(listBox->font());
    int width = 0;
    for (int i = 0; i < (int)list.count(); ++i) {
    int w = fm.width(list[i]);
    if (w > width)
        width = w;
    }
    if (listBox->verticalScrollBar()) width += listBox->verticalScrollBar()->width();
    listBox->setMinimumWidth(QMIN(width, qApp->desktop()->screenGeometry().width() * 0.8));
    updateButtons();
}
*/



void MainForm::fixProductNameSize()
{
    BueName->setMinimumWidth(BueName->width());
    BueName->setMaximumWidth(BueName->width());
}




bool MainForm::eventFilter( QObject *, QEvent * pE )
{
    return (pE->type()==QEvent::LanguageChange);
}


void MainForm::setHostName( const QString & sHost)
{
    host=sHost;
}


QString MainForm::hostName() const
{
    return host;
}


void MainForm::setPort( int iPort )
{
	port=iPort;
}


int MainForm::portNumber() const
{
	return port;
}
