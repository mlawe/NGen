#ifndef _FlxTable_
#define _FlxTable_


#include "neutrinos.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>

class FlxTable
{

 public:
  FlxTable();

  virtual ~FlxTable();

  virtual double GetFlux(float E,NEUTRINO::FLAVOR flav) =0;
  //return flux in units of (m^2 sec GeV)^-1, E is in GeV

  virtual double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR flav) =0;
  //return flux in units of (m^2 sec GeV sr)^-1, E is in GeV, direction is reffering to direction neutrino is traveling, opposite from direction is came

  virtual int LoadFluxTable(std::ifstream& fileStrm) =0;
  virtual int GetRandDir(NEUTRINO::FLAVOR, float, float dir[3]);
  virtual std::vector<float> GetEnergyBins() =0;
  //unite are GeV

  void SetSolarAct(float sA);

 protected:
  float solarAct;
};




#endif
