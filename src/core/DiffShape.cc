#include "DiffShape.h"

DiffShape::DiffShape(DetectorGeom* Ta,DetectorGeom* Tb) : DetectorGeom::DetectorGeom()
{
  T1=Ta;
  T2=Tb;

}

double DiffShape::GetVolume()
{
  return T1->GetVolume()-T2->GetVolume();

}

void DiffShape::GetRandPos(double pos[3],TRandom3 *rndm)
{
  while(true)
    {
      T1->GetRandPos(pos,rndm);
      if(!T2->IsWithin(pos)) break;
    }
  return;

}

bool DiffShape::IsWithin(double pos[3])
{
  bool ret=(T1->IsWithin(pos) && !T2->IsWithin(pos));
  return ret;

}
