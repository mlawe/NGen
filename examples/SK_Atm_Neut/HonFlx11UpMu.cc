#include "HonFlx11UpMu.h"

double HonFlx11UpMu::GetFlux(float E,NEUTRINO::FLAVOR nuFlav, Time_Struct t)
{
  if(nuFlav!=NEUTRINO::MU and nuFlav!=NEUTRINO::MU_BAR)
    {
      return 0;
    }

  return HonFlx11::GetFlux(E,nuFlav,t);

}



double HonFlx11UpMu::GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{

    if(nuFlav!=NEUTRINO::MU and nuFlav!=NEUTRINO::MU_BAR)
    {
      return 0;
    }

    
    return HonFlx11::GetFlux(phi,cosZ,E,NEUTRINO::MU_BAR,t);
    
}
