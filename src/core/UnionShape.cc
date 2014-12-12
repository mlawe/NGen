#include "UnionShape.h"

UnionShape::UnionShape(std::vector<TargetShape*> t) : TargetShape::TargetShape()
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

bool UnionShape::IsWithin(double pos[3])
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

void UnionShape::GetRandPos(double pos[3],TRandom3 * rndm)
{
  std::vector<double> vols;
  if(Targets.size()==0) return;
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

  Targets[iVol]->GetRandPos(pos,rndm);
}


