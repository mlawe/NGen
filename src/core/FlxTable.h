#ifndef _FlxTable_
#define _FlxTable_


#include "neutrinos.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "TimeStruct.h"
#include "TVector3.h"
class FlxTable
{

 public:

  virtual ~FlxTable(){};
  virtual double GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t) =0;
  //return flux in units of (m^2 sec GeV)^-1 if using time based, or (m^2 10e20POT GeV)^-1 if using POT (for beam), E is in GeV

  virtual double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR flav,Time_Struct t) =0;
  //return flux in units of (m^2 sec GeV sr)^-1, E is in GeV, direction is reffering to direction neutrino is traveling, opposite from direction is came.  For beam flux you should probably have this method make an exit call and print out an error message. Call FluxGen::SetDirMode(1), to get direction from this flux table instead.  In this mode, this method will never be called
  virtual bool IsTimeDependent(){return false;}; //Is the flux time dependent?
  virtual Time_Struct GetNextRecalcTime(Time_Struct t1){Time_Struct t;return t;};  //When do we need to recalc EvtRates if last calced at t1?
  virtual int LoadFluxTable(std::ifstream& fileStrm) =0;
  virtual int GetRandDir(NEUTRINO::FLAVOR, float, TVector3 * dir);
  virtual std::vector<float> GetEnergyBins() =0;
  //units are GeV


};




#endif
