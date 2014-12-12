#ifndef _NuanceWriter_
#define _NuanceWriter_
#include "Writer.h"
#include "fcntl.h"
#include <stdio.h>

class NuanceWriter: public Writer
{
 public:
  NuanceWriter();
  void OpenFile(std::string fileName);

  void WriteVector(EvtVector * evtVect);

  void CloseFile();

 private:
  FILE * file;

  



};








#endif
