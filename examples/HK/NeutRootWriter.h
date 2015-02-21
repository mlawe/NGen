#ifndef __NeutRootWriter__
#define __NeutRootWriter__
#include <RootWriter.h>

class NeutRootWriter: public RootWriter
{
 protected:
  void SetupHook();
  void PrefillHook(EvtVector*);


 private:
  int iorgvc[200];
  //  int iflgvc[200];
  int icrnvc[200];
  float timvc[200];
  float posivc[200][3];
  int ivtivc[200];
  float posfvc[200][3];
  int ivtfvc[200];









};


#endif
