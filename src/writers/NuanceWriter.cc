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
	fprintf(file,"$ vertex   %f   %f   %f     dummy \n",evtVect->vertex.X(),evtVect->vertex.Y(),evtVect->vertex.Z());

	int num=(int)evtVect->GetNumTracks();
	int mid;
	for(int i=0;i<num;i++)
	  {
	   
	    if(evtVect->GetTrack(i)->status!=-1)
	      {
		mid=i;
		break;
	      }
	    fprintf(file,"$ track   %10i   %10.5f   %10.5f   %10.5f   %10.5f   %i \n",evtVect->GetTrack(i)->parID,evtVect->GetTrack(i)->momentum,evtVect->GetTrack(i)->pdir[0],evtVect->GetTrack(i)->pdir[1],evtVect->GetTrack(i)->pdir[2],evtVect->GetTrack(i)->status);

	    
	  }
	fprintf(file,"$ info event: %i \n",evtVect->evtWrittenNumber);


	for(int j=mid;j<num;j++)
	  {

	    fprintf(file,"$ track   %10i   %10.5f   %10.5f   %10.5f   %10.5f   %i \n",evtVect->GetTrack(j)->parID,evtVect->GetTrack(j)->momentum,evtVect->GetTrack(j)->pdir[0],evtVect->GetTrack(j)->pdir[1],evtVect->GetTrack(j)->pdir[2],evtVect->GetTrack(j)->status);
	  
		
	      }
	  }
	fprintf(file,"$end \n");
	
	

	
	


}
  




void NuanceWriter::CloseFile()
{

  if(file)
    {
      fclose(file);
    }

}
