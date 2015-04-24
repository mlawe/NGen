#include "GenieFluxGen.h"
#include <cmath>
#include "Utils/XSecSplineList.h"
#include "Interaction/InitialState.h"
#include "EVGDrivers/GEVGDriver.h"
#include "TLorentzVector.h"
#include "GHEP/GHepParticle.h"
#include "EVGCore/EventRecord.h"
#include "TObjArray.h"
#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif



GenieFluxGen::GenieFluxGen():FluxGen::FluxGen()
  {


  }

GenieFluxGen::~GenieFluxGen()
{

 
  /*  if(rndm)
    delete rndm;
  if(evtRate)
    delete evtRate;
  if(currentVect)
    delete currentVect;
  fluxCSIntE.clear();
  fluxCSIntMu.clear();
  fluxCSIntEBar.clear();
  fluxCSIntMuBar.clear();
  */

  /*  delete g_nu_e_H_nc;
  delete g_nu_e_bar_H_nc;
  delete g_nu_mu_H_nc;
  delete g_nu_mu_bar_H_nc;
  delete g_nu_tau_H_nc;
  delete g_nu_tau_bar_H_nc;

  delete g_nu_e_O16_nc;
  delete g_nu_e_bar_O16_nc;
  delete g_nu_mu_O16_nc;
  delete g_nu_mu_bar_O16_nc;
  delete g_nu_tau_O16_nc;
  delete g_nu_tau_bar_O16_nc;

  delete g_nu_e_H_cc;
  delete g_nu_e_bar_H_cc;
  delete g_nu_mu_H_cc;
  delete g_nu_mu_bar_H_cc;
  delete g_nu_tau_H_cc;
  delete g_nu_tau_bar_H_cc;

  delete g_nu_e_O16_cc;
  delete g_nu_e_bar_O16_cc;
  delete g_nu_mu_O16_cc;
  delete g_nu_mu_bar_O16_cc;
  delete g_nu_tau_O16_cc;
  delete g_nu_tau_bar_O16_cc;
  */
  delete evg_driver_nu_e_H;
  delete evg_driver_nu_e_bar_H;
  delete evg_driver_nu_mu_H;
  delete evg_driver_nu_mu_bar_H;
  delete evg_driver_nu_tau_H;
  delete evg_driver_nu_tau_bar_H;
 
  delete evg_driver_nu_e_O16;
  delete evg_driver_nu_e_bar_O16;
  delete evg_driver_nu_mu_O16;
  delete evg_driver_nu_mu_bar_O16;
  delete evg_driver_nu_tau_O16;
  delete evg_driver_nu_tau_bar_O16;
}

int GenieFluxGen::Setup()
{
    genie::XSecSplineList * xspl=genie::XSecSplineList::Instance();
    xspl->LoadFromXml("/disk2/usr5/kachulis/local/share/genie/splines/gxspl-t2k-v2.8.0.xml");
    //    xspl->LoadFromXml("/disk2/usr5/kachulis/local/share/genie/splines/gxspl-t2k-v2.8.0.xml");
    genie::InitialState init_state_nu_e_H(1,1,12);
    genie::InitialState init_state_nu_e_bar_H(1,1,-12);
    genie::InitialState init_state_nu_e_O16(1000080160,12);
    genie::InitialState init_state_nu_e_bar_O16(1000080160,-12);

    genie::InitialState init_state_nu_mu_H(1,1,14);
    genie::InitialState init_state_nu_mu_bar_H(1,1,-14);
    genie::InitialState init_state_nu_mu_O16(1000080160,14);
    genie::InitialState init_state_nu_mu_bar_O16(1000080160,-14);

    genie::InitialState init_state_nu_tau_H(1,1,14); //hack for now, need to produce splines for tau
    genie::InitialState init_state_nu_tau_bar_H(1,1,-14);
    genie::InitialState init_state_nu_tau_O16(1000080160,14);
    genie::InitialState init_state_nu_tau_bar_O16(1000080160,-14);

    evg_driver_nu_e_H=new genie::GEVGDriver();
    evg_driver_nu_e_H->Configure(init_state_nu_e_H);
    evg_driver_nu_e_H->UseSplines();

    evg_driver_nu_e_bar_H=new genie::GEVGDriver();
    evg_driver_nu_e_bar_H->Configure(init_state_nu_e_bar_H);
    evg_driver_nu_e_bar_H->UseSplines();

    evg_driver_nu_mu_H=new genie::GEVGDriver();
    evg_driver_nu_mu_H->Configure(init_state_nu_mu_H);
    evg_driver_nu_mu_H->UseSplines();

    evg_driver_nu_mu_bar_H=new genie::GEVGDriver();
    evg_driver_nu_mu_bar_H->Configure(init_state_nu_mu_bar_H);
    evg_driver_nu_mu_bar_H->UseSplines();

    evg_driver_nu_tau_H=new genie::GEVGDriver();
    evg_driver_nu_tau_H->Configure(init_state_nu_tau_H);
    evg_driver_nu_tau_H->UseSplines();

    evg_driver_nu_tau_bar_H=new genie::GEVGDriver();
    evg_driver_nu_tau_bar_H->Configure(init_state_nu_tau_bar_H);
    evg_driver_nu_tau_bar_H->UseSplines();

    evg_driver_nu_e_O16=new genie::GEVGDriver();
    evg_driver_nu_e_O16->Configure(init_state_nu_e_O16);
    evg_driver_nu_e_O16->UseSplines();

    evg_driver_nu_e_bar_O16=new genie::GEVGDriver();
    evg_driver_nu_e_bar_O16->Configure(init_state_nu_e_bar_O16);
    evg_driver_nu_e_bar_O16->UseSplines();

    evg_driver_nu_mu_O16=new genie::GEVGDriver();
    evg_driver_nu_mu_O16->Configure(init_state_nu_mu_O16);
    evg_driver_nu_mu_O16->UseSplines();

    evg_driver_nu_mu_bar_O16=new genie::GEVGDriver();
    evg_driver_nu_mu_bar_O16->Configure(init_state_nu_mu_bar_O16);
    evg_driver_nu_mu_bar_O16->UseSplines();

    evg_driver_nu_tau_O16=new genie::GEVGDriver();
    evg_driver_nu_tau_O16->Configure(init_state_nu_tau_O16);
    evg_driver_nu_tau_O16->UseSplines();

    evg_driver_nu_tau_bar_O16=new genie::GEVGDriver();
    evg_driver_nu_tau_bar_O16->Configure(init_state_nu_tau_bar_O16);
    evg_driver_nu_tau_bar_O16->UseSplines();
   


    return 0;

}
double GenieFluxGen::GetCrossSection(NEUTRINO::FLAVOR flav,float E)
{
  double ret=0;

  const TLorentzVector nu_p4(0,0,E,E);
  
  if(flav==NEUTRINO::E)
    {
      ret=2*evg_driver_nu_e_H->XSecSum(nu_p4)+evg_driver_nu_e_O16->XSecSum(nu_p4);
	    //ret=evg_driver_nu_e_H->XSecSum(nu_p4);
    }
  if(flav==NEUTRINO::E_BAR)
    {
      ret=2*evg_driver_nu_e_bar_H->XSecSum(nu_p4)+evg_driver_nu_e_bar_O16->XSecSum(nu_p4);
      //ret=evg_driver_nu_e_bar_H->XSecSum(nu_p4);
    }
  if(flav==NEUTRINO::MU)
    {
	  ret=2*evg_driver_nu_mu_H->XSecSum(nu_p4)+evg_driver_nu_mu_O16->XSecSum(nu_p4);

    }
  if(flav==NEUTRINO::MU_BAR)
    {
	  ret=2*evg_driver_nu_mu_bar_H->XSecSum(nu_p4)+evg_driver_nu_mu_bar_O16->XSecSum(nu_p4);

    }
  if(flav==NEUTRINO::TAU)
    {
      	  ret=2*evg_driver_nu_tau_H->XSecSum(nu_p4)+evg_driver_nu_tau_O16->XSecSum(nu_p4);
    }
  if(flav==NEUTRINO::TAU_BAR)
    {
      ret=2*evg_driver_nu_tau_bar_H->XSecSum(nu_p4)+evg_driver_nu_tau_bar_O16->XSecSum(nu_p4);
    }
  ret=ret*3.894E10/18.; //convert from natural units to 10^-32 cm^2



  return ret;


}

int GenieFluxGen::ProcessIncomingTrack()
{


  genie::GEVGDriver * evg_driver;
  
  
  Track * track=currentVect->GetTrack(0);
  double xsec_H;
  double xsec_O16;
  TLorentzVector nu_p4(0,0,track->momentum*1E-3,track->momentum*1E-3);
  float E_GeV=track->momentum*1e-3;
  float fluxRatio;
  Time_Struct t;
  if(track->parID==12)
    {
      fluxRatio=flxTbl->GetFlux(E_GeV,NEUTRINO::MU,t)/flxTbl->GetFlux(E_GeV,NEUTRINO::E,t);
    }
  else if(track->parID==-12)
    {
      fluxRatio=flxTbl->GetFlux(E_GeV,NEUTRINO::MU_BAR,t)/flxTbl->GetFlux(E_GeV,NEUTRINO::E_BAR,t);
    }
  else if(track->parID==14)
    {
      fluxRatio=flxTbl->GetFlux(E_GeV,NEUTRINO::E,t)/flxTbl->GetFlux(E_GeV,NEUTRINO::MU,t);
    }
  else if(track->parID==-14)
    {
      fluxRatio=flxTbl->GetFlux(E_GeV,NEUTRINO::E_BAR,t)/flxTbl->GetFlux(E_GeV,NEUTRINO::MU_BAR,t);
    }
  currentVect->SetUserData<float>("fluxRatio",fluxRatio);

  switch(track->parID)
    {
    case 12:
      xsec_H=evg_driver_nu_e_H->XSecSum(nu_p4);
      xsec_O16=evg_driver_nu_e_O16->XSecSum(nu_p4);
      break;
    case -12:
      xsec_H=evg_driver_nu_e_bar_H->XSecSum(nu_p4);
      xsec_O16=evg_driver_nu_e_bar_O16->XSecSum(nu_p4);
      break;
    case 14:
      xsec_H=evg_driver_nu_mu_H->XSecSum(nu_p4);
      xsec_O16=evg_driver_nu_mu_O16->XSecSum(nu_p4);
      break;
    case -14:
      xsec_H=evg_driver_nu_mu_bar_H->XSecSum(nu_p4);
      xsec_O16=evg_driver_nu_mu_bar_O16->XSecSum(nu_p4);
      break;
    case 16:
      xsec_H=evg_driver_nu_tau_H->XSecSum(nu_p4);
      xsec_O16=evg_driver_nu_tau_O16->XSecSum(nu_p4);
      break;
    case -16:
      xsec_H=evg_driver_nu_tau_bar_H->XSecSum(nu_p4);
      xsec_O16=evg_driver_nu_tau_bar_O16->XSecSum(nu_p4);
      break;
    }

  xsec_H*=1.E10;
  xsec_O16*=1.E10;
  double mag=2*xsec_H+xsec_O16;

  fprintf(stderr,"ration %e\n",xsec_O16/mag);
  //choose target H or O16
  if(track->parID==12)
    {
      double test=rndm->Rndm()*mag;
      fprintf(stderr,"test %f, mag %f, 016 %f.....  ",test,mag,xsec_O16);
      if(test<xsec_O16)
	{
	  fprintf(stderr,"O16\n");
	  evg_driver=evg_driver_nu_e_O16;
	}
      else
	{
	  fprintf(stderr,"H\n");
	  evg_driver=evg_driver_nu_e_H;
	}
    }
  if(track->parID==-12)
    {
      double test=rndm->Rndm()*mag;
      if(test<xsec_O16)
	{
	  evg_driver=evg_driver_nu_e_bar_O16;
	}
      else
	{
	  evg_driver=evg_driver_nu_e_bar_H;
	}
    }
  if(track->parID==14)
    {
      double test=rndm->Rndm()*mag;
      if(test<xsec_O16)
	{
	  evg_driver=evg_driver_nu_mu_O16;
	}
      else
	{
	  evg_driver=evg_driver_nu_mu_H;
	}
    }
  if(track->parID==-14)
    {
      double test=rndm->Rndm()*mag;
      if(test<xsec_O16)
	{
	  evg_driver=evg_driver_nu_mu_bar_O16;
	}
      else
	{
	  evg_driver=evg_driver_nu_mu_bar_H;
	}
    }
  if(track->parID==16)
    {
      double test=rndm->Rndm()*mag;
      if(test<xsec_O16)
	{
	  evg_driver=evg_driver_nu_tau_O16;
	}
      else
	{
	  evg_driver=evg_driver_nu_tau_H;
	}
    }
  if(track->parID==-16)
    {
      double test=rndm->Rndm()*mag;
      if(test<xsec_O16)
	{
	  evg_driver=evg_driver_nu_tau_bar_O16;
	}
      else
	{
	  evg_driver=evg_driver_nu_tau_bar_H;
	}
    }

  /*  if(Mode==2)
    {
      //run upmu checks
      if(!CheckUpMu_Nu((float*)currentVect->vertex,track->pdir,track->momentum))
	{
	  //skip
	  return 1;
	}
      

	}*/
  nu_p4.SetPx(track->pdir[0]*track->momentum*1E-3);
  nu_p4.SetPy(track->pdir[1]*track->momentum*1E-3);
  nu_p4.SetPz(track->pdir[2]*track->momentum*1E-3);
  nu_p4.SetE(track->momentum*1E-3);
  //simulate event
  genie::EventRecord * event=evg_driver->GenerateEvent(nu_p4);
  //set mode need to figure this out
  currentVect->intType=(int)(event->Summary()->ProcInfo().InteractionTypeId());
  //loop through resulting tracks and store in currentVect


  genie::GHepParticle *p=0;
  TObjArrayIter piter(event);
  Track * tmpTrack;

  //run through mother particles

  piter.Next(); //neutrino already in currentVect
  p=(genie::GHepParticle *) piter.Next(); //target

  
  tmpTrack=currentVect->AddNewTrack();
  
  tmpTrack->parID=p->Pdg();
  tmpTrack->mass=p->Mass()*1.e3;
  tmpTrack->status=p->Status();
  float P=sqrt(pow(p->Px(),2)+pow(p->Py(),2)+pow(p->Pz(),2));
  tmpTrack->momentum=P*1E3;  //convert to MeV;
  if(P>0)
    {
      double x=p->Px()/P;
      double y=p->Py()/P;
      double z=p->Pz()/P;



      tmpTrack->pdir.SetXYZ(x,y,z);
    }
  else
    {
      tmpTrack->pdir.SetXYZ(0,0,0);
    }
  
  //continue through daughters, only save final states
  //want neutrino daughter next, so run though and save finals in vector
  std::vector<genie::GHepParticle > pV;
  while( (p=(genie::GHepParticle *) piter.Next()))
    {
      //      if(p->Status()!=1)
      //	continue;

      if(p->Pdg()>1999999999) continue;
      if(p->FirstMother()==0)
	{
	  //this is daughter of neutrino
	  
	  tmpTrack=currentVect->AddNewTrack();
  
	  tmpTrack->parID=p->Pdg();
	  tmpTrack->mass=p->Mass()*1.e3;
	  float P=sqrt(pow(p->Px(),2)+pow(p->Py(),2)+pow(p->Pz(),2));
	  tmpTrack->momentum=P*1E3;  //convert to MeV;
	  tmpTrack->status=p->Status();
	  if(P>0)
	    {
	      double x=p->Px()/P;
	      double y=p->Py()/P;
	      double z=p->Pz()/P;
	      

	      
	      tmpTrack->pdir.SetXYZ(x,y,z);


	    }
	  else
	    {

	      tmpTrack->pdir.SetXYZ(0,0,0);
	    }
  
	  break;
	}
      else
	{
	  pV.push_back(*p);
	}
    }
  //run through saved daughters
  for(size_t i=0;i<pV.size();i++)
    {


      tmpTrack=currentVect->AddNewTrack();
  
      tmpTrack->parID=pV[i].Pdg();
      tmpTrack->mass=pV[i].Mass()*1.e3;
      float P=sqrt(pow(pV[i].Px(),2)+pow(pV[i].Py(),2)+pow(pV[i].Pz(),2));
      tmpTrack->momentum=P*1E3;  //convert to MeV;
      tmpTrack->status=pV[i].Status();
      if(P>0)
	{
	  double x=p->Px()/P;
	  double y=p->Py()/P;
	  double z=p->Pz()/P;
	  
	  
	  
	  tmpTrack->pdir.SetXYZ(x,y,z);
	    
	    
	}
      else
	{
	  
	  tmpTrack->pdir.SetXYZ(0,0,0);
	}
      
      

    }
  
  pV.clear();

  //finally run through rest of daughters
  while( (p=(genie::GHepParticle *) piter.Next()))
    {
      //      if(p->Status()!=1) //not final state particles
      //	continue;
      if(p->Pdg()>1999999999) continue;
	  //this is daughter of neutrino
	  
      tmpTrack=currentVect->AddNewTrack();
  
      tmpTrack->parID=p->Pdg();
      tmpTrack->mass=p->Mass()*1.e3;
      float P=sqrt(pow(p->Px(),2)+pow(p->Py(),2)+pow(p->Pz(),2));
      tmpTrack->momentum=P*1E3;  //convert to MeV;
      tmpTrack->status=p->Status();
      if(P>0)
	{
	  double x=p->Px()/P;
	  double y=p->Py()/P;
	  double z=p->Pz()/P;
	  
	  
	  
	  tmpTrack->pdir.SetXYZ(x,y,z);
	    
	    
	}
      else
	{
	  
	  tmpTrack->pdir.SetXYZ(0,0,0);
	}
      
    }
  //cleanup
  delete event;

  return 0;
}
