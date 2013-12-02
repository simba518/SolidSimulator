#ifndef _DRAGOP_H_
#define _DRAGOP_H_

#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <QGLViewerExt.h>
#include <SelectCtrl.h>
#include <DragCtrl.h>
#include <Selectable.h>
#include "DataModel.h"
using namespace Eigen;
using namespace UTILITY;
using namespace QGLVEXT;

namespace SIMULATOR{
  
  class DragOp:public Selectable, public DragObserver, 
			   public SelfRenderEle, public DragHook{
	
  public:
	DragOp(pQGLViewerExt viewer,pDataModel dm){
	  
	}

	// drag
	int totalEleNum ()const{
	  ///@todo
	}
	void drawWithNames ()const{
	  ///@todo
	}
	void prepareSelection(){
	  draggedGroupId = -1;
	}
	void getDragedPoint(double point[3])const{
	  /// @todo
	}
	
	// observer
	void selectDragEle(int sel_group_id){
	  draggedGroupId = sel_group_id;
	}
	void dragTo (double x,double y,double z){
	  /// @todo
	}
	void stopDrag (double x,double y,double z){
	  draggedGroupId = -1;
	}

	// render
	void draw()const{
	  /// @todo
	}
	
  protected:
	bool hasDraggedGroup()const{
	  return draggedGroupId >= 0;
	}
	
  private:
	pQGLViewerExt viewer;
	pDataModel data_model;
	int draggedGroupId;
	Vector3d initial_dragged_point; // start position of the dragged point.
	Matrix<double,3,-1> initial_displacement; // initial displacements of the dragged group.
  };
  
  typedef boost::shared_ptr<DragOp> pDragOp;
  
}//end of namespace

#endif /*_DRAGOP_H_*/
