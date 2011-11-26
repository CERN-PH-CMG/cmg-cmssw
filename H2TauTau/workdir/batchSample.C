void batchSample(const char * name){
  gROOT->ProcessLine(".x ./TauMuConfig.C");
  gROOT->ProcessLine("analysis.init()");
  gROOT->ProcessLine(TString("analysis.createHistos\(\"")+name+"\"\)");
}
