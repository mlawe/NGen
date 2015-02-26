#ifndef __TimeGen__
#define __TimeGen__




#include "FluxGen.h"

#include "TVector3.h"


class TimeGen:public FluxGen
{


  double GetCrossSection(NEUTRINO::FLAVOR flav,float E);

  int ProcessIncomingTrack(){TVector3 v(1,0,currentVect->GetTrack(0)->momentum);  //currentVect->SetUserData<TVector3>("vector",v);TVector3 v2=currentVect->GetUserData<TVector3>("vector");std::cout<<"momentum "<<currentVect->GetTrack(0)->momentum<<"  vector "<<v2.Z()<<std::endl;
    return 0;};
  

};



#endif
