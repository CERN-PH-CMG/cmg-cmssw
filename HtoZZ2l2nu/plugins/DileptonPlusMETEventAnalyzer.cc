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

#include "HiggsAnalysis/HiggsToGammaGamma/interface/EGEnergyCorrector.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/ProjectedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

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

  float saveMCtruth(const edm::Event &event, const edm::EventSetup &iSetup );

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
  ProjectedMETComputer pmet_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
  EventCategory eventClassifComp_;
  EGEnergyCorrector       ecorr_;
};

using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
  : controlHistos_( iConfig.getParameter<std::string>("dtag") )
{
  try{

    std::string objs[]={"Generator", "Trigger", "Vertices", "Photons", "Electrons", "LooseElectrons", "Muons", "LooseMuons", "Dileptons", "Jets", "MET" };
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
  }
  catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
float DileptonPlusMETEventAnalyzer::saveMCtruth(const edm::Event &event, const edm::EventSetup &iSetup)
{
  ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
  ev.nmcparticles=0;

  float weight(1.0);
  ev.weight = 1.0;
  if(event.isRealData())  return weight;

  //pileup
  edm::Handle<float> puWeightHandle;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("puReweight"), puWeightHandle );
  if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
  ev.weight = weight;

  edm::Handle<float> normPuWeightHandle;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("normPuReweight"), normPuWeightHandle );
  if(normPuWeightHandle.isValid()) ev.normWeight = *(normPuWeightHandle.product());
  
  edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
  event.getByType(puInfoH);
  int npuOOT(0),npuIT(0),npuOOTm1(0);
  if(puInfoH.isValid())
    {
      for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	{
	  if(it->getBunchCrossing()==0) npuIT += it->getPU_NumInteractions();
	  else                          npuOOT += it->getPU_NumInteractions();
	  if(it->getBunchCrossing()<0)  npuOOTm1 += it->getPU_NumInteractions();
	}
    }
  ev.ngenITpu=npuIT;
  ev.ngenOOTpu=npuOOT;
  ev.ngenOOTpum1=npuOOTm1;

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

  //Higgs pT reweighting (for Powheg gg->H)
  try{
    ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]=1;
    ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]=1;
    ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]=1;
    ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]=1;
    ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown]=1;

    int iweight(0);
    std::vector<edm::InputTag> ptWeightSources=objConfig_["Generator"].getParameter<std::vector<edm::InputTag> >("higgsPtWeights");
    for(std::vector<edm::InputTag>::iterator tIt=ptWeightSources.begin();
	tIt!=ptWeightSources.end();
	tIt++,iweight++)
      {
	edm::Handle<double> hkfactorHandle;
	event.getByLabel(*tIt,hkfactorHandle);
	ev.hptWeights[iweight]=*hkfactorHandle;
      }
  }catch(std::exception &e){

  }

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
  int ngenjets(0);
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
      
      ngenjets++;
      ev.mc_px[ev.nmcparticles]=gjIt->px();  
      ev.mc_py[ev.nmcparticles]=gjIt->py();  
      ev.mc_pz[ev.nmcparticles]=gjIt->pz(); 
      ev.mc_en[ev.nmcparticles]=gjIt->energy();  
      ev.mc_id[ev.nmcparticles]=1;
      ev.nmcparticles++;
    }
    
  //all done return the event weight
  return weight;
}


//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) 
{

  try{
    //event summary to be filled
    summaryHandler_.resetStruct();
    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
   
    //event header
    ev.run    = event.id().run();
    ev.lumi   = event.luminosityBlock();
    ev.event  = event.id().event();
    ev.weight = saveMCtruth(event, iSetup );    
    
    //
    // trigger
    //
    edm::Handle<edm::TriggerResults> triggerBitsH;
    edm::InputTag trigSource = objConfig_["Trigger"].getParameter<edm::InputTag>("source");
    event.getByLabel( trigSource, triggerBitsH);
    const edm::TriggerNames &triggerNames = event.triggerNames( *triggerBitsH );
    std::vector<std::string> triggerPaths = objConfig_["Trigger"].getParameter<std::vector<std::string> >("triggerPaths");
    ev.hasTrigger = checkIfTriggerFired( triggerBitsH, triggerNames, triggerPaths ); 
    std::vector<std::string> gammaTriggers=objConfig_["Trigger"].getParameter< std::vector<std::string> >("gammaTriggers");
    std::pair<std::string,double> photonTrig = getHighestPhotonTrigThreshold( triggerBitsH, triggerNames , gammaTriggers );
    
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
    edm::Handle< double > rho;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"),rho);
    ev.rho = *rho;

    //
    // LEPTON SELECTION
    //
    
    //muon selection
    Handle<View<Candidate> > hMu;
    event.getByLabel(objConfig_["Muons"].getParameter<edm::InputTag>("source"), hMu);
    std::vector<CandidatePtr> selLooseMuons = getGoodMuons(hMu, *beamSpot, *rho, objConfig_["LooseMuons"]);
    std::vector<CandidatePtr> selMuons      = getGoodMuons(hMu, *beamSpot, *rho, objConfig_["Muons"]);
    
    //electron selection
    Handle<View<Candidate> > hEle;
    event.getByLabel(objConfig_["Electrons"].getParameter<edm::InputTag>("source"), hEle);
    std::vector<CandidatePtr> selLooseElectrons = getGoodElectrons(hEle, hMu, *beamSpot, *rho, objConfig_["LooseElectrons"]);
    std::vector<CandidatePtr> selElectrons      = getGoodElectrons(hEle, hMu, *beamSpot, *rho, objConfig_["Electrons"]);

    //inclusive collection of leptons to store
    std::vector<CandidatePtr> selLooseLeptons = selLooseMuons;
    selLooseLeptons.insert(selLooseLeptons.end(),selLooseElectrons.begin(),selLooseElectrons.end());
    std::vector<CandidatePtr> selLeptons = selMuons;
    selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());

    //dilepton candidate
    std::vector<CandidatePtr> dilepton = getDileptonCandidate(selLeptons, objConfig_["Dileptons"], iSetup);
    ev.cat = getDileptonId(dilepton);
    if(dilepton.size()==2)
      {
	const reco::GenParticle *genLepton = getLeptonGenMatch(dilepton[0]);
	std::vector<double> leptoniso      = getLeptonIso(dilepton[0]);
	ev.l1_px    = dilepton[0]->px(); 
	ev.l1_py    = dilepton[0]->py(); 
	ev.l1_pz    = dilepton[0]->pz(); 
	ev.l1_en    = dilepton[0]->energy();  
	ev.l1_id    = getLeptonId(dilepton[0]);
	ev.l1_ptErr = getLeptonPtError(dilepton[0]); 
	ev.l2_genid = (genLepton==0? 0 : genLepton->pdgId());
	ev.l1_iso1  = leptoniso[ECAL_ISO]; 
	ev.l1_iso2  = leptoniso[HCAL_ISO]; 
	ev.l1_iso3  = leptoniso[TRACKER_ISO];
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
	ev.l2_iso1   = leptoniso[ECAL_ISO]; 
	ev.l2_iso2   = leptoniso[HCAL_ISO]; 
	ev.l2_iso3   = leptoniso[TRACKER_ISO];
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
	ev.ln_iso1[ev.ln]  = leptoniso[ECAL_ISO];
	ev.ln_iso2[ev.ln]  = leptoniso[HCAL_ISO];
	ev.ln_iso3[ev.ln]  = leptoniso[TRACKER_ISO];
	ev.ln_pid[ev.ln]   = getLeptonPidSummary(*lit);
	ev.ln++;
      }

    
    //
    // PHOTON SELECTION
    //
    edm::Handle<edm::View<reco::Candidate> > hPhoton;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("source"), hPhoton );
    edm::Handle<std::vector<reco::Track> > hTracks;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("trackSource"), hTracks );
    edm::Handle<EcalRecHitCollection> ebrechits;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"), ebrechits );
    EcalClusterLazyTools lazyTool(event,iSetup,
                                  objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"),
                                  objConfig_["Photons"].getParameter<edm::InputTag>("eerechits"));
    edm::Handle<std::vector<reco::Conversion> > hConversions;
    event.getByLabel(objConfig_["Photons"].getParameter<edm::InputTag>("conversionSource"), hConversions);
    edm::Handle< std::vector<int> >  pvAssocCandidates;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("pvAssocCandidatesSource"),pvAssocCandidates);
    edm::Handle<reco::PFCandidateCollection> pfCandsH;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("pfCands"), pfCandsH);
    if( !ecorr_.IsInitialized() ) ecorr_.Initialize( iSetup, objConfig_["Photons"].getParameter<std::string>("phoCorrection") );

    ev.gn=0;
    std::vector<CandidatePtr> selPhotons=getGoodPhotons(hPhoton,lazyTool,ebrechits,hTracks,hEle,hConversions,*rho,objConfig_["Photons"]);
    for(std::vector<CandidatePtr>::iterator phoIt = selPhotons.begin(); phoIt != selPhotons.end(); phoIt++)
      {
	const reco::Photon *pho= dynamic_cast<const reco::Photon *>( phoIt->get() );
	std::pair<double,double> cor = ecorr_.CorrectedEnergyWithError( *pho );
 	ev.g_px[ev.gn]    = pho->px();
	ev.g_py[ev.gn]    = pho->py();
	ev.g_pz[ev.gn]    = pho->pz();
	ev.g_en[ev.gn]    = pho->energy();
	ev.g_r9[ev.gn]    = pho->r9();   
	ev.g_hoe[ev.gn]   = pho->hadronicOverEm();  
	ev.g_sihih[ev.gn] = pho->sigmaIetaIeta();
	ev.g_iso1[ev.gn]  = pho->trkSumPtSolidConeDR04();
	ev.g_iso2[ev.gn]  = pho->ecalRecHitSumEtConeDR04();
	ev.g_iso3[ev.gn]  = pho->hcalTowerSumEtConeDR04();
	ev.g_ecorr[ev.gn] = cor.first/pho->energy();

	//check if it has been matched to the PV bt the clustered MET producer
	double minDR=9999.;
	const reco::PFCandidate *match=0;
	if(pvAssocCandidates.isValid()  && pfCandsH.isValid())
	  {
	    for(std::vector<int>::const_iterator pfIt = pvAssocCandidates->begin();  pfIt != pvAssocCandidates->end(); pfIt++)
	      {
		reco::PFCandidateRef candRef(pfCandsH,*pfIt);
		if(candRef->charge()!=0) continue;
		double dr=deltaR(candRef->eta(),candRef->phi(),pho->eta(),pho->phi());
		if(dr>minDR) continue;
		minDR=dr;
		match=candRef.get();
	      }
	  }
	bool matchFound(minDR<0.05);
	ev.g_conv[ev.gn] = matchFound;
	ev.g_conv_px[ev.gn] = matchFound? match->px() :0;
	ev.g_conv_py[ev.gn] = matchFound? match->py():0;
	ev.g_conv_pz[ev.gn] = matchFound? match->pz():0;
	ev.g_conv_en[ev.gn] = matchFound? match->energy():0;
	ev.gn++;
      }

    if(ev.cat==UNKNOWN && selPhotons.size()) ev.cat=GAMMA+1000*photonTrig.second;


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
    for (std::vector<CandidatePtr>::iterator jIt = selJets.begin(); jIt != selJets.end(); jIt++)
      {
	const pat::Jet *jet = dynamic_cast<const pat::Jet *>(jIt->get());
	ev.jn_px[ev.jn] = jet->px();  
	ev.jn_py[ev.jn] = jet->py(); 
	ev.jn_pz[ev.jn] = jet->pz(); 
	ev.jn_en[ev.jn] = jet->energy();
	const reco::Candidate *genParton = jet->genParton();
	ev.jn_genid[ev.jn]   = genParton ? genParton->pdgId() : -9999;
	ev.jn_genflav[ev.jn] = jet->partonFlavour();
	ev.jn_btag1[ev.jn]   = jet->bDiscriminator("trackCountingHighEffBJetTags");
	ev.jn_btag2[ev.jn]   = jet->bDiscriminator("jetBProbabilityBJetTags");
	ev.jn_btag3[ev.jn]   = jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	ev.jn_btag4[ev.jn]   = jet->bDiscriminator("trackCountingHighPurBJetTags");
	ev.jn_vtxAssoc[ev.jn]=true;
        ev.jn++;

	nbcands += (jet->pt()>30 && fabs(jet->eta())<2.4 && jet->bDiscriminator("trackCountingHighEffBJetTags")>2); 
      }
    

    //
    // MET SELECTION
    //
    //pf-met
    Handle<View<Candidate> > hMET;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("source"), hMET);
    CandidatePtr pfmet = hMET->ptrAt(0);
    ev.met1_phi = pfmet->phi();    ev.met1_pt=  pfmet->pt();
 
    //pseudo-mets
    LorentzVector trkmet(0,0,0,0);
    std::vector<double> sumEts; 
    std::vector<LorentzVector>  clusteredMets;
    try{
      
      edm::Handle< std::vector<double> > sumEtsH;
      event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("sumEtSources"),sumEtsH);
      if(sumEtsH.isValid()) sumEts = *sumEtsH;

      std::vector<edm::InputTag> clusteredMetSources = objConfig_["MET"].getParameter<std::vector<edm::InputTag> >("hzzmetSources");
      for(std::vector<edm::InputTag>::iterator it = clusteredMetSources.begin();
	  it != clusteredMetSources.end();
	  it++)
	{
	  edm::Handle< reco::PFMET > clustMetH;
	  event.getByLabel(*it,clustMetH); 
	  clusteredMets.push_back( LorentzVector(clustMetH->px(),clustMetH->py(),0,clustMetH->pt()) );
	}
      
      //trk met (from WW , do we need it?)
      edm::Handle< edm::ValueMap<reco::PFMET> > trkMetsH;
      event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("trksource"), trkMetsH); 
      const reco::PFMET &trkpfmetObj=(*trkMetsH)[primVertex];
      trkmet=LorentzVector(trkpfmetObj.px(),trkpfmetObj.py(),0,trkpfmetObj.pt());

    }catch(std::exception &e){
      //cout << e.what() << endl;
    }

    ev.met2_phi = trkmet.phi();    ev.met2_pt=trkmet.pt();
    ev.met3_phi = 0;               ev.met3_pt=0;
    if(clusteredMets.size()>0)
      {
	ev.met4_phi  = clusteredMets[0].phi();   ev.met4_pt  = clusteredMets[0].pt();
	ev.met5_phi  = clusteredMets[1].phi();   ev.met5_pt  = clusteredMets[1].pt();
	ev.met6_phi  = clusteredMets[2].phi();   ev.met6_pt  = clusteredMets[2].pt();
	ev.met7_phi  = clusteredMets[3].phi();   ev.met7_pt  = clusteredMets[3].pt();
	ev.met8_phi  = clusteredMets[4].phi();   ev.met8_pt  = clusteredMets[4].pt();
	ev.met9_phi  = clusteredMets[5].phi();   ev.met9_pt  = clusteredMets[5].pt();
	ev.met10_phi = clusteredMets[6].phi();   ev.met10_pt = clusteredMets[6].pt();
	ev.met11_phi = clusteredMets[7].phi();   ev.met11_pt = clusteredMets[7].pt();
	ev.met12_phi = clusteredMets[8].phi();   ev.met12_pt = clusteredMets[8].pt();
	ev.met13_phi = clusteredMets[9].phi();   ev.met13_pt = clusteredMets[9].pt();
        ev.met14_phi = clusteredMets[10].phi();  ev.met14_pt = clusteredMets[10].pt();
        ev.met15_phi = clusteredMets[11].phi();  ev.met15_pt = clusteredMets[11].pt();
      }
    if(sumEts.size()>0)
      {
	ev.sumEt = sumEts[0];              ev.sumEtcentral = sumEts[3];
	ev.chsumEt = sumEts[1];            ev.chsumEtcentral = sumEts[4];
	ev.neutsumEt = sumEts[2];          ev.neutsumEtcentral = sumEts[5];
	ev.primVertexSumEt = sumEts[6];    ev.primVertexChSumEt = sumEts[7];    ev.primVertexNeutSumEt = sumEts[8];
	ev.otherVertexSumEt = sumEts[9];   ev.otherVertexChSumEt = sumEts[10];  ev.otherVertexNeutSumEt = sumEts[11];
      }
        
    //set flags for coarse selection
    if(dilepton.size()==2)
      {
	ev.pass=ev.cat;
	if(ev.ln==0)      ev.pass += 1000;
	if(nbcands==0)    ev.pass += 2000;  
	if(ev.met1_pt>60) ev.pass += 3000;
	if(ev.met1_pt>10) ev.pass += 4000;
	LorentzVector dilP4=dilepton[0]->p4()+dilepton[1]->p4();
	if( fabs(dilP4.mass()-91)<15 ) ev.pass +=5000;
      }
       
    // finish event summary
    summaryHandler_.fillTree();
    
  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
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
  }

  summary= ( (int(ele->electronID("eidVBTF70")) & 0x1) )
    | ( (int(ele->electronID("eidVBTF80")) & 0x1) << 1)
    | ( (int(ele->electronID("eidVBTF85")) & 0x1) << 2)
    | ( (int(ele->electronID("eidVBTF90")) & 0x1) << 3)
    | ( (int(ele->electronID("eidVBTF95")) & 0x1) << 4)
    | ( (int(ele->electronID("eidVeryLoose")) & 0x1) << 5)
    | ( (int(ele->electronID("eidLoose")) & 0x1) << 6)
    | ( (int(ele->electronID("eidMedium")) & 0x1) << 7)
    | ( (int(ele->electronID("eidTight")) & 0x1) << 8)
    | ( (int(ele->electronID("eidSuperTight")) & 0x1) << 9)
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
    | ( (int(mu->muonID("TMLastStationAngTight")) & 0x1) << 3);
  return summary;
}

DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 

//  LocalWords:  beamSpot
