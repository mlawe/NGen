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
	fprintf(file,"$ vertex   %f   %f   %f   %e \n",evtVect->vertex.X(),evtVect->vertex.Y(),evtVect->vertex.Z(),evtVect->evtTime.Val());

	int num=(int)evtVect->GetNumTracks();

	for(int i=0;i<2;i++)
	  {
	   
	    fprintf(file,"$ track   %10i   %10.5f   %10.5f   %10.5f   %10.5f   %i \n",evtVect->GetTrack(i)->parID,evtVect->GetTrack(i)->momentum,evtVect->GetTrack(i)->pdir[0],evtVect->GetTrack(i)->pdir[1],evtVect->GetTrack(i)->pdir[2],evtVect->GetTrack(i)->status);

	    
	  }
	fprintf(file,"$ info %i 0 0\n",evtVect->evtWrittenNumber);


	for(int j=2;j<num;j++)
	  {

	    if(evtVect->GetTrack(j)->status==-1) continue;
	    fprintf(file,"$ track   %10i   %10.5f   %10.5f   %10.5f   %10.5f   %i \n",evtVect->GetTrack(j)->parID,evtVect->GetTrack(j)->momentum,evtVect->GetTrack(j)->pdir[0],evtVect->GetTrack(j)->pdir[1],evtVect->GetTrack(j)->pdir[2],evtVect->GetTrack(j)->status);
	  
		
	      }
	  }
	fprintf(file,"$ end \n");
	
	

	
	


}
  




void NuanceWriter::CloseFile()
{

  if(file)
    {
      fprintf(file,"$ stop\n");
      fclose(file);
    }

}
