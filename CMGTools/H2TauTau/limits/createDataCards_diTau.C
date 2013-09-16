void createDataCards_diTau(){
  gROOT->ProcessLine(".L makeDataCard_diTau.C");
  for(Int_t i=125;i<=125;i+=5){
    makeDataCard_diTau("tauTau",1,i);
  }
  gROOT->ProcessLine(".q");
}
