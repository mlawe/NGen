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
#include "TVector3.h"
class Track
{
 public:
  int parID;
  float mass;//in MeV
  float momentum;//in MeV
  TVector3 pdir;
  int status;

  ~Track();
  template<typename T>
    void SetUserData(std::string,T);

  
  template<typename T>
    T GetUserData(std::string);

private:
std::map<std::string,boost::any> UserData;

};


template<typename T>
void Track::SetUserData(std::string key, T data)
{
  UserData[key]=static_cast<T>(data); 

}
template<typename T>
T Track::GetUserData(std::string key)
{
  if(UserData.find(key)==UserData.end())
    {

      std::cerr<<"No User Data key "<<key<<" in Track"<<std::endl;
      exit(1);
    }

  return boost::any_cast<T>(UserData[key]);


}




#endif
