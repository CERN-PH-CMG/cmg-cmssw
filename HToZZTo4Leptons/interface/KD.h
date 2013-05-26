#include "ZZMatrixElement/MEMCalculators/interface/MEMCalculators.h"




class KD 
{
 public:
  KD() {
    mem_ = new MEMs(8);



  }


  ~KD() {
    if(mem_ !=0) delete mem_;
  }




  void  computeAll(TLorentzVector Z1_lept1, int Z1_lept1Id,
		   TLorentzVector Z1_lept2, int Z1_lept2Id,
		   TLorentzVector Z2_lept1, int Z2_lept1Id,
		   TLorentzVector Z2_lept2, int Z2_lept2Id) {

    std::vector<TLorentzVector> ps;
    ps.push_back(Z1_lept1);
    ps.push_back(Z1_lept2);
    ps.push_back(Z2_lept1);
    ps.push_back(Z2_lept2);

    if ( Z2_lept1Id == Z2_lept2Id)
      Z2_lept2Id=-Z2_lept2Id;


    std::vector<int> id;
    id.push_back(Z1_lept1Id);
    id.push_back(Z1_lept2Id);
    id.push_back(Z2_lept1Id);
    id.push_back(Z2_lept2Id);

    mem_->computeMEs(ps,id);
  }


  float getKD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_qqZZ;
    mem_->computeKD(kSMHiggs, kJHUGen, kqqZZ, kMCFM, &MEMs::probRatio, KD, ME_ggHiggs, ME_qqZZ);
    return KD;
  }

  float getGG0KD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_gg0Minus;
    mem_->computeKD(kSMHiggs, kJHUGen, k0minus, kJHUGen, &MEMs::probRatio, KD, ME_ggHiggs, ME_gg0Minus);
    return KD;
  }

  float getGG0HKD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_gg0hPlus;
    mem_->computeKD(kSMHiggs, kJHUGen, k0hplus, kJHUGen, &MEMs::probRatio, KD, ME_ggHiggs, ME_gg0hPlus);
    return KD;
  }
  float getQQ1MinusKD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_qq1Minus;
    mem_->computeKD(kSMHiggs, kJHUGen, k1minus, kJHUGen, &MEMs::probRatio, KD, ME_ggHiggs, ME_qq1Minus);
    return KD;
  }

  float getQQ1PlusKD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_qq2Plus;
    mem_->computeKD(kSMHiggs, kJHUGen, k1plus, kJHUGen, &MEMs::probRatio, KD, ME_ggHiggs, ME_qq2Plus);
    return KD;
  }
  float getGG2PlusKD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_gg2Plus;
    mem_->computeKD(kSMHiggs, kJHUGen, k2mplus_gg, kJHUGen, &MEMs::probRatio, KD, ME_ggHiggs, ME_gg2Plus);
    return KD;
  }
  float getQQ2PlusKD() {
    using namespace MEMNames;
    double KD,ME_ggHiggs,ME_qq2Plus;
    mem_->computeKD(kSMHiggs, kJHUGen, k2mplus_qqbar, kJHUGen, &MEMs::probRatio, KD, ME_ggHiggs, ME_qq2Plus);
    return KD;
  }


  float getInterferenceWeight() {
    return mem_->getMELAWeight();
  }




 private:

  MEMs * mem_;



};
