void batchSample(const char * name){
  ///afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/workdir
  gROOT->ProcessLine(".x ./TauMuConfig_.C");
  gROOT->ProcessLine("analysis.init()");
  gROOT->ProcessLine(TString("analysis.createHistos\(\"")+name+"\"\)");
}
