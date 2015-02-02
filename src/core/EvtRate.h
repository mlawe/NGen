#ifndef _EvtRate_
#define _EvtRate_

#include "TimeStruct.h"
class EvtRate
{
 public:
  EvtRate();
  ~EvtRate();

  bool Check();

  int all;
  int e;
  int e_bar;
  int mu;
  int mu_bar;
  int tau;
  int tau_bar;

  double total_rt;   //event rate in events per second
  double e_rt;
  double e_bar_rt;
  double mu_rt;
  double mu_bar_rt;
  double tau_rt;
  double tau_bar_rt;

  Time_Struct time; //time for which rate is calc'd
};

#endif
