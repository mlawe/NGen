#include "DetectorGeom.h"

DetectorGeom::DetectorGeom()
{
  density=-1;
  pos0=new TVector3(0,0,0);
  vX=new TVector3(1,0,0);
  vY=new TVector3(0,1,0);
  vZ=new TVector3(0,0,1);

  vXi=new TVector3(1,0,0);
  vYi=new TVector3(0,1,0);
  vZi=new TVector3(0,0,1);

  vM=new TMatrixD(3,3);
  vMi=NULL;
}

DetectorGeom::~DetectorGeom()
{

  delete pos0;
  delete vX;
  delete vY;
  delete vZ;
 
  delete vXi;
  delete vYi;
  delete vZi;
 
  delete vM;

  if(vMi) delete vMi;

}


void DetectorGeom::TransformCoordsMineToGen(TVector3 * v)
{

  double x=v->Dot(*vX);
  double y=v->Dot(*vY);
  double z=v->Dot(*vZ);
  
  v->SetXYZ(x+pos0->X(),y+pos0->Y(),z+pos0->Z());

}

void DetectorGeom::TransformCoordsGenToMine(TVector3 * v)
{
  v->SetXYZ(v->X()-pos0->X(),v->Y()-pos0->Y(),v->Z()-pos0->Z());

  
  double x=v->Dot(*vXi);
  double y=v->Dot(*vYi);
  double z=v->Dot(*vZi);

  v->SetXYZ(x,y,z);



}


void DetectorGeom::UpdatevM()
{
  vM[0][0]=vX->X();
  vM[0][1]=vX->Y();
  vM[0][2]=vX->Z();

  vM[1][0]=vY->X();
  vM[1][1]=vY->Y();
  vM[1][2]=vY->Z();

  vM[2][0]=vZ->X();
  vM[2][1]=vZ->Y();
  vM[2][2]=vZ->Z();

  if(vMi) delete vMi;
  vMi=new TMatrixD(*vM);
  vMi->Invert();

  vXi->SetXYZ(TMatrixDRow(*vMi,0)(0),TMatrixDRow(*vMi,0)(1),TMatrixDRow(*vMi,0)(2));
  vYi->SetXYZ(TMatrixDRow(*vMi,1)(0),TMatrixDRow(*vMi,1)(1),TMatrixDRow(*vMi,1)(2));
  vZi->SetXYZ(TMatrixDRow(*vMi,2)(0),TMatrixDRow(*vMi,2)(1),TMatrixDRow(*vMi,2)(2));
}
