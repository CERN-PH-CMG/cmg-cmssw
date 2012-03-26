#include "../interface/PileupJetIdNtupleAlgo.h"
#include "TTree.h"

// --------------------------------------------------------------------------------
PileupJetIdNtupleAlgo::PileupJetIdNtupleAlgo(int version, const std::string & tmvaWeight, const std::string & tmvaMethod, 
					     Float_t impactParTkThreshod, const std::vector<std::string> & tmvaVariables) : 
	PileupJetIdAlgo(version,tmvaWeight,tmvaMethod,impactParTkThreshod,tmvaVariables)
{
}

// --------------------------------------------------------------------------------
PileupJetIdNtupleAlgo::PileupJetIdNtupleAlgo(const edm::ParameterSet & ps) : 
	PileupJetIdAlgo(ps)
{
} 

// --------------------------------------------------------------------------------
void PileupJetIdNtupleAlgo::bookBranches(TTree * tree,const std::string & prefix, const std::string & postfix)
{
	variables_list_t::const_iterator ivar;
	for(ivar=getVariables().begin(); ivar!=getVariables().end(); ++ivar) {
		std::string vname = prefix+ivar->first+postfix;
		tree->Branch( vname.c_str(), ivar->second.first, (vname+"/F").c_str()  );
	}
	tree->Branch( (prefix+"ijet"+postfix).c_str(),    &ijet_,   (prefix+"ijet/I"+postfix).c_str()   );
	tree->Branch( (prefix+"ievent"+postfix).c_str(), &ievent_,  (prefix+"ievent/I"+postfix).c_str() );
}

// --------------------------------------------------------------------------------
void PileupJetIdNtupleAlgo::fillJet(const PileupJetIdentifier & id, int ijet, int ievent)
{
	internalId_ = id;
	ijet_   = ijet;
	ievent_ = ievent;
}

// --------------------------------------------------------------------------------
void PileupJetIdNtupleAlgo::fillJet(const reco::Jet * jet, float jec, const reco::Vertex * vtx, const reco::VertexCollection & vtxs,  
				    int ijet, int ievent, bool runMva)
{
	computeIdVariables(jet,jec,vtx,vtxs,runMva);
	ijet_   = ijet;
	ievent_ = ievent;
}

// --------------------------------------------------------------------------------
void PileupJetIdNtupleAlgo::setIJetIEvent(int ijet, int ievent)
{
	ijet_   = ijet;
	ievent_ = ievent;
}
