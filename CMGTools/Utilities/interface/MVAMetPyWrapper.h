#ifndef __CMGTools_Utilities_MVAMetPyWrapper__
#define __CMGTools_Utilities_MVAMetPyWrapper__

#include "CMGTools/Utilities/interface/MVAMet.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

class MVAMetPyWrapper : public MVAMet {
  
 public:
  MVAMetPyWrapper() : MVAMet() {}

  void addVisObject(const LorentzVector& v) { visObjects_.push_back(v);}
  void add_iJets(const LorentzVector& v , const float mva, const float neutFrac ) {
        struct MetUtilities::JetInfo JetInfo_;
        JetInfo_.p4 = v;
        JetInfo_.mva = mva;
        JetInfo_.neutFrac = neutFrac;
        iJets_.push_back(JetInfo_);
        }
  void GetMet( 
                const reco::PFMET *iPFMet,
						    const reco::PFMET *iTKMet,
						    const reco::PFMET *iNoPUMet,
						    const reco::PFMET *iPUMet,
						    const reco::PFMET *iPUCMet,
						    const LorentzVector *iLeadJet,
						    const LorentzVector *i2ndJet,
						    int iNJetsGt30,
						    int iNJetsGt1,
						    int iNGoodVtx,
						    bool iPrintDebug
						    ) {
                
    MVAMET_temp = MVAMet::GetMet(visObjects_,
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
			  iJets_,
			  iPrintDebug
			  );
        
  }
  
  MVAMet::LorentzVector GetMet_first(){
    return MVAMET_temp.first;
  }
  cmg::METSignificance GetMet_second(){
    return MVAMET_temp.second;
  }

 private:
  std::vector<LorentzVector>  visObjects_;
  std::vector<MetUtilities::JetInfo>  iJets_;
  std::pair<LorentzVector, cmg::METSignificance > MVAMET_temp;
};

#endif
