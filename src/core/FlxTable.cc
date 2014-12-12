#include "FlxTable.h"

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

int FlxTable::GetRandDir(NEUTRINO::Flavor flav, float e, float dir[3])
{

  std::cerr<<"Error: GetRandDir not implemented in class inheriting from FlxTable, should not be called"<<std::endl;
  exit(1);
}
