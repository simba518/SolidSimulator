#include <QtGui/QApplication>
#include <QInputEventRecorderWidget.h>
#include "MainWindow.h"

using namespace SIMULATOR;
using namespace UTILITY;

int main(int argc, char *argv[]){

  QApplication application(argc,argv);
  QMainWindow w;
  MainWindow m_mainw(&w);

  QInputEventRecorderWidget recorder(&w,&m_mainw);
  recorder.move(900,200);
  recorder.show();

  m_mainw.show();
  return  application.exec();
}
