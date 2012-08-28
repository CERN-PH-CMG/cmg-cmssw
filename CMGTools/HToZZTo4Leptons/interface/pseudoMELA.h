#ifndef pseudoMela_h
#define pseudoMela_h


#include "CMGTools/HToZZTo4Leptons/src/AngularPdfFactory.h"
//#include "RooRealVar.h"

//template <typedef U>
class pseudoMELA{

public:

  RooRealVar* z1mass_rrv;
  RooRealVar* z2mass_rrv;
  RooRealVar* costheta1_rrv;
  RooRealVar* costheta2_rrv;
  RooRealVar* phi_rrv;
  RooRealVar* costhetastar_rrv;
  RooRealVar* phistar1_rrv;
  RooRealVar* mzz_rrv;

  AngularPdfFactory *SMHiggs;
  AngularPdfFactory *PSHiggs;

  pseudoMELA();

  ~pseudoMELA(){};

  void checkZorder(float& z1mass, float& z2mass,
				   float& costhetastar, float& costheta1,
				   float& costheta2, float& phi,
				   float& phistar1);
    
  float eval(float zzmass, float z1mass, 
			      float z2mass, float costhetstar, 
			      float costheta1, float costheta2, 
			      float phi, float phistar1);
    

};

#endif
