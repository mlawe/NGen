#include "TargetCyl.h"
#include <cmath>

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif

TargetCyl::TargetCyl() : TargetShape::TargetShape()
{
  ht=0;
  rad=0;
}
double TargetCyl::GetVolume()
{
  return Pi*ht*pow(rad,2.);

}

void TargetCyl::SetHeight(double h)
{
  ht=h;
}

void TargetCyl::SetR(double r)
{
  rad=r;
}
void TargetCyl::GetRandPos(double pos[3],TRandom3 * rndm)
{
  double x,y,z;
  while(true)
    {
      x=(2.*rndm->Rndm()-1.)*rad;
      y=(2.*rndm->Rndm()-1.)*rad;
      if(sqrt(pow(x,2)+pow(y,2))<=rad)
	{
	  break;
	}
    }
  z=(2.*rndm->Rndm()-1.)*ht/2.;
  
  pos[0]=x*100; //want cm
  pos[1]=y*100;
  pos[2]=z*100;

  //translate by pos0
  pos[0]+=pos0[0];
  pos[1]+=pos0[1];
  pos[2]+=pos0[2];

}

bool TargetCyl::IsWithin(double pos[3])
{
  double r=sqrt(pow(pos[0]-pos0[0],2)+pow(pos[1]-pos0[1],2));
  double z=std::abs(pos[2]-pos0[2]);
  bool ret=(r<rad*100 && z<ht*100/2); //cm

  return ret;

}
