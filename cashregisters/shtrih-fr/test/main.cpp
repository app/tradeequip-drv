#include <shtrih-fr.h>
#include "mainform.h"
#include <qapplication.h>

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
#ifdef Q_OS_WIN32
  QString appPath=app.applicationDirPath();
  app.addLibraryPath(app.applicationDirPath());
#endif
	MainForm *mainForm;
	app.setMainWidget(mainForm=new MainForm());
  if (mainForm->success())
  {
	  mainForm->show();
	  return app.exec();
  }
  return 100;
}
