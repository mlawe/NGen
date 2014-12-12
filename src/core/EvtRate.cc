#include "EvtRate.h"

EvtRate::EvtRate()
{
  all=0;
  e=0;
  e_bar=0;
  mu_bar=0;
  mu=0;
}

EvtRate::~EvtRate()
{}

bool EvtRate::Check()
{
  if(e+e_bar+mu+mu_bar==all)
    {
      return true;
    }
  return false;

}
