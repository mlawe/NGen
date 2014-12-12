#ifndef _SolarFlux_
#define _SolarFlux_


#include "FlxTable.h"
#include "spa.h"


/*Solar flux base class, inherits from FlxTable.  Finds position of sun based
on routine in spa.c from http://www.nrel.gov/midc/spa/.  See spa.c for Copyright 
and usage info*/

class SolarFlux: public FlxTable
{
 public:
  SolarFlux();
  int GetRandDir(NEUTRINO::Flavor flav, float e, float dir[3]);

 protected:
  double longitude;
  double latitude;




};

#endif 
