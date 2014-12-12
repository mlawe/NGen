#ifndef __HKTANK__
#define __HKTANK__

#include "TargetShape.h"

class HKTank:public TargetShape
{
 public:
  HKTank();

  virtual ~HKTank();

  virtual double GetVolume();
  virtual void GetRandPos(double pos[3],TRandom3 * rndm);
  virtual bool IsWithin(double pos[3]);
  
 private:
  double length;


};


#endif
