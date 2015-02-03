#include "RootWriter.h"

RootWriter::RootWriter()
{
  file=NULL;
  
}

void RootWriter::OpenFile(std::string fileName)
{
  if(!file)
    {
      file=new TFile(fileName.c_str(),"recreate");
      t= new TTree("fluxTree","");
      t->Branch("intMode",&intMode,"intMode/I");
      t->Branch("numTracks",&numTracks,"numTracks/I");
      t->Branch("pos",&pos,"pos[3]/F");
      t->Branch("parID",parID,"parID[numTracks]/I");
      t->Branch("mass",mass,"mass[numTracks]/F");
      t->Branch("momentum",momentum,"momentum[numTracks]/F");
      t->Branch("pdir",pdir,"pdir[numTracks][3]/F");
      t->Branch("time",&time,"time/F");
      SetupHook();
    }
}

void RootWriter::WriteVector(EvtVector * evtVect)
{
  //fprintf(stderr,"Writing root file\n");
  if(file)
    {
      time=double(evtVect->evtTime.seconds)+double(evtVect->evtTime.nanoseconds)*1.E-9;
      intMode=evtVect->intType;
      numTracks=(int)evtVect->GetNumTracks();
      pos[0]=evtVect->vertex.X();
      pos[1]=evtVect->vertex.Y();
      pos[2]=evtVect->vertex.Z();

      for(int i=0;i<numTracks;i++)
	{
	  parID[i]=evtVect->GetTrack((size_t)i)->parID;
	  mass[i]=evtVect->GetTrack((size_t)i)->mass;
	  momentum[i]=evtVect->GetTrack((size_t)i)->momentum;
	  for(int j=0;j<3;j++)
	    {
	      pdir[i][j]=evtVect->GetTrack((size_t)i)->pdir[j];
	    }
	}
      PrefillHook(evtVect);
      t->Fill();
    }
      

}

void RootWriter::CloseFile()
{
  if(file)
    {
      file->cd();
      t->Write();
      file->Close();
    }

}
