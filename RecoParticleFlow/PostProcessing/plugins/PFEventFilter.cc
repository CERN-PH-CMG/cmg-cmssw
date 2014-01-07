#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"

#include <sstream>

class PFEventFilter : public edm::EDFilter {
  
public:
  
  explicit PFEventFilter(const edm::ParameterSet & iConfig);
  ~PFEventFilter() {}
  
private:
  
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
  bool postMuonCleaning(reco::PFCandidateCollection *pfCandidates_, 
			const edm::Handle<reco::MuonCollection>& muonh,
			const reco::VertexCollection& primaryVertices, 
			double& metBefore, double& metAfter);
  
  const edm::InputTag pfCandSrc_, muonSrc_, vtxSrc_;
  const bool debug_;
  const bool taggingMode_;

  /// the collection of  cosmics cleaned muon candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfCleanedCandidates_;
  /// the collection of  cosmics cleaned muon candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfCosmicsMuonCleanedCandidates_;
  /// the collection of  tracker/global cleaned muon candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfCleanedTrackerAndGlobalMuonCandidates_;
  /// the collection of  fake cleaned muon candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfFakeMuonCleanedCandidates_;
  /// the collection of  punch-through cleaned muon candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfPunchThroughMuonCleanedCandidates_;
  /// the collection of  punch-through cleaned neutral hadron candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfPunchThroughHadronCleanedCandidates_;
  /// the collection of  added muon candidates
  std::auto_ptr< reco::PFCandidateCollection >    pfAddedMuonCandidates_;

};


PFEventFilter::PFEventFilter(const edm::ParameterSet & iConfig) : 
  pfCandSrc_ (  iConfig.getParameter<edm::InputTag>("PFCandSource") ),
  muonSrc_   ( iConfig.getParameter<edm::InputTag>("MuonSource") ),
  vtxSrc_   ( iConfig.getParameter<edm::InputTag>("VtxSource") ),
  debug_     ( iConfig.getParameter<bool>("Debug") ),
  taggingMode_ ( iConfig.getParameter<bool> ("taggingMode") ) {
  
  produces<reco::PFCandidateCollection>("AfterMuonCleaning");
  produces<reco::PFCandidateCollection>("CleanedCosmicsMuons");
  produces<reco::PFCandidateCollection>("CleanedTrackerAndGlobalMuons");
  produces<reco::PFCandidateCollection>("CleanedFakeMuons");
  produces<reco::PFCandidateCollection>("CleanedPunchThroughMuons");
  produces<reco::PFCandidateCollection>("CleanedPunchThroughNeutralHadrons");
  produces<reco::PFCandidateCollection>("AddedMuonsAndHadrons");
  produces<bool>("Result");

}


bool PFEventFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<reco::PFCandidateCollection> pfCands;
  iEvent.getByLabel(pfCandSrc_, pfCands);
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(muonSrc_, muons);
  edm::Handle<reco::VertexCollection> primaryVertices; 
  iEvent.getByLabel(vtxSrc_, primaryVertices);
  //   edm::Handle<edm::View<pat::Muon> > muons;
  //   iEvent.getByLabel(muonSrc_, muons);
  
  if(pfCleanedCandidates_.get() )
    pfCleanedCandidates_->clear();
  else 
    pfCleanedCandidates_.reset( new reco::PFCandidateCollection );

  if(pfCosmicsMuonCleanedCandidates_.get() )
    pfCosmicsMuonCleanedCandidates_->clear();
  else 
    pfCosmicsMuonCleanedCandidates_.reset( new reco::PFCandidateCollection );

  if(pfCleanedTrackerAndGlobalMuonCandidates_.get() )
    pfCleanedTrackerAndGlobalMuonCandidates_->clear();
  else 
    pfCleanedTrackerAndGlobalMuonCandidates_.reset( new reco::PFCandidateCollection );

  if(pfFakeMuonCleanedCandidates_.get() )
    pfFakeMuonCleanedCandidates_->clear();
  else 
    pfFakeMuonCleanedCandidates_.reset( new reco::PFCandidateCollection );

  if(pfPunchThroughMuonCleanedCandidates_.get() )
    pfPunchThroughMuonCleanedCandidates_->clear();
  else 
    pfPunchThroughMuonCleanedCandidates_.reset( new reco::PFCandidateCollection );

  if(pfPunchThroughHadronCleanedCandidates_.get() )
    pfPunchThroughHadronCleanedCandidates_->clear();
  else 
    pfPunchThroughHadronCleanedCandidates_.reset( new reco::PFCandidateCollection );

  if(pfAddedMuonCandidates_.get() )
    pfAddedMuonCandidates_->clear();
  else 
    pfAddedMuonCandidates_.reset( new reco::PFCandidateCollection );

  reco::PFCandidateCollection pfCandidates_ = *pfCands;
  reco::VertexCollection vertices = *primaryVertices;
  double metBefore = 0.;
  double metAfter = 0.;
  bool fixed = postMuonCleaning(&pfCandidates_,muons,vertices,metBefore,metAfter);

  for(unsigned i=0; i<pfCandidates_.size(); i++) {
    const reco::PFCandidate& pfc = pfCandidates_[i];
    pfCleanedCandidates_->push_back(pfc);
  }

  iEvent.put(pfCosmicsMuonCleanedCandidates_,"CleanedCosmicsMuons");
  iEvent.put(pfCleanedTrackerAndGlobalMuonCandidates_,"CleanedTrackerAndGlobalMuons");
  iEvent.put(pfFakeMuonCleanedCandidates_,"CleanedFakeMuons");
  iEvent.put(pfPunchThroughMuonCleanedCandidates_,"CleanedPunchThroughMuons");
  iEvent.put(pfPunchThroughHadronCleanedCandidates_,"CleanedPunchThroughNeutralHadrons");
  iEvent.put(pfAddedMuonCandidates_,"AddedMuonsAndHadrons");
  iEvent.put(pfCleanedCandidates_,"AfterMuonCleaning");
    
  bool result = !fixed;  // the event has not been modified

  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  if(taggingMode_) return true;
  else return result; // true if no modification of the event is needed.
}

bool
PFEventFilter::postMuonCleaning( reco::PFCandidateCollection *pfCandidates_, 
				 const edm::Handle<reco::MuonCollection>& muonh,
				 const reco::VertexCollection& primaryVertices, 
				 double& metBefore, double& metAfter ) {
  

  bool fixed = false;

  // Estimate SumEt from pile-up
  double sumetPU = 0.;
  for (unsigned short i=1 ;i<primaryVertices.size();++i ) {
    if ( !primaryVertices[i].isValid() || primaryVertices[i].isFake() ) continue; 
    primaryVertices[i];
    for ( reco::Vertex::trackRef_iterator itr = primaryVertices[i].tracks_begin();
	  itr <  primaryVertices[i].tracks_end(); ++itr ) { 
      sumetPU += (*itr)->pt();
    }
  }
  sumetPU /= 0.65; // To estimate the neutral contribution from PU
  // std::cout << "Evaluation of sumetPU from " << primaryVertices.size() << " vertices : " << sumetPU << std::endl;

  //Compute met and met significance (met/sqrt(SumEt))
  double metX = 0.;
  double metY = 0.;
  double sumet = 0;
  double metXCosmics = 0.;
  double metYCosmics = 0.;
  double sumetCosmics = 0.;
  reco::Vertex       primaryVertex_;

  std::map<double,unsigned int> pfMuons;
  std::map<double,unsigned int> pfCosmics;
  typedef std::multimap<double, unsigned int>::iterator IE;

  // Find primary vertex
  for (unsigned short i=0 ;i<primaryVertices.size();++i) {
    if(primaryVertices[i].isValid()&&(!primaryVertices[i].isFake())) {
      primaryVertex_ = primaryVertices[i];
      break;
    }
  }

  // Find muons
  for(unsigned i=0; i<pfCandidates_->size(); i++) {
    const reco::PFCandidate& pfc = (*pfCandidates_)[i];
    double origin = 
      (pfc.vx()-primaryVertex_.x())*(pfc.vx()-primaryVertex_.x()) + 
      (pfc.vy()-primaryVertex_.y())*(pfc.vy()-primaryVertex_.y());

    // Compute MET
    metX += pfc.px();
    metY += pfc.py();
    sumet += pfc.pt();

    // Remember the muons and order them by decreasing energy
    // Remember the cosmic muons too 
    if ( pfc.particleId() == reco::PFCandidate::mu ) { 
      pfMuons.insert(std::pair<double,unsigned int>(-pfc.pt(),i));
      if ( origin > 1.0 ) { 
	pfCosmics.insert(std::pair<double,unsigned int>(-pfc.pt(),i));
	metXCosmics += pfc.px();
	metYCosmics += pfc.py();
	sumetCosmics += pfc.pt();
      }
    }
      
  }

  double met2 = metX*metX+metY*metY;
  double met2Cosmics = (metX-metXCosmics)*(metX-metXCosmics)+(metY-metYCosmics)*(metY-metYCosmics);
  metBefore = std::sqrt(met2);

  // Clean cosmic muons
  if ( sumetCosmics > (sumet-sumetPU)/10. && met2Cosmics < met2 ) { 
    // std::cout << "MEX,MEY,MET Before (Cosmics)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
    for ( IE imu = pfCosmics.begin(); imu != pfCosmics.end(); ++imu ) { 
      const reco::PFCandidate& pfc = (*pfCandidates_)[imu->second];
      pfCosmicsMuonCleanedCandidates_->push_back(pfc);
      //std::cout << "Muon cleaned (cosmic). pt/d0 = " << pfc.pt() << " " 
      //		<< std::sqrt(pfc.vx()*pfc.vx() + pfc.vy()*pfc.vy()) << std::endl; 
      metX -= pfc.px();
      metY -= pfc.py();
      sumet -= pfc.pt();
      (*pfCandidates_)[imu->second].rescaleMomentum(0.);
    }
    met2 = metX*metX+metY*metY;
    fixed = true;
    //std::cout << "MEX,MEY,MET After  (Cosmics)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
  }

  // Clean mismeasured muons
  for ( IE imu = pfMuons.begin(); imu != pfMuons.end(); ++imu ) { 
    const reco::PFCandidate& pfc = (*pfCandidates_)[imu->second];

    // Don't care about low momentum muons
    if ( pfc.pt() < 20. ) continue;

    double metXNO = metX - pfc.px();
    double metYNO = metY - pfc.py();
    double met2NO = metXNO*metXNO + metYNO*metYNO; 
    double sumetNO = sumet - pfc.pt();
    double factor = std::max(2.,2000./sumetNO);

    reco::MuonRef muonRef = pfc.muonRef();
    reco::TrackRef combinedMu = muonRef->combinedMuon();
    reco::TrackRef trackerMu = muonRef->track();
    reco::TrackRef standAloneMu = muonRef->standAloneMuon();

    if ( !combinedMu || !trackerMu ) { 
      if ( sumetNO-sumetPU > 500. && met2NO < met2/4.) { 
	pfFakeMuonCleanedCandidates_->push_back(pfc);
	//std::cout << "Muon cleaned (NO-bad) " << sumetNO << std::endl;
	//	std::cout << "sumet NO " << sumetNO << std::endl;
	(*pfCandidates_)[imu->second].rescaleMomentum(0.);
	//std::cout << "MEX,MEY,MET            " << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	metX = metXNO;
	metY = metYNO;
	met2 = met2NO;
	sumet = sumetNO;
	//std::cout << "Muon cleaned (NO/TK) ! " << std::endl;
	//std::cout << "MEX,MEY,MET Now (NO/TK)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	fixed = true;
      }
    } else { 
      double metXTK = metXNO + trackerMu->px(); 
      double metYTK = metYNO + trackerMu->py();
      double met2TK = metXTK*metXTK + metYTK*metYTK;
 
      double metXGL = metXNO + combinedMu->px();
      double metYGL = metYNO + combinedMu->py();
      double met2GL = metXGL*metXGL + metYGL*metYGL; 

      if ( ( sumetNO-sumetPU > 250. && met2TK < met2/4. && met2TK < met2GL ) || 
	   ( met2TK < met2/2. && trackerMu->pt() < combinedMu->pt()/4. && met2TK < met2GL ) )  { 
	pfCleanedTrackerAndGlobalMuonCandidates_->push_back(pfc);
	math::XYZTLorentzVectorD p4(trackerMu->px(),trackerMu->py(),trackerMu->pz(),
				    std::sqrt(trackerMu->p()*trackerMu->p()+0.1057*0.1057));
	//std::cout << "Muon cleaned (TK) ! " << met2TK/met2 << " " << trackerMu->pt() / combinedMu->pt() << std::endl;
	(*pfCandidates_)[imu->second].setP4(p4);
	//std::cout << "MEX,MEY,MET Before (TK)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	metX = metXTK;
	metY = metYTK;
	met2 = met2TK;
	fixed = true;
	//std::cout << "MEX,MEY,MET Now    (TK)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
      } 
      
      else if ( ( sumetNO-sumetPU > 250. && met2GL < met2/4. && met2GL < met2TK ) ||  
		( met2GL < met2/2. && combinedMu->pt() < trackerMu->pt()/4.&& met2GL < met2TK ) )  { 
	pfCleanedTrackerAndGlobalMuonCandidates_->push_back(pfc);
	math::XYZTLorentzVectorD p4(combinedMu->px(),combinedMu->py(),combinedMu->pz(),
				    std::sqrt(combinedMu->p()*combinedMu->p()+0.1057*0.1057));
	//std::cout << "Muon cleaned (GL) ! " << met2GL/met2 << " " << combinedMu->pt()/trackerMu->pt() <<  std::endl;
	(*pfCandidates_)[imu->second].setP4(p4);
	//std::cout << "MEX,MEY,MET before (GL)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	metX = metXGL;
	metY = metYGL;
	met2 = met2GL;
	fixed = true;
	//std::cout << "MEX,MEY,MET Now    (GL)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
      }


      // Fake candidates at large pseudo-rapidity
      bool fake = 
	fabs ( pfc.eta() ) > 2.15 && 
	met2NO < met2/25. && 
	(met2GL < met2TK/2. || met2TK < met2GL/2.) &&
	standAloneMu->pt() < combinedMu->pt()/10. &&
	standAloneMu->pt() < trackerMu->pt()/10.; 

      // Fake or punch-through candidates
      bool punchthrough1 =  
	( sumetNO-sumetPU > 250. && met2NO < met2/4. && (met2GL < met2TK/factor || met2TK < met2GL/factor) );

      // Now look for punch through candidates (i.e., muon with large neutral hadron behind)
      bool punchthrough2 =  
	pfc.p() > 100. &&  pfc.rawHcalEnergy() > 100. && 
	pfc.rawEcalEnergy()+pfc.rawHcalEnergy() > pfc.p()/3. &&
	!PFMuonAlgo::isIsolatedMuon(muonRef) && met2NO < met2/4.;

      if ( punchthrough1 ||  punchthrough2 || fake ) { 
	
	// Find the block of the muon
	const reco::PFCandidate::ElementsInBlocks& eleInBlocks = pfc.elementsInBlocks();
	if ( !eleInBlocks.size() ) { 
	  std::ostringstream err;
	  err<<"A muon has no associated elements in block. Cannot proceed with postMuonCleaning()";
	  edm::LogError("PFAlgo")<<err.str()<<std::endl;
	}
	else {
	  reco::PFBlockRef blockRefMuon = eleInBlocks[0].first;
	  unsigned indexMuon = eleInBlocks[0].second;
	  for ( unsigned iele = 1; iele < eleInBlocks.size(); ++iele ) { 
	    indexMuon = eleInBlocks[iele].second;
	    break;
	  }
	
	  // Check if the muon gave rise to a neutral hadron
	  double iHad = 1E9;
	  bool hadron = false;
	  for ( unsigned i = imu->second+1; i < pfCandidates_->size(); ++i ) { 
	    const reco::PFCandidate& pfcn = (*pfCandidates_)[i];
	    const reco::PFCandidate::ElementsInBlocks& ele = pfcn.elementsInBlocks();
	    if ( !ele.size() ) { 
	      std::ostringstream err2;
	      err2<<"A pfCandidate has no associated elements in block. Cannot proceed with postMuonCleaning()";
	      edm::LogError("PFAlgo")<<err2.str()<<std::endl;
	      continue;
	    }
	    reco::PFBlockRef blockRefHadron = ele[0].first;
	    unsigned indexHadron = ele[0].second;
	    // We are out of the block -> exit the loop
	    if ( blockRefHadron.key() != blockRefMuon.key() ) break;
	    // Check that this particle is a neutral hadron
	    if ( indexHadron == indexMuon && 
		 pfcn.particleId() == reco::PFCandidate::h0 ) {
	      iHad = i;
	      hadron = true;
	    }
	    if ( hadron ) break;
	  }

	  if ( hadron ) { 
	    const reco::PFCandidate& pfch = (*pfCandidates_)[iHad];
	    pfPunchThroughMuonCleanedCandidates_->push_back(pfc);
	    pfPunchThroughHadronCleanedCandidates_->push_back(pfch);
	    //std::cout << pfc << std::endl;
	    //std::cout << "Raw ecal/hcal : " << pfc.rawEcalEnergy() << " " << pfc.rawHcalEnergy() << std::endl;
	    //std::cout << "Hadron: " << (*pfCandidates_)[iHad] << std::endl;
	    double rescaleFactor = (*pfCandidates_)[iHad].p()/(*pfCandidates_)[imu->second].p();
	    metX -=  (*pfCandidates_)[imu->second].px() + (*pfCandidates_)[iHad].px();
	    metY -=  (*pfCandidates_)[imu->second].py() + (*pfCandidates_)[iHad].py();
	    (*pfCandidates_)[imu->second].rescaleMomentum(rescaleFactor);
	    (*pfCandidates_)[iHad].rescaleMomentum(0);
	    (*pfCandidates_)[imu->second].setParticleType(reco::PFCandidate::h);
	    metX +=  (*pfCandidates_)[imu->second].px();
	    metY +=  (*pfCandidates_)[imu->second].py();	  
	    met2 = metX*metX + metY*metY;
	    //std::cout << "Muon changed to charged hadron" << std::endl;
	    //std::cout << "MEX,MEY,MET Now (NO)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	    fixed = true;
	  } else if ( punchthrough1 || fake ) {
	    const reco::PFCandidate& pfc = (*pfCandidates_)[imu->second];
	    pfFakeMuonCleanedCandidates_->push_back(pfc);
	    metX -=  (*pfCandidates_)[imu->second].px();
	    metY -=  (*pfCandidates_)[imu->second].py();	  
	    met2 = metX*metX + metY*metY;
	    (*pfCandidates_)[imu->second].rescaleMomentum(0.);
	    //std::cout << "Muon cleaned (NO)" << std::endl;
	    //std::cout << "MEX,MEY,MET Now (NO)" << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	    fixed = true;
	  }
	}
      }
    }
  }

  // And now, add muons which did not make it for various reasons
  for ( unsigned imu = 0; imu < muonh->size(); ++imu ) {
    reco::MuonRef muonRef( muonh, imu );
    // If not used, check its effect on met
    reco::TrackRef combinedMu = muonRef->combinedMuon();
    reco::TrackRef trackerMu = muonRef->track();
    reco::TrackRef standAloneMu = muonRef->standAloneMuon();

    // check if the muons has already been taken
    bool used = false;
    bool hadron = false;
    // unsigned iHad = 1E9;
    for(unsigned i=0; i<pfCandidates_->size(); i++) {
      const reco::PFCandidate& pfc = (*pfCandidates_)[i];
      if ( pfc.trackRef().isNonnull() && pfc.trackRef() == trackerMu ) { 
	hadron = true;
	// iHad = i;
      }
      if ( !pfc.muonRef().isNonnull() || pfc.muonRef() != muonRef ) continue;
      used = true;
      if ( used ) break;
    }

    if ( used ) continue;

    double ptGL = muonRef->isGlobalMuon() ? combinedMu->pt() : 0.;
    double pxGL = muonRef->isGlobalMuon() ? combinedMu->px() : 0.;
    double pyGL = muonRef->isGlobalMuon() ? combinedMu->py() : 0.;
    double pzGL = muonRef->isGlobalMuon() ? combinedMu->pz() : 0.;

    double ptTK = muonRef->isTrackerMuon() ? trackerMu->pt() : 0.;
    double pxTK = muonRef->isTrackerMuon() ? trackerMu->px() : 0.;
    double pyTK = muonRef->isTrackerMuon() ? trackerMu->py() : 0.;
    double pzTK = muonRef->isTrackerMuon() ? trackerMu->pz() : 0.;

    double ptST = muonRef->isStandAloneMuon() ? standAloneMu->pt() : 0.;
    double pxST = muonRef->isStandAloneMuon() ? standAloneMu->px() : 0.;
    double pyST = muonRef->isStandAloneMuon() ? standAloneMu->py() : 0.;
    double pzST = muonRef->isStandAloneMuon() ? standAloneMu->pz() : 0.;

    //std::cout << "pT TK/GL/ST : " << ptTK << " " << ptGL << " " << ptST << std::endl;

    double metXTK = metX + pxTK; 
    double metYTK = metY + pyTK;
    double met2TK = metXTK*metXTK + metYTK*metYTK;
    
    double metXGL = metX + pxGL;
    double metYGL = metY + pyGL;
    double met2GL = metXGL*metXGL + metYGL*metYGL; 

    double metXST = metX + pxST; 
    double metYST = metY + pyST;
    double met2ST = metXST*metXST + metYST*metYST;

    //std::cout << "met TK/GL/ST : " << sqrt(met2) << " " << sqrt(met2TK) << " " << sqrt(met2GL) << " " << sqrt(met2ST) << std::endl;


    if ( ptTK > 20. && met2TK < met2/4. && met2TK < met2GL && met2TK < met2ST ) { 
      double energy = std::sqrt(pxTK*pxTK+pyTK*pyTK+pzTK*pzTK+0.1057*0.1057);
      int charge = trackerMu->charge()>0 ? 1 : -1;
      math::XYZTLorentzVector momentum(pxTK,pyTK,pzTK,energy);
      reco::PFCandidate::ParticleType particleType = 
	ptGL > 20. ? reco::PFCandidate::mu : reco::PFCandidate::h;
      double radius = std::sqrt(
				(trackerMu->vertex().x()-primaryVertex_.x())*(trackerMu->vertex().x()-primaryVertex_.x())+
				(trackerMu->vertex().y()-primaryVertex_.y())*(trackerMu->vertex().y()-primaryVertex_.y()));
      
      // Add it to the stack ( if not already in the list ) 
      if ( !hadron && radius < 1.0 ) { 
	pfCandidates_->push_back( reco::PFCandidate( charge, 
						     momentum,
						     particleType ) );
	
	pfCandidates_->back().setVertex( trackerMu->vertex() );
	pfCandidates_->back().setTrackRef( trackerMu );
	pfCandidates_->back().setMuonRef( muonRef );
	
	//std::cout << "MEX,MEY,MET before " << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	//std::cout << "Muon TK added " << std::endl;
	//std::cout << "pT TK/GL/ST : " << ptTK << " " << ptGL << " " << ptST << std::endl;
	metX +=  pfCandidates_->back().px();
	metY +=  pfCandidates_->back().py();	  
	met2 = metX*metX + metY*metY;
	//std::cout <<  pfCandidates_->back() << std::endl;
	//std::cout << "vertex : " << pfCandidates_->back().vx() << " " 
	//	  << pfCandidates_->back().vy() << " " 
	//	  << pfCandidates_->back().vz() << std::endl;
	//std::cout << "MEX,MEY,MET   now " << metX << " " << metY << " " << std::sqrt(met2) << std::endl; 

	const reco::PFCandidate& pfc = pfCandidates_->back();
	pfAddedMuonCandidates_->push_back(pfc);

	fixed = true;

      }

    } else  if (  ptGL > 20. && met2GL < met2/4. && met2GL < met2TK && met2GL < met2ST ) { 

      double energy = std::sqrt(pxGL*pxGL+pyGL*pyGL+pzGL*pzGL+0.1057*0.1057);
      int charge = combinedMu->charge()>0 ? 1 : -1;
      math::XYZTLorentzVector momentum(pxGL,pyGL,pzGL,energy);
      reco::PFCandidate::ParticleType particleType = reco::PFCandidate::mu;
      double radius = std::sqrt(
				(combinedMu->vertex().x()-primaryVertex_.x())*(combinedMu->vertex().x()-primaryVertex_.x())+
				(combinedMu->vertex().y()-primaryVertex_.y())*(combinedMu->vertex().y()-primaryVertex_.y()));
      
      // Add it to the stack
      if ( radius < 1.0 ) { 
	pfCandidates_->push_back( reco::PFCandidate( charge, 
						     momentum,
						     particleType ) );
	
	pfCandidates_->back().setVertex( combinedMu->vertex() );
	//if ( ptTK > 0. ) 
	if (trackerMu.isNonnull() ) pfCandidates_->back().setTrackRef( trackerMu );
	pfCandidates_->back().setMuonRef( muonRef );
	
	//std::cout << "MEX,MEY,MET before " << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	//std::cout << "Muon GL added " << std::endl;
	//std::cout << "pT TK/GL/ST : " << ptTK << " " << ptGL << " " << ptST << std::endl;
	
	metX +=  pfCandidates_->back().px();
	metY +=  pfCandidates_->back().py();	  
	met2 = metX*metX + metY*metY;
	
	//std::cout <<  pfCandidates_->back() << std::endl;
	//std::cout << "vertex : " 
	//	  << pfCandidates_->back().vx() << " " 
	//	  << pfCandidates_->back().vy() << " " 
	//	  << pfCandidates_->back().vz() << std::endl;
	//std::cout << "MEX,MEY,MET   now " << metX << " " << metY << " " << std::sqrt(met2) << std::endl;

	const reco::PFCandidate& pfc = pfCandidates_->back();
	pfAddedMuonCandidates_->push_back(pfc);

	fixed = true;

      }
	
    } else  if ( ptST > 20. &&  met2ST < met2/4. && met2ST < met2TK && met2ST < met2GL ) { 

      double energy = std::sqrt(pxST*pxST+pyST*pyST+pzST*pzST+0.1057*0.1057);
      int charge = standAloneMu->charge()>0 ? 1 : -1;
      math::XYZTLorentzVector momentum(pxST,pyST,pzST,energy);
      reco::PFCandidate::ParticleType particleType = reco::PFCandidate::mu;
      double radius = std::sqrt(
				(standAloneMu->vertex().x()-primaryVertex_.x())*(standAloneMu->vertex().x()-primaryVertex_.x())+
				(standAloneMu->vertex().y()-primaryVertex_.y())*(standAloneMu->vertex().y()-primaryVertex_.y()));
      
      // Add it to the stack
      if ( radius < 1.0 ) { 
	pfCandidates_->push_back( reco::PFCandidate( charge, 
						     momentum,
						     particleType ) );
	
	pfCandidates_->back().setVertex( standAloneMu->vertex() );
	if (trackerMu.isNonnull() ) pfCandidates_->back().setTrackRef( trackerMu );
	pfCandidates_->back().setMuonRef( muonRef );
	
	//std::cout << "MEX,MEY,MET before " << metX << " " << metY << " " << std::sqrt(met2) << std::endl;
	//std::cout << "Muon ST added " << std::endl;
	//std::cout << "pT TK/GL/ST : " << ptTK << " " << ptGL << " " << ptST << std::endl;
	
	metX +=  pfCandidates_->back().px();
	metY +=  pfCandidates_->back().py();	  
	met2 = metX*metX + metY*metY;
	
	//std::cout <<  pfCandidates_->back() << std::endl;
	//std::cout << "vertex : " 
	//	  << pfCandidates_->back().vx() << " " 
	//	  << pfCandidates_->back().vy() << " " 
	//	  << pfCandidates_->back().vz() << std::endl;
	//std::cout << "MEX,MEY,MET   now " << metX << " " << metY << " " << std::sqrt(met2) << std::endl;

	const reco::PFCandidate& pfc = pfCandidates_->back();
	pfAddedMuonCandidates_->push_back(pfc);

	fixed = true;

      }
    }

  }

  metAfter = std::sqrt(met2);
  return fixed;

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFEventFilter);


