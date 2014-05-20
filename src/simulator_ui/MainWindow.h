#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <FileDialog.h>
#include <QGLViewerExt.h>
#include <VolObjCtrl.h>
#include <QInputEventRecorder.h>
#include "PerturbationOp.h"
#include "DataModel.h"
#include "DataModelRender.h"
#include "SelectionOp.h"
#include "DragNodeOp.h"
#include "Manipulatoion.h"
#include <boost/shared_ptr.hpp>
using namespace QGLVEXT;
#include "ui_simulator.h"

namespace SIMULATOR{
  
  class MainWindow: public QMainWindow, public QInputEventRecorderObserver{
	
	Q_OBJECT
	
  public:
	MainWindow(QWidget *parent=0,Qt::WFlags flags=0);
	void createComponents();
	void createConnections();
	void paserCommandLine();
	void loadInitFile(const string filename);

  public:
	void startReplayOperations();
	void stopReplayOperations();

  protected slots:
	void loadInitFile();
	void saveRecordDisp();
	void saveRecordDispVTK();
	void saveMtlAsVTK();
	void saveVolMesh();
	void saveConNodes();

  private:
	Ui::MainWindow _mainwindow;
	pQGLViewerExt _viewer;
	pFileDialog _fileDialog;
	pPerturbationCtrl _perturb;

	pLocalframeManipulatoionCtrl manipulation_ctrl;
	pDataModel _dataModel;
	pVolObjCtrl _volObjCtrl;
	pSimSelectionCtrl _selCtrl;
	pDataModelRenderCtrl _renderCtrl;

	pLocalframeManipulatoionCtrl manipulation_passive_obj;
	pPassiveObject _passiveObject;

	string init_filename;
  };
  
  typedef boost::shared_ptr<MainWindow> pMainWindow;
  
}//end of namespace

#endif /*_MAINWINDOW_H_*/

