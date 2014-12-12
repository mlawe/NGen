#include "NuanceWriter.h"
#include <algorithm>

NuanceWriter::NuanceWriter()
{
  file=NULL;

}
void NuanceWriter::OpenFile(std::string fileName)
{
  if(!file)
    {
      fprintf(stderr,"Open file\n");
      file=fopen(fileName.c_str(),"w");
    }
}


void NuanceWriter::WriteVector(EvtVector * evtVect)
{


  if(file)
    {

        fprintf(file,"$ begin\n");
	fprintf(file,"$ neut    %i \n",evtVect->intType); //find interactiontype!
	fprintf(file,"$ vertex   %f   %f   %f     dummy \n",evtVect->vertex[0],evtVect->vertex[1],evtVect->vertex[2]);

	int num=std::min(2,(int)evtVect->GetNumTracks());
	for(int i=0;i<num;i++)
	  {
	   

	    fprintf(file,"$ track   %10i   %10.5f   %10.5f   %10.5f   %10.5f   -1 \n",evtVect->GetTrack(i)->parID,evtVect->GetTrack(i)->momentum,evtVect->GetTrack(i)->pdir[0],evtVect->GetTrack(i)->pdir[1],evtVect->GetTrack(i)->pdir[2]);

	    
	  }
	fprintf(file,"$ info event: %i \n",evtVect->evtNumber);


	for(int j=0;j<2;j++)
	  {
	    int k=-2+2*j;
	    for(size_t i=2;i<evtVect->GetNumTracks();i++)
	      {

		  fprintf(file,"$ track   %10i   %10.5f   %10.5f   %10.5f   %10.5f   %i \n",evtVect->GetTrack(i)->parID,evtVect->GetTrack(i)->momentum,evtVect->GetTrack(i)->pdir[0],evtVect->GetTrack(i)->pdir[1],evtVect->GetTrack(i)->pdir[2],k);
	  
		
	      }
	  }
	fprintf(file,"$end \n");
	
	

	
	


    }
  

}


void NuanceWriter::CloseFile()
{

  if(file)
    {
      fclose(file);
    }

}
