////////////////////////////////////////////////////////////////
/* Track Struct

Created: Feb 05, 2014 - C. Kachulis
*//////////////////////////////////////////////////////////////

#ifndef _Track_
#define _Track_

#include <string>
#include <map>
//#include "Container.h"
#include <boost/any.hpp>
#include <iostream>
class Track
{
 public:
  int parID;
  float mass;//in MeV
  float momentum;//in MeV
  float pdir[3];
  
  ~Track();
template<typename T>
 void SetUserData(std::string,T);

  
template<typename T>
  T GetUserData(std::string);

private:
std::map<std::string,boost::any> UserData;

};




#endif
