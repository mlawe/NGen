#ifndef _T2HKFlux_
#define _T2HKFlux_

#include "FlxTable.h"
#include "TSpline.h"

class T2HKFlux:public FlxTable
{
 public:

  ~T2HKFlux(){cdf_splines.clear();}
  virtual double GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t);
  virtual double GetFlux(float phi,float cosZ,float E, NEUTRINO::FLAVOR flav,Time_Struct t);

  virtual int GetRandDir(NEUTRINO::FLAVOR, float, TVector3 *);

  virtual std::vector<float> GetEnergyBins();

  virtual int LoadFluxTable(std::string);
 protected:
  int LoadFluxFromTxt(std::string);
  int LoadFluxFromRoot(std::string);
  std::vector<TSpline3*> cdf_splines;
  

};




#endif
