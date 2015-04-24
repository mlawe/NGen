//////////////////////////////////////////////
/* Genieospheric flux generator for simulation
This class will do most of the work during the simulation

Created: Feb 05, 2014 - C. Kachulis
*/////////////////////////////////////////////
#ifndef _GenieFluxGen_
#define _GenieFluxGen_ 

#include <fstream>
#include <iostream>
#include "neutrinos.h"  //neutrino type enum
#include "EvtVector.h"
#include "EvtRate.h"
#include <string>
#include "TRandom3.h"
#include "HonFlx.h"
#include "FluxGen.h"
#include "TFile.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "EVGDrivers/GEVGDriver.h"
class GenieFluxGen:public FluxGen
{


 public:
  GenieFluxGen();
  ~GenieFluxGen();
  void SetMode(int imode);
  void LoadXSec(char * name);
  int Setup();
 private:

  //  float GetRandE(NEUTRINO::FLAVOR flav);
  double GetCrossSection(NEUTRINO::FLAVOR flav,float E);
  int ProcessIncomingTrack();
  int Mode;
  
  genie::GEVGDriver * evg_driver_nu_e_H;
  genie::GEVGDriver * evg_driver_nu_e_bar_H;
  genie::GEVGDriver * evg_driver_nu_mu_H;
  genie::GEVGDriver * evg_driver_nu_mu_bar_H;
  genie::GEVGDriver * evg_driver_nu_tau_H;
  genie::GEVGDriver * evg_driver_nu_tau_bar_H;
 
  genie::GEVGDriver * evg_driver_nu_e_O16;
  genie::GEVGDriver * evg_driver_nu_e_bar_O16;
  genie::GEVGDriver * evg_driver_nu_mu_O16;
  genie::GEVGDriver * evg_driver_nu_mu_bar_O16;
  genie::GEVGDriver * evg_driver_nu_tau_O16;
  genie::GEVGDriver * evg_driver_nu_tau_bar_O16;

  
};


#endif
