#include "TimeFlux.h"
#include <math.h>

std::vector<float> TimeFlux::GetEnergyBins()
{
  std::vector<float> ret;
  float emin,emax,ebin,E;
  int nbins;
  ret.push_back(.002);
  for(int iEgroup=0;iEgroup<7;iEgroup++)
    {
      switch(iEgroup)
	{
	case(0)://10MeV to 200MeV
	  emin=0.01;
	  emax=0.2;
	  nbins=100;
	  break;
	case(1)://200MeV to 5GeV
	  emin=0.2;
	  emax=5.;
	  nbins=100;
	  break;
	case(2)://5GeV to 10 GeV
	  emin=5.;
	  emax=10.;
	  nbins=300;
	  break;
	case(3)://10GeV to 100GeV
	  emin=10;
	  emax=100;
	  nbins=300;
	  break;
	case(4)://100GeV to 1TeV
	  emin=100;
	  emax=1000;
	  nbins=300;
	  break;
	case(5)://1TeV to 10TeV
	  emin=1000;
	  emax=10000;
	  nbins=100;
	  break;
	case(6)://10TeV to 90 TeV
	  emin=10000;
	  emax=90000;
	  nbins=100;
	  break;
	}
      ebin=(emax-emin)/nbins;
      for(int iE=0;iE<nbins;iE++)
	{
	  E=emin+ebin*(float(iE));
	  ret.push_back(E);
	}
    }
  ret.push_back(90000);

  return ret;


}



double TimeFlux::GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{
  if(nuFlav!=NEUTRINO::E and nuFlav!=NEUTRINO::TAU) return 0;

  Time_Struct peak(10,0);
  if(nuFlav==NEUTRINO::TAU) peak.seconds=6;

  Time_Struct diff=peak-t;

  double diff_d=double(diff.seconds)+double(diff.nanoseconds)*1.E-9;

  double t_sig=2;
  double time_fact=exp(-diff_d*diff_d/(t_sig*t_sig));

  float e_peak=500;

  double e_diff=e_peak-E;

  double e_fact=exp(-e_diff*e_diff/(50.*50.));


  double flx=1000.*e_fact*time_fact;

  return 30000.*flx;




}

double TimeFlux::GetFlux(float phi,float cosZ, float E,NEUTRINO::FLAVOR flav,Time_Struct t)
{

  return GetFlux(E,flav,t);

}

Time_Struct TimeFlux::GetNextRecalcTime(Time_Struct t)
{
  Time_Struct tadd(0,1000000.);
    

  t+=tadd;


  return t;



}
