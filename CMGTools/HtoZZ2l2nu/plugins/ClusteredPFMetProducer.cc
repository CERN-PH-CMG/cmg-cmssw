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

#include "RecoParticleFlow/PostProcessing/src/FastJetAlgo.cc"

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
  std::vector<bool> getPFCandidatesMaskForVertex(int iVtx);

  edm::Handle<reco::VertexCollection> vtxH_;  
  edm::Handle<reco::PFCandidateCollection> pfCandsH_;
  edm::InputTag collectionTag_, vertexTag_;    
  double minNeutralPt_, maxNeutralEta_;
  double minJetPt_, maxJetEta_;
  std::vector<int> vertexAssociationMasks_;

  FastJetAlgoPFCandPFJet jetProducer_;

  double jetCone_;

  bool minBiasMode_;

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
  jetCone_(iConfig.getParameter<edm::ParameterSet>("fastjet").getParameter<double>("distance_par")),
  minBiasMode_(iConfig.getParameter<bool>("minBiasMode"))
{
  produces<reco::PFMET>("assocPfMet");
  produces<reco::PFMET>("assocPfMetCorrected");
  produces<reco::PFMET>("assocPfMetWithFwd");
  produces<reco::PFMET>("assocPfMetWithFwd2");
  produces<reco::PFMET>("assocOtherVtxPfMet");
  produces<reco::PFMET>("trkPfMet");
  produces<reco::PFMET>("globalPfMet");
  produces<reco::PFMET>("centralPfMet");
  produces<reco::PFMET>("cleanPfMet");
  produces<std::vector<int> >("pvAssocCandidates");
  produces<std::vector<double> >("globalPfMetSums"); 
}

//
int ClusteredPFMetProducer::computeVertexAssociationFor(const reco::PFCandidateRef &candptr)
{
  int assocVtxRef(-1);

  reco::TrackBaseRef trackBaseRef( candptr->trackRef() );

  //no association for neutrals
  if(trackBaseRef.isNull()) return assocVtxRef;

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
std::vector<bool> ClusteredPFMetProducer::getPFCandidatesMaskForVertex(int iVtx)
{
  size_t nPFCands(pfCandsH_->size());
  std::vector<bool> pfCandMasks(nPFCands,true);
  for(size_t iPFcand=0; iPFcand<nPFCands; iPFcand++)
    pfCandMasks[iPFcand]=(vertexAssociationMasks_[iPFcand] == iVtx);
  
  return pfCandMasks;
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
  
  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
    {
      reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
      int assocVertexRef = computeVertexAssociationFor(candRef);
      if(assocVertexRef<0) continue;
      vertexAssociationMasks_[iPFCand] = assocVertexRef;
    }
 


  //now loop over the vertices and:
  // - build jets from associated candidates
  // - compute charged sums, associated neutral sums
  std::map<int, std::vector<reco::PFJet> > vtxJets;
  LorentzVectorCollection vtxChSum(nVtx,LorentzVector(0,0,0,0)), vtxNeutralSum(nVtx,LorentzVector(0,0,0,0)),  betaCorrectionSum(nVtx,LorentzVector(0,0,0,0)), vtxPFMet(nVtx,LorentzVector(0,0,0,0));   
  std::vector<double>     vtxChSumEt(nVtx,0),                    vtxNeutralSumEt(nVtx,0),                     betaCorrectionSumEt(nVtx,0),                    vtxSumEt(nVtx,0);                       

  for(size_t iVtx=0; iVtx<nVtx; iVtx++)
    {
      const std::vector<bool> candMasks = getPFCandidatesMaskForVertex(iVtx);
  
      //Jet building
      vtxJets[iVtx] = jetProducer_.produce(pfCandsH_,candMasks,false,2.,99999);
      const std::vector<reco::PFJet> &jetColl=vtxJets[iVtx];
      std::vector<reco::PFJet>::const_iterator jIt(jetColl.begin()), jItEnd(jetColl.end());
      
      //loop over PF candidates and compute charged + neutral sums
      for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
	{
	  reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
	  if(candRef->charge()==0)
	    {
	      if(candRef->pt()<minNeutralPt_ || fabs(candRef->eta()) > maxNeutralEta_) continue;
	      if(vertexAssociationMasks_[iPFCand]!=-1) continue;

	      //associate neutrals using the jets
	      jIt=jetColl.begin();
	      for(;jIt!=jItEnd; jIt++)
		{
		  if(jIt->pt()<minJetPt_ || fabs(jIt->eta()) > maxJetEta_) continue;
		  double dr = deltaR(jIt->eta(),jIt->phi(), candRef->eta(), candRef->phi());
		  if(dr>jetCone_) continue;

		  //association is done for the first jet only
		  vertexAssociationMasks_[iPFCand]  = iVtx;
		  vtxNeutralSum[iVtx]              += candRef->p4();
		  vtxNeutralSumEt[iVtx]            += candRef->pt();
		  break;
		}
	    }
	  else if(vertexAssociationMasks_[iPFCand]==int(iVtx))
	    {
	      vtxChSum[iVtx]   += candRef->p4();
	      vtxChSumEt[iVtx] += candRef->pt();
            }
          else if(vertexAssociationMasks_[iPFCand]!=int(iVtx))
            {
              //BetaCorrection --> Removing 50% of the energy of the charged inside the jets (assuming that there is twice more charged than neutral from PU inside this jet)
              jIt=jetColl.begin();
              for(;jIt!=jItEnd; jIt++)
                {

                  if(jIt->pt()<minJetPt_ || fabs(jIt->eta()) > maxJetEta_) continue;
                  double dr = deltaR(jIt->eta(),jIt->phi(), candRef->eta(), candRef->phi());
                  if(dr>jetCone_) continue;

                  //association is done for the first jet only
                  betaCorrectionSum[iVtx]              -= 0.5 * candRef->p4();
                  betaCorrectionSumEt[iVtx]            -= 0.5 * candRef->pt();
                  break;
                }
	    }
	}
      
      vtxPFMet[iVtx] = vtxChSum[iVtx]   + vtxNeutralSum[iVtx];
      vtxSumEt[iVtx] = vtxChSumEt[iVtx] + vtxNeutralSumEt[iVtx];
    }  

  //compute the pf-MET components
  // - check how many PF candidates we were able to associate
  int           nch(0),               nneut(0),                  nneutfwd(0),                  nneut2v4(0),                  nneut2v4ass(0),                  nneut2v4notass(0);
  int           nchnotass(0),         nchass(0);
  LorentzVector globalChSum(0,0,0,0), globalNeutralSum(0,0,0,0), globalNeutralFwdSum(0,0,0,0), globalNeutral2v4Sum(0,0,0,0), globalNeutral2v4assSum(0,0,0,0), globalNeutral2v4notassSum(0,0,0,0);
  double        globalChSumEt(0),     globalNeutralSumEt(0),     globalNeutralFwdSumEt(0),     globalNeutral2v4SumEt(0),     globalNeutral2v4assSumEt(0),     globalNeutral2v4notassSumEt(0);
  LorentzVector globalNonAssocNeutralFwdSum(0,0,0,0);
  double        globalNonAssocNeutralFwdSumEt(0);
  LorentzVector globalNonAssocNeutralFwd2Sum(0,0,0,0);
  double        globalNonAssocNeutralFwd2SumEt(0);
  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
    {
      reco::PFCandidateRef candptr(pfCandsH_,iPFCand);
      bool isAssociatedToVertex(vertexAssociationMasks_[iPFCand]>-1);      
      if(candptr->charge()== 0)
	{
	  nneut ++;
	  globalNeutralSum    += candptr->p4();
	  globalNeutralSumEt  += candptr->pt();
	  
	  if( fabs(candptr->eta())<2.4 )
	    {
      	      nneut2v4++;
	      globalNeutral2v4Sum += candptr->p4();
	      globalNeutral2v4SumEt += candptr->pt();
	      
	      if(isAssociatedToVertex)
		{
		  nneut2v4ass++;
		  globalNeutral2v4assSum += candptr->p4();
		  globalNeutral2v4assSumEt += candptr->pt();
		}
	      else
		{
		  nneut2v4notass++;
		  globalNeutral2v4notassSum += candptr->p4();
		  globalNeutral2v4notassSumEt += candptr->pt();
		}
	    }
	  else
	    {
	      nneutfwd++;
	      globalNeutralFwdSum += candptr->p4();
              globalNeutralFwdSumEt += candptr->pt();

	      if(vertexAssociationMasks_[iPFCand]==-1)
		{
		  globalNonAssocNeutralFwdSum += candptr->p4();
		  globalNonAssocNeutralFwdSumEt += candptr->pt();
		}

	    }

          if( fabs(candptr->eta())>2.2 && vertexAssociationMasks_[iPFCand]==-1 ){
                  globalNonAssocNeutralFwd2Sum += candptr->p4();
                  globalNonAssocNeutralFwd2SumEt += candptr->pt();
          }

	}
      else
	{
	  nch++;
	  globalChSum         += candptr->p4();
	  globalChSumEt       += candptr->pt();
	  if(isAssociatedToVertex) nchass++;
	  else                     nchnotass++; 
	}
    }
  LorentzVector globalPFMet(globalChSum+globalNeutralSum);
  double        globalSumEt(globalNeutralSumEt+globalChSumEt);
  LorentzVector globalFwdPFMet(globalNeutralFwdSum);
  double        globalFwdSumEt(globalNeutralFwdSumEt);
  LorentzVector global2v4PFMet(globalChSum+globalNeutral2v4Sum);
  double        global2v4SumEt(globalNeutral2v4SumEt+globalChSumEt);
  LorentzVector global2v4assPFMet(globalChSum+globalNeutral2v4assSum);
  double        global2v4assSumEt(globalNeutral2v4assSumEt+globalChSumEt);
  
  //compute the residual MET
  // - subtracting all associated to other vertices than the PV
  // - except if mibiasStudy is active where all that is associated to all vertices is removed
  size_t iVtxStart( minBiasMode_ ? 0 : 1 );
  LorentzVector resChSum(globalChSum),     resNeutralSum(globalNeutralSum),     res2v4NeutralSum(globalNeutral2v4Sum);
  double        resChSumEt(globalChSumEt), resNeutralSumEt(globalNeutralSumEt), res2v4NeutralSumEt(globalNeutral2v4SumEt);
  for(size_t iVtx=iVtxStart; iVtx<nVtx; iVtx++)
    {
      resChSum        -= vtxChSum[iVtx];
      resChSumEt      -= vtxChSumEt[iVtx];

      resNeutralSum   -= vtxNeutralSum[iVtx];
      resNeutralSumEt -= vtxNeutralSumEt[iVtx];

      res2v4NeutralSum   -= vtxNeutralSum[iVtx];
      res2v4NeutralSumEt -= vtxNeutralSumEt[iVtx];
    }
  LorentzVector resPFMet(resChSum+resNeutralSum);
  double        resSumEt(resChSumEt+resNeutralSumEt);
  LorentzVector res2v4PFMet(resChSum+res2v4NeutralSum);
  double        res2v4SumEt(resChSumEt+res2v4NeutralSumEt);
  LorentzVector resFwdPFMet(globalNeutralFwdSum);
  double        resFwdSumEt(globalNeutralFwdSumEt);

  // and finally put it in the event
  std::auto_ptr<reco::PFMET> assocPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> assocPfMetCorrectedPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> assocPfMetWithFwdPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> assocPfMetWithFwd2Ptr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> assocOtherVtxPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> globalPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> centralPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> cleanPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> trkPfMetPtr(new reco::PFMET);


  //check which candidates got associated
  std::vector<int> pvAssocCandidates;
  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
    {
      if(vertexAssociationMasks_[iPFCand]!=0) continue;
      pvAssocCandidates.push_back( iPFCand );
    }
  std::auto_ptr< std::vector<int> > pvAssocCandidatesPtr( new std::vector<int> );
  pvAssocCandidatesPtr->resize(pvAssocCandidates.size());
  for(size_t iPFCand=0; iPFCand<pvAssocCandidates.size(); iPFCand++) (*pvAssocCandidatesPtr)[iPFCand] = pvAssocCandidates[iPFCand];

  CommonMETData pfOutput;
  PFSpecificAlgo pf;
  edm::Handle<edm::View<reco::Candidate> > pfRecoCandsH;
  iEvent.getByLabel(collectionTag_, pfRecoCandsH); 
  pfOutput.mex = -globalPFMet.px();
  pfOutput.mey = -globalPFMet.py();
  pfOutput.mez = -globalPFMet.pz();
  pfOutput.met = globalPFMet.pt();
  pfOutput.sumet = globalSumEt;
  pfOutput.phi = atan2(-globalPFMet.py(),-globalPFMet.px());
  *globalPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex = -global2v4PFMet.px();
  pfOutput.mey = -global2v4PFMet.py();
  pfOutput.mez = -global2v4PFMet.pz();
  pfOutput.met = global2v4PFMet.pt();
  pfOutput.sumet = global2v4SumEt;
  pfOutput.phi = atan2(-global2v4PFMet.py(),-global2v4PFMet.px());
  *centralPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex = -vtxPFMet[0].px();
  pfOutput.mey = -vtxPFMet[0].py();
  pfOutput.mez = -vtxPFMet[0].pz();
  pfOutput.met = vtxPFMet[0].pt();
  pfOutput.sumet = vtxSumEt[0];
  pfOutput.phi = atan2(-vtxPFMet[0].py(),-vtxPFMet[0].px());
  *assocPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  LorentzVector vtxPFMetCorrected =   vtxChSum  [0] + vtxNeutralSum  [0];
   double       vtxPFMetCorrectedEt=  vtxChSumEt[0] + vtxNeutralSumEt[0];
  if(vtxNeutralSum[0].pt()>betaCorrectionSum[0].pt()){ vtxPFMetCorrected -= betaCorrectionSum[0];  vtxPFMetCorrectedEt -= betaCorrectionSumEt[0];}
  pfOutput.mex = -vtxPFMetCorrected.px();
  pfOutput.mey = -vtxPFMetCorrected.py();
  pfOutput.mez = -vtxPFMetCorrected.pz();
  pfOutput.met = vtxPFMetCorrected.pt();
  pfOutput.sumet = vtxPFMetCorrectedEt;
  pfOutput.phi = atan2(-vtxPFMetCorrected.py(),-vtxPFMetCorrected.px());
  *assocPfMetCorrectedPtr = pf.addInfo(pfRecoCandsH,pfOutput);


  LorentzVector assocPfMetWithFwd(vtxPFMet[0]+globalNonAssocNeutralFwdSum);
  double assocPfMetWithFwdSumEt(vtxSumEt[0]+globalNonAssocNeutralFwdSumEt);
  pfOutput.mex = -assocPfMetWithFwd.px();
  pfOutput.mey = -assocPfMetWithFwd.py();
  pfOutput.mez = -assocPfMetWithFwd.pz();
  pfOutput.met = assocPfMetWithFwd.pt();
  pfOutput.sumet = assocPfMetWithFwdSumEt;
  pfOutput.phi = atan2(-assocPfMetWithFwd.py(),-assocPfMetWithFwd.px());
  *assocPfMetWithFwdPtr = pf.addInfo(pfRecoCandsH,pfOutput);


  LorentzVector assocPfMetWithFwd2(vtxPFMet[0]+globalNonAssocNeutralFwd2Sum);
  double assocPfMetWithFwd2SumEt(vtxSumEt[0]+globalNonAssocNeutralFwd2SumEt);
  pfOutput.mex = -assocPfMetWithFwd2.px();
  pfOutput.mey = -assocPfMetWithFwd2.py();
  pfOutput.mez = -assocPfMetWithFwd2.pz();
  pfOutput.met = assocPfMetWithFwd2.pt();
  pfOutput.sumet = assocPfMetWithFwd2SumEt;
  pfOutput.phi = atan2(-assocPfMetWithFwd2.py(),-assocPfMetWithFwd2.px());
  *assocPfMetWithFwd2Ptr = pf.addInfo(pfRecoCandsH,pfOutput);



  pfOutput.mex=0;
  pfOutput.mey=0;
  pfOutput.mez=0;
  pfOutput.sumet=0;
  for(size_t ivtx=1; ivtx<nVtx; ivtx++)
    {
      pfOutput.mex -= vtxPFMet[ivtx].px();
      pfOutput.mey -= vtxPFMet[ivtx].py();
      pfOutput.mez -= vtxPFMet[ivtx].pz();
      pfOutput.sumet += vtxSumEt[ivtx];
    }
  pfOutput.met = sqrt(pow(pfOutput.mex,2)+pow(pfOutput.mey,2));
  pfOutput.phi = atan2(pfOutput.mey,pfOutput.mex);
  *assocOtherVtxPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);
  
  pfOutput.mex = -vtxChSum[0].px();
  pfOutput.mey = -vtxChSum[0].py();
  pfOutput.mez = -vtxChSum[0].pz();
  pfOutput.met = vtxChSum[0].pt();
  pfOutput.sumet = vtxChSumEt[0];
  pfOutput.phi = atan2(-vtxChSum[0].py(),-vtxChSum[0].px());
  *trkPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex = -resPFMet.px();
  pfOutput.mey = -resPFMet.py();
  pfOutput.mez = -resPFMet.pz();
  pfOutput.met = resPFMet.pt();
  pfOutput.sumet = resSumEt;
  pfOutput.phi = atan2(-resPFMet.py(),-resPFMet.px());
  *cleanPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  std::auto_ptr<std::vector<double> > globalPfMetSumsPtr(new std::vector<double> );
  globalPfMetSumsPtr->resize(12,0);
  (*globalPfMetSumsPtr)[0] = globalSumEt;
  (*globalPfMetSumsPtr)[1] = globalChSumEt;
  (*globalPfMetSumsPtr)[2] = globalNeutralSumEt;
  (*globalPfMetSumsPtr)[3] = global2v4SumEt;
  (*globalPfMetSumsPtr)[4] = globalChSumEt;
  (*globalPfMetSumsPtr)[5] = globalNeutral2v4SumEt;
  (*globalPfMetSumsPtr)[6] = vtxSumEt[0];
  (*globalPfMetSumsPtr)[7] = vtxChSumEt[0];
  (*globalPfMetSumsPtr)[8] = vtxNeutralSumEt[0];
  for(size_t ivtx=1; ivtx<nVtx; ivtx++)
    {
      (*globalPfMetSumsPtr)[9] += vtxSumEt[ivtx]; 
      (*globalPfMetSumsPtr)[10] += vtxChSumEt[ivtx]; 
      (*globalPfMetSumsPtr)[11] += vtxNeutralSumEt[ivtx]; 
    }
  
  iEvent.put(pvAssocCandidatesPtr,"pvAssocCandidates");
  iEvent.put(assocPfMetPtr,"assocPfMet");
  iEvent.put(assocPfMetCorrectedPtr,"assocPfMetCorrected");
  iEvent.put(assocPfMetWithFwdPtr,"assocPfMetWithFwd");
  iEvent.put(assocPfMetWithFwd2Ptr,"assocPfMetWithFwd2");
  iEvent.put(assocOtherVtxPfMetPtr,"assocOtherVtxPfMet");
  iEvent.put(globalPfMetPtr,"globalPfMet");
  iEvent.put(centralPfMetPtr,"centralPfMet");
  iEvent.put(trkPfMetPtr,"trkPfMet");
  iEvent.put(cleanPfMetPtr,"cleanPfMet");
  iEvent.put(globalPfMetSumsPtr,"globalPfMetSums");
}

//
ClusteredPFMetProducer::~ClusteredPFMetProducer() { }

//
void ClusteredPFMetProducer::beginJob() { }

//
void ClusteredPFMetProducer::endJob() { }


DEFINE_FWK_MODULE(ClusteredPFMetProducer);

//  LocalWords:  numberOfValidPixelHits
