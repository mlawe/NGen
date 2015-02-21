#ifndef _HonFlx11UpMu_
#define _HonFlx11UpMu_

#include "HonFlx11.h"

class HonFlx11UpMu:public HonFlx11
{

 public:
 ~HonFlx11UpMu(){};
  virtual double GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Get Flux for particular energy, neutrino flavor (summed over all directions) units are (m^2 sec GeV)-1
  virtual double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t); //Units are (m^2 sec sr GEV)^-1



};



#endif
