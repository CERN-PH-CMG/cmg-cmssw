#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/EventHypothesisLooper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "SHarper/HEEPAnalyzer/interface/HEEPCutCodes.h"

#include "CMGTools/External/interface/PileupJetIdAlgo.h"

using namespace std;
using namespace edm;
using namespace reco;

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:

  void saveMCtruth(const edm::Event &event, const edm::EventSetup &iSetup );


  inline int getLeptonIsoSummary(reco::CandidatePtr l ,std::vector<double> &leptoniso, bool isTight)
  {
    int id = fabs(getLeptonId(l));
    
    double maxRelIso = objConfig_[id==ELECTRON ? "Electrons" : "Muons"].getParameter<double>("maxRelIso");
    if(!isTight) maxRelIso = objConfig_[id==ELECTRON ? "LooseElectrons" : "LooseMuons"].getParameter<double>("maxRelIso");
    
    int isoSummary(0);
    isoSummary = (leptoniso[REL_ISO] < maxRelIso) 
      | ((leptoniso[RELRHOCORR_ISO] < maxRelIso) << 1)
      | ((leptoniso[PFREL_ISO] < maxRelIso) << 2)  
      | ((leptoniso[PFRELBETCORR_ISO] < maxRelIso) << 3);

    return isoSummary;
  }


  inline int getLeptonPidSummary(reco::CandidatePtr l)
  {
    int idsummary(0);
    int lid = fabs(getLeptonId(l));
    if(lid==MUON) idsummary=getMuonPidSummary( dynamic_cast<const pat::Muon *>(l.get()) );
    else          idsummary=getElectronPidSummary( dynamic_cast<const pat::Electron *>(l.get()) );
    return idsummary;
  }
  int getMuonPidSummary(const pat::Muon *mu);
  int getElectronPidSummary(const pat::Electron *ele);

  std::map<std::string, edm::ParameterSet> objConfig_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
  EventCategory eventClassifComp_;
  PileupJetIdAlgo puJetIdAlgo_;

};

using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
  : controlHistos_( iConfig.getParameter<std::string>("dtag") ),
    puJetIdAlgo_(iConfig.getParameter<edm::ParameterSet>("Jets").getParameter<edm::ParameterSet>("puJetId"))
{
  try{

    std::string objs[]={"Generator", "Trigger", "Vertices", "Photons",
			"Electrons", "LooseElectrons", "Muons","LooseMuons", "SoftMuons", "Dileptons", "Jets", "AssocJets", "MET" };
    for(size_t iobj=0; iobj<sizeof(objs)/sizeof(string); iobj++)
      objConfig_[ objs[iobj] ] = iConfig.getParameter<edm::ParameterSet>( objs[iobj] );

    
    edm::Service<TFileService> fs;
    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    

    //cut flow histograms for book keeping
    TString selFilters[]={"Reco","no scrap","#geq 1 vertex","HB/HE noise","No beam halo"};
    const size_t nselFilters=sizeof(selFilters)/sizeof(TString);
    controlHistos_.addHistogram("cutflow", ";Steps; Events", nselFilters, 0.,nselFilters);
    TH1 *h = controlHistos_.getHisto("cutflow");
    for(size_t istep=0; istep<nselFilters; istep++) h->GetXaxis()->SetBinLabel(istep+1,selFilters[istep]);

    controlHistos_.addHistogram("pileup", ";Pileup; Events",50,-0.5,49.5);
    controlHistos_.addHistogram("pileuptrue", ";True pileup; Events",50,-0.5,49.5);
   
  }
  catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
void DileptonPlusMETEventAnalyzer::saveMCtruth(const edm::Event &event, const edm::EventSetup &iSetup)
{
  ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
  ev.nmcparticles=0;

  float weight(1.0);
  ev.puWeight = 1.0;
  if(event.isRealData())  return;



  //pileup
  edm::Handle<float> puWeightHandle;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("puReweight"), puWeightHandle );
  if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
  ev.puWeight = weight;

  edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
  event.getByType(puInfoH);
  int npuOOT(0),npuIT(0),npuOOTm1(0);
  float truePU(0);
  if(puInfoH.isValid())
    {
      for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	{
	  if(it->getBunchCrossing()==0) { npuIT += it->getPU_NumInteractions(); truePU = it->getTrueNumInteractions()/3.; }
	  else                          npuOOT += it->getPU_NumInteractions();
	  if(it->getBunchCrossing()<0)  npuOOTm1 += it->getPU_NumInteractions();

	}
    }
  ev.ngenITpu=npuIT;
  ev.ngenOOTpu=npuOOT;
  ev.ngenOOTpum1=npuOOTm1;
  controlHistos_.fillHisto("pileup","all",ev.ngenITpu);
  controlHistos_.fillHisto("pileuptrue","all",truePU);


  //retrieve pdf info
  edm::Handle<GenEventInfoProduct> genEventInfoProd;
  event.getByType( genEventInfoProd );
  if(genEventInfoProd.isValid())
    {
      ev.genWeight = genEventInfoProd->weight();
      ev.qscale = genEventInfoProd->qScale();
      if(genEventInfoProd->pdf())
	{
	  ev.x1  = genEventInfoProd->pdf()->x.first;
	  ev.x2  = genEventInfoProd->pdf()->x.second;
	  ev.id1 = genEventInfoProd->pdf()->id.first;
	  ev.id2 = genEventInfoProd->pdf()->id.second;
	}
      if(genEventInfoProd->binningValues().size()>0) ev.pthat = genEventInfoProd->binningValues()[0];
    }

  //Higgs pT reweighting (for Powheg gg->H) - to be filled a posteriori
  ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]=1;
  ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]=1;
  ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]=1;
  ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]=1;
  ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown]=1;

  //generator level event
  Handle<View<Candidate> > hGen;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("source"), hGen);
  std::vector<const reco::Candidate *> genEvent = getGeneratorEvent(hGen,objConfig_["Generator"]);
  ev.nmcparticles = 0;
  bool isHiggsEvent(false);
  for(size_t i=0; i<genEvent.size(); i++)
    {
      const reco::Candidate *genpart = genEvent[i];
      
      if(fabs(genpart->pdgId())==25) 
	{
	  isHiggsEvent=true;
	  ev.h_px=genpart->px();
	  ev.h_py=genpart->py();
	  ev.h_pz=genpart->pz();
	  ev.h_en=genpart->energy();
	}
      else
	{
	  ev.mc_px[ev.nmcparticles]=genpart->px();  
	  ev.mc_py[ev.nmcparticles]=genpart->py();  
	  ev.mc_pz[ev.nmcparticles]=genpart->pz(); 
	  ev.mc_en[ev.nmcparticles]=genpart->energy();  
	  ev.mc_id[ev.nmcparticles]=genpart->pdgId();
	  ev.nmcparticles++;
	}
    }
   
  //mc truth channel
  if(isHiggsEvent) ev.mccat=HIGGS;
  else             ev.mccat=assignPhysicsChannel(hGen);


  //add the generator level jets
  edm::Handle<edm::View<reco::Candidate> > genJetsH;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("genJets"), genJetsH );
//  if(!genJetsH.isValid())return;
  for(size_t ijet=0; ijet<genJetsH.product()->size(); ijet++){
      reco::CandidatePtr gjIt = genJetsH->ptrAt(ijet);
      if(gjIt->pt()<10) continue;
      
      //remove overlaps with leptons
      bool overlap(false);
      for(int imcpart=0; imcpart<ev.nmcparticles; imcpart++){
	  int id=fabs(ev.mc_id[imcpart]);
	  if(id!=11 && id!=13 && id!=15) continue;
	  LorentzVector p4(ev.mc_px[imcpart],ev.mc_py[imcpart],ev.mc_pz[imcpart],ev.mc_en[imcpart]);
	  double dr = deltaR(p4,gjIt->p4());
	  if(dr<0.4) overlap=true;
      }
      if(overlap) continue;
      
      ev.mc_px[ev.nmcparticles]=gjIt->px();  
      ev.mc_py[ev.nmcparticles]=gjIt->py();  
      ev.mc_pz[ev.nmcparticles]=gjIt->pz(); 
      ev.mc_en[ev.nmcparticles]=gjIt->energy();  
      ev.mc_id[ev.nmcparticles]=1;
      ev.nmcparticles++;
    }
}


//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) 
{
  try{
    //event summary to be filled
    summaryHandler_.resetStruct();

    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();

    //pfmet
    Handle<View<Candidate> > hMET;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("source"), hMET);
    CandidatePtr pfmet = hMET->ptrAt(0);
   
    //event header
    ev.run    = event.id().run();
    ev.lumi   = event.luminosityBlock();
    ev.event  = event.id().event();
    saveMCtruth(event, iSetup );    
    
    //
    // trigger (require at least for data)
    //
    edm::Handle<edm::TriggerResults> triggerBitsH;
    edm::InputTag trigSource = objConfig_["Trigger"].getParameter<edm::InputTag>("source");
    event.getByLabel( trigSource, triggerBitsH);
    const edm::TriggerNames &triggerNames = event.triggerNames( *triggerBitsH );
    std::vector<std::string> triggerPaths=objConfig_["Trigger"].getParameterSet("triggerPaths").getParameterNames();
    std::map<std::string,bool> triggerBits; 
    std::pair<std::string,double> photonTrig;
    ev.hasTrigger=false;
    for(size_t it=0; it<triggerPaths.size(); it++)
      {
	std::vector<std::string> itriggers=objConfig_["Trigger"].getParameterSet("triggerPaths").getParameter<std::vector<std::string> >( triggerPaths[it] );
	triggerBits[ triggerPaths[it] ] = checkIfTriggerFired( triggerBitsH, triggerNames,itriggers);
	ev.hasTrigger |= triggerBits[ triggerPaths[it] ];
	if(triggerPaths[it]!="gamma") continue;
	photonTrig = getHighestPhotonTrigThreshold( triggerBitsH, triggerNames , itriggers);
      }
	    
    //
    // vertex and beam spot
    //
    edm::Handle<reco::BeamSpot> beamSpot;
    event.getByLabel( objConfig_["Vertices"].getParameter<edm::InputTag>("beamSpot"), beamSpot);
    Handle<reco::VertexCollection> hVtx;
    event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
    std::vector<reco::VertexRef> selVertices = getGoodVertices(hVtx,objConfig_["Vertices"]);

    //quit if no good vertex
    if(selVertices.size()==0) return;

    reco::VertexRef primVertex = selVertices[0];
    ev.nvtx=selVertices.size();
    ev.vtx_px = primVertex->p4().px(); 
    ev.vtx_py = primVertex->p4().py(); 
    ev.vtx_pz = primVertex->p4().pz();  
    ev.vtx_en = primVertex->p4().energy();
    
    // average energy density
    edm::Handle< double > rho, rho25;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"),rho);
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("rho25"),rho25);
    ev.rho = *rho;
    ev.rho25 = *rho25;

    //
    // LEPTON SELECTION
    //
    
    //muon selection
    Handle<View<Candidate> > hMu;
    event.getByLabel(objConfig_["Muons"].getParameter<edm::InputTag>("source"), hMu);
    std::vector<CandidatePtr> selSoftMuons = getGoodMuons(hMu, primVertex, *rho, objConfig_["SoftMuons"]);
    std::vector<CandidatePtr> selLooseMuons = getGoodMuons(hMu, primVertex, *rho, objConfig_["LooseMuons"]);
    std::vector<CandidatePtr> selMuons      = getGoodMuons(hMu, primVertex, *rho, objConfig_["Muons"]);

    //electron selection
    Handle<View<Candidate> > hEle;
    event.getByLabel(objConfig_["Electrons"].getParameter<edm::InputTag>("source"), hEle);
    std::vector<CandidatePtr> selLooseElectrons = getGoodElectrons(hEle, hMu, primVertex, *rho, objConfig_["LooseElectrons"]);
    std::vector<CandidatePtr> selElectrons      = getGoodElectrons(hEle, hMu, primVertex, *rho, objConfig_["Electrons"]);

    //inclusive collection of leptons to store (include the soft muon selection)
    std::vector<CandidatePtr> selLooseLeptons = selLooseMuons;
    selLooseLeptons.insert(selLooseLeptons.end(),selLooseElectrons.begin(),selLooseElectrons.end());
    std::vector<CandidatePtr> selLeptons = selMuons;
    selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
    for(std::vector<CandidatePtr>::iterator smIt = selSoftMuons.begin(); smIt != selSoftMuons.end(); smIt++)
      {
	bool keep(true);
	for(std::vector<CandidatePtr>::iterator lmIt=selLooseMuons.begin(); lmIt != selLooseMuons.end(); lmIt++)
	  {
	    if(deltaR((*smIt)->eta(),(*smIt)->phi(),(*lmIt)->eta(),(*lmIt)->phi())>0.1) continue;
	    keep=false;
	    break;
	  }
	if(!keep) continue;
	selLooseLeptons.push_back(*smIt);
      }

    //dilepton candidate
    std::vector<CandidatePtr> dilepton = getDileptonCandidate(selLeptons, objConfig_["Dileptons"], iSetup);
    ev.cat = getDileptonId(dilepton);

    if(dilepton.size()==2)
      {
	//require trigger for each event category
	if(event.isRealData())
	  {
	    if(ev.cat==EE   && triggerBits["ee"]==false)    return;
	    if(ev.cat==MUMU && triggerBits["mumu"]==false) return;
	    if(ev.cat==EMU  && (triggerBits["emu"]==false || triggerBits["ee"]==true || triggerBits["mumu"]==true))  return;
	  }
	ev.hasTrigger=true;
	
	const reco::GenParticle *genLepton = getLeptonGenMatch(dilepton[0]);
	std::vector<double> leptoniso      = getLeptonIso(dilepton[0]);
	ev.l1_px    = dilepton[0]->px(); 
	ev.l1_py    = dilepton[0]->py(); 
	ev.l1_pz    = dilepton[0]->pz(); 
	ev.l1_en    = dilepton[0]->energy();  
	ev.l1_id    = getLeptonId(dilepton[0]);
	ev.l1_ptErr = getLeptonPtError(dilepton[0]); 
	ev.l1_genid = (genLepton==0? 0 : genLepton->pdgId());
	ev.l1_gIso  = leptoniso[G_ISO]; 
	ev.l1_nhIso  = leptoniso[N_ISO]; 
	ev.l1_chIso  = leptoniso[C_ISO];
	ev.l1_puchIso = leptoniso[CPU_ISO];
	ev.l1_ecalIso  = leptoniso[ECAL_ISO]; 
	ev.l1_hcalIso  = leptoniso[HCAL_ISO]; 
	ev.l1_trkIso  = leptoniso[TRACKER_ISO];
	ev.l1_passIso = getLeptonIsoSummary(dilepton[0], leptoniso, true);
	ev.l1_pid   = getLeptonPidSummary( dilepton[0]);

	genLepton = getLeptonGenMatch(dilepton[1]);
	leptoniso = getLeptonIso(dilepton[1]);
	ev.l2_px     = dilepton[1]->px(); 
	ev.l2_py     = dilepton[1]->py(); 
	ev.l2_pz     = dilepton[1]->pz(); 
	ev.l2_en     = dilepton[1]->energy();  
	ev.l2_id     = getLeptonId(dilepton[1]);
	ev.l2_genid  = (genLepton==0? 0 : genLepton->pdgId());
	ev.l2_ptErr  = getLeptonPtError(dilepton[1]);
	ev.l2_gIso  = leptoniso[G_ISO]; 
	ev.l2_nhIso  = leptoniso[N_ISO]; 
	ev.l2_chIso  = leptoniso[C_ISO];
	ev.l2_puchIso = leptoniso[CPU_ISO];
	ev.l2_ecalIso  = leptoniso[ECAL_ISO]; 
	ev.l2_hcalIso  = leptoniso[HCAL_ISO]; 
	ev.l2_trkIso  = leptoniso[TRACKER_ISO];
	ev.l2_passIso = getLeptonIsoSummary(dilepton[1], leptoniso, true);
	ev.l2_pid    = getLeptonPidSummary( dilepton[1]);
      }
    
    ev.ln=0;
    for (std::vector<CandidatePtr>::iterator lit= selLooseLeptons.begin(); lit!=selLooseLeptons.end(); lit++)
      {
	//do not consider if the lepton is building the dilepton
	if(dilepton.size()==2 )
	  {
	    if(
	       deltaR((*lit)->eta(),(*lit)->phi(),dilepton[0]->eta(),dilepton[0]->phi())<0.1 || 
	       deltaR((*lit)->eta(),(*lit)->phi(),dilepton[1]->eta(),dilepton[1]->phi())<0.1 
	       ) 
	      continue;
	  }
	
	const reco::GenParticle *genLepton = getLeptonGenMatch(*lit);
	std::vector<double> leptoniso      = getLeptonIso(*lit);
	ev.ln_px[ev.ln]    = (*lit)->px();
	ev.ln_py[ev.ln]    = (*lit)->py();
	ev.ln_pz[ev.ln]    = (*lit)->pz();
	ev.ln_en[ev.ln]    = (*lit)->energy();
	ev.ln_id[ev.ln]    = getLeptonId(*lit);
	ev.ln_genid[ev.ln] = (genLepton==0? 0 : genLepton->pdgId());
	ev.ln_ptErr[ev.ln] = getLeptonPtError(*lit);
	ev.ln_gIso[ev.ln]     = leptoniso[G_ISO]; 
	ev.ln_nhIso[ev.ln]    = leptoniso[N_ISO]; 
	ev.ln_chIso[ev.ln]    = leptoniso[C_ISO];
	ev.ln_puchIso[ev.ln]  = leptoniso[CPU_ISO];
	ev.ln_ecalIso[ev.ln]  = leptoniso[ECAL_ISO]; 
	ev.ln_hcalIso[ev.ln]  = leptoniso[HCAL_ISO]; 
	ev.ln_trkIso[ev.ln]   = leptoniso[TRACKER_ISO];
	ev.ln_passIso[ev.ln] = getLeptonIsoSummary(*lit, leptoniso, false);
	ev.ln_pid[ev.ln]   = getLeptonPidSummary(*lit);
	ev.ln++;
      }

    //
    // PHOTON SELECTION
    //
    edm::Handle<edm::View<reco::Candidate> > hPhoton;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("source"), hPhoton );
    edm::Handle<EcalRecHitCollection> ebrechits;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"), ebrechits );
    EcalClusterLazyTools lazyTool(event,iSetup,
                                  objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"),
                                  objConfig_["Photons"].getParameter<edm::InputTag>("eerechits"));

    edm::Handle<std::vector<reco::Track> > hTracks;
    try{ event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("trackSource"), hTracks ); } catch(std::exception &e){}
    edm::Handle<std::vector<reco::Track> > hGsfTracks;
    try{ event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("gsfTrackSource"), hGsfTracks ); } catch(std::exception &e){}
    ev.gn=0;
    std::vector<CandidatePtr> selPhotons=getGoodPhotons(hPhoton,lazyTool,ebrechits,*rho25,objConfig_["Photons"]);
    for(std::vector<CandidatePtr>::iterator phoIt = selPhotons.begin(); phoIt != selPhotons.end(); phoIt++)
      {
	const reco::Photon *pho = dynamic_cast<const reco::Photon *>(phoIt->get());
 	ev.g_px[ev.gn]    = pho->px();
	ev.g_py[ev.gn]    = pho->py();
	ev.g_pz[ev.gn]    = pho->pz();
	ev.g_en[ev.gn]    = pho->energy();
	ev.g_hoe[ev.gn]   = pho->hadronicOverEm();  
	ev.g_sihih[ev.gn] = pho->sigmaIetaIeta();
	ev.g_iso1[ev.gn]  = pho->trkSumPtSolidConeDR04();
	ev.g_iso2[ev.gn]  = pho->ecalRecHitSumEtConeDR04();
	ev.g_iso3[ev.gn]  = pho->hcalTowerSumEtConeDR04();
	ev.g_r9[ev.gn]    = pho->r9();
	ev.g_conv[ev.gn] = false;
	ev.g_trkVeto[ev.gn] = getPhotonTrackVeto(pho,hTracks,hGsfTracks,hEle);
	LorentzVector convP4(0,0,0,0);
	/*
	  ev.g_conv_px[ev.gn] = convP4.px();
	  ev.g_conv_py[ev.gn] = convP4.py();
	  ev.g_conv_pz[ev.gn] = convP4.pz();
	  ev.g_conv_en[ev.gn] = convP4.energy();
	*/
	ev.gn++;
      }
    if(ev.cat==UNKNOWN && selPhotons.size() && triggerBits["gamma"]==true) ev.cat=GAMMA+1000*photonTrig.second;

    //quit if no gamma or dilepton candidate
    if(ev.cat==UNKNOWN) return;


    //
    // JET SELECTION
    //
    Handle<View<Candidate> > hJet;
    event.getByLabel(objConfig_["Jets"].getParameter<edm::InputTag>("source"), hJet);
    ev.jn=0;
    std::vector<CandidatePtr> selJets = getGoodJets(hJet, selLeptons, objConfig_["Jets"]);    
    int nbcands(0);
    LorentzVector jetSum(0,0,0,0);
    for (std::vector<CandidatePtr>::iterator jIt = selJets.begin(); jIt != selJets.end(); jIt++)
      {
	const pat::Jet *jet = dynamic_cast<const pat::Jet *>(jIt->get());
	jetSum += LorentzVector(jet->px(),jet->py(),jet->pz(),jet->energy());
	ev.jn_px[ev.jn] = jet->px();  
	ev.jn_py[ev.jn] = jet->py(); 
	ev.jn_pz[ev.jn] = jet->pz(); 
	ev.jn_en[ev.jn] = jet->energy();
	const reco::Candidate *genParton = jet->genParton();
	ev.jn_genid[ev.jn]       = genParton ? genParton->pdgId() : -9999;
	ev.jn_genflav[ev.jn]     = jet->partonFlavour();
	ev.jn_btag1[ev.jn]       = jet->bDiscriminator("trackCountingHighEffBJetTags");
	ev.jn_btag2[ev.jn]       = jet->bDiscriminator("combinedSecondaryVertexBJetTags");
	ev.jn_neutHadFrac[ev.jn] = jet->neutralHadronEnergyFraction();
	ev.jn_neutEmFrac[ev.jn]  = jet->neutralEmEnergyFraction();
	ev.jn_chHadFrac[ev.jn]   = jet->chargedHadronEnergyFraction();
	const reco::GenJet *gJet=jet->genJet();
	ev.jn_genpt[ev.jn]=gJet ? gJet->pt() : 0;
	PileupJetIdentifier puIdentifier = puJetIdAlgo_.computeIdVariables(dynamic_cast<const reco::Jet*>(jet), 0, primVertex.get(), true);
	ev.jn_pumva[ev.jn]=puIdentifier.mva();
	ev.jn++;
	nbcands += (jet->pt()>30 && fabs(jet->eta())<2.4 && jet->bDiscriminator("trackCountingHighEffBJetTags")>2); 
      }
    ev.htvec_px = jetSum.px();
    ev.htvec_py = jetSum.py();
    ev.htvec_pz = jetSum.pz();
    ev.htvec_en = jetSum.energy();


    // JET SELECTION
    //
    ev.ajn=0;
    //Handle<std::vector<PFJet> > haJet;
    //std::vector<PFJet> selaJets = *haJet.product();
    //for (std::vector<PFJet>::iterator jIt = selaJets.begin(); jIt != selaJets.end(); jIt++)
    //{
    //const PFJet* jet = &(*jIt);  
    Handle<View<Candidate> > haJet;
    event.getByLabel(objConfig_["AssocJets"].getParameter<edm::InputTag>("source"), haJet);
    std::vector<CandidatePtr> selaJets = getGoodJets(haJet, selLeptons, objConfig_["AssocJets"]);
    for (std::vector<CandidatePtr>::iterator jIt = selaJets.begin(); jIt != selaJets.end(); jIt++)
      {  
	const pat::Jet *jet = dynamic_cast<const pat::Jet *>(jIt->get());
        ev.ajn_px[ev.ajn] = jet->px();
        ev.ajn_py[ev.ajn] = jet->py(); 
        ev.ajn_pz[ev.ajn] = jet->pz(); 
        ev.ajn_en[ev.ajn] = jet->energy();
        const reco::Candidate *genParton = jet->genParton();
        ev.ajn_genid[ev.ajn]       = genParton ? genParton->pdgId() : -9999;
        ev.ajn_genflav[ev.ajn]     = jet->partonFlavour();
        ev.ajn_btag1[ev.ajn]       = jet->bDiscriminator("trackCountingHighEffBJetTags");
        ev.ajn_btag2[ev.ajn]       = jet->bDiscriminator("combinedSecondaryVertexBJetTags");
        ev.ajn_neutHadFrac[ev.ajn] = jet->neutralHadronEnergyFraction();
        ev.ajn_neutEmFrac[ev.ajn]  = jet->neutralEmEnergyFraction();
        ev.ajn_chHadFrac[ev.ajn]   = jet->chargedHadronEnergyFraction();
        const reco::GenJet *gJet   = jet->genJet();
        ev.ajn_genpt[ev.ajn]       = gJet ? gJet->pt() : 0;
        PileupJetIdentifier puIdentifier = puJetIdAlgo_.computeIdVariables(dynamic_cast<const reco::Jet*>(jet), 0, primVertex.get(), true);
        ev.ajn_pumva[ev.jn]=puIdentifier.mva();
	ev.ajn++;
      }

    //
    // MET SELECTION
    //
    std::vector<edm::InputTag> clusteredMetSources = objConfig_["MET"].getParameter<std::vector<edm::InputTag> >("hzzmetSources");
    ev.nmet=clusteredMetSources.size()+1;

    //pf-met
    ev.met_phi[0] = pfmet->phi();    ev.met_pt[0] =  pfmet->pt();
 
    //pseudo-mets
    std::vector<double> sumEts; 
    std::vector<LorentzVector>  clusteredMets;

    try{
      edm::Handle< std::vector<double> > sumEtsH;
      event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("sumEtSources"),sumEtsH);

      if(sumEtsH.isValid())
	{
	  ev.sumEt = (*sumEtsH)[0];              ev.sumEtcentral = (*sumEtsH)[3];
	  ev.chsumEt = (*sumEtsH)[1];            ev.chsumEtcentral = (*sumEtsH)[4];
	  ev.neutsumEt = (*sumEtsH)[2];          ev.neutsumEtcentral = (*sumEtsH)[5];
	  ev.primVertexSumEt = (*sumEtsH)[6];    ev.primVertexChSumEt = (*sumEtsH)[7];    ev.primVertexNeutSumEt = (*sumEtsH)[8];
	  ev.otherVertexSumEt = (*sumEtsH)[9];   ev.otherVertexChSumEt = (*sumEtsH)[10];  ev.otherVertexNeutSumEt = (*sumEtsH)[11];
	}

      for(size_t i=0; i<clusteredMetSources.size(); i++)
	{
	  edm::Handle< reco::PFMET > clustMetH;
	  event.getByLabel(clusteredMetSources[i],clustMetH); 
	  LorentzVector iclustMet(clustMetH->px(),clustMetH->py(),0,clustMetH->pt());
	  ev.met_phi[i+1]  = iclustMet.phi();   ev.met_pt[i+1]  = iclustMet.pt();
	}

    }catch(std::exception &e){
      cout << e.what() << endl;
    }
        
    //set flags for coarse selection
    if(dilepton.size()==2)
      {
	ev.pass=ev.cat;
	if(ev.ln==0)      ev.pass += 1000;
	if(nbcands==0)    ev.pass += 2000;  
	if(ev.met_pt[0]>60) ev.pass += 3000;
	if(ev.met_pt[0]>100) ev.pass += 4000;
	LorentzVector dilP4=dilepton[0]->p4()+dilepton[1]->p4();
	if( fabs(dilP4.mass()-91)<15 ) ev.pass +=5000;
      }

    // finish event summary
    summaryHandler_.fillTree();
  }catch(std::exception &e){
    std::cout << "[DileptonPlusMETEventAnalyzer][analyze] failed with " << e.what() << std::endl;
  }


}

//
void DileptonPlusMETEventAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  TString filterCtrs[]={"startCounter","noScrapCounter","goodVertexCounter","noHBHEnoiseCounter","nobeamHaloCounter"};
  const size_t nselFilters=sizeof(filterCtrs)/sizeof(TString);
  for(size_t istep=0; istep<nselFilters; istep++)
    {
      std::string fname(filterCtrs[istep].Data());
      try{
	edm::Handle<edm::MergeableCounter> ctrHandle;
	iLumi.getByLabel(fname, ctrHandle);
	if(!ctrHandle.isValid()) continue;
	controlHistos_.fillHisto("cutflow","all",istep,ctrHandle->value);
      }catch(std::exception){
	controlHistos_.fillHisto("cutflow","all",istep);
      }
    }
}


//
int DileptonPlusMETEventAnalyzer::getElectronPidSummary(const pat::Electron *ele)
{
  int summary(0);
  bool isEcalDriven(true);
  try{
    isEcalDriven = ele->ecalDrivenSeed();
  }catch(std::exception &e){
    cout << e.what() << endl;
  }

  bool hasHEEPid(false);  
  try{
    int myHeepBits[]={heep::CutCodes::DETAIN,          
		      heep::CutCodes::DPHIIN,        
		      heep::CutCodes::HADEM,
		      heep::CutCodes::SIGMAIETAIETA, 
		      heep::CutCodes::E2X5OVER5X5,
		      heep::CutCodes::NRMISSHITS};
    int heepIdVal( ele->electronID("eidHEEP"));
    hasHEEPid=true;
    if(heepIdVal>0)
      {
	for(size_t ibit=0; ibit<sizeof(myHeepBits)/sizeof(int); ibit++)
	  hasHEEPid &= !(heep::CutCodes::passCuts(heepIdVal,myHeepBits[ibit]));
      }
  }catch(std::exception &e){
  }

  summary= ( (int(ele->electronID("eidVBTF70")) & 0x1) )
    | ( (int(ele->electronID("eidVBTF80")) & 0x1) << 1)
    | ( (int(ele->electronID("eidVBTF85")) & 0x1) << 2)
    | ( (int(ele->electronID("eidVBTF90")) & 0x1) << 3)
    | ( (int(ele->electronID("eidVBTF95")) & 0x1) << 4)
    | (hasHEEPid << 5)
    | ( isEcalDriven << 10);
 
  return summary;
}

//
int DileptonPlusMETEventAnalyzer::getMuonPidSummary(const pat::Muon *mu)
{
  int summary(0);
  summary=( (int(mu->muonID("GlobalMuonPromptTight")) & 0x1) )
    | ( (int(mu->muonID("TMLastStationLoose")) & 0x1) << 1)
    | ( (int(mu->muonID("TMLastStationTight")) & 0x1) << 2)
    | ( (int(mu->muonID("TMLastStationAngTight")) & 0x1) << 3)
    | (  mu->isTrackerMuon() << 4 )
    | (  mu->isGlobalMuon() << 5 );
  return summary;
}

DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 

//  LocalWords:  beamSpot
