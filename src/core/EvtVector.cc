#include "EvtVector.h"

EvtVector::EvtVector()
{
  
  kWrittenOut=false;
 
}
Track* EvtVector::AddNewTrack()
{

  tracks.push_back(new Track);
  

  return tracks.back();
}

Track*  EvtVector::GetTrack(size_t i)
{

  if(i<tracks.size())
    {

     return tracks[i];
    }
  fprintf(stderr,"Cannot get track %zu, only %zu tracks exist\n",i,tracks.size());
  return NULL;
}

size_t EvtVector::GetNumTracks()
{
  
  return tracks.size();

}
