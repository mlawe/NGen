#ifndef __TargetCyl__
#define __TargetCyl__

#include "DetectorGeom.h"
#include "TRandom3.h"
#include <cmath>
class TargetCyl: public DetectorGeom
{
 public:
  TargetCyl();
  double GetVolume();

  void GetRandPos(double pos[3], TRandom3 * rndm);
  bool IsWithin(double pos[3]);
  void SetHeight(double h);//height in m
  void SetR(double r);//radius in m
  double GetHeight(){return ht;};
  double GetR(){return rad;};


 private:
  double ht;
  double rad;


};


#endif
