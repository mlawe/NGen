#include "DGBox.h"

double DGBox::GetVolume()
{
  return xdim*ydim*zdim;

}
TVector3 DGBox::GetRandPos(TRandom3 * rndm)
{
  double x=100.*xdim*rndm->Rndm();//cm
  double y=100.*xdim*rndm->Rndm();
  double z=100.*xdim*rndm->Rndm();

  TVector3 v(x,y,z);
  TransformCoordsMineToGen(&v);

  return v;

}

bool DGBox::IsWithin(TVector3* v)
{
  TransformCoordsGenToMine(v);

  double x=v->X();
  double y=v->Y();
  double z=v->Z();
  bool ret=(x<xdim*100 && x>0 && y<ydim*100 && y>0 && z<zdim*100 && zdim>0);
  return ret;
}




