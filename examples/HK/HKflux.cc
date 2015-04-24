///////////////////////////////////////////////////////////////
/* 
Simulation to create atmospheric netutrino event vectors for SK, using Honda Flux (or other) 
tables and NEUT neutrino event generator.

Begun: Feb 05,2014-C. Kachulis

*//////////////////////////////////////////////////////////////// 

#include <cstdio>
#include <stdlib.h>
#include "TargetCyl.h"
#include "HonFlx11.h"
#include "HonFlx11Tau.h"
#include "HonFlx11UpMu.h"
#include "HonFlx.h"
#include "HonFlxTau.h"
#include "HonFlxUpMu.h"
#include "NuanceWriter.h"
#include "NeutRootWriter.h"
#include "DiffShape.h"
#include <fstream>
#include <string>
#ifdef NEUT
#include "NeutGen.h"
#include "necardatmC.h"
#include "nesolactC.h"
#include "necardC.h"
#include "UpMuFluxGen.h"
#else
#ifdef GENIE
#include "GenieFluxGen.h"
#endif
#endif
#include "HKTank.h"
#include <sys/stat.h>
#include "T2HKFlux.h"
#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif



extern "C"{
  void necard_();
  void necardne2_(char *,unsigned int);//NEUT subroutine for reading neutcard into common blocks
  //  void necardap_();
  void nefillmodel_();
  void nefillver_();
  // void necardvc_();
  void necardatm_(char *,unsigned int);
  }

int main(int argc,char * argv[])
{
  if(argc<4)
    {
#ifdef NEUT
      fprintf(stderr,"Input format is ./atmflux infiles.txt neut.card outfiles.txt |job #  seed| \n");
#else
#ifdef GEINIE
      fprintf(stderr,"Input format is ./atmflux infiles.txt genie_in.txt outfiles.txt |job #  seed| \n");
#endif
#endif
      return 0;
    }
  if(argc==5)
    {
      fprintf(stderr,"Must input seed manually if using batch mode \n");
      return 0;
      
    }
  /* Input is three files:
     1: txt List of input flux data files
     2: NEUT Card, or settings text file for genie
     3: txt List of output files
     4: (optional) job #
  */

  int iDetector=0;
  int fluxType=1;
  int itau=0;
  int imode=0;
  float neyears=0;
  float pot=0;
  float solact=0;
#ifdef NEUT
  char c[100];
  sprintf(c,"%s",argv[2]);
  unsigned int len=strlen(c);
  fprintf(stderr,"c %s len %u\n",c,len);
  necardatm_(c,len);
  
  //copy input neutcard to supersim.card if tau mode
  if(neutcardatm_.itau==1)
    {
      //check if supersim.card exists
      struct stat buffer;   
      neutcard_.itauflgcore=1;
      if (!(stat ("supersim.card", &buffer) == 0))
	{
	  char cmnd[100];
	  sprintf(cmnd,"cp %s supersim.card",c);
	  system(cmnd);
	}
    } 
  iDetector=neutcardatm_.iDetector;
  fluxType=neutcardatm_.fluxType;
  itau=neutcardatm_.itau;
  imode=neutcardatm_.imode;
  neyears=neutcardatm_.neyears;
  pot=neutcardatm_.pot;
  solact=nesolact_.solact;
#else
#ifdef GENIE
  std::ifstream settings_file;
  settings_file.open(argv[2]);
  float val;
  std::string key;
  char c[100];
  while(settings_file.good() and !settings_file.eof())
    {
      settings_file>>key>>val;
      settings_file.getline(c,100);
      if(key=="iDetector") iDetector=(int)val;
      else if(key=="fluxType") fluxType=(int)val;
      else if(key=="itau") itau=(int)val;
      else if(key=="imode") imode=(int)val;
      else if(key=="neyears") neyears=val;
      else if(key=="pot") pot=val;
      else if(key=="solact") solact=val;

      if(settings_file.bad()) 
	{
	  std::cout<<"Error reading in settings from file"<<std::endl;
	  return 0;
	}
    }
  if(imode==1)
    {
      std::cout<<"UpMu not available with genie yet, sorry"<<std::endl;
      return 0;
    }
#endif
#endif
  /*  struct stat buffer;   
  neutcard_.itauflgcore=1;
  if (!(stat ("supersim.card", &buffer) == 0))
    {
      char cmnd[100];
      sprintf(cmnd,"cp %s supersim.card",c);
      system(cmnd);
    }
  */
  //Setup Detectors
  TargetCyl * hk_cyl=new TargetCyl();
  hk_cyl->SetR(19.65);
  hk_cyl->SetHeight(41.40);
 

 
 
  HKTank *hk_long_egg=new HKTank();

  DetectorGeom * hk;
  //Set hk according to neutcardatm_.iDetector
  switch(iDetector)
    {
    case 0:
      hk=hk_long_egg;
      break;
    case 1:
      hk=hk_cyl;
      break;
    default:
      std::cout<<"Detector type "<<iDetector<<" invalid"<<std::endl;
      return 0;
    }
  hk->SetDensity(1.E3); //kg/m^3
  //Target rock for atmospheric upmu
  TargetCyl * rockCyl=new TargetCyl();
  rockCyl->SetR(250.);//m
  rockCyl->SetDensity(2700); //kg/m^3
  
  //setup fluxes
  HonFlx* honFlx=new HonFlx(); //HONDA 06
  HonFlxTau* honFlxtau=new HonFlxTau();
  HonFlxUpMu * honFlxUpMu=new HonFlxUpMu();
  honFlx->SetSolarAct(solact);
  honFlxtau->SetSolarAct(solact);
  honFlxUpMu->SetSolarAct(solact);
  
  HonFlx11* honFlx11=new HonFlx11(); //HONDA 11
  HonFlx11Tau* honFlx11tau=new HonFlx11Tau();
  HonFlx11UpMu * honFlx11UpMu=new HonFlx11UpMu();
  honFlx11->SetSolarAct(solact);
  honFlx11tau->SetSolarAct(solact);
  honFlx11UpMu->SetSolarAct(solact);
  
  T2HKFlux * t2hkflux=new T2HKFlux();
  

  
  FlxTable * flx;
  switch(fluxType)
    {
    case 0:
      if(itau==1)
	{
	  flx=honFlxtau;
	}
      else if(itau==0)
	{
	  if(imode==0)
	    {
	      flx=honFlx;
	    }
	  else if(imode==1)
	    {
	      flx=honFlxUpMu;
	    }

	  else
	    {
	      std::cout<<"Simulation mode "<<imode<<" invalid"<<std::endl;
	      return 0;
	    }
	}
      else
	{
	  std::cout<<"Tau mode "<<itau<<" invalid"<<std::endl;
	  return 0;
	}
      break;
    case 1:
      if(itau==1)
	{
	  flx=honFlx11tau;
	}
      else if(itau==0)
	{
	  if(imode==0)
	    {
	      flx=honFlx11;
	    }
	  else if(imode==1)
	    {
	      flx=honFlx11UpMu;
	    }

	  else
	    {
	      std::cout<<"Simulation mode "<<imode<<" invalid"<<std::endl;
	      return 0;
	    }
	}
      else
	{
	  std::cout<<"Tau mode "<<itau<<" invalid"<<std::endl;
	  return 0;
	}
      break;
    case 2:
      std::cout<<"T2HK"<<std::endl;
      flx=t2hkflux;
      break;
    default:
      std::cout<<"Flux Type "<<fluxType<<" invalid"<<std::endl;
      return 0;
    }
  
  //Load fluxes
  ifstream inFile(argv[1]);
  char s[300];
  std::cerr<<"reading in files"<<std::endl;
  while(true)
    {
 
      if(inFile.eof())
	{
	  break;
	}
      if(inFile.fail())
	{
	  return 0;
	}
      inFile.getline(s,300);
      std::cout<<s<<std::endl;
      std::string name=s;
      if(name=="")
	continue;
      //      fprintf(stderr,s);
      if(flx->LoadFluxTable(s)<0)
	{
	  return 0;
	}
    }
  
  //Setup generators
#ifdef NEUT
  NeutGen* Gen=new NeutGen();
  
  UpMuFluxGen * upmufluxGen=new UpMuFluxGen();
  upmufluxGen->SetRMax(30000);//cm
  upmufluxGen->SetDetector(rockCyl);
  upmufluxGen->SetTrueDetector(hk);
  
#else
#ifdef GENIE
  GenieFluxGen * Gen=new GenieFluxGen(); 
  GenieFluxGen* upmufluxGen=NULL; //upmu not yet w/ genie
  
#endif
#endif
  Gen->SetDetector(hk);
  

  FluxGen * fluxGen;

  switch(imode==1)
    {
    case 0:
      fluxGen=Gen;
      break;
    case 1:
      fluxGen=upmufluxGen;
      break;
    default:
      std::cout<<"simulation mode "<<imode<<" invalid"<<std::endl;
      return 0;
    }
  
  
  
  switch(fluxType)
    {
    case 0: case 1:
      fluxGen->SetNYears(neyears);
      //fluxGen->SetEMode(1); //use analytic inversion sampling
      break;
    case 2:
      fluxGen->SetPOT(pot);
      fluxGen->SetDirMode(1); //get direction from T2HK flux class, do not caculate in generator
      break;
    }
#ifdef GENIE
  fluxGen->SetEMode(0);  //genie's "GetCrossSection" is much slower, so need to use method which calls cross section from genie as rarely as possible
#endif

  fluxGen->SetFlxTable(flx);
  
  int ijob=0;
  if(argc>4) 
    {
      ijob=atoi(argv[4]);
      int seed=atoi(argv[5]);
      fluxGen->SetSeed(seed);
    }

  //Set Coordinate system  
  /*double angle=40.583;
  double angleR=angle*Pi/180.;
  TVector3 vX(-cos(angleR),sin(angleR),0);
  TVector3 vY(-sin(angleR),-cos(angleR),0);
  TVector3 vZ(0,0,1);
  fluxGen->SetvX(vX);
  fluxGen->SetvY(vY);
  fluxGen->SetvZ(vZ);
*/

  //Setup output
  ifstream outFile(argv[3]);
  char os[300];

  
  TString * name;
  while(true)
    {
      if(outFile.eof())
	{
	  break;
	}
      if(outFile.fail())
	{
	  return 0;
	}
      outFile.getline(s,300,' ');
      //outFile.getline(os,1);
      outFile.getline(os,300);
      name=new TString(os);

      if(ijob>0)
	{
	  size_t pos=name->Last('.');
	  if(pos>1000) continue;
	  fprintf(stderr,"pos is %zu\n",pos);
	  char num[100];
	  sprintf(num,".%05i.",ijob);
	  name->Replace(pos,1,num);
	  
	}
      if(strcmp(s,"r")==0)
	{
#ifdef NEUT
	  fluxGen->AddNewWriter(new NeutRootWriter,name->Data());
#else
	  fluxGen->AddNewWriter(new RootWriter,name->Data());
#endif
	}
      else if(strcmp(s,"n")==0)
	{
	  fluxGen->AddNewWriter(new NuanceWriter,name->Data());
	}
    }




    
 

  //setup nesting for upmu
#ifdef NEUT //upmu not yet implemented w/ genie
  if(imode==1 and fluxType<2)
    {
      double RMax=4000;
      std::vector<double> EMins;
      std::vector<double> EMaxs;
      std::vector<double> Rs;
      EMins.push_back(1.6);
      EMaxs.push_back(10);
      EMins.push_back(10);
      EMaxs.push_back(100);
      EMins.push_back(100);
      EMaxs.push_back(1000);
      EMins.push_back(1000);
      EMaxs.push_back(90000);
      
      for(int i=0;i<29;i++)
	{
	  EMins.push_back(10+i*10);
	  EMaxs.push_back(10+(1+i)*10);
	
	}
      for(int i=0;i<37;i++)
	{
	  EMins.push_back(300+i*100);
	  EMaxs.push_back(300+(1+i)*100);
	
	}
     
      for(int i=0;i<6;i++)
	{
	  EMins.push_back(4000+i*1000);
	  EMaxs.push_back(4000+(1+i)*1000);
	}
      
      for(int i=0;i<10;i++)
	{
	  EMins.push_back(10000+i*1000);
	  EMaxs.push_back(10000+(i+1)*1000);
	
	}
      for(int i=0;i<7;i++)
	{
	  EMins.push_back(20000+i*10000);
	  EMaxs.push_back(20000+(i+1)*10000);
	}
      
      for(size_t i=0;i<EMaxs.size();i++)
	{
	  Rs.push_back((upmufluxGen->GetRange(EMaxs[i]*1000.)*3+upmufluxGen->GetRMax())/100.);
	}
      //RMax combining
      for(size_t i=0;i<Rs.size();i++)
	{
	  if(Rs[i]>RMax) Rs[i]=RMax;
	}
      if(Rs.size()!=EMins.size())
	{
	  fprintf(stderr,"Rs vector is wrong size for upmu nesting\n");
	  return 0;
	}
      if(EMaxs.size()!=EMins.size())
	{
	  fprintf(stderr,"EMaxs vector is wrong size for upmu nesting\n");
	  return 0;
	}
      while(Rs.size()>1 and Rs.back()==Rs[Rs.size()-2])
	{
	  Rs.pop_back();
	  EMaxs[EMaxs.size()-2]=EMaxs.back();
	  EMaxs.pop_back();
	  EMins.pop_back();
	  }
      if(Rs.size()!=EMins.size())
	{
	  fprintf(stderr,"Rs vector is wrong size for upmu nesting\n");
	  return 0;
	}
      if(EMaxs.size()!=EMins.size())
	{
	  fprintf(stderr,"EMaxs vector is wrong size for upmu nesting\n");
	  return 0;
	}
      TVector3 pos;

      for(size_t i=0;i<EMins.size();i++)
	{
	  fprintf(stderr,"Running upmu nest %zu with EMin %f.1 EMax %f.1 and R %f.0\n",i,EMins[i],EMaxs[i],Rs[i]);
	  upmufluxGen->SetEThresh(EMins[i]);
	  upmufluxGen->SetEMax(EMaxs[i]);
	  upmufluxGen->ResetEHookCount();
	
	  rockCyl->SetHeight(Rs[i]+upmufluxGen->GetRMax()/100.);
	  pos.SetZ(rockCyl->GetHeight()*100/2.);//cm
	  rockCyl->SetPos0(pos);
	 

	  upmufluxGen->ResetFailedMu_Count();
	  upmufluxGen->ResetFailedMu_NuCount();

	  upmufluxGen->Run();
	      
	
	  fprintf(stderr,"%i events failed EHook, %i events failed Mu_Nu, %i events faile Mu, %i events > 1 TeV\n",upmufluxGen->GetFailedEHookCount(),upmufluxGen->GetFailedMu_NuCount(),upmufluxGen->GetFailedMu_Count(),upmufluxGen->GetTeVCount());	  

	  
	}
    }
  else
    {
      fluxGen->Run();
    }
#else
  fluxGen->Run();
#endif
  fprintf(stderr,"All finished up\n");
#ifdef NEUT
  if(neutcardatm_.itau==1)
    {
      //do not remove if running in batch mode
      if(ijob==0)
	system("rm supersim.card");
    }
#endif
  fluxGen->CloseWriters();
  delete Gen;
  if(upmufluxGen)
    delete upmufluxGen;
  delete honFlx;
  delete honFlxtau;
  delete honFlxUpMu;
  delete honFlx11;
  delete honFlx11tau;
  delete honFlx11UpMu;
  delete t2hkflux;
  delete hk_cyl;
  delete hk_long_egg;
  delete rockCyl;
  return 0;
}
