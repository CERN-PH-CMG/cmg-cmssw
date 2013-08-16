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
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"
#include "TSystem.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Luminosity/interface/LumiSummary.h"

using namespace std;
using namespace edm;
using namespace reco;

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void beginLuminosityBlock(const edm::LuminosityBlock &iLumi, const edm::EventSetup & iSetup );
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  virtual void beginRun(const edm::Run & iRun, edm::EventSetup const & iSetup); 
 
private:
  void saveMCtruth(const edm::Event &event, const edm::EventSetup &iSetup );
  int addPidSummary(ObjectIdSummary &obj);

  std::map<std::string, edm::ParameterSet> objConfig_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
  EventCategory eventClassifComp_;
  std::vector<PileupJetIdAlgo *> puJetIdAlgo_;
  edm::Handle<reco::VertexCollection> hVtx_;

  //regression corrector for electrons/photons
  EGEnergyCorrector phocorr_,ecorr_;

  PFIsolationEstimator eIsolator;

  HLTConfigProvider hltConfig_;
  
  //heavy flavors from gluon splitting
  std::vector<reco::CandidatePtr> hfFromGsplit; 
  float curAvgInstLumi_, curIntegLumi_;
  int iErr_;
};

using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
  : controlHistos_( iConfig.getParameter<std::string>("dtag") ),
    curAvgInstLumi_(0), curIntegLumi_(0),iErr_(0)
{
  try{
    std::string objs[]={"Generator", "Trigger", "Vertices", "Photons",
			"Electrons", "LooseElectrons", "Muons","LooseMuons", "Dileptons", "Jets", "AssocJets", "MET" };
    for(size_t iobj=0; iobj<sizeof(objs)/sizeof(string); iobj++)
      objConfig_[ objs[iobj] ] = iConfig.getParameter<edm::ParameterSet>( objs[iobj] );

    std::vector<edm::ParameterSet> jetIdPsets =iConfig.getParameter<edm::ParameterSet>("Jets").getParameter<std::vector<edm::ParameterSet> >("puJetIds");
    for(size_t iid=0; iid<jetIdPsets.size(); iid++) puJetIdAlgo_.push_back( new PileupJetIdAlgo( jetIdPsets[iid] ) );
    
    edm::Service<TFileService> fs;
    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    

    //cut flow histograms for book keeping
    TString selFilters[]={"Reco","no scrap","#geq 1 vertex","HB/HE noise","No beam halo"};
    const size_t nselFilters=sizeof(selFilters)/sizeof(TString);
    controlHistos_.addHistogram("cutflow", ";Steps; Events", nselFilters, 0.,nselFilters);
    TH1 *h = controlHistos_.getHisto("cutflow");
    for(size_t istep=0; istep<nselFilters; istep++) h->GetXaxis()->SetBinLabel(istep+1,selFilters[istep]);

    controlHistos_.addHistogram("pileup", ";Pileup; Events",100,-0.5,99.5);
    controlHistos_.addHistogram("integlumi", ";Integrated luminosity ; Events",100,0,1e5);
    controlHistos_.addHistogram("instlumi", ";Max average inst. luminosity; Events",100,0,1e5);
    controlHistos_.addHistogram("pileuptrue", ";True pileup; Events",100,-0.5,99.5);

    //configure PF isolation cone size=0.4 
    eIsolator.initializeElectronIsolation(kTRUE);
    eIsolator.setConeSize(0.4); 
  }
  catch(std::exception &e){
    cout << e.what() << endl;
  }  
}


//
int DileptonPlusMETEventAnalyzer::addPidSummary(ObjectIdSummary &obj)
{
  ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
  int index(0);
  if(fabs(obj.id)==1)
    {
    }
  else if(fabs(obj.id)==11)
    {
      ev.en_idbits[ev.en]=obj.idBits;
      ev.en_hoe[ev.en]=obj.hoe;
      ev.en_hoebc[ev.en]=obj.hoebc;
      ev.en_dphiin[ev.en]=obj.dPhiTrack;
      ev.en_detain[ev.en]=obj.dEtaTrack;
      ev.en_sihih[ev.en]=obj.sihih;
      ev.en_sipip[ev.en]=obj.sipip;
      ev.en_sce[ev.en]=obj.sce;
      ev.en_sceta[ev.en]=obj.sceta;
      ev.en_scphi[ev.en]=obj.scphi;
      ev.en_e2x5max[ev.en]=obj.e2x5max;
      ev.en_e1x5[ev.en]=obj.e1x5;
      ev.en_e5x5[ev.en]=obj.e5x5;
      ev.en_h2te[ev.en]=obj.h2te;
      ev.en_h2tebc[ev.en]=obj.h2tebc;
      ev.en_r9[ev.en]=obj.r9;
      ev.en_ooemoop[ev.en]=obj.ooemoop;
      ev.en_fbrem[ev.en]=obj.fbrem;
      ev.en_eopin[ev.en]=obj.eopin;
      ev.en_mvatrigv0[ev.en]=obj.emvatrigv0;
      ev.en_mvanontrigv0[ev.en]=obj.emvanontrigv0;
      ev.en_dEtaCalo[ev.en]=obj.dEtaCalo;
      ev.en_dPhiCalo[ev.en]=obj.dPhiCalo;
      ev.en_kfchi2[ev.en]=obj.kfchi2;
      ev.en_kfhits[ev.en]=obj.kfhits;
      ev.en_kfhitsall[ev.en]=obj.kfhitsall;
      ev.en_sihip[ev.en]=obj.sihip;
      ev.en_nbrems[ev.en]=obj.nbrems;
      ev.en_etawidth[ev.en]=obj.etawidth;
      ev.en_phiwidth[ev.en]=obj.phiwidth;
      ev.en_e1x5e5x5[ev.en]=obj.e1x5e5x5;
      ev.en_preShowerOverRaw[ev.en]=obj.preShowerOverRaw;
      ev.en_eopout[ev.en]=obj.eopout;
      index=ev.en;
      ev.en++;
    }
  else if(fabs(obj.id)==13)
    {
      ev.mn_idbits[ev.mn]                     = obj.idBits;
      ev.mn_nMatches[ev.mn]                   = obj.trkMatches; 
      ev.mn_nMatchedStations[ev.mn]           = obj.trkMatchedStations;
      ev.mn_validMuonHits[ev.mn]              = obj.trkValidMuonHits;
      ev.mn_pixelLayersWithMeasurement[ev.mn] = obj.pixelLayersWithMeasurement;
      ev.mn_trkLayersWithMeasurement[ev.mn]   = obj.trkLayersWithMeasurement;
      ev.mn_innerTrackChi2[ev.mn]             = obj.innerTrackChi2;
      index=ev.mn;
      ev.mn++;
    }
  else if(fabs(obj.id)==22)
    {
    }

  return index;
}

//
void DileptonPlusMETEventAnalyzer::saveMCtruth(const edm::Event &event, const edm::EventSetup &iSetup)
{
  ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
  ev.nmcparticles=0;

  //  float weight(1.0);
  ev.puWeight = 1.0;
  if(event.isRealData())  return;

  edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
  event.getByType(puInfoH);
  int npuOOT(0),npuIT(0),npuOOTm1(0);
  float truePU(0);
  if(puInfoH.isValid())
    {
      for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	{
	  if(it->getBunchCrossing()==0) { npuIT += it->getPU_NumInteractions(); truePU = it->getTrueNumInteractions(); }
	  else                          npuOOT += it->getPU_NumInteractions();
	  if(it->getBunchCrossing()<0)  npuOOTm1 += it->getPU_NumInteractions();
	  
	}
    }
  ev.ngenITpu=npuIT;
  ev.ngenOOTpu=npuOOT;
  ev.ngenOOTpum1=npuOOTm1;
  ev.ngenTruepu=truePU;
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
	  //	  cout << ev.qscale << " " << genEventInfoProd->pdf()->scalePDF << endl;
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
  try{
    Handle<LHEEventProduct> lheH;
    event.getByType(lheH);
    ev.mc_nup=0;
    if(lheH.isValid()) ev.mc_nup=lheH->hepeup().NUP;
  }catch(std::exception &e){
  }

  Handle<View<Candidate> > hGen;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("source"), hGen);
  std::pair<int,std::vector<const reco::Candidate *> > genEvent = assignPhysicsChannel(hGen,objConfig_["Generator"]);
  ev.nmcparticles = 0;
  ev.mccat=genEvent.first;
  for(size_t i=0; i<genEvent.second.size(); i++)
    {
      const reco::Candidate *genpart = genEvent.second[i];
      
      if(fabs(genpart->pdgId())==25 || fabs(genpart->pdgId())==39) 
	{
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
	  ev.mc_lxy[ev.nmcparticles]=0;
	  ev.mc_id[ev.nmcparticles]=genpart->pdgId();
	  ev.mc_status[ev.nmcparticles]=genpart->status();
	  ev.nmcparticles++;
	}
    }

  //   try{
  //     edm::Handle<TrackingVertexCollection> TVCollection;
  //     event.getByType(TVCollection);
  //     for (std::size_t index = 0; index < TVCollection->size(); index++)
  //       {
  // 	TrackingVertexRef vertex(TVCollection, index);
  // 	cout << *vertex << endl;
  //       }
  //   }catch(std::exception &e){
  //     cout << e.what() << endl;
  //   }
  
  //heavy flavors
  hfFromGsplit = filterHFfromGSplit(hGen); 
  for(size_t i=0; i<hfFromGsplit.size(); i++)
    {
      const reco::Candidate *genpart = hfFromGsplit[i].get();
      ev.mc_px[ev.nmcparticles]=genpart->px();  
      ev.mc_py[ev.nmcparticles]=genpart->py();  
      ev.mc_pz[ev.nmcparticles]=genpart->pz(); 
      ev.mc_en[ev.nmcparticles]=genpart->energy();  
      ev.mc_lxy[ev.nmcparticles]=0;
      if(abs(genpart->pdgId())>500)
	{
	  float vxMother = genpart->vx();
	  float vyMother = genpart->vy();
	  int dauId(0), gDauId(0);
	  float vxDaughter(0), vyDaughter(0);
	  float vxGdaughter(0), vyGdaughter(0);
	  const reco::Candidate *fsPart=getGeneratorFinalStateFor(genpart);
	  if(fsPart && fsPart->numberOfDaughters()>0)
	    {
	      dauId      = fsPart->daughter(0)->pdgId();
	      vxDaughter = fsPart->daughter(0)->vx();
	      vyDaughter = fsPart->daughter(0)->vy();
	     
	      const reco::Candidate *fsDauPart=getGeneratorFinalStateFor(fsPart);
	      float vxGdaughter(0),vyGdaughter(0);
	      if(fsDauPart && fsDauPart->numberOfDaughters()>1)
		{
		  gDauId      = fsDauPart->daughter(1)->pdgId();
		  vxGdaughter = fsDauPart->daughter(1)->vx();
		  vyGdaughter = fsDauPart->daughter(1)->vy();
		}
	    }

	  float bDecayLength = max(sqrt(pow(vxMother-vxDaughter,2)+pow(vyMother-vyDaughter,2)),sqrt(pow(vxMother-vxGdaughter,2)+pow(vyMother-vyGdaughter,2))); 
	  ev.mc_lxy[ev.nmcparticles]=max(ev.mc_lxy[ev.nmcparticles],bDecayLength);
	}
      ev.mc_id[ev.nmcparticles]=genpart->pdgId();
      ev.mc_status[ev.nmcparticles]=genpart->status();
      ev.nmcparticles++;
    }
       
  //add the generator level jets
  edm::Handle<edm::View<reco::Candidate> > genJetsH;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("genJets"), genJetsH );
  for(size_t ijet=0; ijet<genJetsH.product()->size(); ijet++)
    {
      reco::CandidatePtr gjIt = genJetsH->ptrAt(ijet);
      if(gjIt->pt()<10) continue;
      
      //remove overlaps with leptons
      bool overlap(false);
      for(int imcpart=0; imcpart<ev.nmcparticles; imcpart++)
	{
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
      ev.mc_lxy[ev.nmcparticles]=0;
      ev.mc_status[ev.nmcparticles]=0;
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
    ev.curAvgInstLumi=curAvgInstLumi_;
    ev.curIntegLumi=curIntegLumi_;

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
	unsigned int gn_triggerWord=0;
	photonTrig = getHighestPhotonTrigThreshold( triggerBitsH, triggerNames , itriggers, gn_triggerWord);

	//save all triggers that fired and individual pre-scales
	ev.gn_triggerWord=gn_triggerWord;
	int ntrigs = triggerBitsH->size();
	for (int itrig = 0; itrig < ntrigs; itrig++) {
	  string trigName = triggerNames.triggerName(itrig);
	  if ( trigName.find("Photon") == string::npos ) continue;
	  int gTrigCount = 0;
	  for (vector<string>::iterator tIt = itriggers.begin(); tIt != itriggers.end(); tIt++, gTrigCount++) {
	    if (trigName.find(*tIt) != string::npos) {
	      ev.gn_prescale[gTrigCount]=hltConfig_.prescaleValue(event, iSetup, trigName);
	      break;
	    }
	  }
	}
      }
    if(triggerBits["singleMu"]==true && triggerBits["mumu"]==true) triggerBits["singleMu"]=false;   //veto overlaps: single muon triggers should be used exclusively 
	    
    //
    // vertex and beam spot
    //
    edm::Handle<reco::BeamSpot> beamSpot;
    event.getByLabel( objConfig_["Vertices"].getParameter<edm::InputTag>("beamSpot"), beamSpot);
    event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx_);
    std::vector<reco::VertexRef> selVertices = getGoodVertices(hVtx_,objConfig_["Vertices"]);

    //quit if no good vertex
    if(selVertices.size()==0) return;

    reco::VertexRef primVertex = selVertices[0];
    ev.nvtx=selVertices.size();
    ev.vtx_px = primVertex->p4().px(); 
    ev.vtx_py = primVertex->p4().py(); 
    ev.vtx_pz = primVertex->p4().pz();  
    ev.vtx_en = primVertex->p4().energy();

    // average energy density
    edm::Handle< double > rho, rho25,rho25Neut;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"),rho);
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("rho25"),rho25);
    event.getByLabel( objConfig_["Muons"].getParameter<edm::InputTag>("rho25Neut"),rho25Neut);
    ev.rho = *rho;
    ev.rho25 = *rho25;
    ev.rho25Neut = *rho25Neut;

    //
    // LEPTON SELECTION
    //
    
    //muon selection
    edm::Handle<View<Candidate> > hMu;
    event.getByLabel(objConfig_["Muons"].getParameter<edm::InputTag>("source"), hMu);
    std::vector<ObjectIdSummary> muonSummary;
    std::vector<CandidatePtr> selMuons      = getGoodMuons(hMu, primVertex, *rho, objConfig_["Muons"], iSetup, muonSummary);
    
    //electron selection
    Handle<View<Candidate> > hEle;
    event.getByLabel(objConfig_["Electrons"].getParameter<edm::InputTag>("source"), hEle);
    EcalClusterLazyTools lazyTool(event,iSetup,objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"),objConfig_["Photons"].getParameter<edm::InputTag>("eerechits"));
    edm::Handle<reco::ConversionCollection> hConversions;
    try{ event.getByLabel(objConfig_["Photons"].getParameter<edm::InputTag>("conversions"), hConversions); }  catch(std::exception &e){ cout << e.what() << endl; }
    edm::Handle<reco::PFCandidateCollection> hPFCands;
    event.getByLabel("particleFlow",hPFCands);
    std::vector<ObjectIdSummary> eleSummary;
    std::vector<CandidatePtr> selElectrons   = getGoodElectrons(hEle, hMu, hVtx_, *beamSpot, hConversions, &ecorr_, lazyTool, &eIsolator, hPFCands, *rho, objConfig_["Electrons"], iSetup, eleSummary);

    //build the dilepton candidate
    ev.cat = UNKNOWN;
    ev.ln=0; ev.mn=0; ev.en=0;
    std::vector<CandidatePtr> selLeptons = selMuons;
    selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
    std::vector<ObjectIdSummary> selLeptonsSummary = muonSummary;
    selLeptonsSummary.insert(selLeptonsSummary.end(), eleSummary.begin(), eleSummary.end());
    std::vector<int> dileptonIdx = getDileptonCandidate(selLeptons, objConfig_["Dileptons"], iSetup);
    if(dileptonIdx.size()==2)
      {
	int l1idx(dileptonIdx[0]), l2idx(dileptonIdx[1]);
	ev.cat = getDileptonId(selLeptons[l1idx],selLeptons[l2idx]);
	
	//require trigger for each event category
	ev.triggerType=0;
	//if(event.isRealData())
	  {
	    if(ev.cat==EE   && triggerBits["ee"]==false)   return;
	    if(ev.cat==MUMU && triggerBits["singleMu"]==false && triggerBits["mumu"]==false) return;  
	    if(ev.cat==EMU  && triggerBits["emu"]==false)  return;
	    
	    ev.triggerType = (triggerBits["ee"] << 0 ) |
	      (triggerBits["mumu"] << 1 ) |
	      (triggerBits["emu"] << 2 ) |
	      (triggerBits["singleMu"] << 3 );
	  }
	  //ev.hasTrigger=true;

	//leading lepton
	ev.l1_px                   = selLeptonsSummary[l1idx].p4.px();
	ev.l1_py                   = selLeptonsSummary[l1idx].p4.py();
	ev.l1_pz                   = selLeptonsSummary[l1idx].p4.pz();
	ev.l1_en                   = selLeptonsSummary[l1idx].p4.energy();
	ev.l1_id                   = selLeptonsSummary[l1idx].id*selLeptonsSummary[l1idx].charge;
	ev.l1_ptErr                = selLeptonsSummary[l1idx].p4.pt()*selLeptonsSummary[l1idx].ensferr;
	ev.l1_genid                = selLeptonsSummary[l1idx].genid;
	ev.l1_gIso                 = selLeptonsSummary[l1idx].isoVals[G_ISO]; 
	ev.l1_nhIso                = selLeptonsSummary[l1idx].isoVals[N_ISO]; 
	ev.l1_chIso                = selLeptonsSummary[l1idx].isoVals[C_ISO];
	ev.l1_puchIso              = selLeptonsSummary[l1idx].isoVals[CPU_ISO];
	ev.l1_ecalIso              = selLeptonsSummary[l1idx].isoVals[ECAL_ISO]; 
	ev.l1_hcalIso              = selLeptonsSummary[l1idx].isoVals[HCAL_ISO]; 
	ev.l1_trkIso               = selLeptonsSummary[l1idx].isoVals[TRACKER_ISO];
	ev.l1_d0                   = selLeptonsSummary[l1idx].trkd0;
	ev.l1_dZ                   = selLeptonsSummary[l1idx].trkdZ;
	ev.l1_ip3d                 = selLeptonsSummary[l1idx].trkip3d;
	ev.l1_ip3dsig              = selLeptonsSummary[l1idx].trkip3dsig;
	ev.l1_trkpt                = selLeptonsSummary[l1idx].trkpt;
	ev.l1_trketa               = selLeptonsSummary[l1idx].trketa;
	ev.l1_trkphi               = selLeptonsSummary[l1idx].trkphi;
	ev.l1_trkchi2              = selLeptonsSummary[l1idx].trkchi2;
	ev.l1_trkValidPixelHits    = selLeptonsSummary[l1idx].trkValidPixelHits;
	ev.l1_trkValidTrackerHits  = selLeptonsSummary[l1idx].trkValidTrackerHits;
	ev.l1_trkLostInnerHits     = selLeptonsSummary[l1idx].trkLostInnerHits;
	ev.l1_ensf                 = selLeptonsSummary[l1idx].ensf;
	ev.l1_ensferr              = selLeptonsSummary[l1idx].ensferr;
	ev.l1_pid                  = addPidSummary(selLeptonsSummary[l1idx]);

	//trailer lepton
	ev.l2_px                   = selLeptonsSummary[l2idx].p4.px();
	ev.l2_py                   = selLeptonsSummary[l2idx].p4.py();
	ev.l2_pz                   = selLeptonsSummary[l2idx].p4.pz();
	ev.l2_en                   = selLeptonsSummary[l2idx].p4.energy();
	ev.l2_id                   = selLeptonsSummary[l2idx].id*selLeptonsSummary[l2idx].charge;
	ev.l2_ptErr                = selLeptonsSummary[l2idx].p4.pt()*selLeptonsSummary[l2idx].ensferr;
	ev.l2_genid                = selLeptonsSummary[l2idx].genid;
	ev.l2_gIso                 = selLeptonsSummary[l2idx].isoVals[G_ISO]; 
	ev.l2_nhIso                = selLeptonsSummary[l2idx].isoVals[N_ISO]; 
	ev.l2_chIso                = selLeptonsSummary[l2idx].isoVals[C_ISO];
	ev.l2_puchIso              = selLeptonsSummary[l2idx].isoVals[CPU_ISO];
	ev.l2_ecalIso              = selLeptonsSummary[l2idx].isoVals[ECAL_ISO]; 
	ev.l2_hcalIso              = selLeptonsSummary[l2idx].isoVals[HCAL_ISO]; 
	ev.l2_trkIso               = selLeptonsSummary[l2idx].isoVals[TRACKER_ISO];
	ev.l2_d0                   = selLeptonsSummary[l2idx].trkd0;
	ev.l2_dZ                   = selLeptonsSummary[l2idx].trkdZ;
	ev.l2_ip3d                 = selLeptonsSummary[l2idx].trkip3d;
	ev.l2_ip3dsig              = selLeptonsSummary[l2idx].trkip3dsig;
	ev.l2_trkpt                = selLeptonsSummary[l2idx].trkpt;
	ev.l2_trketa               = selLeptonsSummary[l2idx].trketa;
	ev.l2_trkphi               = selLeptonsSummary[l2idx].trkphi;
	ev.l2_trkchi2              = selLeptonsSummary[l2idx].trkchi2;
	ev.l2_trkValidPixelHits    = selLeptonsSummary[l2idx].trkValidPixelHits;
	ev.l2_trkValidTrackerHits  = selLeptonsSummary[l2idx].trkValidTrackerHits;
	ev.l2_trkLostInnerHits     = selLeptonsSummary[l2idx].trkLostInnerHits;
	ev.l2_ensf                 = selLeptonsSummary[l2idx].ensf;
	ev.l2_ensferr              = selLeptonsSummary[l2idx].ensferr;
	ev.l2_pid                  = addPidSummary( selLeptonsSummary[l2idx] );
      }
    
    //save extra leptons (including softer ones)
    std::vector<ObjectIdSummary> looseMuonSummary;
    std::vector<CandidatePtr>    looseMuons = getGoodMuons(hMu, primVertex, *rho, objConfig_["LooseMuons"], iSetup, looseMuonSummary);
    std::vector<ObjectIdSummary> looseEleSummary;
    std::vector<CandidatePtr>    looseElectrons = getGoodElectrons(hEle, hMu, hVtx_, *beamSpot, hConversions, &ecorr_,  lazyTool, &eIsolator, hPFCands, *rho, objConfig_["LooseElectrons"], iSetup, looseEleSummary);
    std::vector<ObjectIdSummary> selLooseLeptonsSummary = selLeptonsSummary;
    selLooseLeptonsSummary.insert( selLooseLeptonsSummary.end(), looseMuonSummary.begin(), looseMuonSummary.end() );
    selLooseLeptonsSummary.insert( selLooseLeptonsSummary.end(), looseEleSummary.begin(),  looseEleSummary.end() );
    std::vector<CandidatePtr>     selLooseLeptons = selLeptons;
    selLooseLeptons.insert( selLooseLeptons.end(), looseMuons.begin(),     looseMuons.end() );
    selLooseLeptons.insert( selLooseLeptons.end(), looseElectrons.begin(), looseElectrons.end() );
    std::vector<LorentzVector> savedLeptonsP4;
    if(dileptonIdx.size()>0) 
      {
	savedLeptonsP4.push_back( selLeptonsSummary[dileptonIdx[0]].p4 ); 
	savedLeptonsP4.push_back( selLeptonsSummary[dileptonIdx[1]].p4 ); 
      }
    for(size_t ilep=0; ilep<selLooseLeptonsSummary.size(); ilep++)
      {
	ObjectIdSummary &lep=selLooseLeptonsSummary[ilep];

	//check if objects are not repeated...
	bool veto(false);
	for(size_t isavLep=0; isavLep<savedLeptonsP4.size(); isavLep++)
	  {
	    if( deltaR( savedLeptonsP4[isavLep],lep.p4)>0.1 ) continue;
	    veto=true;
	  }
	if(veto) continue;
	savedLeptonsP4.push_back(lep.p4);
	
	//save it
	ev.ln_px[ev.ln]                  = lep.p4.px();
	ev.ln_py[ev.ln]                  = lep.p4.py();
	ev.ln_pz[ev.ln]                  = lep.p4.pz();
	ev.ln_en[ev.ln]                  = lep.p4.energy();
	ev.ln_id[ev.ln]                  = lep.id*lep.charge;
	ev.ln_genid[ev.ln]               = lep.genid;
	ev.ln_ptErr[ev.ln]               = lep.p4.pt()*lep.ensferr;
	ev.ln_gIso[ev.ln]                = lep.isoVals[G_ISO]; 
	ev.ln_nhIso[ev.ln]               = lep.isoVals[N_ISO]; 
	ev.ln_chIso[ev.ln]               = lep.isoVals[C_ISO];
	ev.ln_puchIso[ev.ln]             = lep.isoVals[CPU_ISO];
	ev.ln_ecalIso[ev.ln]             = lep.isoVals[ECAL_ISO]; 
	ev.ln_hcalIso[ev.ln]             = lep.isoVals[HCAL_ISO]; 
	ev.ln_trkIso[ev.ln]              = lep.isoVals[TRACKER_ISO];
	ev.ln_d0[ev.ln]                  = lep.trkd0;
	ev.ln_dZ[ev.ln]                  = lep.trkdZ;
	ev.ln_ip3d[ev.ln]                = lep.trkip3d;
	ev.ln_ip3dsig[ev.ln]             = lep.trkip3dsig;
	ev.ln_trkpt[ev.ln]               = lep.trkpt;
	ev.ln_trketa[ev.ln]              = lep.trketa;
	ev.ln_trkphi[ev.ln]              = lep.trkphi;
	ev.ln_trkchi2[ev.ln]             = lep.trkchi2;
	ev.ln_trkValidPixelHits[ev.ln]   = lep.trkValidPixelHits;
	ev.ln_trkValidTrackerHits[ev.ln] = lep.trkValidTrackerHits;
	ev.ln_trkLostInnerHits[ev.ln]    = lep.trkLostInnerHits;
	ev.ln_ensf[ev.ln]                = lep.ensf;
	ev.ln_ensferr[ev.ln]             = lep.ensferr;
	ev.ln_pid[ev.ln]                 = addPidSummary(lep);
	ev.ln++;
      }

    //
    // PHOTON SELECTION
    //
    edm::Handle<edm::View<reco::Candidate> > hPhoton;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("source"), hPhoton );
    edm::Handle<std::vector<reco::Track> > hTracks;
    try{ event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("trackSource"), hTracks ); } catch(std::exception &e){}
    edm::Handle<reco::GsfElectronCollection> hGsfEle;
    try{ event.getByLabel(objConfig_["Photons"].getParameter<edm::InputTag>("gsfElectrons"),hGsfEle); }  catch(std::exception &e){}
    edm::Handle<EcalRecHitCollection> hEBrechits;
    try{ event.getByLabel(objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"),hEBrechits); } catch(std::exception &e){}
    ev.gn=0;
    std::vector<ObjectIdSummary> selPhotonIds;
    std::vector<CandidatePtr> selPhotons=getGoodPhotons(hPhoton,&phocorr_,lazyTool,hEBrechits,hGsfEle,hConversions,hTracks,hVtx_,beamSpot,*rho25,objConfig_["Photons"],iSetup,selPhotonIds);
    for(size_t i=0; i<selPhotonIds.size(); i++)
      {
 	ev.g_px[ev.gn]        = selPhotonIds[i].p4.px();
	ev.g_py[ev.gn]        = selPhotonIds[i].p4.py();
	ev.g_pz[ev.gn]        = selPhotonIds[i].p4.pz();
	ev.g_en[ev.gn]        = selPhotonIds[i].p4.energy();
	ev.g_hoe[ev.gn]       = selPhotonIds[i].hoe;
	ev.g_htoe[ev.gn]      = selPhotonIds[i].hoebc;
	ev.g_sihih[ev.gn]     = selPhotonIds[i].sihih;
	ev.g_sipip[ev.gn]     = selPhotonIds[i].sipip;
	ev.g_iso1[ev.gn]      = selPhotonIds[i].isoVals[TRACKER_ISO];
	ev.g_iso2[ev.gn]      = selPhotonIds[i].isoVals[ECAL_ISO];
	ev.g_iso3[ev.gn]      = selPhotonIds[i].isoVals[HCAL_ISO];
	ev.g_r9[ev.gn]        = selPhotonIds[i].r9;
	ev.g_corren[ev.gn]    = selPhotonIds[i].ensf;
	ev.g_correnerr[ev.gn] =  selPhotonIds[i].ensferr;
	ev.g_idbits[ev.gn]    = selPhotonIds[i].idBits;
	ev.gn++;
      }
    if(ev.cat==UNKNOWN && selPhotons.size())
      if(!event.isRealData() || triggerBits["gamma"]==true) 
	{
	  ev.cat=GAMMA+1000*photonTrig.second;
	}

    //quit if no gamma or dilepton candidate
    if(ev.cat==UNKNOWN) return;


    //
    // JET SELECTION
    //
    Handle<View<Candidate> > hJet;
    event.getByLabel(objConfig_["Jets"].getParameter<edm::InputTag>("source"), hJet);
    std::vector<edm::Handle<reco::JetTagCollection> > jetTagsH;
    std::vector<edm::InputTag> jetTags=objConfig_["Jets"].getParameter<std::vector<edm::InputTag> >("jetTags");
    for(size_t ijt=0; ijt<jetTags.size(); ijt++)
      {
	try{
	  edm::Handle<reco::JetTagCollection>  iJetTagH;
	  event.getByLabel(jetTags[ijt], iJetTagH);
	  jetTagsH.push_back( iJetTagH );
	}catch(std::exception &e){
	}
      }
    ev.jn=0;
    std::vector<ObjectIdSummary> selJetsId;
    std::vector<CandidatePtr> selJets = getGoodJets(hJet, selLeptons, hVtx_, puJetIdAlgo_, jetTagsH, objConfig_["Jets"],selJetsId);    
    LorentzVector jetSum(0,0,0,0);
    for(size_t ijet=0; ijet<selJetsId.size(); ijet++)
      {
	jetSum += selJetsId[ijet].p4;
	ev.jn_px[ev.jn]          = selJetsId[ijet].p4.px();
	ev.jn_py[ev.jn]          = selJetsId[ijet].p4.py();
	ev.jn_pz[ev.jn]          = selJetsId[ijet].p4.pz();
	ev.jn_en[ev.jn]          = selJetsId[ijet].p4.energy();
	ev.jn_genid[ev.jn]       = selJetsId[ijet].genid;
	ev.jn_genflav[ev.jn]     = selJetsId[ijet].genflav;
	ev.jn_btag1[ev.jn]       = selJetsId[ijet].tche;
	ev.jn_btag2[ev.jn]       = selJetsId[ijet].csv;
	ev.jn_btag3[ev.jn]       = selJetsId[ijet].jp;
	ev.jn_btag4[ev.jn]       = selJetsId[ijet].tchp;
	if(selJetsId[ijet].customTaggers.size()>0) ev.jn_btag5[ev.jn] = selJetsId[ijet].customTaggers[0];
	if(selJetsId[ijet].customTaggers.size()>1) ev.jn_btag6[ev.jn] = selJetsId[ijet].customTaggers[1];
	if(selJetsId[ijet].customTaggers.size()>2) ev.jn_btag7[ev.jn] = selJetsId[ijet].customTaggers[2];
	ev.jn_btag8[ev.jn]       = selJetsId[ijet].ssvhe;
	ev.jn_btag9[ev.jn]       = selJetsId[ijet].ssvhp;
	ev.jn_neutHadFrac[ev.jn] = selJetsId[ijet].neutHadFrac;
	ev.jn_neutEmFrac[ev.jn]  = selJetsId[ijet].neutEmFrac;
	ev.jn_chHadFrac[ev.jn]   = selJetsId[ijet].chHadFrac;
	ev.jn_beta[ev.jn]        = selJetsId[ijet].beta;
	ev.jn_betaStar[ev.jn]    = selJetsId[ijet].betaStar;
	ev.jn_dRMean[ev.jn]      = selJetsId[ijet].dRMean;
	ev.jn_ptD[ev.jn]         = selJetsId[ijet].ptD;
	ev.jn_ptRMS[ev.jn]       = selJetsId[ijet].ptRMS;
	ev.jn_etaW[ev.jn]        = selJetsId[ijet].sihih;
	ev.jn_phiW[ev.jn]        = selJetsId[ijet].sipip;
	ev.jn_genpt[ev.jn]       = selJetsId[ijet].genP4.pt();
	ev.jn_idbits[ev.jn]      = selJetsId[ijet].idBits;
	ev.jn_puminmva[ev.jn]    = selJetsId[ijet].mva[0];
	ev.jn_pumva[ev.jn]       = selJetsId[ijet].mva[1];
	ev.jn_rawsf[ev.jn]       = selJetsId[ijet].ensf;
	ev.jn_isBfromGsplit[ev.jn]      = (getHFmatchFromGSplit(selJets[ijet],hfFromGsplit,5)!=0);
	ev.jn_isCfromGsplit[ev.jn]      = (getHFmatchFromGSplit(selJets[ijet],hfFromGsplit,4)!=0);
	ev.jn_lxy[ev.jn]                = selJetsId[ijet].lxy;
	ev.jn_lxyErr[ev.jn]             = selJetsId[ijet].slxy;
	ev.jn_svmass[ev.jn]             = selJetsId[ijet].svmass;
	ev.jn_svpt[ev.jn]             = selJetsId[ijet].svpt;
	ev.jn_svdr[ev.jn]             = selJetsId[ijet].svdr;
	ev.jn++;
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
    std::vector<ObjectIdSummary> selAJetsId;
    std::vector<CandidatePtr> selaJets = getGoodJets(haJet, selLeptons, hVtx_,puJetIdAlgo_, jetTagsH, objConfig_["AssocJets"],selAJetsId);
    for(size_t ijet=0; ijet<selAJetsId.size(); ijet++)
      {
	ev.ajn_px[ev.ajn]          = selAJetsId[ijet].p4.px();
	ev.ajn_py[ev.ajn]          = selAJetsId[ijet].p4.py();
	ev.ajn_pz[ev.ajn]          = selAJetsId[ijet].p4.pz();
	ev.ajn_en[ev.ajn]          = selAJetsId[ijet].p4.energy();
	ev.ajn_rawsf[ev.ajn]       = selAJetsId[ijet].ensf;
	ev.ajn_genid[ev.ajn]       = selAJetsId[ijet].genid;
	ev.ajn_genflav[ev.ajn]     = selAJetsId[ijet].genflav;
	ev.ajn_btag1[ev.ajn]       = selAJetsId[ijet].tche;
	ev.ajn_btag2[ev.ajn]       = selAJetsId[ijet].csv;
	ev.ajn_btag3[ev.ajn]       = selAJetsId[ijet].jp;
	ev.ajn_btag4[ev.ajn]       = selAJetsId[ijet].tchp;
	if(selAJetsId[ijet].customTaggers.size()>0) ev.ajn_btag5[ev.ajn] = selAJetsId[ijet].customTaggers[0];
	if(selAJetsId[ijet].customTaggers.size()>1) ev.ajn_btag6[ev.ajn] = selAJetsId[ijet].customTaggers[1];
	if(selAJetsId[ijet].customTaggers.size()>2) ev.ajn_btag7[ev.ajn] = selAJetsId[ijet].customTaggers[2];
	ev.ajn_btag8[ev.ajn]       = selAJetsId[ijet].ssvhe;
	ev.ajn_btag9[ev.ajn]       = selAJetsId[ijet].ssvhp;
	ev.ajn_neutHadFrac[ev.ajn] = selAJetsId[ijet].neutHadFrac;
	ev.ajn_neutEmFrac[ev.ajn]  = selAJetsId[ijet].neutEmFrac;
	ev.ajn_chHadFrac[ev.ajn]   = selAJetsId[ijet].chHadFrac;
	ev.ajn_beta[ev.ajn]        = selAJetsId[ijet].beta;
	ev.ajn_betaStar[ev.ajn]    = selAJetsId[ijet].betaStar;
	ev.ajn_dRMean[ev.ajn]      = selAJetsId[ijet].dRMean;
	ev.ajn_ptD[ev.ajn]         = selAJetsId[ijet].ptD;
	ev.ajn_ptRMS[ev.ajn]       = selAJetsId[ijet].ptRMS;
	ev.ajn_genpt[ev.ajn]       = selAJetsId[ijet].genP4.pt();
	ev.ajn_idbits[ev.ajn]      = selAJetsId[ijet].idBits;
	ev.ajn_etaW[ev.ajn]        = selAJetsId[ijet].sihih;
	ev.ajn_phiW[ev.ajn]        = selAJetsId[ijet].sipip;
	ev.ajn_puminmva[ev.ajn]    = selAJetsId[ijet].mva[0];
	ev.ajn_pumva[ev.ajn]       = selAJetsId[ijet].mva[1];
	ev.ajn_rawsf[ev.ajn]       = selAJetsId[ijet].ensf;
	ev.ajn_isBfromGsplit[ev.ajn]     = (getHFmatchFromGSplit(selaJets[ijet],hfFromGsplit,5)!=0);
	ev.ajn_isCfromGsplit[ev.ajn]     = (getHFmatchFromGSplit(selaJets[ijet],hfFromGsplit,4)!=0);
	ev.ajn_lxy[ev.ajn]              = selAJetsId[ijet].lxy;
	ev.ajn_lxyErr[ev.ajn]           = selAJetsId[ijet].slxy;
	ev.ajn_svmass[ev.ajn]           = selAJetsId[ijet].svmass;
	ev.ajn_svpt[ev.ajn]             = selAJetsId[ijet].svpt;
	ev.ajn_svdr[ev.ajn]             = selAJetsId[ijet].svdr;
	ev.ajn++;
      }
   
    //
    // MET SELECTION
    //
    ev.nmet=0;
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
	  if(clustMetH.isValid())
	    {
	      LorentzVector iclustMet(clustMetH->px(),clustMetH->py(),0,clustMetH->pt());
	      ev.met_phi[i+1]  = iclustMet.phi();   ev.met_pt[i+1]  = iclustMet.pt();
	    }
	  else
	    {
	      ev.met_phi[i+1]  = 0;    ev.met_pt[i+1]  = -1;
	    }
	}

    }catch(std::exception &e){
      cout << e.what() << endl;
    }

    // finish event summary
    ev.nmeasurements=0;
    summaryHandler_.fillTree();
  }catch(std::exception &e){
    if(iErr_<10) std::cout << "[DileptonPlusMETEventAnalyzer][analyze] failed with " << e.what() << std::endl;
    if(iErr_==10) std::cout << "Suppressing further this error" << endl;
    iErr_++;
  }


}

//
void DileptonPlusMETEventAnalyzer::beginLuminosityBlock(const edm::LuminosityBlock&lumi, const edm::EventSetup & setup ) 
{
  edm::Handle<LumiSummary> l;
  lumi.getByLabel("lumiProducer", l); 
  if (!l.isValid())  return;
  curAvgInstLumi_ = l->avgInsDelLumi();
  curIntegLumi_   = l->intgDelLumi();
  controlHistos_.fillHisto("instlumi","all",curAvgInstLumi_);
  controlHistos_.fillHisto("integlumi","all",curIntegLumi_);
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
void DileptonPlusMETEventAnalyzer::beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup) 
{
  bool changed(true);
  hltConfig_.init(iRun, iSetup,"HLT",changed);
}


 
 


DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);

