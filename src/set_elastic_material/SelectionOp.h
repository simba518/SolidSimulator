#ifndef _SELECTIONOP_H_
#define _SELECTIONOP_H_

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <SelectCtrl.h>
#include <Selectable.h>
#include "DataModel.h"
using namespace UTILITY;
using namespace QGLVEXT;

namespace SIMULATOR{
  
  class SelectNodes:public Selectable, public SelectObserver{
	
  public:
	SelectNodes(pDataModel dm):_dataModel(dm){
	  assert(dm);
	}
	int totalEleNum ()const{
	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  assert(tetmesh);
	  return tetmesh->nodes().size();
	}
	void drawWithNames ()const{
	  pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  assert(tetmesh);
	  glFlush();
	  for (int i=0; i<tetmesh->nodes().size(); i++){
		const Vector3d &v = tetmesh->nodes()[i];
		glPushName(i);
		glBegin(GL_POINTS);
		glVertex3d(v[0], v[1], v[2]);
		glEnd();
		glPopName();
	  }
	}
	void addSelection(const vector<int> &sel_ids){
	  _dataModel->addFixedNodes(sel_ids);
	}
	void removeSelection(const vector<int> &sel_ids){
	  _dataModel->removeFixedNodes(sel_ids);
	}
	
  private:
	pDataModel _dataModel;
  };
  typedef boost::shared_ptr<SelectNodes> pSelectNodes;

  class SelectTets:public Selectable, public SelectObserver{
	
  public:
	SelectTets(pDataModel dm):_dataModel(dm){
	  assert(dm);
	  _fixSelectedTets = false;
	}
	int totalEleNum ()const{
	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  assert(tetmesh);
	  return tetmesh->tets().size();
	}
	void drawWithNames ()const{

	  pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  assert(tetmesh);
	  const VVec3d& v = tetmesh->nodes();
	  const VVec4i& t = tetmesh->tets();
	  glFlush();
	  for (int i=0; i<t.size(); i++){
		glPushName(i);
		glBegin(GL_POINTS);
		for (int j = 0; j < 4; ++j){
		  const Vector3d& n = v[t[i][j]];
		  glVertex3d(n[0],n[1],n[2]);
		}
		glEnd();
		glPopName();
	  }
	}
	void addSelection(const vector<int> &sel_ids){
	  if (_fixSelectedTets){
		vector<int> s = sel_ids;
		_dataModel->getElasticMtlGroups().removeGroup(s);
		_dataModel->getElasticMtlGroups().addGroup(s,totalEleNum());
	  }else{
		_dataModel->getElasticMtlGroups().addGroup(sel_ids,totalEleNum());
	  }
	}
	void removeSelection(const vector<int> &sel_ids){
	  if (_fixSelectedTets){
		vector<int> s = sel_ids;
		_dataModel->getElasticMtlGroups().removeGroup(s);
		_dataModel->getElasticMtlGroups().removeGroup(s,totalEleNum());
	  }else{
		_dataModel->getElasticMtlGroups().removeGroup(sel_ids,totalEleNum());
	  }
	}
	void toggleFixSelectedTets(){
	  _fixSelectedTets = _fixSelectedTets ? false:true;
	}
	
  private:
	pDataModel _dataModel;
	bool _fixSelectedTets;
  };
  typedef boost::shared_ptr<SelectTets> pSelectTets;

  class SimSelectionCtrl: public QObject{

	Q_OBJECT
	
  public:
	SimSelectionCtrl(pQGLViewerExt viewer, pDataModel dm){
	  _selectNodes = pSelectNodes(new SelectNodes(dm));
	  _selectTets = pSelectTets(new SelectTets(dm));
	  _selCtrl = pSelectCtrl(new SelectCtrl(viewer,_selectNodes,10));
	  _selCtrl->setObserver(_selectNodes);
	}
	
  public slots:
	void selectNodes(){
	  _selCtrl->setSelector(_selectNodes);
	  _selCtrl->setObserver(_selectNodes);
	}
	void selectTets(){
	  _selCtrl->setSelector(_selectTets);
	  _selCtrl->setObserver(_selectTets);	  
	}
	void togglePrintSelEle(){ 
	  _selCtrl->togglePrintSelEle(); 
	}
	void toggleFixSelectedTets(){_selectTets->toggleFixSelectedTets();}

  private:
	pSelectCtrl _selCtrl;
	pSelectNodes _selectNodes;
	pSelectTets _selectTets;
  };
  typedef boost::shared_ptr<SimSelectionCtrl> pSimSelectionCtrl;
  
}//end of namespace

#endif /*_SELECTIONOP_H_*/
