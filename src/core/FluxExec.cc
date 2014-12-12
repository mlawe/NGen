#include "FluxExec.h"


FluxExec::FluxExec()
{
  runmode=0; //standard=0,upmu=1

  target=NULL;
  gen=NULL;
  



}

FluxExec::FluxExec(std::vector<char*> cmd):FluxExec::FluxExec()
{
  cmdline=cmd;

}

FluxExec::~FluxExec()
{
  if(target) delete target;
  if(gen) delete gen;
	    
}

void FluxExec::Setup()
{
  SetupNucSim();
  SetupGen();
  SetupTarget();
  SetupWriters();

  gen->SetTarget(target);



}

void FluxExec::SetupGen()
{
  switch(runmode)
    {
    case 0:
      SetupStandardGen();
      break;
    case 1:
      SetupUpMuGen();
      break;
    default:
      SetupUserDefinedGen();
      break;
    }

}

void FluxExec::SetupTarget()
{
  switch(runmode)
    {
    case 0:
      SetupTargetStandard();
      break;
    case 1:
      SetupTargetUpMu();
      break;
    case 3:
      SetupTargetUserDefined();
      break;

    }

}

void FluxExec::SetupTagetUpMu():FluxExec::SetupTargetStandard()
{


}

void FluxExec::SetupUserDefinedGen()
{
  std::cout<<"No User defined modes, should not be here.  error..."<<std::endl;
  exit(1);

}

void FluxExec::SetupTargetUserDefined()
{
  std::cout<<"No User defined modes, should not be here.  error..."<<std::endl;
  exit(1);
  

}

void Run()
{
  switch(runmode)
    {
    case 0:
      RunStandard();
      break;
    case 1:
      RunUpmu();
      break;
    default:
      RunUserDefined();
      break;

    }

}


void FluxExec::RunStandard()
{
  gen->CreateEventRate();
  while(true)
    {
      count++;
      if(count/10000>outCount)
	{
	  outCount++;
	  fprintf(stderr,"count %i\n",count);
	}
      int flxOut;
      if((flxOut=gen->CreateNextVector())<0)
	{
	  fprintf(stderr,"************Error CreateNextVector failed, quitting event loop*********\n");
	  exit(1);
	}
      else if(flxOut==0)
	{

	  if(gen->WriteCurrentVector()<0)
	    {
	      std::cout<<"Failed to write out event"<<std::endl;
	      exit(1);
	    }
	}
      else if(flxOut==2)
	{
	  //finished                                                                                                                              
	  break;
	}
      //flxOut==1 skips writing event                                                                                                             


    }



}



void  FluxExec::RunUpmu()
{

  for(size_t i=0;i<EMins.size();i++)
    {
      fprintf(stderr,"Running upmu nest %zu with EMin %f.1 EMax %f.1 and R %f.0\n",i,EMins[i],EMaxs[i],Rs[i]);
      gen->SetEThresh(EMins[i]);
      gen->SetEMax(EMaxs[i]);
      upmufluxGen->ResetEHookCount();
      sphereTrunc->SetR(Rs[i]);
      fluxGen->CreateEvtRate();

      int writeoutcount=0;

      while(true)
	{
	  count++;
	  if(count/1000>outCount)
	    {
	      outCount++;
	      fprintf(stderr,"count %i\n",count);
	    }
	  int flxOut;
	  if((flxOut=fluxGen->CreateNextVector())<0)
	    {

	      break;
	    }
	  else if(flxOut==0)
	    {
	      writeoutcount++;
	      //Rotate Coords                                                                                                                     
	      //fluxGen->RotateCoords();                                                                                                          
	      //write out event                                                                                                                   
	      if(fluxGen->WriteCurrentVector()<0)
		{
		  break;
		}
	    }
	  else if(flxOut==2)
	    {
	      //finished                                                                                                                          
	      break;
	    }
	  //flxOut==1 skips writing event                                                                                                         


	}
      fprintf(stderr,"Wrote out %i events, %i events failed EHook\n",writeoutcount,upmufluxGen->GetFailedEHookCount());

    }





}
