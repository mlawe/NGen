#include "Track.h"



Track::~Track()
{
  std::cout<<"deleting track"<<std::endl;
  UserData.clear();

}
