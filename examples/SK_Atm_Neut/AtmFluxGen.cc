#include "AtmFluxGen.h"
#include "nesolactC.h"
#include "necardC.h"
#include "necardapC.h"
#include <cmath>
#include "vcworkC.h"
#include "neworkC.h"
#include "vcvrtxC.h"
#include "necardatmC.h"

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


extern "C"{
  extern double fntotpau_(int&,float&);
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
  void vcclfsicm_();

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
AtmFluxGen::AtmFluxGen()
  {
  
    //testing for tau crash
    //kzinit_();
    fprintf(stderr,"Seed is : %i\n",rndm->GetSeed());
    //    e_thresh=0;
    // e_max=1000000000;
  }
int AtmFluxGen::Setup()
{
  necard_();
  nefillmodel_();
  nefillver_();

  if(neutcardatm_.itau==1)
    {
      neutcard_.itauflgcore=1;
    }
  return 0;
}
 
double AtmFluxGen::GetCrossSection(NEUTRINO::FLAVOR flav,float E)
{
  int iPar[6]={12,-12,14,-14,16,-16};
  double ret=0;
  
  //  std::cerr<<"E is "<<E<<" parID is"<<iPar[flav]<<std::endl;
  //std::cerr<<"E is "<<E<<std::endl;
  ret=fntotpau_(iPar[flav],E);
    

  return ret;


}

int AtmFluxGen::ProcessIncomingTrack(Track *track)
{



  float pMom[3];
  

  for(int i=0;i<3;i++)
    {
      pMom[i]=track->pdir[i]*track->momentum;
    }
  int iErr=0;
  //nevectap_(track->parID,(float*)currentVect->vertex,pMom,&iErr);
  vcclfsicm_();
  std::cerr<<"parID: "<<track->parID<<std::endl;
  float pos[3];
  pos[0]=currentVect->vertex.X();
  pos[1]=currentVect->vertex.Y();
  pos[2]=currentVect->vertex.Z();
  nevect_(track->parID,pos,pMom,&iErr);
  //nevent_(track->parID,pMom,&iErr);
  //necpnewk_((float*)currentVect->vertex);

  if(iErr>0)
    {
     
      return -1;
    }
  //set interaction type
  currentVect->intType=nework_.modene;
  //loop through resulting tracks and store in currentVect
  track->SetUserData<int>("iorgvc",vcwork_.iorgvc[0]);
  track->SetUserData<int>("iflgvc",vcwork_.iflgvc[0]);
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
  Track * tmpTrack;
  //  Track * testTrack=currentVect->AddNewTrack();
  //testTrack->UserData["test"]=4.755;
  for(int i=1;i<vcwork_.nvc;i++)
    {
      tmpTrack=currentVect->AddNewTrack();
      tmpTrack->SetUserData<int>("iorgvc",vcwork_.iorgvc[i]);
      tmpTrack->SetUserData<int>("iflgvc",vcwork_.iflgvc[i]);
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

      //std::map<std::string, boost::any> testmap;

      //testmap["testchar1"]=2.1;
      //   tmpTrack->SetUserData<std::string>("testchar1","testchar1");
      //tmpTrack->SetUserData<double>("testchar2",6.3);
      //tmpTrack->SetUserData<std::string>("testchar1","yipeee!");
      //std::cout<<"  AtmFluxGen:: testchar2: "<<tmpTrack->GetUserData<double>("testchar2")<<std::endl;
      //std::cout<<"tmpTrack testchar1: "<<tmpTrack->GetUserData<std::string>("testchar1");
      //std::cout<<"  tmpTrack testchar2: "<<tmpTrack->GetUserData<double>("testchar2")<<std::endl;
      //  tmpTrack->UserData["testchar1"]=static_cast<std::string>("testchar1");
      //tmpTrack->UserData["testchar2"]=3.6;
    }
  /// Testing tau crash with zbs writer structure here.
  /*  mcmkhd_();
  mcmkmh_();
  mcmkwt_();

  nemknebk_((float*)currentVect->vertex);
  
  nemkmodelbk_();
  
  nemkcrsbk_();
  //nemknpos_();
  int lun=1;
  if(lun>0)
    {
      //fprintf(stderr,"Really writing zbs\n");
      //set neut commons correctly.  must be done in mutliple places because neut
      //for nework
      nework_.modene=currentVect->intType;
      nework_.numne=(int)currentVect->GetNumTracks();
      //for vcwork
      vcwork_.nvc=currentVect->intType;

      //      if(currentVect->GetUserData<bool>("data1")) std::cout<<"data1 true"<<std::endl;
      //else std::cout<<"data1 false"<<std::endl;
      //double *a=currentVect->GetUserData<double*>("arr");
      //std::cout<<"arr[0]: "<<a[0]<<" arr[1]: "<<a[1]<<" arr[2]: "<<a[2]<<std::endl;
      for(int i=0;i<3;i++)
	{
	   //for vcvrtx
	  vcvrtx_.pvtxvc[0][i]=(float)currentVect->vertex[i];
	  vcwork_.posvc[i]=(float)currentVect->vertex[i];     
	}
      //loopthrough particles
      Track * tmpTrack;
      for(size_t i=0;i<currentVect->GetNumTracks();i++)
	{
	  tmpTrack=new Track(*currentVect->GetTrack(i));
	  //tmpTrack=currentVect->GetTrack(i);

	  nework_.ipne[i]=tmpTrack->parID;
	  vcwork_.ipvc[i]=tmpTrack->parID;
	  for(int j=0;j<3;j++)
	    {
	      nework_.pne[i][j]=(float)tmpTrack->pdir[j]*tmpTrack->momentum/1000.; //nework is in GeV
	      vcwork_.pvc[i][j]=(float)tmpTrack->pdir[j]*tmpTrack->momentum;//vcwork is in MeV

	      //do i need to adjust other vcwork position variable (posivc,posfvc)?
	    }

	  //	  std::cout<<" ZBSWriter::  testchar1: "<<tmpTrack->GetUserData<std::string>("testchar1")<<std::endl;
	  //std::cout<<" ZBSWriter::  testchar2: "<<tmpTrack->GetUserData<double>("testchar2")<<std::endl;
	  //	  std::cout<<"testchar1: "<<tmpTrack->GetUserData<std::string>("testchar1");


	}
      
      

      vcmkvc_();
      vcmkvx_();
      nemkfsibk_();
      nemknetarg_();
      

      //      kzwrit_(&lun);
      kzeclr_();
   
      }*/
  return 0;
}
/*
void AtmFluxGen::RotateCoords()
{
  //rotate coords from HKflux coords to HKsim coors.  last thing in event loop

  //first rotate vertex
  TVector3 * vect=new TVector3(currentVect->vertex[0], currentVect->vertex[1], currentVect->vertex[2]);
  RotatePoint(vect);
  currentVect->vertex[0]=vect->X();
  currentVect->vertex[1]=vect->Y();
  currentVect->vertex[2]=vect->Z();

  //now rotate direction of all tracks
  size_t nTracks=currentVect->GetNumTracks();
  
  for(size_t i=0;i<nTracks;i++)
    {
      Track * track=currentVect->GetTrack(i);

      vect->SetX(track->pdir[0]);
      vect->SetY(track->pdir[1]);
      vect->SetZ(track->pdir[2]);

      RotatePoint(vect);

      track->pdir[0]=vect->X();
      track->pdir[1]=vect->Y();
      track->pdir[2]=vect->Z();

    }

  //done so clean up memory
  delete vect;
    
}


void AtmFluxGen::RotatePoint(TVector3 * v)
{

  v->RotateX(Pi/2.);

  v->SetX(-v->X());

  v->SetY(-v->Y());




}
*/


