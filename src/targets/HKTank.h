#ifndef __HKTANK__
#define __HKTANK__

#include "DetectorGeom.h"

class HKTank:public DetectorGeom
{
 public:
  HKTank();

  virtual ~HKTank();

  virtual double GetVolume();
  virtual void GetRandPos(double pos[3],TRandom3 * rndm);
  virtual bool IsWithin(double pos[3]);
  



};


#endif
