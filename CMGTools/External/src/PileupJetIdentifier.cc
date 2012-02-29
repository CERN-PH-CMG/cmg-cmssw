#include "../interface/PileupJetIdentifier.h"

#include "DataFormats/PatCandidates/interface/Jet.h" 
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#define INIT_VARIABLE(NAME,VAL)  \
	NAME ## _ = VAL; \
	variables_[ # NAME ] = & NAME ## _;

// ------------------------------------------------------------------------------------------
PileupJetIdentifier::PileupJetIdentifier() 
{
	impactParTkThreshod_ = 1.;
	
	INIT_VARIABLE(nCharged   ,0.);
	INIT_VARIABLE(nNeutrals,0.);

	INIT_VARIABLE(chgEMfrac  ,0.);
	INIT_VARIABLE(neuEMfrac  ,0.);
	INIT_VARIABLE(chgHadrfrac,0.);
	INIT_VARIABLE(neuHadrfrac,0.);
	
	INIT_VARIABLE(d0         ,0.);   
	INIT_VARIABLE(dZ         ,0.);  
	INIT_VARIABLE(nParticles ,0.);  

	INIT_VARIABLE(leadPt     ,0.);  
	INIT_VARIABLE(leadEta    ,0.);  
	INIT_VARIABLE(leadPhi    ,0.);  
	INIT_VARIABLE(secondPt   ,0.);  
	INIT_VARIABLE(secondEta  ,0.);  
	INIT_VARIABLE(secondPhi  ,0.);  
	INIT_VARIABLE(leadNeutPt ,0.);  
	INIT_VARIABLE(leadNeutEta,0.);  
	INIT_VARIABLE(leadNeutPhi,0.);  
	INIT_VARIABLE(leadEmPt   ,0.);  
	INIT_VARIABLE(leadEmEta  ,0.);  
	INIT_VARIABLE(leadEmPhi  ,0.);  
	INIT_VARIABLE(leadChPt   ,0.);  
	INIT_VARIABLE(leadChEta  ,0.);  
	INIT_VARIABLE(leadChPhi  ,0.);  
	INIT_VARIABLE(leadFrac   ,0.);  

	INIT_VARIABLE(dRLeadCent ,0.);  
	INIT_VARIABLE(dRLead2nd  ,0.);  
	INIT_VARIABLE(dRMean     ,0.);  
	INIT_VARIABLE(dRMeanNeut ,0.);  
	INIT_VARIABLE(dRMeanEm   ,0.);  
	INIT_VARIABLE(dRMeanCh   ,0.);  
	
	INIT_VARIABLE(ptD,0.);
}

#undef INIT_VARIABLE


// ------------------------------------------------------------------------------------------
PileupJetIdentifier::~PileupJetIdentifier() 
{
}

// ------------------------------------------------------------------------------------------
std::string PileupJetIdentifier::dumpVariables() const
{
	std::stringstream out;
	for(std::map<std::string, Float_t * >::const_iterator it=variables_.begin(); 
	    it!=variables_.end(); ++it ) {
		out <<  std::setw(15) << it->first << std::setw(3) << "=" << std::setw(5) << *it->second << std::endl;
	}
	return out.str();
}

// ------------------------------------------------------------------------------------------
void PileupJetIdentifier::resetVariables()
{
	for(std::map<std::string, Float_t * >::iterator it=variables_.begin(); 
	    it!=variables_.end(); ++it ) {
		*it->second = 0.;
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
void PileupJetIdentifier::computeIdVariablesFromPat(const pat::Jet * jet, const reco::Vertex * vtx) 
{
	computeIdVariables((reco::Jet *)jet, vtx);
}

// ------------------------------------------------------------------------------------------
void PileupJetIdentifier::computeIdVariables(const reco::Jet * jet, const reco::Vertex * vtx) 
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
	
	for(constituents_iterator it=constituents.begin(); it!=constituents.end(); ++it) {
		reco::PFCandidatePtr & icand = *it;
		float candDr = reco::deltaR(**it,*jet);
		float candPt = icand->pt();
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
	} else if(printWarning) {
		std::cerr << "WARNING : did not find any valid track reference attached to the jet " << std::endl;
		d0_ = 99999.;
		dZ_ = 99999.;
		--printWarning;
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
}

