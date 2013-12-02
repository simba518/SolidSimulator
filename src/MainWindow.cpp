#include "MainWindow.h"
using namespace SIMULATOR;

MainWindow::MainWindow(QWidget *parent,Qt::WFlags flags):QMainWindow(parent,flags){
	  
  createComponents();
  createConnections();
  paserCommandLine();
}

void MainWindow::createComponents(){

  _mainwindow.setupUi(this);
  _viewer = _mainwindow.left_view;
  _fileDialog = pFileDialog(new FileDialog(this));	  

  // data model	  
  pTetMeshEmbeding embeding = pTetMeshEmbeding(new TetMeshEmbeding());
  _volObjCtrl = pVolObjCtrl(new VolObjCtrl(this,embeding));
  _dataModel = pDataModel(new DataModel(embeding));

  // render
  pDataModelRender _dataModelRender = pDataModelRender(new DataModelRender(_dataModel));
  _viewer->addSelfRenderEle(_dataModelRender);

  // selection
  _selCtrl = pSimSelectionCtrl(new SimSelectionCtrl(_viewer,_dataModel));
  
  // drag
  pDragOp drag = pDragOp(new DragOp(_viewer,_dataModel));
  pDragCtrl dragCtrl = pDragCtrl( new DragCtrl(_viewer,drag) );
  dragCtrl->setObserver(drag);
  dragCtrl->setDragHook(drag);
  _viewer->addSelfRenderEle(drag);
}

void MainWindow::createConnections(){
  
  connect(_mainwindow.actionLoadInitFile, SIGNAL(triggered()),this,SLOT(loadInitFile()));
  connect(_mainwindow.actionLoadObj, SIGNAL(triggered()), _volObjCtrl.get(), SLOT(loadObjMesh()));
  connect(_mainwindow.actionLoadVol, SIGNAL(triggered()), _volObjCtrl.get(), SLOT(loadVolMesh()));
  connect(_mainwindow.actionSaveConNodes, SIGNAL(triggered()),this,SLOT(saveConNodes()));
  connect(_mainwindow.actionLoadConNodes, SIGNAL(triggered()),this,SLOT(loadConNodes()));
  connect(_volObjCtrl.get(),SIGNAL(resetSceneMsg(double,double,double,double,double,double)),
		  _viewer,SLOT(resetSceneBoundBox(double,double,double,double,double,double)));

  connect(_mainwindow.actionSimulate,SIGNAL(triggered()),_dataModel.get(),SLOT(simulate()));
}

void MainWindow::paserCommandLine(){

  QStringList cmdline_args = QCoreApplication::arguments();
  if(cmdline_args.size() >= 2 ){

	string init_filename = cmdline_args.at(1).toStdString();
	bool succ = false;
	if(boost::filesystem::exists(init_filename)){
	  succ = _dataModel->loadSetting(init_filename) >= 0;
	  if(succ){
		INFO_LOG("success to load initfile from  " << init_filename);
	  }else{
		ERROR_LOG("failed to load initfile from  "<< init_filename);
	  }	  
	}else{
	  ERROR_LOG("file " << init_filename <<" is not exist!" );
	}
  }
}

void MainWindow::loadInitFile(){

  const string filename = _fileDialog->load("ini");
  if(filename.size() >0)
	_fileDialog->warning(_dataModel->loadSetting(filename));
}

void MainWindow::saveConNodes(){
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveConNodes(fname));
}

void MainWindow::loadConNodes(){
  const string fname = _fileDialog->load();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->loadConNodes(fname));
}
