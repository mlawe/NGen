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

TVector3 DiffShape::GetRandPos(TRandom3 *rndm)
{
  TVector3 p;
  while(true)
    {
      p=T1->GetRandPos(rndm);
      if(!T2->IsWithin(&p)) break;
    }
  return p;

}

bool DiffShape::IsWithin(TVector3* pos)
{
  bool ret=(T1->IsWithin(pos) && !T2->IsWithin(pos));
  return ret;

}
