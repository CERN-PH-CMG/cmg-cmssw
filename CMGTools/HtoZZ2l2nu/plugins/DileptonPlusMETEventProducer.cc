#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/EventHypothesisLooper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

#include "TH1D.h"
#include "TString.h"
 

class DileptonPlusMETEventProducer : public edm::EDProducer {
public:
  DileptonPlusMETEventProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;
private:
  std::map<std::string, edm::ParameterSet > objConfig;
  std::map<TString,TH1D *> controlHistos_;
};


using namespace std;


//
DileptonPlusMETEventProducer::DileptonPlusMETEventProducer(const edm::ParameterSet &iConfig)
{
  produces<std::vector<pat::EventHypothesis> >("selectedEvent");
  produces<reco::VertexCollection>("selectedVertices");
  produces<std::vector<int> >("selectionInfo");
  std::string objs[]={"Generator", "Vertices", "Electrons", "Muons", "Dileptons", "Jets", "MET" };
  for(size_t iobj=0; iobj<sizeof(objs)/sizeof(string); iobj++)
    objConfig[ objs[iobj] ] = iConfig.getParameter<edm::ParameterSet>( objs[iobj] );

  edm::Service<TFileService> fs;
  TString cats[]={"electron","muon"};
  size_t ncats=sizeof(cats)/sizeof(TString);
  for(size_t icat=0; icat<ncats; icat++)
    {
      TFileDirectory newDir=fs->mkdir(cats[icat].Data());
      controlHistos_[cats[icat]+"_rho"] = (TH1D *) formatPlot( newDir.make<TH1F>(cats[icat]+"_rho", "; #rho; Events", 100, 0.,10.), 1,1,1,20,0,false,true,1,1,1 );
      controlHistos_[cats[icat]+"_ecaliso"] = (TH1D *) formatPlot( newDir.make<TH1F>(cats[icat]+"_ecaliso", ";ECAL isolation; Events", 100, 0.,10.), 1,1,1,20,0,false,true,1,1,1 );
      controlHistos_[cats[icat]+"_hcaliso"] = (TH1D *) formatPlot( newDir.make<TH1F>(cats[icat]+"_hcaliso", ";HCAL isolation; Events", 100, 0.,10.), 1,1,1,20,0,false,true,1,1,1 );
      controlHistos_[cats[icat]+"_caloiso"] = (TH1D *) formatPlot( newDir.make<TH1F>(cats[icat]+"_caloiso", ";Calorimeter isolation; Events", 100, 0.,10.), 1,1,1,20,0,false,true,1,1,1 );
      controlHistos_[cats[icat]+"_trackiso"] = (TH1D *) formatPlot( newDir.make<TH1F>(cats[icat]+"_trackiso", ";Tracker Isolation; Events", 100, 0.,10.), 1,1,1,20,0,false,true,1,1,1 );
      controlHistos_[cats[icat]+"_reliso"] = (TH1D *) formatPlot( newDir.make<TH1F>(cats[icat]+"_reliso", "; Isolation; Events", 100, 0.,10.), 1,1,1,20,0,false,true,1,1,1 );
    }
}

//
void DileptonPlusMETEventProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;
  using namespace pat::eventhypothesis;
  using reco::Candidate; 
  using reco::CandidatePtr;

  
  pat::EventHypothesis hyp;
  int selStep(0),selPath(0);
  
  //pre-select vertices
  Handle<reco::VertexCollection> hVtx;
  iEvent.getByLabel(objConfig["Vertices"].getParameter<edm::InputTag>("source"), hVtx);  
  std::vector<reco::VertexRef> selVertices = vertex::filter(hVtx,objConfig["Vertices"]);
  const reco::Vertex *theSelVertex=0;
  if(selVertices.size()>0) selStep=1;

  //average energy density
  edm::Handle< double > rho;
  iEvent.getByLabel(edm::InputTag("kt6PFJets:rho"),rho);

  //select muons (id+very loose isolation)
  Handle<View<Candidate> > hMu; 
  iEvent.getByLabel(objConfig["Muons"].getParameter<edm::InputTag>("source"), hMu);
  std::vector<CandidatePtr> selMuons = muon::filter(hMu, objConfig["Muons"]);

  //select electrons (id+conversion veto+very loose isolation)
  Handle<View<Candidate> > hEle; 
  iEvent.getByLabel(objConfig["Electrons"].getParameter<edm::InputTag>("source"), hEle);
  std::vector<CandidatePtr> selElectrons = electron::filter(hEle, hMu, objConfig["Electrons"]);
  
  //build inclusive collection
  std::vector<CandidatePtr> selLeptons = selMuons;
  selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
  if(selLeptons.size()>0) selStep=2;

  //build the dilepton (all tightly isolated leptons will be returned)
  std::vector<CandidatePtr> isolLeptons;
  if(selVertices.size())
    {
      std::pair<reco::VertexRef, std::vector<CandidatePtr> > dileptonWithVertex = dilepton::filter(selLeptons,
												   selVertices,
												   objConfig["Dileptons"],
												   iSetup,
												   *rho,
												   isolLeptons,
												   &controlHistos_);
      selPath = dilepton::classify(dileptonWithVertex.second);
      if(selPath>0)
	{
	  selStep=3;
	  
	  std::vector<CandidatePtr> &dilepton = dileptonWithVertex.second;
	  hyp.add(dilepton[0],"leg1");
	  hyp.add(dilepton[1],"leg2");
	  
	  //add the remaining isolated leptons now
	  for(std::vector<CandidatePtr>::iterator lIt = isolLeptons.begin(); lIt != isolLeptons.end(); lIt++)
	    {
	      if(lIt->get()== dilepton[0].get() || lIt->get() == dilepton[1].get()) continue;
	      hyp.add( *lIt , fabs(dilepton::getLeptonId(*lIt))==13 ? "muon" : "electron" );
	    }
	  theSelVertex = dileptonWithVertex.first.get();
	}
    }
  
  //add the jets
  Handle<View<Candidate> > hJet; 
  iEvent.getByLabel(objConfig["Jets"].getParameter<edm::InputTag>("source"), hJet);
  std::vector<CandidatePtr> selJets = jet::filter(hJet, isolLeptons, objConfig["Jets"]);
  for(std::vector<CandidatePtr>::iterator jIt = selJets.begin(); jIt != selJets.end(); jIt++) hyp.add(*jIt,"jet");

  //add the met
  Handle<View<Candidate> > hMET; 
  iEvent.getByLabel(objConfig["MET"].getParameter<edm::InputTag>("source"), hMET);
  CandidatePtr met = hMET->ptrAt(0);
  hyp.add(met, "met");

  //if event is MC filter out the genparticle collection also
  if(!iEvent.isRealData())
    {
      Handle<View<Candidate> > hGen;
      iEvent.getByLabel(objConfig["Generator"].getParameter<edm::InputTag>("source"), hGen);
      std::map<std::string,std::vector<CandidatePtr> > genEvent = gen::filter(hGen, objConfig["Generator"]);
      for(std::map<std::string,std::vector<CandidatePtr> >::iterator it = genEvent.begin();
	  it != genEvent.end();
	  it++)
	{
	  for(std::vector<CandidatePtr>::iterator itt = it->second.begin();
	      itt != it->second.end();
	      itt++)
	      hyp.add( *itt, it->first );
	}
    }
      
  // work done, save results
  auto_ptr<std::vector<pat::EventHypothesis> > hyps(new std::vector<pat::EventHypothesis>() );
  hyps->push_back(hyp);
  iEvent.put(hyps,"selectedEvent");

  auto_ptr<std::vector<int> > selectionInfo(new std::vector<int>());
  selectionInfo->push_back( selPath );
  selectionInfo->push_back( selStep );
  iEvent.put(selectionInfo,"selectionInfo");

  auto_ptr<reco::VertexCollection> selVertex(new reco::VertexCollection() );
  if(theSelVertex)  selVertex->push_back( *theSelVertex );   
  iEvent.put(selVertex,"selectedVertices");
}

//  
DEFINE_FWK_MODULE(DileptonPlusMETEventProducer);
