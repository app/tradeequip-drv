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

#include <qbutton.h>
#include <qtextcodec.h>
#include <stdlib.h>

void Form1::init()
{
// QSocket::readLine() uses conversion from QByteArray to QString, so we must set codec.
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));
    connect(lineEdit1,SIGNAL(textChanged(const QString &)),this,SLOT(onLine1Changed(const QString &)));
    connect(lineEdit2,SIGNAL(textChanged(const QString &)),this,SLOT(onLine2Changed(const QString &)));
    connect(slider1,SIGNAL(valueChanged(int)),this,SIGNAL(brightnessChanged(int)));
    connect(pushButton1_2_2,SIGNAL(clicked()),this,SIGNAL(initDisplay()));
    sock = new QSocket( this );
    connect( sock, SIGNAL(connected()),
        SLOT(socketConnected()) );
    connect( sock, SIGNAL(connectionClosed()),
        SLOT(socketConnectionClosed()) );
    connect( sock, SIGNAL(readyRead()),
        SLOT(socketReadyRead()) );
    connect( sock, SIGNAL(error(int)),
        SLOT(socketError(int)) );

    // connect to the server
//    infoText->append( tr("Trying to connect to the server\n") );
    host=QString("192.168.1.217");
    port=8200;
    sock->connectToHost( host, port );        
    startTimer(2000);
    reset();
    lineEdit9->setText(host);    
    lineEdit9_2->setText(QString("%1").arg(port));
}

void Form1::onUpdateIntervalChanged( const QString & val)
{
    emit updateIntervalChanged(val.toInt());
}

void Form1::onLine1Changed( const QString & l1)
{
    emit textChanged(l1,checkBox1->state()==QButton::Off?QString("line1"):QString("full"),le1m->text());
}

void Form1::onLine2Changed( const QString & l2 )
{
    emit textChanged(l2,QString("line2"),le2m->text());
}


void Form1::onUploadPushed()
{
    QByteArray ba((lineEdit1_3->text().length()+1)/2);
    QString str=lineEdit1_3->text();
    bool ok;
    for(uint i=0;i<str.length();i+=2)
    {
    ba.data()[i/2]=str.mid(i,2).toUInt(&ok,16);
    };
    emit uploadChars(lineEdit1_2->text().toInt(),lineEdit1_2_2->text().toInt(),ba);
}


void Form1::onUploadGarbage()
{
    QByteArray ba(6*(128));
    for(int i=0;i<128;i++)
    {
    ba.data()[i*6]=5;
    for(int j=1;j<6;j++)
        ba.data()[i*6+j]=rand();
    }
    emit uploadChars(128,255,ba);
}

void Form1::onComSelChanged()
{
    emit comPortNumChanged(comboBox1->currentItem()+1);
}


void Form1::onComboBox2ValChanged()
{
    emit baudRateChanged(comboBox2->currentText().toInt());
}


void Form1::socketConnected()
{

}


void Form1::socketConnectionClosed()
{

}


void Form1::socketReadyRead()
{
    QString input;
    QString info, cmd, sname, name, name2;
    QString iKL, iBueN, iBueNC, iBarCode, iDep, iPrice, iQuantity, iSumma, iDiscount, iTotal;
    QString iCache, iChange;

    QTextCodec *dcodec = QTextCodec::codecForName("IBM 866");
 
//    char buf[1024];
    
//    int l = sock->readBlock( buf, sizeof( buf ) );
//    if ( l ) {
      while (sock->canReadLine())
      { 
//       buf[ l ] =0;
//       info = QString::fromUtf8( buf ).simplifyWhiteSpace();
        input=sock->readLine().simplifyWhiteSpace();
        info=input;
        printf("%s\n", (const char *) dcodec->fromUnicode( info ) );
        cmd = info.section( " ",0,0 );
        sname = info.section( " ",1,1 );
//        if ( termname == sname ) 
        {
        name = info.section( "|", 1, 1 );
        name2 = info.section( "|", 2, 2 );
        info = info.section( "|", 0, 0 );
        if ( cmd == "add" ){
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
            emit textChanged(name,"line1","");
            emit textChanged(tr("Price:")+iPrice+tr(" Qua:")+iQuantity+tr(" Sum:")+iSumma,"line2","");
//            Discount->setText( " "+iDiscount );
//            Total->setText( " "+iTotal );
  //              Cache->setText("");
    //            Change->setText("");
 //           int row = BuesList->numRows();
//            BuesList->setNumRows( row+1 );
//                BuesList->setText( row, 0, name );
//                BuesList->setText( row, 1, iPrice );
//                BuesList->setText( row, 2, iQuantity );
//                BuesList->setText( row, 3, iSumma );
//                BuesList->selectRow( row );
        }
        if ( cmd == "reset" ) reset();
        if ( cmd == "closecheck" ){
            
        }
        if ( cmd == "notfound" ){
            emit textChanged( iBarCode+tr(" - not found"), "full", "");
//            Price->setText("");
//            Quantity->setText("");
//            Summa->setText("");
//            Discount->setText("");
        }
        if ( cmd == "itog" ){
/*      
       iTotal = info.section( " ",2,2 );
            iCache = iBueNC = info.section( " ",3,3 );
            iChange = iBarCode = info.section( " ",4,4 );
            Total->setText( " "+iTotal );
            Cache->setText( " "+iCache );
            Change->setText( " "+iChange );
*/    
        }
        if ( cmd == "discount" ){
/*      
            BueName->setText( name );
            TDiscount->setText( name2 );
            Price->setText( " " );
            Quantity->setText( " " );
            Summa->setText( " " );
            Discount->setText( " " );
*/      
        }
        if (cmd=="show")
        {
           QString sname=input.section(" ",1);
           QString text=sname.section("|",0,0); 
           QString pos=sname.section("|",1,1); 
           QString mode=sname.section("|",2,2);
           emit textChanged(text,pos,mode);
        };
//      if ( !bueName.isNull() ) BueName->setText( buename );
        }
    }
 //   qWarning("w=%i h=%i tw=%i\n", width(), height(), BuesList->width());
 //   BuesList->setColumnWidth(0, BuesList->width() - 240);
}


void Form1::socketError( int )
{

}


void Form1::timerEvent( QTimerEvent * )
{
    if ( sock->state() == QSocket::Idle ) {
    sock->connectToHost( host, port );        
    }    
}


void Form1::reset()
{
    emit textChanged("","line2","");
    emit textChanged(tr("Welcome to our shop!"),"full","");
}


void Form1::onServerAddressChanged()
{
    sock->clearPendingData();
    sock->close();
    host=lineEdit9->text();
    port=lineEdit9_2->text().toInt();
    sock->connectToHost(host,port);
}
