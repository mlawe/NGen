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
  void GetRandPos(double pos[3],TRandom3 * rndm);
  bool IsWithin(double pos[3]);
  void SetR(double r){rad=r;}; //radius in meters
  void SetMaxZ(double z){zmax=z;}; //in meters
 private:
  double rad;
  double zmax;

};

#endif
