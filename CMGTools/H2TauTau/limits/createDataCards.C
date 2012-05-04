void createDataCards(){
  gROOT->ProcessLine(".L makeDataCard.C");
  for(Int_t i=110;i<=145;i+=5){
    makeDataCard("muTau",0,i);
    makeDataCard("muTau",1,i);
    makeDataCard("muTau",2,i);
  }
  gROOT->ProcessLine(".q");
}
