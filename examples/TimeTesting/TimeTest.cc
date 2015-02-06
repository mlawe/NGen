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
  
  gen->Run();

  //std::cout<<"closing"<<std::endl;
  gen->CloseWriters();
  //std::cout<<"closed"<<std::endl;
  delete sk;
  //std::cout<<"deleted sk"<<std::endl;
  delete gen;
  //  std::cout<<"finished"<<std::endl;
  delete flux;


}
