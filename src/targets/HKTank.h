#ifndef __HKTANK__
#define __HKTANK__

#include "DetectorGeom.h"

class HKTank:public DetectorGeom
{
 public:
  HKTank();

  virtual ~HKTank();

  virtual double GetVolume();
  virtual TVector3 GetRandPos(TRandom3 * rndm);
  virtual bool IsWithin(TVector3 * pos);
  



};


#endif
