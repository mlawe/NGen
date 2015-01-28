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
int FluxGen::CreateEvtRate()
{

  if(!flxTbl)
    {
      fprintf(stderr,"Error:Flux tables not loaded yet\n");
      return -1;
    }
  if(nYears<0)
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
  fluxCSIntE.clear();
  fluxCSIntEBar.clear();
  fluxCSIntMu.clear();
  fluxCSIntMuBar.clear();
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
  float E;
  
  for(size_t iE=0;iE<(EVec.size()-1);iE++)
    {

      
      E=(EVec[iE]+EVec[iE+1])/2;
      ebin=EVec[iE+1]-EVec[iE];
      double fCSE=flxTbl->GetFlux(E,NEUTRINO::E)*GetCrossSec(NEUTRINO::E,E);
      double fCSEBAR=flxTbl->GetFlux(E,NEUTRINO::E_BAR)*GetCrossSec(NEUTRINO::E_BAR,E);
      double fCSMU=flxTbl->GetFlux(E,NEUTRINO::MU)*GetCrossSec(NEUTRINO::MU,E);
      double fCSMUBAR=flxTbl->GetFlux(E,NEUTRINO::MU_BAR)*GetCrossSec(NEUTRINO::MU_BAR,E);
      //      if(E<10)fprintf(stderr,"E %f, %e, %e\n",E,flxTbl->GetFlux(E,NEUTRINO::MU_BAR)/(4*Pi),GetCrossSec(NEUTRINO::MU_BAR,E));
      if(fCSE<0 or fCSEBAR<0 or fCSMU<0 or fCSMUBAR<0)
	{
	  return -1;
	}
      if(iE==0)
	{
	  fluxCSIntE.push_back(fCSE*ebin);
	  fluxCSIntMu.push_back(fCSMU*ebin);
	  fluxCSIntEBar.push_back(fCSEBAR*ebin);
	  fluxCSIntMuBar.push_back(fCSMUBAR*ebin);
	  
	}
      else
	{
	  fluxCSIntE.push_back(fluxCSIntE.back()+fCSE*ebin);
	  fluxCSIntMu.push_back(fluxCSIntMu.back()+fCSMU*ebin);
	  fluxCSIntEBar.push_back(fluxCSIntEBar.back()+fCSEBAR*ebin);
	  fluxCSIntMuBar.push_back(fluxCSIntMuBar.back()+fCSMUBAR*ebin);
	}	 
      //      fprintf(stderr,"iE %zu E %f fluxcsint %e\n",iE,E,fluxCSIntE.back()+fluxCSIntMu.back()+fluxCSIntEBar.back()+fluxCSIntMuBar.back());
      tmpErt+=fCSE*ebin;
      tmpEBARrt+=fCSEBAR*ebin;
      tmpMUrt+=fCSMU*ebin;
      tmpMUBARrt+=fCSMUBAR*ebin;
      /*      if(E<10)
	{
	  fprintf(stderr,"E= %f,tmpMUBARrt = %f\n",E,tmpMUBARrt*time*pnum*1.e-38*1.e-4);
    
	  }*/
    }
  //  fprintf(stderr,"ratio %f\n",(fluxCSIntE[116]+fluxCSIntEBar[116]+fluxCSIntMu[116]+fluxCSIntMuBar[116])/(fluxCSIntE.back()+fluxCSIntMu.back()+fluxCSIntEBar.back()+fluxCSIntMuBar.back()));
  evtRate->e=(int)(tmpErt*time*pnum*fidMass*nYears*1.e-38*1.e-4);//last two factors convert 1e-38 cm^2 units to m^2 units
  evtRate->e_bar=(int)(tmpEBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->mu=(int)(tmpMUrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->mu_bar=(int)(tmpMUBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
  evtRate->all=evtRate->e+evtRate->e_bar+evtRate->mu+evtRate->mu_bar;

  fprintf(stderr,"e %f e_bar %f, mu %f mubar %f\n",tmpErt*time*pnum*fidMass*nYears*1.e-38*1.e-4,tmpEBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4,tmpMUrt*time*pnum*fidMass*nYears*1.e-38*1.e-4,tmpMUBARrt*time*pnum*fidMass*nYears*1.e-38*1.e-4);
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
  if(evtRate->all==0)
    {
      return 2;
    }

  //load up new empty vetor
  float evtN=0;
  if(currentVect)
    {
      evtN=currentVect->evtNumber+1;
      delete currentVect;
    }
  currentVect=new EvtVector();
  currentVect->evtNumber=evtN;
  //create first track, incoming neutrino
  Track* tmpTrack=currentVect->AddNewTrack();

  //now choose flavor
  double test=rndm->Rndm();

  double testFlav=test*evtRate->all;
  NEUTRINO::FLAVOR flav;
  if(testFlav<=(double)evtRate->e)
    {
      flav=NEUTRINO::E;
      tmpTrack->parID=12;
      evtRate->e-=1;
      evtRate->all-=1;
    }
  else if(testFlav<=(double)(evtRate->e+evtRate->e_bar))
    {
      flav=NEUTRINO::E_BAR;
      tmpTrack->parID=-12;
      evtRate->e_bar-=1;
      evtRate->all-=1;
    }
  else if(testFlav<=(double)(evtRate->e+evtRate->e_bar+evtRate->mu))
    {
      flav=NEUTRINO::MU;
      tmpTrack->parID=14;
      evtRate->mu-=1;
      evtRate->all-=1;
    }
  else 
    {
      flav=NEUTRINO::MU_BAR;
      tmpTrack->parID=-14;
      evtRate->mu_bar-=1;
      evtRate->all-=1;
    }
  for(int i=0;i<3;i++)
    {
      currentVect->vertex[i]=0;
    }
  detector->GetRandPos(currentVect->vertex,rndm); 
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
      
      for(int i=0;i<3;i++)
	{

	  tmpTrack->pdir[i]=0;
	}
      //Get position of vertex
      if(!CheckEHook(tmpTrack->momentum,currentVect->vertex)) return 1;

      //get incoming direction of neutrino 
      
      if(GetRandDirection(flav,e,tmpTrack->pdir)<0)
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

int FluxGen::GetRandDirection(NEUTRINO::FLAVOR flav, float e, float dir[3])
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


int FluxGen::GetRandDir(NEUTRINO::FLAVOR flav,float e,float dir[3])
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
	  flx=flxTbl->GetFlux(phi,cosZ,e,flav);
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
      if(flxTbl->GetFlux(phi,cosZ,e,flav)>flxMax*rndm->Rndm())
	{
	  break;
	}
      if(flxTbl->GetFlux(phi,cosZ,e,flav)<0)
	{
	  return -1;
	}
      
    }
  dir[0]=cos(phi)*sqrt(1-pow(cosZ,2));
  dir[1]=sin(phi)*sqrt(1-pow(cosZ,2));
  dir[2]=cosZ;
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
  float emin=flxTbl->GetEnergyBins().front();
  float emax=flxTbl->GetEnergyBins().back();

  float b=specinds[flav];
  float A=ratios[flav];
  float e;
  while(true)
    {
      float etmp=rndm->Rndm()*(pow(emax,b+1)-pow(emin,b+1))+pow(emin,b+1);

      e=pow(etmp,1/(b+1));
      float test=A*pow(e,b);
      float flxCS=flxTbl->GetFlux(e,flav)*GetCrossSec(flav,e);
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
    case NEUTRINO::NUMBER:
      return -1;
    }
  double r=rndm->Rndm()*fluxCSInt.back();
  int iEbin=-1;
  for(int i=0;i<(int)fluxCSInt.size();i++)
    {
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
      if(flxTbl->GetFlux(e,flav)*GetCrossSec(flav,e)>rndm->Rndm()*test)
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
