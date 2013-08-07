//-----------------------------------------------------------------------------------------
//
// Computation of the trackIsolation, for use with the isolated track veto 
// used for the stop quark search in the single lepton channel
// Author: Ben Hooberman
//
// For each PFCandidate above threshold minPt_PFCandidate store 4 quantities:
// pT of PFCandidate
// charge of PFCandidate
// dz of PFCandidate w.r.t. the 1st good vertex
// the trackIsolation value
//
// In the analysis, we veto any event containing IN ADDITION TO the selected lepton a charged PFCandidate with:
// pT > 10 GeV, dz < 0.05 cm, and trackIso/pT < 0.1
//
//-----------------------------------------------------------------------------------------

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "CMGTools/Common/plugins/TrackIsolationMaker.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

typedef math::XYZTLorentzVectorF LorentzVector;
typedef math::XYZPoint Point;
using namespace reco;
using namespace edm;
using namespace std;

//
// class decleration
//

//
// constructors and destructor
//

TrackIsolationMaker::TrackIsolationMaker(const edm::ParameterSet& iConfig) {

  pfCandidatesTag_		= iConfig.getParameter<InputTag>	("pfCandidatesTag");
  vertexInputTag_               = iConfig.getParameter<InputTag>        ("vertexInputTag");
  
  dR_               = iConfig.getParameter<double>          ("dR_ConeSize");       // dR value used to define the isolation cone                (default 0.3 )
  dzcut_            = iConfig.getParameter<double>          ("dz_CutValue");       // cut value for dz(trk,vtx) for track to include in iso sum (default 0.05)
  minPt_            = iConfig.getParameter<double>          ("minPt_PFCandidate"); // store PFCandidates with pt above this cut                 (default 0   )
  
  produces<vector<float> >("pfcandstrkiso").setBranchAlias("pfcands_trkiso");
  produces<vector<float> >("pfcandsdzpv"  ).setBranchAlias("pfcands_dzpv");
  produces<vector<float> >("pfcandspt"    ).setBranchAlias("pfcands_pt");
  produces<vector<int>   >("pfcandschg"   ).setBranchAlias("pfcands_chg");
    
}

TrackIsolationMaker::~TrackIsolationMaker() 
{

}

void  TrackIsolationMaker::beginRun(edm::Run&, const edm::EventSetup& es) {}
void  TrackIsolationMaker::beginJob() {}
void  TrackIsolationMaker::endJob()   {}

// ------------ method called to produce the data  ------------

void TrackIsolationMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  auto_ptr<vector<float> >  pfcands_trkiso(new vector<float>);
  auto_ptr<vector<float> >  pfcands_dzpv  (new vector<float>);
  auto_ptr<vector<float> >  pfcands_pt    (new vector<float>);
  auto_ptr<vector<int>   >  pfcands_chg   (new vector<int>  );

  //---------------------------------
  // get PFCandidate collection
  //---------------------------------
  
  Handle<PFCandidateCollection> pfCandidatesHandle;
  iEvent.getByLabel(pfCandidatesTag_, pfCandidatesHandle);
  pfCandidates  = pfCandidatesHandle.product();

  //---------------------------------
  // get Vertex Collection
  //---------------------------------
  
  Handle<reco::VertexCollection> vertex_h;
  iEvent.getByLabel(vertexInputTag_, vertex_h);
  const reco::VertexCollection *vertices = vertex_h.product();

  //-----------------------------------
  // Find 1st good vertex
  //-----------------------------------

  VertexCollection::const_iterator firstGoodVertex = vertices->end();

  int firstGoodVertexIdx = 0;

  for (VertexCollection::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx, ++firstGoodVertexIdx) {
    if (  !vtx->isFake() && vtx->ndof()>=4. && vtx->position().Rho()<=2.0 && fabs(vtx->position().Z())<=24.0) {
      firstGoodVertex = vtx;
      break;
    }
  }

  //-------------------------------------------------------------------------------------------------
  // loop over PFCandidates and calculate the trackIsolation and dz w.r.t. 1st good PV for each one
  // for neutral PFCandidates, store trkiso = 999 and dzpv = 999
  //-------------------------------------------------------------------------------------------------

  for( PFCandidateCollection::const_iterator pf_it = pfCandidates->begin(); pf_it != pfCandidates->end(); pf_it++ ) {

    //-------------------------------------------------------------------------------------
    // only store PFCandidate values if pt > minPt
    //-------------------------------------------------------------------------------------

    if( pf_it->pt() < minPt_ ) continue;

    //-------------------------------------------------------------------------------------
    // store pt and charge of PFCandidate
    //-------------------------------------------------------------------------------------

    pfcands_pt->push_back(pf_it->pt());
    pfcands_chg->push_back(pf_it->charge());

    //-------------------------------------------------------------------------------------
    // if there's no good vertex in the event, we can't calculate anything so store 999999
    //-------------------------------------------------------------------------------------
    
    if ( firstGoodVertex==vertices->end() ) {
      pfcands_trkiso->push_back(999);
      pfcands_dzpv->push_back(999);
      continue;
    }

    //-------------------------------------------------------
    // require PFCandidate is charged, otherwise store 999 
    //-------------------------------------------------------

    if( pf_it->charge() != 0 ){

      //----------------------------------------------------------------------------
      // now loop over other PFCandidates in the event to calculate trackIsolation
      //----------------------------------------------------------------------------

      float trkiso = 0.0;

      for( PFCandidateCollection::const_iterator pf_other = pfCandidates->begin(); pf_other != pfCandidates->end(); pf_other++ ) {

	// don't count the PFCandidate in its own isolation sum
	if( pf_it == pf_other       ) continue;

	// require the PFCandidate to be charged
	if( pf_other->charge() == 0 ) continue;

        // cut on dR between the PFCandidates
        float dR = deltaR(pf_it->eta(), pf_it->phi(), pf_other->eta(), pf_other->phi());
        if( dR > dR_ ) continue;

	// cut on the PFCandidate dz
	float dz_other = 100;

	if ( pf_other->trackRef().isNonnull()) {
	  dz_other = pf_other->trackRef()->dz( firstGoodVertex->position() );
	}

	if( fabs(dz_other) > dzcut_ ) continue;

	trkiso += pf_other->pt();
      }

      // calculate the dz of this candidate
      float dz_it = 100; //

      if ( pf_it->trackRef().isNonnull()) {
	dz_it = pf_it->trackRef()->dz( firstGoodVertex->position() );
      }

      // store trkiso and dz values
      pfcands_trkiso->push_back(trkiso);
      pfcands_dzpv->push_back(dz_it);

    }else{
      //neutral particle, set trkiso and dzpv to 9999
      pfcands_trkiso->push_back(9999);
      pfcands_dzpv->push_back(9999);
    }

  }
            
  // put trkiso and dz values back into event
  iEvent.put(pfcands_trkiso,"pfcandstrkiso");
  iEvent.put(pfcands_dzpv  ,"pfcandsdzpv"  );
  iEvent.put(pfcands_pt    ,"pfcandspt"    );
  iEvent.put(pfcands_chg   ,"pfcandschg"   );
 
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackIsolationMaker);

