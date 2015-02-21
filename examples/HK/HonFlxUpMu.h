#ifndef _HonFlxUpMu_
#define _HonFlxUpMu_

#include "HonFlx.h"

class HonFlxUpMu:public HonFlx
{

 public:
 ~HonFlxUpMu(){};
  
  virtual double GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Get Flux for particular energy, neutrino flavor (summed over all directions) units are (m^2 sec GeV)-1
  virtual double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Units are (m^2 sec sr GEV)^-1



};



#endif
