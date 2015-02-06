#include "UpMuFluxGen.h"
#include "nesolactC.h"
#include "necardC.h"
#include "necardapC.h"
#include <cmath>
#include "vcworkC.h"
#include "neworkC.h"
#include <stdlib.h>
#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif


extern "C"{
  extern double fntotpau_(int&,float&);
  //  extern void nevectap_(int&,float*,float*,int*);
  extern void nevect_(int&,float*,float*,int*);
  extern void mcmass_(int&,float&);
  extern void kzeclr_();
  extern void kzinit_();
  extern void geoset_();
  extern void hrange_(float,float,float);
  //  extern void nevent_(int&,float*,int*);
}

UpMuFluxGen::UpMuFluxGen():AtmFluxGen::AtmFluxGen()
  {
    TevCount=0;
    e_thresh=0;
    e_max=0;
    failedEHookCount=0;
    failedMu_NuCount=0;
    if(!LoadMuonRange())
      {
	std::cout<<"Failed to open Muon Range file (Muon_Range.dat)"<<std::endl;
	exit(0);
      }

    rotaxis1=new TVector3(0,1,0);
    raxis=new TVector3(0,0,1);
  }


int UpMuFluxGen::ProcessIncomingTrack(Track *track)
{
  float pMom[3];
  /*  if(Mode==1)
    {//tau mode, turn nu_mu into nu_tau
      if(track->parID==14)
	{
	  track->parID=16;
	}
      else if(track->parID==-14)
	{
	  track->parID=-16;
	}
      
    }
  */  

  if(track->momentum>1000000.) TevCount++;
  RotateNuTrack();
  
  //Check if resulting position is inside water
  if(true_detector->IsWithin(&(currentVect->vertex))) return 1;
  for(int i=0;i<3;i++)
    {
      pMom[i]=track->pdir[i]*track->momentum;
    }

  if(!CheckUpMu_Nu(currentVect->vertex,track->pdir,track->momentum))
    {
      /*      Track * t=currentVect->GetTrack(0);
      TVector3 v(-t->pdir[0],-t->pdir[1],-t->pdir[2]);
      

      TVector3 pos(currentVect->vertex[0],currentVect->vertex[1],currentVect->vertex[2]);

      std::cerr<<"Failed Nu: Dir: "<<-v.X()<<" "<<-v.Y()<<" "<<-v.Z()<<" pos:  "<<pos.X()<<"  "<<pos.Y()<<"  "<<pos.Z();
      double Cang=pos.Dot(v)/pos.Mag();
      double Sang=sqrt(1-Cang*Cang);
      TVector3 vT=pos-v*pos.Mag()*Cang;


      std::cerr<<"  rtest: "<<vT.Mag()<<"  rtest2: "<<pos.Mag()*Sang<<std::endl;*/
      failedMu_NuCount++;
      return 1;

    }
  int iErr=0;
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
  //loop through resulting tracks and store in 

  Track * tmpTrack;

  for(int i=1;i<vcwork_.nvc;i++)
    {
      tmpTrack=currentVect->AddNewTrack();
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

  Track * MuTrack;
  for(size_t i=0;i<currentVect->GetNumTracks();i++)
    {
      MuTrack=currentVect->GetTrack(2);

      if(abs(MuTrack->parID)!=13)
	{
	  //don't store if not a muon
	  continue;
	}

      if(CheckUpMu(currentVect->vertex,MuTrack->pdir,MuTrack->momentum))
	{
	  //found a muon that could get to detector
	  return 0;
	}
    }
  failedMu_Count++;
  return 1; //no muon that could get to detector
}

bool UpMuFluxGen::LoadMuonRange()
{
  ifstream inRangeFile("Muon_Range.dat");
  if(inRangeFile.fail())
    {
      return false;

    }
  char line[200];

  //read past first two lines
  inRangeFile.getline(line,200);
  inRangeFile.getline(line,200);
  
  double P[145]; //MeV
  double Range[145]; //g/cm^2
  double dummy;

  for(int i=0;i<145;i++)
    {
      inRangeFile>>dummy;
      inRangeFile>>P[i];
      for(int j=0;j<6;j++)
	{
	  inRangeFile>>dummy;
	}
      inRangeFile>>Range[i];
      std::cout<<Range[i]<<std::endl;
      inRangeFile.getline(line,200);
    }
  
  if(inRangeFile.bad()) 
    {
      fprintf(stderr,"failed to load muon range file\n"); 
      return false;
    }
  muonRange=new TSpline3("muRange",P,Range,145);

  return true;

}

bool UpMuFluxGen::CheckUpMu_Nu(TVector3 pos,TVector3 dir,float p)
{
  double dist=pos.Mag();
  double x0=10.5;
  //check upward goingness
  

  /* double upangle=acos(dir[2]);
  fprintf(stderr,"upangle %f theta_mu %f theta_nu %f\n",upangle,theta_mu,theta_nu);
  if((upangle-1*(theta_mu+theta_nu))>2.*Pi/3.)
    {
      fprintf(stderr,"Regected downgoing muon\n");
      //too downward going 
      return false;
      }*/

  if(dist<rmax) return true; //already inside vicinity
  double Range=3*muonRange->Eval(p)/(detector->GetDensity()/1000.);//kg/m^3-->g/cm^3

  // fprintf(stderr,"Mu_Nu p %f, range %f dist %f \n",p,Range,dist);

  if(dist-rmax>Range) 
    {

      //cannot reach vicinity of detector
      return false;
    }

  //compute angle between direction to center of vicinity sphere and tangential path to vicinity sphere

  double theta_tan=asin(rmax/dist);

  //compute angle between momentum direction and direction to center of vicinity sphere

  double theta_pdir=acos(-pos.Dot(dir)/dist);
 //compute multiple scatter thetarms and ccqe rms
  double theta_mu=sqrt(2)*(13.6/p)*sqrt(dist/x0)*(1+0.038*log(dist/x0));
  double theta_nu=sqrt(2)*(3.14/180)*2.6*sqrt(100000./p);
 
  //fprintf(stderr,"theta_ta %f theta_pdir %f theta_mu %f theta_nu %f\n",theta_tan,theta_pdir,theta_mu,theta_nu);
  if(theta_pdir>theta_tan+6*(theta_mu+theta_nu))
    {

      //will miss vicinity sphere at 6 sigma
      return false;
    }


  return true;


}



bool UpMuFluxGen::CheckUpMu(TVector3 pos,TVector3 dir,float p)
{
  double dist=pos.Mag();
  //fprintf(stderr,"dist %f\n",dist);
  if(dist<rmax) return true; //already inside vicinity
  double Range=3*muonRange->Eval(p)/(detector->GetDensity()/1000.);//kg/m^3-->g/cm^^3
  double x0=10.5;
  //fprintf(stderr,"p %f, range %f\n",p,Range);

  if(dist-rmax>Range) 
    {
      //cannot reach vicinity of detector
      return false;
    }

  //compute angle between direction to center of vicinity sphere and tangential path to vicinity sphere

  double theta_tan=asin(rmax/dist);

  //compute angle between momentum direction and direction to center of vicinity sphere

  double theta_pdir=acos(-pos.Dot(dir)/dist);

  //compute multiple scatter thetarms
  double theta_mu=sqrt(2)*(13.6/p)*sqrt(dist/x0)*(1+0.038*log(dist/x0));
 

  if(theta_pdir>theta_tan+8*theta_mu)
    {
      //will miss vicinity sphere at 4 sigma
      return false;
    }

  return true;


}

bool UpMuFluxGen::CheckEHook(float p,TVector3 pos)
{
  double dist=pos.Mag();
 
  if(dist<rmax) return true; //already inside vicinity
  double Range=3*muonRange->Eval(p)/(detector->GetDensity()/1000.);//kg/m^3-->g/cm^^3
 
 

  if(dist-rmax>Range) 
    {
      failedEHookCount++;
      //cannot reach vicinity of detector
      return false;
    }

  return true;
}


void UpMuFluxGen::RotateNuTrack()
{
  //Rotate incoming neutrino interaction position so that momentum points parallel to "R" aixs to detector (rotated volume technique)

  //want "R" axis of rotated upmu volume to be in opposite direction as neutrino momentum


  /*
  Track * t=currentVect->GetTrack(0);
  TVector3 v(-t->pdir[0],-t->pdir[1],-t->pdir[2]);

  double theta=v.Theta();
  double phi=v.Phi();

  TVector3 pos(currentVect->vertex[0],currentVect->vertex[1],currentVect->vertex[2]);
  pos.RotateY(theta);
  pos.RotateZ(phi);
  currentVect->vertex[0]=pos.X();
  currentVect->vertex[1]=pos.Y();
  currentVect->vertex[2]=pos.Z();
  */


}
