#ifndef __TargetSphereTrunc__
#define __TargetSphereTrunc__

#include "DetectorGeom.h"
#include "TRandom3.h"
#include <cmath>

class TargetSphereTrunc: public DetectorGeom
{
 public:
  TargetSphereTrunc();
  double GetVolume();
  TVector3 GetRandPos(TRandom3 * rndm);
  bool IsWithin(TVector3*);
  void SetR(double r){rad=r;}; //radius in meters
  void SetMaxZ(double z){zmax=z;}; //in meters
 private:
  double rad;
  double zmax;

};

#endif
