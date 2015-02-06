#include <iostream>
#include <math.h>
#include <TFile.h>
#include <TGraph.h>
#include <stdlib.h>
#include "neworkC.h"
#include <sstream>
#include <vector>
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
  //  double fnels_rpascl_(double&,int&);


  double fnelscrs_(float&,int&);
  double rsspicrs_(float&,int&,int&);
}




int main(int argc,char* argv[])
{
  if(argc<4)
    {
      std::cout<<"Input format: ./NeutCRS E_low(GeV) E_high(GeV) neutrino_code En"<<std::endl;
      return 0;
    }

  necard_();
  nefillmodel_();
  nefillver_();

  float Elow=atof(argv[1]);
  float EHigh=atof(argv[2]);
  int nuID=atoi(argv[3]);
  
  TFile * f=TFile::Open("NeutCRS.root","recreate");

  int nStep=500;
  double E[nStep];
  double crs[nStep];
  float q2min,q2max,q2step;
  double q2;
  int q2Nsteps=200;
  double Q2x[q2Nsteps];
  double crsQ2[q2Nsteps];
  std::vector<TGraph*> vq2G;
  double E2[nStep];
  double prob[nStep];
  std::stringstream s;
  int itype=11;
  for(int i=0;i<nStep;i++)
    {
      E[i]=Elow+(EHigh-Elow)*float(i)/float(nStep);
      // En=E[i];
      float En=E[i];
      q2min=fnq2min_(En,nuID);
      q2max=fnq2max_(En,nuID);
      q2step=(q2max-q2min)/float(q2Nsteps);
      crs[i]=0;
      q2=q2min;
      double crq2max=0;
      for(int j=0;j<q2Nsteps;j++)
	{
	  q2+=double(q2step);
	  //std::cout<<"q2: "<<q2<<std::endl;
	  double cv=0;
	  //	  cv=dnelsq2_(E[i],nuID,q2)*fnels_rpascl_(q2,nuID)*q2step;
	  if(cv/q2step>crq2max)crq2max=cv/q2step;
	  //crs[i]+=dnelsq2_(E[i],nuID,q2)*fnels_rpascl_(q2,nuID)*q2step;
	  crs[i]+=cv;
	  if(i%10==0)
	    {

	      Q2x[j]=q2;
	      //	      crsQ2[j]=dnelsq2_(E[i],nuID,q2)*fnels_rpascl_(q2,nuID);
	      crsQ2[j]=cv;//dnelsq2_(E[i],nuID,q2)*fnels_rpascl_(q2,nuID);

	      //	      if(crsQ2[j]>crq2max) crq2max=crsQ2[j];
	    }
	 
	  //crs[i]+=dnelsq2_(E[i],nuID,q2)*q2step;
	}
      
      if(i%10==0)
	{
	  s.str("");
	  s<<"gq2_E_"<<E[i];
	  vq2G.push_back(new TGraph(q2Nsteps,Q2x,crsQ2));
	  vq2G.back()->SetName(s.str().c_str());
	  
	}
      E2[i]=E[i];
      if(crq2max>0.00000001 and q2step>0.0000001)prob[i]=crs[i]/(q2step*q2Nsteps*crq2max);
      else prob[i]=0;

      crs[i]=0;
      crs[i]=rsspicrs_(En,nuID,itype);
      //      crs[i]=fnelscrs_(En,nuID);
      //crssec=fntotpau_(nuID,En);
      std::cout<<E[i]<<"  "<<crs[i]<<"  q2min: "<<q2min<<"  q2max: "<<q2max<<" crq2max: "<<crq2max<<std::endl;
	    //fprintf(stderr,"E %f crs %f \n",E[i],crssec);
    }

  
  TGraph * g=new TGraph(nStep,E,crs);
  g->SetName("gCRS");

  g->Write();
  TGraph * g2=new TGraph(nStep,E2,prob);
  g2->SetName("gProb");
  g2->Write();
  for(size_t i=0;i<vq2G.size();i++)
    {
      vq2G[i]->Write();

    }
  f->Close();
    




}
