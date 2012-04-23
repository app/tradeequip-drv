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


void Form1::onDevEvent( int ev)
{
    if (ev==EVENT_READER_DATAARRIVED)
    {
    textEdit1->append(m_pS->text());
    }
    else
    {
    
    }
    m_pS->next();
}


void Form1::setScanner( TEReaderBase * pS )
{
    m_pS=pS;
}
