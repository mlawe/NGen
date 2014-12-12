*************************README for atmflux base code*************************************************
Author: Chris Kachulis
Date: Apr 02, 2014

CONTENTS

	1. INTRO
	2. ABSTRACT CLASSES
	   2.1 FluxGen
	   2.2 FlxTable
	   2.3 TargetShape
	   2.4 Writer
	3. HELPER CLASSES YOU MUST INTERACT WITH
	   3.1 EvtVector
	   3.2 Track
	4. WRITING AN APPLICATION
	   4.1 Setup
	   4.2 While loop
	   4.3 Cleanup

 
1. INTRO 

   atmflux is a software package for simulating atmospheric neutrino fluxes based on and intended as an improvement upon neutflux.  It is written in c++ and designed to be flexible with respect to detector, flux model, and input/output data types.  It consits of four abstract base classes which must be inherited by children by an application, as well as three helper classes which the base classes use, and an application may as well.  

2. ABSTRACT CLASSES
   To write an application, you must write children to inherit the three abstract base classes, and then a simple main function which will include some setup and a while loop.  Let's go through the three abstract classes to understand the pure virtual methods your child classes must override     
   2.1 FluxGen
       FluxGen is the class which does most of the work during the simulation.  In the example, see AtmFluxGen.cc. Your child class must override two pure virtual functions:
       	       GetCrossSec(NEUTRINO::FLAVOR, float E) must return the cross section of the particular neutrino flavor at energy E (GeV)
	       ProcessIncomingTrack(Track* track) is called from CreateNextEvent() once an neutrino flavor, direction, momentum, and position have been choosen.  The information is stored in currentVect, and in the input track.  This method must interface with your nuclear interaction simulation package to choose an interaction mode, and create resulting partciles, and the resulting information should be stored in currentVect.
       
   2.2 FlxTable
       FlxTable is the class which holds the information of the neutrino flux.  in the example, see HonFlx.cc.  Your child class must override three pure virtual functions:
       		GetFlux(float phi,float cosZ, float E, NEUTRINO::FLAVOR flav) returns the flux in units of (m^2 sec GeV sr)^-1 .  Note the phi and cosZ are for the direction of the neutrinos momentum, so the opposite of the direction the neutrino is coming from.  Be careful of this possible confusion.
		LoadFLuxTable(std::ifstream& fileStrm) should read in theflux data int the fileStrm and store is somewhere reasonable to be used later in GetFlux
		GetEnergyBins() should return the edges of the energy bins that will be used to integrate the total flux by FluxGen
		
   2.3  TargetShape
   	TargetShape holds the information about the shape and density of your detector target.  See TargetCyl.cc in examples.  Your child class must override three pur virtual functions:
		    GetVolume() must return the volume of your target in m^3
		    GetRandPos(double pos[3], TRandom* rndm) must use the random number generator rndm to set pos to  a randomly distributed position in your target in cm
		    IsWithin(double pos[3]) must return true is pos is inside your target, false if outside (pos in cm)
   2.4 Writer
       Writer is an abstract base class to describe a class which will write out events.  In examples see RootWriter.cc, NuanceWriter.cc, or ZBSWriter.cc.  Your child class must override three pure virtual functions:
       	      OpenFile(std::string) should the a file with the input name and prepare it to be written out to
	      WriteVector(EvtVector *) should write the given EvtVector to the file
	      CloseFile() shoudl do any ending of the file and close 
3. HELPER CLASSES YOU MUST INTERACT WITH
   Your only required interaction with helper classes will be in FluxGen::ProcessIncomingTrack and Writer::WriteVector.  You will need to interact with the following two classes.
   3.1 EvtVector
       FluxGen stores the inforamtion about each event in currentVect which is pointer to an instance of the EvtVector class.  FluxGen will set the evtNumber, the vertex, and the first track (the incoming neutrino) for you, but you are resposible for setting the intType (interaction code) and adding any additional tracks in your FluxGen::ProcessIncomingTrack method.  
   3.2 Track
       Track is the struct which stores the information about each particle in an event.  It is self explanitory if you look at Track.h.  an EvtVector instance contains a vector of tracks.    

   You never need to interact with the other helper classes, they are generally used by the base abstract classes.  You may find it useful to use the UnionShape and DiffShape classes to create odd shapes, but they are in no way required.

4. WRITING AN APPLICATION
   With your three abstract classes you can write an atmospheric neutrino flux simulation with a simple main function.  Your main function will contain three section (see atmflux.cc in example);

   4.1 Setup
       You must create an instance of each of your three child classes and call the follow methods:
       FlxTable::SetSolarAct
       TargetShape::SetDensity
       FluxGen::SetNYears
       FluxGen::SetTarget
       FluxGen::SetTable
       FluxGen::LoadFluxTable (just opens up stringstream from name and callse FlxTable::LoadFluxTable
       Writer::OpenFile
       FluxGen::AddWriter
       Note the you must can only set one table and one target, but you can add as many different writers as you like, and you can call FluxGen::LoadFluxTable with as many different files as you like but the same FlxTable child class must be able to handle them all.
       You should also do any other setting up required here.  Your TargetShape for instance may require some setting of variable like radius or height.
       You should the call FluxGen::CreateEvtRate.  This will calculate the event rate based on the flux table, target, and interaction cross section.    
   
   4.2 While loop	  
       Next you should start a while loop.  In the while loop you should call CreateNextVector.  This will run through the simulation of the next atmospheric neutrino event.  If it returns 0, you should the call WriteCurrentVector(), which will use whatever writers you have specified to write the event out.  If it returns 1, you should not call WriteCurrentVector(), but should continue through the while loop.  this just means that everything worked, but the event should not be written out.  This is used for upmu in sk.  If it returns 2, there are no more events to be simulated, and you should break out of the while loop.

   4.3 Cleanup
       Once you have broken out of the while loop, you should call Writer::CloseFile for all of your writers.  At this point clean up all your memory, and voila, you're done!  
   
