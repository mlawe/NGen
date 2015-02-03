/////////////////////////////////////////////////////
/* Event Vector Class.  Holds information about event

Created: Feb 05, 2014 - C. Kachulis
*///////////////////////////////////////////////////
#ifndef _EvtVector_
#define _EvtVector_

#include <vector>
#include "Track.h"
#include <cstdio>
#include <boost/any.hpp>
#include <iostream>
#include "TimeStruct.h"
#include "TVector3.h"
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
  TVector3 vertex; //in cm
  Time_Struct evtTime; //time of eveent
  int evtNumber;
  int evtWrittenNumber;
  void WrittenOut(){kWrittenOut=true;};
  bool IsWrittenOut(){return kWrittenOut;};
template<typename T>
void SetUserData(std::string,T);

  
template<typename T>
  T GetUserData(std::string);

private:
 bool kWrittenOut;

  std::vector<Track*> tracks;
  size_t numTracks;
  std::map<std::string,boost::any> UserData;
};


template<typename T>
void EvtVector::SetUserData(std::string key, T data)
{
  UserData[key]=static_cast<T>(data); 

}
template<typename T>
T EvtVector::GetUserData(std::string key)
{
  if(UserData.find(key)==UserData.end())
    {

      std::cerr<<"No User Data key "<<key<<" in Track"<<std::endl;
      exit(1);
    }

  return boost::any_cast<T>(UserData[key]);


}

#endif
