void createDataCards(){
  //previous shell code very slow
// for i in 110 115 120 125 130 135 140 145; do
// echo $i
// rootb makeDataCard.C\(\"muTau\",0,$i\) 
// rootb makeDataCard.C\(\"muTau\",1,$i\) 
// rootb makeDataCard.C\(\"muTau\",2,$i\) 
// done

  gROOT->ProcessLine(".L makeDataCard.C");
  for(Int_t i=110;i<=145;i+=5){
    makeDataCard("muTau",0,i);
    makeDataCard("muTau",1,i);
    makeDataCard("muTau",2,i);
  }
  
}
