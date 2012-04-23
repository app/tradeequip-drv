/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qstatusbar.h>
#include <qapplication.h>
#include <tescalesbase.h>

extern QApplication *application;
extern TEScalesBase *unit;

void MainForm::init()
{

 delete statusBar();
 Price->setText( "0.00" );
 Weight->setText( "0.000" );
}

void MainForm::destroy()
{
// unit->stopPoll();
 delete unit;
}

void MainForm::SetPortOptions()
{
// unit->stopPoll();
 unit->close();
 unit->setPortNumber( ePortNum->value() );
 unit->open();
 connect( unit, SIGNAL( deviceEvent( int ) ), this, SLOT( display() ) );
// unit->startPoll(500);
}


void MainForm::GetNum()
{
 unit->readDisplay();
 display();
}


void MainForm::SetNum()
{
 blockSignals( true );
 unit->setPrice( Price->text().toDouble() );
 blockSignals( false );
 GetNum();
}



void MainForm::display()
{
        Weight->setText( QString::number( unit->weight(), 'f', 3 ) );
        Price->setText( QString::number( unit->price(), 'f', 2 ) );
        Summ->setText( QString::number( unit->summa(), 'f', 2 ) );
}
