////////////////////////////////////////////////////////////////////
/* Class to hold flux information for honda flux
Created: Feb 06, 2014 - C. Kachulis
*//////////////////////////////////////////////////////////////////

#ifndef _HonFlx_
#define _HonFlx_

#include "neutrinos.h"
#include <string.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "FlxTable.h"
#include "TimeStruct.h"
#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif

class HonFlx:public FlxTable
{

public:

  HonFlx();

  virtual ~HonFlx(){};
 
  virtual double GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Get Flux for particular energy, neutrino flavor (summed over all directions) units are (m^2 sec GeV)-1
  virtual double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Units are (m^2 sec sr GEV)^-1

  int LoadFluxTable(std::ifstream& fileStrm);
 
  std::vector<float> GetEnergyBins();

  void SetSolarAct(double sA){solarAct=sA;};
 protected:
 
  float honTable[107][20][12][NEUTRINO::NUMBER][2]; //table of honda fluxes, dimensions are Energy,cosZ,phi,flavor,solar activity, units are (m^2 sec sr GeV)^-1
  float honTableL[35][20][NEUTRINO::NUMBER][2];//table of honda fluxes for E<.1 GeV, only 1D, units are (m^2 sec sr GeV)^-1
  float honE[107]; //array of energy, corresponds to honTable Energy
  float honEL[35];
  float honEFull[141];
  float honTableEn[141][NEUTRINO::NUMBER][2]; //Direction Integrated, units are (m^2 sec GeV)^-1 
  bool hfluxLoaded;
  bool lowfluxLoaded;
 
  double GetFlux(float cosZ,float E,NEUTRINO::FLAVOR nuFlav); //For E<.1 GeV where only have 1D, called from 3D GetFlux
  void FillIntegratedTables();
  int LoadLowFluxTable(std::ifstream& fileStrm);

  double solarAct;
};



#endif
