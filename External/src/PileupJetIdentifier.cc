#include "../interface/PileupJetIdentifier.h"

#include "DataFormats/PatCandidates/interface/Jet.h" 
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"

// ------------------------------------------------------------------------------------------
const Float_t large_val = std::numeric_limits<Float_t>::max();

/// // ------------------------------------------------------------------------------------------
/// PileupJetIdentifier::PileupJetIdentifier(edm::ParameterSet & ps) 
/// {
/// 	impactParTkThreshod_ = ps.getUntrackedParameter<double>("impactParTkThreshod",1.);
/// 	tmvaWeights_         = edm::FileInPath(ps.getUntrackedParameter<std::string>("tmvaWeights","CMGTools/External/data/mva_JetID.weights.xml")).fullPath();
/// 	tmvaMethod_          = ps.getUntrackedParameter<std::string>("tmvaMethod","JetID");
/// 	
/// 	reader_              = 0;
/// 
/// 	initVariables();
/// }

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
	
	INIT_VARIABLE(d0         , "jd0_1"    , -1000.);   
	INIT_VARIABLE(dZ         , "jdZ_1"    , -1000.);  
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

	INIT_VARIABLE(secondFrac  ,"" ,0.);  
	INIT_VARIABLE(thirdFrac   ,"" ,0.);  
	INIT_VARIABLE(fourthFrac  ,"" ,0.);  

	INIT_VARIABLE(leadChFrac    ,"" ,0.);  
	INIT_VARIABLE(secondChFrac  ,"" ,0.);  
	INIT_VARIABLE(thirdChFrac   ,"" ,0.);  
	INIT_VARIABLE(fourthChFrac  ,"" ,0.);  

	INIT_VARIABLE(leadNeutFrac    ,"" ,0.);  
	INIT_VARIABLE(secondNeutFrac  ,"" ,0.);  
	INIT_VARIABLE(thirdNeutFrac   ,"" ,0.);  
	INIT_VARIABLE(fourthNeutFrac  ,"" ,0.);  

	INIT_VARIABLE(leadEmFrac    ,"" ,0.);  
	INIT_VARIABLE(secondEmFrac  ,"" ,0.);  
	INIT_VARIABLE(thirdEmFrac   ,"" ,0.);  
	INIT_VARIABLE(fourthEmFrac  ,"" ,0.);  

	INIT_VARIABLE(jetW  ,"" ,1.);  
	INIT_VARIABLE(etaW  ,"" ,1.);  
	INIT_VARIABLE(phiW  ,"" ,1.);  

	INIT_VARIABLE(majW  ,"" ,1.);  
	INIT_VARIABLE(minW  ,"" ,1.);  

	INIT_VARIABLE(frac01    ,"" ,0.);  
	INIT_VARIABLE(frac02    ,"" ,0.);  
	INIT_VARIABLE(frac03    ,"" ,0.);  
	INIT_VARIABLE(frac04    ,"" ,0.);  
	INIT_VARIABLE(frac05   ,"" ,0.);  
	
	INIT_VARIABLE(chFrac01    ,"" ,0.);  
	INIT_VARIABLE(chFrac02    ,"" ,0.);  
	INIT_VARIABLE(chFrac03    ,"" ,0.);  
	INIT_VARIABLE(chFrac05   ,"" ,0.);  

	INIT_VARIABLE(chFrac01    ,"" ,0.);  
	INIT_VARIABLE(chFrac02    ,"" ,0.);  
	INIT_VARIABLE(chFrac03    ,"" ,0.);  
	INIT_VARIABLE(chFrac05   ,"" ,0.);  

	INIT_VARIABLE(neutFrac01    ,"" ,0.);  
	INIT_VARIABLE(neutFrac02    ,"" ,0.);  
	INIT_VARIABLE(neutFrac03    ,"" ,0.);  
	INIT_VARIABLE(neutFrac05   ,"" ,0.);  

	INIT_VARIABLE(emFrac01    ,"" ,0.);  
	INIT_VARIABLE(emFrac02    ,"" ,0.);  
	INIT_VARIABLE(emFrac03    ,"" ,0.);  
	INIT_VARIABLE(emFrac05   ,"" ,0.);  
	
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
void assign(const std::vector<float> & vec, float & a, float & b, float & c, float & d )
{
	size_t sz = vec.size();
	a = ( sz > 0 ? vec[0] : 0. );
	b = ( sz > 1 ? vec[1] : 0. );
	c = ( sz > 2 ? vec[2] : 0. );
	d = ( sz > 3 ? vec[3] : 0. );
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
void PileupJetIdentifier::computeIdVariables(const reco::Jet * jet, float jec, const reco::Vertex * vtx, bool calculateMva) 
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
	if( patjet != 0 && jec == 0. ) { // if this is a pat jet and no jec has been passed take the jec from the object
		jec = patjet->jecFactor(1); // FIXME double-check 
	}
	constituents_type constituents = pfjet ? pfjet->getPFConstituents() : patjet->getPFConstituents();
	
	reco::PFCandidatePtr lLead, lSecond, lLeadNeut, lLeadEm, lLeadCh, lTrail;
	std::vector<float> frac, fracCh, fracEm, fracNeut;
	float cones[] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
	size_t ncones = sizeof(cones)/sizeof(float);
	float * coneFracs[]     = { &frac01_, &frac02_, &frac03_, &frac04_, &frac05_ }; 
	float * coneEmFracs[]   = { &emFrac01_, &emFrac02_, &emFrac03_, &emFrac04_, &emFrac05_ }; 
	float * coneNeutFracs[] = { &neutFrac01_, &neutFrac02_, &neutFrac03_, &neutFrac04_, &neutFrac05_ }; 
	float * coneChFracs[]   = { &chFrac01_, &chFrac02_, &chFrac03_, &chFrac04_, &chFrac05_ }; 
	TMatrixDSym covMatrix(2); covMatrix = 0.;
	
	reco::TrackRef impactTrack;
	float jetPt = jet->pt() / jec; // use uncorrected pt for shape variables
	float sumPt = 0., sumPt2 = 0.;
	setPtEtaPhi(*jet,jetPt_,jetEta_,jetPhi_); // use corrected pt for jet kinematics
	jetM_ = jet->mass(); 
	
	for(constituents_iterator it=constituents.begin(); it!=constituents.end(); ++it) {
		reco::PFCandidatePtr & icand = *it;
		float candPt = icand->pt();
		float candPtFrac = candPt/jetPt;
		float candDr   = reco::deltaR(**it,*jet);
		float candDeta = fabs( (*it)->eta() - jet->eta() );
		float candDphi = reco::deltaPhi(**it,*jet);
		float candPtDr = candPt * candDr;
		size_t icone = std::lower_bound(&cones[0],&cones[ncones],candDr) - &cones[0];
		
		// all particles
		if( lLead.isNull() || candPt > lLead->pt() ) {
			lSecond = lLead;
			lLead = icand; 
		} else if( (lSecond.isNull() || candPt > lSecond->pt()) && (candPt < lLead->pt()) ) {
			lSecond = icand;
		}
		dRMean_     += candPtDr;
		frac.push_back(candPtFrac);
		if( icone < ncones ) { *coneFracs[icone] += candPt; }
		// neutrals
		if( icand->particleId() == reco::PFCandidate::h0 ) {
			if (lLeadNeut.isNull() || candPt > lLeadNeut->pt()) { lLeadNeut = icand; }
			dRMeanNeut_ += candPtDr;
			fracNeut.push_back(candPtFrac);
			if( icone < ncones ) { *coneNeutFracs[icone] += candPt; }
		}
		// EM candidated
		if( icand->particleId() == reco::PFCandidate::gamma ) {
			if(lLeadEm.isNull() || candPt > lLeadEm->pt())  { lLeadEm = icand; }
			dRMeanEm_ += candPtDr;
			fracEm.push_back(candPtFrac);
			if( icone < ncones ) { *coneEmFracs[icone] += candPt; }
		}
		// Charged hadrons
		if( icand->particleId() == reco::PFCandidate::h ) {
			if (lLeadCh.isNull() || candPt > lLeadCh->pt()) { lLeadCh = icand; }
			dRMeanCh_  += candPtDr;
			fracCh.push_back(candPtFrac);
			if( icone < ncones ) { *coneChFracs[icone] += candPt; }
		}
		//// if( impactTrack.isNull() && icand->trackRef().isNonnull() && candPt > impactParTkThreshod_ ) {
		//// 	impactTrack = icand->trackRef();
		//// }
		if( lTrail.isNull() || candPt < lTrail->pt() ) {
			lTrail = icand; 
		}
		
		covMatrix(0,0) += candPt*candPt*candDeta*candDeta;
		covMatrix(0,1) += candPt*candPt*candDeta*candDphi;
		covMatrix(1,1) += candPt*candPt*candDphi*candDphi;
		ptD_ += candPt*candPt;
		sumPt += candPt;
		sumPt2 += candPt*candPt;
	}
	assert( lLead.isNonnull() );
	
	if ( lSecond.isNull() )   { lSecond   = lTrail; }
	if ( lLeadNeut.isNull() ) { lLeadNeut = lTrail; }
	if ( lLeadEm.isNull() )   { lLeadEm   = lTrail; }
	if ( lLeadCh.isNull() )   { lLeadCh   = lTrail; }
	impactTrack = lLeadCh->trackRef();

	// Finalize all variables
	nCharged_  = pfjet ? pfjet->chargedMultiplicity() : patjet->chargedMultiplicity();
	nNeutrals_ = pfjet ? pfjet->neutralMultiplicity() : patjet->neutralMultiplicity();

	chgEMfrac_   = ( pfjet ? pfjet->chargedEmEnergy()     : patjet->chargedEmEnergy()      ) /jet->energy();
	neuEMfrac_   = ( pfjet ? pfjet->neutralEmEnergy()     : patjet->neutralEmEnergy()      ) /jet->energy();
	chgHadrfrac_ = ( pfjet ? pfjet->chargedHadronEnergy() : patjet->chargedHadronEnergy()  ) /jet->energy();
	neuHadrfrac_ = ( pfjet ? pfjet->neutralHadronEnergy() : patjet->neutralHadronEnergy()  ) /jet->energy();
	
	if( impactTrack.isNonnull() && impactTrack.isAvailable() ) {
		d0_ = fabs(impactTrack->dxy(vtx->position()));
		dZ_ = fabs(impactTrack->dz(vtx->position()));
	} else {
		if(printWarning-- > 0) { std::cerr << "WARNING : did not find any valid track reference attached to the jet " << std::endl; }
		//// d0_ = 99999.;
		//// dZ_ = 99999.;
	}
	nParticles_ = constituents.size(); 

	setPtEtaPhi(*lLead,leadPt_,leadEta_,leadPhi_);                 
	setPtEtaPhi(*lSecond,secondPt_,secondEta_,secondPhi_);	      
	setPtEtaPhi(*lLeadNeut,leadNeutPt_,leadNeutEta_,leadNeutPhi_); 
	setPtEtaPhi(*lLeadEm,leadEmPt_,leadEmEta_,leadEmPhi_);	      
	setPtEtaPhi(*lLeadCh,leadChPt_,leadChEta_,leadChPhi_);         

	std::sort(frac.begin(),frac.end(),std::greater<float>());
	std::sort(fracCh.begin(),fracCh.end(),std::greater<float>());
	std::sort(fracEm.begin(),fracEm.end(),std::greater<float>());
	std::sort(fracNeut.begin(),fracNeut.end(),std::greater<float>());
	assign(frac,    leadFrac_,    secondFrac_,    thirdFrac_,    fourthFrac_);
	assign(fracCh,  leadChFrac_,  secondChFrac_,  thirdChFrac_,  fourthChFrac_);
	assign(fracEm,  leadEmFrac_,  secondEmFrac_,  thirdEmFrac_,  fourthEmFrac_);
	assign(fracNeut,leadNeutFrac_,secondNeutFrac_,thirdNeutFrac_,fourthNeutFrac_);
	
	covMatrix(0,0) /= sumPt2;
	covMatrix(0,1) /= sumPt2;
	covMatrix(1,1) /= sumPt2;
	covMatrix(1,0)  = covMatrix(0,1);
	etaW_ = sqrt(covMatrix(0,0));
	phiW_ = sqrt(covMatrix(1,1));
	jetW_ = 0.5*(etaW_+phiW_);
	TVectorD eigVals(2); eigVals = TMatrixDSymEigen(covMatrix).GetEigenValues();
	majW_ = sqrt(fabs(eigVals(0)));
	minW_ = sqrt(fabs(eigVals(1)));
	if( majW_ < minW_ ) { std::swap(majW_,minW_); }
	
	dRLeadCent_ = reco::deltaR(*jet,*lLead);
	if( lSecond.isNonnull() ) { dRLead2nd_  = reco::deltaR(*jet,*lSecond); }
	dRMean_     /= jetPt;
	dRMeanNeut_ /= jetPt;
	dRMeanEm_   /= jetPt;
	dRMeanCh_   /= jetPt;

	for(size_t ic=0; ic<ncones; ++ic){
		*coneFracs[ic]     /= jetPt;
		*coneEmFracs[ic]   /= jetPt;
		*coneNeutFracs[ic] /= jetPt;
		*coneChFracs[ic]   /= jetPt;
	}
	
	ptD_ = sqrt( ptD_ ) / sumPt;
	
	if( calculateMva ) {
		if( ! reader_ ) { bookReader(); }
		mva_ = reader_->EvaluateMVA( tmvaMethod_.c_str() );
	}
}

