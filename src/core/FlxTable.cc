#include "FlxTable.h"
#include <stdlib.h>

FlxTable::FlxTable()
{
  solarAct=-1;

}

FlxTable::~FlxTable()
{

}


void FlxTable::SetSolarAct(float sA)
{
  solarAct=sA;
}

int FlxTable::GetRandDir(NEUTRINO::FLAVOR flav, float e, TVector3* dir)
{

  std::cerr<<"Error: GetRandDir not implemented in class inheriting from FlxTable, should not be called"<<std::endl;
  exit(1);
}
