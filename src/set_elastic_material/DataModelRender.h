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
							   TET_GROUPS=8,
							   TET_MATERIAL=16};
  
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
		const string msg = string("recorded z ")+TOSTR(_dataModel->getRecoredZ().size());
		_text->update(msg,40,40);
	  }else{
		_text->update("recorded z 0",10,10);
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
	  if (_renderType & TET_GROUPS){
		drawTetGroups();
	  }
	  if (_renderType & TET_MATERIAL){
		drawTetMaterial();
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
	  const set<int> &nodes = _dataModel->getFixedNodes();
	  glPointSize(20.0f);
	  node_group_render.draw(tetmesh,nodes,NULL,UTILITY::DRAW_POINT);
	}
	void drawTetGroups()const{

	  pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  assert(tetmesh);
	  const VVec3d& v = tetmesh->nodes();
	  const VVec4i& t = tetmesh->tets();

	  const ElasticMtlGroups &mtlGroup = _dataModel->getElasticMtlGroups();
	  const vector<set<int> > &g = mtlGroup.groups();

	  glDisable(GL_LIGHTING);
	  glPointSize(20.0f);
	  for (size_t i = 0; i < g.size(); ++i){
		double c[3];
		mtlGroup.getGroupColor(i,c);
		glColor3d(c[0],c[1],c[2]);
		glBegin(GL_POINTS);
		BOOST_FOREACH(const int vi, g[i]){
		  assert_in(vi,0,t.size()-1);
		  for (int j = 0; j < 4; ++j){
			const Vector3d& n = v[t[vi][j]];
			glVertex3d(n[0],n[1],n[2]);
		  }
		}
		glEnd();
	  }
	}
	void drawTetMaterial()const{

	  pTetMesh_const tetmesh = _dataModel->getVolMesh();
	  assert(tetmesh);
	  const VVec3d& v = tetmesh->nodes();
	  const VVec4i& t = tetmesh->tets();

	  const ElasticMtlGroups &mtlGroup = _dataModel->getElasticMtlGroups();
	  const vector<set<int> > &g = mtlGroup.groups();
	  vector<double> colors;
	  mtlGroup.getMtlColors(colors);
	  assert_eq(colors.size(),g.size()*3);

	  glDisable(GL_LIGHTING);
	  glPointSize(8.0f);
	  for (size_t i = 0; i < g.size(); ++i){
		
		glColor3d(colors[i*3+0],colors[i*3+1],colors[i*3+2]);
		glBegin(GL_POINTS);
		BOOST_FOREACH(const int vi, g[i]){
		  assert_in(vi,0,t.size()-1);
		  const Vector3d n = (v[t[vi][0]]+v[t[vi][1]]+v[t[vi][2]]+v[t[vi][3]])/4.0f;
		  glVertex3d(n[0],n[1],n[2]);
		}		
		glEnd();
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
	void toggleShowTetGroups(){
	  if (_modelRender != NULL)
		_modelRender->toggleRenderType(TET_GROUPS);
	  if (_viewer != NULL) _viewer->update();
	}
	void toggleShowTetMaterials(){
	  if (_modelRender != NULL)
		_modelRender->toggleRenderType(TET_MATERIAL);
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
