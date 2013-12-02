#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <FileDialog.h>
#include <QGLViewerExt.h>
#include <VolObjCtrl.h>
#include "DataModel.h"
#include "DataModelRender.h"
#include "DragOp.h"
#include "SelectionOp.h"
#include <boost/shared_ptr.hpp>
using namespace QGLVEXT;
#include <src/ui_simulator.h>

namespace SIMULATOR{
  
  class MainWindow: public QMainWindow{
	
	Q_OBJECT
	
  public:
	MainWindow(QWidget *parent=0,Qt::WFlags flags=0);
	void createComponents();
	void createConnections();
	void paserCommandLine();

  public slots:
	void loadInitFile();
	void saveConNodes();
	void loadConNodes();
	
  private:
	Ui::MainWindow _mainwindow;
	pQGLViewerExt _viewer;
	pFileDialog _fileDialog;

	pDataModel _dataModel;
	pVolObjCtrl _volObjCtrl;
	pSimSelectionCtrl _selCtrl;
	pDataModelRenderCtrl _renderCtrl;
  };
  
  typedef boost::shared_ptr<MainWindow> pMainWindow;
  
}//end of namespace

#endif /*_MAINWINDOW_H_*/

