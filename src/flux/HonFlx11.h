////////////////////////////////////////////////////////////////////
/* Class to hold flux information for honda flux
Created: Feb 06, 2014 - C. Kachulis
*//////////////////////////////////////////////////////////////////

#ifndef _HonFlx11_
#define _HonFlx11_

#include "neutrinos.h"
#include <string.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "FlxTable.h"
#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif

class HonFlx11:public FlxTable
{

public:

  HonFlx11();
  ~HonFlx11();

 
<<<<<<< HEAD
  double GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Get Flux for particular energy, neutrino flavor (summed over all directions) units are (m^2 sec GeV)-1
  double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Units are (m^2 sec sr GEV)^-1
  //phi is angle of direction of neutrino momentum measured from North.  phi=0 is North phi =90 is West, etc
=======
  double GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Get Flux for particular energy, neutrino flavor (summed over all directions) units are (m^2 sec GeV)-1
  double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Units are (m^2 sec sr GEV)^-1
  //phi is angle of direction of neutrino momentum measured from North(+y).  phi=0 is North(+y) phi =90 is West(-x), etc
>>>>>>> time_dependence
  //cosZ is also direction of neutrino momentum, measured from up.  so cosZ=+1 is an upward going neutrino, cosZ=-1 is downward going
 
  int LoadFluxTable(std::ifstream& fileStrm);
 
  std::vector<float> GetEnergyBins();

 private:
 
  float honTable[147][20][12][NEUTRINO::NUMBER][2]; //table of honda fluxes, dimensions are Energy,cosZ,phi,flavor,solar activity, units are (m^2 sec sr GeV)^-1
  //second index runs through cosZ, from -1 to 1 in steps of .1
  //this index runs through phi, from 0 to 360 in steps of 30 degrees
  float honE[147]; //array of energy, corresponds to honTable Energy
  float honTableEn[147][NEUTRINO::NUMBER][2]; //Direction Integrated, units are (m^2 sec GeV)^-1 
  bool hfluxLoaded;
  bool lowfluxLoaded;
 

  void FillIntegratedTables();


};



#endif
