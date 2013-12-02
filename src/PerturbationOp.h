#ifndef _PERTURBATIONOP_H_
#define _PERTURBATIONOP_H_

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <Perturbator.h>
#include <SelectCtrl.h>
#include <Selectable.h>
#include <DragCtrl.h>
#include "DataModel.h"
using namespace UTILITY;
using namespace QGLVEXT;

namespace SIMULATOR{

  /**
   * @class PerturbationOp control the perturbate operation.
   * @see Utility/Perturbator.h
   */
  class Perturbation:public Selectable, public DragObserver,
					 public SelfRenderEle, public DragHook{
	
  public:
	Perturbation(pQGLViewerExt viewer,pDataModel dm):_viewer(viewer),_dataModel(dm){}
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
	
	// observer
	void selectDragEle(int nodeId){
	  _perturbator.setPerturbNodeId(nodeId);
	}
	void startDrag (double x,double y,double z){
	  _perturbator.setStartPosition(x,y,z);
	}
	void dragTo (double x,double y,double z){
	  _perturbator.dragTo(x,y,z);
	  double force[3];
	  _perturbator.getPerturbForce(force);
	  _dataModel->setForces(_perturbator.getPerturbNodeId(),force);
	  if(_viewer)
		_viewer->update();
	}
	void stopDrag (double x,double y,double z){
	  double force[3] = {0,0,0};
	  _dataModel->setForces(_perturbator.getPerturbNodeId(),force);
	  _perturbator.clear();
	}
	
	// render
	void draw()const{
	  /// @todo
	}

	// hook
	void getDragedPoint(double point[3])const{
	  assert(_dataModel);
	  assert(_dataModel->getVolMesh());
	  const VVec3d &nodes = _dataModel->getVolMesh()->nodes();
	  const int n = _perturbator.getPerturbNodeId();
	  assert_in(n,0,nodes.size()-1);
	  point[0] = nodes[n][0];
	  point[1] = nodes[n][1];
	  point[2] = nodes[n][2];
	}
	
  private:
	pQGLViewerExt _viewer;
	pDataModel _dataModel;
	Perturbator _perturbator;
  };
  typedef boost::shared_ptr<Perturbation> pPerturbation;

  class PerturbationCtrl:public QObject{

	Q_OBJECT
	
  public: 
	PerturbationCtrl(pQGLViewerExt view,pDataModel dm){

	  _perturb = pPerturbation(new Perturbation(view,dm));
	  _dragCtrl = pDragCtrl(new DragCtrl(view,_perturb));
	  _dragCtrl->setObserver(_perturb);
	  _dragCtrl->setDragHook(_perturb);
	  if (view != NULL){
		_perturb->setRenderPriority(FIRST_RENDER);
		view->addSelfRenderEle(_perturb);
	  }
	}

  private:
	pPerturbation _perturb;
	pDragCtrl _dragCtrl;
  };
  typedef boost::shared_ptr<PerturbationCtrl> pPerturbationCtrl;

}//end of namespace

#endif /*_PERTURBATIONOP_H_*/
