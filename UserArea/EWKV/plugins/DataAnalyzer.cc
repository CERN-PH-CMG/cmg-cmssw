#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

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
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"

#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"
#include "EGamma/EGammaAnalysisTools/interface/PFIsolationEstimator.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "CMGTools/External/interface/PileupJetIdAlgo.h"

#include "TH1D.h"

#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"
#include "UserCode/EWKV/interface/MacroUtils.h"

using namespace std;
using namespace edm;
using namespace reco;

//
class DataAnalyzer : public edm::EDAnalyzer 
{

public:
  DataAnalyzer(const edm::ParameterSet &iConfig);
  void beginLuminosityBlock(const edm::LuminosityBlock &iLumi, const edm::EventSetup & iSetup );
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  virtual void beginRun(const edm::Run & iRun, edm::EventSetup const & iSetup); 
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
 
private:

  //monitoring
  DataEventSummaryHandler summary_;
  TH1D *obsPU_h, *truePU_h, *cutflow_h, *trigger_h,*filter_h;
  
  //selection configuration
  edm::ParameterSet analysisCfg_;
  
  //tool to trace prescale changes
  HLTConfigProvider hltConfig_;

  //pf isolation for e/g objects
  PFIsolationEstimator eIsolator03_, eIsolator04_, gIsolator03_, gIsolator04_;

  //pileup jet id
  PileupJetIdAlgo *puJetIdAlgo_;
};

using namespace std;


//
DataAnalyzer::DataAnalyzer(const edm::ParameterSet &iConfig) : obsPU_h(0), truePU_h(0)
{
  //configure selection
  analysisCfg_ = iConfig.getParameter<edm::ParameterSet>("cfg");
  std::vector<string> trigs=analysisCfg_.getParameter<std::vector<string> >("triggerPaths");
  std::vector<string> filts=analysisCfg_.getParameter<std::vector<string> >("metFilters");

  //init monitoring tools
  edm::Service<TFileService> fs;
  summary_.init(  fs->make<TTree>("data","Event Summary") );
  obsPU_h   = fs->make<TH1D>( "pileup",     ";Pileup; Events",      100,-0.5,99.5);
  truePU_h  = fs->make<TH1D>( "pileuptrue", ";True pileup; Events", 100,-0.5,99.5);
  cutflow_h = fs->make<TH1D>( "cutflow",    ";Cutflow; Events",     3,0,3);
  string selSteps[]={"RECO","No scrapping","#geq 1 vertex"};
  for(size_t istep=0; istep<sizeof(selSteps)/sizeof(string); istep++)  cutflow_h->GetXaxis()->SetBinLabel(istep+1,selSteps[istep].c_str());
  trigger_h = fs->make<TH1D>( "trigger",    ";Trigger path;",       trigs.size(),0,trigs.size());
  for(size_t itrig=0; itrig<trigs.size(); itrig++) trigger_h->GetXaxis()->SetBinLabel(itrig+1,trigs[itrig].c_str());
  filter_h = fs->make<TH1D>( "filter",    ";Filter bits;",       filts.size(),0,filts.size());
  for(size_t ifilt=0; ifilt<filts.size(); ifilt++) filter_h->GetXaxis()->SetBinLabel(ifilt+1,filts[ifilt].c_str());

  //init e/g PF isolation tools
  eIsolator03_.initializeElectronIsolation(true); eIsolator03_.setConeSize(0.3);
  eIsolator04_.initializeElectronIsolation(true); eIsolator04_.setConeSize(0.4);
  gIsolator03_.initializePhotonIsolation(true);   gIsolator03_.setConeSize(0.3);
  gIsolator04_.initializePhotonIsolation(true);   gIsolator04_.setConeSize(0.4);

  //pileup jet id
  puJetIdAlgo_ = new PileupJetIdAlgo( analysisCfg_.getParameter< std::vector<edm::ParameterSet> >( "pujetidAlgo")[0] );
}

//
void DataAnalyzer::beginLuminosityBlock(const edm::LuminosityBlock&lumi, const edm::EventSetup & setup ) 
{
  edm::Handle<LumiSummary> l;
  lumi.getByLabel("lumiProducer", l); 
  if (!l.isValid())  return;
  summary_.getEvent().instLumi = l->avgInsDelLumi();
}

//
void DataAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  string filterCtrs[]={"startCounter","scrapCounter","vtxCounter"};
  for(size_t istep=0; istep<sizeof(filterCtrs)/sizeof(string); istep++)
    {
      try{
	edm::Handle<edm::MergeableCounter> ctrHandle;
	iLumi.getByLabel(filterCtrs[istep], ctrHandle);
	if(!ctrHandle.isValid()) continue;
	cutflow_h->Fill(istep,ctrHandle->value);
      }catch(std::exception){
      }
    }
}

//
void DataAnalyzer::beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup) 
{
  bool changed(true);
  hltConfig_.init(iRun, iSetup,"HLT",changed);
}

//
void DataAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) 
{
  bool isData=event.isRealData();

  //event summary to be filled
  DataEventSummary &ev=summary_.getEvent();
  
  //event header
  ev.run    = event.id().run();
  ev.lumi   = event.luminosityBlock();
  ev.event  = event.id().event();

  //filter bits
  ev.f_bits=0;
  std::vector<string> filts=analysisCfg_.getParameter<std::vector<string> >("metFilters");
  for(size_t ifilt=0; ifilt<filts.size(); ifilt++)
    {
      edm::Handle<bool> tagResultH;
      event.getByLabel(filts[ifilt],tagResultH);
      ev.f_bits |= tagResultH.isValid() ? ((*tagResultH) << ifilt) : 0;
    }


  //
  // GENERATOR LEVEL
  //
  if(!isData){

    //pileup
    edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
    event.getByType(puInfoH);
    ev.ngenITpu    = 0;
    ev.ngenOOTpu   = 0;
    ev.ngenOOTpum1 = 0;
    ev.ngenTruepu  = 0;
    if(puInfoH.isValid())
      {
	for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	  {
	    if(it->getBunchCrossing()==0)      { ev.ngenITpu += it->getPU_NumInteractions();   ev.ngenTruepu  = it->getTrueNumInteractions(); }
	    else if(it->getBunchCrossing()<0)  { ev.ngenOOTpum1 += it->getPU_NumInteractions(); }
	    else                               { ev.ngenOOTpu   += it->getPU_NumInteractions(); }
	  }
      }
    obsPU_h->Fill(ev.ngenITpu);
    truePU_h->Fill(ev.ngenTruepu);

    //pdf info
    edm::Handle<GenEventInfoProduct> genEventInfoProd;
    event.getByType( genEventInfoProd );
    if(genEventInfoProd.isValid())
      {
	ev.genWeight = genEventInfoProd->weight();
	ev.qscale = genEventInfoProd->qScale();
	if(genEventInfoProd->pdf())
	  {
	    ev.qscale = genEventInfoProd->pdf()->scalePDF;
	    ev.x1  = genEventInfoProd->pdf()->x.first;
	    ev.x2  = genEventInfoProd->pdf()->x.second;
	    ev.id1 = genEventInfoProd->pdf()->id.first;
	    ev.id2 = genEventInfoProd->pdf()->id.second;
	  }
	if(genEventInfoProd->binningValues().size()>0) ev.pthat = genEventInfoProd->binningValues()[0];
      }
    
    //matrix element info
    Handle<LHEEventProduct> lheH;
    event.getByType(lheH);
    if(lheH.isValid()) ev.nup=lheH->hepeup().NUP;

    //generator level event
    Handle<View<Candidate> > genParticlesH;
    event.getByLabel(analysisCfg_.getParameter<edm::InputTag>("genSource"), genParticlesH);

    ev.mcn=0;

    //analyze first hard process
    for(size_t i = 0; i < genParticlesH->size(); ++ i)
      {
	const reco::GenParticle & p = dynamic_cast<const reco::GenParticle &>( (*genParticlesH)[i] );
	if(p.status()!=3 && !(p.status()==1 && abs(p.pdgId()==22) && p.pt()>20) ) continue;
	ev.mc_id[ev.mcn]=p.pdgId();
	ev.mc_status[ev.mcn]=p.status();
	ev.mc_px[ev.mcn]=p.px();
	ev.mc_py[ev.mcn]=p.py();
	ev.mc_pz[ev.mcn]=p.pz();
	ev.mc_en[ev.mcn]=p.energy();
	ev.mc_lxy[ev.mcn]=0;

	//check if photon is prompt or radiated from quark/line
	if(fabs(p.pdgId())==22)
	  {
	    bool isPrompt(false);
	    for(size_t b = 0; b < p.numberOfMothers(); ++ b)
	      {
		const reco::Candidate *p_m = p.mother(b);
		if(abs(p_m->pdgId())>25) continue;
		isPrompt=true;
	      }
	    if(!isPrompt) ev.mc_lxy[ev.mcn]=99999.;
	  } 
	
	ev.mcn++;
      }

    //heavy flavors
    for (size_t i=0; i<genParticlesH->size(); i++)
      {
	const reco::GenParticle & p = dynamic_cast<const reco::GenParticle &>( (*genParticlesH)[i] );
	if(p.status()!=3 || abs(p.pdgId())!=5) continue;

	const reco::Candidate *fs = utils::cmssw::getGeneratorFinalStateFor(&p);
	if(fs->numberOfDaughters()==0) continue;
	fs = utils::cmssw::getGeneratorFinalStateFor( fs->daughter(0) );
	for(size_t j=0; j<fs->numberOfDaughters(); j++)
	  {
	    const reco::Candidate *d=fs->daughter(j);
	    if(d==0) continue;
	    int absid=abs(d->pdgId());
	    if(!utils::cmssw::isBhadron(absid)) continue;
	    
	    //find first stable particle to trace decay length
	    const reco::Candidate *gd=d;
	    while(1)
	      {
		if(gd->status()==1) break;
		if(gd==0 || gd->numberOfDaughters()==0) break;
		const reco::Candidate *newGd=gd; 
		for(size_t k=0; k<gd->numberOfDaughters(); k++)
		  {
		    if(gd->daughter(k)->pdgId()==22) continue;
		    if(gd->daughter(k)==gd) continue;
		    newGd=gd->daughter(k);
		  }
		if(gd==newGd) break;
		gd=newGd;
	      }

	    float vxMother(p.vx()),     vyMother(p.vy());
	    float vxDaughter(gd->vx()), vyDaughter(gd->vy());

	    //save B-hadron information
	    ev.mc_id[ev.mcn]     = d->pdgId();
	    ev.mc_status[ev.mcn] = d->status();
	    ev.mc_px[ev.mcn]     = d->px();  
	    ev.mc_py[ev.mcn]     = d->py();  
	    ev.mc_pz[ev.mcn]     = d->pz(); 
	    ev.mc_en[ev.mcn]     = d->energy();  
	    ev.mc_lxy[ev.mcn]    = sqrt(pow(vxMother-vxDaughter,2)+pow(vyMother-vyDaughter,2));
	    ev.mcn++; 
	  }
      }
  }
  
  //
  // trigger (require at least for data)
  //
  edm::InputTag trigSource              = analysisCfg_.getParameter<edm::InputTag>("triggerSource");
  std::vector<std::string> triggerPaths = analysisCfg_.getParameter<std::vector<std::string> >("triggerPaths");
  std::vector<int> triggerCats          = analysisCfg_.getParameter<std::vector<int> >("triggerCats");

  ev.tn = triggerPaths.size();  
  edm::Handle<edm::TriggerResults> triggerBitsH;
  event.getByLabel( trigSource, triggerBitsH);
  const edm::TriggerNames &triggerNames = event.triggerNames( *triggerBitsH );
  
  for(int i=0; i<ev.tn; i++) { ev.t_bits[i]=0; ev.t_prescale[i]=0; }
  for (size_t itrig = 0; itrig != triggerBitsH->size(); ++itrig)
    {
      if( !triggerBitsH->wasrun(itrig) ) continue;
      if( triggerBitsH->error(itrig) ) continue;
      if( !triggerBitsH->accept(itrig) ) continue;
      std::string trigName = triggerNames.triggerName(itrig);
      for(size_t it=0; it<triggerPaths.size(); it++)
	{
	  if(trigName.find(triggerPaths[it]) == std::string::npos) continue;
	  //cout << trigName << endl;
	  ev.t_bits[it]=true;
	  ev.t_prescale[it]=hltConfig_.prescaleValue(event, iSetup, trigName);
	  break;
	}
    }


  //
  // vertex, beam spot, avg energy densities
  //
  edm::Handle<reco::BeamSpot> beamSpotH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("beamSpotSource"), beamSpotH);
  edm::Handle<reco::VertexCollection> vtxH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("vtxSource"), vtxH);
  ev.nvtx=vtxH->size();
  if(ev.nvtx==0) return;
  reco::VertexRef primVtx(vtxH,0);

  edm::Handle< double > rho, rho25;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("rhoSource"),rho);
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("rho25Source"), rho25);
  ev.rho = *rho;
  ev.rho25 = *rho25;

  //particle flow candidates
  edm::Handle<reco::PFCandidateCollection> pfH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("pfSource"),pfH);

  //
  // charged leptons
  // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
  // https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification
  // https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPFBasedIsolation
  //
  ev.ln=0; ev.egn=0; ev.mn=0; 
  edm::Handle<View<Candidate> > muH, eH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("muonSource"),     muH);
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("electronSource"), eH);
  int nMuons(0), nElecs(0);
  edm::Handle<reco::ConversionCollection> convH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("conversionSource"), convH);
  EcalClusterLazyTools egLazyTool( event, iSetup, analysisCfg_.getParameter<edm::InputTag>("ebrechitsSource"), analysisCfg_.getParameter<edm::InputTag>("eerechitsSource") );

  for(size_t imu=0; imu< muH->size(); ++imu)
    {
      reco::CandidatePtr muonPtr    = muH->ptrAt(imu);
      const pat::Muon *muon         = dynamic_cast<const pat::Muon *>( muonPtr.get() );
      const reco::Candidate *genLep = muon->genLepton();

      //apply a loose pre-selection to our muon candidates 
      bool isPF( muon->isPFMuon() );
      bool isGlobal( muon->isGlobalMuon() );
      bool isTracker( muon->isTrackerMuon() );
      bool isLoose( isPF && (isGlobal || isTracker) );
      if(!isLoose) continue;
      if(muon->pt()<3 || fabs(muon->eta())>2.5) continue;
      
      //store information
      ev.ln_id[ev.ln]                         = 13*muon->charge();
      ev.ln_pid[ev.ln]                        = ev.mn;
      ev.ln_px[ev.ln]                         = muon->px();
      ev.ln_py[ev.ln]                         = muon->py();
      ev.ln_pz[ev.ln]                         = muon->pz();
      ev.ln_en[ev.ln]                         = muon->energy();
      ev.ln_genid[ev.ln]                      = genLep ? genLep->pdgId() :0;
      ev.ln_genpx[ev.ln]                      = genLep ? genLep->px() : 0;
      ev.ln_genpy[ev.ln]                      = genLep ? genLep->py() : 0;
      ev.ln_genpz[ev.ln]                      = genLep ? genLep->pz() : 0;
      ev.ln_genen[ev.ln]                      = genLep ? genLep->energy() : 0;
      ev.ln_trkpt[ev.ln]                      = isGlobal ? muon->globalTrack()->pt()                                    : muon->innerTrack()->pt();
      ev.ln_trketa[ev.ln]                     = isGlobal ? muon->globalTrack()->eta()                                   : muon->innerTrack()->eta();
      ev.ln_trkphi[ev.ln]                     = isGlobal ? muon->globalTrack()->phi()                                   : muon->innerTrack()->phi();
      ev.ln_trkchi2[ev.ln]                    = isGlobal ? muon->globalTrack()->normalizedChi2()                        : muon->innerTrack()->normalizedChi2();	  
      ev.ln_trkValidPixelHits[ev.ln]          = isGlobal ? muon->globalTrack()->hitPattern().numberOfValidPixelHits()   : muon->innerTrack()->hitPattern().numberOfValidPixelHits();
      ev.ln_trkValidTrackerHits[ev.ln]        = isGlobal ? muon->globalTrack()->hitPattern().numberOfValidTrackerHits() : muon->innerTrack()->hitPattern().numberOfValidTrackerHits();
      ev.ln_trkLostInnerHits[ev.ln]           = muon->innerTrack()->trackerExpectedHitsInner().numberOfLostHits();
      ev.ln_trkPtErr[ev.ln]                   = fabs(muon->innerTrack()->ptError()/muon->innerTrack()->pt());
      ev.ln_d0[ev.ln]                         = fabs(muon->muonBestTrack()->dxy(primVtx->position()));
      ev.ln_dZ[ev.ln]                         = fabs(muon->muonBestTrack()->dz(primVtx->position()));
      std::pair<bool,Measurement1D> ip3dRes   = utils::cmssw::getImpactParameter<reco::TrackRef>(muon->innerTrack(), primVtx, iSetup, true);
      ev.ln_ip3d[ev.ln]                       = ip3dRes.second.value();
      ev.ln_ip3dsig[ev.ln]                    = ip3dRes.second.significance();
      ev.ln_ecalIso03[ev.ln]                  = muon->isolationR03().emEt;
      ev.ln_hcalIso03[ev.ln]                  = muon->isolationR03().hadEt;
      ev.ln_trkIso03[ev.ln]                   = muon->isolationR03().sumPt;
      ev.ln_ecalIso04[ev.ln]                  = muon->isolationR05().emEt; //oh well....
      ev.ln_hcalIso04[ev.ln]                  = muon->isolationR05().hadEt;
      ev.ln_trkIso04[ev.ln]                   = muon->isolationR05().sumPt;
      ev.ln_gIso03[ev.ln]                     = muon->pfIsolationR03().sumPhotonEt;
      ev.ln_chIso03[ev.ln]                    = muon->pfIsolationR03().sumChargedHadronPt;
      ev.ln_puchIso03[ev.ln]                  = muon->pfIsolationR03().sumPUPt;
      ev.ln_nhIso03[ev.ln]                    = muon->pfIsolationR03().sumNeutralHadronEt;
      ev.ln_gIso04[ev.ln]                     = muon->pfIsolationR04().sumPhotonEt;
      ev.ln_chIso04[ev.ln]                    = muon->pfIsolationR04().sumChargedHadronPt;
      ev.ln_puchIso04[ev.ln]                  = muon->pfIsolationR04().sumPUPt;
      ev.ln_nhIso04[ev.ln]                    = muon->pfIsolationR04().sumNeutralHadronEt;
      ev.mn_nMatches[ev.mn]                   = muon->numberOfMatches();
      ev.mn_nMatchedStations[ev.mn]           = muon->numberOfMatchedStations();
      ev.mn_validMuonHits[ev.mn]              = isGlobal ? muon->globalTrack()->hitPattern().numberOfValidMuonHits() : 0.;
      ev.mn_innerTrackChi2[ev.mn]             = isTracker ? muon->innerTrack()->normalizedChi2() : 0.;
      ev.mn_trkLayersWithMeasurement[ev.mn]   = muon->track()->hitPattern().trackerLayersWithMeasurement();
      ev.mn_pixelLayersWithMeasurement[ev.mn] = isTracker ? muon->innerTrack()->hitPattern().pixelLayersWithMeasurement() : 0.;

      bool isTight( isPF                                 && isGlobal              
		    && fabs(ev.ln_d0[ev.ln])<0.2 	 && fabs(ev.ln_dZ[ev.ln])<0.5         && ev.ln_trkValidPixelHits[ev.ln]>0         && ev.ln_trkchi2[ev.ln]<10. 
		    && ev.mn_validMuonHits[ev.mn]>0.     && ev.mn_nMatchedStations[ev.mn]>1   && ev.mn_trkLayersWithMeasurement[ev.mn]>5 );
      bool isSoft(isTracker && muon->muonID("TMOneStationTight") 
		  && fabs(ev.ln_d0[ev.ln])<3.  && fabs(ev.ln_dZ[ev.ln])<30.
		  && ev.mn_trkLayersWithMeasurement[ev.mn]>5 && ev.mn_pixelLayersWithMeasurement[ev.mn]>1  && ev.mn_innerTrackChi2[ev.mn] < 1.8 );
      
      //save id summary
      ev.ln_idbits[ev.ln]                     = 
	( (int(muon->muonID("GlobalMuonPromptTight")) & 0x1)   << 0)
	| ( (int(muon->muonID("TMLastStationLoose")) & 0x1)    << 1)
	| ( (int(muon->muonID("TMLastStationTight")) & 0x1)    << 2)
	| ( (int(muon->muonID("TMLastStationAngTight")) & 0x1) << 3)
	| ( (int(muon->muonID("TMOneStationTight")) & 0x1)     << 4)
	| ( isTracker                                          << 5)
	| ( isGlobal                                           << 6)
	| ( isPF                                               << 7)
	| ( isLoose                                            << 8)
	| ( isSoft                                             << 9)
	| ( isTight                                            << 10);
      
      //increment
      ev.ln++;
      ev.mn++;
      if(muon->pt()>20) nMuons++;
    }
  
  for(size_t iele=0; iele< eH->size(); ++iele)
    {
      reco::CandidatePtr elePtr       = eH->ptrAt(iele);
      const pat::Electron *ele        = dynamic_cast<const pat::Electron *>( elePtr.get() );
      const reco::Candidate *genLep   = ele->genLepton();
      const reco::GsfElectron *gsfEle = dynamic_cast<const reco::GsfElectron *>(ele);

      //pre-selection
      if(ele->gsfTrack().isNull() || ele->superCluster().isNull() || gsfEle==0) continue;
      if(ele->pt()<10 || !(ele->isEB() || ele->isEE()) )                        continue;
      bool overlapFound(false);
      for(int ilep=0; ilep<ev.ln; ilep++)
	{
	  LorentzVector p4(ev.ln_px[ilep],ev.ln_py[ilep], ev.ln_pz[ilep], ev.ln_en[ilep]);
	  if( deltaR( p4.eta(), p4.phi(), ele->eta(), ele->phi()) > 0.1) continue;
	  overlapFound=true;
	  break;
	}
      if(overlapFound) continue;

      //store information
      ev.ln_id[ev.ln]                         = 11*ele->charge();
      ev.ln_pid[ev.ln]                        = ev.egn;
      ev.ln_px[ev.ln]                         = ele->px();
      ev.ln_py[ev.ln]                         = ele->py();
      ev.ln_pz[ev.ln]                         = ele->pz();
      ev.ln_en[ev.ln]                         = ele->energy();
      ev.ln_genid[ev.ln]                      = genLep ? genLep->pdgId() :0;
      ev.ln_genpx[ev.ln]                      = genLep ? genLep->px() : 0;
      ev.ln_genpy[ev.ln]                      = genLep ? genLep->py() : 0;
      ev.ln_genpz[ev.ln]                      = genLep ? genLep->pz() : 0;
      ev.ln_genen[ev.ln]                      = genLep ? genLep->energy() : 0;
      ev.ln_trkpt[ev.ln]                      = ele->gsfTrack()->pt();
      ev.ln_trketa[ev.ln]                     = ele->gsfTrack()->eta();
      ev.ln_trkphi[ev.ln]                     = ele->gsfTrack()->phi();
      ev.ln_trkchi2[ev.ln]                    = ele->gsfTrack()->normalizedChi2();
      ev.ln_trkValidPixelHits[ev.ln]          = ele->gsfTrack()->hitPattern().numberOfValidPixelHits();
      ev.ln_trkValidTrackerHits[ev.ln]        = ele->gsfTrack()->hitPattern().numberOfValidTrackerHits();
      ev.ln_trkLostInnerHits[ev.ln]           = ele->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits();
      ev.ln_trkPtErr[ev.ln]                   = fabs(ele->gsfTrack()->ptError()/ele->gsfTrack()->pt());
      ev.ln_d0[ev.ln]                         = fabs(ele->gsfTrack()->dxy(primVtx->position()));
      ev.ln_dZ[ev.ln]                         = fabs(ele->gsfTrack()->dz(primVtx->position()));
      std::pair<bool,Measurement1D> ip3dRes = utils::cmssw::getImpactParameter<reco::GsfTrackRef>(ele->gsfTrack(), primVtx, iSetup, true);
      ev.ln_ip3d[ev.ln]                       = ip3dRes.second.value();
      ev.ln_ip3dsig[ev.ln]                    = ip3dRes.second.significance();
      ev.ln_ecalIso03[ev.ln]                  = ele->dr03EcalRecHitSumEt();
      ev.ln_hcalIso03[ev.ln]                  = ele->dr03HcalTowerSumEt();
      ev.ln_trkIso03[ev.ln]                   = ele->dr03TkSumPt();
      ev.ln_ecalIso04[ev.ln]                  = ele->dr04EcalRecHitSumEt();
      ev.ln_hcalIso04[ev.ln]                  = ele->dr04HcalTowerSumEt();
      ev.ln_trkIso04[ev.ln]                   = ele->dr04TkSumPt();
      eIsolator03_.fGetIsolation(gsfEle, &(*pfH), primVtx, vtxH);
      ev.ln_gIso03[ev.ln]                     = eIsolator03_.getIsolationPhoton();
      ev.ln_chIso03[ev.ln]                    = eIsolator03_.getIsolationCharged();
      ev.ln_puchIso03[ev.ln]                  = 0;
      ev.ln_nhIso03[ev.ln]                    = eIsolator03_.getIsolationNeutral();
      eIsolator04_.fGetIsolation(gsfEle, &(*pfH), primVtx, vtxH);
      ev.ln_gIso04[ev.ln]                     = eIsolator04_.getIsolationPhoton();
      ev.ln_chIso04[ev.ln]                    = eIsolator04_.getIsolationCharged();
      ev.ln_puchIso04[ev.ln]                  = 0;
      ev.ln_nhIso04[ev.ln]                    = eIsolator04_.getIsolationNeutral();
      ev.egn_isConv[ev.egn]                   = ConversionTools::hasMatchedConversion(*gsfEle,convH,beamSpotH->position());
      ev.egn_eopin[ev.egn]                    = ele->eSuperClusterOverP(); 
      ev.egn_eopout[ev.egn]                   = ele->eEleClusterOverPout();
      ev.egn_sce[ev.egn]                      = ele->superCluster()->energy();
      ev.egn_sceta[ev.egn]                    = ele->superCluster()->eta();
      ev.egn_scphi[ev.egn]                    = ele->superCluster()->phi();
      ev.egn_fbrem[ev.egn]                    = ele->fbrem();
      ev.egn_sihih[ev.egn]                    = ele->sigmaIetaIeta();
      vector<float> cov                       = egLazyTool.localCovariances(*ele->superCluster()->seed()); 
      ev.egn_sipip[ev.egn]                    = sqrt(cov[2]); 
      ev.egn_sihip[ev.egn]                    = cov[1];
      ev.egn_r9[ev.egn]                       = egLazyTool.e3x3(*ele->superCluster()->seed())/ele->superCluster()->rawEnergy();
      ev.egn_mvatrigv0[ev.egn]                = ele->electronID("mvaTrigV0");
      ev.egn_mvanontrigv0[ev.egn]             = ele->electronID("mvaNonTrigV0");
      ev.egn_hoe[ev.egn]                      = ele->hadronicOverEm();
      ev.egn_dphiin[ev.egn]                   = ele->deltaPhiSuperClusterTrackAtVtx();
      ev.egn_detain[ev.egn]                   = ele->deltaEtaSuperClusterTrackAtVtx();
      ev.egn_ooemoop[ev.egn]                  = (1.0/ele->ecalEnergy() - ele->eSuperClusterOverP()/ele->ecalEnergy());

      //save id summary
      bool hasVetoId(false);
      ev.ln_idbits[ev.ln] = 
	(ele->ecalDrivenSeed()                                                                                  << 0)
	| ( ele->trackerDrivenSeed()                                                                            << 1)
	| ( EgammaCutBasedEleId::PassEoverPCuts(ev.egn_sceta[ev.egn],ev.egn_eopin[ev.egn],ev.egn_fbrem[ev.egn]) << 2);

      for(size_t iid=0; iid<4; iid++)
	{
	  int id(EgammaCutBasedEleId::VETO);
	  if(iid==1) id=EgammaCutBasedEleId::LOOSE;
	  if(iid==2) id=EgammaCutBasedEleId::MEDIUM;
	  if(iid==3) id=EgammaCutBasedEleId::TIGHT;
	  bool hasId=EgammaCutBasedEleId::PassWP( EgammaCutBasedEleId::WorkingPoint(id), ele->isEB(), ele->pt(), ele->eta(), 
						  ev.egn_detain[ev.egn], ev.egn_dphiin[ev.egn], ev.egn_sihih[ev.egn], ev.egn_hoe[ev.egn], ev.egn_ooemoop[ev.egn], 
						  ev.ln_d0[ev.ln], ev.ln_dZ[ev.ln], 
						  0., 0., 0., ev.egn_isConv[ev.egn], ev.ln_trkLostInnerHits[ev.ln], *rho);
	  if(iid==0) hasVetoId=hasId;
	  ev.ln_idbits[ev.ln] |=  (hasId << (3+iid));
	}
      for(size_t iid=0; iid<2; iid++)
	{
	  int id(EgammaCutBasedEleId::TRIGGERTIGHT);
	  if(iid==1) id=EgammaCutBasedEleId::TRIGGERWP70;
	  bool hasId = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TriggerWorkingPoint(id),
							    ele->isEB(), ele->pt(),
							    ev.egn_detain[ev.egn], ev.egn_dphiin[ev.egn], ev.egn_sihih[ev.egn], ev.egn_hoe[ev.egn],
							    ev.ln_trkIso03[ev.ln], ev.ln_ecalIso03[ev.ln], ev.ln_hcalIso03[ev.ln]);
	  ev.ln_idbits[ev.ln] |= (hasId << (7+iid));
	}
      
      //require a very loose baseline id
      if(!hasVetoId) continue;

      //increment counters
      ev.ln++;
      ev.egn++;
      if(ele->pt()>20) nElecs++;
    }

  //
  // photon selection
  // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012
  //
  ev.gn=0;
  int nPhotons(0);
  edm::Handle<edm::View<reco::Candidate> > photonH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("photonSource"), photonH );
  edm::Handle<reco::GsfElectronCollection> gsfEleH;
  event.getByLabel( "gsfElectrons", gsfEleH );
  for(size_t ipho=0; ipho<photonH->size(); ipho++)
      {
	const reco::Photon *pho = dynamic_cast<const reco::Photon *>( photonH->ptrAt(ipho).get() );
	if(pho==0) continue;
	if(pho->pt()<30 || !(pho->isEB() || pho->isEE())) continue;
	bool matchesElectron(ConversionTools::hasMatchedPromptElectron(pho->superCluster(), gsfEleH, convH, beamSpotH->position()));
	bool matchesMuon(false);
	for(int ilep=0; ilep<ev.ln; ilep++)
	  {
	    if(fabs(ev.ln_id[ilep])==11) continue;
	    LorentzVector p4(ev.ln_px[ilep],ev.ln_py[ilep], ev.ln_pz[ilep], ev.ln_en[ilep]);
	    if( deltaR( p4.eta(), p4.phi(), pho->eta(), pho->phi()) > 0.1) continue;
	    matchesMuon=true;
	    break;
	  }
	if(matchesElectron || matchesMuon) continue;
	
	ev.gn_pid[ev.gn]                        = ev.egn;
	ev.gn_px[ev.gn]                         = pho->px();
	ev.gn_py[ev.gn]                         = pho->py();
	ev.gn_pz[ev.gn]                         = pho->pz();
	ev.gn_en[ev.gn]                         = pho->energy();
	ev.gn_ecalIso03[ev.ln]                  = pho->ecalRecHitSumEtConeDR03();
	ev.gn_hcalIso03[ev.ln]                  = pho->hcalTowerSumEtConeDR03();
	ev.gn_trkIso03[ev.ln]                   = pho->trkSumPtHollowConeDR03();
	ev.gn_ecalIso04[ev.ln]                  = pho->ecalRecHitSumEtConeDR04();
	ev.gn_hcalIso04[ev.ln]                  = pho->hcalTowerSumEtConeDR04();
	ev.gn_trkIso04[ev.ln]                   = pho->trkSumPtHollowConeDR04();
	gIsolator03_.fGetIsolation(pho, &(*pfH), primVtx, vtxH);
	ev.gn_gIso03[ev.ln]                     = gIsolator03_.getIsolationPhoton();
	ev.gn_chIso03[ev.ln]                    = gIsolator03_.getIsolationCharged();
	ev.gn_puchIso03[ev.ln]                  = 0;
	ev.gn_nhIso03[ev.ln]                    = gIsolator03_.getIsolationNeutral();
	eIsolator04_.fGetIsolation(pho, &(*pfH), primVtx, vtxH);
	ev.gn_gIso04[ev.ln]                     = gIsolator04_.getIsolationPhoton();
	ev.gn_chIso04[ev.ln]                    = gIsolator04_.getIsolationCharged();
	ev.gn_puchIso04[ev.ln]                  = 0;
	ev.gn_nhIso04[ev.ln]                    = gIsolator04_.getIsolationNeutral();
	ev.egn_isConv[ev.egn]                   = !( ConversionTools::matchedConversion(*(pho->superCluster()),convH,beamSpotH->position()).isNull() );
	ev.egn_eopin[ev.egn]                    = 0;
	ev.egn_eopout[ev.egn]                   = 0;
	ev.egn_sce[ev.egn]                      = pho->superCluster()->energy();
	ev.egn_sceta[ev.egn]                    = pho->superCluster()->eta();
	ev.egn_scphi[ev.egn]                    = pho->superCluster()->phi();
	ev.egn_fbrem[ev.egn]                    = 0;
	ev.egn_sihih[ev.egn]                    = pho->sigmaIetaIeta();
	vector<float> cov                       = egLazyTool.localCovariances(*pho->superCluster()->seed()); 
	ev.egn_sipip[ev.egn]                    = sqrt(cov[2]); 
	ev.egn_sihip[ev.egn]                    = cov[1];
	ev.egn_r9[ev.egn]                       = egLazyTool.e3x3(*pho->superCluster()->seed())/pho->superCluster()->rawEnergy();
	ev.egn_mvatrigv0[ev.egn]                = 0;
	ev.egn_mvanontrigv0[ev.egn]             = 0;
	ev.egn_hoe[ev.egn]                      = pho->hadTowOverEm();
	ev.egn_dphiin[ev.egn]                   = 0;
	ev.egn_detain[ev.egn]                   = 0;
	ev.egn_ooemoop[ev.egn]                  = 0;

	bool isLoose ( ev.egn_sihih[ev.egn]<0.012 && ev.egn_hoe[ev.egn]<0.05 );
	bool isMedium( ev.egn_sihih[ev.egn]<0.011 && ev.egn_hoe[ev.egn]<0.05 );
	bool isTight ( ev.egn_sihih[ev.egn]<0.011 && ev.egn_hoe[ev.egn]<0.05 );
	if(pho->isEE())
	  {
	    isLoose  = (ev.egn_sihih[ev.egn]<0.034 && ev.egn_hoe[ev.egn]<0.05 );
	    isMedium = (ev.egn_sihih[ev.egn]<0.033 && ev.egn_hoe[ev.egn]<0.05 );
	    isTight  = (ev.egn_sihih[ev.egn]<0.031 && ev.egn_hoe[ev.egn]<0.05 );
	  }

	ev.gn_idbits[ev.gn]    = (isLoose << 0) | (isMedium << 1 ) | (isTight << 2);
	ev.gn++;
	ev.egn++;
	if(isTight && pho->isEB() && ev.egn_r9[ev.egn]>0.9 && pho->pt()>36) nPhotons++;
      }


  //now check if at least one trigger condition is fullfilled
  bool toSave(false);
  for(int itrig=0; itrig<ev.tn; itrig++)
    {
      if(!ev.t_bits[itrig]) continue;
      int cat=triggerCats[itrig];
      if     (cat==11   && nElecs==0)                continue;
      else if(cat==13   && nMuons==0)                continue;
      else if(cat==22   && nPhotons==0)              continue;
      else if(cat==1111 && nElecs<2)                 continue;
      else if(cat==1113 && (nMuons==0 || nElecs==0)) continue;
      else if(cat==1313 && nMuons<2)                 continue;
      toSave=true;
      break;
    }
  if(!toSave) return;


  
  //
  // jets
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  // https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID
  // https://twiki.cern.ch/twiki/bin/view/CMS/GluonTag
  // 
  ev.jn=0; ev.pfn=0;
  Handle<pat::JetCollection> jetH;
  event.getByLabel( analysisCfg_.getParameter<edm::InputTag>("jetSource"), jetH);
  edm::Handle<edm::ValueMap<float> >  qgTaggerH;
  event.getByLabel("QGTagger","qgLikelihood", qgTaggerH);
  edm::Handle<reco::JetTagCollection> tchpTags,   jpTags,    ssvheTags,    ivfTags,    origcsvTags,    csvTags,    jpcsvTags,    slcsvTags, supercsvTags;
  event.getByLabel("trackCountingHighPurBJetTags",                  tchpTags);          
  event.getByLabel("jetProbabilityBJetTags",                        jpTags);            
  event.getByLabel("simpleSecondaryVertexHighEffBJetTags",          ssvheTags);  
  event.getByLabel("simpleInclusiveSecondaryVertexHighEffBJetTags", ivfTags); 
  event.getByLabel("combinedSecondaryVertexBJetTags",               origcsvTags);       
  event.getByLabel("combinedSecondaryVertexRetrainedBJetTags",      csvTags);           
  event.getByLabel("combinedCSVJPBJetTags",                         jpcsvTags);         
  event.getByLabel("combinedCSVSLBJetTags",                         slcsvTags);         
  event.getByLabel("combinedCSVJPSLBJetTags",                 supercsvTags);      
  edm::Handle<std::vector<reco::SecondaryVertexTagInfo> > svTagInfo, ivfTagInfo;
  event.getByLabel("secondaryVertexTagInfos",                        svTagInfo);
  event.getByLabel("inclusiveSecondaryVertexFinderTagInfosFiltered", ivfTagInfo);

  for(size_t ijet=0; ijet<jetH->size(); ++ijet)
    {
      edm::RefToBase<pat::Jet> jetRef(edm::Ref<pat::JetCollection>(jetH,ijet));
      const pat::Jet *jet              = &((*jetH)[ijet]);
      const reco::Candidate *genParton = jet->genParton();
      const reco::GenJet *genJet       = jet->genJet();

      //pre-selection
      float nhf    = ( jet->neutralHadronEnergy() + jet->HFHadronEnergy() ) / jet->energy();
      float nef    = jet->neutralEmEnergyFraction();
      float nconst = jet->numberOfDaughters();
      float cef    = jet->chargedEmEnergyFraction();
      float chf    = jet->chargedHadronEnergyFraction();
      float nch    = jet->chargedMultiplicity();
      bool passLooseId(nhf<0.99  && nef<0.99 && nconst>1);
      bool passMediumId(nhf<0.95 && nef<0.95 && nconst>1);
      bool passTightId(nhf<0.90  && nef<0.90 && nconst>1);
      if(fabs(jet->eta())<2.4) {
	passLooseId  &= (chf>0 && nch>0 && cef<0.99);
	passMediumId &= (chf>0 && nch>0 && cef<0.99);
	passTightId  &= (chf>0 && nch>0 && cef<0.99);
      }
      if(jet->pt()<10 || fabs(jet->eta())>4.7 || !passLooseId) continue;
      
      //save information
      ev.jn_px[ev.jn]          = jet->px();
      ev.jn_py[ev.jn]          = jet->py();
      ev.jn_pz[ev.jn]          = jet->pz();
      ev.jn_en[ev.jn]          = jet->energy();
      ev.jn_torawsf[ev.jn]     = jet->correctedJet("Uncorrected").pt()/jet->pt();
      ev.jn_genflav[ev.jn]     = jet->partonFlavour();
      ev.jn_genid[ev.jn]       = genParton ? genParton->pdgId() : 0;
      ev.jn_genpx[ev.jn]       = genParton ? genParton->px()    : 0;
      ev.jn_genpy[ev.jn]       = genParton ? genParton->py()    : 0;
      ev.jn_genpz[ev.jn]       = genParton ? genParton->pz()    : 0;
      ev.jn_genen[ev.jn]       = genParton ? genParton->energy(): 0;
      ev.jn_genjpx[ev.jn]      = genJet    ? genJet->px()       : 0;
      ev.jn_genjpy[ev.jn]      = genJet    ? genJet->py()       : 0;
      ev.jn_genjpz[ev.jn]      = genJet    ? genJet->pz()       : 0;
      ev.jn_genjen[ev.jn]      = genJet    ? genJet->energy()   : 0;
      ev.jn_neutHadFrac[ev.jn] = jet->neutralHadronEnergyFraction();
      ev.jn_neutEmFrac[ev.jn]  = jet->neutralEmEnergyFraction();
      ev.jn_chHadFrac[ev.jn]   = jet->chargedHadronEnergyFraction();;
      ev.jn_area[ev.jn]        = jet->jetArea();

      ev.jn_tchp[ev.jn]        = (*tchpTags)[ijet].second;
      ev.jn_jp[ev.jn]          = (*jpTags)[ijet].second;
      ev.jn_ssvhe[ev.jn]       = (*ssvheTags)[ijet].second;
      ev.jn_ivf[ev.jn]         = (*ivfTags)[ijet].second;
      ev.jn_origcsv[ev.jn]     = (*origcsvTags)[ijet].second;
      ev.jn_csv[ev.jn]         = (*csvTags)[ijet].second;
      ev.jn_jpcsv[ev.jn]       = (*jpcsvTags)[ijet].second;
      ev.jn_slcsv[ev.jn]       = (*slcsvTags)[ijet].second;
      ev.jn_supercsv[ev.jn]    = (*supercsvTags)[ijet].second;

      //secondary vertex from associated tracks
      if(svTagInfo.isValid() && svTagInfo->size()>ijet);
      {
	const reco::SecondaryVertexTagInfo &sv= (*svTagInfo)[ijet];
	int nsvtx=sv.nVertices();
	ev.jn_svxNtrk[ev.jn]=0;
	ev.jn_svxLxy[ev.jn]=0;
	ev.jn_svxLxyErr[ev.jn]=0;
	ev.jn_svxM[ev.jn]=0;
	ev.jn_svxPx[ev.jn]=0;
	ev.jn_svxPy[ev.jn]=0;
	ev.jn_svxPz[ev.jn]=0;
	if(nsvtx)
	  {  
	    for (reco::Vertex::trackRef_iterator titt = sv.secondaryVertex(0).tracks_begin(); titt != sv.secondaryVertex(0).tracks_end(); titt++) ev.jn_svxNtrk[ev.jn]++;
	    ev.jn_svxLxy[ev.jn]    = sv.flightDistance(0).value();
	    ev.jn_svxLxyErr[ev.jn] = sv.flightDistance(0).error();
	    ev.jn_svxM[ev.jn]      = sv.secondaryVertex(0).p4().mass();
	    ev.jn_svxPx[ev.jn]     = sv.secondaryVertex(0).p4().px();
	    ev.jn_svxPy[ev.jn]     = sv.secondaryVertex(0).p4().py();
	    ev.jn_svxPz[ev.jn]     = sv.secondaryVertex(0).p4().pz();
	  }
      }
      
      //secondary vertex from inclusive tracks
      if(ivfTagInfo.isValid() && ivfTagInfo->size()>ijet)
	{
	  const reco::SecondaryVertexTagInfo &sv= (*ivfTagInfo)[ijet];
	  int nsvtx=sv.nVertices();
	  ev.jn_ivfNtrk[ev.jn]=0;
	  ev.jn_ivfLxy[ev.jn]=0;
	  ev.jn_ivfLxyErr[ev.jn]=0;
	  ev.jn_ivfM[ev.jn]=0;
	  ev.jn_ivfPx[ev.jn]=0;
	  ev.jn_ivfPy[ev.jn]=0;
	  ev.jn_ivfPz[ev.jn]=0;
	  if(nsvtx)
	    {
	      for (reco::Vertex::trackRef_iterator titt = sv.secondaryVertex(0).tracks_begin(); titt != sv.secondaryVertex(0).tracks_end(); titt++) ev.jn_ivfNtrk[ev.jn]++;
	      ev.jn_ivfLxy[ev.jn]    = sv.flightDistance(0).value();
	      ev.jn_ivfLxyErr[ev.jn] = sv.flightDistance(0).error();
	      ev.jn_ivfM[ev.jn]      = sv.secondaryVertex(0).p4().mass();
	      ev.jn_ivfPx[ev.jn]     = sv.secondaryVertex(0).p4().px();
	      ev.jn_ivfPy[ev.jn]     = sv.secondaryVertex(0).p4().py();
	      ev.jn_ivfPz[ev.jn]     = sv.secondaryVertex(0).p4().pz();
	    }
	}

      PileupJetIdentifier puIdentifier = puJetIdAlgo_->computeIdVariables(dynamic_cast<const reco::Jet*>(jet), 0, primVtx.get(), *vtxH.product(), true);
      ev.jn_beta[ev.jn]        = puIdentifier.beta();
      ev.jn_dRMean[ev.jn]      = puIdentifier.dRMean();
      ev.jn_ptRMS[ev.jn]       = puIdentifier.ptRMS();
      ev.jn_etaW[ev.jn]        = puIdentifier.etaW();
      ev.jn_phiW[ev.jn]        = puIdentifier.phiW();
      ev.jn_puMVA[ev.jn]       = puIdentifier.mva();
      
      //save pf constituents (only for jets with pT>20 in the central region)
      ev.jn_pfstart[ev.jn]=-1;
      ev.jn_pfend[ev.jn]=-1;
      if(jet->pt()>20 && fabs(jet->eta())<3)
	{
	  const std::vector<reco::PFCandidatePtr> pfConst = jet->getPFConstituents();
	  ev.jn_pfstart[ev.jn]=ev.pfn;
	  for(size_t ipf=0; ipf<pfConst.size(); ipf++)
	    {
	      ev.pf_id[ev.pfn]     = pfConst[ipf]->pdgId();
	      ev.pf_charge[ev.pfn] = pfConst[ipf]->charge();
	      ev.pf_px[ev.pfn]     = pfConst[ipf]->px();
	      ev.pf_py[ev.pfn]     = pfConst[ipf]->py();
	      ev.pf_pz[ev.pfn]     = pfConst[ipf]->pz();
	      ev.pf_en[ev.pfn]     = pfConst[ipf]->energy();
	      ev.pfn++;
	    }
	  ev.jn_pfend[ev.jn]=ev.pfn-1;
	}

      //a summary of the id bits
      ev.jn_idbits[ev.jn] =
	(passLooseId << 0)
	| (passMediumId << 1)
	| (passTightId << 2)
	| ( ( uint(puIdentifier.idFlag()) & 0xf ) << 3 );
      
      ev.jn++;
    }
  
  //
  // missing transverse energy
  //
  std::vector<edm::InputTag> metSources=analysisCfg_.getParameter<std::vector<edm::InputTag> >("metSource");
  ev.metn=0;
  for(size_t imet=0; imet<metSources.size(); imet++)
    {
      Handle<View<reco::PFMET> > metH;
      event.getByLabel(metSources[imet], metH);
      ev.met_pt[ev.metn]    = metH.isValid() ? metH->ptrAt(0)->pt() : 0; 
      ev.met_phi[ev.metn]   = metH.isValid() ? metH->ptrAt(0)->phi() : 0; 
      ev.met_sigx2[ev.metn] = metH.isValid() ? metH->ptrAt(0)->getSignificanceMatrix()(0,0) : 0;
      ev.met_sigxy[ev.metn] = metH.isValid() ? metH->ptrAt(0)->getSignificanceMatrix()(0,1) : 0;
      ev.met_sigy2[ev.metn] = metH.isValid() ? metH->ptrAt(0)->getSignificanceMatrix()(1,1) : 0;
      Float_t significance(0.);
      if(metH.isValid() && ev.met_sigx2[ev.metn]<1.e10 && ev.met_sigy2[ev.metn]<1.e10) significance = metH->ptrAt(0)->significance();
      ev.met_sig[ev.metn] = significance;
      ev.metn++;
    }    


  //
  // charged PF candidates which haven't been clustered
  //
  for(size_t ipf=0; ipf<pfH->size(); ipf++)
    {
      const reco::PFCandidate &cand=(*pfH)[ipf];

      //require charged and with track associated
      if(cand.charge()==0) continue;
      reco::TrackBaseRef trackBaseRef( cand.trackRef() );
      if(trackBaseRef.isNull()) continue;

      //minimum pT of 500 GeV
      if(cand.pt()<0.3) continue;
      
      //check for overlaps
      bool matches(false);
      for(int jpf=0; jpf<ev.pfn; jpf++)
	{
	  LorentzVector p4(ev.pf_px[jpf],ev.pf_py[jpf], ev.pf_pz[jpf], ev.pf_en[jpf]);
	  if( deltaR( p4.eta(), p4.phi(), cand.eta(), cand.phi()) > 0.1) continue;
	  matches=true;
	  break;
	}
      if(matches) continue;
      
      //require it to be associated to the primary vertex
      int bestVtx(-1);
      if(trackBaseRef.isAvailable())
	{
	  float bestDz(9999.);
	  for(size_t jVtx=0; jVtx<vtxH->size(); jVtx++)
	    {
	      const reco::VertexRef vtxref(vtxH,jVtx);
	      float vtxDz( fabs( trackBaseRef->dz( vtxref->position()) ) );
	      if(vtxDz > bestDz) continue;
	      bestDz=vtxDz;
	      bestVtx=jVtx;
	    }
	} 
      if(bestVtx!=0) continue;

      ev.pf_id[ev.pfn]     = cand.pdgId();
      ev.pf_charge[ev.pfn] = cand.charge();
      ev.pf_px[ev.pfn]     = cand.px();
      ev.pf_py[ev.pfn]     = cand.py();
      ev.pf_pz[ev.pfn]     = cand.pz();
      ev.pf_en[ev.pfn]     = cand.energy();
      ev.pfn++;
    }



  //all done here
  summary_.fill();
}




DEFINE_FWK_MODULE(DataAnalyzer);

