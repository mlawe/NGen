/////////////////////////////////////////////////////
/* Event Vector Class.  Holds information about event

Created: Feb 05, 2014 - C. Kachulis
*///////////////////////////////////////////////////
#ifndef _EvtVector_
#define _EvtVector_

#include <vector>
#include "Track.h"
#include <cstdio>
class EvtVector
{
public:
  EvtVector();

  ~EvtVector()
  {
    tracks.clear();
  }
  
  Track * AddNewTrack();
  
  Track*  GetTrack(size_t i);
  size_t GetNumTracks();
  int intType; //interaction Type
  double vertex[3]; //in cm
  double time; //unix time (fractional, in seconds)
  int evtNumber;
private:
  std::vector<Track*> tracks;
  size_t numTracks;

};

#endif
