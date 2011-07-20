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

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

#include "FWCore/Utilities/interface/Exception.h"


class HZZPFMetProducer : public edm::EDProducer 
{
public:
  explicit HZZPFMetProducer(const edm::ParameterSet&);
  ~HZZPFMetProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag collectionTag_;    
  edm::InputTag vertexTag_;
  edm::InputTag jetTag_;
  double dzCut_;
  double minNeutralPt_;
  double maxNeutralEta_;
  PFJetIDSelectionFunctor jetId_;
  double minJetPt_;
  double maxJetEta_;

};

//
HZZPFMetProducer::HZZPFMetProducer(const edm::ParameterSet& iConfig):
  collectionTag_(iConfig.getParameter<edm::InputTag>("collectionTag")),
  vertexTag_(iConfig.getParameter<edm::InputTag>("vertexTag")),
  jetTag_(iConfig.getParameter<edm::InputTag>("jetTag")),
  dzCut_(iConfig.getParameter<double>("dzCut")),
  minNeutralPt_(iConfig.getParameter<double>("minNeutralPt")),
  maxNeutralEta_(iConfig.getParameter<double>("maxNeutralEta")),
  jetId_(iConfig.getParameter<edm::ParameterSet>("jetId")),
  minJetPt_(iConfig.getParameter<double>("minJetPt")),
  maxJetEta_(iConfig.getParameter<double>("maxJetEta"))
{
  produces<edm::ValueMap<reco::PFMET> >("hzzPfMet");
  produces<edm::ValueMap<reco::PFMET> >("hzzTkMet");
  produces<edm::ValueMap<reco::PFMET> >("hzzPfMetNoPileup");
  produces<edm::ValueMap<reco::PFMET> >("hzzPfMetNoPileupJetNeutralVeto");
  produces<edm::ValueMap<reco::PFMET> >("hzzPfMetNoPileupClusteredNeutrals");
}

//
void HZZPFMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  //vertices
  edm::Handle<edm::View<reco::Vertex> > vtxH;
  iEvent.getByLabel(vertexTag_ ,vtxH);
  if( vtxH->size() == 0 ) 
    {
      throw cms::Exception("CorruptData") << "Weird that the vertex collection is empty, how is this possible?\n";
    }

  //jets
  edm::Handle<edm::View<pat::Jet> > hJet;
  iEvent.getByLabel(jetTag_, hJet);
  std::map<const reco::Vertex *, std::vector<const pat::Jet *> > selJets;
  std::vector<const pat::Jet *> jetContainer;
  for(size_t j=0;j<vtxH->size();++j)  selJets[&(vtxH->at(j))]=jetContainer;
  pat::strbitset hasId = jetId_.getBitTemplate();
  for(size_t iJet=0; iJet< hJet.product()->size(); ++iJet)
    {
      const pat::Jet *jet = &((*hJet)[iJet]);

      //kinematics
      double pt = jet->pt();
      double eta = jet->eta();
      if(pt<minJetPt_ || fabs(eta)>maxJetEta_) continue;

      //jet id
      hasId.set(false);
      if( !jetId_( *jet, hasId ) ) continue;

      //associate a vertex using the beta variable                                                                                                                                                                                          
      double betaMax(-1);
      const reco::Vertex *assocVtx=0;
      for(size_t j=0;j<vtxH->size();++j) 
	{
	  double beta=jet::fAssoc(jet,&(vtxH->at(j)));
	  if(beta<0) continue;
	  if(beta<betaMax) continue;
	  betaMax=beta;
	  assocVtx=&(vtxH->at(j));
	}

      //store jet in the associated vertex
      if(betaMax<0) continue;
      selJets[assocVtx].push_back(jet);
    }
  
  //pf candidates
  edm::Handle<edm::View<reco::Candidate> > pfCandsH;
  iEvent.getByLabel(collectionTag_,  pfCandsH);

  //prepare the MET results (one per vertex)
  std::vector<LorentzVector> p4s(vtxH->size(),LorentzVector(0,0,0,0)), tkP4s(p4s), p4sNoPileup(p4s), p4sNoPileupJetNeutralVeto(p4s), p4sClusteredPfMetNoPileup(p4s);
  std::vector<float> sumEts( vtxH->size(),0), sumTkEts(sumEts), sumEtsNoPileup(sumEts), sumEtsNoPileupJetNeutralVeto(sumEts), sumEtsClusteredPfMetNoPileup(sumEts);

  //loop over the pf candidates
  edm::View<reco::Candidate>::const_iterator itPF, endprobes = pfCandsH->end();
  for (itPF = pfCandsH->begin(); itPF != endprobes; ++itPF) 
    {

      //neutral case:
      if( itPF->charge() == 0 ) 
	{
	  for(size_t j=0;j<vtxH->size();++j) {
	    
	    p4s[j] += itPF->p4();
	    sumEts[j] += itPF->pt();
	    
	    if(  itPF->pt() > minNeutralPt_ && fabs(itPF->eta()) < maxNeutralEta_ ) 
	      {
		tkP4s[j] += itPF->p4();
		sumTkEts[j] += itPF->pt();
	    }
	    
	    p4sNoPileup[j] += itPF->p4();
	    sumEtsNoPileup[j] += itPF->pt();
	  }
	} 

      //charged case:
      else 
	{
	  //get charged position at vertex
	  float tempDz(10000);
	  if( dynamic_cast<const reco::RecoCandidate*>(&*itPF) ) 
	    {
	      tempDz =  dynamic_cast<const reco::RecoCandidate*>(&*itPF)->track().isNonnull() ?
		dynamic_cast<const reco::RecoCandidate*>(&*itPF)->track()->vz() :
		dynamic_cast<const reco::RecoCandidate*>(&*itPF)->gsfTrack()->vz() ;
	    } 
	  else if( dynamic_cast<const reco::PFCandidate*>(&*itPF) ) 
	    {
	      tempDz =  dynamic_cast<const reco::PFCandidate*>(&*itPF)->vz();
	    } 
	  else throw cms::Exception("InvalidData") << "Data pointed to by collectionTag must inherit from reco::RecoCandidate or reco::PFCandidate\n";
	  
	  //loop over vertices
	  float closeDz = fabs(tempDz - vtxH->at(0).z());
	  size_t iClose = 0;
	  for(size_t j=0;j<vtxH->size();++j) 
	    {
	      
	      p4s[j] += itPF->p4();
	      sumEts[j] += itPF->pt();
	      
	      if( fabs(tempDz - vtxH->at(j).z()) < closeDz ) 
		{
		  closeDz = fabs(tempDz - vtxH->at(j).z());
		  iClose = j;
		}
	    }

	  //associate to the best vertex if within reasonable values
	  if( fabs(tempDz - vtxH->at(iClose).z()) < dzCut_ ) 
	    {
	      tkP4s[iClose] += itPF->p4();
	      sumTkEts[iClose] += itPF->pt();
	      
	      p4sNoPileup[iClose] += itPF->p4();
	      sumEtsNoPileup[iClose] += itPF->pt();
	    }
        }
    }

  //case 2 for neutrals: use jets associated to vertex to veto/or add associated neutrals
  std::vector<LorentzVector> clusteredNeutralsp4s(vtxH->size(),LorentzVector(0,0,0,0));
  std::vector<float> sumClusteredNeutralsEts( vtxH->size(),0);
  for(size_t j=0;j<vtxH->size();++j)
    {
      const reco::Vertex *vtx=&(vtxH->at(j));
      if(selJets.find( vtx ) != selJets.end() )
	{
	  for(std::vector<const pat::Jet *>::iterator jit = selJets[vtx].begin();
	      jit != selJets[vtx].end();
	      jit++)
	    {
	      const std::vector<reco::PFCandidatePtr>  &assocPFCands = (*jit)->getPFConstituents();
	      for(std::vector<reco::PFCandidatePtr>::const_iterator pfCandIt = assocPFCands.begin();
		  pfCandIt != assocPFCands.end();
		  pfCandIt++)
		{
		  if( pfCandIt->isNull() ) continue;
		  if( pfCandIt->get()->charge() != 0 ) continue; 
		  clusteredNeutralsp4s[j]    += pfCandIt->get()->p4();
		  sumClusteredNeutralsEts[j] += pfCandIt->get()->pt();
		}
	    }
	}
    }

  for(size_t j=0;j<vtxH->size();++j)
    {
      //subtract neutrals clustered in jets associated to other vertices
      p4sNoPileupJetNeutralVeto[j]=p4sNoPileup[j];
      sumEtsNoPileupJetNeutralVeto[j]=sumEtsNoPileup[j];
      for(size_t i=0; i<vtxH->size();++i)
	{
	  if(i==j) continue;
	  p4sNoPileupJetNeutralVeto[j] -= clusteredNeutralsp4s[i];
	  sumEtsNoPileupJetNeutralVeto[j] -= sumClusteredNeutralsEts[j];
	}

      //use charged hadrons and clustered neutrals only 
      p4sClusteredPfMetNoPileup[j]=tkP4s[j]+clusteredNeutralsp4s[j];
      sumEtsClusteredPfMetNoPileup[j]=sumTkEts[j]+sumClusteredNeutralsEts[j];
    }
  



  //finish building costumized MET collections
  std::vector<reco::PFMET> pfMets, tkMets, pfMetsNoPileup, pfMetsNoPileupJetNeutralVeto, pfMetsNoPileupClusteredNeutrals;
  for(size_t i=0;i<tkP4s.size();++i) 
    {
      CommonMETData pfOutput;
      pfOutput.mex = -p4s[i].px();
      pfOutput.mey = -p4s[i].py();
      pfOutput.mez = -p4s[i].pz();
      pfOutput.met = p4s[i].pt();
      pfOutput.sumet = sumEts[i];
      pfOutput.phi = atan2(-p4s[i].py(),-p4s[i].px());
      PFSpecificAlgo pf;
      pfMets.push_back(pf.addInfo(pfCandsH,pfOutput));

      CommonMETData tkOutput;
      tkOutput.mex = -tkP4s[i].px();
      tkOutput.mey = -tkP4s[i].py();
      tkOutput.mez = -tkP4s[i].pz();
      tkOutput.met = tkP4s[i].pt();
      tkOutput.sumet = sumTkEts[i];
      tkOutput.phi = atan2(-tkP4s[i].py(),-tkP4s[i].px());
      PFSpecificAlgo tkpf;
      tkMets.push_back(tkpf.addInfo(pfCandsH,tkOutput));

      CommonMETData pfMetsNoPileupOutput;
      pfMetsNoPileupOutput.mex = -p4sNoPileup[i].px();
      pfMetsNoPileupOutput.mey = -p4sNoPileup[i].py();
      pfMetsNoPileupOutput.mez = -p4sNoPileup[i].pz();
      pfMetsNoPileupOutput.met = p4sNoPileup[i].pt();
      pfMetsNoPileupOutput.sumet = sumEtsNoPileup[i];
      pfMetsNoPileupOutput.phi = atan2(-p4sNoPileup[i].py(),-p4sNoPileup[i].px());
      PFSpecificAlgo pfNoPileup;
      pfMetsNoPileup.push_back(pfNoPileup.addInfo(pfCandsH,pfMetsNoPileupOutput));

      CommonMETData pfMetsNoPileupJetNeutralVetoOutput;
      pfMetsNoPileupJetNeutralVetoOutput.mex   = -p4sNoPileupJetNeutralVeto[i].px();
      pfMetsNoPileupJetNeutralVetoOutput.mey   = -p4sNoPileupJetNeutralVeto[i].py();
      pfMetsNoPileupJetNeutralVetoOutput.mez   = -p4sNoPileupJetNeutralVeto[i].pz();
      pfMetsNoPileupJetNeutralVetoOutput.met   = p4sNoPileupJetNeutralVeto[i].pt();
      pfMetsNoPileupJetNeutralVetoOutput.sumet = sumEtsNoPileupJetNeutralVeto[i];
      pfMetsNoPileupJetNeutralVetoOutput.phi   = atan2(-p4sNoPileupJetNeutralVeto[i].py(),-p4sNoPileupJetNeutralVeto[i].px());
      PFSpecificAlgo pfNoPileupJetNeutralVeto;
      pfMetsNoPileupJetNeutralVeto.push_back(pfNoPileupJetNeutralVeto.addInfo(pfCandsH,pfMetsNoPileupJetNeutralVetoOutput));
      
      CommonMETData pfMetsNoPileupClusteredNeutralsOutput;
      pfMetsNoPileupClusteredNeutralsOutput.mex   = -p4sClusteredPfMetNoPileup[i].px();
      pfMetsNoPileupClusteredNeutralsOutput.mey   = -p4sClusteredPfMetNoPileup[i].py();
      pfMetsNoPileupClusteredNeutralsOutput.mez   = -p4sClusteredPfMetNoPileup[i].pz();
      pfMetsNoPileupClusteredNeutralsOutput.met   = p4sClusteredPfMetNoPileup[i].pt();
      pfMetsNoPileupClusteredNeutralsOutput.sumet = sumEtsClusteredPfMetNoPileup[i];
      pfMetsNoPileupClusteredNeutralsOutput.phi   = atan2(-p4sClusteredPfMetNoPileup[i].py(),-p4sClusteredPfMetNoPileup[i].px());
      PFSpecificAlgo pfNoPileupclustered;
      pfMetsNoPileupClusteredNeutrals.push_back(pfNoPileupclustered.addInfo(pfCandsH,pfMetsNoPileupClusteredNeutralsOutput));
    }
  

  //Get original vertex collection
  edm::Handle<edm::View<reco::Vertex> > originalVtxH;
  iEvent.get( vtxH->refAt(0).id() , originalVtxH );

  // set up the ValueMap for adding to the event
  std::auto_ptr<edm::ValueMap<reco::PFMET> > pfMetMap(new edm::ValueMap<reco::PFMET>);
  std::auto_ptr<edm::ValueMap<reco::PFMET> > tkMetMap(new edm::ValueMap<reco::PFMET>);
  std::auto_ptr<edm::ValueMap<reco::PFMET> > pfMetNoPileupMap(new edm::ValueMap<reco::PFMET>);  
  std::auto_ptr<edm::ValueMap<reco::PFMET> > pfMetNoPileupJetNeutralVetoMap(new edm::ValueMap<reco::PFMET>);  
  std::auto_ptr<edm::ValueMap<reco::PFMET> > pfMetNoPileupClusteredNeutralsMap(new edm::ValueMap<reco::PFMET>);  
  std::vector<reco::PFMET> emptypfMets(originalVtxH->size(),reco::PFMET());
  std::vector<reco::PFMET> emptytkMets(originalVtxH->size(),reco::PFMET());
  std::vector<reco::PFMET> emptypfMetsNoPileup(originalVtxH->size(),reco::PFMET());
  std::vector<reco::PFMET> emptypfMetsNoPileupJetNeutralVeto(originalVtxH->size(),reco::PFMET());
  std::vector<reco::PFMET> emptypfMetsNoPileupClusteredNeutrals(originalVtxH->size(),reco::PFMET());

  //fill the value maps
  edm::ValueMap<reco::PFMET>::Filler pfmetfiller(*pfMetMap);
  pfmetfiller.insert(originalVtxH, emptypfMets.begin(), emptypfMets.end());
  pfmetfiller.fill();

  edm::ValueMap<reco::PFMET>::Filler tkfiller(*tkMetMap);
  tkfiller.insert(originalVtxH, emptytkMets.begin(), emptytkMets.end());
  tkfiller.fill();

  edm::ValueMap<reco::PFMET>::Filler pfmetNoPileupfiller(*pfMetNoPileupMap);
  pfmetNoPileupfiller.insert(originalVtxH, emptypfMetsNoPileup.begin(), emptypfMetsNoPileup.end());
  pfmetNoPileupfiller.fill();

  edm::ValueMap<reco::PFMET>::Filler pfmetNoPileupJetNeutralVetofiller(*pfMetNoPileupJetNeutralVetoMap);
  pfmetNoPileupJetNeutralVetofiller.insert(originalVtxH, emptypfMetsNoPileupJetNeutralVeto.begin(), emptypfMetsNoPileupJetNeutralVeto.end());
  pfmetNoPileupJetNeutralVetofiller.fill();

  edm::ValueMap<reco::PFMET>::Filler pfmetNoPileupclusteredneutralsfiller(*pfMetNoPileupClusteredNeutralsMap);
  pfmetNoPileupclusteredneutralsfiller.insert(originalVtxH, emptypfMetsNoPileupClusteredNeutrals.begin(), emptypfMetsNoPileupClusteredNeutrals.end());
  pfmetNoPileupclusteredneutralsfiller.fill();


  // add the the results to the value map
  for(size_t i=0;i<vtxH->size();++i) {
    for(size_t j=0;j<originalVtxH->size();++j) {
      if( vtxH->refAt(i) == originalVtxH->refAt(j) ) {
	
	(*pfMetMap)[originalVtxH->refAt(j)] = pfMets[i];
	(*tkMetMap)[originalVtxH->refAt(j)] = tkMets[i];
	(*pfMetNoPileupMap)[originalVtxH->refAt(j)] = pfMetsNoPileup[i];
	(*pfMetNoPileupJetNeutralVetoMap)[originalVtxH->refAt(j)] = pfMetsNoPileupJetNeutralVeto[i];
	(*pfMetNoPileupClusteredNeutralsMap)[originalVtxH->refAt(j)] = pfMetsNoPileupClusteredNeutrals[i];
      }
    }
  }
  
  // and finally put it in the event
  iEvent.put(pfMetMap,"hzzPfMet");
  iEvent.put(tkMetMap,"hzzTkMet");
  iEvent.put(pfMetNoPileupMap,"hzzPfMetNoPileup");
  iEvent.put(pfMetNoPileupJetNeutralVetoMap,"hzzPfMetNoPileupJetNeutralVeto");
  iEvent.put(pfMetNoPileupClusteredNeutralsMap,"hzzPfMetNoPileupClusteredNeutrals");
}

//
HZZPFMetProducer::~HZZPFMetProducer() { }

//
void HZZPFMetProducer::beginJob() { }
void HZZPFMetProducer::endJob() { } 


DEFINE_FWK_MODULE(HZZPFMetProducer);
