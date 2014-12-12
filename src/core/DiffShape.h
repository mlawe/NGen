#ifndef __DiffShape__
#define __DiffShape__

#include "TRandom3.h"
#include "TargetShape.h"

class DiffShape : public TargetShape
{

 public:
  DiffShape(TargetShape*,TargetShape*);
  //first shape minus second shape
  //virtual ~DiffShape(){};
  double GetVolume();
  void GetRandPos(double pos[3],TRandom3 * rndm);
  bool IsWithin(double pos[3]);
  void Cleanup(){delete T1; T1=NULL; delete T2; T2=NULL;};

 protected:
  TargetShape* T1;
  TargetShape* T2;






};

#endif
