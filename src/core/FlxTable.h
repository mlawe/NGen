#ifndef _FlxTable_
#define _FlxTable_


#include "neutrinos.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "TimeStruct.h"
class FlxTable
{

 public:


  virtual double GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t) =0;
  //return flux in units of (m^2 sec GeV)^-1, E is in GeV

  virtual double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR flav,Time_Struct t) =0;
  //return flux in units of (m^2 sec GeV sr)^-1, E is in GeV, direction is reffering to direction neutrino is traveling, opposite from direction is came
  virtual bool IsTimeDependent(){return false;}; //Is the flux time dependent?
  virtual Time_Struct GetNextRecalcTime(Time_Struct t1){Time_Struct t;return t;};  //When do we need to recalc EvtRates if last calced at t1?
  virtual int LoadFluxTable(std::ifstream& fileStrm) =0;
  virtual int GetRandDir(NEUTRINO::FLAVOR, float, float dir[3]);
  virtual std::vector<float> GetEnergyBins() =0;
  //units are GeV


};




#endif
