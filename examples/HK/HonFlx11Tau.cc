#include "HonFlx11Tau.h"

double HonFlx11Tau::GetFlux(float E,NEUTRINO::FLAVOR nuFlav, Time_Struct t)
{
  if(nuFlav==NEUTRINO::TAU)
    {
      return HonFlx11::GetFlux(E,NEUTRINO::MU,t);
    }
  else if(nuFlav==NEUTRINO::TAU_BAR)
    {
      return HonFlx11::GetFlux(E,NEUTRINO::MU_BAR,t);
    }
  else
    {
      return 0;
    }


}



double HonFlx11Tau::GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{
  if(nuFlav==NEUTRINO::TAU)
    {
      return HonFlx11::GetFlux(phi,cosZ,E,NEUTRINO::MU,t);
    }
  else if(nuFlav==NEUTRINO::TAU_BAR)
    {
      return HonFlx11::GetFlux(phi,cosZ,E,NEUTRINO::MU_BAR,t);
    }
  else
    {
      return 0;
    }
  



}
