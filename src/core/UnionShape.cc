#include "UnionShape.h"
#include <iostream>
UnionShape::UnionShape(std::vector<DetectorGeom*> t) : DetectorGeom::DetectorGeom()
{
  for(size_t i=0;i<t.size();i++)
    {
      Targets.push_back(t[i]);
    }

}

double UnionShape::GetVolume()
{
  double v=0;
  for(size_t i=0;i<Targets.size();i++)
    {
      v+=Targets[i]->GetVolume();
    }
  return v;

}

bool UnionShape::IsWithin(TVector3* pos)
{

  for(size_t i=0;i<Targets.size();i++)
    {
      if(Targets[i]->IsWithin(pos))
	{
	  return true;
	}
    }

  return false;

}

TVector3 UnionShape::GetRandPos(TRandom3 * rndm)
{
  std::vector<double> vols;
  if(Targets.size()==0) 
    {
      std::cerr<<"Error in UnionShape::GetRandPos: There are no volumes"<<std::endl;
      
      TVector3 v(-9999999999999,-9999999999999,-9999999999999);
      return v;
    }
  vols.push_back(Targets[0]->GetVolume());
  for(size_t i=1;i<Targets.size();i++)
    {
      vols.push_back(Targets[i]->GetVolume()+vols.back());
      
    }

  //choose a volume based on relative sizes
  size_t iVol;
  double rV=rndm->Rndm()*vols.back();
  for(iVol=0;iVol<vols.size();iVol++)
    {
      if(rV<vols[iVol]) break;
    }

  return Targets[iVol]->GetRandPos(rndm);
}


