#ifndef __CMGTools_Utilities_MVAMetPyWrapper__
#define __CMGTools_Utilities_MVAMetPyWrapper__

#include "CMGTools/Utilities/interface/MVAMet.h"

class MVAMetPyWrapper : public MVAMet {
  
 public:
  MVAMetPyWrapper() : MVAMet() {}
  void addVisObject(const LorentzVector& v) { visObjects_.push_back(v);}
  std::pair<MVAMet::LorentzVector,TMatrixD> GetMet( const reco::PFMET *iPFMet,
						    const reco::PFMET *iTKMet,
						    const reco::PFMET *iNoPUMet,
						    const reco::PFMET *iPUMet,
						    const reco::PFMET *iPUCMet,
						    const LorentzVector *iLeadJet,
						    const LorentzVector *i2ndJet,
						    int iNJetsGt30,
						    int iNJetsGt1,
						    int iNGoodVtx,
						    std::vector<MetUtilities::JetInfo>  &iJets,
						    bool iPrintDebug
						    
						    ) {
    return MVAMet::GetMet(visObjects_,
			  iPFMet,
			  iTKMet,
			  iNoPUMet,
			  iPUMet,
			  iPUCMet,
			  iLeadJet,
			  i2ndJet,
			  iNJetsGt30,
			  iNJetsGt1,
			  iNGoodVtx,
			  iJets,
			  iPrintDebug
			  );
  }


 private:
  std::vector<LorentzVector>  visObjects_;
};

#endif
