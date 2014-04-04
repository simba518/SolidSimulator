#include "MainWindow.h"
#include "ManipulateOP.h"
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
  _viewer->show3DGrid();
  _viewer->startAnimation();

  // data model	  
  pTetMeshEmbeding embeding = pTetMeshEmbeding(new TetMeshEmbeding());
  _volObjCtrl = pVolObjCtrl(new VolObjCtrl(this,embeding));
  _dataModel = pDataModel(new DataModel(embeding));

  // render
  _renderCtrl = pDataModelRenderCtrl(new DataModelRenderCtrl(_viewer,_dataModel));

  // selection, drag
  _selCtrl = pSimSelectionCtrl(new SimSelectionCtrl(_viewer,_dataModel));
  _perturb = pPerturbationCtrl(new PerturbationCtrl(_viewer,_dataModel));

  pLocalframeManipulatoion mani = pLocalframeManipulatoion(new ManipulateOP(_viewer,_dataModel));
  manipulation_ctrl = pLocalframeManipulatoionCtrl(new LocalframeManipulatoionCtrl(_viewer, mani));
}

void MainWindow::createConnections(){
  
  connect(_mainwindow.actionLoadInitFile, SIGNAL(triggered()),this,SLOT(loadInitFile()));

  connect(_mainwindow.actionLoadObj, SIGNAL(triggered()), _volObjCtrl.get(), SLOT(loadObjMesh()));
  connect(_mainwindow.actionLoadVol, SIGNAL(triggered()), _volObjCtrl.get(), SLOT(loadVolMesh()));
  connect(_volObjCtrl.get(),SIGNAL(resetSceneMsg(double,double,double,double,double,double)),
		  _viewer,SLOT(resetSceneBoundBox(double,double,double,double,double,double)));
  connect(_mainwindow.actionPrepareSimulation,SIGNAL(triggered()),
		  _dataModel.get(),SLOT(prepareSimulation()));
  
  // selection
  connect(_mainwindow.actionSelectNodes, SIGNAL(triggered()), _selCtrl.get(), SLOT(selectNodes()));
  connect(_mainwindow.actionPrintSelected, SIGNAL(triggered()), _selCtrl.get(), SLOT(togglePrintSelEle()));

  // render
  connect(_mainwindow.actionObjMesh,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowObj())); 
  connect(_mainwindow.actionVolMesh,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowVol()));
  connect(_mainwindow.actionConNodes,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowConNodes())); 

  // simulation
  connect(_mainwindow.actionSimulate,SIGNAL(triggered()),_dataModel.get(),SLOT(simulate()));
  connect(_viewer,SIGNAL(updateAnimation()),_dataModel.get(),SLOT(simulate()));
  connect(_mainwindow.actionPauseSimulation,SIGNAL(triggered()),_viewer,SLOT(pauseAnimation()));
  connect(_mainwindow.actionReset,SIGNAL(triggered()),_dataModel.get(),SLOT(reset()));

  // record
  connect(_mainwindow.actionRecordDisp,SIGNAL(triggered()),_dataModel.get(),SLOT(toggleRecord()));
  connect(_mainwindow.actionClearRecordDisp,SIGNAL(triggered()),_dataModel.get(),SLOT(clearRecord()));
  connect(_mainwindow.actionSaveRecordDisp,SIGNAL(triggered()),this,SLOT(saveRecordDisp()));
  connect(_mainwindow.actionSaveRecordVTK,SIGNAL(triggered()),this,SLOT(saveRecordDispVTK()));

  connect(_mainwindow.actionSaveMtlAsVTK,SIGNAL(triggered()),this,SLOT(saveMtlAsVTK()));

}

void MainWindow::paserCommandLine(){

  QStringList cmdline_args = QCoreApplication::arguments();
  if( cmdline_args.size() >= 2 ){
	const string init_filename = cmdline_args.at(1).toStdString();
	loadInitFile(init_filename);
  }
}

void MainWindow::loadInitFile(const string filename){

  this->init_filename = filename;
  if(boost::filesystem::exists(filename)){

	DEBUG_LOG("begin to load vol_obj");
	bool succ = _volObjCtrl->initialize(filename);
	succ &= _dataModel->loadSetting(filename);
	_dataModel->prepareSimulation();

	JsonFilePaser jsonf;
	jsonf.open(filename);
	double con_penalty;
	jsonf.read("con_penalty", con_penalty, 1.0);
	_perturb->setPerturCompilance(con_penalty);
	INFO_LOG("con_penalty: "<<con_penalty );
	_fileDialog->warning(succ);

  }else{

	ERROR_LOG("file " << filename <<" is not exist!" );
	_fileDialog->warning(false);
  }
}

void MainWindow::loadInitFile(){

  const string filename = _fileDialog->load("ini");
  if(filename.size() >0) loadInitFile(filename);
}

void MainWindow::saveRecordDisp(){
  
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveRecord(fname));
}

void MainWindow::saveRecordDispVTK(){
  
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveRecordAsVTK(fname));
}

void MainWindow::saveMtlAsVTK(){

  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveMtlAsVTK(fname));
}

void MainWindow::startReplayOperations(){
  
  _viewer->startAnimation();
  _dataModel->setRecord(true);
}

void MainWindow::stopReplayOperations(){

  _viewer->stopAnimation();
  _dataModel->setRecord(false);
  saveRecordDispVTK();
}
