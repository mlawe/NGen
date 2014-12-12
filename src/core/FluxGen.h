#ifndef _FluxGen_
#define _FluxGen_

#include <string>
#include "TRandom3.h"
#include "EvtRate.h"
#include "EvtVector.h"
#include "TargetShape.h"
#include "FlxTable.h"
#include "TargetShape.h"
#include "Track.h"
#include "Writer.h"
#include "TVector3.h"
class FluxGen
{
  //abstract base class for flux generation.  Handles main work in simulation
  // all virtual funtions should return 0 if successful, -1 if unsuccesful
 public:
  FluxGen();
  
  virtual ~FluxGen();

  int LoadFluxTable(std::string flxFile);
  
  int CreateEvtRate();
  int CreateNextVector(); // return 0 on success, 1 to skip writeout of this event, 2 if no more events left
  int WriteCurrentVector();
  void AddNewWriter(Writer * writer,const char* name);
  void CloseWriters(); //closes files and clear memory of writers
  void SetSeed(unsigned seed);
  void SetTarget(TargetShape* t);
  void SetSolarAct(float s);
  void SetNYears(float y);
  void SetFlxTable(FlxTable* tbl);
  void SetvX(TVector3 v){vX->SetXYZ(v.X(),v.Y(),v.Z());};//x-unit vector of final coordinates in generator coords
  void SetvY(TVector3 v){vY->SetXYZ(v.X(),v.Y(),v.Z());};//y-unit vector of final coordinates in generator coords
  void SetvZ(TVector3 v){vZ->SetXYZ(v.X(),v.Y(),v.Z());};//z-unit vector of final coordinates in generator coords
  void SetSpectralIndecies(std::vector<float>);
  void SetRatios(std::vector<float>);
  void SetEMode(int i){EMode=i;};
  void SetDirMode(int i){dirMode=i;};
 
  void SetEThresh(double eth){e_thresh=eth;};
  void SetEMax(double em){e_max=em;};
 protected:
  void RotateCoords(); //transforms from coords of flux generator to requested final coords
  int GetRandDir(NEUTRINO::FLAVOR flav, float e, float dir[3]);
  virtual int GetRandDir(NEUTRINO::FLAVOR flav,float e,float dir[3]);
  //sets dir as random selected momentum direction according to distribution in tables
  
  virtual bool CheckEHook(float p,double pos[3]){return true;};
  
  virtual float GetRandE(NEUTRINO::FLAVOR flav);
  //returns random energy in units of GeV
  float GetRandEStepping(NEUTRINO::FLAVOR flav);
  float GetRandEInversion(NEUTRINO::FLAVOR flav);
  double GetCrossSec(NEUTRINO::FLAVOR flav, float E); //considers e_thresh, e_max
  virtual double GetCrossSection(NEUTRINO::FLAVOR flav, float E)=0;
//return cross section in units of 10^-38 cm^2 nucleons^-1, E in units of GeV
  
  virtual int ProcessIncomingTrack(Track* track) =0; //0 if successful, 1 to skip writeout,-1 on error
  
  void RotatePoint(TVector3 *v);
  float nYears;
  TRandom3 * rndm;

  EvtRate * evtRate;
  EvtVector * currentVect;

  FlxTable * flxTbl;

  double e_thresh;
  double e_max;

  TargetShape* target;
  TVector3 * vX;
  TVector3 * vY;
  TVector3 * vZ;
  std::vector<double> fluxCSIntE;
  std::vector<double> fluxCSIntMu;
  std::vector<double> fluxCSIntEBar;
  std::vector<double> fluxCSIntMuBar;

  std::vector<Writer *> Writers;

  int EMode; //0 for Stepping, 1 for Inversion sampling
  int dirMode; //0 calc here base on flux distro, 1 get from call to flux table
  std::vector<float> specinds;
  std::vector<float> ratios;
  //  int count;
  // int count2,count3;
};


#endif
