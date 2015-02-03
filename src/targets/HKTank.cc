#include "HKTank.h"
#include <math.h>
#include <cmath>

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


HKTank::HKTank():DetectorGeom::DetectorGeom()
{

}

HKTank::~HKTank()
{


}

double HKTank::GetVolume()
{


  return 99668.433*5;
}


TVector3 HKTank::GetRandPos(TRandom3 * rndm)
{

  double x,y,z;
  //choose y position

  y=(2.*rndm->Rndm()-1.)*25.25*100.*5;

  //choose positive or negative x
  if(rndm->Rndm()>.5)
    {
      //choose x>0
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
      //choose x<0
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

  TVector3 v(x,y,z);

  TransformCoordsMineToGen(&v);
  return v;
}

bool HKTank::IsWithin(TVector3 * v)
{

  TransformCoordsGenToMine(v);
  if(std::abs(v->Z())>24*100) return false;
  if(std::abs(v->Y())>25.25*100*5) return false;

  if(v->X()>0)
    {
      if(sqrt(pow((v->X()+8*100),2)+pow(v->Z(),2))>32*100) return false;
    }
  else if(sqrt(pow((v->X()-8*100),2)+pow(v->Z(),2))>32*100) return false;
      
  return true;
}


