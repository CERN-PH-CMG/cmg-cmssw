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
  
  TString dilcats[]={"ee","mumu","emu"};
  size_t ndilcats=sizeof(dilcats)/sizeof(TString);
  for(size_t icat=0; icat<ndilcats; icat++)
    {
      TFileDirectory newDir=fs->mkdir(dilcats[icat].Data());
      controlHistos_[dilcats[icat]+"_dz"] = (TH1D *) formatPlot( newDir.make<TH1F>(dilcats[icat]+"_dz", "; #Delta Z; Events", 100, -5.,5.), 1,1,1,20,0,false,true,1,1,1 );
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
  
  //get the weight for the MC event
  float weight=1;
  if(!iEvent.isRealData())
    {
      edm::Handle<float> puWeightHandle;
      iEvent.getByLabel("puWeights","puWeight",puWeightHandle);
      if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
    }

  
  //pre-select vertices
  Handle<reco::VertexCollection> hVtx;
  iEvent.getByLabel(objConfig["Vertices"].getParameter<edm::InputTag>("source"), hVtx);  
  std::vector<reco::VertexRef> selVertices = vertex::filter(hVtx,objConfig["Vertices"]);
  std::vector<reco::VertexRef> primaryVertexHyps;
  if(selVertices.size()>0) selStep=1;

  //beam spot                                                                                                                                                                                                  
  edm::Handle<reco::BeamSpot> beamSpot;
  iEvent.getByLabel( objConfig["Vertices"].getParameter<edm::InputTag>("beamSpot"), beamSpot);
  
  //average energy density
  edm::Handle< double > rho;
  iEvent.getByLabel(edm::InputTag("kt6PFJets:rho"),rho);

  //select muons (id+very loose isolation)
  Handle<View<Candidate> > hMu; 
  iEvent.getByLabel(objConfig["Muons"].getParameter<edm::InputTag>("source"), hMu);
  CandidateWithVertexCollection selMuons = muon::filter(hMu, selVertices, *beamSpot, objConfig["Muons"]);

  //select electrons (id+conversion veto+very loose isolation)
  Handle<View<Candidate> > hEle; 
  iEvent.getByLabel(objConfig["Electrons"].getParameter<edm::InputTag>("source"), hEle);
  CandidateWithVertexCollection selElectrons = electron::filter(hEle, hMu, selVertices, *beamSpot, objConfig["Electrons"]);
  
  //build inclusive collection
  CandidateWithVertexCollection selLeptons = selMuons;
  selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
  if(selLeptons.size()>0) selStep=2;

  //build the dilepton (all tightly isolated leptons will be returned)
  if(selStep==2)
    {
      //control histos for leptons
      for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
	{
	  using namespace lepton;
	  int id = getLeptonId(selLeptons[ilep].first);
	  std::vector<double> isol=getLeptonIso(selLeptons[ilep].first,objConfig["Dileptons"].getParameter<double>("minPt"));
	  TString ptype(fabs(id)==ELECTRON ? "electron" : "muon");
	  controlHistos_[ptype+"_rho"]->Fill(*rho,weight);
	  controlHistos_[ptype+"_ecaliso"]->Fill(isol[ECAL_ISO],weight);
	  controlHistos_[ptype+"_hcaliso"]->Fill(isol[HCAL_ISO],weight);
	  controlHistos_[ptype+"_trackiso"]->Fill(isol[TRACKER_ISO],weight);
	  controlHistos_[ptype+"_caloiso"]->Fill(isol[ECAL_ISO]+isol[HCAL_ISO]+isol[TRACKER_ISO],weight);
	  controlHistos_[ptype+"_reliso"]->Fill(isol[REL_ISO],weight);
	}
      
      //search for dileptons
      CandidateWithVertexCollection isolLeptons;
      std::pair<CandidateWithVertex,CandidateWithVertex> dileptonWithVertex = dilepton::filter(selLeptons,
											       isolLeptons,
											       *rho,
											       objConfig["Dileptons"],
											       iSetup);
      selPath = dilepton::classify(dileptonWithVertex);
      if(selPath!= dilepton::UNKNOWN)
	{
	  std::vector<TString> dilCats;
	  if(selPath==dilepton::EE) dilCats.push_back("ee"); 
	  if(selPath==dilepton::EMU) dilCats.push_back("emu"); 
	  if(selPath==dilepton::MUMU) dilCats.push_back("mumu"); 

	  //add to the event
	  selStep=3;
	  hyp.add(dileptonWithVertex.first.first,"leg1");
	  primaryVertexHyps.push_back(dileptonWithVertex.first.second);
	  hyp.add(dileptonWithVertex.second.first,"leg2");
	  primaryVertexHyps.push_back(dileptonWithVertex.second.second);

	  //control histos
	  double dz =-1000;
	  if(dileptonWithVertex.first.second.isNonnull() && dileptonWithVertex.second.second.isNonnull())
	    dz=dileptonWithVertex.first.second->position().z()-dileptonWithVertex.second.second->position().z();
	  for(size_t icat=0; icat<dilCats.size(); icat++)
	    {
	      controlHistos_[dilCats[icat]+"_dz"]->Fill(dz,weight);
	    }
	  
	  //add the remaining isolated leptons now
	  for(CandidateWithVertexCollection::iterator lIt = isolLeptons.begin(); lIt != isolLeptons.end(); lIt++)
	    {
	      if(lIt->first.get()== dileptonWithVertex.first.first.get()) continue;
	      if(lIt->first.get()== dileptonWithVertex.second.first.get()) continue;

	      //check if lepton is associated to the same vertex as the dilepton
	      if(lIt->second.get()== dileptonWithVertex.first.second.get() 
		 || lIt->second.get() == dileptonWithVertex.second.second.get())
		hyp.add( lIt->first , fabs(lepton::getLeptonId(lIt->first))==lepton::MUON ? "muon" : "electron" );
	      else
		hyp.add( lIt->first , fabs(lepton::getLeptonId(lIt->first))==lepton::MUON ? "pumuon" : "puelectron" );
	      
	    }
  
	  //add also the jets
	  Handle<View<Candidate> > hJet; 
	  iEvent.getByLabel(objConfig["Jets"].getParameter<edm::InputTag>("source"), hJet);
	  CandidateWithVertexCollection selJets = jet::filter(hJet, isolLeptons, selVertices, objConfig["Jets"]);
	  CandidateWithVertexCollection assocJets, puJets;
	  jet::classifyJetsForDileptonEvent(selJets,dileptonWithVertex,assocJets,puJets,objConfig["Dileptons"].getParameter<double>("maxDz"));
	  for(CandidateWithVertexCollection::iterator jIt = assocJets.begin(); jIt != assocJets.end(); jIt++) hyp.add(jIt->first,"jet");
	  for(CandidateWithVertexCollection::iterator jIt = puJets.begin(); jIt != puJets.end(); jIt++) hyp.add(jIt->first,"pujet");
	  
	  //add the met
	  Handle<View<Candidate> > hMET; 
	  iEvent.getByLabel(objConfig["MET"].getParameter<edm::InputTag>("source"), hMET);
	  CandidatePtr met = hMET->ptrAt(0);
	  hyp.add(met, "met");
	}
    }
  
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

  auto_ptr<reco::VertexCollection> primVertices(new reco::VertexCollection() );
  for(std::vector<reco::VertexRef>::iterator vit=primaryVertexHyps.begin(); vit != primaryVertexHyps.end(); vit++)
    primVertices->push_back( *(vit->get()) );
  iEvent.put(primVertices,"selectedVertices");
}

//  
DEFINE_FWK_MODULE(DileptonPlusMETEventProducer);
