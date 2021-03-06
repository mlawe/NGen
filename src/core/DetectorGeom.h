#ifndef __DetectorGeom__
#define __DetectorGeom__

#include "TRandom3.h"
#include "TVector3.h"
#include "TMatrixD.h"
class DetectorGeom
{
 public:
  DetectorGeom();

  virtual ~DetectorGeom();

  virtual double GetMass(){return GetVolume()*density;};//return mass in kg
  virtual double GetVolume() =0; //return volume in m^3
  virtual TVector3 GetRandPos(TRandom3 * rndm) =0;//return random positon in cm, return in generator coords 
  virtual bool IsWithin(TVector3 * pos) =0;// pos in cm, in generator coods
  
  void SetvM(TVector3 * vx,TVector3* vy,TVector3* vz){vX=vx; vY=vy;vZ=vz;UpdatevM();}

  void SetDensity(double d){density=d;};
  virtual void SetPos0(TVector3 p){for(int i=0;i<3;i++)pos0->SetXYZ(p.X(),p.Y(),p.Z());};//pos0 in cm, in generator coords.
  double GetDensity(){return density;};
 protected:
  double density; //kg/m^3
  TVector3 * pos0;//cm position of target shape origin in generator coords
  TMatrixD * vM;//transform matrix from my coords to gen coords (not including translation)
  TMatrixD * vMi;//inverse of vM
  TVector3 * vX;//x-unit vector of generator coordinates in target shape coords
  TVector3 * vY;//y-unit vector of generator coordinates in target shape coords
  TVector3 * vZ;//z-unit vector of generator coordinates in target shape coords

  TVector3 * vXi;
  TVector3 * vYi;
  TVector3 * vZi;

  void TransformCoordsGenToMine(TVector3 *);
  void TransformCoordsMineToGen(TVector3 *);

  void UpdatevM();
};





#endif
