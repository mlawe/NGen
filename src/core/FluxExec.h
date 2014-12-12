#ifndef _FluxExec_
#define _FluxExec_









class FluxExec
{


 public:
  FluxExec();

  ~FluxExec();

  FluxExec(std::vector<char*> cmd);

  void SetCmdLine(std::vector<char*> cmd){cmdline=cmd;};

  void Setup();
  void Run();
  virtual void Cleanup();
  void SetMode(int mode){runmode=mode}; //standard,upmu,tau,upmu-tau,etc...

  int GetMode(){return runmode;};
 protected:
  virtual void RunStandard();
  virtual void RunUpmu();
  virtual void RunUserDefined();
  std::vector<char*> cmdline;
  int runmode;

  virtual void SetupNucSim()=0;
  void SetupGen();
  virtual void SetupUpmuGen()=0;
  virtual void SetupStandardGen()=0;
  virtual void SetupUserDefinedGen();
  void SetupTarget();
  virtual void SetupTargetStandard()=0;
  virtual void SetupTargetUpMu();
  virtual void SetupTargetUserDefined();
  virtual void SetupWriters()=0;
  std::vector<double> Emins,Emaxs,Rs; //for upmu nesting, must be set in SetupUpmuGen
  TargetShape * target;
  SphereTrunc * stupmu;
  TargetShape * detector;
  FluxGen * gen;

  

}
