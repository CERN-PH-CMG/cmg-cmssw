#include "../interface/PileupJetIdentifier.h"

#include "DataFormats/PatCandidates/interface/Jet.h" 
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

// ------------------------------------------------------------------------------------------
const Float_t large_val = std::numeric_limits<Float_t>::max();

// ------------------------------------------------------------------------------------------
PileupJetIdentifier::PileupJetIdentifier(const std::string & tmvaWeights, 
					 const std::string & tmvaMethod, 
					 Float_t impactParTkThreshod
	) 
{
	impactParTkThreshod_ = impactParTkThreshod;
	tmvaWeights_         = tmvaWeights;
	tmvaMethod_          = tmvaMethod;
	
	reader_              = 0;

	initVariables();
}

// ------------------------------------------------------------------------------------------
PileupJetIdentifier::PileupJetIdentifier(const PileupJetIdentifier &p)
{
	impactParTkThreshod_ = p.impactParTkThreshod_;
	tmvaWeights_         = p.tmvaWeights_;
	tmvaMethod_          = p.tmvaMethod_;
	tmvaVariables_       = p.tmvaVariables_;
	
	reader_              = 0;

	initVariables();
	for(variables_list_t::const_iterator it=variables_.begin(); 
	    it!=variables_.end(); ++it ) {
		*it->second.first = *p.variables_.find( it->first )->second.first;
	}
}

// ------------------------------------------------------------------------------------------
#define INIT_VARIABLE(NAME,TMVANAME,VAL)	\
	NAME ## _ = VAL; \
	variables_[ # NAME   ] = std::make_pair(& NAME ## _, VAL); \
	tmvaNames_[ TMVANAME ] = # NAME ;			 \

// ------------------------------------------------------------------------------------------
void PileupJetIdentifier::initVariables()
{
  	INIT_VARIABLE(mva        , "", -100.);
	INIT_VARIABLE(jetPt      , "jspt_1", 0.);
	INIT_VARIABLE(jetEta     , "jseta_1", large_val);
	INIT_VARIABLE(jetPhi     , "jsphi_1", large_val);
	INIT_VARIABLE(jetM       , "jm_1", 0.);

	INIT_VARIABLE(nCharged   , "", 0.);
	INIT_VARIABLE(nNeutrals  , "", 0.);
	
	INIT_VARIABLE(chgEMfrac  , "", 0.);
	INIT_VARIABLE(neuEMfrac  , "", 0.);
	INIT_VARIABLE(chgHadrfrac, "", 0.);
	INIT_VARIABLE(neuHadrfrac, "", 0.);
	
	INIT_VARIABLE(d0         , "jd0_1"    , large_val);   
	INIT_VARIABLE(dZ         , "jdZ_1"    , large_val);  
	INIT_VARIABLE(nParticles , "npart_1"  , 0.);  
	
	INIT_VARIABLE(leadPt     , "lpt_1"    , 0.);  
	INIT_VARIABLE(leadEta    , "leta_1"   , large_val);  
	INIT_VARIABLE(leadPhi    , "lphi_1"   , large_val);  
	INIT_VARIABLE(secondPt   , "spt_1"    , 0.);  
	INIT_VARIABLE(secondEta  , "seta_1"   , large_val);  
	INIT_VARIABLE(secondPhi  , "sphi_1"   , large_val);  
	INIT_VARIABLE(leadNeutPt , "lnept_1"    , 0.);  
	INIT_VARIABLE(leadNeutEta, "lneeta_1"   , large_val);  
	INIT_VARIABLE(leadNeutPhi, "lnephi_1"   , large_val);  
	INIT_VARIABLE(leadEmPt   , "lempt_1"  , 0.);  
	INIT_VARIABLE(leadEmEta  , "lemeta_1" , large_val);  
	INIT_VARIABLE(leadEmPhi  , "lemphi_1" , large_val);  
	INIT_VARIABLE(leadChPt   , "lchpt_1"  , 0.);  
	INIT_VARIABLE(leadChEta  , "lcheta_1" , large_val);  
	INIT_VARIABLE(leadChPhi  , "lchphi_1" , large_val);  
	INIT_VARIABLE(leadFrac   , "lLfr_1"   , 0.);  
	
	INIT_VARIABLE(dRLeadCent , "drlc_1"   , 0.);  
	INIT_VARIABLE(dRLead2nd  , "drls_1"   , 0.);  
	INIT_VARIABLE(dRMean     , "drm_1"    , 0.);  
	INIT_VARIABLE(dRMeanNeut , "drmne_1"  , 0.);  
	INIT_VARIABLE(dRMeanEm   , "drem_1"   , 0.);  
	INIT_VARIABLE(dRMeanCh   , "drch_1"   , 0.);  
	
	INIT_VARIABLE(ptD        , "", 0.);
	
	if( tmvaVariables_.empty() ) {
		tmvaVariables_.push_back( "jspt_1"  );
		tmvaVariables_.push_back( "jseta_1" );
		tmvaVariables_.push_back( "jsphi_1" );
		tmvaVariables_.push_back( "jd0_1"   );
		tmvaVariables_.push_back( "jdZ_1"   );
		tmvaVariables_.push_back( "jm_1"    );
		tmvaVariables_.push_back( "npart_1" );
		tmvaVariables_.push_back( "lpt_1"   );
		tmvaVariables_.push_back( "leta_1"  );
		tmvaVariables_.push_back( "lphi_1"  );
		tmvaVariables_.push_back( "spt_1"   );
		tmvaVariables_.push_back( "seta_1"  );
		tmvaVariables_.push_back( "sphi_1"  );
		tmvaVariables_.push_back( "lnept_1" );
		tmvaVariables_.push_back( "lneeta_1");
		tmvaVariables_.push_back( "lnephi_1");
		tmvaVariables_.push_back( "lempt_1" );
		tmvaVariables_.push_back( "lemeta_1");
		tmvaVariables_.push_back( "lemphi_1");
		tmvaVariables_.push_back( "lchpt_1" );
		// tmvaVariables_.push_back( "lcheta_1"); FIXME missing in weights file
		tmvaVariables_.push_back( "lchphi_1");
		tmvaVariables_.push_back( "lLfr_1"  );
		tmvaVariables_.push_back( "drlc_1"  );
		tmvaVariables_.push_back( "drls_1"  );
		tmvaVariables_.push_back( "drm_1"   );
		tmvaVariables_.push_back( "drmne_1" );
		tmvaVariables_.push_back( "drem_1"  );
		tmvaVariables_.push_back( "drch_1"  );
	}
}

#undef INIT_VARIABLE


// ------------------------------------------------------------------------------------------
PileupJetIdentifier::~PileupJetIdentifier() 
{
	if( reader_ ) {
		delete reader_;
	}
}

// ------------------------------------------------------------------------------------------
std::string PileupJetIdentifier::dumpVariables() const
{
	std::stringstream out;
	for(variables_list_t::const_iterator it=variables_.begin(); 
	    it!=variables_.end(); ++it ) {
		out << std::setw(15) << it->first << std::setw(3) << "=" 
		    << std::setw(5) << *it->second.first 
		    << " (" << std::setw(5) << it->second.second << ")" << std::endl;
	}
	return out.str();
}

// ------------------------------------------------------------------------------------------
void PileupJetIdentifier::resetVariables()
{
	for(variables_list_t::iterator it=variables_.begin(); 
	    it!=variables_.end(); ++it ) {
		*it->second.first = it->second.second;
	}
}

// ------------------------------------------------------------------------------------------
void setPtEtaPhi(const reco::Candidate & p, float & pt, float & eta, float &phi )
{
	pt  = p.pt();
	eta = p.eta();
	phi = p.phi();
}


// ------------------------------------------------------------------------------------------
void PileupJetIdentifier::bookReader()
{
	reader_ = new TMVA::Reader("!Color:!Silent");
	assert( ! tmvaMethod_.empty() && !  tmvaWeights_.empty() );
	for(std::vector<std::string>::iterator it=tmvaVariables_.begin(); it!=tmvaVariables_.end(); ++it) {
		reader_->AddVariable( *it, variables_[ tmvaNames_[*it] ].first );
	}
	reader_->BookMVA(tmvaMethod_.c_str(), tmvaWeights_.c_str());
}

// ------------------------------------------------------------------------------------------
void PileupJetIdentifier::computeIdVariables(const reco::Jet * jet, const reco::Vertex * vtx, bool calculateMva) 
{
	static int printWarning = 10; 
	typedef std::vector <reco::PFCandidatePtr> constituents_type;
	typedef std::vector <reco::PFCandidatePtr>::iterator constituents_iterator;

	// initialize all variables to 0
	resetVariables();
	
	// loop over constituents, accumulate sums and find leading candidates
	const pat::Jet * patjet = dynamic_cast<const pat::Jet *>(jet);
	const reco::PFJet * pfjet = dynamic_cast<const reco::PFJet *>(jet);
	assert( patjet != 0 || pfjet != 0 );

	constituents_type constituents = pfjet ? pfjet->getPFConstituents() : patjet->getPFConstituents();
	reco::PFCandidatePtr lLead, lSecond, lLeadNeut, lLeadEm, lLeadCh;
	reco::TrackRef impactTrack;
	float jetPt = jet->pt(); // FIXME: corrected pt?
	
	setPtEtaPhi(*jet,jetPt_,jetEta_,jetPhi_);
	jetM_ = jet->mass(); 
	
	for(constituents_iterator it=constituents.begin(); it!=constituents.end(); ++it) {
		reco::PFCandidatePtr & icand = *it;
		float candPt = icand->pt();
		if ( candPt <= 0. ) {
			// if( printWarning-- > 0) { 
			std::cerr << "WARNING: candidate with 0 pt" << std::endl; 
			//}
			continue;
		}
		float candDr = reco::deltaR(**it,*jet);
		float candPtDr = candPt * candDr;
		
		// all particles
		if( lLead.isNull() || candPt > lLead->pt() ) {
			lSecond = lLead;
			lLead = icand; 
		} else if( (lSecond.isNull() || candPt > lSecond->pt()) && (candPt < lLead->pt()) ) {
			lSecond = lLead;
		}
		dRMean_     += candPtDr;
		// neutrals
		if( icand->particleId() == reco::PFCandidate::h0 ) {
			if (lLeadNeut.isNull() || candPt > lLeadNeut->pt()) { lLeadNeut = icand; }
			dRMeanNeut_ += candPtDr;
		}
		// EM candidated
		if( icand->particleId() == reco::PFCandidate::gamma ) {
			if(lLeadEm.isNull() || candPt > lLeadEm->pt())  { lLeadEm = icand; }
			dRMeanEm_ += candPtDr;
		}
		// Charged hadrons
		if( icand->particleId() == reco::PFCandidate::h ) {
			if (lLeadCh.isNull() || candPt > lLeadCh->pt()) { lLeadCh = icand; }
			dRMeanCh_  += candPtDr;
		}
		if( impactTrack.isNull() && icand->trackRef().isNonnull() && candPt > impactParTkThreshod_ ) {
			impactTrack = icand->trackRef();
		}
		
		ptD_ += candPt*candPt;
	}
	// Finalize all variables
	nCharged_  = pfjet ? pfjet->chargedMultiplicity() : patjet->chargedMultiplicity();
	nNeutrals_ = pfjet ? pfjet->neutralMultiplicity() : patjet->neutralMultiplicity();

	chgEMfrac_   = ( pfjet ? pfjet->chargedEmEnergy()     : patjet->chargedEmEnergy()      ) /jet->energy();
	neuEMfrac_   = ( pfjet ? pfjet->neutralEmEnergy()     : patjet->neutralEmEnergy()      ) /jet->energy();
	chgHadrfrac_ = ( pfjet ? pfjet->chargedHadronEnergy() : patjet->chargedHadronEnergy()  ) /jet->energy();
	neuHadrfrac_ = ( pfjet ? pfjet->neutralHadronEnergy() : patjet->neutralHadronEnergy()  ) /jet->energy();
	
	// FIXME: check definition of impact parameters 
	if( impactTrack.isNonnull() && impactTrack.isAvailable() ) {
		d0_ = -impactTrack->dxy(vtx->position());
		dZ_ = impactTrack->dz(vtx->position());
	} else {
		if(printWarning-- > 0) { std::cerr << "WARNING : did not find any valid track reference attached to the jet " << std::endl; }
		//// d0_ = 99999.;
		//// dZ_ = 99999.;
	}
	nParticles_ = constituents.size(); 

	assert( lLead.isNonnull() );

	if ( lLead.isNonnull() )     { setPtEtaPhi(*lLead,leadPt_,leadEta_,leadPhi_);                 }
	if ( lSecond.isNonnull() )   { setPtEtaPhi(*lSecond,secondPt_,secondEta_,secondPhi_);	      }
	if ( lLeadNeut.isNonnull() ) { setPtEtaPhi(*lLeadNeut,leadNeutPt_,leadNeutEta_,leadNeutPhi_); }
	if ( lLeadEm.isNonnull() )   { setPtEtaPhi(*lLeadEm,leadEmPt_,leadEmEta_,leadEmPhi_);	      }
	if ( lLeadCh.isNonnull() )   { setPtEtaPhi(*lLeadCh,leadChPt_,leadChEta_,leadChPhi_);         }
	leadFrac_ = leadPt_ / jetPt;
	
	dRLeadCent_ = reco::deltaR(*jet,*lLead);
	if( lSecond.isNonnull() ) { dRLead2nd_  = reco::deltaR(*jet,*lSecond); }
	dRMean_     /= jetPt;
	dRMeanNeut_ /= jetPt;
	dRMeanEm_   /= jetPt;
	dRMeanCh_   /= jetPt;
	
	ptD_ = sqrt( ptD_ / (jetPt*jetPt) );
	
	if( calculateMva ) {
		if( ! reader_ ) { bookReader(); }
		mva_ = reader_->EvaluateMVA( tmvaMethod_.c_str() );
	}
}

