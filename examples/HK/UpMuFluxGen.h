//////////////////////////////////////////////
/* Atmospheric flux generator for UpMu simulation
This class will do most of the work during the simulation
This class is specialized version of NeutGen intended for UpMu vector production

Created: Jul 28, 2014 - C. Kachulis
*/////////////////////////////////////////////
#ifndef _UpMuFluxGen_
#define _UpMuFluxGen_ 

#include <fstream>
#include <iostream>
#include "neutrinos.h"  //neutrino type enum
#include "EvtVector.h"
#include "EvtRate.h"
#include <string>
#include "TRandom3.h"
#include "FluxGen.h"
#include "TVector3.h"
#include "NeutGen.h"
#include "TSpline.h"

class UpMuFluxGen:public NeutGen
{


 public:
  UpMuFluxGen();
  void SetRMax(double r){rmax=r;};//cm
  //  void SetEThresh(double eth){e_thresh=eth;};
  //void SetEMax(double emax){e_max=emax;};
  double GetRange(double e){return muonRange->Eval(e)/(detector->GetDensity()/1000.);};//cm, e in MeV
  double GetRMax(){return rmax;}; //cm
  void ResetEHookCount(){failedEHookCount=0;};
  int GetFailedEHookCount(){return failedEHookCount;};
  int GetFailedMu_Count(){return failedMu_Count;};
  int GetFailedMu_NuCount(){return failedMu_NuCount;};
  void ResetFailedMu_Count(){failedMu_Count=0;};
  void ResetFailedMu_NuCount(){failedMu_NuCount=0;};
  void SetTrueDetector(DetectorGeom * t){true_detector=t;};
  int GetTeVCount(){return TevCount;};
  void SetFirstRotationAxis(TVector3 * a){rotaxis1=a;}; //Set axis about which first rotation is performed in RotateNuTrack.  Second rotation will the be performed around axis orthogonal to rotaxis1 and neutrino direction.  This should generally be the long axis of the detector. If neutrino direction is paralell to rotaxis1, first rotation is by 0, and second rotation is around axis orthogonal to rotaxis1 and "R" Axis   
  void SetRAxis(TVector3 * a){raxis=a;}; //Set "R" Axis.  This axis point along height of unrotated upmu volume.

 private:

  TVector3 * rotaxis1;
  TVector3* raxis;
  int TevCount;
  bool CheckEHook(float p,TVector3);
  int ProcessIncomingTrack(Track* track);
  bool CheckUpMu_Nu(TVector3,TVector3,float p);
  bool CheckUpMu(TVector3,TVector3,float p);
  bool LoadMuonRange();
  void RotateNuTrack();
  double rmax;
  //double e_thresh; //energy threshold GeV
  //double e_max;
  int failedEHookCount;
  TSpline3 * muonRange;

  int failedMu_NuCount;
  int failedMu_Count;
  DetectorGeom * true_detector;

};


#endif
