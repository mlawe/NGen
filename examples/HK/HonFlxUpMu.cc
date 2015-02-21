#include "HonFlxUpMu.h"

double HonFlxUpMu::GetFlux(float E,NEUTRINO::FLAVOR nuFlav, Time_Struct t)
{
  if(nuFlav!=NEUTRINO::MU and nuFlav!=NEUTRINO::MU_BAR)
    {
      return 0;
    }

  return HonFlx::GetFlux(E,nuFlav,t);

}



double HonFlxUpMu::GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{

    if(nuFlav!=NEUTRINO::MU and nuFlav!=NEUTRINO::MU_BAR)
    {
      return 0;
    }

    
    return HonFlx::GetFlux(phi,cosZ,E,NEUTRINO::MU_BAR,t);
    
}
