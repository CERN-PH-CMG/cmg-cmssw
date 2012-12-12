// This is a class that creates a collection of tracks not belonging
// to the 4 leading jets, or in the space between the 2 most b-tagged jets

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/PatCandidates/interface/Jet.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidateFwd.h"

#include <TLorentzVector.h>

class VbfHbbTracksOutOfJets : public edm::EDProducer {
   public:
      explicit VbfHbbTracksOutOfJets(const edm::ParameterSet&);
      ~VbfHbbTracksOutOfJets();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      edm::InputTag srcJets_ , srcVtx_ , srcTrk_ ;
      std::string   srcBtag_ ;
};

VbfHbbTracksOutOfJets::VbfHbbTracksOutOfJets(const edm::ParameterSet& iConfig)
{
  produces<reco::RecoChargedRefCandidateCollection>();

  srcJets_ = iConfig.getParameter<edm::InputTag> ("jets") ;
  srcVtx_  = iConfig.getParameter<edm::InputTag> ("vtx") ;
  srcTrk_  = iConfig.getParameter<edm::InputTag> ("tracks") ; 
  srcBtag_ = iConfig.getParameter<std::string>   ("btagger");   
}

VbfHbbTracksOutOfJets::~VbfHbbTracksOutOfJets()
{
}

void VbfHbbTracksOutOfJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   std::auto_ptr<RecoChargedRefCandidateCollection> outTracks(new RecoChargedRefCandidateCollection);

   edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByLabel(srcJets_,jets);
   edm::View<pat::Jet> pat_jets = *jets;

   Handle<reco::VertexCollection> privtxs;
   iEvent.getByLabel(srcVtx_,privtxs);
  
   reco::TrackRefVector pftracksin;
   pftracksin.clear();
  
   vector<TLorentzVector> vJetsAll;
   vJetsAll.clear();
   int JetsPF(0);
   vector<double> PFBJBP;
  
   TLorentzVector a(0.0,0.0,0.0,0.0);
    
   vJetsAll.reserve(pat_jets.size());
   PFBJBP.reserve(pat_jets.size());

   for (unsigned i=0; i != pat_jets.size(); ++i) {
     a.SetPxPyPzE(pat_jets[i].px(),pat_jets[i].py(),pat_jets[i].pz(),pat_jets[i].energy());
     vJetsAll.push_back(a);
    
     reco::TrackRefVector pftrack = pat_jets[i].associatedTracks();  
    
     for(unsigned pftr=0; pftr<pftrack.size(); ++pftr) {
       if (i < 4) {
         pftracksin.push_back(pftrack[pftr]);
       }
     }
    
     PFBJBP.push_back(pat_jets[i].bDiscriminator(srcBtag_));
     ++JetsPF;
   }
  
   if (JetsPF > 3 && privtxs->size() > 0) {

     int ibtag[4];
     TMath::Sort(4,&(PFBJBP.at(0)),ibtag);
    
     TLorentzVector *bjet0 = &vJetsAll[ibtag[0]];
     TLorentzVector *bjet1 = &vJetsAll[ibtag[1]];

     TLorentzVector t(0.0,0.0,0.0,0.0);
     Handle<TrackCollection> tracks;
     iEvent.getByLabel(srcTrk_,tracks);

     // get the primary vertices points
     vector<math::XYZPoint> vpoints;
     vpoints.reserve(privtxs->size());
     for(VertexCollection::const_iterator vtx = privtxs->begin(); vtx != privtxs->end();++vtx) {
       vpoints.push_back(vtx->position());       
     }
     int j(0);
     for(TrackCollection::const_iterator itTrack = tracks->begin();itTrack != tracks->end(); ++itTrack,++j){
       bool out(true);
       // only high purity tracks with pT > 300 MeV
       if (itTrack->quality(TrackBase::highPurity) && itTrack->pt() > 0.3) {
	 for (unsigned i1 = 0; i1 != pftracksin.size(); ++i1) {
	   if (&*itTrack == &*(pftracksin[i1])) { 
             out = false;
           }
	 }
	 float dzPV0 = itTrack->dz(vpoints[0]);
	 float dzErr = itTrack->dzError();
         float relErr(0.0);
         if (dzErr != 0) {
           relErr = fabs(dzPV0/dzErr);
         }
	 // minimum z-distance of track to the first PV : 1mm && 3sigma
	 if (fabs(dzPV0) > 0.2 || relErr > 3) {
           out = false;
         }
	 // loop over secondary (softer) Primary Vertices and exclude tracks more compatible with those 
	 for(unsigned iv = 1; iv < privtxs->size(); ++iv) {
	   float dz = itTrack->dz(vpoints[iv]);
	   if (fabs(dz) < fabs(dzPV0)) {
             out = false;
           }
	 }
	 float dR0 = 0.5;
	 double energy = sqrt(0.139*0.139+itTrack->px()*itTrack->px()+itTrack->py()*itTrack->py()+itTrack->pz()*itTrack->pz());
	 t.SetPxPyPzE(itTrack->px(),itTrack->py(),itTrack->pz(),energy);
	 // remove tracks in an ellipse area around the two b-jets
	 float dRbb    = bjet0->DeltaR(*bjet1);
	 float dRbjet0 = t.DeltaR(*bjet0); 
	 float dRbjet1 = t.DeltaR(*bjet1); 
	 if (dRbjet0 + dRbjet1 < dRbb + 2*dR0) {
           out = false;
         }
	 if (out) {
	   TrackRef Tref = TrackRef(tracks,j);
	   RecoChargedRefCandidate refCand(RecoChargedRefCandidate(Tref,0.139));
	   outTracks->push_back(refCand);
	 }
       }
     }// track loop
   }// if 4 jets and at least one vertex
   iEvent.put(outTracks);
}

void VbfHbbTracksOutOfJets::beginJob()
{
}

void VbfHbbTracksOutOfJets::endJob() 
{
}

DEFINE_FWK_MODULE(VbfHbbTracksOutOfJets);
