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
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ProjectedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:

  float addMCtruth(const pat::EventHypothesis &evhyp, const edm::Event &event, const edm::EventSetup &iSetup );

  int getMuonPidSummary(const pat::Muon &mu);
  int getElectronPidSummary(const pat::Electron &ele);

  std::map<std::string, edm::ParameterSet> objConfig_;
  ReducedMETComputer rmet_;
  ProjectedMETComputer pmet_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
  EventCategory eventClassifComp_;
};

using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
  : rmet_(1.0,1.0,0.0,0.0,1.0),
    controlHistos_( iConfig.getParameter<std::string>("dtag") )
{
  try{
    
    edm::Service<TFileService> fs;
    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    

    //
    objConfig_["MET"] = iConfig.getParameter<edm::ParameterSet>("MET");
    objConfig_["Jets"] = iConfig.getParameter<edm::ParameterSet>("Jets");
    objConfig_["Trigger"] = iConfig.getParameter<edm::ParameterSet>("Trigger");

    //generator level
    objConfig_["Generator"] = iConfig.getParameter<edm::ParameterSet>("Generator");

    //generated pileup (use directly loose selection)
    objConfig_["Vertices"] = iConfig.getParameter<edm::ParameterSet>("Vertices");
    controlHistos_.addHistogram("ngenpileup", ";Pileup; Events", 25, 0.,25.);
    controlHistos_.addHistogram("ngenpileupOOT", ";Out-of-time Pileup; Events", 25, 0.,25.);

    //vertices
    controlHistos_.addHistogram("ngoodvertex", ";Vertices; Events", 25, 0.,25.);

    //selection streams
    TString selFilters[]={"Reco","no scrap","#geq 1 vertex","HB/HE noise","No beam halo"};
    const size_t nselFilters=sizeof(selFilters)/sizeof(TString);
    controlHistos_.addHistogram("preselcutflow", ";Steps; Events", nselFilters, 0.,nselFilters);
    TH1 *h=controlHistos_.getHisto("preselcutflow");
    for(size_t istep=0; istep<nselFilters; istep++) h->GetXaxis()->SetBinLabel(istep+1,selFilters[istep]);

    TString selsteps[]={"Reco","#geq 2 very loose leptons","#geq 2 tight leptons","no extra loose leptons","b-veto","|M-M_{Z}|<15","red-E_{T}^{miss}>medium"};
    const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
    controlHistos_.addHistogram("cutflow", ";Steps; Events", nselsteps, 0.,nselsteps);
    h=controlHistos_.getHisto("cutflow");
    for(size_t istep=0; istep<nselsteps; istep++) h->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);

    controlHistos_.addHistogram("vertex_mindz", ";min #Delta z [cm]; Events", 100, -25.,25.);
    controlHistos_.addHistogram("vertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.);
    controlHistos_.addHistogram("othervertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.);
    controlHistos_.addHistogram("vertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.);
    controlHistos_.addHistogram("othervertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.);
    controlHistos_.addHistogram("ngoodvertex", ";Vertices; Events", 25, 0.,25.);

    //lepton control
    controlHistos_.addHistogram("nleptons",";Lepton multiplicity; Events",4,2,6);
    h=controlHistos_.getHisto("nleptons");
    for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
      {
	TString ilabel(""); ilabel+=(ibin+1);
	if(ibin==h->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	h->GetXaxis()->SetBinLabel(ibin,ilabel);
      }

    //dilepton control plots
    TString dilTypes[2]={"ss","os"};
    for(size_t idilType=0; idilType<2; idilType++)
      {
	controlHistos_.addHistogram(dilTypes[idilType]+"dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 50, 0.,300.);
	controlHistos_.addHistogram(dilTypes[idilType]+"dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.);
	controlHistos_.addHistogram(dilTypes[idilType]+"dilepton_dphi", ";|#Delta #phi| [rad]; Events", 50, 0.,3.2);
	controlHistos_.addHistogram(dilTypes[idilType]+"dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.);
      }
    
    controlHistos_.addHistogram("dilepton_atres", ";a_{#perp} difference [GeV]; Events", 100, -50.,50.);
    controlHistos_.addHistogram("dilepton_alres", ";a_{#parallel} difference [GeV]; Events", 100, -50.,50.);

    //jets
    controlHistos_.addHistogram("jetfassoc",";#beta_{vtx}(Jet); Jets",50,0,1.0);
    controlHistos_.addHistogram("jetpt",";p_{T} [GeV/c]; Jets",50,0,200);
    controlHistos_.addHistogram("jeteta",";#eta; Jets",50,-2.5,2.5);
    controlHistos_.addHistogram("jetbtags",";b tags (TCHE); Jets",50,-5,45);
    controlHistos_.addHistogram("jetmult",";Jet multiplicity; Events",4,0,4);
    controlHistos_.addHistogram("jetbmult",";b tag multiplicity (TCHEL); Events",4,0,4);
    h=controlHistos_.getHisto("jetbmult");
    for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
      {
	TString ilabel(""); ilabel+=(ibin-1);
	if(ibin==h->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	h->GetXaxis()->SetBinLabel(ibin,ilabel);
	controlHistos_.getHisto("jetmult")->GetXaxis()->SetBinLabel(ibin,ilabel);
      }
 
    
    //MET
    controlHistos_.addHistogram("met", ";{E}_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("chmet", ";charged-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("trkmet", ";track-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("redmet", ";red-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("projmet", ";proj-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("projchmet", ";proj-charged-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("puffomet", ";min-proj-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);


    controlHistos_.initMonitorForStep("ee");
    controlHistos_.initMonitorForStep("mumu");
    controlHistos_.initMonitorForStep("emu");
    controlHistos_.initMonitorForStep("gamma");
  }
  catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
float DileptonPlusMETEventAnalyzer::addMCtruth( const pat::EventHypothesis &evhyp, const edm::Event &event, const edm::EventSetup &iSetup)
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
  int npuOOT(0),npuIT(0);
  if(puInfoH.isValid())
    {
      for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	{
	  if(it->getBunchCrossing()==0) npuIT += it->getPU_NumInteractions();
	  else                          npuOOT += it->getPU_NumInteractions();
	}
      controlHistos_.fillHisto("ngenpileup","all",npuIT,weight);
      controlHistos_.fillHisto("ngenpileupOOT","all",npuOOT,weight);
    }
  ev.ngenITpu=npuIT;
  ev.ngenOOTpu=npuOOT;

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

      controlHistos_.fillHisto("q2","all",ev.qscale,weight);
      controlHistos_.fillHisto("pthat","all",ev.pthat,weight);
      controlHistos_.fill2DHisto("x1vsx2","all",ev.x1,ev.x2,weight);
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
  ev.nmcparticles=0;
  LorentzVector genHiggs(0,0,0,0);
  LorentzVector genMET(0,0,0,0),genZll(0,0,0,0);
  int igenpart(0);
  bool higgsFound(false);
  for (pat::eventhypothesis::Looper<reco::GenParticle> genpart = evhyp.loopAs<reco::GenParticle>("genparticle"); genpart; ++genpart) 
    {
      //higgs level (H)
      if(fabs(genpart->pdgId())==25) higgsFound=true;
      ev.h_px=genpart->px();
      ev.h_py=genpart->py();
      ev.h_pz=genpart->pz();
      ev.h_en=genpart->energy();
	    
      int igenpartdau(0);
      char buf[20];
      sprintf(buf,"gendaughter_%d",igenpart);
      for(pat::eventhypothesis::Looper<reco::GenParticle> genpartdau = evhyp.loopAs<reco::GenParticle>(buf); genpartdau; ++genpartdau)
	{
	  //higgs daughters level (ZZ/WW)
	  bool isZ(fabs(genpartdau->pdgId())==23);
	  LorentzVector zcand=genpartdau->p4();
	  ev.mc_px[ev.nmcparticles]=genpartdau->px();  
	  ev.mc_py[ev.nmcparticles]=genpartdau->py();  
	  ev.mc_pz[ev.nmcparticles]=genpartdau->pz(); 
	  ev.mc_en[ev.nmcparticles]=genpartdau->energy();  
	  ev.mc_id[ev.nmcparticles]=genpartdau->pdgId();
	  ev.nmcparticles++;

	  char buf[20];
	  sprintf(buf,"gendaughter_%d_%d",igenpart,igenpartdau);
	  int nleptons(0);
	  for(pat::eventhypothesis::Looper<reco::GenParticle> genpartgdau = evhyp.loopAs<reco::GenParticle>(buf); genpartgdau; ++genpartgdau)
	    {
	      //final state products (leptons, jets, neutrinos)
	      int pdgid=fabs(genpartgdau->pdgId());
	      if(pdgid==12||pdgid==14||pdgid==16) genMET += genpartgdau->p4();
	      if(pdgid==11||pdgid==13||pdgid==15) nleptons++;
	      ev.mc_px[ev.nmcparticles]=genpartgdau->px();  
	      ev.mc_py[ev.nmcparticles]=genpartgdau->py();  
	      ev.mc_pz[ev.nmcparticles]=genpartgdau->pz(); 
	      ev.mc_en[ev.nmcparticles]=genpartgdau->energy();  
	      ev.mc_id[ev.nmcparticles]=genpartgdau->pdgId();
	      ev.nmcparticles++;
	    }
	  
	  if(isZ and nleptons==2) genZll=zcand;
	  
	  igenpartdau++;
	}
      igenpart++;
    }

  //mc truth channel
  if(higgsFound) ev.mccat=gen::HIGGS;
  else
    {
      edm::Handle<edm::View<reco::Candidate> > hGen;
      event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("source"), hGen);
      ev.mccat = gen::assignDileptonChannel(hGen);
    }
  
  //gen MET (longitudinal momentum is included - do not use directly the energy afterwards)
  ev.mc_px[ev.nmcparticles]=genMET.px();
  ev.mc_py[ev.nmcparticles]=genMET.py();
  ev.mc_pz[ev.nmcparticles]=genMET.pz();
  ev.mc_en[ev.nmcparticles]=genMET.energy();
  ev.mc_id[ev.nmcparticles]=0;
  ev.nmcparticles++;

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

 
  //returns the event weight
  return weight;
}


//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) 
{

  try{

    //get triggers
    bool hasTrigger(false);
    edm::Handle<edm::TriggerResults> allTriggerBits_;
    edm::InputTag trigSource("TriggerResults::HLT");
    event.getByLabel( trigSource, allTriggerBits_);
    const edm::TriggerNames &triggerNames = event.triggerNames( *allTriggerBits_);
    std::vector<std::string> triggerPaths = objConfig_["Trigger"].getParameter<std::vector<std::string> >("triggerPaths"); 
    for (size_t itrig = 0; itrig != allTriggerBits_->size(); ++itrig)
      {
	std::string trigName = triggerNames.triggerName(itrig);
	if( !allTriggerBits_->wasrun(itrig) ) continue;
	if( allTriggerBits_->error(itrig) ) continue;
	if( !allTriggerBits_->accept(itrig) ) continue;
	if( find(triggerPaths.begin(), triggerPaths.end(), trigName) == triggerPaths.end() ) continue;
	hasTrigger=true;
	break;
      }
	
    
    //get objects for this event
    edm::Handle<std::vector<pat::EventHypothesis> > evHandle;
    event.getByLabel(edm::InputTag("cleanEvent:selectedEvent"),evHandle);
    if(!evHandle.isValid()) 
      {
	cout << "No valid event hypothesis" << endl;
	return;
      }

    //event summary to be filled
    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
    ev.run=event.id().run();
    ev.lumi=event.luminosityBlock();
    ev.event=event.id().event();

    //event hypothesis and weight
    const pat::EventHypothesis &evhyp = (*(evHandle.product()))[0];
    float weight = addMCtruth(evhyp, event, iSetup );    

    //selected path and step
    edm::Handle< std::vector<int> > selInfo;
    event.getByLabel(edm::InputTag("cleanEvent:selectionInfo"),selInfo);
    if(!selInfo.isValid()) 
      {
	cout << "No valid selection info" << endl;
	return;
      }
    int selPath = (*(selInfo.product()))[0];
    int selStep = (*(selInfo.product()))[1];
    ev.cat=selPath;
    
    //require that a dilepton has been selected
    if(selPath==0 or selStep<3) return;
    std::string istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";
    bool isGammaEvent(false);
    if(selPath>=22) { isGammaEvent=true; istream="gamma"; }
    controlHistos_.fillHisto("cutflow","all",2,weight);
    controlHistos_.fillHisto("cutflow",istream,2,weight);

    // 
    // VERTEX KINEMATICS (get primary vertex selected)       
    // 
    edm::Handle<std::vector<reco::Vertex> > vertexHandle;
    event.getByLabel(edm::InputTag("cleanEvent:selectedVertices"),vertexHandle);
    if(!vertexHandle.isValid()) 
      {
	cout << "No vertex selected" << endl;
	return;
      }

    //other vertices
    std::vector<reco::VertexRef> selVertices;
    try{
      edm::Handle<reco::VertexCollection> hVtx;
      event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
      selVertices = vertex::filter(hVtx,objConfig_["Vertices"]);
    }catch(std::exception &e){
    }
    ev.nvtx=selVertices.size();

    reco::VertexRef primVertex(vertexHandle,0);
    controlHistos_.fillHisto("ngoodvertex",istream,selVertices.size(),weight);
    controlHistos_.fillHisto("vertex_sumpt",istream,vertex::getVertexMomentumFlux(primVertex.get()),weight);
    controlHistos_.fillHisto("vertex_pt",istream,primVertex->p4().pt(),weight);
    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++)
      {
	if(vit->get()->position()==primVertex->position()) continue;
	controlHistos_.fillHisto("othervertex_sumpt",istream,vertex::getVertexMomentumFlux(vit->get()),weight);
	controlHistos_.fillHisto("othervertex_pt",istream,vit->get()->p4().pt(),weight);
      }
    ev.vtx_px = primVertex->p4().px(); ev.vtx_py = primVertex->p4().py(); ev.vtx_pz = primVertex->p4().pz();  ev.vtx_en = primVertex->p4().energy();
   
        
    //
    // LEPTON KINEMATICS
    //
    //basic dilepton kinematics
    bool isZcandidate(true);
    LorentzVector lepton1P(0,0,0,0),lepton2P(0,0,0,0);
    if(!isGammaEvent)
      {
	reco::CandidatePtr lepton1 = evhyp["leg1"];
	lepton1P=lepton1->p4();
	int l1id=lepton::getLeptonId(lepton1);
	double lepton1pterr=lepton::getPtErrorFor(lepton1);
	std::vector<double> lepton1iso=lepton::getLeptonIso(lepton1);
	const reco::GenParticle *genLepton=lepton::getLeptonGenMatch(lepton1);
	int genid1 = (genLepton==0? 0 : genLepton->pdgId());
	ev.l1_px=lepton1P.px(); ev.l1_py=lepton1P.py(); ev.l1_pz=lepton1P.pz(); ev.l1_en=lepton1P.energy();  ev.l1_id=l1id; ev.l1_genid = genid1;  ev.l1_ptErr=lepton1pterr;  ev.l1_iso1=lepton1iso[0]; ev.l1_iso2=lepton1iso[1]; ev.l1_iso3=lepton1iso[2];
	ev.l1_pid= (fabs(l1id)==lepton::MUON ? 
		    getMuonPidSummary( dynamic_cast<const pat::Muon &>(*(lepton1.get())) ) :
		    getElectronPidSummary( dynamic_cast<const pat::Electron &>(*(lepton1.get())) ) );
	
	reco::CandidatePtr lepton2 = evhyp["leg2"];
	lepton2P=lepton2->p4();
	int l2id=lepton::getLeptonId(lepton2);
	double lepton2pterr=lepton::getPtErrorFor(lepton2);
	std::vector<double> lepton2iso=lepton::getLeptonIso(lepton2);
	genLepton=lepton::getLeptonGenMatch(lepton2);
	int genid2 = (genLepton==0? 0 : genLepton->pdgId());
	ev.l2_px=lepton2P.px(); ev.l2_py=lepton2P.py(); ev.l2_pz=lepton2P.pz(); ev.l2_en=lepton2P.energy();  ev.l2_id=l2id; ev.l2_genid = genid2;  ev.l2_ptErr=lepton2pterr;  ev.l2_iso1=lepton2iso[0]; ev.l2_iso2=lepton2iso[1]; ev.l2_iso3=lepton2iso[2];
	ev.l2_pid= (fabs(l2id)==lepton::MUON ? 
		    getMuonPidSummary( dynamic_cast<const pat::Muon &>(*(lepton2.get())) ) :
		    getElectronPidSummary( dynamic_cast<const pat::Electron &>(*(lepton2.get())) ) );
	
	bool isOS(lepton1->charge()*lepton2->charge()<0);
	TString dilCat( isOS ? "osdilepton" : "ssdilepton" );
	
	//select Z window
	LorentzVector dileptonP=lepton1P+lepton2P;
	controlHistos_.fillHisto(dilCat+"_mass",istream,dileptonP.mass(),weight);
	isZcandidate=(fabs(dileptonP.mass()-91)<15);
	//if(dileptonP.mass()<40) return;
	//if(fabs(l1id)==fabs(l2id) && fabs(dileptonP.mass()-91)>15) return;
	//if(!isZcandidate) return;
	
	double dphill=deltaPhi(lepton1P.phi(),lepton2P.phi());
	controlHistos_.fillHisto(dilCat+"_sumpt",istream,lepton1P.pt()+lepton2P.pt(),weight);
	controlHistos_.fillHisto(dilCat+"_pt",istream,dileptonP.pt(),weight);
	controlHistos_.fillHisto(dilCat+"_dphi",istream, fabs(dphill) ,weight );
      }

    //photons
    ev.gn=0;
    for (pat::eventhypothesis::Looper<pat::Photon> pho = evhyp.loopAs<pat::Photon>("photon"); pho; ++pho)
      {
	ev.g_px[ev.gn]=pho->px();
	ev.g_py[ev.gn]=pho->py();
	ev.g_pz[ev.gn]=pho->pz();
	ev.g_en[ev.gn]=pho->energy();
	ev.g_r9[ev.gn] = pho->r9();   
	ev.g_hoe[ev.gn]=pho->hadronicOverEm();  
	ev.g_sihih[ev.gn]=pho->sigmaIetaIeta();
	ev.g_iso1[ev.gn] = pho->trkSumPtSolidConeDR04();
	ev.g_iso2[ev.gn] = pho->ecalRecHitSumEtConeDR04();
	ev.g_iso3[ev.gn] = pho->hcalTowerSumEtConeDR04();
	ev.gn++;
      }

    //other isolated leptons
    ev.ln=0;
    for (pat::eventhypothesis::Looper<pat::Electron> ele = evhyp.loopAs<pat::Electron>("electron"); ele; ++ele) 
      {
	ev.ln_px[ev.ln]=ele->px();
	ev.ln_py[ev.ln]=ele->py();
	ev.ln_pz[ev.ln]=ele->pz();
	ev.ln_en[ev.ln]=ele->energy();
	ev.ln_pid[ev.ln]=getElectronPidSummary(*(ele.get()));
	ev.ln_id[ev.ln]=lepton::ELECTRON*ele->charge();
	ev.ln_genid[ev.ln] = (ele->genLepton()==0 ? 0 : ele->genLepton()->pdgId());
	ev.ln_ptErr[ev.ln]=(ele->pt()/ele->p()) * ele->trackMomentumError();
	ev.ln_iso1[ev.ln]=ele->neutralHadronIso();
	ev.ln_iso2[ev.ln]=ele->chargedHadronIso();
	ev.ln_iso3[ev.ln]=ele->photonIso();
	ev.ln++;
      }
    for (pat::eventhypothesis::Looper<pat::Muon> mu = evhyp.loopAs<pat::Muon>("muon"); mu; ++mu) 
      {
	ev.ln_px[ev.ln]=mu->px();
        ev.ln_py[ev.ln]=mu->py();
        ev.ln_pz[ev.ln]=mu->pz();
        ev.ln_en[ev.ln]=mu->energy();
        ev.ln_pid[ev.ln]=getMuonPidSummary(*(mu.get()));
	ev.ln_id[ev.ln]=lepton::MUON*mu->charge();
        ev.ln_genid[ev.ln] = (mu->genLepton()==0 ? 0 : mu->genLepton()->pdgId());
        ev.ln_ptErr[ev.ln]=mu->innerTrack()->ptError();
        ev.ln_iso1[ev.ln]=mu->neutralHadronIso();
        ev.ln_iso2[ev.ln]=mu->chargedHadronIso();
        ev.ln_iso3[ev.ln]=mu->photonIso();
	ev.ln++;
      }
    controlHistos_.fillHisto("nleptons",istream,2+ev.ln,weight);

    bool pass3rdLepton=(ev.ln==0);
    if(pass3rdLepton)
      {
	controlHistos_.fillHisto("cutflow","all",3,weight);
	controlHistos_.fillHisto("cutflow",istream,3,weight);
      }

    //
    // JET KINEMATICS
    //

    //average energy density
    edm::Handle< double > rho;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"), rho );
    ev.rho=*rho;

    //count the jets in the event
    int njets(0),nbjets(0);
    std::vector<LorentzVector> jetmomenta;
    ev.jn=0;
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) 
      {
	njets++;
	jetmomenta.push_back(jet->p4());
	float jbp=jet->bDiscriminator("jetBProbabilityBJetTags");
	float ssvhe=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	if(jet->pt()>30 && fabs(jet->eta())<2.4) nbjets += (jbp>1.33 || ssvhe>1.74);
	
	controlHistos_.fillHisto("jetfassoc",istream,jet::fAssoc(jet.get(),primVertex.get()),weight);
	controlHistos_.fillHisto("jetbtags",istream,jet->bDiscriminator("jetBProbabilityBJetTags"),weight);
	controlHistos_.fillHisto("jetpt",istream,jet->pt(),weight);
	controlHistos_.fillHisto("jeteta",istream,jet->eta(),weight);

	//save for event summary
	ev.jn_px[ev.jn] = jet->px();  ev.jn_py[ev.jn]=jet->py();  ev.jn_pz[ev.jn]=jet->pz(); ev.jn_en[ev.jn]=jet->energy();
	const reco::Candidate *genParton = jet->genParton();
	ev.jn_genid[ev.jn] = genParton ? genParton->pdgId() : -9999;
	ev.jn_genflav[ev.jn] = jet->partonFlavour();
	ev.jn_btag1[ev.jn]=jet->bDiscriminator("trackCountingHighEffBJetTags");
	ev.jn_btag2[ev.jn]=jet->bDiscriminator("jetBProbabilityBJetTags");
	ev.jn_btag3[ev.jn]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	ev.jn_btag4[ev.jn]=jet->bDiscriminator("trackCountingHighPurBJetTags");
	ev.jn_vtxAssoc[ev.jn]=true;
        ev.jn++;
      }
    controlHistos_.fillHisto("jetmult",istream,njets,weight);
    controlHistos_.fillHisto("jetbmult",istream,nbjets,weight);	
    bool passBveto(nbjets==0);
    if(pass3rdLepton && passBveto)
      {
	controlHistos_.fillHisto("cutflow","all",4,weight);
	controlHistos_.fillHisto("cutflow",istream,4,weight);
	if(isZcandidate)
	  {
	    controlHistos_.fillHisto("cutflow","all",5,weight);
	    controlHistos_.fillHisto("cutflow",istream,5,weight);
	  }
      }
    
    //
    // MET kinematics
    //
    //the met
    const pat::MET *evmet = dynamic_cast<const pat::MET *>(evhyp["met"].get());
    LorentzVector met(evmet->px(),evmet->py(),0,evmet->pt());

    //other met possibilities mapped to vertices
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
      
      edm::Handle< edm::ValueMap<reco::PFMET> > trkMetsH;
      event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("trksource"), trkMetsH); 

      //get the original vertex collecion
      edm::Handle<reco::VertexCollection> hVtx;
      event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);  
      
      //get the met corresponding to the selected vertex
      for(size_t ivtx=0; ivtx<hVtx->size(); ivtx++)
	{
	  reco::VertexRef vtxRef(hVtx,ivtx);
	  if(vtxRef->position().z()!=primVertex->position().z()) continue;

	  const reco::PFMET &trkpfmetObj=(*trkMetsH)[vtxRef];
	  trkmet=LorentzVector(trkpfmetObj.px(),trkpfmetObj.py(),0,trkpfmetObj.pt());
	  break;
	}
    }catch(std::exception &e){
      //cout << e.what() << endl;
    }

    //reduced met
    rmet_.compute(lepton1P,0, lepton2P,0, jetmomenta, met,isGammaEvent);
    float reducedMET=rmet_.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
    
    //projected MET
    float projMet =  pmet_.compute(lepton1P,lepton2P,met);
    float projTrkmet =  pmet_.compute(lepton1P,lepton2P,trkmet);
    float puffoMet = min(fabs(projMet),fabs(projTrkmet));
    
    //met control histograms
    controlHistos_.fillHisto("met",istream,met.pt(),weight);
    controlHistos_.fillHisto("trkmet",istream,trkmet.pt(),weight);
    controlHistos_.fillHisto("trkmet",istream,trkmet.pt(),weight);
    controlHistos_.fillHisto("redmet",istream,reducedMET,weight);
    controlHistos_.fillHisto("projmet",istream,projMet,weight);
    controlHistos_.fillHisto("projtrkmet",istream,projTrkmet,weight);
    controlHistos_.fillHisto("puffomet",istream,puffoMet,weight);
    
    //save for posterior analysis
    ev.met1_phi = met.phi();       ev.met1_pt=  met.pt();
    ev.met2_phi = trkmet.phi();    ev.met2_pt=trkmet.pt();
    ev.met3_phi = 0;               ev.met3_pt=reducedMET;      
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
      }
    if(sumEts.size()>0)
      {
	ev.sumEt = sumEts[0];              ev.sumEtcentral = sumEts[3];
	ev.chsumEt = sumEts[1];            ev.chsumEtcentral = sumEts[4];
	ev.neutsumEt = sumEts[2];          ev.neutsumEtcentral = sumEts[5];
	ev.primVertexSumEt = sumEts[6];    ev.primVertexChSumEt = sumEts[7];    ev.primVertexNeutSumEt = sumEts[8];
	ev.otherVertexSumEt = sumEts[9];   ev.otherVertexChSumEt = sumEts[10];  ev.otherVertexNeutSumEt = sumEts[11];
      }

    //classify the event
    PhysicsEvent_t physEvent=getPhysicsEventFrom(ev);
    int eventCategory = eventClassifComp_.Get(physEvent);
    bool passMediumRedMet(reducedMET>rmet_.getCut(eventCategory,ReducedMETComputer::MEDIUMWP));
    if(pass3rdLepton && passBveto && isZcandidate && passMediumRedMet)
      {
	controlHistos_.fillHisto("cutflow","all",6,weight);
	controlHistos_.fillHisto("cutflow",istream,6,weight);
      }
    

    //fill the events selection flags
    ev.pass=eventCategory;    
    if(pass3rdLepton) 
      {
      ev.pass += 1000;
      if(passBveto)   
	{
	  ev.pass += 2000;
	  if(passMediumRedMet)
	    {
	      ev.pass += 3000;
	      if(isZcandidate) ev.pass += 4000;
	    }
	}
    }
    
    //trigger bit
    ev.hasTrigger=hasTrigger;

    // finish event summary
    summaryHandler_.fillTree();

  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
  }

}

//
void DileptonPlusMETEventAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  //  cout << "[DileptonPlusMETEventAnalyzer::endLuminosityBlock]" << endl;
  TString streams[]={"ee","mumu","emu","gamma"};
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      if(ctrHandle.isValid())
	{
	  controlHistos_.fillHisto("cutflow",streams[istream],0.,ctrHandle->value);
	  if(istream==0) controlHistos_.fillHisto("cutflow","all",0.,ctrHandle->value);
	}
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      std::string inpt= std::string(streams[istream])+"SelectionCounter";
      iLumi.getByLabel(inpt, streamCtrHandle);
      if(streamCtrHandle.isValid())
	{
	  controlHistos_.fillHisto("cutflow",streams[istream],1.,streamCtrHandle->value);
	  controlHistos_.fillHisto("cutflow","all",1.,streamCtrHandle->value);
	}
    }

  //  cout << "[DileptonPlusMETEventAnalyzer::endLuminosityBlock]" << endl;
  TString filterCtrs[]={"preSelectionCounter","noScrapCounter","goodVertexCounter","noHBHEnoiseCounter","nobeamHaloCounter"};
  const size_t nselFilters=sizeof(filterCtrs)/sizeof(TString);
  for(size_t istep=0; istep<nselFilters; istep++)
    {
      std::string fname(filterCtrs[istep].Data());
      try{
	edm::Handle<edm::MergeableCounter> ctrHandle;
	iLumi.getByLabel(fname, ctrHandle);
	if(!ctrHandle.isValid()) continue;
	controlHistos_.fillHisto("preselcutflow","all",istep,ctrHandle->value);
      }catch(std::exception){
      }
    }

 
}


//
int DileptonPlusMETEventAnalyzer::getElectronPidSummary(const pat::Electron &ele)
{
  int summary(0);
  bool isEcalDriven(true);
  try{
    isEcalDriven = ele.ecalDrivenSeed();
  }catch(std::exception &e){
  }

  summary= ( (int(ele.electronID("eidVBTF70")) & 0x1) )
    | ( (int(ele.electronID("eidVBTF80")) & 0x1) << 1)
    | ( (int(ele.electronID("eidVBTF85")) & 0x1) << 2)
    | ( (int(ele.electronID("eidVBTF90")) & 0x1) << 3)
    | ( (int(ele.electronID("eidVBTF95")) & 0x1) << 4)
    | ( (int(ele.electronID("eidVeryLoose")) & 0x1) << 5)
    | ( (int(ele.electronID("eidLoose")) & 0x1) << 6)
    | ( (int(ele.electronID("eidMedium")) & 0x1) << 7)
    | ( (int(ele.electronID("eidTight")) & 0x1) << 8)
    | ( (int(ele.electronID("eidSuperTight")) & 0x1) << 9)
    | ( isEcalDriven << 10);
  return summary;
}

//
int DileptonPlusMETEventAnalyzer::getMuonPidSummary(const pat::Muon &mu)
{
  int summary(0);
  summary=( (int(mu.muonID("GlobalMuonPromptTight")) & 0x1) )
    | ( (int(mu.muonID("TMLastStationLoose")) & 0x1) << 1)
    | ( (int(mu.muonID("TMLastStationTight")) & 0x1) << 2)
    | ( (int(mu.muonID("TMLastStationAngTight")) & 0x1) << 3);
  return summary;
}

DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 
