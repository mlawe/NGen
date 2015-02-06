#include <iostream>
#include <math.h>
#include <TFile.h>
#include <TGraph.h>
#include <stdlib.h>
#include "neworkC.h"


extern "C"{

  void necard_();
  void nefillmodel_();
  void nefillver_();


  double cohcrsnerein_(float&,int&,int&);
  double fntotpau_(int&, float&);
  double dcohnerein_(float&,int&,int&,float&,float&,float&,float&);
  
  double dnelsq2_(float&,int&,float&);
  double fnq2min_(float&,int&);
  double fnq2max_(float&,int&);
  double fnels_rpascl_(float&,int&);

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

  int nStep=200;
  float E[nStep];
  float crs[nStep];
  
  float step=log(EHigh/Elow)/float(nStep);
  std::cout<<Elow<<"  "<<EHigh<<"step: "<<step<<std::endl;
  int CC=1;
  //double crssec;
  //float En;
  float Xmin=0;
  float Xmax=1;
  float Zmin=-1;
  float Zmax=1;
  float Ymin,Ymax;
  float phimin=0;
  float phimax=2*3.14159;

  float X,Y,Z,phi;
  int nxstep=200;
  int nystep=200;
  int nzstep=200;
  int nphistep=200;
  float xstep=(Xmax-Xmin)/float(nxstep);
  float zstep=(Zmax-Zmin)/float(nzstep);
  float phistep=(phimax-phimin)/float(nphistep);
  float ystep;
  float En=6.9;
  if(argc>4) En=atof(argv[4]);
  nuID=14;
  int mode=16;
  nework_.modene=mode;
  nework_.ipne[4]=211;
  float totcrs1=cohcrsnerein_(En,nuID,CC);
  float totcrs=0.0;
  // double crsv;
  float prmass=.939566;//.93827;
  float pimass=.13957;
  for(int ix=0;ix<nxstep;ix++)
    {
      std::cout<<ix<<std::endl;
      X=Xmin+float(ix)*xstep;
      Ymin=pimass/En;
      Ymax=1/(1+prmass*X/(2*En));
      ystep=(Ymax-Ymin)/float(nystep);
      for(int iy=0;iy<nystep;iy++)
	{
	  Y=Ymin+float(iy)*ystep;
	  for(int iz=0;iz<nzstep;iz++)
	    {
	      Z=Zmin+zstep*float(iz);
	      for(int iphi=0;iphi<nphistep;iphi++)
		{
		  phi=phimin+float(iphi)*phistep;
		  // crsv=dcohnerein_(En,nuID,mode,X,Y,Z,phi);
		  totcrs+=dcohnerein_(En,nuID,mode,X,Y,Z,phi)*xstep*ystep*zstep*phistep;
		  /*  if(crsv>0)
		    {
		      fprintf(stderr,"crsv: %e \n",crsv);
		      std::cout<<"crsv: "<<crsv<<" "<<xstep<<" "<<ystep<<" "<<zstep<<" "<<phistep<<std::endl;
		      }*/
		}
	      
	    }



	}
      


    }

  //  std::cout<<totcrs1<<" Summed:  "<<totcrs<<std::endl;
  fprintf(stderr,"%f Summed: %e \n",totcrs1,totcrs);
  for(int i=0;i<nStep;i++)
    {
      E[i]=Elow*exp(step*float(i));
      // En=E[i];
      crs[i]=cohcrsnerein_(E[i],nuID,CC);
      //crssec=fntotpau_(nuID,En);
      //std::cout<<E[i]<<"  "<<crs[i]<<std::endl;
	    //fprintf(stderr,"E %f crs %f \n",E[i],crssec);
    }

  TGraph * g=new TGraph(nStep,E,crs);
  g->SetName("gCRS");

  g->Write();
	
  f->Close();
    




}
