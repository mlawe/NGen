#ifndef _BeamFlux_
#define _BeamFlux_

#include "FlxTable.h"
#include "TSpline.h"

class BeamFlux:public FlxTable
{
 public:

  virtual ~BeamFlux(){
    for(size_t i=0;i<cdf_splines.size();i++)
      {
	delete (cdf_splines[i]);
      }
    cdf_splines.clear();
  };

  virtual double GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t);
  virtual double GetFlux(float phi,float cosZ,float E, NEUTRINO::FLAVOR flav,Time_Struct t);

  // virtual int GetRandDir(NEUTRINO::FLAVOR, float, TVector3 *);

  //  virtual std::vector<float> GetEnergyBins();

  virtual int LoadFluxTable(std::string);
  virtual bool CheckFileFormat(std::string)=0;
 protected:
  std::vector<TSpline3*> cdf_splines;
  

};




#endif
