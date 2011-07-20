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
  produces<edm::ValueMap<reco::PFMET> >("hzzClusteredPfMetNoPileup");
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
  std::vector<const pat::Jet *> selJets;
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
      selJets.push_back(jet);
    }
  
  //pf candidates
  edm::Handle<edm::View<reco::Candidate> > pfCandsH;
  iEvent.getByLabel(collectionTag_,  pfCandsH);


  //prepare the MET results (one per vertex)
  std::vector<math::XYZTLorentzVector> p4s(vtxH->size(),math::XYZTLorentzVector(0,0,0,0)), tkP4s(p4s), p4sNoPileup(p4s), p4sNoPileupJetNeutralVeto(p4s), p4sClusteredPfMetNoPileup(p4s);
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
	  for(size_t j=1;j<vtxH->size();++j) 
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

  //finish building costumized MET collections
  std::vector<reco::PFMET> pfmets, tkmets, pfmetsnopileup;
  for(size_t i=0;i<tkP4s.size();++i) 
    {
      CommonMETData pfoutput;
      pfoutput.mex = -p4s[i].px();
      pfoutput.mey = -p4s[i].py();
      pfoutput.mez = -p4s[i].pz();
      pfoutput.met = p4s[i].pt();
      pfoutput.sumet = sumEts[i];
      pfoutput.phi = atan2(-p4s[i].py(),-p4s[i].px());
      PFSpecificAlgo pf;
      pfmets.push_back(pf.addInfo(pfCandsH,pfoutput));

      CommonMETData tkoutput;
      tkoutput.mex = -tkP4s[i].px();
      tkoutput.mey = -tkP4s[i].py();
      tkoutput.mez = -tkP4s[i].pz();
      tkoutput.met = tkP4s[i].pt();
      tkoutput.sumet = sumTkEts[i];
      tkoutput.phi = atan2(-tkP4s[i].py(),-tkP4s[i].px());
      PFSpecificAlgo tkpf;
      tkmets.push_back(tkpf.addInfo(pfCandsH,tkoutput));

      CommonMETData pfmetsnopileupoutput;
      pfmetsnopileupoutput.mex = -p4sNoPileup[i].px();
      pfmetsnopileupoutput.mey = -p4sNoPileup[i].py();
      pfmetsnopileupoutput.mez = -p4sNoPileup[i].pz();
      pfmetsnopileupoutput.met = p4sNoPileup[i].pt();
      pfmetsnopileupoutput.sumet = sumEtsNoPileup[i];
      pfmetsnopileupoutput.phi = atan2(-p4sNoPileup[i].py(),-p4sNoPileup[i].px());
      PFSpecificAlgo pfnopileup;
      pfmetsnopileup.push_back(pfnopileup.addInfo(pfCandsH,pfmetsnopileupoutput));
    }
  

  //Get original vertex collection
  edm::Handle<edm::View<reco::Vertex> > originalVtxH;
  iEvent.get( vtxH->refAt(0).id() , originalVtxH );

  // set up the ValueMap for adding to the event
  std::auto_ptr<edm::ValueMap<reco::PFMET> > pfMetMap(new edm::ValueMap<reco::PFMET>);
  std::auto_ptr<edm::ValueMap<reco::PFMET> > tkMetMap(new edm::ValueMap<reco::PFMET>);
  std::auto_ptr<edm::ValueMap<reco::PFMET> > pfMetNoPileupMap(new edm::ValueMap<reco::PFMET>);  
  std::vector<reco::PFMET> emptypfmets(originalVtxH->size(),reco::PFMET());
  std::vector<reco::PFMET> emptytkmets(originalVtxH->size(),reco::PFMET());
  std::vector<reco::PFMET> emptypfmetsnopileup(originalVtxH->size(),reco::PFMET());

  //fill the value maps
  edm::ValueMap<reco::PFMET>::Filler pfmetfiller(*pfMetMap);
  pfmetfiller.insert(originalVtxH, emptypfmets.begin(), emptypfmets.end());
  pfmetfiller.fill();

  edm::ValueMap<reco::PFMET>::Filler tkfiller(*tkMetMap);
  tkfiller.insert(originalVtxH, emptytkmets.begin(), emptytkmets.end());
  tkfiller.fill();

  edm::ValueMap<reco::PFMET>::Filler pfmetnopileupfiller(*pfMetNoPileupMap);
  pfmetnopileupfiller.insert(originalVtxH, emptypfmetsnopileup.begin(), emptypfmetsnopileup.end());
  pfmetnopileupfiller.fill();

  // add the the results to the value map
  std::cout << "**********" << std::endl;
  for(size_t i=0;i<vtxH->size();++i) {
    for(size_t j=0;j<originalVtxH->size();++j) {
      if( vtxH->refAt(i) == originalVtxH->refAt(j) ) {
	
	(*pfMetMap)[originalVtxH->refAt(j)] = pfmets[i];
	(*tkMetMap)[originalVtxH->refAt(j)] = tkmets[i];
	(*pfMetNoPileupMap)[originalVtxH->refAt(j)] = pfmetsnopileup[i];
	
	std::cout << originalVtxH->refAt(j)->p4().pt() << " " << pfmets[i].pt() << " " << tkmets[i].pt() << " " << pfmetsnopileup[i].pt() << std::endl;
      }
    }
  }
 
  // and finally put it in the event
  iEvent.put(pfMetMap,"hzzPfMet");
  iEvent.put(tkMetMap,"hzzTkMet");
  iEvent.put(pfMetNoPileupMap,"hzzPfMetNoPileup");

}

//
HZZPFMetProducer::~HZZPFMetProducer() { }

//
void HZZPFMetProducer::beginJob() { }
void HZZPFMetProducer::endJob() { } 


DEFINE_FWK_MODULE(HZZPFMetProducer);
