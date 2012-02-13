// system include files
#include <memory>
#include <cmath>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CMGTools/HtoZZ2l2nu/interface/FastJetAlgoWrapper.h"

#include "Math/LorentzVector.h"

#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ClusteredPFMetProducer : public edm::EDProducer 
{
public:
  explicit ClusteredPFMetProducer(const edm::ParameterSet&);
  ~ClusteredPFMetProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int computeVertexAssociationFor(const reco::PFCandidateRef &candptr);
  int computeVertexAssociationFor(const  edm::Ptr<reco::PFCandidate> &candptr);
  int computeVertexAssociationFor(const reco::TrackBaseRef& trackBaseRef);

  CommonMETData getMETData(const LorentzVector& met, double& set);

  void getSeedJetsFromCandidateAssociatedToVertex(std::vector<reco::PFJet>& inputSeedJet, int iVtx);

  double getJetRadius(const reco::PFJet& jet);

  edm::Handle<reco::VertexCollection> vtxH_;  
  edm::Handle<reco::PFCandidateCollection> pfCandsH_;
  edm::InputTag collectionTag_, vertexTag_;    
  double minNeutralPt_, maxNeutralEta_;
  double minJetPt_, maxJetEta_;
  std::vector<int> vertexAssociationMasks_;

  FastJetAlgoWrapper jetProducer_;

  bool minBiasMode_;

   bool simpleNeutralAssociation_;

};

using namespace std;

//
ClusteredPFMetProducer::ClusteredPFMetProducer(const edm::ParameterSet& iConfig):
  collectionTag_(iConfig.getParameter<edm::InputTag>("collectionTag")),
  vertexTag_(iConfig.getParameter<edm::InputTag>("vertexTag")),
  minNeutralPt_(iConfig.getParameter<double>("minNeutralPt")),
  maxNeutralEta_(iConfig.getParameter<double>("maxNeutralEta")),
  minJetPt_(iConfig.getParameter<double>("minJetPt")),
  maxJetEta_(iConfig.getParameter<double>("maxJetEta")),
  jetProducer_(iConfig.getParameter<edm::ParameterSet>("fastjet")),
  minBiasMode_(iConfig.getParameter<bool>("minBiasMode"))
//  simpleNeutralAssociation_(iConfig.getParameter<bool>("simpleNeutralAssociation"))
{
  produces<reco::PFMET>("standard");
  produces<reco::PFMET>("central");
  produces<reco::PFMET>("assocCharged");
  produces<reco::PFMET>("assoc");
  produces<reco::PFMET>("assocWithFwd");
  produces<reco::PFMET>("cleaned");

  produces<reco::PFMET>("assocBeta");
  produces<reco::PFMET>("assocWithFwdBeta");


  produces<std::vector<int> >("pvAssocCandidates");
  produces<std::vector<double> >("globalPfMetSums"); 
  produces<std::vector<reco::PFJet> >("JET");
}

//
int ClusteredPFMetProducer::computeVertexAssociationFor(const reco::PFCandidateRef &candptr)
{
  reco::TrackBaseRef trackBaseRef( candptr->trackRef() );
  //if(!trackBaseRef.isNull() && !trackBaseRef.isAvailable())std::cout << "PFlow candidate = " << candptr->pdgId() << " pt = " << candptr->pt() << endl;
  return computeVertexAssociationFor(trackBaseRef);
}


int ClusteredPFMetProducer::computeVertexAssociationFor(const  edm::Ptr<reco::PFCandidate> &candptr)
{
  reco::TrackBaseRef trackBaseRef( candptr->trackRef() );
  //if(!trackBaseRef.isNull() && !trackBaseRef.isAvailable())std::cout << "PFlow candidate = " << candptr->pdgId() << " pt = " << candptr->pt() << endl;
  return computeVertexAssociationFor(trackBaseRef);
}


int ClusteredPFMetProducer::computeVertexAssociationFor(const reco::TrackBaseRef& trackBaseRef)
{
  int assocVtxRef(-1);

  //no association for neutrals
  if(trackBaseRef.isNull() || !trackBaseRef.isAvailable()){
     return assocVtxRef;
   }

  //check the vertices which use the track in the fit
  int nVerticesAss(0);
  float bestweight(0),bestDz(9999.);
  const size_t nVtx(vtxH_->size());
  for(size_t jVtx=0; jVtx<nVtx; jVtx++)
    {
      const reco::VertexRef vtxref(vtxH_,jVtx);
      float vtxWeight(0);

      try{
        vtxWeight= vtxref->trackWeight(trackBaseRef);
      }catch(std::exception &e){
        //if not available then track was not used in vertex fit
      }

      float vtxDz( fabs( trackBaseRef->dz( vtxref->position()) ) );

      if(vtxWeight > bestweight || ( vtxWeight == bestweight && vtxDz < bestDz))
        {
          bestweight=vtxWeight;
          bestDz=vtxDz;
          assocVtxRef=jVtx;
          nVerticesAss++;
        }
    }

  return assocVtxRef;
}




//
void ClusteredPFMetProducer::getSeedJetsFromCandidateAssociatedToVertex(std::vector<reco::PFJet>& inputSeedJet, int iVtx){
      for(unsigned int i=0;i<pfCandsH_->size();i++){
         if(vertexAssociationMasks_[i] != iVtx)continue;
         reco::PFCandidateRef candRef(pfCandsH_,i);
         reco::Particle::LorentzVector p4(candRef->px(),candRef->py(), candRef->pz(),candRef->energy() );
         reco::Particle::Point vertex(iVtx, iVtx, iVtx);
         reco::PFJet::Specific specific;
         // need to add the constituents as well (see base Jet, or CompositePtrCandidate)
         reco::Jet::Constituents ptrsToConstituents;
         ptrsToConstituents.push_back( edm::Ptr<reco::PFCandidate>(pfCandsH_, i));
         makeSpecific( ptrsToConstituents, &specific );
         reco::PFJet jet(p4, vertex, specific, ptrsToConstituents);
         inputSeedJet.push_back(jet);
      }
}

CommonMETData ClusteredPFMetProducer::getMETData(const LorentzVector& met, double& set){
   CommonMETData pfOutput;
   pfOutput.mex = -met.px();
   pfOutput.mey = -met.py();
   pfOutput.mez = -met.pz();
   pfOutput.met = met.pt();
   pfOutput.sumet = set;
   pfOutput.phi = atan2(-met.py(),-met.px());
   return pfOutput;
}

double ClusteredPFMetProducer::getJetRadius(const reco::PFJet& jet){
   for(double R=0.3;R<1.0;R+=0.1){
      if(jet.etInAnnulus(0, R)>=0.90)return R;
   }
   return 0.5;
}


//
void ClusteredPFMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  //vertices
  iEvent.getByLabel(vertexTag_ ,vtxH_);
  const size_t nVtx(vtxH_->size());
  if( nVtx == 0 ) throw cms::Exception("CorruptData") << "No vertex was found in the event\n";
  
  //particle flow candidates
  iEvent.getByLabel(collectionTag_, pfCandsH_); 
  const size_t nPFCands( pfCandsH_->size() );
  if( nPFCands == 0 ) throw cms::Exception("CorruptData") << "No particle flow candidates collection was found in the event\n";


  //loop over the pf candidates and:
  // - compute sums 
  // - associate pf candidates to vertices
  vertexAssociationMasks_.clear();
  vertexAssociationMasks_.resize(nPFCands,-1);
  

  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++){
      reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
      int assocVertexRef = computeVertexAssociationFor(candRef);
      if(assocVertexRef<0) continue;
      vertexAssociationMasks_[iPFCand] = assocVertexRef;
    }

  //Nomenclature:  Pvtx=associated to the PrimaryVertex, Ovtx=associated to an other vertex than the PrimaryVertex, Nvtx:Not associated to the primary vertex
  //Nomenclature:  g=Global, All=No cuts, Fwd=Eta>2.4, Cen=Eta<2.4
  LorentzVectorCollection met_PvtxCharged(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_PvtxCharged(nVtx,0);
  LorentzVectorCollection met_PvtxNeutral(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_PvtxNeutral(nVtx,0);
  LorentzVectorCollection met_PvtxBetaCor(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_PvtxBetaCor(nVtx,0);
  LorentzVectorCollection met_OvtxCharged(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_OvtxCharged(nVtx,0);
  LorentzVectorCollection met_NvtxCharged(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_NvtxCharged(nVtx,0); 
  LorentzVectorCollection met_NvtxNeutral(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_NvtxNeutral(nVtx,0);
  LorentzVectorCollection met_NvtxNeutFwd(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_NvtxNeutFwd(nVtx,0);
  LorentzVectorCollection met_NvtxNeutCen(nVtx,LorentzVector(0,0,0,0));   std::vector<double>   set_NvtxNeutCen(nVtx,0);
  LorentzVector           met_gAllCharged(0,0,0,0);                       double                set_gAllCharged(0);
  LorentzVector           met_gAllNeutral(0,0,0,0);                       double                set_gAllNeutral(0);
  LorentzVector           met_gFwdNeutral(0,0,0,0);                       double                set_gFwdNeutral(0);
  LorentzVector           met_gCenNeutral(0,0,0,0);                       double                set_gCenNeutral(0);


  //#######################################################################
  // BUILD ALL COMPNENT VECTOR THAT WILL BE USED TO BUILD MET VECTOR LATER
  //#######################################################################

  std::map<int, std::vector<reco::PFJet> > vtxJets;
  std::map<int, std::vector<reco::PFJet> > vtxJetsPlusNeutral;
  std::vector<reco::PFJet> inputSeedJetForUnassocPart; //build the vector of particles unassociated to any vertex (mostly neutral candidates)
  getSeedJetsFromCandidateAssociatedToVertex(inputSeedJetForUnassocPart, -1);

  for(size_t iVtx=0; iVtx<nVtx; iVtx++){
      //Jet building
      std::vector<reco::PFJet> inputSeedJetAssociatedPartToiVtx;
      getSeedJetsFromCandidateAssociatedToVertex(inputSeedJetAssociatedPartToiVtx, iVtx);
      vtxJets[iVtx] = jetProducer_.produce(inputSeedJetAssociatedPartToiVtx,2.); 
      const std::vector<reco::PFJet> &jetColl=vtxJets[iVtx];
      std::vector<reco::PFJet>::const_iterator jIt(jetColl.begin()), jItEnd(jetColl.end());

      std::vector<reco::PFJet> inputSeedJetTemp;
      inputSeedJetTemp.insert(inputSeedJetTemp.end(), vtxJets[iVtx].begin(), vtxJets[iVtx].end());
      inputSeedJetTemp.insert(inputSeedJetTemp.end(), inputSeedJetForUnassocPart.begin(), inputSeedJetForUnassocPart.end() );
      std::vector<reco::PFJet> vtxJetPlusNeutralTemp = jetProducer_.produce(inputSeedJetTemp, 2.);
      for(unsigned int i=0;i<vtxJetPlusNeutralTemp.size();i++){if(vtxJetPlusNeutralTemp[i].chargedMultiplicity()>0)vtxJetsPlusNeutral[iVtx].push_back(vtxJetPlusNeutralTemp[i]);}

//      printf("### Inputs:\n");
//      for(unsigned int i=0;i<inputSeedJetAssociatedPartToiVtx.size();i++){printf("%3i --> Pt%4f Eta%+6.3f Phi%+6.3f NConstituent%3i\n", i, inputSeedJetAssociatedPartToiVtx[i].pt(), inputSeedJetAssociatedPartToiVtx[i].eta(), inputSeedJetAssociatedPartToiVtx[i].phi(),(int) inputSeedJetAssociatedPartToiVtx[i].getPFConstituents().size());}
//      printf("### Outputs:\n");
//      for(unsigned int i=0;i<vtxJets[iVtx].size();i++){printf("%3i --> Pt%4f Eta%+6.3f Phi%+6.3f NConstituent%3i", i, vtxJets[iVtx][i].pt(), vtxJets[iVtx][i].eta(), vtxJets[iVtx][i].phi(),(int) vtxJets[iVtx][i].getPFConstituents().size());
//         for(unsigned int j=0;j<vtxJets[iVtx][i].getPFConstituents().size();j++){printf("(%6.2f,%i) ", vtxJets[iVtx][i].getPFConstituents()[j]->pt(), vtxJets[iVtx][i].getPFConstituents()[j]->charge()); }printf("\n");
//      }
//      printf("### Outputs with Neutrals:\n");
//      for(unsigned int i=0;i<vtxJetsPlusNeutral[iVtx].size();i++){printf("%3i --> Pt%4f Eta%+6.3f Phi%+6.3f NConstituent%3i", i, vtxJetsPlusNeutral[iVtx][i].pt(), vtxJetsPlusNeutral[iVtx][i].eta(), vtxJetsPlusNeutral[iVtx][i].phi(),(int) vtxJetsPlusNeutral[iVtx][i].getPFConstituents().size());
//         for(unsigned int j=0;j<vtxJetsPlusNeutral[iVtx][i].getPFConstituents().size();j++){printf("(%6.2f,%i) ", vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->pt(), vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->charge()); }printf("\n");
//      }
//      printf("----------------------------------------------\n");
   }

  for(size_t iVtx=0;iVtx<nVtx; iVtx++){   
      //loop over charged PF candidates in order to make the Charged MET vector (and compute DeltaBeta corrections)
      for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++){
	reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
        if(candRef->charge()==0){
           //NEUTRALS
           if(iVtx==0)met_gAllNeutral       += candRef->p4();
           if(iVtx==0)set_gAllNeutral       += candRef->pt();

           met_NvtxNeutral[iVtx] += candRef->p4();
           set_NvtxNeutral[iVtx] += candRef->pt();

           if(fabs(candRef->eta())>2.4){
              if(iVtx==0)met_gFwdNeutral       += candRef->p4();
              if(iVtx==0)set_gFwdNeutral       += candRef->pt();

              met_NvtxNeutFwd[iVtx] += candRef->p4();
              set_NvtxNeutFwd[iVtx] += candRef->pt();
           }else{
              if(iVtx==0)met_gCenNeutral       += candRef->p4();
              if(iVtx==0)set_gCenNeutral       += candRef->pt();

              met_NvtxNeutCen[iVtx] += candRef->p4();
              set_NvtxNeutCen[iVtx] += candRef->pt();
           }
        }else{
           if(iVtx==0)met_gAllCharged       += candRef->p4();
           if(iVtx==0)set_gAllCharged       += candRef->pt();           

           //CHARGED
           if(vertexAssociationMasks_[iPFCand]==int(iVtx)){
              met_PvtxCharged[iVtx] += candRef->p4();
              set_PvtxCharged[iVtx] += candRef->pt();
           }else if(vertexAssociationMasks_[iPFCand]!=int(iVtx)){
              met_NvtxCharged[iVtx] += candRef->p4();
              set_NvtxCharged[iVtx] += candRef->pt();

              if(vertexAssociationMasks_[iPFCand]<=-1)continue; //skip all unassociated charged particles
              //BetaCorrection --> Removing 50% of the energy of the charged inside the jets (assuming that there is twice more charged than neutral from PU inside this jet)
              for(unsigned int i=0;i<vtxJetsPlusNeutral[iVtx].size();i++){
                 met_OvtxCharged[iVtx] += candRef->p4();
                 set_OvtxCharged[iVtx] += candRef->pt();

                 double dr = deltaR(vtxJetsPlusNeutral[iVtx][i].eta(),vtxJetsPlusNeutral[iVtx][i].phi(), candRef->eta(), candRef->phi());
                 double jetCone = getJetRadius(vtxJetsPlusNeutral[iVtx][i]);
                 if(dr>jetCone)continue;
                 met_PvtxBetaCor[iVtx] += -0.5 * candRef->p4();
                 set_PvtxBetaCor[iVtx] += -0.5 * candRef->pt();
                 //A charged particle can be associated to only one single jet!
                 break;
              }
           }
        }
     }


     //Now Loop on Jet collection in order to build MET vectors related to the neutral particles associated to that vertex
     for(unsigned int i=0;i<vtxJetsPlusNeutral[iVtx].size();i++){
        for(unsigned int j=0;j<vtxJetsPlusNeutral[iVtx][i].getPFConstituents().size();j++){
           if(vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->charge()==0){
              //neutral constituent associated to this charged jet
              met_PvtxNeutral[iVtx] += vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->p4();
              set_PvtxNeutral[iVtx] += vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->pt();

              //need to remove the neutral associated to the pVtx
              met_NvtxNeutral[iVtx] -= vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->p4();
              set_NvtxNeutral[iVtx] -= vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->pt();

              if(fabs(vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->eta())>2.4){
                 met_NvtxNeutFwd[iVtx] -= vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->p4();
                 set_NvtxNeutFwd[iVtx] -= vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->pt();
              }else{
                 met_NvtxNeutCen[iVtx] -= vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->p4();
                 set_NvtxNeutCen[iVtx] -= vtxJetsPlusNeutral[iVtx][i].getPFConstituents()[j]->pt();
              }
           }
        }
     }  
  }

  //#######################################################################
  // BUILD HIGH LEVEL MET VECTOR AND PUT THEM INTO THE EVENT 
  //#######################################################################


  //check which candidates got associated
  std::auto_ptr< std::vector<int> > pvAssocCandidatesPtr( new std::vector<int> );
  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++){
      if(vertexAssociationMasks_[iPFCand]!=0) continue;
      pvAssocCandidatesPtr->push_back( iPFCand );
  }
  iEvent.put(pvAssocCandidatesPtr,"pvAssocCandidates");

  PFSpecificAlgo pf;
  edm::Handle<edm::View<reco::Candidate> > pfRecoCandsH;
  iEvent.getByLabel(collectionTag_, pfRecoCandsH); 

  LorentzVector met_standard = met_gAllCharged + met_gAllNeutral;
  double        set_standard = set_gAllCharged + set_gAllNeutral;
  std::auto_ptr<reco::PFMET> standardPtr(new reco::PFMET);
  *standardPtr = pf.addInfo(pfRecoCandsH,getMETData(met_standard, set_standard));
  iEvent.put(standardPtr,"standard");

  LorentzVector met_central = met_gAllCharged + met_gCenNeutral;
  double        set_central = set_gAllCharged + set_gCenNeutral;
  std::auto_ptr<reco::PFMET> centralPtr(new reco::PFMET);
  *centralPtr = pf.addInfo(pfRecoCandsH,getMETData(met_central, set_central));
  iEvent.put(centralPtr,"central");

  LorentzVector met_assocCharged = met_PvtxCharged[0];
  double        set_assocCharged = set_PvtxCharged[0];
  std::auto_ptr<reco::PFMET> assocChargedPtr(new reco::PFMET);
  *assocChargedPtr = pf.addInfo(pfRecoCandsH,getMETData(met_assocCharged, set_assocCharged));
  iEvent.put(assocChargedPtr,"assocCharged");

  LorentzVector met_assoc = met_PvtxCharged[0] + met_PvtxNeutral[0];
  double        set_assoc = set_PvtxCharged[0] + set_PvtxNeutral[0];
  std::auto_ptr<reco::PFMET> assocPtr(new reco::PFMET);
  *assocPtr = pf.addInfo(pfRecoCandsH,getMETData(met_assoc, set_assoc));
  iEvent.put(assocPtr,"assoc");

  LorentzVector met_assocFwd = met_PvtxCharged[0] + met_PvtxNeutral[0] + met_NvtxNeutFwd[0];
  double        set_assocFwd = set_PvtxCharged[0] + set_PvtxNeutral[0] + set_NvtxNeutFwd[0];
  std::auto_ptr<reco::PFMET> assocFwdPtr(new reco::PFMET);
  *assocFwdPtr = pf.addInfo(pfRecoCandsH,getMETData(met_assocFwd, set_assocFwd));
  iEvent.put(assocFwdPtr,"assocWithFwd");

  LorentzVector met_cleaned = met_PvtxCharged[0] + met_PvtxNeutral[0] + met_NvtxNeutFwd[0] + met_NvtxCharged[0];
  double        set_cleaned = set_PvtxCharged[0] + set_PvtxNeutral[0] + set_NvtxNeutFwd[0] + set_NvtxCharged[0];
  std::auto_ptr<reco::PFMET> cleanedPtr(new reco::PFMET);
  *cleanedPtr = pf.addInfo(pfRecoCandsH,getMETData(met_cleaned, set_cleaned));
  iEvent.put(cleanedPtr,"cleaned");

  //BetaCorrected are below
  LorentzVector met_assocB = met_PvtxCharged[0] + met_PvtxNeutral[0] + met_PvtxBetaCor[0];
  double        set_assocB = set_PvtxCharged[0] + set_PvtxNeutral[0] + set_PvtxBetaCor[0];
  std::auto_ptr<reco::PFMET> assocBPtr(new reco::PFMET);
  *assocBPtr = pf.addInfo(pfRecoCandsH,getMETData(met_assocB, set_assocB));
  iEvent.put(assocBPtr,"assocBeta");

  LorentzVector met_assocFwdB = met_PvtxCharged[0] + met_PvtxNeutral[0] + met_NvtxNeutFwd[0] + met_PvtxBetaCor[0];
  double        set_assocFwdB = set_PvtxCharged[0] + set_PvtxNeutral[0] + set_NvtxNeutFwd[0] + set_PvtxBetaCor[0];
  std::auto_ptr<reco::PFMET> assocFwdBPtr(new reco::PFMET);
  *assocFwdBPtr = pf.addInfo(pfRecoCandsH,getMETData(met_assocFwdB, set_assocFwdB));
  iEvent.put(assocFwdBPtr,"assocWithFwdBeta");

  std::auto_ptr<std::vector<double> > globalPfMetSumsPtr(new std::vector<double> );
  globalPfMetSumsPtr->resize(12,0);
  (*globalPfMetSumsPtr)[0] = set_gAllCharged + set_gAllNeutral;
  (*globalPfMetSumsPtr)[1] = set_gAllCharged;
  (*globalPfMetSumsPtr)[2] = set_gAllNeutral;
  (*globalPfMetSumsPtr)[3] = set_gAllCharged + set_gCenNeutral;
  (*globalPfMetSumsPtr)[4] = set_gAllCharged;
  (*globalPfMetSumsPtr)[5] = set_gCenNeutral;
  (*globalPfMetSumsPtr)[6] = set_PvtxCharged[0] + set_PvtxNeutral[0];
  (*globalPfMetSumsPtr)[7] = set_PvtxCharged[0];
  (*globalPfMetSumsPtr)[8] = set_PvtxNeutral[0];
  for(size_t ivtx=1; ivtx<nVtx; ivtx++){
      (*globalPfMetSumsPtr)[9]  += set_PvtxCharged[ivtx] + set_PvtxNeutral[ivtx]; 
      (*globalPfMetSumsPtr)[10] += set_PvtxCharged[ivtx]; 
      (*globalPfMetSumsPtr)[11] += set_PvtxNeutral[ivtx]; 
  }
  iEvent.put(globalPfMetSumsPtr,"globalPfMetSums");

  std::auto_ptr<std::vector< reco::PFJet> > jetCollWithUnAssocPtr(new std::vector< reco::PFJet> );
  for(unsigned int i=0;i<vtxJetsPlusNeutral[0].size();i++){jetCollWithUnAssocPtr->push_back(vtxJetsPlusNeutral[0][i]);}
  iEvent.put(jetCollWithUnAssocPtr,"JET");
}

//
ClusteredPFMetProducer::~ClusteredPFMetProducer() { }

//
void ClusteredPFMetProducer::beginJob() { }

//
void ClusteredPFMetProducer::endJob() { }


DEFINE_FWK_MODULE(ClusteredPFMetProducer);

//  LocalWords:  numberOfValidPixelHits
