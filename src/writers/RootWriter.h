#ifndef __RootWriter__
#define __RootWriter__
#include "Writer.h"
#include "TFile.h"
#include "TTree.h"

class RootWriter: public Writer
{

 public:
  RootWriter();

  void OpenFile(std::string fileName);

  void WriteVector(EvtVector * evtVect);

  void CloseFile();

 private:
  TFile * file;
  TTree * t;

  int intMode;
  int numTracks;
  float pos[3];
  int parID[200];
  float mass[200];
  float momentum[200];
  float pdir[200][3];


};

#endif
