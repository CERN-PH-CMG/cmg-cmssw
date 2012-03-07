//--------------------------------------------------------------------------------------------------
// $Id $
//
// PileupJetIdentifier
//
// Author: P. Musella, P. Harris
//--------------------------------------------------------------------------------------------------

#ifndef PileupJetIdentifier_h
#define PileupJetIdentifier_h

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

namespace pat { class Jet; }

#define DECLARE_VARIABLE(NAME,TYPE)		\
	private: \
	TYPE NAME ## _;			\
	public: \
	const TYPE & NAME() const { return NAME ## _; }


class PileupJetIdentifier {
public:
	PileupJetIdentifier(const PileupJetIdentifier &p);
	PileupJetIdentifier(const std::string & tmvaWeight="", const std::string & tmvaMethod="", Float_t impactParTkThreshod_=1.);
	~PileupJetIdentifier(); 
	
	void computeIdVariables(const reco::Jet * jet, const reco::Vertex *, bool calculateMva=false);
	
	DECLARE_VARIABLE(mva      ,Float_t);
	
	DECLARE_VARIABLE(jetEta   ,Float_t);
	DECLARE_VARIABLE(jetPt    ,Float_t);
	DECLARE_VARIABLE(jetPhi   ,Float_t);
	DECLARE_VARIABLE(jetM     ,Float_t);

	DECLARE_VARIABLE(nCharged   ,Float_t);
	DECLARE_VARIABLE(nNeutrals,Float_t);

	DECLARE_VARIABLE(chgEMfrac  ,Float_t);
	DECLARE_VARIABLE(neuEMfrac  ,Float_t);
	DECLARE_VARIABLE(chgHadrfrac,Float_t);
	DECLARE_VARIABLE(neuHadrfrac,Float_t);
	
	DECLARE_VARIABLE(d0         ,Float_t);   
	DECLARE_VARIABLE(dZ         ,Float_t);  
	DECLARE_VARIABLE(nParticles ,Float_t);  
	DECLARE_VARIABLE(leadPt     ,Float_t);  
	DECLARE_VARIABLE(leadEta    ,Float_t);  
	DECLARE_VARIABLE(leadPhi    ,Float_t);  
	DECLARE_VARIABLE(secondPt   ,Float_t);  
	DECLARE_VARIABLE(secondEta  ,Float_t);  
	DECLARE_VARIABLE(secondPhi  ,Float_t);  
	DECLARE_VARIABLE(leadNeutPt ,Float_t);  
	DECLARE_VARIABLE(leadNeutEta,Float_t);  
	DECLARE_VARIABLE(leadNeutPhi,Float_t);  
	DECLARE_VARIABLE(leadEmPt   ,Float_t);  
	DECLARE_VARIABLE(leadEmEta  ,Float_t);  
	DECLARE_VARIABLE(leadEmPhi  ,Float_t);  
	DECLARE_VARIABLE(leadChPt   ,Float_t);  
	DECLARE_VARIABLE(leadChEta  ,Float_t);  
	DECLARE_VARIABLE(leadChPhi  ,Float_t);  
	DECLARE_VARIABLE(leadFrac   ,Float_t);  

	DECLARE_VARIABLE(dRLeadCent ,Float_t);  
	DECLARE_VARIABLE(dRLead2nd  ,Float_t);  
	DECLARE_VARIABLE(dRMean     ,Float_t);  
	DECLARE_VARIABLE(dRMeanNeut ,Float_t);  
	DECLARE_VARIABLE(dRMeanEm   ,Float_t);  
	DECLARE_VARIABLE(dRMeanCh   ,Float_t);  

	DECLARE_VARIABLE(ptD,Float_t);
	
	std::string dumpVariables() const;

protected:
	typedef std::map<std::string,std::pair<Float_t *,Float_t> > variables_list_t;

private:
	TMVA::Reader * reader_;
	std::string    tmvaWeights_, tmvaMethod_; 
	std::vector<std::string>  tmvaVariables_;
	std::map<std::string,std::string>  tmvaNames_;
	
	void bookReader();
	
	void resetVariables();
	void initVariables();
	
	variables_list_t variables_;
	
	Float_t impactParTkThreshod_;
};

#undef DECLARE_VARIABLE

#endif
