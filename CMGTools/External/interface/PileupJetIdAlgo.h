//--------------------------------------------------------------------------------------------------
// $Id $
//
// PileupJetIdAlgo
//
// Author: P. Musella, P. Harris
//--------------------------------------------------------------------------------------------------

#ifndef PileupJetIdAlgo_h
#define PileupJetIdAlgo_h

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include "PileupJetIdentifier.h"

// ----------------------------------------------------------------------------------------------------
class PileupJetIdAlgo {
public:
	enum version_t { USER=-1, PHILv0=0 };
	
	PileupJetIdAlgo(int version=PHILv0, const std::string & tmvaWeight="", const std::string & tmvaMethod="", 
			Float_t impactParTkThreshod_=1., const std::vector<std::string> & tmvaVariables= std::vector<std::string>());
	PileupJetIdAlgo(const edm::ParameterSet & ps); 
	~PileupJetIdAlgo(); 
	
	PileupJetIdentifier computeIdVariables(const reco::Jet * jet, 
					       float jec, const reco::Vertex *, bool calculateMva=false);
protected:
	void setup(); 
	void bookReader();	
	const PileupJetIdentifier::variables_list_t getVariables() const { return internalId_.variables_; };
	
	PileupJetIdentifier internalId_;

	TMVA::Reader * reader_;
	std::string    tmvaWeights_, tmvaMethod_; 
	std::vector<std::string>  tmvaVariables_;
	std::map<std::string,std::string>  tmvaNames_;
	
	Int_t version_;
	Float_t impactParTkThreshod_;

};

#endif
