#ifndef __TimeFlux__
#define __TimeFlux__

#include "FlxTable.h"
#include "TimeStruct.h"

class TimeFlux:public FlxTable
{

  int LoadFluxTable(std::ifstream & fileStrm){return 0;};

  std::vector<float> GetEnergyBins();

  
  double GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t);
  //return flux in units of (m^2 sec GeV)^-1, E is in GeV

  double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR flav,Time_Struct t);
  //return flux in units of (m^2 sec GeV sr)^-1, E is in GeV, direction is reffering to direction neutrino is traveling, opposite from direction is came


  bool IsTimeDependent(){return true;};

  Time_Struct GetNextRecalcTime(Time_Struct t);
};



#endif
