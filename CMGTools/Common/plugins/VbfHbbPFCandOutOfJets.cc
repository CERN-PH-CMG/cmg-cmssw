// This is a class that creates a collection of charged PF candidates that do not belong
// to the 4 leading jets, or in the space between the 2 most b-tagged jets

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Jet.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "TMath.h"

class VbfHbbPFCandOutOfJets : public edm::EDProducer {
   public:
      typedef math::XYZTLorentzVector LorentzVector;
      explicit VbfHbbPFCandOutOfJets(const edm::ParameterSet&);
      ~VbfHbbPFCandOutOfJets();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      edm::InputTag srcJets_ , srcVtx_ , srcPF_ ;
      std::string   srcBtag_ ;
};

VbfHbbPFCandOutOfJets::VbfHbbPFCandOutOfJets(const edm::ParameterSet& iConfig)
{
  produces<reco::PFCandidateCollection>();

  srcJets_ = iConfig.getParameter<edm::InputTag> ("jets") ;
  srcVtx_  = iConfig.getParameter<edm::InputTag> ("vtx") ;
  srcPF_   = iConfig.getParameter<edm::InputTag> ("pfcandidates") ; 
  srcBtag_ = iConfig.getParameter<std::string>   ("btagger");   
}

VbfHbbPFCandOutOfJets::~VbfHbbPFCandOutOfJets()
{
}

void VbfHbbPFCandOutOfJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   std::auto_ptr<PFCandidateCollection> outPFCand(new PFCandidateCollection); 

   edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByLabel(srcJets_,jets);
   edm::View<pat::Jet> pat_jets = *jets;

   Handle<reco::VertexCollection> privtxs;
   iEvent.getByLabel(srcVtx_,privtxs);
  
   vector<reco::PFCandidatePtr> pfcandsin;
   pfcandsin.clear();
   
   vector<LorentzVector> vJetsP4;
   vJetsP4.clear();
   vector<double> vJetsBtag;
   vJetsP4.reserve(pat_jets.size());
   vJetsBtag.reserve(pat_jets.size());

   for (unsigned j=0; j != pat_jets.size(); ++j) {
     vJetsP4.push_back(pat_jets[j].p4());
    
     if (j < 4) { 
       vector<reco::PFCandidatePtr> pfcands = pat_jets[j].getPFConstituents();
       //cout<<"Found "<<pfcands.size()<<" constituents in jet #"<<j<<endl;
       for(unsigned i=0; i<pfcands.size(); ++i) {
         if ((PFCandidate::ParticleType(pfcands[i]->particleId()) == PFCandidate::h) || 
             (PFCandidate::ParticleType(pfcands[i]->particleId()) == PFCandidate::e) ||
             (PFCandidate::ParticleType(pfcands[i]->particleId()) == PFCandidate::mu)) {  
           pfcandsin.push_back(pfcands[i]);
         }
       }
     }
    
     vJetsBtag.push_back(pat_jets[j].bDiscriminator(srcBtag_));
   }
  
   if (pat_jets.size() > 3 && privtxs->size() > 0) {

     int ibtag[4];
     TMath::Sort(4,&(vJetsBtag.at(0)),ibtag);
    
     LorentzVector *bjet0P4 = &vJetsP4[ibtag[0]];
     LorentzVector *bjet1P4 = &vJetsP4[ibtag[1]];

     Handle<PFCandidateCollection> pfcands;
     iEvent.getByLabel(srcPF_,pfcands);

     // get the primary vertices points
     vector<math::XYZPoint> vpoints;
     vpoints.reserve(privtxs->size());
     for(VertexCollection::const_iterator vtx = privtxs->begin(); vtx != privtxs->end();++vtx) {
       vpoints.push_back(vtx->position());       
     }
     for(PFCandidateCollection::const_iterator ipf = pfcands->begin();ipf != pfcands->end(); ++ipf) {
       bool out(true);
       if ((PFCandidate::ParticleType(ipf->particleId()) == PFCandidate::h) || 
           (PFCandidate::ParticleType(ipf->particleId()) == PFCandidate::e) ||
           (PFCandidate::ParticleType(ipf->particleId()) == PFCandidate::mu)) {
	 for (unsigned i1 = 0; i1 != pfcandsin.size(); ++i1) {
	   if (&*ipf == &*(pfcandsin[i1])) { 
             out = false;
             break;
           }
	 }
         
         if (ipf->trackRef().isNonnull()) {
           if (ipf->trackRef()->pt() < 0.3) {
             out = false;
           }
	   float dzPV0 = ipf->trackRef()->dz(vpoints[0]);
	   float dzErr = ipf->trackRef()->dzError();
           float relErr(0.0);
           if (dzErr != 0) {
             relErr = fabs(dzPV0/dzErr);
           }
	   // minimum z-distance of track to the first PV : 2mm && 3sigma
	   if (fabs(dzPV0) > 0.2 || relErr > 3) {
             out = false;
           }
	   // loop over secondary (softer) Primary Vertices and exclude tracks more compatible with those 
	   for(unsigned iv = 1; iv < privtxs->size(); ++iv) {
	     float dz = ipf->trackRef()->dz(vpoints[iv]);
	     if (fabs(dz) < fabs(dzPV0)) {
               out = false;
             }
	   }
	   float dR0 = 0.5;
	   // remove tracks in an ellipse area around the two b-jets
	   float dRbb    = deltaR(*bjet0P4,*bjet1P4);
	   float dRbjet0 = deltaR(ipf->p4(),*bjet0P4); 
	   float dRbjet1 = deltaR(ipf->p4(),*bjet1P4); 
	   if (dRbjet0 + dRbjet1 < dRbb + 2*dR0) {
             out = false;
           }
         } // if trackRef is not null
         else {
           //cout<<"Found candidate with null track reference"<<endl; 
           out = false;
         }
	 if (out) {
	   outPFCand->push_back(*ipf);
	 }
       }// if charged hadron or lepton
     }// pf cand loop
   }// if 4 jets and at least one vertex
   iEvent.put(outPFCand);
}

void VbfHbbPFCandOutOfJets::beginJob()
{
}

void VbfHbbPFCandOutOfJets::endJob() 
{
}

DEFINE_FWK_MODULE(VbfHbbPFCandOutOfJets);
