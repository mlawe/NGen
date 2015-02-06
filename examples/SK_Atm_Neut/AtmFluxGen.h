//////////////////////////////////////////////
/* Atmospheric flux generator for simulation
This class will do most of the work during the simulation

Created: Feb 05, 2014 - C. Kachulis
*/////////////////////////////////////////////
#ifndef _AtmFluxGen_
#define _AtmFluxGen_ 

#include <fstream>
#include <iostream>
#include "neutrinos.h"  //neutrino type enum
#include "EvtVector.h"
#include "EvtRate.h"
#include <string>
#include "TRandom3.h"
//#include "HonFlx11.h"
#include "FluxGen.h"
#include "TVector3.h"

class AtmFluxGen:public FluxGen
{


 public:
  AtmFluxGen();
  void SetEThresh(double eth){e_thresh=eth;};
  void SetEMax(double emax){e_max=emax;};
  virtual int Setup();
 protected:
  //  double e_thresh;
  //  double e_max; //GeV

 private:

  //  float GetRandE(NEUTRINO::FLAVOR flav);
  virtual double GetCrossSection(NEUTRINO::FLAVOR flav,float E);
  virtual int ProcessIncomingTrack(Track* track);
};


#endif
