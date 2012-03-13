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
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#define DECLARE_VARIABLE(NAME,TYPE)		\
	private: \
	TYPE NAME ## _;			\
	public: \
	const TYPE & NAME() const { return NAME ## _; }

// ----------------------------------------------------------------------------------------------------
class PileupJetIdentifier {
public:
	PileupJetIdentifier(const std::string & tmvaWeight="", const std::string & tmvaMethod="", 
			    Float_t impactParTkThreshod_=1.);
	// PileupJetIdentifier(edm::ParameterSet & ps); 
	PileupJetIdentifier(const PileupJetIdentifier &p);
	~PileupJetIdentifier(); 
	
	void computeIdVariables(const reco::Jet * jet, float jec, const reco::Vertex *, bool calculateMva=false);
	
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

	DECLARE_VARIABLE(dRLeadCent ,Float_t);  
	DECLARE_VARIABLE(dRLead2nd  ,Float_t);  
	DECLARE_VARIABLE(dRMean     ,Float_t);  
	DECLARE_VARIABLE(dRMeanNeut ,Float_t);  
	DECLARE_VARIABLE(dRMeanEm   ,Float_t);  
	DECLARE_VARIABLE(dRMeanCh   ,Float_t);  

	DECLARE_VARIABLE(ptD,Float_t);

	DECLARE_VARIABLE(leadFrac    ,Float_t);  
	DECLARE_VARIABLE(secondFrac  ,Float_t);  
	DECLARE_VARIABLE(thirdFrac   ,Float_t);  
	DECLARE_VARIABLE(fourthFrac  ,Float_t);  

	DECLARE_VARIABLE(leadChFrac    ,Float_t);  
	DECLARE_VARIABLE(secondChFrac  ,Float_t);  
	DECLARE_VARIABLE(thirdChFrac   ,Float_t);  
	DECLARE_VARIABLE(fourthChFrac  ,Float_t);  

	DECLARE_VARIABLE(leadNeutFrac    ,Float_t);  
	DECLARE_VARIABLE(secondNeutFrac  ,Float_t);  
	DECLARE_VARIABLE(thirdNeutFrac   ,Float_t);  
	DECLARE_VARIABLE(fourthNeutFrac  ,Float_t);  

	DECLARE_VARIABLE(leadEmFrac    ,Float_t);  
	DECLARE_VARIABLE(secondEmFrac  ,Float_t);  
	DECLARE_VARIABLE(thirdEmFrac   ,Float_t);  
	DECLARE_VARIABLE(fourthEmFrac  ,Float_t);  

	DECLARE_VARIABLE(jetW  ,Float_t);  
	DECLARE_VARIABLE(etaW  ,Float_t);  
	DECLARE_VARIABLE(phiW  ,Float_t);  

	DECLARE_VARIABLE(majW  ,Float_t);  
	DECLARE_VARIABLE(minW  ,Float_t);  

	DECLARE_VARIABLE(frac01    ,Float_t);  
	DECLARE_VARIABLE(frac02    ,Float_t);  
	DECLARE_VARIABLE(frac03    ,Float_t);  
	DECLARE_VARIABLE(frac04    ,Float_t);  
	DECLARE_VARIABLE(frac05   ,Float_t);  
	
	DECLARE_VARIABLE(chFrac01    ,Float_t);  
	DECLARE_VARIABLE(chFrac02    ,Float_t);  
	DECLARE_VARIABLE(chFrac03    ,Float_t);  
	DECLARE_VARIABLE(chFrac04    ,Float_t);  
	DECLARE_VARIABLE(chFrac05   ,Float_t);  

	DECLARE_VARIABLE(neutFrac01    ,Float_t);  
	DECLARE_VARIABLE(neutFrac02    ,Float_t);  
	DECLARE_VARIABLE(neutFrac03    ,Float_t);  
	DECLARE_VARIABLE(neutFrac04    ,Float_t);  
	DECLARE_VARIABLE(neutFrac05   ,Float_t);  

	DECLARE_VARIABLE(emFrac01    ,Float_t);  
	DECLARE_VARIABLE(emFrac02    ,Float_t);  
	DECLARE_VARIABLE(emFrac03    ,Float_t);  
	DECLARE_VARIABLE(emFrac04    ,Float_t);  
	DECLARE_VARIABLE(emFrac05   ,Float_t);  
	
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
