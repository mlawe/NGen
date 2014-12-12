#ifndef __TargetSphere__
#define __TargetSphere__

#include "TargetShape.h"
#include "TRandom3.h"
#include <cmath>

class TargetSphere: public TargetShape
{
 public:
  TargetSphere();
  double GetVolume();
  void GetRandPos(double pos[3],TRandom3 * rndm);
  bool IsWithin(double pos[3]);
  void SetR(double r){rad=r;}; //radius in meters
  
 private:
  double rad;
    

};

#endif
