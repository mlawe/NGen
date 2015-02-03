#include "TargetSphereTrunc.h"

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


TargetSphereTrunc::TargetSphereTrunc() : DetectorGeom::DetectorGeom()
{

  rad=0;
  zmax=0;
}

double TargetSphereTrunc::GetVolume()
{
  double sphereV=(4./3.)*Pi*pow(rad,3);
  double cutV=(2./3.)*Pi*pow(rad,3)*(1.-zmax/rad);
  double coneV=Pi*pow(rad,2)*(1.-pow(zmax/rad,2))*zmax/3.;

  return sphereV+coneV-cutV;

}

TVector3 TargetSphereTrunc::GetRandPos(TRandom3 * rndm)
{

  double x,y,z;
  while(true)
    {
      double R=rad*pow(rndm->Rndm(),1./3.);

      double x1=rndm->Gaus();
      double x2=rndm->Gaus();
      double x3=rndm->Gaus();
  

      x=100.*R*x1/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
      y=100.*R*x2/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
      z=100.*R*x3/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
      if(z<zmax*100) break;
    }


  TVector3 v(x,y,z);

  TransformCoordsMineToGen(&v);
 
  return v;
}

bool TargetSphereTrunc::IsWithin(TVector3 * v)
{

  TransformCoordsGenToMine(v);

  double r=sqrt(pow(v->X(),2)+pow(v->Y(),2)+pow(v->Z(),2));

  bool ret=(r<rad*100 && (v->Z())<(zmax*100));

  return ret;

}
