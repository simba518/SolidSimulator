#ifndef _DATAMODELRENDER_H_
#define _DATAMODELRENDER_H_

#include <boost/shared_ptr.hpp>
#include <SelfRenderEle.h>
#include <MeshRender.h>
#include <VolNodeGroupRender.h>
#include "DataModel.h"
using namespace UTILITY;
using namespace QGLVEXT;

namespace SIMULATOR{
  
  /**
   * @class DataModelRender render all the data in DataModel.
   * 
   */
  class DataModelRender:public SelfRenderEle{
	
  public:
	DataModelRender(pDataModel dm):_dataModel(dm){
	  assert(dm);
	}
	void draw()const{
	  drawMesh();
	  drawConstraints();
	  drawTetGroups();
	}

  protected:
	void drawMesh()const{
	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  if(tetmesh) UTILITY::draw(tetmesh);
	}
	void drawConstraints()const{
	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  const PartialConstraints &posCon = _dataModel->getPosConstraints();
	  node_group_render.draw(tetmesh,posCon.getConNodesSet(),NULL,UTILITY::DRAW_POINT);
	}
	void drawTetGroups()const{
	  /// @todo
	}
	
  private:
	VolNodeGroupRender node_group_render;
	pDataModel _dataModel;
  };
  
  typedef boost::shared_ptr<DataModelRender> pDataModelRender;
  
}//end of namespace

#endif /*_DATAMODELRENDER_H_*/
