#include "FluxGen.h"
#include <vector>
#include <cmath>
#include <stdlib.h>

#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif

FluxGen::FluxGen()
{
  rndm=new TRandom3(0);
  flxTbl=NULL;
  currentVect=NULL;
  evtRate=NULL;
  detector=NULL;
  nYears=-1;

  vX=new TVector3(1,0,0);
  vY=new TVector3(0,1,0);
  vZ=new TVector3(0,0,1);

  specinds.push_back(-2.3); //e
  specinds.push_back(-2.1); //ebar 
  specinds.push_back(-2.0); //mu
  specinds.push_back(-1.9); //mubar

  ratios.push_back(400.*4*Pi); //e
  ratios.push_back(130.*4*Pi); //ebar
  ratios.push_back(600.*4*Pi); //mu
  ratios.push_back(250.*4*Pi); //mubar

  EMode=0;
  dirMode=0;
  e_thresh=0;
  e_max=100000000;

  //count=0;
  //count2=0;
  //count3=0;
}

FluxGen::~FluxGen()
{
 
  if(rndm)
    delete rndm;
  if(evtRate)
    delete evtRate;
  if(currentVect)
    delete currentVect;
  fluxCSIntE.clear();
  fluxCSIntMu.clear();
  fluxCSIntEBar.clear();
  fluxCSIntMuBar.clear();

  specinds.clear();
  ratios.clear();
  delete vX;
  delete vY;
  delete vZ;
}


int FluxGen::LoadFluxTable(std::string fluxFileN)
{
  if(!flxTbl)
    {
      fprintf(stderr,"Error:no flux table set\n");
      return -1;
    }
  
  std::ifstream fileStrm;
  fileStrm.open(fluxFileN.c_str());
  int ret=flxTbl->LoadFluxTable(fileStrm);
  fileStrm.close();
  return ret;
}

void FluxGen::SetSolarAct(float s)
{
  flxTbl->SetSolarAct(s);
}

void FluxGen::SetSeed(unsigned seed)
{
  rndm->SetSeed(seed);
  fprintf(stderr,"New Seed set to %u\n",seed);
}

void FluxGen::SetNYears(float y)
{
  nYears=y;
}

void FluxGen::SetDetector(DetectorGeom * t)
{
  detector=t;
}
void FluxGen::SetFlxTable(FlxTable* tbl)
{
  flxTbl=tbl;

}
int FluxGen::CreateEvtRate(Time_Struct t)
{

  if(!flxTbl)
    {
      fprintf(stderr,"Error:Flux tables not loaded yet\n");
      return -1;
    }
  if(nYears<0 and !(flxTbl->IsTimeDependent()))
    {
      fprintf(stderr,"Error: nYears not set yet \n");
      return -1;
    }
  if(!detector)
    {
      fprintf(stderr,"Error:Detector not set yet \n");
      return -1;
    }
  if(evtRate)
    {
      delete evtRate;
    }
  evtRate=new EvtRate();
  evtRate->time=t;
  fluxCSIntE.clear();
  fluxCSIntEBar.clear();
  fluxCSIntMu.clear();
  fluxCSIntMuBar.clear();
  fluxCSIntTau.clear();
  fluxCSIntTauBar.clear();
  double pnum=6.03e32; //nucleons per kton of water
  double time=3.15576e7; //seconds in a year
  double fidMass=detector->GetMass()/1.e6;//fiducial mass in kilotons;
  
  if(fidMass<0)
    {
      fprintf(stderr,"Error: density of detector not set yet \n");
      return -1;
    }
  
  //Get energy bins from flxTbl
  std::vector<float> EVec=flxTbl->GetEnergyBins();
  float ebin;
  double tmpErt=0;
  double tmpMUrt=0;
  double tmpEBARrt=0;
  double tmpMUBARrt=0;
  double tmpTAUrt=0;
  double tmpTAUBARrt=0;
  float E;
  
  for(size_t iE=0;iE<(EVec.size()-1);iE++)
    {

      
      E=(EVec[iE]+EVec[iE+1])/2;
      ebin=EVec[iE+1]-EVec[iE];
      double fCSE=flxTbl->GetFlux(E,NEUTRINO::E,evtRate->time)*GetCrossSec(NEUTRINO::E,E);
      double fCSEBAR=flxTbl->GetFlux(E,NEUTRINO::E_BAR,evtRate->time)*GetCrossSec(NEUTRINO::E_BAR,E);
      double fCSMU=flxTbl->GetFlux(E,NEUTRINO::MU,evtRate->time)*GetCrossSec(NEUTRINO::MU,E);
      double fCSMUBAR=flxTbl->GetFlux(E,NEUTRINO::MU_BAR,evtRate->time)*GetCrossSec(NEUTRINO::MU_BAR,E);
      double fCSTAU=flxTbl->GetFlux(E,NEUTRINO::TAU,evtRate->time)*GetCrossSec(NEUTRINO::TAU,E);
      double fCSTAUBAR=flxTbl->GetFlux(E,NEUTRINO::TAU_BAR,evtRate->time)*GetCrossSec(NEUTRINO::TAU_BAR,E);




      if(fCSE<0 or fCSEBAR<0 or fCSMU<0 or fCSMUBAR<0 or fCSTAU<0 or fCSTAUBAR<0)
	{
	  return -1;
	}
      if(iE==0)
	{
	  fluxCSIntE.push_back(fCSE*ebin);
	  fluxCSIntMu.push_back(fCSMU*ebin);
	  fluxCSIntEBar.push_back(fCSEBAR*ebin);
	  fluxCSIntMuBar.push_back(fCSMUBAR*ebin);
	  fluxCSIntTauBar.push_back(fCSTAUBAR*ebin);
	  fluxCSIntTau.push_back(fCSTAU*ebin);
	  
	}
      else
	{
	  fluxCSIntE.push_back(fluxCSIntE.back()+fCSE*ebin);
	  fluxCSIntMu.push_back(fluxCSIntMu.back()+fCSMU*ebin);
	  fluxCSIntEBar.push_back(fluxCSIntEBar.back()+fCSEBAR*ebin);
	  fluxCSIntMuBar.push_back(fluxCSIntMuBar.back()+fCSMUBAR*ebin);
	  fluxCSIntTau.push_back(fluxCSIntTau.back()+fCSTAU*ebin);
	  fluxCSIntTauBar.push_back(fluxCSIntTauBar.back()+fCSTAUBAR*ebin);
	}	 

      tmpErt+=fCSE*ebin;
      tmpEBARrt+=fCSEBAR*ebin;
      tmpMUrt+=fCSMU*ebin;
      tmpMUBARrt+=fCSMUBAR*ebin;
      tmpTAUrt+=fCSTAU*ebin;
      tmpTAUBARrt+=fCSTAUBAR*ebin;
      /*      if(E<10)
	{
	  fprintf(stderr,"E= %f,tmpMUBARrt = %f\n",E,tmpMUBARrt*time*pnum*1.e-38*1.e-4);
    
	  }*/
    }
  //  fprintf(stderr,"ratio %f\n",(fluxCSIntE[116]+fluxCSIntEBar[116]+fluxCSIntMu[116]+fluxCSIntMuBar[116])/(fluxCSIntE.back()+fluxCSIntMu.back()+fluxCSIntEBar.back()+fluxCSIntMuBar.back()));
  //fprintf(stderr,"tmpMurt %e tmpErt %e time %e pnum %e fidMass %e nYears %f\n",tmpMUrt,tmpErt, time,pnum,fidMass,nYears);
  evtRate->e=(int)(tmpErt*time*pnum*fidMass*nYears*1.e-38*1.e-4);//last two factors convert 1e-38 cm^2 units to m^2 units
  evtRate->e_bar=(int)(tmpEBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->mu=(int)(tmpMUrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->mu_bar=(int)(tmpMUBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->tau_bar=(int)(tmpTAUBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->tau=(int)(tmpTAUrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->all=evtRate->e+evtRate->e_bar+evtRate->mu+evtRate->mu_bar+evtRate->tau+evtRate->tau_bar;

  evtRate->e_rt=tmpErt*pnum*fidMass*1.e-38*1.e-4;//last two factors convert 1e-38 cm^2 units to m^2 units
  evtRate->e_bar_rt=tmpEBARrt*pnum*fidMass*1.e-38*1.e-4;
  evtRate->mu_rt=tmpMUrt*pnum*fidMass*1.e-38*1.e-4;
  evtRate->mu_bar_rt=tmpMUBARrt*pnum*fidMass*1.e-38*1.e-4;
  evtRate->tau_rt=tmpTAUrt*pnum*fidMass*1.e-38*1.e-4;
  evtRate->tau_bar_rt=tmpTAUBARrt*pnum*fidMass*1.e-38*1.e-4;
  evtRate->total_rt=evtRate->e_rt+evtRate->e_bar_rt+evtRate->mu_rt+evtRate->mu_bar_rt+evtRate->tau_rt+evtRate->tau_bar_rt;


  //  fprintf(stderr,"e %f e_bar %f, mu %f mubar %f\n",tmpErt*time*pnum*fidMass*nYears*1.e-38*1.e-4,tmpEBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4,tmpMUrt*time*pnum*fidMass*nYears*1.e-38*1.e-4,tmpMUBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
   fprintf(stderr,"evtRate %i\n",evtRate->all);
  
  return 0;
}


int FluxGen::CreateNextVector()
{


  if(!evtRate)
    {
      fprintf(stderr,"Error in CreateNextVector(): evtRate not yet filled\n");
      return -1;

    }

  //first check that evt rates add up
  if(!evtRate->Check())
    {
      fprintf(stderr,"Error: Event rates do not add up\n");
      return -1;
    }
  if(evtRate->all==0 and !(flxTbl->IsTimeDependent()))
    {
      return 2;
    }
  Time_Struct theTime=startTime;
  float evtN=0;
  float evtNwritten=0;
  if(currentVect)
    {
      
      theTime=currentVect->evtTime;
      evtN=currentVect->evtNumber+1;

      evtNwritten=currentVect->evtWrittenNumber;
      if(currentVect->IsWrittenOut()) evtNwritten++;
    }


  
  while(flxTbl->IsTimeDependent())
    {

      if(theTime>flxTbl->GetNextRecalcTime(evtRate->time))
	{
	  CreateEvtRate(theTime);
	}

      //choose time until next event
      double rate=evtRate->total_rt;//rate, evts per second
      /* if(rate<1.E-15) 
	{


	  continue;
	  }*/
      //  std::cout<<"rate "<<rate<<std::endl;
      //choose time til next event from exponential distribution

      double rnd=rndm->Rndm();
      double step=-log(1-rnd)/rate;//in nanoseconds
      int64_t seconds=(int64_t)(step);
      int64_t nanos=(int64_t)((step-seconds)*1.E9);
      //     std::cout<<"seconds "<<seconds<<" nanos "<<nanos<<std::endl;
      Time_Struct tStep(seconds,nanos);
      Time_Struct last_time=theTime;
      theTime+=tStep;
      //check if we have stepped too far.  can happen in low flux time interval, can step past flux increase.
      if(theTime<flxTbl->GetNextRecalcTime(last_time))
	{
	  //good, can use this time
	  break;
	  
	}
      //we need to step a small amount and try again
      theTime=flxTbl->GetNextRecalcTime(evtRate->time);
      
      if(theTime>endTime) return 2;
      CreateEvtRate(theTime);
      
    }

  if(theTime>endTime) return 2;

  //load up new empty vetor
  
  /*  if(currentVect)
    {
      evtN=currentVect->evtNumber+1;
      delete currentVect;
      }*/
  if(currentVect) delete currentVect;
  currentVect=new EvtVector();
  currentVect->evtNumber=evtN;
  currentVect->evtWrittenNumber=evtNwritten;
  currentVect->evtTime=theTime;
  //create first track, incoming neutrino
  Track* tmpTrack=currentVect->AddNewTrack();

  //now choose flavor
  double test=rndm->Rndm();
  double e_rt=(double)evtRate->e;
  double e_bar_rt=(double)evtRate->e_bar;
  double mu_rt=(double)evtRate->mu;
  double mu_bar_rt=(double)evtRate->mu_bar;
  double tau_rt=(double)evtRate->tau;
  double tau_bar_rt=(double)evtRate->tau_bar;
  double tot_rt=(double)evtRate->all;
  if(flxTbl->IsTimeDependent())
    {
      e_rt=evtRate->e_rt;
      e_bar_rt=evtRate->e_bar_rt;
      mu_rt=evtRate->mu_rt;
      mu_bar_rt=evtRate->mu_bar_rt;
      tau_rt=evtRate->tau_rt;
      tau_bar_rt=evtRate->tau_bar_rt;
      tot_rt=evtRate->total_rt;
    }
  if(tot_rt!=e_rt+e_bar_rt+mu_rt+mu_bar_rt+tau_rt+tau_bar_rt)
    {
      std::cerr<<"Event rate error in FluxGen::CreateNextEvent"<<std::endl;

      return 1;

    }

  double testFlav=test*tot_rt;
  NEUTRINO::FLAVOR flav;
  if(testFlav<=e_rt)
    {
      flav=NEUTRINO::E;
      tmpTrack->parID=12;
      evtRate->e-=1;
      evtRate->all-=1;
    }
  else if(testFlav<=(e_rt+e_bar_rt))
    {
      flav=NEUTRINO::E_BAR;
      tmpTrack->parID=-12;
      evtRate->e_bar-=1;
      evtRate->all-=1;
    }
  else if(testFlav<=(e_rt+e_bar_rt+mu_rt))
    {
      flav=NEUTRINO::MU;
      tmpTrack->parID=14;
      evtRate->mu-=1;
      evtRate->all-=1;
    }
  else if(testFlav<=(e_rt+e_bar_rt+mu_rt+mu_bar_rt))
    {
      flav=NEUTRINO::MU_BAR;
      tmpTrack->parID=-14;
      evtRate->mu_bar-=1;
      evtRate->all-=1;
    }
  else if(testFlav<=(e_rt+e_bar_rt+mu_rt+mu_bar_rt+tau_rt))
    {
    
      flav=NEUTRINO::TAU;
      tmpTrack->parID=16;
      evtRate->tau-=1;
      evtRate->all-=1;
    }
  else 
    {
      flav=NEUTRINO::TAU_BAR;
      tmpTrack->parID=-16;
      evtRate->tau_bar-=1;
      evtRate->all-=1;
    }


  currentVect->vertex=detector->GetRandPos(rndm); 
  tmpTrack->mass=0;

  while(true)
    {
      float e=GetRandE(flav); //GeV
      if(e<0)
	{
	  fprintf(stderr,"Error: Negative return from GetRandE %f\n",e);
	  return -1;
	}
      
      tmpTrack->momentum=e*1.e3;//convert GeV to MeV
      

      //Get position of vertex
      if(!CheckEHook(tmpTrack->momentum,currentVect->vertex)) return 1;

      //get incoming direction of neutrino 
      
      if(GetRandDirection(flav,e,&tmpTrack->pdir)<0)
	{
	  return -1;
	}
      

      //run the incoming neutino through nuclear simulation!!!!!!!!!!!
      int ret=ProcessIncomingTrack(tmpTrack);
   
      if(ret<0)
	{
	  continue; //if nuclear simulation successful finish, if failed choose new momentum and try again
	}
      //rotate coords into final desired coord system
      RotateCoords();

      return ret;
    } 

}

int FluxGen::GetRandDirection(NEUTRINO::FLAVOR flav, float e, TVector3 * dir)
{
  switch(dirMode)
    {
    case 0:
      return GetRandDir(flav,e,dir);
      break;
    case 1:
      return flxTbl->GetRandDir(flav,e,dir);
      break;
    default:
      std::cerr<<"dirMode not set to valid value in FluxGenerator"<<std::endl;
      exit(1);
      

    }


}


int FluxGen::GetRandDir(NEUTRINO::FLAVOR flav,float e,TVector3* dir)
{
  //return random direction by flux distro, e given in units of GeV

  //first get max flux value to compare against for metropolis monte carlo
  float cosZ;
  float phi;
  float flxMax=0;
  float flx;
  for(int i=0;i<40;i++)
    {
      for(int j=0;j<18;j++)
	{
	  cosZ=-1.+float(i)*0.05;
	  phi=float(j)*Pi/9.;
	  flx=flxTbl->GetFlux(phi,cosZ,e,flav,currentVect->evtTime);
	  if(flx>flxMax)
	    {
	      flxMax=flx;
	    }
	  if(flx<0)
	    {
	      return flx;
	    }
	}
    }

  flxMax=flxMax*2.;

  
  while(true)
    {
      cosZ=rndm->Rndm()*2.-1.;
      phi=2.*rndm->Rndm()*Pi;
      if(flxTbl->GetFlux(phi,cosZ,e,flav,currentVect->evtTime)>flxMax*rndm->Rndm())
	{
	  break;
	}
      if(flxTbl->GetFlux(phi,cosZ,e,flav,currentVect->evtTime)<0)
	{
	  return -1;
	}
      
    }
  dir->SetX(cos(phi)*sqrt(1-pow(cosZ,2)));
  dir->SetY(sin(phi)*sqrt(1-pow(cosZ,2)));
  dir->SetZ(cosZ);
  return 0;

}
float FluxGen::GetRandE(NEUTRINO::FLAVOR flav)
{
  switch(EMode)
    {
    case 0:
      return GetRandEStepping(flav);
      break;
    case 1:
      return GetRandEInversion(flav);
      break;
    default:
      fprintf(stderr,"EMode not set to valid value in FluxGenerator\n");
      exit(1);
    }

}
void FluxGen::SetSpectralIndecies(std::vector<float> si)
{
  if(si.size()!=4)
    {
      fprintf(stderr,"Spectral Indecies vector must have 4 entries\n");
      exit(1);
    }
  specinds=si;
}
void FluxGen::SetRatios(std::vector<float> rs)
{
  if(rs.size()!=4)
    {
      fprintf(stderr,"Ratios vector must have 4 entries\n");
      exit(1);
    }
  ratios=rs;
}

float FluxGen::GetRandEInversion(NEUTRINO::FLAVOR flav)
{
  if(ratios.size()!=4 or specinds.size()!=4)
    {
      fprintf(stderr,"Ratios and Spectral Indecies vectors wrond size\n");
      exit(1);

    }
  
  float emin=e_thresh;
  float emax=e_max;
  if(emin<flxTbl->GetEnergyBins().front()) emin=flxTbl->GetEnergyBins().front();
  if(emax>flxTbl->GetEnergyBins().back()) emax=flxTbl->GetEnergyBins().back();

  float b=specinds[flav];
  float A=ratios[flav];
  float e;
  while(true)
    {
      float etmp=rndm->Rndm()*(pow(emax,b+1)-pow(emin,b+1))+pow(emin,b+1);

      e=pow(etmp,1/(b+1));
      float test=A*pow(e,b);
      float flxCS=flxTbl->GetFlux(e,flav,currentVect->evtTime)*GetCrossSec(flav,e);
      if(flxCS>test)
	{
	  fprintf(stderr,"Error in random E inversion, flxCS>test\n");
	  exit(1);
	}
      if(flxCS>rndm->Rndm()*test) break;
    }
  return e;
}
float FluxGen::GetRandEStepping(NEUTRINO::FLAVOR flav)
{ 
  //This is basically a numerical inversion sampling to choose a bin and then metropolis to choose energy in that bin
 
  std::vector<double> fluxCSInt;
  switch(flav)
    {
    case NEUTRINO::E:
      fluxCSInt=fluxCSIntE;
      break;
    case NEUTRINO::MU:
      fluxCSInt=fluxCSIntMu;
      break;
    case NEUTRINO::E_BAR:
      fluxCSInt=fluxCSIntEBar;
      break;
    case NEUTRINO::MU_BAR:
      fluxCSInt=fluxCSIntMuBar;
      break;
    case NEUTRINO::TAU:
      fluxCSInt=fluxCSIntTau;
      break;
    case NEUTRINO::TAU_BAR:
      fluxCSInt=fluxCSIntTauBar;
      break;
    case NEUTRINO::NUMBER:
      return -1;
    
    }
  double r=rndm->Rndm()*fluxCSInt.back();
  int iEbin=-1;
  for(int i=0;i<(int)fluxCSInt.size();i++)
    {
      //      fprintf(stderr,"%e   %e\n",fluxCSInt[i],fluxCSInt.back());
      //      std::cout<<fluxCSInt[i]<<"  "<<fluxCSInt.back()<<std::endl;
      if(r<fluxCSInt[i])
	{
	  iEbin=i;
	  break;
	}

    }
  std::vector<float> Ebins=flxTbl->GetEnergyBins();
  float emin,emax;
 
  emin=Ebins[iEbin];
  emax=Ebins[iEbin+1];
  if(iEbin<0)
    {
      fprintf(stderr,"Error iEbin not set \n");
      return -1;
    }
  //  if(iEbin<=116) count2++;
  double test;
  if(iEbin==0)
    {
      test=2.*fluxCSInt[0]/(emax-emin);
    }
  else
    {
      test=2.*(fluxCSInt[iEbin]-fluxCSInt[iEbin-1])/(emax-emin);//flxTbl->GetFlux(Ebins[iEbin],flav)*GetCrossSec(flav,Ebins[iEbin]);
    }  
  float e;

  while(true)
    {

      e=rndm->Rndm()*(emax-emin)+emin;
      if(flxTbl->GetFlux(e,flav,currentVect->evtTime)*GetCrossSec(flav,e)>rndm->Rndm()*test)
	{
	  break;
	}

    }
  //  count++;
  //if(e<1) count3++;
  //  fprintf(stderr,"count %i, count2 %i, count3 %i\n",count,count2,count3);
  return e;
}

int FluxGen::WriteCurrentVector()
{
  if(Writers.size()==0)
    {
      fprintf(stderr,"No Writers \n");
      return -1;

    }

  for(size_t i=0;i<Writers.size();i++)
    {
      Writers[i]->WriteVector(currentVect);
      currentVect->WrittenOut();
    }

  return 0;
}


void FluxGen::AddNewWriter(Writer * writer,const char * name)
{
  writer->OpenFile(name);
  Writers.push_back(writer);

}

void FluxGen::CloseWriters()
{
  for(size_t i=0;i<Writers.size();i++)
    {
      Writers[i]->CloseFile();
    }
  Writers.clear();
}

void FluxGen::RotateCoords()
{
  //rotate from generator coords to detector coords.  Detector must be originally in generator using generator coords

  //first rotate vertex

  RotatePoint(&currentVect->vertex);
 
  //now rotate direction of all tracks
  size_t nTracks=currentVect->GetNumTracks();

  for(size_t i=0;i<nTracks;i++)
    {



      Track * track=currentVect->GetTrack(i);
      RotatePoint(&(track->pdir));



    }

  //done so clean up memory

    
}


void FluxGen::RotatePoint(TVector3 * v)
{
  double x=v->Dot(*vX);
  double y=v->Dot(*vY);
  double z=v->Dot(*vZ);
  v->SetXYZ(x,y,z);



}

double FluxGen::GetCrossSec(NEUTRINO::FLAVOR flav, float E)
{
  if(E<e_thresh or E>e_max)
    {
      return 0;

    }

  return GetCrossSection(flav,E);

}
