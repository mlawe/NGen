#ifndef __Writer__
#define __Writer__

#include <string>
#include "EvtVector.h"
class Writer
{
 public:
  Writer(){};
  virtual ~Writer(){};
  virtual void OpenFile(std::string)=0;

  virtual void WriteVector(EvtVector *) =0;

  virtual void CloseFile() = 0;

};






#endif
