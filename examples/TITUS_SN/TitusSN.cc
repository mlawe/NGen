#include <iostream>
// #include "SNGen.hxx"
#include "NeutGen.h"
#include "SNFlux.h"
#include "TargetCyl.h"
#include "NuanceWriter.h"

int main(int argc, char* argv[])
{
  // Introduce command line arguments to set flux file
  std::string fluxFile = "NOFLUX.dat";
  
  for (;;) {
    int a = getopt(argc, argv, "f:");
    if (a<0) break;
    switch (a) {
    case 'f':
      fluxFile = optarg;
      break;
    }
  } // Closes process options for loop 


  // TimeGen* gen = new TimeGen();
  // TimeSNFlux* flux = new TimeSNFlux();

  // Four ingredients:
  //    1) Writer
  //    2) Geometry
  //    3) Flux Table
  //    4) Flux Generator

  // 1-3 feed into 4, so instatiate that first.
  // SNGen* gen = new SNGen();
  NeutGen* gen = new NeutGen();
  // ---- Add Writer to generator ----
  NuanceWriter* writer = new NuanceWriter();
  std::string writername = "TitusSN_" + fluxFile;
  gen->AddNewWriter(writer,writername.c_str());
  std::cout<<"Created write file "<<writername<<std::endl;
  // ---- ----------------------- ----

  // ---- Add TITUS geometry to generator ----
  TargetCyl* det = new TargetCyl();
  det->SetR(5.5); // m
  det->SetHeight(22.0); // m
  det->SetDensity(1.E3); //kg/m^3
  gen->SetDetector(det);
  // ---- ------------------------------- ----

  // ---- Add SN flux to generator ----
  SNFlux* flux = new SNFlux();
  flux->IsTimeDependent();
  if (!fluxFile.empty())
    flux->LoadFluxTable(fluxFile);
  gen->SetFlxTable(flux);
  // ---- ------------------------ ----

  // ---- Set the generator flux period ----
  gen->SetStartTime(flux->GetMinTime());
  gen->SetEndTime(flux->GetMaxTime());
  
  // ---- Set the generator E mode ----
  // 0 or 1, I don't know what the diffference is...
  gen->SetEMode(0);
  
  // ---- Run the generator! ----
  gen->Run();

  //   std::cout<<"closing"<<std::endl;
  gen->CloseWriters();
  //   std::cout<<"closed"<<std::endl;
  delete det;
  //   std::cout<<"deleted det"<<std::endl;
  delete gen;
  //  std::cout<<"finished"<<std::endl;
  delete flux;

}
