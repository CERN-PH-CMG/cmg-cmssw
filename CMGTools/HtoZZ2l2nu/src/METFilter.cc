#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace met{

  //
  LorentzVectorCollection filter(LorentzVector &pfMET,  std::vector<const pat::Jet *> &assocJets,  std::vector<const pat::Jet *> &puJets, bool isRaw)
  {
    LorentzVectorCollection themets(3,pfMET);

    //iterate over the jets
    LorentzVector rawJetsSum(0,0,0,0), corJetsSum(0,0,0,0);
    for(std::vector<const pat::Jet *>::iterator jit=assocJets.begin();jit != assocJets.end(); jit++)
      {
	corJetsSum += (*jit)->p4();
	rawJetsSum += (*jit)->correctedP4("Uncorrected");
      }
    
    //add the pu jets
    LorentzVector corPUJetsSum(0,0,0,0),rawPUJetsSum(0,0,0,0);
    for(std::vector<const pat::Jet *>::iterator jit=puJets.begin();jit != puJets.end(); jit++)
      {
	corPUJetsSum += (*jit)->p4();
	rawPUJetsSum += (*jit)->correctedP4("Uncorrected");
      }
    
    if(isRaw)
      {
    	//compute the mets (and fix final value)
	themets[1]=-(corJetsSum-rawJetsSum)-(corPUJetsSum-rawPUJetsSum)+themets[0];
	themets[2]=-(corJetsSum-rawJetsSum)+rawPUJetsSum+themets[0];
      }
    else
      {
	//remove the non associated jets
	themets[2]=corPUJetsSum+themets[0];
      }
       

    for(size_t imet=0; imet<3; imet++) themets[imet] = LorentzVector(themets[imet].px(),themets[imet].py(),0,themets[imet].pt());
    return themets;
  }
}
