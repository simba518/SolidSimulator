#include "MathBasic.h"
#include "MakeMesh.h"
#include "IO.h"

#include <boost/filesystem/operations.hpp>
#include <vector>

USE_PRJ_NAMESPACE

//solving the distance QP problem in 3D, the formulation is:
//	min_{curr}	\|v-v0\|^2
//	s.t.		\forall i, v is in front of plane p_i
//
//you should provide:
//an initial guess:	v
//the active set:	aSet
//
//here the initial guess must be feasible or you should call:
//findFeasible(p,v)
FORCE_INLINE scalarD dist(const Vec4d& p,const Vec3d& v){return v.dot(p.block<3,1>(0,0))+p[3];}
FORCE_INLINE bool isFeasible(const std::vector<Vec4d>& p,const Vec3d& v)
{
	sizeType nrP=(sizeType)p.size();
	for(sizeType i=0;i<nrP;i++)
		if(dist(p[i],v) < 0.0f)
			return false;
	return true;
}
FORCE_INLINE bool findFeasible(const std::vector<Vec4d>& p,Vec3d& v)
{
	sizeType nrP=(sizeType)p.size();
	std::vector<scalarD> weight(nrP,1.0f);
	for(sizeType iter=0;iter<100*nrP;iter++)
	{
		Mat3d H=Mat3d::Zero();
		Vec3d G=Vec3d::Zero();
		for(sizeType i=0;i<nrP;i++)
		{
			scalarD E=weight[i]*std::exp(-dist(p[i],v));
			H+=p[i].block<3,1>(0,0)*p[i].block<3,1>(0,0).transpose()*E;
			G-=p[i].block<3,1>(0,0)*E;
		}
		if(std::abs(H.determinant()) < 1E-9f)
			H.diagonal().array()+=1E-9f;
		v-=H.inverse()*G;

		scalarD minDist=0.0f;
		sizeType minId=-1;
		for(sizeType i=0;i<nrP;i++)
		{
			scalarD currDist=dist(p[i],v);
			if(currDist < minDist)
			{
				minDist=currDist;
				minId=i;
			}
		}
		if(minId == -1)break;
		weight[minId]*=2.0f;
	}
	return isFeasible(p,v);
}
FORCE_INLINE bool findClosestPoint(const std::vector<Vec4d>& p,const Vec3d& v0,Vec3d& v,Vec3i& aSet,scalarD eps=1E-18)
{
	//rearrange
	char nrA=0;
	sizeType nrP=(sizeType)p.size();
	vector<bool> aTag(nrP,false);
	for(char d=0;d<3;d++)
		if(aSet[d] != -1)
		{
			aTag[aSet[d]]=true;
			aSet[nrA++]=aSet[d];
		}

	//forward decl for mainIter
	sizeType minA;
	scalarD minLambda,alphaK,nDotDir,distP;

	Mat2d M2;
	Mat3d A,M3;
	Vec3d dir,lambda;
	while(true)
	{
		//step 1: solve the following equation:
		// I*v + A^T*\lambda = v0
		// A*x + d = 0
		//where we use schur complementary method
		for(char d=0;d<nrA;d++)
			lambda[d]=dist(p[aSet[d]],v0);
		if(nrA == 0){
			dir=v0;
			dir-=v;
		}else if(nrA == 1){
			//the plane's normal has already been normalized
			dir=v0-p[aSet[0]].block<3,1>(0,0)*lambda[0];
			dir-=v;
		}else if(nrA == 2){
			A.row(0)=p[aSet[0]].block<3,1>(0,0);
			A.row(1)=p[aSet[1]].block<3,1>(0,0);
			M2=A.block<2,3>(0,0)*A.block<2,3>(0,0).transpose();
			lambda.block<2,1>(0,0)=M2.llt().solve(lambda.block<2,1>(0,0));
			dir=v0-A.block<2,3>(0,0).transpose()*lambda.block<2,1>(0,0);
			dir-=v;
		}else if(nrA == 3){
			A.row(0)=p[aSet[0]].block<3,1>(0,0);
			A.row(1)=p[aSet[1]].block<3,1>(0,0);
			A.row(2)=p[aSet[2]].block<3,1>(0,0);
			M3=A*A.transpose();
			lambda=M3.llt().solve(lambda);
			//dir=v0-A.transpose()*lambda;
			dir.setZero();	//in that case, no dir can be allowed
		}
		//step 2: test stop if p is very small
		if(dir.squaredNorm() < eps)
		{
			minA=-1;
			minLambda=0.0f;
			for(char d=0;d<nrA;d++)
				if(lambda[d] > minLambda)
				{
					minA=d;
					minLambda=lambda[d];
				}
			//aha, we have all negative lagrangian multiplier, exit now!
			if(minA == -1)
				return true;
			//for the most positive component, we remove it from active set
			if(nrA > 1)
			{
				aTag[minA]=false;
				aSet[minA]=aSet[nrA-1];
			}
			aSet[nrA-1]=-1;
			nrA--;
		}
		else
		{
			ASSERT_MSG(nrA <= 2,"My God, that's impossible!")
			//step 3: move until we are blocked
			minA=-1;
			alphaK=1.0f;
			for(sizeType i=0;i<nrP;i++)
			{
				nDotDir=p[i].block<3,1>(0,0).dot(dir);
				if(nDotDir < 0.0f)
				{
					distP=dist(p[i],v);
					if(distP <= 0.0f)
					{
						if(!aTag[i])
						{
							alphaK=0.0f;
							minA=i;
							break;
						}
					}
					else
					{
						distP/=-nDotDir;
						if(distP < alphaK)
						{
							alphaK=distP;
							minA=i;
						}
					}
				}
			}
			v+=alphaK*dir;
			if(minA >= 0)
			{
				//already in active set, so this is rounding error
				for(char d=0;d<nrA;d++)
					if(minA == aSet[d])
						return false;
				//expand active set
				aTag[minA]=true;
				aSet[nrA++]=minA;
			}
		}
	}
	return false;
}

//debugger writer and test scene creator
void toPlanes(const ObjMesh& m,std::vector<Vec4d>& p)	//note that m must be convex
{
	sizeType nrT=(sizeType)m.getI().size();
	p.resize(nrT);
	for(sizeType i=0;i<nrT;i++)
	{
		Vec3i iT=m.getI()[i];
		p[i].block<3,1>(0,0)=-m.getTN()[i].cast<scalarD>();
		p[i][3]=m.getV()[iT[0]].dot(m.getTN()[i]);
		scalarD coef=p[i].block<3,1>(0,0).norm();
		p[i]/=coef;
	}
}
void writeVTK(const Vec3d& v0,const Vec3d& v,const std::string& path)
{
	VTKWriter<scalarD> os("distQP3D frame",path,true);
	std::vector<Vec3d,Eigen::aligned_allocator<Vec3d> > vss;
	vss.push_back(v0);
	vss.push_back(v);
	std::vector<scalarD> css;
	css.push_back(0.0f);
	css.push_back(1.0f);
	os.appendPoints(vss.begin(),vss.end());
	os.appendCustomPointData("Color",css.begin(),css.end());
	os.appendCells(VTKWriter<scalarD>::IteratorIndex<Vec3i>(0,0,0),
					VTKWriter<scalarD>::IteratorIndex<Vec3i>(2,0,0),
					VTKWriter<scalarD>::POINT);
	os.appendCells(VTKWriter<scalarD>::IteratorIndex<Vec3i>(0,2,0),
					VTKWriter<scalarD>::IteratorIndex<Vec3i>(1,2,0),
					VTKWriter<scalarD>::LINE);
}
void testScene(const ObjMesh& m,const std::string& path,scalarD dTheta,scalarD dPhi)
{
	std::vector<Vec4d> p;
	toPlanes(m,p);
	{
		boost::filesystem::create_directory(path);
		ostringstream oss;oss << path << "/mesh.vtk";
		m.writeVTK(oss.str(),true);
	}

	Vec3d v=Vec3d::Zero();
	if(!findFeasible(p,v))
	{
		INFO("Cannot Find Feasible Point!")
		system("pause");
		exit(EXIT_FAILURE);
	}
	ostringstream oss;oss << path << "/init.vtk";
	writeVTK(v,v,oss.str());

	BBox<scalar> bb=m.getBB();
	Vec3d ctr=(bb._minC+bb._maxC).cast<scalarD>()/2;
	scalarD len=(bb._maxC.cast<scalarD>()-ctr).norm()*1.5f;
	Vec3i aSet=Vec3i::Constant(-1);

	sizeType nrI=(sizeType)std::ceil(M_PI*2.0f/dTheta);
	sizeType nrJ=(sizeType)std::ceil(M_PI*2.0f/dPhi);
	for(sizeType i=0;i<nrI;i++)
	for(sizeType j=0;j<nrJ;j++)
	{
		Vec3d X(cos(i*dTheta),0.0f,sin(i*dTheta));
		Vec3d Y(0.0f,1.0f,0.0f);
		Vec3d D=X*cos(j*dPhi)+Y*sin(j*dPhi);
		Vec3d v0=ctr+D*len;
		findClosestPoint(p,v0,v,aSet);

		ostringstream oss;oss << path << "/frm" << (i*nrI+j) << ".vtk";
		writeVTK(v0,v,oss.str());
	}
}
int main()
{
	ObjMesh m;
	MakeMesh::makeTet3D(m);
	m.smooth();
	testScene(m,"./pathTet",0.1f,0.1f);
	system("pause");
}
