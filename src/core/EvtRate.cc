#include "EvtRate.h"

EvtRate::EvtRate()
{
  all=0;
  e=0;
  e_bar=0;
  mu_bar=0;
  mu=0;
  tau=0;
  tau_bar=0;

  total_rt=0;
  e_rt=0;
  e_bar_rt=0;
  mu_rt=0;
  mu_bar_rt=0;
  tau_rt=0;
  tau_bar_rt=0;
  
  
}

EvtRate::~EvtRate()
{}

bool EvtRate::Check()
{
  if(e+e_bar+mu+mu_bar==all and e_rt+e_bar_rt+mu_rt+mu_bar_rt+tau_rt+tau_bar_rt==total_rt)
    {
      return true;
    }
  return false;

}
