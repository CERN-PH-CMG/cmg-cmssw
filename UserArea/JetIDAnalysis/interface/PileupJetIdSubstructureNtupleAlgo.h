#ifndef PileupJetIdSubstructureNtupleAlgo_h
#define PileupJetIdSubstructureNtupleAlgo_h

#include "CMGTools/External/interface/PileupJetIdAlgoSubstructure.h"

class Tree;

// --------------------------------------------------------------------------------
class PileupJetIdSubstructureNtupleAlgo : public PileupJetIdAlgoSubstructure
{
public:
	PileupJetIdSubstructureNtupleAlgo(int version=0, const std::string & tmvaWeight="", const std::string & tmvaMethod="", 
					  Float_t impactParTkThreshod_=1., const std::vector<std::string> & tmvaVariables= std::vector<std::string>());
	PileupJetIdSubstructureNtupleAlgo(const edm::ParameterSet & ps); 
	
	void bookBranches(TTree *,const std::string & prefix="", const std::string & postfix="");
	
	void fillJet(const PileupJetIdentifierSubstructure & id, int ijet, int ievent);
	void fillJet(const reco::Jet * jet, float jec, const reco::Vertex *, const reco::VertexCollection & vtxs, int ijet, int ievent, bool runMva=false);

	void setIJetIEvent(int ijet, int ievent);
private:
	int ijet_, ievent_;

};

#endif
