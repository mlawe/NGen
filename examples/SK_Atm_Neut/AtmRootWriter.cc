#include "AtmRootWriter.h"


void AtmRootWriter::SetupHook()
{
  t->Branch("iorgvc",&iorgvc,"iorgvc[numTracks]/I");
  t->Branch("iflgvc",&iflgvc,"iflgvc[numTracks]/I");
  t->Branch("icrnvc",&icrnvc,"icrnvc[numTracks]/I");
  t->Branch("timvc",&timvc,"timvc[numTracks]/F");
  t->Branch("ivtivc",&ivtivc,"ivtivc[numTracks]/I");
  t->Branch("ivtfvc",&ivtfvc,"ivtfvc[numTracks]/I");
  t->Branch("posivc",&posivc,"posivc[numTracks][3]/F");
  t->Branch("posfvc",&posfvc,"posfvc[numTracks][3]/F");

}


void AtmRootWriter::PrefillHook(EvtVector * evtVect)
{
  Track * tmpTrack;
  for(size_t i=0;i<(size_t)numTracks;i++)
    {
      tmpTrack=evtVect->GetTrack(i);
      iorgvc[i]=tmpTrack->GetUserData<int>("iorgvc");
      iflgvc[i]=tmpTrack->GetUserData<int>("iflgvc");
      icrnvc[i]=tmpTrack->GetUserData<int>("icrnvc");
      timvc[i]=tmpTrack->GetUserData<float>("timvc");
      posivc[i][0]=tmpTrack->GetUserData<float>("posivc_x");
      posivc[i][1]=tmpTrack->GetUserData<float>("posivc_y");
      posivc[i][2]=tmpTrack->GetUserData<float>("posivc_z");
      ivtivc[i]=tmpTrack->GetUserData<int>("ivtivc");
      
      posfvc[i][0]=tmpTrack->GetUserData<float>("posfvc_x");
      posfvc[i][1]=tmpTrack->GetUserData<float>("posfvc_y");
      posfvc[i][2]=tmpTrack->GetUserData<float>("posfvc_z");
      ivtfvc[i]=tmpTrack->GetUserData<int>("ivtfvc");
      
      


    }




}
