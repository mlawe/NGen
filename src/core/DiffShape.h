#ifndef __DiffShape__
#define __DiffShape__

#include "TRandom3.h"
#include "DetectorGeom.h"

class DiffShape : public DetectorGeom
{

 public:
  DiffShape(DetectorGeom*,DetectorGeom*);
  //first shape minus second shape
  //virtual ~DiffShape(){};
  double GetVolume();
  TVector3 GetRandPos(TRandom3 * rndm);
  bool IsWithin(TVector3 *);
  void Cleanup(){delete T1; T1=NULL; delete T2; T2=NULL;};

 protected:
  DetectorGeom* T1;
  DetectorGeom* T2;






};

#endif
