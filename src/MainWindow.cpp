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
  _renderCtrl = pDataModelRenderCtrl(new DataModelRenderCtrl(_viewer,_dataModel));

  // selection, drag
  _selCtrl = pSimSelectionCtrl(new SimSelectionCtrl(_viewer,_dataModel));
  _perturb = pPerturbationCtrl(new PerturbationCtrl(_viewer,_dataModel));

  // menu
  QActionGroup* selection_group = new QActionGroup(this);
  _mainwindow.actionSelectNodes->setCheckable(true);
  _mainwindow.actionSelectTets->setCheckable(true);
  _mainwindow.actionSelectNodes->setActionGroup(selection_group);
  _mainwindow.actionSelectTets->setActionGroup(selection_group);
  _mainwindow.actionSelectNodes->setChecked(true);
}

void MainWindow::createConnections(){
  
  connect(_mainwindow.actionLoadInitFile, SIGNAL(triggered()),this,SLOT(loadInitFile()));
  connect(_mainwindow.actionSaveConNodes, SIGNAL(triggered()),this,SLOT(saveConNodes()));
  connect(_mainwindow.actionLoadConNodes, SIGNAL(triggered()),this,SLOT(loadConNodes()));
  connect(_mainwindow.actionSaveMaterial, SIGNAL(triggered()),this, SLOT(saveElasticMaterial()));
  connect(_mainwindow.actionLoadMaterial, SIGNAL(triggered()),this, SLOT(loadElasticMaterial()));  

  connect(_mainwindow.actionLoadObj, SIGNAL(triggered()), _volObjCtrl.get(), SLOT(loadObjMesh()));
  connect(_mainwindow.actionLoadVol, SIGNAL(triggered()), _volObjCtrl.get(), SLOT(loadVolMesh()));
  connect(_volObjCtrl.get(),SIGNAL(resetSceneMsg(double,double,double,double,double,double)),
		  _viewer,SLOT(resetSceneBoundBox(double,double,double,double,double,double)));
  connect(_volObjCtrl.get(),SIGNAL(resetSceneMsg(double,double,double,double,double,double)),
		  _dataModel.get(),SLOT(resetMaterialGroup()));
  connect(_volObjCtrl.get(),SIGNAL(resetSceneMsg(double,double,double,double,double,double)),
		  _dataModel.get(),SLOT(prepareSimulation()));
  connect(_mainwindow.actionPrepareSimulation,SIGNAL(triggered()),
		  _dataModel.get(),SLOT(prepareSimulation()));
  
  // selection
  connect(_mainwindow.actionSelectTets, SIGNAL(triggered()), _selCtrl.get(), SLOT(selectTets()));
  connect(_mainwindow.actionSelectNodes, SIGNAL(triggered()), _selCtrl.get(), SLOT(selectNodes()));
  connect(_mainwindow.actionFixSelectedTets, SIGNAL(triggered()), _selCtrl.get(), SLOT(toggleFixSelectedTets()));
  connect(_mainwindow.actionPrintSelected, SIGNAL(triggered()), _selCtrl.get(), SLOT(togglePrintSelEle()));

  // render
  connect(_mainwindow.actionObjMesh,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowObj())); 
  connect(_mainwindow.actionVolMesh,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowVol()));
  connect(_mainwindow.actionConNodes,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowConNodes())); 
  connect(_mainwindow.actionTetGroups,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowTetGroups()));  
  connect(_mainwindow.actionMaterialGroups,SIGNAL(triggered()),_renderCtrl.get(),SLOT(toggleShowTetMaterials())); 

  // simulation
  connect(_mainwindow.actionSimulate,SIGNAL(triggered()),_dataModel.get(),SLOT(simulate()));
  connect(_viewer,SIGNAL(updateAnimation()),_dataModel.get(),SLOT(simulate()));
  connect(_mainwindow.actionPauseSimulation,SIGNAL(triggered()),_viewer,SLOT(pauseAnimation()));
  connect(_mainwindow.actionReset,SIGNAL(triggered()),_dataModel.get(),SLOT(reset()));

  // record
  connect(_mainwindow.actionRecord,SIGNAL(triggered()),_dataModel.get(),SLOT(toggleRecord()));
  connect(_mainwindow.actionSaveRecordZ,SIGNAL(triggered()),this,SLOT(saveRecordZ()));
  connect(_mainwindow.actionSaveEigenValues,SIGNAL(triggered()),this,SLOT(saveEigenValues()));
  connect(_mainwindow.actionSaveEigenVectors,SIGNAL(triggered()),this,SLOT(saveEigenVectors()));
  
}

void MainWindow::paserCommandLine(){

  QStringList cmdline_args = QCoreApplication::arguments();
  if(cmdline_args.size() >= 2 ){

	string init_filename = cmdline_args.at(1).toStdString();
	bool succ = false;
	if(boost::filesystem::exists(init_filename)){
	  succ = _volObjCtrl->initialize(init_filename);
	  succ &= _dataModel->loadSetting(init_filename);
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
	_fileDialog->warning(_dataModel->saveFixedNodes(fname));
}

void MainWindow::loadConNodes(){

  const string fname = _fileDialog->load();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->loadFixedNodes(fname));
}

void MainWindow::saveElasticMaterial(){
  
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveElasticMaterial(fname));
}

void MainWindow::loadElasticMaterial(){
  const string fname = _fileDialog->load();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->loadElasticMaterial(fname));
}

void MainWindow::saveRecordZ(){
  
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveRecordZ(fname));
}

void MainWindow::saveEigenValues(){
  
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveEigenValues(fname));
}

void MainWindow::saveEigenVectors(){
  
  const string fname = _fileDialog->save();
  if(fname.size() >0)
	_fileDialog->warning(_dataModel->saveEigenVectors(fname));
}
