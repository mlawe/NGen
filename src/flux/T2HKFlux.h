#ifndef _T2HKFlux_
#define _T2HKFlux_

#include "BeamFlux.h"
#include "TSpline.h"

class T2HKFlux:public BeamFlux
{
 public:

  int GetRandDir(NEUTRINO::FLAVOR, float, TVector3 *);

  std::vector<float> GetEnergyBins();

  bool CheckFileFormat(std::string);

};




#endif
