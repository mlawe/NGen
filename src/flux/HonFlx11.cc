#include "HonFlx11.h"
#include "TVector3.h"
#include <cmath>
#ifndef __PI__
#define __PI__
const double Pi=4.0*atan(1.0);
#endif

HonFlx11::HonFlx11() : FlxTable::FlxTable()
{
  hfluxLoaded=false;
  lowfluxLoaded=false;

}

std::vector<float> HonFlx11::GetEnergyBins()
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
double HonFlx11::GetFlux(float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{

  if(!(lowfluxLoaded and hfluxLoaded))
    {
      fprintf(stderr,"Error:Flux not fully loaded\n");
      return -1;
    }
  if(solarAct<0)
    {
      fprintf(stderr,"Error: Solar Activity Not Set\n");
      return -1;
    }
  if(honE[0]>E or honE[146]<E)
    {
      fprintf(stderr,"Error: Energy out of range, E= %f  %f %f\n",E,honE[0],honE[140]);
      return -1;
    }
  if(nuFlav>NEUTRINO::MU_BAR) return 0.;
  //find energy just below and above E
  int iE=0;
  for(int i=0;i<147;i++)
    {
      if(honE[i]>E)
	{
	  iE=i;
	  break;
	}
    }


	 
  //get A and B for min solar Activity and max solar Activity

  double BMin=log(double(honTableEn[iE-1][nuFlav][0]/honTableEn[iE][nuFlav][0]))/log(double(honE[iE-1]/honE[iE]));
  double AMin=honTableEn[iE-1][nuFlav][0]/pow(honE[iE-1],BMin);

  double BMax=log(double(honTableEn[iE-1][nuFlav][1]/honTableEn[iE][nuFlav][1]))/log(double(honE[iE-1]/honE[iE]));
  double AMax=honTableEn[iE-1][nuFlav][1]/pow(honE[iE-1],BMax);

  double flux=solarAct*(AMax*pow(E,BMax))+(1-solarAct)*(AMin*pow(E,BMin));
  //  fprintf(stderr,"E %f flux %e\n",E,flux);
  return flux;
}

double HonFlx11::GetFlux(float phi, float cosZ,float E,NEUTRINO::FLAVOR nuFlav,Time_Struct t)
{
  
  if(!(lowfluxLoaded and hfluxLoaded))
    {
      fprintf(stderr,"Error:Flux not fully loaded\n");
      return -1;
    }
 
  if(solarAct<0)
    {
      fprintf(stderr,"Error: Solar Activity Not Set\n");
      return -1;
    }
 if(phi<0 or phi>2*Pi)
   {
     fprintf(stderr,"Error: phi out of range\n");
     return -1;
   }
 if(cosZ<-1 or cosZ>1)
   {
     fprintf(stderr,"Error: cosZ out of range\n");
     return -1;
   }
 if(honE[0]>E or honE[146]<E)
   {
     fprintf(stderr,"Error: Energy out of range angle version E =%f\n",E);
     return -1;
   }
 
 if(nuFlav>NEUTRINO::MU_BAR) return 0.;
 int iCosZL,iPhiL,iEL;//index lower entry (below value)
 int iCosZH,iPhiH,iEH;//index uper entry (above value)
 float CosZL,PhiL;
 float CosZH,PhiH;
 //gemoetric conversion between sk coords and honda coords
 /*cosZ=-cosZ;
 phi=-phi;
 float x=cos(phi);
 float y=sin(phi);
 TVector3 v;
 v.SetX(-cos(Pi*2435./60./180.)*x+sin(Pi*2435./60./180.)*y);
 v.SetY(sin(Pi*2435./60./180.)*x+cos(Pi*2435./60./180.)*y);
 phi=v.Phi();*/
 //conversion relic of old system, no longer necessary
 TVector3 v;
 

 if(phi<0)
   {
     phi=2*Pi+phi;
   }
 if(cosZ<-0.95)
   {
     iCosZL=0;
     iCosZH=0;
    }
 else if(cosZ>0.95)
   {
     iCosZL=19;
     iCosZH=19;
   }
 else
   {
     iCosZL=int(cosZ*10+9.5);
     iCosZH=iCosZL+1;
   }

 CosZL=float(iCosZL+1)*0.1-1.05;
 CosZH=float(iCosZH+1)*0.1-1.05;

 if((phi<Pi/12.) or (phi>23.*Pi/12.))
   {
     iPhiL=11;
     iPhiH=0;
   }
 else
   {
     iPhiL=int(6.*phi/Pi-0.5);
     iPhiH=iPhiL+1;
   }
 if(iPhiL==11)
   {
     PhiL=-Pi/12.;
   }
 else
   {
     PhiL=Pi/6.*(float(iPhiL+1)-0.5);
   }
 PhiH=Pi/6.*(float(iPhiH+1)-0.5);
 if(phi>23.*Pi/12.)
   {
     phi=phi-2.*Pi;
   }
 iEL=0;
 for(iEH=0;iEH<147;iEH++)
   {
     if(honE[iEH]>E)
       {
	 iEL=iEH-1;
	 break;
       }
   }

 //use wieghted averages to move down to fluxHE and fluxLE, the computed fluxes for the energy bin above and below E

 float fluxELCosZLPhiL=honTable[iEL][iCosZL][iPhiL][nuFlav][0]*(1-solarAct)+honTable[iEL][iCosZL][iPhiL][nuFlav][1]*solarAct;
 float fluxELCosZLPhiH=honTable[iEL][iCosZL][iPhiH][nuFlav][0]*(1-solarAct)+honTable[iEL][iCosZL][iPhiH][nuFlav][1]*solarAct;
 
 float fluxELCosZL=(fluxELCosZLPhiH-fluxELCosZLPhiL)*(phi-PhiL)/(PhiH-PhiL)+fluxELCosZLPhiL;


 float fluxELCosZHPhiL=honTable[iEL][iCosZH][iPhiL][nuFlav][0]*(1-solarAct)+honTable[iEL][iCosZH][iPhiL][nuFlav][1]*solarAct;
 float fluxELCosZHPhiH=honTable[iEL][iCosZH][iPhiH][nuFlav][0]*(1-solarAct)+honTable[iEL][iCosZH][iPhiH][nuFlav][1]*solarAct;
 
 float fluxELCosZH=(fluxELCosZHPhiH-fluxELCosZHPhiL)*(phi-PhiL)/(PhiH-PhiL)+fluxELCosZHPhiL;

 float fluxEL;
 if(iCosZH==iCosZL)
   {
     fluxEL=fluxELCosZL;
   }
 else
   {
     fluxEL=(fluxELCosZH-fluxELCosZL)*(cosZ-CosZL)/(CosZH-CosZL)+fluxELCosZL;
   }



 float fluxEHCosZLPhiL=honTable[iEH][iCosZL][iPhiL][nuFlav][0]*(1-solarAct)+honTable[iEH][iCosZL][iPhiL][nuFlav][1]*solarAct;
 float fluxEHCosZLPhiH=honTable[iEH][iCosZL][iPhiH][nuFlav][0]*(1-solarAct)+honTable[iEH][iCosZL][iPhiH][nuFlav][1]*solarAct;
 
 float fluxEHCosZL=(fluxEHCosZLPhiH-fluxEHCosZLPhiL)*(phi-PhiL)/(PhiH-PhiL)+fluxEHCosZLPhiL;


 float fluxEHCosZHPhiL=honTable[iEH][iCosZH][iPhiL][nuFlav][0]*(1-solarAct)+honTable[iEH][iCosZH][iPhiL][nuFlav][1]*solarAct;
 float fluxEHCosZHPhiH=honTable[iEH][iCosZH][iPhiH][nuFlav][0]*(1-solarAct)+honTable[iEH][iCosZH][iPhiH][nuFlav][1]*solarAct;
 
 float fluxEHCosZH=(fluxEHCosZHPhiH-fluxEHCosZHPhiL)*(phi-PhiL)/(PhiH-PhiL)+fluxEHCosZHPhiL;

 float fluxEH;
  
 if(iCosZH==iCosZL)
   {
     fluxEH=fluxEHCosZL;
   }
 else
   {
     fluxEH=(fluxEHCosZH-fluxEHCosZL)*(cosZ-CosZL)/(CosZH-CosZL)+fluxEHCosZL;
   }

 //use same technique as in energy only flux now
 
 

 double B=log(double(fluxEL/fluxEH))/log(double(honE[iEL]/honE[iEH]));
 double A=fluxEL/pow(honE[iEL],B);
 


 double flux=A*pow(E,B);
 return flux;

}


int HonFlx11::LoadFluxTable(std::string fileName)
 {
   std::ifstream fileStrm;
   fileStrm.open(fileName.c_str());

   //check if filestream open
   if(!fileStrm.is_open())
     {
       fprintf(stderr,"Error: file not opened\n");
       return -1;
     }

   char firstWord[200];
   //   char firstWordC[8];
   fileStrm.getline(firstWord,200);
   // snprintf(firstWordC,8,firstWord);
   if(strncmp(firstWord,"average",7)!=0)
     {
       fprintf(stderr,"Wrong format for Honda Flux file\n");
       return -1;
       }
   char line[200];
   

   bool firstLine=true;
   int maxVal=1;
   for(int iS=0;iS<2;iS++)
     {

       for(int iCosZ=0;iCosZ<20;iCosZ++)
	 {
	   for(int iPhi=0;iPhi<12;iPhi++)
	     {
	      
	       //read past column label lines
	       int nL=2;
	       if(firstLine)
		 {
		   nL=1;
		   firstLine=false;
		 }
	       for(int il=0;il<nL;il++)
		 {
		   fileStrm.getline(line,200);
		 }
	       for(int iE=0;iE<maxVal;iE++)
		 {
		   if(!fileStrm.good())
		     {
		       fprintf(stderr,"Error reading in Honda File\n");
		       return -1;
		     }
		   if(iE==0)
		     {

		       float E;
		       fileStrm>>E;
		      
		       if(E<1E-2)
			 {
			   maxVal=101;
			   lowfluxLoaded=true;
			 }
		       else
			 {
			   iE=40;
			   maxVal=141;
			   hfluxLoaded=true;
			
			 }
		       honE[iE]=E;
		     }
		   else
		     {
		       fileStrm>>honE[iE];
		     }
		   if(!fileStrm.good())
		     {
		       fprintf(stderr,"Error reading in Honda File\n");
		       return -1;
		     }
		  
		   fileStrm>>honTable[iE][iCosZ][iPhi][NEUTRINO::MU][iS];
		  
		   if(!fileStrm.good())
		     {
		       fprintf(stderr,"Error reading in Honda File\n");
		       return -1;
		     }
		  
		   fileStrm>>honTable[iE][iCosZ][iPhi][NEUTRINO::MU_BAR][iS];
		  
		   if(!fileStrm.good())
		     {
		       fprintf(stderr,"Error reading in Honda File\n");
		       return -1;
		     }
		  
		   fileStrm>>honTable[iE][iCosZ][iPhi][NEUTRINO::E][iS];
		  
		   if(!fileStrm.good())
		     {
		       fprintf(stderr,"Error reading in Honda File\n");
		       return -1;
		     }
		  
		   fileStrm>>honTable[iE][iCosZ][iPhi][NEUTRINO::E_BAR][iS];
		   		  
		   // read past end of line space
		   fileStrm.getline(line,200);
		 }
	     }
	 }

     }
   if(fileStrm.bad())
     {
       fprintf(stderr,"Error reading in Honda File \n");
       return -1;
     }
   //Input Volkova flux for highest energy bins
   
   float VolkFlux[8][10]={{413.8,289.0,216.2,173.8,150.5,138.7,129.2,120.4,112.5,105.2},
	    {0.3002E+03, 0.1987E+03, 0.1382E+03, 0.1046E+03, 0.8675E+02,
	     0.7715E+02, 0.6979E+02, 0.6350E+02, 0.5814E+02, 0.5340E+02},
	    {0.1816E+03, 0.1151E+03, 0.7684E+02, 0.5652E+02, 0.4583E+02,
	     0.4016E+02, 0.3594E+02, 0.3240E+02, 0.2944E+02, 0.2685E+02},
	    {0.8730E+02, 0.5288E+02, 0.3421E+02, 0.2475E+02, 0.1989E+02,
	     0.1737E+02, 0.1553E+02, 0.1400E+02, 0.1271E+02, 0.1159E+02},
	    {0.4445E+02, 0.2662E+02, 0.1690E+02, 0.1214E+02, 0.9766E+01,
	     0.8452E+01, 0.7490E+01, 0.6725E+01, 0.6111E+01, 0.5588E+01},
	    {0.1578E+02, 0.9449E+01, 0.6002E+01, 0.4351E+01, 0.3525E+01,
	     0.3023E+01, 0.2654E+01, 0.2377E+01, 0.2165E+01, 0.1992E+01},
	    {0.5154E+01, 0.3082E+01, 0.1955E+01, 0.1417E+01, 0.1146E+01,
	     0.9835E+00, 0.8633E+00, 0.7727E+00, 0.7033E+00, 0.6460E+00},
	    {0.1486E+01, 0.8884E+00, 0.5658E+00, 0.4084E+00, 0.3282E+00,
	     0.2840E+00, 0.2517E+00, 0.2260E+00, 0.2054E+00, 0.1878E+00}};
  
   float VolkFluxBar[8][10]={{271.0,218.3,177.2,145.5,121.0,101.5, 85.4, 72.0, 60.7, 51.3},
	       {0.1819E+03, 0.1389E+03, 0.1049E+03, 0.8103E+02, 0.6455E+02,
		0.5225E+02, 0.4270E+02, 0.3514E+02, 0.2903E+02, 0.2410E+02},
	       {0.1028E+03, 0.7514E+02, 0.5442E+02, 0.4089E+02, 0.3184E+02,
		0.2539E+02, 0.2053E+02, 0.1674E+02, 0.1373E+02, 0.1131E+02},
	       {0.4606E+02, 0.3217E+02, 0.2259E+02, 0.1669E+02, 0.1288E+02,
		0.1024E+02, 0.8269E+01, 0.6742E+01, 0.5525E+01, 0.4553E+01},
	       {0.2208E+02, 0.1525E+02, 0.1051E+02, 0.7711E+01, 0.5956E+01,
		0.4692E+01, 0.3756E+01, 0.3051E+01, 0.2501E+01, 0.2067E+01},
	       {0.7371E+01, 0.5090E+01, 0.3508E+01, 0.2597E+01, 0.2021E+01,
		0.1578E+01, 0.1251E+01, 0.1013E+01, 0.8331E+00, 0.6925E+00},
	       {0.2282E+01, 0.1574E+01, 0.1083E+01, 0.8017E+00, 0.6231E+00,
		0.4866E+00, 0.3858E+00, 0.3124E+00, 0.2565E+00, 0.2130E+00},
	       {0.6226E+00, 0.4292E+00, 0.2966E+00, 0.2187E+00, 0.1688E+00,
		0.1329E+00, 0.1064E+00, 0.8647E-01, 0.7090E-01, 0.5859E-01}};

  float VolkE[8]={.316E+04,.100E+05,.300E+05,.100E+06,.300E+06,.100E+07,.300E+07,.100E+08};

  float VolkCoeff[4]={0.03039,0.03375,0.96184,0.84404};

  for(int iS=0;iS<2;iS++)
    {
       for(int iCosZ=0;iCosZ<20;iCosZ++)
	 {
	   int iCosZV;
	   if(iCosZ<10)
	     {
	       iCosZV=iCosZ;
	     }
	   else
	     {
	       iCosZV=iCosZ-10;
	     }
	   for(int iPhi=0;iPhi<12;iPhi++)
	     {
	       for(int iE=141;iE<147;iE++)
		 {
		   honE[iE]=VolkE[iE-139];
		   honTable[iE][iCosZ][iPhi][NEUTRINO::MU][iS]=VolkFlux[iE-139][iCosZV]*VolkCoeff[NEUTRINO::MU]/pow(honE[iE],3);
		   honTable[iE][iCosZ][iPhi][NEUTRINO::MU_BAR][iS]=VolkFluxBar[iE-139][iCosZV]*VolkCoeff[NEUTRINO::MU_BAR]/pow(honE[iE],3);
		   honTable[iE][iCosZ][iPhi][NEUTRINO::E][iS]=VolkFlux[iE-139][iCosZV]*VolkCoeff[NEUTRINO::E_BAR]/pow(honE[iE],3);;
		   honTable[iE][iCosZ][iPhi][NEUTRINO::E_BAR][iS]=VolkFluxBar[iE-139][iCosZV]*VolkCoeff[NEUTRINO::E_BAR]/pow(honE[iE],3);;
		 }
	     }
	   
	 }
     }

   if(lowfluxLoaded && hfluxLoaded)
     {
       FillIntegratedTables();
     }
   fileStrm.close();
   return 0;
 }

 void HonFlx11::FillIntegratedTables()
 {
   if(!(lowfluxLoaded and hfluxLoaded))
     {
       fprintf(stderr,"Error: Cannot fill compressed tables without high and low tabels\n");
       return;
     }

     for(int iS=0;iS<2;iS++)
       {
	 for(int nuFlav=0;nuFlav<4;nuFlav++)
	   {
	     for(int iE=0;iE<147;iE++)
	       {
		 honTableEn[iE][nuFlav][iS]=0;
		 for(int iCosZ=0;iCosZ<20;iCosZ++)
		   {
		     for(int iPhi=0;iPhi<12;iPhi++)
		       {
			 
			 honTableEn[iE][nuFlav][iS]=honTableEn[iE][nuFlav][iS]+honTable[iE][iCosZ][iPhi][nuFlav][iS];
			 
		       }

		   }
		 honTableEn[iE][nuFlav][iS]=honTableEn[iE][nuFlav][iS]*0.1*Pi/6.;

	       }
	   }
		
		 
	       
       }
 }


