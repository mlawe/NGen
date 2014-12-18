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
  
  /*  pos[0]=x*100; //want cm
  pos[1]=y*100;
  pos[2]=z*100;
  */
  //transform position
  TVector3 v(x*100,y*100,z*100);

  TransformCoordsMineToGen(&v);
  pos[0]=v.X();
  pos[1]=v.Y();
  pos[2]=v.Z();

}

bool TargetCyl::IsWithin(double pos[3])
{
  TVector3 v(pos[0],pos[1],pos[2]);
  TransformCoordsGenToMine(&v);

  double r=sqrt(pow(v.X(),2)+pow(v.Y(),2));
  double z=std::abs(v.Z());
  bool ret=(r<rad*100 && z<ht*100/2); //cm

  return ret;

}
