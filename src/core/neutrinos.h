//////////////////////////////////////////////////
/* ENUM for different neutrino flavors
Created: Feb 05, 2014 - C.Kachulis
*////////////////////////////////////////////////
#ifndef _NEUTRINO_
#define _NEUTRINO_

namespace NEUTRINO
{
  enum FLAVOR{
      E=0,
      E_BAR,//1
      MU,//2
      MU_BAR,//3
      NUMBER, //ALWAYS MUST BE LAST! IF WE ADD OTHER NEUTRINOS, THEY MUST COME ABOVE
          
  };
}

#endif
