#include <stdlib.h>
#include <QtGui/QApplication>
#include <Log.h>
#include <QInputEventRecorderWidget.h>
#include "MainWindow.h"

using namespace SIMULATOR;
using namespace UTILITY;

int main(int argc, char *argv[]){

  QApplication application(argc,argv);
  QMainWindow w;
  MainWindow m_mainw(&w);

  QInputEventRecorderWidget recorder;
  recorder.setObj(&w);

  recorder.show();
  m_mainw.show();

  return  application.exec();
}
