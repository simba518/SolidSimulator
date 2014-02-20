#ifndef _DATAMODELRENDER_H_
#define _DATAMODELRENDER_H_

#include <boost/shared_ptr.hpp>
#include <SelfRenderEle.h>
#include <MeshRender.h>
#include <VolNodeGroupRender.h>
#include <QObject>
#include "DataModel.h"
#include <QGLViewerExt.h>
#include <GL/gl.h>
using namespace UTILITY;
using namespace QGLVEXT;

namespace SIMULATOR{

  enum DATA_MODEL_RENDER_TYPE {NOTHING=0,
							   OBJ=1,
							   VOL=2,
							   CON_NODES=4};
  
  /**
   * @class DataModelRender render all the data in DataModel.
   * 
   */
  class DataModelRender:public SelfRenderEle{
	
  public:
	DataModelRender(pDataModel dm):_dataModel(dm){
	  assert(dm);
	  _text = pTextForRender(new TextForRender());
	}
	void render(){
	  if (_dataModel){
		_text->update(_dataModel->simulatorName(),40,40);
	  }else{
		_text->update("no data model",40,40);
	  }
	  this->draw();
	}
	void draw()const{
	  
	  if (_renderType & OBJ){
		drawObjMesh();
	  }
	  if (_renderType & VOL){
		drawVolMesh();
	  } 
	  if (_renderType & CON_NODES){
		drawConstraints();
	  }
	}
	pTextForRender getTextForRender()const{
	  return _text;
	}

	void setRenderType(const int type){
	  _renderType = ( type >= 0 ? type : NOTHING );
	}
	void addRenderType(const DATA_MODEL_RENDER_TYPE type){
	  _renderType = _renderType | type;
	}
	void removeRenderType(const DATA_MODEL_RENDER_TYPE type){
	  _renderType = _renderType & (~type);
	}
	void toggleRenderType(const DATA_MODEL_RENDER_TYPE type){
	  includeRenderType(type) ? removeRenderType(type) : addRenderType(type);
	}
	bool includeRenderType(const DATA_MODEL_RENDER_TYPE type)const{
	  return (_renderType&type) != NOTHING;
	}
	const int getRenderType()const{
	  return _renderType;
	}

  protected:
	void drawObjMesh()const{
	  const pObjmesh_const objmesh = _dataModel->getObjMesh();
	  if(objmesh) UTILITY::draw(objmesh);
	}
	void drawVolMesh()const{
	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  if(tetmesh){
		const VectorXd &u = _dataModel->getU();
		if(u.size() > 0){
		  assert_eq(u.size(),tetmesh->nodes().size()*3);
		  UTILITY::draw(tetmesh,&u[0]);
		}else{
		  UTILITY::draw(tetmesh);
		}
	  }
	}
	void drawConstraints()const{

	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  if (tetmesh){
		const vector<set<int> > &nodes = _dataModel->getConNodes();
		const VectorXd &vol_u = _dataModel->getU();
		glPointSize(20.0f);
		if (vol_u.size() >= 3)
		  node_group_render.draw(tetmesh, nodes,&vol_u[0],UTILITY::DRAW_POINT);
	  }
	}
	
  private:
	VolNodeGroupRender node_group_render;
	pDataModel _dataModel;
	pTextForRender _text;
	int _renderType;
  };
  typedef boost::shared_ptr<DataModelRender> pDataModelRender;

  class DataModelRenderCtrl: public QObject{
	
	Q_OBJECT
	
  public:
	DataModelRenderCtrl(pQGLViewerExt viewer,pDataModel dm):_viewer(viewer){
  
	  _modelRender=pDataModelRender(new DataModelRender(dm));
	  _modelRender->setRenderType(OBJ|VOL|CON_NODES);
	  if(_viewer != NULL){
		_viewer->addSelfRenderEle(_modelRender);
		_viewer->addTextForRender(_modelRender->getTextForRender());
	  }
	}
	
  public slots:
	void toggleShowObj(){
	  if (_modelRender != NULL)
		_modelRender->toggleRenderType(OBJ);
	  if (_viewer != NULL) _viewer->update();
	}
	void toggleShowVol(){
	  if (_modelRender != NULL)
		_modelRender->toggleRenderType(VOL);
	  if (_viewer != NULL) _viewer->update();
	}
	void toggleShowConNodes(){
	  if (_modelRender != NULL)
		_modelRender->toggleRenderType(CON_NODES);
	  if (_viewer != NULL) _viewer->update();
	}

  protected:
	void showType(bool show, DATA_MODEL_RENDER_TYPE type){
	  if(show && _modelRender != NULL){
		_modelRender->addRenderType(type);
	  }else if (_modelRender != NULL){
		_modelRender->removeRenderType(type);
	  }
	  if (_viewer != NULL) _viewer->update();
	}
	
  private:
	pDataModelRender _modelRender;
	pQGLViewerExt _viewer;
	
  };
  typedef boost::shared_ptr<DataModelRenderCtrl> pDataModelRenderCtrl;
  
  
}//end of namespace

#endif /*_DATAMODELRENDER_H_*/
