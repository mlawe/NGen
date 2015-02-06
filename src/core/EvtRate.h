#ifndef _EvtRate_
#define _EvtRate_

#include "TimeStruct.h"
class EvtRate
{
 public:
  EvtRate();
  ~EvtRate();

  bool Check();

  void Times(double); //mult all evt rate by factor
  double total_rt;   //event rate.  If flux is time-dependent it is in Hz, if flux is time-independent it is total events to simulate
  double e_rt;
  double e_bar_rt;
  double mu_rt;
  double mu_bar_rt;
  double tau_rt;
  double tau_bar_rt;

  Time_Struct time; //time for which rate is calc'd
};

#endif
