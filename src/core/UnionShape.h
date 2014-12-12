#ifndef __UnionShape__
#define __UnionShape__

#include "TRandom3.h"
#include "TargetShape.h"
//Must be no overlap between shapes
class UnionShape : public TargetShape
{
 public:
  UnionShape(std::vector<TargetShape*>);

  virtual ~UnionShape();
  void Cleanup(){Targets.clear();};
  double GetVolume();
  void GetRandPos(double pos[3],TRandom3 * rndm);
  bool IsWithin(double pos[3]);
  void AddTarget(TargetShape* t){Targets.push_back(t);};

 protected:
  std::vector<TargetShape*> Targets;







};

#endif
