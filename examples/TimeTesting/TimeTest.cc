#include <iostream>
//#include "TimeStruct.h"
#include "TimeFlux.h"
#include "TimeGen.h"
#include "RootWriter.h"
#include "TargetCyl.h"

int main(int argc, char * argv[])
{

  TimeGen * gen=new TimeGen();

  TimeFlux * flux=new TimeFlux();

  RootWriter * writer=new RootWriter();

  TargetCyl * sk=new TargetCyl();
  sk->SetR(19.65);
  sk->SetHeight(41.40);
  sk->SetDensity(1.E3); //kg/m^3


  gen->SetDetector(sk);

  gen->SetFlxTable(flux);
  gen->LoadFluxTable("");

  gen->AddNewWriter(writer,"TimeFluxTest.root");
  
  Time_Struct start(0,0);
  Time_Struct end(20,0);
  gen->SetStartTime(start);
  gen->SetEndTime(end);

  gen->SetEMode(0);


  gen->CreateEvtRate(start);
  while(true)
    {
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
  std::cout<<"closing"<<std::endl;
  gen->CloseWriters();
  std::cout<<"closed"<<std::endl;
  delete sk;
  std::cout<<"deleted sk"<<std::endl;
  delete gen;
  std::cout<<"finished"<<std::endl;
  //  delete writer;
  //delete flux;

/*
  Time_Struct t1(3,878976457);
  Time_Struct t2(2,900764009);

  if(t1>t2) std::cout<<"t1 greater"<<std::endl;
  else if(t1<t2) std::cout<<"t2 greater"<<std::endl;
  else std::cout<<"t1 should be equl to t2"<<std::endl;

  if(t1==t2) std::cout<<"t1==t2"<<std::endl;

  Time_Struct t3=t1+t2;

  std::cout<<"t3 is "<<t3.seconds<<" s "<<t3.nanoseconds<<" ns"<<std::endl;
  std::cout<<"t1 is "<<t1.seconds<<" s "<<t1.nanoseconds<<" ns"<<std::endl;
  std::cout<<"t2 is "<<t2.seconds<<" s "<<t2.nanoseconds<<" ns"<<std::endl;

  Time_Struct t4=t1-t2;
  
  std::cout<<"t4 is "<<t4.seconds<<" s "<<t4.nanoseconds<<" ns"<<std::endl;
  std::cout<<"t1 is "<<t1.seconds<<" s "<<t1.nanoseconds<<" ns"<<std::endl;
  std::cout<<"t2 is "<<t2.seconds<<" s "<<t2.nanoseconds<<" ns"<<std::endl;


  Time_Struct t5=t2-t1;

  std::cout<<"t5 is "<<t5.seconds<<" s "<<t5.nanoseconds<<" ns"<<std::endl;
 */
}
