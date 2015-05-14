#include "NeutGen.h"
// #include "nesolactC.h"
#include "necardC.h"
// #include "necardapC.h"
#include <cmath>
#include "vcworkC.h"
#include "neworkC.h"
#include "vcvrtxC.h"
// #include "necardatmC.h"

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


extern "C"{
  extern Float_t fntotpau_(int&,float&);
  extern void nevectap_(int&,float*,float*,int*);
  extern void nevect_(int&,float*,float*,int*);
  extern void nevent_(int&,float*,int*);

  void mcmkhd_();
  void mcmkmh_();
  void mcmkwt_();

  void nemknebk_(float*);
  void nemkmodelbk_();
  void nemkcrsbk_();

  void necpnewk_(float*);
  void nemknpos_();
  // void vcclfsicm_();

  void necard_();
  void nefillmodel_();
  void nefillver_();
  //extern void mcmass_(int&,float&);
  //extern void kzeclr_();
  // extern void kzinit_();
  //extern void geoset_();
  //extern void hrange_(float,float,float);
  //  extern void nevent_(int&,float*,int*);
  /*
  void mcmkhd_();
  void mcmkmh_();
  void mcmkwt_();

  void nemknebk_(float*);
  void nemkmodelbk_();
  void nemkcrsbk_();
  
  void nemknpos_();
  void vcmkvc_();
  void vcmkvx_();
  void nemkfsibk_();
  void nemknetarg_();
*/

}
NeutGen::NeutGen()
  {
  
    //testing for tau crash
    //kzinit_();
    fprintf(stderr,"Seed is : %i\n",rndm->GetSeed());
    //    e_thresh=0;
    // e_max=1000000000;
  }
int NeutGen::Setup()
{
  necard_();
  nefillmodel_();
  nefillver_();

  return 0;
}
 
double NeutGen::GetCrossSection(NEUTRINO::FLAVOR flav,float E)
{
  int iPar[6]={12,-12,14,-14,16,-16};
  double ret;
  
  ret=fntotpau_(iPar[flav],E);

  return ret;
}

int NeutGen::ProcessIncomingTrack()
{


  Track * track=currentVect->GetTrack(0);
  float pMom[3];
  

  for(int i=0;i<3;i++)
    {
      pMom[i]=track->pdir[i]*track->momentum;
    }
  int iErr=0;

  // vcclfsicm_();
  std::cerr<<"parID: "<<track->parID<<std::endl;
  float pos[3];
  pos[0]=currentVect->vertex.X();
  pos[1]=currentVect->vertex.Y();
  pos[2]=currentVect->vertex.Z();
  nevect_(track->parID,pos,pMom,&iErr);

  if(iErr>0)
    {
     
      return -1;
    }
  //set interaction type
  currentVect->intType=nework_.modene;
  //loop through resulting tracks and store in currentVect
  track->SetUserData<int>("iorgvc",vcwork_.iorgvc[0]);
  //  track->SetUserData<int>("iflgvc",vcwork_.iflgvc[0]);
  track->SetUserData<int>("icrnvc",vcwork_.icrnvc[0]);
  track->SetUserData<float>("timvc",vcwork_.timvc[0]);
  track->SetUserData<float>("posivc_x",vcwork_.posivc[0][0]);
  track->SetUserData<float>("posivc_y",vcwork_.posivc[0][1]);
  track->SetUserData<float>("posivc_z",vcwork_.posivc[0][2]);
  track->SetUserData<int>("ivtivc",vcwork_.ivtivc[0]);
  track->SetUserData<float>("posfvc_x",vcwork_.posfvc[0][0]);
  track->SetUserData<float>("posfvc_y",vcwork_.posfvc[0][1]);
  track->SetUserData<float>("posfvc_z",vcwork_.posfvc[0][2]);
  track->SetUserData<int>("ivtfvc",vcwork_.ivtfvc[0]);
  track->status=vcwork_.iflgvc[0];
  Track * tmpTrack;
  //  Track * testTrack=currentVect->AddNewTrack();
  //testTrack->UserData["test"]=4.755;
  for(int i=1;i<vcwork_.nvc;i++)
    {
      tmpTrack=currentVect->AddNewTrack();
      tmpTrack->SetUserData<int>("iorgvc",vcwork_.iorgvc[i]);
      //tmpTrack->SetUserData<int>("iflgvc",vcwork_.iflgvc[i]);
      tmpTrack->status=vcwork_.iflgvc[i];
      tmpTrack->SetUserData<int>("icrnvc",vcwork_.icrnvc[i]);
      tmpTrack->SetUserData<float>("timvc",vcwork_.timvc[i]);
      tmpTrack->SetUserData<float>("posivc_x",vcwork_.posivc[i][0]);
      tmpTrack->SetUserData<float>("posivc_y",vcwork_.posivc[i][1]);
      tmpTrack->SetUserData<float>("posivc_z",vcwork_.posivc[i][2]);
      tmpTrack->SetUserData<int>("ivtivc",vcwork_.ivtivc[i]);
      tmpTrack->SetUserData<float>("posfvc_x",vcwork_.posfvc[i][0]);
      tmpTrack->SetUserData<float>("posfvc_y",vcwork_.posfvc[i][1]);
      tmpTrack->SetUserData<float>("posfvc_z",vcwork_.posfvc[i][2]);
      tmpTrack->SetUserData<int>("ivtfvc",vcwork_.ivtfvc[i]);
      

      tmpTrack->parID=vcwork_.ipvc[i];
      tmpTrack->mass=vcwork_.amasvc[i];
      float P=sqrt(pow(vcwork_.pvc[i][0],2)+pow(vcwork_.pvc[i][1],2)+pow(vcwork_.pvc[i][2],2));
      tmpTrack->momentum=P;
      if(P>0)
	{
	  for(int j=0;j<3;j++)
	    {
	      tmpTrack->pdir[j]=vcwork_.pvc[i][j]/P;
	    }
	}
      else
	{
	  for(int j=0;j<3;j++)
	    {
	      tmpTrack->pdir[j]=0;
	    }

	}
    }
  return 0;
}
