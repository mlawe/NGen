#include "TargetSphere.h"

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


TargetSphere::TargetSphere() : TargetShape::TargetShape()
{

  rad=0;

}

double TargetSphere::GetVolume()
{
  return (4./3.)*Pi*pow(rad,3);

}

void TargetSphere::GetRandPos(double pos[3],TRandom3 * rndm)
{
  double R=rad*pow(rndm->Rndm(),1./3.);

  double x1=rndm->Gaus();
  double x2=rndm->Gaus();
  double x3=rndm->Gaus();
  

  pos[0]=100.*R*x1/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
  pos[1]=100.*R*x2/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
  pos[2]=100.*R*x3/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));

  TVector3 v(pos[0],pos[1],pos[2]);

  TransformCoordsMineToGen(&v);
  pos[0]=v.X();
  pos[1]=v.Y();
  pos[2]=v.Z();

}

bool TargetSphere::IsWithin(double pos[3])
{

  TVector3 v(pos[0],pos[1],pos[2]);
  TransformCoordsGenToMine(&v);

  double r=sqrt(pow(v.X(),2)+pow(v.Y(),2)+pow(v.Z(),2));

  bool ret=(r<rad*100);

  return ret;

}
