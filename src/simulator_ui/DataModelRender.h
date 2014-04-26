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
							   CON_NODES=4,
							   REST_VOL=8,
							   REST_OBJ = 16,
							   GROUND = 32,
							   SCENE = 64};
  
  /**
   * @class DataModelRender render all the data in DataModel.
   * 
   */
  class DataModelRender:public SelfRenderEle{
	
  public:
	DataModelRender(pQGLViewerExt viewer,pDataModel dm):_viewer(viewer),_dataModel(dm){
	  assert(dm);
	  assert(viewer);
	  _text = pTextForRender(new TextForRender());
	  tran.setZero();
	  rot_axi << 1.0f,0.0f,0.0f;
	  angle = 0.0f;
	}
	void render(){
	  if (_dataModel){
		_text->update(_dataModel->simulatorName(),40,40);
		const string msg = string("recorded ")+TOSTR(_dataModel->getRecord().size());
		_text->update(msg,40,80);
	  }else{
		_text->update("no data model",40,40);
	  }
	  if (_viewer){
		const string msg = _viewer->animationIsStarted() ? "paly":"stop";
		_text->update(msg,40,120);
	  }
	  this->draw();
	}
	void draw()const{
	  
	  if (_renderType & OBJ) drawObjMesh();
	  if (_renderType & VOL) drawVolMesh();
	  if (_renderType & CON_NODES) drawConstraints();
	  if (_renderType & REST_VOL) drawRestVolMesh();
	  if (_renderType & REST_OBJ) drawRestObjMesh();
	  if (_renderType & GROUND)	drawGround();
	  if (_renderType & SCENE)	drawScene();
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
	void setGround(const Vector3f &tran, const Vector3f &rot_axi, const double &angle){
	  this->tran = tran;
	  this->rot_axi = rot_axi;
	  this->angle = angle;
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
	void drawRestVolMesh()const{
	  const pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  if (tetmesh) UTILITY::draw(tetmesh);
	}
	void drawRestObjMesh()const{
	  const pObjmesh_const objmesh = _dataModel->getRestObjMesh();
	  if(objmesh) UTILITY::draw(objmesh);
	}
	void drawScene()const{
	  const pObjmesh_const objmesh = _dataModel->getScene();
	  if(objmesh) UTILITY::draw(objmesh);
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
	void drawGround()const{
	  if (_viewer){
		glColor3f(0.1,0.1,0.1);
		glLineWidth(1.0f);
		glPushMatrix();
		glTranslatef(tran[0], tran[1], tran[2]);
		glRotatef(angle, rot_axi[0], rot_axi[1], rot_axi[2]);
		QGLViewer::drawGrid();
		glPopMatrix();
	  }
	}
	
  private:
	VolNodeGroupRender node_group_render;
	pQGLViewerExt _viewer;
	pDataModel _dataModel;
	pTextForRender _text;
	int _renderType;

	// ground
	Vector3f tran, rot_axi;
	double angle;
  };
  typedef boost::shared_ptr<DataModelRender> pDataModelRender;

  class DataModelRenderCtrl: public QObject{
	
	Q_OBJECT
	
  public:
	DataModelRenderCtrl(pQGLViewerExt viewer,pDataModel dm):_viewer(viewer){
  
	  _modelRender=pDataModelRender(new DataModelRender(viewer,dm));
	  _modelRender->setRenderType(OBJ|VOL|CON_NODES|SCENE);
	  if(_viewer != NULL){
		_viewer->addSelfRenderEle(_modelRender);
		_viewer->addTextForRender(_modelRender->getTextForRender());
	  }
	}
	void setGround(const Vector3f &tran, const Vector3f &rot_axi, const double &angle){
	  _modelRender->setGround(tran, rot_axi, angle);
	}
	
  public slots:
	void toggleShowObj(){ toggleShowType(OBJ); }
	void toggleShowVol(){ toggleShowType(VOL); }
	void toggleShowConNodes(){ toggleShowType(CON_NODES); }
	void toggleShowRestVol(){ toggleShowType(REST_VOL); }
	void toggleShowRestObj(){ toggleShowType(REST_OBJ); }
	void toggleShowGround(){ toggleShowType(GROUND); }
	void showGround(const bool show){showType(show, GROUND);}

  protected:
	void showType(bool show, DATA_MODEL_RENDER_TYPE type){
	  if(show && _modelRender != NULL)
		_modelRender->addRenderType(type);
	  else if (_modelRender != NULL)
		_modelRender->removeRenderType(type);
	  if (_viewer != NULL) _viewer->update();
	}
	void toggleShowType(DATA_MODEL_RENDER_TYPE type){
	  if (_modelRender != NULL)
		_modelRender->toggleRenderType(type);
	  if (_viewer != NULL) _viewer->update();
	}
	
  private:
	pDataModelRender _modelRender;
	pQGLViewerExt _viewer;
	
  };
  typedef boost::shared_ptr<DataModelRenderCtrl> pDataModelRenderCtrl;
  
  
}//end of namespace

#endif /*_DATAMODELRENDER_H_*/
