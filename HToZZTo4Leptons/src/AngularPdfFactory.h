#ifndef AngPdfFact_h
#define AngPdfFact_h
// Included verbatim from UserCode/scasasso/HZZ4lAnalysis/HZZ4lCommon/src tag V00-00-00

#include "RooRealVar.h"
#include "RooXZsZs_5D.h"
#include <cmath>

class AngularPdfFactory{

public:

  RooRealVar* mZ;     
  RooRealVar* mX;     
  RooRealVar* gamZ;   
    
  RooRealVar* a1Val;  
  RooRealVar* phi1Val;
  RooRealVar* a2Val;  
  RooRealVar* phi2Val;
  RooRealVar* a3Val;  
  RooRealVar* phi3Val;
    
  RooRealVar* R1Val;  
  RooRealVar* R2Val;  
  
  RooXZsZs_5D *PDF;

  int modelIndex;  //0 - SM Higgs, 1 - PS Higgs, 2 - Fully Longitudinal Scalar, -1 - Custom

  AngularPdfFactory(RooRealVar* m1,RooRealVar* m2,RooRealVar* h1,RooRealVar* h2,RooRealVar* Phi,RooRealVar* mZZ){

    // Parameters
    mZ     = new RooRealVar("mZ","mZ",91.188);
    gamZ   = new RooRealVar("gamZ","gamZ",2.5);
           
    a1Val  = new RooRealVar("a1Val","a1Val",1);
    phi1Val= new RooRealVar("phi1Val","phi1Val",0);
    a2Val  = new RooRealVar("a2Val","a2Val",0);
    phi2Val= new RooRealVar("phi2Val","phi2Val",0);
    a3Val  = new RooRealVar("a3Val","a3Val",0);
    phi3Val= new RooRealVar("phi3Val","phi3Val",0);
           
    R1Val  = new RooRealVar("R1Val","R1Val",0.15);
    R2Val  = new RooRealVar("R2Val","R2Val",0.15);

    PDF = new RooXZsZs_5D("PDF","PDF",*m1,*m2,*h1,*h2,*Phi,*a1Val,*phi1Val,*a2Val,*phi2Val,*a3Val,*phi3Val,*mZ,*gamZ,*mZZ,*R1Val,*R2Val);

  };

  ~AngularPdfFactory(){

    delete mZ;
    delete gamZ;
    delete a1Val;
    delete phi1Val;
    delete a2Val;
    delete phi2Val;
    delete a3Val;
    delete phi3Val;
    delete R1Val;
    delete R2Val;
    
    delete PDF;

  };

  void makeSMHiggs(){
    a1Val->setVal(1.0);
    phi1Val->setVal(0.0);
    a2Val->setVal(0.0);
    phi2Val->setVal(0.0);
    a3Val->setVal(0.0);
    phi3Val->setVal(0.0);
    modelIndex=0;
  };

  void makeLGHiggs(){          
    a1Val->setVal(0.0);        // need to calculate the proper normalizations
    phi1Val->setVal(0.0);
    a2Val->setVal(1.0);
    phi2Val->setVal(0.0);
    a3Val->setVal(0.0);
    phi3Val->setVal(0.0);
    modelIndex=2;
  };


  void makePSHiggs(){
    a1Val->setVal(0.0);
    phi1Val->setVal(0.0);
    a2Val->setVal(0.0);
    phi2Val->setVal(0.0);
    a3Val->setVal(1.0);
    phi3Val->setVal(0.0);
    modelIndex=1;
  };

  void makeCustom(double a1, double a2, double a3,
		  double phi1, double phi2, double phi3){
    a1Val->setVal(a1);
    phi1Val->setVal(phi1);
    a2Val->setVal(a2);
    phi2Val->setVal(phi2);
    a3Val->setVal(a3);
    phi3Val->setVal(phi3);
    modelIndex=-1;
  };

  void makeParamsConst(bool yesNo=true){
    if(yesNo){
      a1Val->setConstant(kTRUE);
      phi1Val->setConstant(kTRUE);
      a2Val->setConstant(kTRUE);
      phi2Val->setConstant(kTRUE);
      a3Val->setConstant(kTRUE);
      phi3Val->setConstant(kTRUE);
      gamZ->setConstant(kTRUE);
      mZ->setConstant(kTRUE);
      R1Val->setConstant(kTRUE);
      R2Val->setConstant(kTRUE);
    }else{
      a1Val->setConstant(kFALSE);
      phi1Val->setConstant(kFALSE);
      a2Val->setConstant(kFALSE);
      phi2Val->setConstant(kFALSE);
      a3Val->setConstant(kFALSE);
      phi3Val->setConstant(kFALSE);
      gamZ->setConstant(kFALSE);
      mZ->setConstant(kFALSE);
      R1Val->setConstant(kFALSE);
      R2Val->setConstant(kFALSE);
    }
  };

  double getVal(double mZZ){

    double Norm[80];
    
    if(modelIndex==0){  // SMHiggs


      Norm[0 ]= 4*3.1415*0.715995;
      Norm[1 ]= 4*3.1415*0.807688;
      Norm[2 ]= 4*3.1415*0.914359;
      Norm[3 ]= 4*3.1415*1.04167;
      Norm[4 ]= 4*3.1415*1.19928;
      Norm[5 ]= 4*3.1415*1.39749;
      Norm[6 ]= 4*3.1415*1.64484;
      Norm[7 ]= 4*3.1415*1.95036;
      Norm[8 ]= 4*3.1415*2.32449;
      Norm[9 ]= 4*3.1415*2.77916;
      Norm[10]= 4*3.1415*3.9858;
      Norm[11]= 4*3.1415*3.32788;
      Norm[12]= 4*3.1415*4.76983;
      Norm[13]= 4*3.1415*5.69869;
      Norm[14]= 4*3.1415*6.79307;
      Norm[15]= 4*3.1415*8.07577;
      Norm[16]= 4*3.1415*9.57179;
      Norm[17]= 4*3.1415*11.3085;
      Norm[18]= 4*3.1415*13.3159;
      Norm[19]= 4*3.1415*15.6266;
      Norm[20]= 4*3.1415*18.2761;
      Norm[21]= 4*3.1415*21.3032;
      Norm[22]= 4*3.1415*24.7498;
      Norm[23]= 4*3.1415*28.6616;
      Norm[24]= 4*3.1415*33.0881;
      Norm[25]= 4*3.1415*38.0828;
      Norm[26]= 4*3.1415*43.704;
      Norm[27]= 4*3.1415*50.0145;
      Norm[28]= 4*3.1415*57.0824;
      Norm[29]= 4*3.1415*64.9817;
      Norm[30]= 4*3.1415*73.7921;
      Norm[31]= 4*3.1415*83.6001;
      Norm[32]= 4*3.1415*94.4997;
      Norm[33]= 4*3.1415*106.592;
      Norm[34]= 4*3.1415*119.988;
      Norm[35]= 4*3.1415*134.806;
      Norm[36]= 4*3.1415*151.177;
      Norm[37]= 4*3.1415*169.24;
      Norm[38]= 4*3.1415*189.15;
      Norm[39]= 4*3.1415*211.073;
      Norm[40]= 4*3.1415*235.19;
      Norm[41]= 4*3.1415*261.701;
      Norm[42]= 4*3.1415*290.82;
      Norm[43]= 4*3.1415*322.785;
      Norm[44]= 4*3.1415*357.855;
      Norm[45]= 4*3.1415*396.316;
      Norm[46]= 4*3.1415*438.479;
      Norm[47]= 4*3.1415*484.69;
      Norm[48]= 4*3.1415*535.329;
      Norm[49]= 4*3.1415*590.819;
      Norm[50]= 4*3.1415*651.625;
      Norm[51]= 4*3.1415*718.268;
      Norm[52]= 4*3.1415*791.328;
      Norm[53]= 4*3.1415*871.453;
      Norm[54]= 4*3.1415*959.373;
      Norm[55]= 4*3.1415*1055.91;
      Norm[56]= 4*3.1415*1161.98;
      Norm[57]= 4*3.1415*1278.65;
      Norm[58]= 4*3.1415*1407.12;
      Norm[59]= 4*3.1415*1548.77;
      Norm[60]= 4*3.1415*1705.19;
      Norm[61]= 4*3.1415*1878.21;
      Norm[62]= 4*3.1415*2069.98;
      Norm[63]= 4*3.1415*2283.03;
      Norm[64]= 4*3.1415*2520.35;
      Norm[65]= 4*3.1415*2785.48;
      Norm[66]= 4*3.1415*3082.73;
      Norm[67]= 4*3.1415*3417.3;
      Norm[68]= 4*3.1415*3795.62;
      Norm[69]= 4*3.1415*4225.66;
      Norm[70]= 4*3.1415*4717.54;
      Norm[71]= 4*3.1415*5284.21;
      Norm[72]= 4*3.1415*5942.62;
      Norm[73]= 4*3.1415*6715.39;
      Norm[74]= 4*3.1415*7633.45;
      Norm[75]= 4*3.1415*8740.24;
      Norm[76]= 4*3.1415*10098.8;
      Norm[77]= 4*3.1415*11803.7;
      Norm[78]= 4*3.1415*14002.8;
      Norm[79]= 4*3.1415*16935.5;

    }else if(modelIndex==1){  //PS Higgs
      Norm[0]  =4*3.1415*0.0114024;
      Norm[1]  =4*3.1415*0.0127468;
      Norm[2]  =4*3.1415*0.0142621;
      Norm[3]  =4*3.1415*0.0159798;
      Norm[4]  =4*3.1415*0.0179465;
      Norm[5]  =4*3.1415*0.0202286;
      Norm[6]  =4*3.1415*0.0229098;
      Norm[7]  =4*3.1415*0.0260894;
      Norm[8]  =4*3.1415*0.0298842;
      Norm[9]  =4*3.1415*0.0344311;
      Norm[10] =4*3.1415*0.0398892;
      Norm[11] =4*3.1415*0.0464432;
      Norm[12] =4*3.1415*0.0543054;
      Norm[13] =4*3.1415*0.0637191;
      Norm[14] =4*3.1415*0.0749613;
      Norm[15] =4*3.1415*0.0883459;
      Norm[16] =4*3.1415*0.104227 ;
      Norm[17] =4*3.1415*0.123003 ;
      Norm[18] =4*3.1415*0.145117 ;
      Norm[19] =4*3.1415*0.171068 ;
      Norm[20] =4*3.1415*0.201405 ;
      Norm[21] =4*3.1415*0.23674  ;
      Norm[22] =4*3.1415*0.277747 ;
      Norm[23] =4*3.1415*0.325173 ;
      Norm[24] =4*3.1415*0.379834 ;
      Norm[25] =4*3.1415*0.44263  ;
      Norm[26] =4*3.1415*0.514547 ;
      Norm[27] =4*3.1415*0.596663 ;
      Norm[28] =4*3.1415*0.690156 ;
      Norm[29] =4*3.1415*0.796314 ;
      Norm[30] =4*3.1415*0.916538 ;
      Norm[31] =4*3.1415*1.05236  ;
      Norm[32] =4*3.1415*1.20544  ;
      Norm[33] =4*3.1415*1.3776	  ;
      Norm[34] =4*3.1415*1.5708	  ;    
      Norm[35] =4*3.1415*1.78719  ;
      Norm[36] =4*3.1415*2.0291	  ;
      Norm[37] =4*3.1415*2.29908  ;
      Norm[38] =4*3.1415*2.59987  ;
      Norm[39] =4*3.1415*2.93449  ;
      Norm[40] =4*3.1415*3.3062	  ;
      Norm[41] =4*3.1415*3.71856  ;
      Norm[42] =4*3.1415*4.17546  ;
      Norm[43] =4*3.1415*4.68112  ;
      Norm[44] =4*3.1415*5.24017  ;
      Norm[45] =4*3.1415*5.85764  ;	    
      Norm[46] =4*3.1415*6.53907  ;
      Norm[47] =4*3.1415*7.2905	  ;
      Norm[48] =4*3.1415*8.11857  ;
      Norm[49] =4*3.1415*9.03056  ;     
      Norm[50] =4*3.1415*10.0345  ;
      Norm[51] =4*3.1415*11.1392  ;
      Norm[52] =4*3.1415*12.3545  ;
      Norm[53] =4*3.1415*13.6911  ;
      Norm[54] =4*3.1415*15.1611  ;
      Norm[55] =4*3.1415*16.7778  ;
      Norm[56] =4*3.1415*18.5561  ;
      Norm[57] =4*3.1415*20.5126  ;
      Norm[58] =4*3.1415*22.6661  ;
      Norm[59] =4*3.1415*25.0377  ;
      Norm[60] =4*3.1415*27.6511  ;
      Norm[61] =4*3.1415*30.5335  ;
      Norm[62] =4*3.1415*33.7157  ;
      Norm[63] =4*3.1415*37.2331  ;
      Norm[64] =4*3.1415*41.1266  ;
      Norm[65] =4*3.1415*45.4436  ;
      Norm[66] =4*3.1415*50.2392  ;
      Norm[67] =4*3.1415*55.5787  ;
      Norm[68] =4*3.1415*61.5392  ;
      Norm[69] =4*3.1415*68.2131  ;
      Norm[70] =4*3.1415*75.7123  ;
      Norm[71] =4*3.1415*84.1736  ;
      Norm[72] =4*3.1415*93.7669  ;   
      Norm[73] =4*3.1415*104.706  ;
      Norm[74] =4*3.1415*117.267  ;
      Norm[75] =4*3.1415*131.808  ;
      Norm[76] =4*3.1415*148.814  ;
      Norm[77] =4*3.1415*168.954  ;
      Norm[78] =4*3.1415*193.182  ;
      Norm[79] =4*3.1415*222.918  ;
    }else if(modelIndex==2){           //longitudinal scalar
      Norm[0 ]= 4*3.1415*0.0376864;
      Norm[1 ]= 4*3.1415*0.0417462;
      Norm[2 ]= 4*3.1415*0.046229;
      Norm[3 ]= 4*3.1415*0.0511833;
      Norm[4 ]= 4*3.1415*0.0566669;
      Norm[5 ]= 4*3.1415*0.0627517;
      Norm[6 ]= 4*3.1415*0.0695288;
      Norm[7 ]= 4*3.1415*0.0771124;
      Norm[8 ]= 4*3.1415*0.0856444;
      Norm[9 ]= 4*3.1415*0.0952989;
      Norm[10]= 4*3.1415*0.106287;
      Norm[11]= 4*3.1415*0.118861;
      Norm[12]= 4*3.1415*0.133322;
      Norm[13]= 4*3.1415*0.150023;
      Norm[14]= 4*3.1415*0.169379;
      Norm[15]= 4*3.1415*0.191869;
      Norm[16]= 4*3.1415*0.218043;
      Norm[17]= 4*3.1415*0.248535;
      Norm[18]= 4*3.1415*0.284062;
      Norm[19]= 4*3.1415*0.325438;
      Norm[20]= 4*3.1415*0.373579;
      Norm[21]= 4*3.1415*0.429513;
      Norm[22]= 4*3.1415*0.49439;
      Norm[23]= 4*3.1415*0.569486;
      Norm[24]= 4*3.1415*0.65622;
      Norm[25]= 4*3.1415*0.756159;
      Norm[26]= 4*3.1415*0.871034;
      Norm[27]= 4*3.1415*1.00275;
      Norm[28]= 4*3.1415*1.15338;
      Norm[29]= 4*3.1415*1.32522;
      Norm[30]= 4*3.1415*1.52076;
      Norm[31]= 4*3.1415*1.74273;
      Norm[32]= 4*3.1415*1.99407;
      Norm[33]= 4*3.1415*2.27802;
      Norm[34]= 4*3.1415*2.59807;
      Norm[35]= 4*3.1415*2.958	;
      Norm[36]= 4*3.1415*3.36191;
      Norm[37]= 4*3.1415*3.81423;
      Norm[38]= 4*3.1415*4.31976;
      Norm[39]= 4*3.1415*4.88367;
      Norm[40]= 4*3.1415*5.51153;
      Norm[41]= 4*3.1415*6.20935;
      Norm[42]= 4*3.1415*6.98363;
      Norm[43]= 4*3.1415*7.84135;
      Norm[44]= 4*3.1415*8.79001;
      Norm[45]= 4*3.1415*9.83771;
      Norm[46]= 4*3.1415*10.9932;
      Norm[47]= 4*3.1415*12.2658;
      Norm[48]= 4*3.1415*13.6656;
      Norm[49]= 4*3.1415*15.2035;
      Norm[50]= 4*3.1415*16.8912;
      Norm[51]= 4*3.1415*18.7413;
      Norm[52]= 4*3.1415*20.7675;
      Norm[53]= 4*3.1415*22.9842;
      Norm[54]= 4*3.1415*25.4075;
      Norm[55]= 4*3.1415*28.0543;
      Norm[56]= 4*3.1415*30.9432;
      Norm[57]= 4*3.1415*34.0942;
      Norm[58]= 4*3.1415*37.5289;
      Norm[59]= 4*3.1415*41.2709;
      Norm[60]= 4*3.1415*45.3459;
      Norm[61]= 4*3.1415*49.7816;
      Norm[62]= 4*3.1415*54.6086;
      Norm[63]= 4*3.1415*59.8603;
      Norm[64]= 4*3.1415*65.5731;
      Norm[65]= 4*3.1415*71.7874;
      Norm[66]= 4*3.1415*78.5476;
      Norm[67]= 4*3.1415*85.9032;
      Norm[68]= 4*3.1415*93.909	;
      Norm[69]= 4*3.1415*102.627;
      Norm[70]= 4*3.1415*112.125;
      Norm[71]= 4*3.1415*122.484;
      Norm[72]= 4*3.1415*133.791;
      Norm[73]= 4*3.1415*146.151;
      Norm[74]= 4*3.1415*159.684;
      Norm[75]= 4*3.1415*174.533;
      Norm[76]= 4*3.1415*190.868;
      Norm[77]= 4*3.1415*208.896;
      Norm[78]= 4*3.1415*228.878;
      Norm[79]= 4*3.1415*251.149;
    }else if(modelIndex==-1){
      return PDF->getVal()/1.0;
    }else{

      cout << "ERROR: modelIndex not set... Did you make sure to pick a specific model??" << endl;
      return PDF->getVal()/1.0;

    }

    if((int)floor(mZZ-100)>79){

      cout << "Normalization is not available for this value of mZZ: " << (int)floor(mZZ-100) << endl;
      return PDF->getVal()/Norm[79];

    }if((int)floor(mZZ-100)<0){

      cout << "Normalization is not available for this value of mZZ: " << (int)floor(mZZ-100) << endl;
      return PDF->getVal()/Norm[0];

    }

    return PDF->getVal()/Norm[(int)floor(mZZ-100)];

  };

  double getValIntegrOutAngles(RooRealVar* m1,RooRealVar* m2,RooRealVar* h1,RooRealVar* h2,RooRealVar* Phi,RooRealVar* mZZ){
    RooAbsPdf* PDFIntegratedOut =PDF->createProjection(RooArgSet(*h1,*h2,*Phi));
    double norm = PDFIntegratedOut->getNorm(RooArgSet(*m1, *m2, *mZZ));
    cout<<"norm "<<norm<<endl;
    double val = PDFIntegratedOut->getVal()/norm;
    cout<<"val "<<val<<endl;
   return val;
  }


};
#endif
