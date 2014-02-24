#ifndef _MANIPULATEOP_H_
#define _MANIPULATEOP_H_

#include <QGLViewerExt.h>
#include <SelectCtrl.h>
#include <Manipulatoion.h>
#include <Selectable.h>
#include <DragGroupSelDraw.h>
#include "DataModel.h"
using namespace Eigen;
using namespace UTILITY;
using namespace QGLVEXT;

namespace SIMULATOR{
  
  /**
   * @class ManipulateOP
   * 
   */
  class ManipulateOP:public LocalframeManipulatoionExt{
	
  public:
	ManipulateOP(QGLViewer *v,pDataModel dm):
	  LocalframeManipulatoionExt(v),data_model(dm){
	  dragged_group_id = -1;
	}

	// select
	int totalEleNum()const{
	  return data_model != NULL ? data_model->getConNodes().size() : 0;
	}
	void drawWithNames()const{
	  pTetMesh_const vol_mesh = data_model->getVolMesh();
	  if( vol_mesh != NULL ){
		const vector<set<int> > con_groups = data_model->getConNodes();
		const VectorXd &vol_u = data_model->getU();
		DragGroupSelDraw::drawAllGroupsWithPoints(vol_mesh,con_groups,&vol_u[0]);
	  }
	}
	void select(const vector<int> &sel_ids){
	  if(sel_ids.size()>0){
		assert_in(sel_ids[0],0,totalEleNum());
		dragged_group_id = sel_ids[0];
		manipulated_pos = data_model->getXc(sel_ids[0]);
		assert_gt(manipulated_pos.size(),0);
	  }
	}

	Matrix<double,3,-1> &getCurrentPositions(){
	  return manipulated_pos;
	}
	const Matrix<double,3,-1> &getCurrentPositions()const{
	  return manipulated_pos;
	}
	void applyTransform(){
	  LocalframeManipulatoionExt::applyTransform();
	  if(data_model){
		assert_ge(dragged_group_id,0);
		data_model->updateXc(manipulated_pos,dragged_group_id);
	  }
	}

  private:
	pDataModel data_model;
	Matrix<double,3,-1> manipulated_pos;
	int dragged_group_id;
  };
  typedef boost::shared_ptr<ManipulateOP> pManipulateOP;
  
}//end of namespace

#endif /*_MANIPULATEOP_H_*/
