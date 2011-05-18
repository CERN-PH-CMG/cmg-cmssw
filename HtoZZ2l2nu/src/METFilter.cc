#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace met{

  //
  LorentzVectorCollection filter(const reco::PFMET &pfMET,  std::vector<const pat::Jet *> &assocJets,  std::vector<const pat::Jet *> &puJets)
  {
    LorentzVectorCollection themets(3,LorentzVector(pfMET.px(),pfMET.py(),0,pfMET.pt()));

    //iterate over the jets
    LorentzVector rawJetsSum(0,0,0,0), corJetsSum(0,0,0,0);
    for(std::vector<const pat::Jet *>::iterator jit=assocJets.begin();jit != assocJets.end(); jit++)
      {
	corJetsSum += (*jit)->p4();
	rawJetsSum += (*jit)->correctedP4("Uncorrected");
      }

    //add the pu jets
    LorentzVector corPUJetsSum(0,0,0,0);
    for(std::vector<const pat::Jet *>::iterator jit=puJets.begin();jit != puJets.end(); jit++)
      {
	corJetsSum += (*jit)->p4();
	corPUJetsSum += (*jit)->p4();
	rawJetsSum += (*jit)->correctedP4("Uncorrected");
      }
    
    //compute the mets (and fix final value)
    themets[1]=rawJetsSum-corJetsSum+themets[0];
    themets[2]=corPUJetsSum+themets[1];
    for(size_t imet=0; imet<3; imet++) themets[imet] = LorentzVector(themets[imet].px(),themets[imet].py(),0,themets[imet].pt());

    return themets;
  }
}
