////////////////////////////////////////////////////////////////
/* 
Simulation to create atmospheric netutrino event vectors for SK, using Honda Flux (or other) 
tables and NEUT neutrino event generator.

Begun: Feb 05,2014-C. Kachulis

*//////////////////////////////////////////////////////////////// 
#include "NeutGen.h"
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
#include "necardatmC.h"
#include "nesolactC.h"
#include "necardC.h"
#include "UpMuFluxGen.h"
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
      fprintf(stderr,"Input format is ./atmflux infiles.txt neut.card outfiles.txt |job #  seed| \n");
      return 0;
    }
  if(argc==5)
    {
      fprintf(stderr,"Must input seed manually if using batch mode \n");
      return 0;
      
    }
  /* Input is three files:
     1: txt List of input flux data files
     2: NEUT Card
     3: txt List of output files
     4: (optional) job #
  */

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

  //Setup Detectors
  TargetCyl * hk_cyl=new TargetCyl();
  hk_cyl->SetR(19.65);
  hk_cyl->SetHeight(41.40);
 
 
 
  HKTank *hk_long_egg=new HKTank();

  DetectorGeom * hk;
  //Set hk according to neutcardatm_.iDetector
  switch(neutcardatm_.iDetector)
    {
    case 0:
      hk=hk_long_egg;
      break;
    case 1:
      hk=hk_cyl;
      break;
    default:
      std::cout<<"Detector type "<<neutcardatm_.iDetector<<" invalid"<<std::endl;
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
  honFlx->SetSolarAct(nesolact_.solact);
  honFlxtau->SetSolarAct(nesolact_.solact);
  honFlxUpMu->SetSolarAct(nesolact_.solact);
  
  HonFlx11* honFlx11=new HonFlx11(); //HONDA 11
  HonFlx11Tau* honFlx11tau=new HonFlx11Tau();
  HonFlx11UpMu * honFlx11UpMu=new HonFlx11UpMu();
  honFlx11->SetSolarAct(nesolact_.solact);
  honFlx11tau->SetSolarAct(nesolact_.solact);
  honFlx11UpMu->SetSolarAct(nesolact_.solact);
  
  T2HKFlux * t2hkflux=new T2HKFlux();
  

  
  FlxTable * flx;
  switch(neutcardatm_.fluxType)
    {
    case 0:
      if(neutcardatm_.itau==1)
	{
	  flx=honFlxtau;
	}
      else if(neutcardatm_.itau==0)
	{
	  if(neutcardatm_.imode==0)
	    {
	      flx=honFlx;
	    }
	  else if(neutcardatm_.imode==1)
	    {
	      flx=honFlxUpMu;
	    }

	  else
	    {
	      std::cout<<"Simulation mode "<<neutcardatm_.imode<<" invalid"<<std::endl;
	      return 0;
	    }
	}
      else
	{
	  std::cout<<"Tau mode "<<neutcardatm_.itau<<" invalid"<<std::endl;
	  return 0;
	}
      break;
    case 1:
      if(neutcardatm_.itau==1)
	{
	  flx=honFlx11tau;
	}
      else if(neutcardatm_.itau==0)
	{
	  if(neutcardatm_.imode==0)
	    {
	      flx=honFlx11;
	    }
	  else if(neutcardatm_.imode==1)
	    {
	      flx=honFlx11UpMu;
	    }

	  else
	    {
	      std::cout<<"Simulation mode "<<neutcardatm_.imode<<" invalid"<<std::endl;
	      return 0;
	    }
	}
      else
	{
	  std::cout<<"Tau mode "<<neutcardatm_.itau<<" invalid"<<std::endl;
	  return 0;
	}
      break;
    case 2:
      std::cout<<"T2HK"<<std::endl;
      flx=t2hkflux;
      break;
    default:
      std::cout<<"Flux Type "<<neutcardatm_.fluxType<<" invalid"<<std::endl;
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
  NeutGen* Gen_neut=new NeutGen();
  Gen_neut->SetDetector(hk);
  
  UpMuFluxGen * upmufluxGen=new UpMuFluxGen();
  upmufluxGen->SetRMax(30000);//cm
  upmufluxGen->SetDetector(rockCyl);
  upmufluxGen->SetTrueDetector(hk);
  

  FluxGen * fluxGen;

  switch(neutcardatm_.imode==1)
    {
    case 0:
      fluxGen=Gen_neut;
      break;
    case 1:
      fluxGen=upmufluxGen;
      break;
    default:
      std::cout<<"simulation mode "<<neutcardatm_.imode<<" invalid"<<std::endl;
      return 0;
    }
  
  
  
  switch(neutcardatm_.fluxType)
    {
    case 0: case 1:
      fluxGen->SetNYears(neutcardatm_.neyears);
      fluxGen->SetEMode(1); //use analytic inversion sampling
      break;
    case 2:
      fluxGen->SetPOT(neutcardatm_.pot);
      fluxGen->SetDirMode(1); //get direction from T2HK flux class, do not caculate in generator
      break;
    }
      
  fluxGen->SetFlxTable(flx);
  
  int ijob=0;
  if(argc>4) 
    {
      ijob=atoi(argv[4]);
      int seed=atoi(argv[5]);
      fluxGen->SetSeed(seed);
    }

  //Set Coordinate system  
  double angle=40.583;
  double angleR=angle*Pi/180.;
  TVector3 vX(-cos(angleR),sin(angleR),0);
  TVector3 vY(-sin(angleR),-cos(angleR),0);
  TVector3 vZ(0,0,1);
  fluxGen->SetvX(vX);
  fluxGen->SetvY(vY);
  fluxGen->SetvZ(vZ);


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
	  
	  fluxGen->AddNewWriter(new NeutRootWriter,name->Data());
	}
      else if(strcmp(s,"n")==0)
	{
	  fluxGen->AddNewWriter(new NuanceWriter,name->Data());
	}
    }




    
 

  //setup nesting for upmu
  if(neutcardatm_.imode==1 and neutcardatm_.fluxType<2)
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
  fprintf(stderr,"All finished up\n");
  if(neutcardatm_.itau==1)
    {
      //do not remove if running in batch mode
      if(ijob==0)
	system("rm supersim.card");
    }
  fluxGen->CloseWriters();
  delete Gen_neut;
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
