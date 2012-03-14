#ifndef PileupJetIdNtupleAlgo_h
#define PileupJetIdNtupleAlgo_h

#include "CMGTools/External/interface/PileupJetIdAlgo.h"

class Tree;

// --------------------------------------------------------------------------------
class PileupJetIdNtupleAlgo : public PileupJetIdAlgo
{
public:
	PileupJetIdNtupleAlgo(int version=PHILv0, const std::string & tmvaWeight="", const std::string & tmvaMethod="", 
			Float_t impactParTkThreshod_=1., const std::vector<std::string> & tmvaVariables= std::vector<std::string>());
	PileupJetIdNtupleAlgo(const edm::ParameterSet & ps); 

	void bookBranches(TTree *,const std::string & prefix="", const std::string & postfix="");
	
	void fillJet(const PileupJetIdentifier & id, int ijet, int ievent);
	void fillJet(const reco::Jet * jet, float jec, const reco::Vertex *, int ijet, int ievent, bool runMva=false);

private:
	int ijet_, ievent_;

};

#endif
