#include "Track.h"


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

Track::~Track()
{
  std::cout<<"deleting track"<<std::endl;
  UserData.clear();

}
