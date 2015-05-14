#include "SNFlux.h"
#include <math.h>

std::vector<float> SNFlux::GetEnergyBins() {
  
  std::vector<float> ret;
  for (float energy = ENERGYMIN; energy < ENERGYMAX; energy += 0.0002)
    ret.push_back(energy);

  return ret;
}

int SNFlux::GetRandDir(NEUTRINO::FLAVOR flav, float E, TVector3 * dir)
{
  dir->SetXYZ(1,0,0);
  return 0;
}

int SNFlux::LoadFluxTable(std::string fileName) {
  std::cout<<"Attempting to load flux table"<<std::endl;

  fRandom = new TRandom3(1001);

  std::ifstream fileStream;
  fileStream.open(fileName.c_str());
  
  // floats to store vales for vectors used to create TSpline3s
  float time_f, energy, nu_e, nu_mu, nu_tau, nubar_e, nubar_mu, nubar_tau;
  int64_t time, prev_time = -1;
  energy = nu_e = nu_mu = nu_tau = nubar_e = nubar_mu = nubar_tau = -1.0;
  // vectors used to create TSpline3s of entries vs energy
  std::vector<float> energy_vec, nu_e_vec, nu_mu_vec, nu_tau_vec, nubar_e_vec, nubar_mu_vec, nubar_tau_vec;
  energy_vec.clear(); nu_e_vec.clear(); nu_mu_vec.clear(); nu_tau_vec.clear(); nubar_e_vec.clear(); nubar_mu_vec.clear(); nubar_tau_vec.clear();

  while (!fileStream.eof()) {
    // std::cout<<"File stream is good"<<std::endl;
    if (!fileStream.good()) {
      std::cout<<"Bad file stream"<<std::endl;
      return -1;
    }
    
    fileStream>>time_f>>energy>>nu_e>>nu_mu>>nu_tau>>nubar_e>>nubar_mu>>nubar_tau;
    time=(int64_t)time_f;
    int64_t rem=time%1000000;
    if(rem>500000) time+=1000000;
    time=time-time%1000000;
  // std::cout<<time<<" "<<energy<<" "<<nu_e<<" "<<nu_mu<<" "<<nu_tau<<" "<<nubar_e<<" "<<nubar_mu<<" "<<nubar_tau<<std::endl;
    
    if ( (prev_time != time) && !energy_vec.empty() ) {
      // std::cerr<<"Load Time "<<prev_time<<std::endl;
      std::cout<<"Loaded Time "<<prev_time<<"\r"<<std::flush;
      // fnubar_tau_time_flux[ (int64_t) prev_time] = std::map<float, float, FloatCmp >();
      fnu_e_time_flux[ (int64_t) prev_time] = new TH1F(Form("nu_e_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
      fnu_mu_time_flux[ (int64_t) prev_time] = new TH1F(Form("nu_mu_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
      fnu_tau_time_flux[ (int64_t) prev_time] = new TH1F(Form("nu_tau_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
      fnubar_e_time_flux[ (int64_t) prev_time] = new TH1F(Form("nubar_e_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
      fnubar_mu_time_flux[ (int64_t) prev_time] = new TH1F(Form("nubar_mu_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
      fnubar_tau_time_flux[ (int64_t) prev_time] = new TH1F(Form("nubar_tau_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);

      for (unsigned int iE = 0; iE<energy_vec.size(); iE++) {
	fnu_e_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nu_e_vec[iE] );
	fnu_mu_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nu_mu_vec[iE] );
	fnu_tau_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nu_tau_vec[iE] );
	fnubar_e_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nubar_e_vec[iE] );
	fnubar_mu_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nubar_mu_vec[iE] );
	fnubar_tau_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nubar_tau_vec[iE] );
      }
      
      // clear vectors after creating maps
      energy_vec.clear();
      nu_e_vec.clear();
      nu_mu_vec.clear();
      nu_tau_vec.clear();
      nubar_e_vec.clear();
      nubar_mu_vec.clear();
      nubar_tau_vec.clear();
    }
    
    energy_vec.push_back(energy);
    nu_e_vec.push_back(nu_e*fRandom->Poisson(SQCMTOSM));
    nu_mu_vec.push_back(nu_mu*fRandom->Poisson(SQCMTOSM));
    nu_tau_vec.push_back(nu_tau*fRandom->Poisson(SQCMTOSM));
    nubar_e_vec.push_back(nubar_e*fRandom->Poisson(SQCMTOSM));
    nubar_mu_vec.push_back(nubar_mu*fRandom->Poisson(SQCMTOSM));
    nubar_tau_vec.push_back(nubar_tau*fRandom->Poisson(SQCMTOSM));
    
    prev_time = time;
  }

  // std::cerr<<"Load Time "<<prev_time<<std::endl;
  std::cout<<"Loaded Time "<<prev_time<<"\r\n"<<std::flush;
  fnu_e_time_flux[ (int64_t) prev_time] = new TH1F(Form("nu_e_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
  fnu_mu_time_flux[ (int64_t) prev_time] = new TH1F(Form("nu_mu_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
  fnu_tau_time_flux[ (int64_t) prev_time] = new TH1F(Form("nu_tau_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
  fnubar_e_time_flux[ (int64_t) prev_time] = new TH1F(Form("nubar_e_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
  fnubar_mu_time_flux[ (int64_t) prev_time] = new TH1F(Form("nubar_mu_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
  fnubar_tau_time_flux[ (int64_t) prev_time] = new TH1F(Form("nubar_tau_%i", (int)prev_time), "", 501, ENERGYMIN, ENERGYMAX);
  
  for (unsigned int iE = 0; iE<energy_vec.size(); iE++) {
    fnu_e_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nu_e_vec[iE] );
    fnu_mu_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nu_mu_vec[iE] );
    fnu_tau_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nu_tau_vec[iE] );
    fnubar_e_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nubar_e_vec[iE] );
    fnubar_mu_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nubar_mu_vec[iE] );
    fnubar_tau_time_flux.find( (int64_t) prev_time )->second->Fill( energy_vec[iE] , nubar_tau_vec[iE] );
  }
  
  // for (std::map<int64_t, TH1F*, std::less<int64_t> >::iterator iter = fnu_e_time_flux.begin(); iter !=  fnu_e_time_flux.end(); ++iter)
  //   std::cout<<iter->first<<std::endl;
  
  fnu_flav_time_flux[NEUTRINO::E] = fnu_e_time_flux;
  fnu_flav_time_flux[NEUTRINO::MU] = fnu_mu_time_flux;
  fnu_flav_time_flux[NEUTRINO::TAU] = fnu_tau_time_flux;
  fnu_flav_time_flux[NEUTRINO::E_BAR] = fnubar_e_time_flux;
  fnu_flav_time_flux[NEUTRINO::MU_BAR] = fnubar_mu_time_flux;
  fnu_flav_time_flux[NEUTRINO::TAU_BAR] = fnubar_tau_time_flux;
  
  fileStream.close();
  
  std::cout<<"Added "<<fnu_e_time_flux.size()<<" time bins."<<std::endl;

  delete fRandom;

  return 0;
}

double SNFlux::GetFlux(float phi,float cosZ,float E,NEUTRINO::FLAVOR flav,Time_Struct t) {
  return GetFlux(E,flav,t);
}

double SNFlux::GetFlux(float E,NEUTRINO::FLAVOR flav,Time_Struct t) {
  if (fnu_flav_time_flux.size()<NEUTRINO::NUMBER) {
    std::cerr<<"SNFlux::GetFlux fnu_flav_time_flux map must be loaded first"<<std::endl;
    exit(1);
  }

  int64_t time = t.seconds*STONS+t.nanoseconds;
  int64_t rem=time%1000000;
  if(rem>500000) time+=1000000;
  time=time-time%1000000;

  // Try and match absolute time to keys
  if (fnu_flav_time_flux.find(flav)->second.count(time) > 0) {
    return SCALEFACTOR*fnu_flav_time_flux.find(flav)->second.find(time)->second->GetBinContent(fnu_flav_time_flux.find(flav)->second.find(time)->second->GetXaxis()->FindBin(E)); // Good luck figuring that one out!
  }
  else {
    std::cerr<<"Time "<<time<<" not in map"<<std::endl;
    exit(1);
  }
  return 0.0;
}

Time_Struct SNFlux::GetNextRecalcTime(Time_Struct t) {
  Time_Struct tadd(0, 1000000);
  t+=tadd;
  return t;
}

// map based implementation
// Time_Struct SNFlux::GetMinTime() {
//   if (fnu_e_time_flux.size()) {
//     std::cout<<"Min time: "<<(int64_t) fnu_e_time_flux.begin()->first<<std::endl;
//     return Time_Struct( (int64_t) 0, (int64_t) fnu_e_time_flux.begin()->first);
//   }
//   else
//     return Time_Struct(0,0);
// }

// Time_Struct SNFlux::GetMaxTime() {
//   if (fnu_e_time_flux.size()) {
//     std::cout<<"Max time: "<<(int64_t) fnu_e_time_flux.rbegin()->first<<std::endl;
//     return Time_Struct( (int64_t) 0, (int64_t) fnu_e_time_flux.rbegin()->first);
//   }
//   else
//     return Time_Struct(0,0);
// }

// unordered_map based implementation
Time_Struct SNFlux::GetMinTime() {
  if (fnu_e_time_flux.size()) {
    int64_t min = 9223372036854775807;
    for (boost::unordered_map<int64_t, TH1F* >::iterator iter = fnu_e_time_flux.begin(); iter != fnu_e_time_flux.end(); ++iter)
      if (iter->first < min) min = iter->first;
    std::cout<<"Min time: "<<min<<std::endl;
    return Time_Struct(0, min);
  }
  else
    return Time_Struct(0,0);
}

Time_Struct SNFlux::GetMaxTime() {
  if (fnu_e_time_flux.size()) {
    int64_t max = -9223372036854775807;
    for (boost::unordered_map<int64_t, TH1F* >::iterator iter = fnu_e_time_flux.begin(); iter != fnu_e_time_flux.end(); ++iter)
      if (iter->first > max) max = iter->first;
    std::cout<<"Max time: "<<max<<std::endl;
    return Time_Struct(0, max);
  }
  else
    return Time_Struct(0,0);
}
