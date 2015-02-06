#include "ZBSWriter.h"
#include <stdio.h>
#include <string.h>
#include "vcvrtxC.h"
#include "neworkC.h"
#include "vcworkC.h"
extern "C"{
  void kzinit_();
  void kzwrit_(int*);
  void kzend_(int*);
  void fortzbsclose_(int*);
  void fortzbsopen_(int*,const char*,unsigned int);
  void testwrit_(int*);
  void geoset_();
  void kzeclr_();

  void mcmkhd_();
  void mcmkmh_();
  void mcmkwt_();

  void nemknebk_(float*);
  void nemkmodelbk_();
  void nemkcrsbk_();
  
  void nemknpos_();
  void vcmkvc_();
  void vcmkvx_();
  void nemkfsibk_();
  void nemknetarg_();


}
ZBSWriter::ZBSWriter()
{
  //  geoset_();
  //  lun=21;
  lun=-1;
  kzinit_();
}

void ZBSWriter::OpenFile(std::string fileName)
{
  if(lun<0)
    {
      //      geoset_();
      lun=21;

      //kzinit_();
      fortzbsopen_(&lun,fileName.c_str(),strlen(fileName.c_str()));
    }
}

void ZBSWriter::WriteVector(EvtVector * evtVect)
{


  mcmkhd_();
  mcmkmh_();
  mcmkwt_();
  float vtx[3];
  vtx[0]=evtVect->vertex.X();
  vtx[1]=evtVect->vertex.Y();
  vtx[2]=evtVect->vertex.Z();
  nemknebk_(vtx);
  
  nemkmodelbk_();
  
  nemkcrsbk_();
  //nemknpos_();
  if(lun>0)
    {
      //fprintf(stderr,"Really writing zbs\n");
      //set neut commons correctly.  must be done in mutliple places because neut
      //for nework
      nework_.modene=evtVect->intType;
      nework_.numne=(int)evtVect->GetNumTracks();
      //for vcwork
      vcwork_.nvc=evtVect->intType;

      //      if(evtVect->GetUserData<bool>("data1")) std::cout<<"data1 true"<<std::endl;
      //else std::cout<<"data1 false"<<std::endl;
      //double *a=evtVect->GetUserData<double*>("arr");
      //std::cout<<"arr[0]: "<<a[0]<<" arr[1]: "<<a[1]<<" arr[2]: "<<a[2]<<std::endl;
      for(int i=0;i<3;i++)
	{
	   //for vcvrtx
	  vcvrtx_.pvtxvc[0][i]=(float)evtVect->vertex[i];
	  vcwork_.posvc[i]=(float)evtVect->vertex[i];     
	}
      //loopthrough particles
      Track * tmpTrack;
      for(size_t i=0;i<evtVect->GetNumTracks();i++)
	{
	  tmpTrack=new Track(*evtVect->GetTrack(i));
	  //tmpTrack=evtVect->GetTrack(i);

	  nework_.ipne[i]=tmpTrack->parID;
	  vcwork_.ipvc[i]=tmpTrack->parID;
	  for(int j=0;j<3;j++)
	    {
	      nework_.pne[i][j]=(float)tmpTrack->pdir[j]*tmpTrack->momentum/1000.; //nework is in GeV
	      vcwork_.pvc[i][j]=(float)tmpTrack->pdir[j]*tmpTrack->momentum;//vcwork is in MeV

	      //do i need to adjust other vcwork position variable (posivc,posfvc)?
	    }

	  //	  std::cout<<" ZBSWriter::  testchar1: "<<tmpTrack->GetUserData<std::string>("testchar1")<<std::endl;
	  //std::cout<<" ZBSWriter::  testchar2: "<<tmpTrack->GetUserData<double>("testchar2")<<std::endl;
	  //	  std::cout<<"testchar1: "<<tmpTrack->GetUserData<std::string>("testchar1");


	}
      
      

      vcmkvc_();
      vcmkvx_();
      nemkfsibk_();
      nemknetarg_();
      

      kzwrit_(&lun);
      kzeclr_();
     
    }
}

void ZBSWriter::CloseFile()
{
  if(lun>0)
    {
      kzend_(&lun);
      fortzbsclose_(&lun);
    }
}

