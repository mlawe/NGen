#include "TargetSphere.h"

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


TargetSphere::TargetSphere() : DetectorGeom::DetectorGeom()
{

  rad=0;

}

double TargetSphere::GetVolume()
{
  return (4./3.)*Pi*pow(rad,3);

}

TVector3 TargetSphere::GetRandPos(TRandom3 * rndm)
{
  double R=rad*pow(rndm->Rndm(),1./3.);

  double x1=rndm->Gaus();
  double x2=rndm->Gaus();
  double x3=rndm->Gaus();
  
  double x,y,z;
  x=100.*R*x1/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
  y=100.*R*x2/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
  z=100.*R*x3/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));

  TVector3 v(x,y,z);

  TransformCoordsMineToGen(&v);

  return v;

}

bool TargetSphere::IsWithin(TVector3* v)
{


  TransformCoordsGenToMine(v);

  double r=sqrt(pow(v->X(),2)+pow(v->Y(),2)+pow(v->Z(),2));

  bool ret=(r<rad*100);

  return ret;

}
