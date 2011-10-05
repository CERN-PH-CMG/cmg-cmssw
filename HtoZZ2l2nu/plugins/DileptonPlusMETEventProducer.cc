#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"
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
#include "DataFormats/Common/interface/MergeableCounter.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

#include "TH1D.h"
#include "TString.h"


 class DileptonPlusMETEventProducer : public edm::EDProducer {
 public:
   DileptonPlusMETEventProducer(const edm::ParameterSet &iConfig) ;
   virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;
   void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
 private:
   std::map<std::string, edm::ParameterSet > objConfig;
   TSelectionMonitor controlHistos_;
 };


 using namespace std;


 //
 DileptonPlusMETEventProducer::DileptonPlusMETEventProducer(const edm::ParameterSet &iConfig)
   : controlHistos_( iConfig.getParameter<std::string>("dtag") )
 {
   produces<std::vector<pat::EventHypothesis> >("selectedEvent");
   produces<reco::VertexCollection>("selectedVertices");
   produces<std::vector<int> >("selectionInfo");
   std::string objs[]={"Generator", "Trigger", "Vertices", "Photons", "Electrons", "LooseElectrons", "Muons", "LooseMuons", "Dileptons", "Jets", "MET" };
   for(size_t iobj=0; iobj<sizeof(objs)/sizeof(string); iobj++)
     objConfig[ objs[iobj] ] = iConfig.getParameter<edm::ParameterSet>( objs[iobj] );

   controlHistos_.addHistogram( "pt", ";p_{T}; Leptons", 100, 0.,200. );
   controlHistos_.addHistogram( "d0", ";d0;Leptons", 100, -0.1,0.1  );
   controlHistos_.addHistogram( "eta", ";#eta; Leptons", 100, -3.,3. );

   controlHistos_.addHistogram( "rho", "; #rho; Events", 100, 0.,10. );
   controlHistos_.addHistogram( "ecaliso", ";Photon isolation; Events", 100, 0.,10. );
   controlHistos_.addHistogram( "hcaliso", ";Neutral hadron isolation; Events", 100, 0.,10. );
   controlHistos_.addHistogram( "trackiso", ";Charged hadron Isolation; Events", 100, 0.,10. );
   controlHistos_.addHistogram( "absiso", ";Absolute isolation; Events", 100, 0.,20. );
   controlHistos_.addHistogram( "reliso", "; Isolation; Events", 100, 0.,10.);

   controlHistos_.addHistogram( "rhovsnpu", ";Pileup events;#rho; Events",25,0,25, 100, 0.,10. );
   controlHistos_.addHistogram( "ecalisovsnpu", ";Pileup events;Photon isolation; Events",25,0,25, 100, 0.,10. );
   controlHistos_.addHistogram( "hcalisovsnpu", ";Pileup events;Neutral hadron isolation; Events",25,0,25, 100, 0.,10. );
   controlHistos_.addHistogram( "trackisovsnpu", ";Pileup events;Charged hadron Isolation; Events",25,0,25, 100, 0.,10. );
   controlHistos_.addHistogram( "absisovsnpu", ";Pileup events;Absolute isolation; Events",25,0,25, 100, 0.,20. );
   controlHistos_.addHistogram( "relisovsnpu", ";Pileup events; Isolation; Events",25,0,25, 100, 0.,10.);

   controlHistos_.initMonitorForStep( "matchedelectron");
   controlHistos_.initMonitorForStep( "electron");
   controlHistos_.initMonitorForStep( "matchedmuon");
   controlHistos_.initMonitorForStep( "muon");

   TString selsteps[]={"Reco","no scrap","#geq 1 vertex","HB/HE noise","No beam halo"};
   const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
   controlHistos_.addHistogram("cutflow", ";Steps; Events", nselsteps, 0.,nselsteps);
   TH1 *h=controlHistos_.getHisto("cutflow");
   for(size_t istep=0; istep<nselsteps; istep++) h->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
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

  //trigger
  edm::Handle<edm::TriggerResults> triggerBitsH;
  edm::InputTag trigSource = objConfig["Trigger"].getParameter<edm::InputTag>("source");
  iEvent.getByLabel( trigSource, triggerBitsH);
  const edm::TriggerNames &triggerNames = iEvent.triggerNames( *triggerBitsH );
   
  //get the weight for the MC event
  float weight=1;
  float nITpu(0);
  if(!iEvent.isRealData())
    {
      edm::Handle<float> puWeightHandle;
      iEvent.getByLabel("puWeights","puWeight",puWeightHandle);
      if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());

      edm::Handle<int> nITpuHandle;
      iEvent.getByLabel("puWeights","nITpu",nITpuHandle);
      if(nITpuHandle.isValid()) nITpu = *(nITpuHandle.product());
    }
   
  std::vector<const reco::Candidate *> trigMatchMu,trigMatchEle;
  trigger::getLeadDileptonTriggerCandidates(iEvent,objConfig["Trigger"],trigMatchMu,trigMatchEle);
  std::vector<std::string> gammaTriggers=objConfig["Trigger"].getParameter< std::vector<std::string> >("gammaTriggers");
  std::pair<std::string,double> photonTrig = photon::getPhotonTrigThreshold( triggerBitsH, triggerNames , gammaTriggers );
   
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
  iEvent.getByLabel( objConfig["Jets"].getParameter<edm::InputTag>("rho"),rho);

  //select photons (id+tight isolation)
  CandidateWithVertexCollection isolPhotonsWithVertex;
  CandidateCollection  isolPhotons;
  try{
    edm::Handle<edm::View<reco::Candidate> > hPhoton;
    iEvent.getByLabel( objConfig["Photons"].getParameter<edm::InputTag>("source"), hPhoton );
    edm::Handle<EcalRecHitCollection> ebrechits;
    iEvent.getByLabel( objConfig["Photons"].getParameter<edm::InputTag>("ebrechits"), ebrechits );
    EcalClusterLazyTools lazyTool(iEvent,iSetup,
				  objConfig["Photons"].getParameter<edm::InputTag>("ebrechits"),
				  objConfig["Photons"].getParameter<edm::InputTag>("eerechits"));
    isolPhotons=photon::filter(hPhoton,lazyTool,ebrechits,objConfig["Photons"],20);
    for(CandidateCollection::iterator pIt = isolPhotons.begin(); pIt != isolPhotons.end(); pIt++) isolPhotonsWithVertex.push_back( CandidateWithVertex(*pIt, selVertices[0]) );
  }catch(std::exception &e){
  }

  //select muons (id+very loose isolation)
  Handle<View<Candidate> > hMu; 
  iEvent.getByLabel(objConfig["Muons"].getParameter<edm::InputTag>("source"), hMu);
  CandidateWithVertexCollection selLooseMuons = muon::filter(hMu, selVertices, *beamSpot, objConfig["LooseMuons"]);
  CandidateWithVertexCollection selMuons = muon::filter(hMu, selVertices, *beamSpot, objConfig["Muons"]);
  
  for(size_t iMuon=0; iMuon< hMu.product()->size(); ++iMuon)
    {
      using namespace lepton;
      reco::CandidatePtr muonPtr = hMu->ptrAt(iMuon);
      std::vector<double> isol=getLeptonIso(muonPtr,objConfig["LooseMuons"].getParameter<double>("minPt"));

      const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muonPtr.get() );
      if( !muon->isTrackerMuon() || !muon->isGlobalMuon() ) continue;
      
      std::vector<TString> catsToFill;
      catsToFill.push_back("all"); 
      catsToFill.push_back("muon");
      const reco::GenParticle *genMu=muon->genLepton();
      if(genMu && fabs(genMu->pdgId())==13) catsToFill.push_back("matchedmuon");
      for(std::vector<TString>::iterator cIt = catsToFill.begin(); cIt != catsToFill.end(); cIt++)
	{
	  controlHistos_.fillHisto("pt",*cIt,muon->pt(),weight);
	  controlHistos_.fillHisto("eta",*cIt,muon->eta(),weight);
	  controlHistos_.fillHisto("d0",*cIt,muon->innerTrack()->dxy(beamSpot->position()),weight);

	  controlHistos_.fillHisto("rho",*cIt,*rho,weight);
	  controlHistos_.fillHisto("ecaliso",*cIt,isol[ECAL_ISO],weight);
	  controlHistos_.fillHisto("hcaliso",*cIt,isol[HCAL_ISO],weight);
	  controlHistos_.fillHisto("trackiso",*cIt,isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("absiso",*cIt,isol[ECAL_ISO]+isol[HCAL_ISO]+isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("reliso",*cIt,isol[REL_ISO],weight);

	  controlHistos_.fillHisto("rhovsnpu",*cIt,nITpu,*rho,weight);
	  controlHistos_.fillHisto("ecalisovsnpu",*cIt,nITpu,isol[ECAL_ISO],weight);
	  controlHistos_.fillHisto("hcalisovsnpu",*cIt,nITpu,isol[HCAL_ISO],weight);
	  controlHistos_.fillHisto("trackisovsnpu",*cIt,nITpu,isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("absisovsnpu",*cIt,nITpu,isol[ECAL_ISO]+isol[HCAL_ISO]+isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("relisovsnpu",*cIt,nITpu,isol[REL_ISO],weight);
	}
    }

  //select electrons (id+conversion veto+very loose isolation)
  Handle<View<Candidate> > hEle; 
  iEvent.getByLabel(objConfig["Electrons"].getParameter<edm::InputTag>("source"), hEle);
  CandidateWithVertexCollection selLooseElectrons = electron::filter(hEle, hMu, selVertices, *beamSpot, objConfig["LooseElectrons"]);
  CandidateWithVertexCollection selElectrons = electron::filter(hEle, hMu, selVertices, *beamSpot, objConfig["Electrons"]);
  for(size_t iEle=0; iEle< hEle.product()->size(); ++iEle)
    {
      reco::CandidatePtr elePtr = hEle->ptrAt(iEle);
      using namespace lepton;
      std::vector<double> isol=getLeptonIso(elePtr,objConfig["LooseElectrons"].getParameter<double>("minPt"));

      const pat::Electron *ele = dynamic_cast<const pat::Electron *>( elePtr.get() );
      const reco::GsfTrackRef & eTrack = ele->gsfTrack();
      if(eTrack.isNull()) continue;

      std::vector<TString> catsToFill;
      catsToFill.push_back("all");
      catsToFill.push_back("electron");
      const reco::GenParticle *genEle=ele->genLepton();
      if(genEle && fabs(genEle->pdgId())==11) catsToFill.push_back("matchedelectron");
      for(std::vector<TString>::iterator cIt = catsToFill.begin(); cIt != catsToFill.end(); cIt++)
	{
	  controlHistos_.fillHisto("pt",*cIt,ele->pt(),weight);
	  controlHistos_.fillHisto("eta",*cIt,ele->eta(),weight);
	  controlHistos_.fillHisto("d0",*cIt,eTrack->dxy(beamSpot->position()),weight);

	  controlHistos_.fillHisto("rho",*cIt,*rho,weight);
	  controlHistos_.fillHisto("ecaliso",*cIt,isol[ECAL_ISO],weight);
	  controlHistos_.fillHisto("hcaliso",*cIt,isol[HCAL_ISO],weight);
	  controlHistos_.fillHisto("trackiso",*cIt,isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("absiso",*cIt,isol[ECAL_ISO]+isol[HCAL_ISO]+isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("reliso",*cIt,isol[REL_ISO],weight);

	  controlHistos_.fillHisto("rhovsnpu",*cIt,nITpu,*rho,weight);
	  controlHistos_.fillHisto("ecalisovsnpu",*cIt,nITpu,isol[ECAL_ISO],weight);
	  controlHistos_.fillHisto("hcalisovsnpu",*cIt,nITpu,isol[HCAL_ISO],weight);
	  controlHistos_.fillHisto("trackisovsnpu",*cIt,nITpu,isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("absisovsnpu",*cIt,nITpu,isol[ECAL_ISO]+isol[HCAL_ISO]+isol[TRACKER_ISO],weight);
	  controlHistos_.fillHisto("relisovsnpu",*cIt,nITpu,isol[REL_ISO],weight);
	}
    }
    
  //build inclusive lepton collection
  CandidateWithVertexCollection selLooseLeptons = selLooseMuons;
  selLooseLeptons.insert(selLooseLeptons.end(),selLooseElectrons.begin(),selLooseElectrons.end());
  CandidateWithVertexCollection selLeptons = selMuons;
  selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
  
  //build the dilepton (all tightly isolated leptons will be returned)
  auto_ptr<reco::VertexCollection> primVertices(new reco::VertexCollection() );
  if(selLeptons.size()>0)
    {
      selStep=2;

      //search for dileptons
      CandidateWithVertexCollection isolLeptons;
      std::pair<CandidateWithVertex,CandidateWithVertex> dileptonWithVertex = dilepton::filter(selLeptons, objConfig["Dileptons"], iSetup);
      selPath = dilepton::classify(dileptonWithVertex);
      if(selPath!= dilepton::UNKNOWN)
	{
	  std::vector<TString>        dilCats;
	  if(selPath==dilepton::EE)   dilCats.push_back("ee"); 
	  if(selPath==dilepton::EMU)  dilCats.push_back("emu"); 
	  if(selPath==dilepton::MUMU) dilCats.push_back("mumu"); 

	  //add to the event
	  selStep=3;
	  hyp.add(dileptonWithVertex.first.first,"leg1");
	  primaryVertexHyps.push_back(dileptonWithVertex.first.second);
	  hyp.add(dileptonWithVertex.second.first,"leg2");
	  primaryVertexHyps.push_back(dileptonWithVertex.second.second);
    	
	  //add the remaining loose leptons now
	  for(CandidateWithVertexCollection::iterator lIt = selLooseLeptons.begin(); lIt != selLooseLeptons.end(); lIt++)
	    {
	      //veto if selected for the dilepton
	      if(lIt->first.get() == dileptonWithVertex.first.first.get()) continue;
	      if(lIt->first.get() == dileptonWithVertex.second.first.get()) continue;
	      hyp.add( lIt->first , fabs(lepton::getLeptonId(lIt->first))==lepton::MUON ? "muon" : "electron" );
	    }
	  
	  //add also the jets
	  Handle<View<Candidate> > hJet; 
	  iEvent.getByLabel(objConfig["Jets"].getParameter<edm::InputTag>("source"), hJet);
	  CandidateWithVertexCollection selJets = jet::filter(hJet, isolLeptons, selVertices, objConfig["Jets"]);
	  for(CandidateWithVertexCollection::iterator jIt = selJets.begin(); jIt != selJets.end(); jIt++) hyp.add(jIt->first,"jet");
	}
      
      //the primary vertex assigned from the dilepton
      for(std::vector<reco::VertexRef>::iterator vit=primaryVertexHyps.begin(); vit != primaryVertexHyps.end(); vit++) primVertices->push_back( *(vit->get()) );
    }
  
  //a gamma+jets event
  else if(selLooseLeptons.size()==0 && selLeptons.size()==0 && isolPhotons.size()>0) 
    { 
      selStep=3;
      selPath=dilepton::GAMMA+1000*photonTrig.second; 
      
      //add also the jets                                                                                                                                                                                                                                                                                              
      Handle<View<Candidate> > hJet;
      iEvent.getByLabel(objConfig["Jets"].getParameter<edm::InputTag>("source"), hJet);
      CandidateWithVertexCollection selJets = jet::filter(hJet, isolPhotonsWithVertex, selVertices, objConfig["Jets"]);
      for(CandidateWithVertexCollection::iterator jIt = selJets.begin(); jIt != selJets.end(); jIt++) hyp.add(jIt->first,"jet");
      
      //the leading vertex
      primVertices->push_back( *(selVertices[0].get()) );
    }

  //add the photons
  for(CandidateWithVertexCollection::iterator pIt = isolPhotonsWithVertex.begin(); pIt != isolPhotonsWithVertex.end(); pIt++)   hyp.add( pIt->first , "photon" );
  
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
  
      for(std::map<std::string,std::vector<CandidatePtr> >::iterator it = genEvent.begin();  it != genEvent.end();  it++)
	{
	  for(std::vector<CandidatePtr>::iterator itt = it->second.begin(); itt != it->second.end(); itt++)
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

  iEvent.put(primVertices,"selectedVertices");
}


//
void DileptonPlusMETEventProducer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  TString filterCtrs[]={"preSelectionCounter","noScrapCounter","goodVertexCounter","noHBHEnoiseCounter","nobeamHaloCounter"};
  const size_t nselFilters=sizeof(filterCtrs)/sizeof(TString);
  for(size_t istep=0; istep<nselFilters; istep++)
    {
      std::string fname(filterCtrs[istep].Data());
      try{
	edm::Handle<edm::MergeableCounter> ctrHandle;
	iLumi.getByLabel(fname, ctrHandle);
	if(!ctrHandle.isValid()) continue;
	controlHistos_.fillHisto("cutflow","all",istep,0.,ctrHandle->value);
      }catch(std::exception){
      }
    }
}


//  
DEFINE_FWK_MODULE(DileptonPlusMETEventProducer);
