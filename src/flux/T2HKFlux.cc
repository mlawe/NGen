#include "T2HKFlux.h"
#include <iostream>
#include <stdlib.h>
#include <boost/algorithm/string/predicate.hpp>
#include <algorithm>

std::vector<float> T2HKFlux::GetEnergyBins()
{
  std::vector<float> ret;
  float start=.01;
  float end=9.95;
  int nSteps=20000;
  for(int i=0;i<nSteps;i++)
    {
      ret.push_back((end-start)*float(i)/float(nSteps)+start);

    }

  return ret;

}

int T2HKFlux::GetRandDir(NEUTRINO::FLAVOR flav, float E, TVector3 * dir)
{
  dir->SetXYZ(0.669764,-0.742179,0.024223);
  return 0;
}

bool T2HKFlux::CheckFileFormat(std::string line)
{
 if(!boost::algorithm::starts_with(line,"T2HK flux"))
    {
      return false;
    }
 return true;
}
