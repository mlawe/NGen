#ifndef __TargetSphere__
#define __TargetSphere__

#include "DetectorGeom.h"
#include "TRandom3.h"
#include <cmath>

class TargetSphere: public DetectorGeom
{
 public:
  TargetSphere();
  double GetVolume();
  TVector3 GetRandPos(TRandom3 * rndm);
  bool IsWithin(TVector3 *pos);
  void SetR(double r){rad=r;}; //radius in meters
  
 private:
  double rad;
    

};

#endif
