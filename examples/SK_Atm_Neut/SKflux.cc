////////////////////////////////////////////////////////////////
/* 
Simulation to create atmospheric netutrino event vectors for SK, using Honda Flux (or other) 
tables and NEUT neutrino event generator.

Begun: Feb 05,2014-C. Kachulis

*//////////////////////////////////////////////////////////////// 
#include "AtmFluxGen.h"
#include <cstdio>
#include <stdlib.h>
#include "TargetCyl.h"
#include "HonFlx11.h"
#include "HonFlx.h"
#include "HonFlxTau.h"
#include "HonFlxUpMu.h"
#include "NuanceWriter.h"
#include "AtmRootWriter.h"
#include "ZBSWriter.h"
#include "DiffShape.h"
#include "necardatmC.h"
#include "nesolactC.h"
#include "necardC.h"
#include "UpMuFluxGen.h"
#include "TargetSphere.h"
#include "TargetSphereTrunc.h"
#include <sys/stat.h>
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
   //  skheadg_.sk_geometry=4;
  AtmFluxGen* atmfluxGen=new AtmFluxGen();
  UpMuFluxGen * upmufluxGen=new UpMuFluxGen();

  TargetCyl * sk=new TargetCyl();
  sk->SetR(19.65);
  sk->SetHeight(41.40);
  sk->SetDensity(1.E3); //kg/m^3
 


 

  atmfluxGen->SetDetector(sk);


  upmufluxGen->SetRMax(3000);//cm
  TargetCyl * rockCyl=new TargetCyl();
  rockCyl->SetR(100.);//m
  rockCyl->SetDensity(2700); //kg/m^3
  AtmFluxGen* fluxGen;
  upmufluxGen->SetDetector(rockCyl);
  upmufluxGen->SetTrueDetector(sk);
  if(neutcardatm_.imode==1)
    {
      fluxGen=upmufluxGen;
    }
  else
    {
      fluxGen=atmfluxGen;
    }
  //  fluxGen->SetTauMode(neutcardatm_.itau==1 ? true:false);
  //HonFlx11* honFlx11=new HonFlx11();

  HonFlx* honFlx=new HonFlx(); //old version for cross check w/ neutflux
  HonFlxTau* honFlxtau=new HonFlxTau();
  HonFlxUpMu * honFlxUpMu=new HonFlxUpMu();
  honFlx->SetSolarAct(nesolact_.solact);
  honFlxtau->SetSolarAct(nesolact_.solact);
  honFlxUpMu->SetSolarAct(nesolact_.solact);
  
  ifstream inFile(argv[1]);
  
  FlxTable * flx;
  if(neutcardatm_.itau==1)
    {
      flx=honFlxtau;
    }
  else
    {
      flx=honFlx;
      if(neutcardatm_.imode==1)
	{
	  flx=honFlxUpMu;
	}
    }
  
  //  honFlx11->SetSolarAct(nesolact_.solact);
  
  
  
 
  fluxGen->SetNYears(neutcardatm_.neyears);
  
  fluxGen->SetFlxTable(flx);
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
      fprintf(stderr,s);
      if(flx->LoadFluxTable(s)<0)
	{
	  fprintf(stderr,"Gen\n");
	  break;
	}
    }
  
  ifstream outFile(argv[3]);
  char os[300];
  int ijob=0;
  if(argc>4) 
    {
      ijob=atoi(argv[4]);
      int seed=atoi(argv[5]);
      fluxGen->SetSeed(seed);
    }
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
	  
	  fluxGen->AddNewWriter(new AtmRootWriter,name->Data());
	}
      else if(strcmp(s,"n")==0)
	{
	  fluxGen->AddNewWriter(new NuanceWriter,name->Data());
	}
      else if(strcmp(s,"z")==0)
	{
	  
	  fluxGen->AddNewWriter(new ZBSWriter,name->Data());
	}
      fprintf(stderr,"past writer \n");
    }
  //testing portions of spectrum only
  //fluxGen->SetEThresh(1);
  // fluxGen->SetEMax(1.016);

  fluxGen->SetEMode(1); //use analytic inversion sampling

  //Set Coordinate system  
  double angle=40.583;
  double angleR=angle*Pi/180.;
  TVector3 vX(-cos(angleR),sin(angleR),0);
  TVector3 vY(-sin(angleR),-cos(angleR),0);
  TVector3 vZ(0,0,1);
  fluxGen->SetvX(vX);
  fluxGen->SetvY(vY);
  fluxGen->SetvZ(vZ);

    
 
  int count=0;
  // int outCount=0;
  if(neutcardatm_.imode==1)
    {
      double RMax=4000;
      //nesting for upmu
      std::vector<double> EMins;
      std::vector<double> EMaxs;
      std::vector<double> Rs;
      EMins.push_back(1.6);
      EMaxs.push_back(10);
      //EMins.push_back(10);
      //EMaxs.push_back(100);
      //EMins.push_back(100);
      //EMaxs.push_back(1000);
      //EMins.push_back(1000);
      //EMaxs.push_back(90000);
      
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
      //      double pos[3];
      //pos[0]=0;
      //pos[1]=0;
      //pos[2]=0;
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
  fprintf(stderr,"All finished up %i \n",count);
  if(neutcardatm_.itau==1)
    {
      //do not remove if running in batch mode
      if(ijob==0)
	system("rm supersim.card");
    }
  fluxGen->CloseWriters();
  delete atmfluxGen;
  delete upmufluxGen;
  delete honFlx;
  delete honFlxtau;
  delete honFlxUpMu;
  delete sk;
  delete rockCyl;
  return 0;
}
