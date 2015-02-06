#include "EvtRate.h"
#include <cmath>
EvtRate::EvtRate()
{

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

void EvtRate::Times(double fact)
{
  e_rt*=fact;
  e_bar_rt*=fact;
  mu_rt*=fact;
  mu_bar_rt*=fact;
  tau_rt*=fact;
  tau_bar_rt*=fact;
  total_rt*=fact;
  
  
}
bool EvtRate::Check()
{
  if(std::abs(e_rt+e_bar_rt+mu_rt+mu_bar_rt+tau_rt+tau_bar_rt-total_rt)<0.0001)
    {
      return true;
    }
  return false;

}
