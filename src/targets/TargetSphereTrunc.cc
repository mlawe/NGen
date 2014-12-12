#include "TargetSphereTrunc.h"

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


TargetSphereTrunc::TargetSphereTrunc() : TargetShape::TargetShape()
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

void TargetSphereTrunc::GetRandPos(double pos[3],TRandom3 * rndm)
{
  while(true)
    {
      double R=rad*pow(rndm->Rndm(),1./3.);

      double x1=rndm->Gaus();
      double x2=rndm->Gaus();
      double x3=rndm->Gaus();
  

      pos[0]=100.*R*x1/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
      pos[1]=100.*R*x2/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
      pos[2]=100.*R*x3/sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
      if(pos[2]<zmax*100) break;
    }
  pos[0]+=pos0[0];
  pos[1]+=pos0[1];
  pos[2]+=pos0[2];

}

bool TargetSphereTrunc::IsWithin(double pos[3])
{
  double r=sqrt(pow(pos[0]-pos0[0],2)+pow(pos[1]-pos0[1],2)+pow(pos[2]-pos0[2],2));

  bool ret=(r<rad*100 && (pos[2]-pos0[2])<(zmax*100));

  return ret;

}
