
#include "ZZMatrixElement/MELA/interface/Mela.h"
#include "ZZMatrixElement/MELA/interface/PseudoMELA.h"
#include "ZZMatrixElement/MELA/interface/SpinTwoMinimalMELA.h"


class KD 
{
 public:
  KD() {
    mela_ = new Mela();
    pseudomela_ = new PseudoMELA();
    spintwomela_ = new SpinTwoMinimalMELA();


  }


  ~KD() {
    if(mela_ !=0) delete mela_;
    if(pseudomela_ !=0) delete pseudomela_;
    if(spintwomela_ !=0) delete spintwomela_;
  }

  float computeMELA(TLorentzVector Z1_lept1, int Z1_lept1Id,
		   TLorentzVector Z1_lept2, int Z1_lept2Id,
		   TLorentzVector Z2_lept1, int Z2_lept1Id,
		   TLorentzVector Z2_lept2, int Z2_lept2Id,
		   bool withPt = false,
		    bool withY = false) {

    
    float melaKD,melaS,melaB;
    mela_->computeKD(Z1_lept1,Z1_lept1Id,Z1_lept2,Z1_lept2Id,Z2_lept1,Z2_lept1Id,Z2_lept2,Z2_lept2Id,
		     costhetastar_,costheta1_,costheta2_,phi_,phistar1_,melaKD,sig_,bkg_,withPt,withY);
    return melaKD;
  }



  float computePseudoMELA(TLorentzVector Z1_lept1, int Z1_lept1Id,
			  TLorentzVector Z1_lept2, int Z1_lept2Id,
			  TLorentzVector Z2_lept1, int Z2_lept1Id,
			  TLorentzVector Z2_lept2, int Z2_lept2Id) {
    
    float melaKD,melaS,melaB;
    pseudomela_->computeKD(Z1_lept1,Z1_lept1Id,Z1_lept2,Z1_lept2Id,Z2_lept1,Z2_lept1Id,Z2_lept2,Z2_lept2Id,melaKD,melaS,melaB);
    return melaKD;
  }



  float computeSpinTwoMELA(TLorentzVector Z1_lept1, int Z1_lept1Id,
			  TLorentzVector Z1_lept2, int Z1_lept2Id,
			  TLorentzVector Z2_lept1, int Z2_lept1Id,
			  TLorentzVector Z2_lept2, int Z2_lept2Id) {
    
    float melaKD,melaS,melaB;
    spintwomela_->computeKD(Z1_lept1,Z1_lept1Id,Z1_lept2,Z1_lept2Id,Z2_lept1,Z2_lept1Id,Z2_lept2,Z2_lept2Id,melaKD,melaS,melaB);
    return melaKD;
  }




  float costhetastar() {return costhetastar_;}
  float costheta1() {return costheta1_;}
  float costheta2() {return costheta2_;}
  float phi() {return phi_;}
  float phistar1() {return phistar1_;}
  float melaS() {return sig_;}
  float melaB() {return bkg_;}


 private:
  Mela * mela_;
  PseudoMELA * pseudomela_;
  SpinTwoMinimalMELA * spintwomela_;
  float costhetastar_;
  float costheta1_; 
  float costheta2_;
  float phi_;
  float phistar1_;
  float sig_;
  float bkg_;




};
