#include "CMGTools/HToZZTo4Leptons/interface/pseudoMELA.h"

pseudoMELA::pseudoMELA(){

  z1mass_rrv = new RooRealVar("z1mass","m_{Z1}",0,180);
  z2mass_rrv = new RooRealVar("z2mass","m_{Z2}",0,120);
  costheta1_rrv = new RooRealVar("costheta1","cos#theta_{1}",-1,1);
  costheta2_rrv = new RooRealVar("costheta2","cos#theta_{2}",-1,1);
  phi_rrv= new RooRealVar("phi","#Phi",-3.1415,3.1415);
  costhetastar_rrv = new RooRealVar("costhetastar","cos#theta^{*}",-1,1);
  phistar1_rrv= new RooRealVar("phistar1","#Phi^{*}_{1}",-3.1415,3.1415);
  mzz_rrv= new RooRealVar("mzz","mZZ",80,1000);

  SMHiggs = new AngularPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);
  SMHiggs->makeSMHiggs();
  SMHiggs->makeParamsConst(true);

  PSHiggs = new AngularPdfFactory(z1mass_rrv,z2mass_rrv,costheta1_rrv,costheta2_rrv,phi_rrv,mzz_rrv);

  PSHiggs->makePSHiggs();
  PSHiggs->makeParamsConst(true);

}

  //template <typedef U>
void pseudoMELA::checkZorder(float& z1mass, float& z2mass,
			    float& costhetastar, float& costheta1,
			    float& costheta2, float& phi,
			    float& phistar1){
  
  float tempZ1mass=z1mass;
  float tempZ2mass=z2mass;
  float tempH1=costheta1;
  float tempH2=costheta2;
  float tempHs=costhetastar;
  float tempPhi1=phistar1;
  float tempPhi=phi;

  if(z2mass>z1mass){

    z1mass=tempZ2mass;
    z2mass=tempZ1mass;
    costhetastar=-tempHs;
    costheta1=tempH2;
    costheta2=tempH1;
    phi=tempPhi;
    phistar1=-tempPhi1-tempPhi;
    if(phistar1>3.1415)
      phistar1=phistar1-2*3.1415;
    if(phistar1<-3.1415)
      phistar1=phistar1+2*3.1415;

  }else

    return;

}


  //template <typedef U>
float pseudoMELA::eval(float zzmass, float z1mass, 
		      float z2mass, float costhetastar, 
		      float costheta1, float costheta2, 
		      float phi, float phistar1){

  if(zzmass<100.0 || zzmass>179.9999)
    return 0.0;

    checkZorder(z1mass,z2mass,costhetastar,costheta1,costheta2,phi,phistar1);

    z1mass_rrv->setVal(z1mass);
    z2mass_rrv->setVal(z2mass);
    costhetastar_rrv->setVal(costhetastar);
    costheta1_rrv->setVal(costheta1);
    costheta2_rrv->setVal(costheta2);
    phi_rrv->setVal(phi);
    phistar1_rrv->setVal(phistar1);

    mzz_rrv->setVal(zzmass);

    return 1/(1+PSHiggs->getVal(zzmass)/SMHiggs->getVal(zzmass));
    
}


