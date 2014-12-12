#include "HKTank.h"
#include <math.h>
#include <cmath>

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


HKTank::HKTank():TargetShape::TargetShape()
{

}

HKTank::~HKTank()
{


}

double HKTank::GetVolume()
{


  return 99668.433*5;
}


void HKTank::GetRandPos(double pos[3],TRandom3 * rndm)
{

  double x,y,z;
  //choose y position

  y=(2.*rndm->Rndm()-1.)*25.25*100.*5;

  //choose positive or negative x
  if(rndm->Rndm()>.5)
    {
      while(true)
	{
	  double r=rndm->Rndm();
	  double theta=(2.*rndm->Rndm()-1)*Pi/2;


	  x=32*100*sqrt(r)*cos(theta)-8*100;
	  z=32*100*sqrt(r)*sin(theta);

	  if(x>0 && std::abs(z)<24*100)
	    {
	      break;
	    }
	  
	}
    }

  else
    {

      while(true)
	{
	  double r=rndm->Rndm();
	  double theta=rndm->Rndm()*2.*Pi;


	  x=-32*100*sqrt(r)*cos(theta)+8*100;
	  z=32*100*sqrt(r)*sin(theta);

	  if(x<0 && std::abs(z)<24*100)
	    {
	      break;
	    }
	  
	}
    }

  pos[0]=x+pos0[0];
  pos[1]=y+pos0[1];
  pos[2]=z+pos0[2];
}

bool HKTank::IsWithin(double pos[3])
{
  if(std::abs(pos[2]-pos0[2])>24*100) return false;
  if(std::abs(pos[1]-pos0[1])>25.25*100*5) return false;

  if(pos[0]-pos0[0]>0)
    {
      if(sqrt(pow((pos[0]-pos0[0]+8*100),2)+pow(pos[2]-pos0[2],2))>32*100) return false;
    }
  else if(sqrt(pow((pos[0]-pos0[0]-8*100),2)+pow(pos[2]-pos0[2],2))>32*100) return false;
      
  return true;
}


