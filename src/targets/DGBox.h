#ifndef _DGBox_
#define _DGBox_

#include "DetectorGeom.h"
#include "TRandom3.h"
#include <cmath>

class DGBox: public DetectorGeom
{
 public:

  double GetVolume();
  TVector3 GetRandPos(TRandom3 * rndm);
  bool IsWithin(TVector3 *pos);
  void SetX(double x){xdim=x;};
  void SetY(double y){ydim=y;};
  void SetZ(double z){zdim=z;};

  void SetXYZ(double x,double y,double z){xdim=x;ydim=y;zdim=z;};


 protected:
  //x,y,z are size in each dimension, extends in positive octant from (0,0,0).  So bottom left front corner at (0,0,0) in detector coords.
  double xdim,ydim,zdim; //in meters

};



#endif
