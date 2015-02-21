#include "HonFlxTau.h"

double HonFlxTau::GetFlux(float E,NEUTRINO::FLAVOR nuFlav, Time_Struct t)
{
  if(nuFlav==NEUTRINO::TAU)
    {
      return HonFlx::GetFlux(E,NEUTRINO::MU,t);
    }
  else if(nuFlav==NEUTRINO::TAU_BAR)
    {
      return HonFlx::GetFlux(E,NEUTRINO::MU_BAR,t);
    }
  else
    {
      return 0;
    }


}



double HonFlxTau::GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{
  if(nuFlav==NEUTRINO::TAU)
    {
      return HonFlx::GetFlux(phi,cosZ,E,NEUTRINO::MU,t);
    }
  else if(nuFlav==NEUTRINO::TAU_BAR)
    {
      return HonFlx::GetFlux(phi,cosZ,E,NEUTRINO::MU_BAR,t);
    }
  else
    {
      return 0;
    }
  



}
