#ifndef __UnionShape__
#define __UnionShape__

#include "TRandom3.h"
#include "DetectorGeom.h"
//Must be no overlap between shapes
class UnionShape : public DetectorGeom
{
 public:
  UnionShape(std::vector<DetectorGeom*>);

  virtual ~UnionShape();
  void Cleanup(){Targets.clear();};
  double GetVolume();
  void GetRandPos(double pos[3],TRandom3 * rndm);
  bool IsWithin(double pos[3]);
  void AddTarget(DetectorGeom* t){Targets.push_back(t);};

 protected:
  std::vector<DetectorGeom*> Targets;







};

#endif
