#include <iostream>
#include <math.h>
#include <TFile.h>
#include <TH1F.h>
#include <stdlib.h>
#include "neworkC.h"


extern "C"{

  void necard_();
  void nefillmodel_();
  void nefillver_();


  double cohcrsnerein_(float&,int&,int&);
  double fntotpau_(int&, float&);
  double dcohnerein_(float&,int&,int&,float&,float&,float&,float&);
  
  double dnelsq2_(double&,int&,double&);
  double fnq2min_(float&,int&);
  double fnq2max_(float&,int&);
  double fnels_rpascl_(double&,int&);

  void neelsspff_(float&,int&,float*,int&,int&);
}




int main(int argc,char* argv[])
{

  necard_();
  nefillmodel_();
  nefillver_();

  
  TFile * f=TFile::Open("Fermi.root","recreate");
  TH1F * h=new TH1F("h","h",30,-1,1);
  TH1F * hfabs=new TH1F("habs","habs",200,0,1);
  float pf[3];
  float e=atof(argv[1]);
  int nuID=14;
  int icall=0;
  int ierr;

  int nRuns=10000000;
  for(int i=0;i<nRuns;i++)
    {
      neelsspff_(e,nuID,pf,icall,ierr);
      
      icall++;
      
      float pfa=sqrt(pf[0]*pf[0]+pf[1]*pf[1]+pf[2]*pf[2]);
      hfabs->Fill(pfa);
      h->Fill(pf[0]/pfa);
    }

  h->SetMinimum(0);
  h->Write();
  hfabs->Write();
  f->Close();
    




}
