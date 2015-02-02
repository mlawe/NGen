#include "EvtVector.h"

EvtVector::EvtVector()
{
  numTracks=0;
  kWrittenOut=false;
 
}
Track* EvtVector::AddNewTrack()
{

  tracks.push_back(new Track);
  numTracks++;
  if(numTracks!=tracks.size())
    {
      fprintf(stderr,"Warning: numTracks did not match size of tracks(vector)\n");
      numTracks=tracks.size();
      
    }

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
  if(numTracks!=tracks.size())
    {
      fprintf(stderr,"Warning: numTracks did not match size of tracks(vector)\n");
      numTracks=tracks.size();
      
    }

  return numTracks;

}
