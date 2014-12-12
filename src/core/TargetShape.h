#ifndef __TargetShape__
#define __TargetShape__

#include "TRandom3.h"

class TargetShape
{
 public:
  TargetShape();

  virtual ~TargetShape();

  virtual double GetMass(){return GetVolume()*density;};//return mass in kg
  virtual double GetVolume() =0; //return volume in m^3
  virtual void GetRandPos(double pos[3],TRandom3 * rndm) =0;//set pos to random positon in cm 
  virtual bool IsWithin(double pos[3]) =0;// pos in cm
  void SetDensity(double d){density=d;};
  virtual void SetPos0(double p[3]){for(int i=0;i<3;i++)pos0[i]=p[i];};//pos0 in cm
  double GetDensity(){return density;};
 protected:
  double density; //kg/m^3
  double pos0[3];//cm
  



};





#endif
