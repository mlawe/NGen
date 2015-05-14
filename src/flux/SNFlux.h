#ifndef __SNFlux__
#define __SNFlux__

#include <map>
#include <boost/unordered_map.hpp>

#include "FlxTable.h"
#include "TimeStruct.h"
#include "TH1F.h"
#include "TRandom3.h"

// Note, this class makes use of the boost::unordered_map. It is possible to run without boost by uncommenting the map based implementations below and the map based implementations of GetMinTime and GetMaxTime in the cxx file

class SNFlux : public FlxTable
{
  static const int SQCMTOSM = 10000; // convert flux in square centimetres to square metres
  static const int64_t STONS = 1000000000; // convert seconds to nanoseconds
  static const float ENERGYMIN = 0.0; // Minimum energy in GeV
  static const float ENERGYMAX = 0.1002; // Maximum energy in GeV
  static const int SCALEFACTOR = 1000000; // Upscaling factor to get some values out. THIS IS TOTALLY ARBITARY AND NEEDS INVESTIGATING!!!

public:
  std::vector<float> GetEnergyBins();
  bool IsTimeDependent(){return true;};
  int GetRandDir(NEUTRINO::FLAVOR flav, float E, TVector3 * dir);
  int LoadFluxTable(std::string fileName);
  double GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR flav,Time_Struct t); //return flux in units of (m^2 sec GeV sr)^-1, E is in GeV, direction is reffering to direction neutrino is traveling, opposite from direction is came
  double GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t); //return flux in units of (m^2 sec GeV)^-1, E is in GeV
  Time_Struct GetNextRecalcTime(Time_Struct t);
  Time_Struct GetMinTime();
  Time_Struct GetMaxTime();
  
protected:
  // map based implementation
  // std::map<int64_t, TH1F*, std::less<int64_t> > fnu_e_time_flux;
  // std::map<int64_t, TH1F*, std::less<int64_t> > fnu_mu_time_flux;
  // std::map<int64_t, TH1F*, std::less<int64_t> > fnu_tau_time_flux;
  // std::map<int64_t, TH1F*, std::less<int64_t> > fnubar_e_time_flux;
  // std::map<int64_t, TH1F*, std::less<int64_t> > fnubar_mu_time_flux;
  // std::map<int64_t, TH1F*, std::less<int64_t> > fnubar_tau_time_flux;

  // unordered_map based implementation
  boost::unordered_map<int64_t, TH1F* > fnu_e_time_flux;
  boost::unordered_map<int64_t, TH1F* > fnu_mu_time_flux;
  boost::unordered_map<int64_t, TH1F* > fnu_tau_time_flux;
  boost::unordered_map<int64_t, TH1F* > fnubar_e_time_flux;
  boost::unordered_map<int64_t, TH1F* > fnubar_mu_time_flux;
  boost::unordered_map<int64_t, TH1F* > fnubar_tau_time_flux;

  // map flavour to time and energy map

  // map based implementation
  // std::map<NEUTRINO::FLAVOR, std::map<int64_t, TH1F*, std::less<int64_t> >, std::less<NEUTRINO::FLAVOR> > fnu_flav_time_flux;
  // unordered_map based implementation
  std::map<NEUTRINO::FLAVOR, boost::unordered_map<int64_t, TH1F* >, std::less<NEUTRINO::FLAVOR> > fnu_flav_time_flux;

private:
  TRandom3* fRandom;
};

#endif
