#ifndef __ZBSWriter__
#define __ZBSWriter__
#include "Writer.h"

class ZBSWriter:public Writer
{
 public:
  ZBSWriter();

  void OpenFile(std::string fileName);

  void WriteVector(EvtVector * evtVect);

  void CloseFile();

 private:
  int lun;
};



#endif
