#ifndef __TimeGen__
#define __TimeGen__




#include "FluxGen.h"




class TimeGen:public FluxGen
{


  double GetCrossSection(NEUTRINO::FLAVOR flav,float E);

  int ProcessIncomingTrack(Track* track){return 0; currentVect->GetTrack(0)->SetUserData<int>("test",2);};
};



#endif
