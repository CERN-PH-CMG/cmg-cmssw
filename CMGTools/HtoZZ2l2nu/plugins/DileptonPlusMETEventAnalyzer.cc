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
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:

  float addMCtruth(const pat::EventHypothesis &evhyp, const edm::Event &event, const edm::EventSetup &iSetup );

  std::map<std::string, edm::ParameterSet> objConfig_;
  ReducedMETComputer rmet_;
  ProjectedMETComputer pmet_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
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
    TString selsteps[]={"Reco","2 leptons","2 good leptons","|M-M_{Z}|<15"};
    const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
    controlHistos_.addHistogram("cutflow", ";Steps; Events", nselsteps, 0.,nselsteps);
    TH1 *h=controlHistos_.getHisto("cutflow");
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
    TString jetTypes[2] ={"jet","pujet"};
    for(size_t ijetType=0; ijetType<2; ijetType++)
      {
	controlHistos_.addHistogram(jetTypes[ijetType]+"fassoc",";#beta_{vtx}(Jet); Jets",50,0,1.0);
	controlHistos_.addHistogram(jetTypes[ijetType]+"pt",";p_{T} [GeV/c]; Jets",50,0,200);
	controlHistos_.addHistogram(jetTypes[ijetType]+"eta",";#eta; Jets",50,-2.5,2.5);
	controlHistos_.addHistogram(jetTypes[ijetType]+"btags",";b tags (TCHE); Jets",50,-5,45);
	controlHistos_.addHistogram(jetTypes[ijetType]+"mult",";Jet multiplicity; Events",4,0,4);
	controlHistos_.addHistogram(jetTypes[ijetType]+"bmult",";b tag multiplicity (TCHEL); Events",4,0,4);
	h=controlHistos_.getHisto(jetTypes[ijetType]+"bmult");
	for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
	    {
	      TString ilabel(""); ilabel+=(ibin-1);
	      if(ibin==h->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	      h->GetXaxis()->SetBinLabel(ibin,ilabel);
	      controlHistos_.getHisto(jetTypes[ijetType]+"mult")->GetXaxis()->SetBinLabel(ibin,ilabel);
	    }
      }
    
    //MET
    controlHistos_.addHistogram("met", ";{E}_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("chmet", ";charged-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("redmet", ";red-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("projmet", ";proj-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("projchmet", ";proj-charged-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("puffomet", ";min-proj-E_{T}^{miss} [GeV/c]; Events", 100,  0.,500.);


    controlHistos_.initMonitorForStep("ee");
    controlHistos_.initMonitorForStep("mumu");
    controlHistos_.initMonitorForStep("emu");
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
  for (pat::eventhypothesis::Looper<reco::GenParticle> genpart = evhyp.loopAs<reco::GenParticle>("genparticle"); genpart; ++genpart) 
    {
      //higgs level (H)
      if(fabs(genpart->pdgId())==25){ev.h_px= genpart->px(); ev.h_py= genpart->py(); ev.h_pz= genpart->pz(); ev.h_en= genpart->energy();}
      if(fabs(genpart->pdgId())==25) genHiggs=genpart->p4();
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
    reco::CandidatePtr lepton1 = evhyp["leg1"];
    LorentzVector lepton1P(lepton1->p4());
    int l1id=lepton::getLeptonId(lepton1);
    double lepton1pterr=lepton::getPtErrorFor(lepton1);
    std::vector<double> lepton1iso=lepton::getLeptonIso(lepton1);
    const reco::GenParticle *genLepton=lepton::getLeptonGenMatch(lepton1);
    int genid1 = (genLepton==0? 0 : genLepton->pdgId());
    ev.l1_px=lepton1P.px(); ev.l1_py=lepton1P.py(); ev.l1_pz=lepton1P.pz(); ev.l1_en=lepton1P.energy();  ev.l1_id=l1id; ev.l1_genid = genid1;  ev.l1_ptErr=lepton1pterr;  ev.l1_iso1=lepton1iso[0]; ev.l1_iso2=lepton1iso[1]; ev.l1_iso3=lepton1iso[2];

    reco::CandidatePtr lepton2 = evhyp["leg2"];
    LorentzVector lepton2P(lepton2->p4());
    int l2id=lepton::getLeptonId(lepton2);
    double lepton2pterr=lepton::getPtErrorFor(lepton2);
    std::vector<double> lepton2iso=lepton::getLeptonIso(lepton2);
    genLepton=lepton::getLeptonGenMatch(lepton2);
    int genid2 = (genLepton==0? 0 : genLepton->pdgId());
    ev.l2_px=lepton2P.px(); ev.l2_py=lepton2P.py(); ev.l2_pz=lepton2P.pz(); ev.l2_en=lepton2P.energy();  ev.l2_id=l2id; ev.l2_genid = genid2;  ev.l2_ptErr=lepton2pterr;  ev.l2_iso1=lepton2iso[0]; ev.l2_iso2=lepton2iso[1]; ev.l2_iso3=lepton2iso[2];

    bool isOS(lepton1->charge()*lepton2->charge()<0);
    TString dilCat( isOS ? "osdilepton" : "ssdilepton" );

    //select Z window
    LorentzVector dileptonP=lepton1P+lepton2P;
    controlHistos_.fillHisto(dilCat+"_mass",istream,dileptonP.mass(),weight);
    if(dileptonP.mass()<40) return;
    if(fabs(l1id)==fabs(l2id) && fabs(dileptonP.mass()-91)>15) return;
    controlHistos_.fillHisto("cutflow","all",3,weight);
    controlHistos_.fillHisto("cutflow",istream,3,weight);

    double dphill=deltaPhi(lepton1P.phi(),lepton2P.phi());
    controlHistos_.fillHisto(dilCat+"_sumpt",istream,lepton1P.pt()+lepton2P.pt(),weight);
    controlHistos_.fillHisto(dilCat+"_pt",istream,dileptonP.pt(),weight);
    controlHistos_.fillHisto(dilCat+"_dphi",istream, fabs(dphill) ,weight );

    //other isolated leptons
    ev.ln=0;
    for (pat::eventhypothesis::Looper<pat::Electron> ele = evhyp.loopAs<pat::Electron>("electron"); ele; ++ele) 
      {
	ev.ln_px[ev.ln]=ele->px();
	ev.ln_py[ev.ln]=ele->py();
	ev.ln_pz[ev.ln]=ele->pz();
	ev.ln_en[ev.ln]=ele->energy();
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
        ev.ln_id[ev.ln]=lepton::MUON*mu->charge();
        ev.ln_genid[ev.ln] = (mu->genLepton()==0 ? 0 : mu->genLepton()->pdgId());
        ev.ln_ptErr[ev.ln]=mu->innerTrack()->ptError();
        ev.ln_iso1[ev.ln]=mu->neutralHadronIso();
        ev.ln_iso2[ev.ln]=mu->chargedHadronIso();
        ev.ln_iso3[ev.ln]=mu->photonIso();
	ev.ln++;
      }
    controlHistos_.fillHisto("nleptons",istream,2+ev.ln,weight);
    
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
    std::vector<const pat::Jet *> assocJets;
    ev.jn=0;
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) 
      {
	njets++;
	jetmomenta.push_back(jet->p4());
	assocJets.push_back( jet.get() );
	float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
	nbjets += (btag>1.7);
	controlHistos_.fillHisto("jetfassoc",istream,jet::fAssoc(jet.get(),primVertex.get()),weight);
	controlHistos_.fillHisto("jetbtags",istream,btag,weight);
	controlHistos_.fillHisto("jetpt",istream,jet->pt(),weight);
	controlHistos_.fillHisto("jeteta",istream,jet->eta(),weight);

	//save for event summary
	ev.jn_px[ev.jn] = jet->px();  ev.jn_py[ev.jn]=jet->py();  ev.jn_pz[ev.jn]=jet->pz(); ev.jn_en[ev.jn]=jet->energy();
	const reco::Candidate *genParton = jet->genParton();
	ev.jn_genid[ev.jn] = genParton ? genParton->pdgId() : -9999;
	ev.jn_btag1[ev.jn]=btag;
	ev.jn_btag2[ev.jn]=jet->bDiscriminator("trackCountingHighPurBJetTags");
	ev.jn_btag3[ev.jn]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	ev.jn_btag4[ev.jn]=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
	ev.jn_vtxAssoc[ev.jn]=true;
        ev.jn++;
      }
    controlHistos_.fillHisto("jetmult",istream,njets,weight);
    controlHistos_.fillHisto("jetbmult",istream,nbjets,weight);	

    //count the pu jets
    std::vector<reco::CandidatePtr> pujets= evhyp.all("pujet");
    int npujets(0),npubjets(0);
    std::vector<LorentzVector> pujetmomenta;
    std::vector<const pat::Jet *> puJets;
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("pujet"); jet; ++jet) 
      {
	npujets++;
	pujetmomenta.push_back(jet->p4());
	puJets.push_back( jet.get() );
	float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
	npubjets += (btag>1.7);
	controlHistos_.fillHisto("pujetfassoc",istream,jet::fAssoc(jet.get(),primVertex.get()),weight);
	controlHistos_.fillHisto("pujetbtags",istream,btag,weight);
        controlHistos_.fillHisto("pujetpt",istream,jet->pt(),weight);
	controlHistos_.fillHisto("pujeteta",istream,jet->eta(),weight);

	ev.jn_px[ev.jn] = jet->px();  ev.jn_py[ev.jn]=jet->py();  ev.jn_pz[ev.jn]=jet->pz(); ev.jn_en[ev.jn]=jet->energy();
	const reco::Candidate *genParton = jet->genParton();
	ev.jn_genid[ev.jn] = genParton ? genParton->pdgId() : -9999;
	ev.jn_btag1[ev.jn]=btag;
	ev.jn_btag2[ev.jn]=jet->bDiscriminator("trackCountingHighPurBJetTags");
	ev.jn_btag3[ev.jn]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	ev.jn_btag4[ev.jn]=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
	ev.jn_vtxAssoc[ev.jn]=false;
	ev.jn++;
      }
    controlHistos_.fillHisto("pujetmult",istream,npujets,weight);
    controlHistos_.fillHisto("pujetbmult",istream,npubjets,weight);

    //
    // MET kinematics
    //
    //the met
    const pat::MET *evmet = dynamic_cast<const pat::MET *>(evhyp["met"].get());
    LorentzVector met(evmet->px(),evmet->py(),0,evmet->pt());
    ev.met1_phi = met.phi();   ev.met1_pt=  met.pt();

    //other met possibilities mapped to vertices
    LorentzVector chmet(0,0,0,0);
    std::vector<LorentzVector> hzzmets;
    try{
      edm::Handle< edm::ValueMap<reco::PFMET> > chargedMets;
      event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("chsource"), chargedMets); 

      std::vector< edm::Handle< edm::ValueMap<reco::PFMET> > > hzzMetsH;
      std::vector<edm::InputTag> hzzMetSources = objConfig_["MET"].getParameter< std::vector<edm::InputTag> >("hzzmetSources");
      for(std::vector<edm::InputTag>::iterator it = hzzMetSources.begin(); it != hzzMetSources.end(); it++)
	{
	  try{
	    edm::Handle< edm::ValueMap<reco::PFMET> > newMetH;
	    event.getByLabel(*it,newMetH);
	    hzzMetsH.push_back(newMetH);
	  }catch(std::exception &e){
	    //    cout << e.what() << endl;
	  }
	}

      //get the original vertex collecion
      edm::Handle<reco::VertexCollection> hVtx;
      event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);  
      
      //get the met corresponding to the selected vertex
      for(size_t ivtx=0; ivtx<hVtx->size(); ivtx++)
	{
	  reco::VertexRef vtxRef(hVtx,ivtx);
	  if(vtxRef->position().z()!=primVertex->position().z()) continue;

	  const reco::PFMET &chpfmet=(*chargedMets)[vtxRef];
	  chmet=LorentzVector(chpfmet.px(),chpfmet.py(),0,chpfmet.pt());
	  
	  for(std::vector< edm::Handle< edm::ValueMap<reco::PFMET> > >::iterator mIt = hzzMetsH.begin();
	      mIt != hzzMetsH.end(); mIt++)
	    {
	      if(mIt->isValid())
		{
		  const reco::PFMET &newmet=(*(*mIt))[vtxRef];
		  hzzmets.push_back( LorentzVector(newmet.px(),newmet.py(),0,newmet.pt()) );
		}
	    }
	  break;
	}
    }catch(std::exception &e){
      //cout << e.what() << endl;
    }


    ev.met2_phi = chmet.phi(); ev.met2_pt=chmet.pt();
    if(hzzmets.size()>0) { ev.met3_phi = hzzmets[0].phi(); ev.met3_pt=hzzmets[0].pt(); }
    if(hzzmets.size()>1) { ev.met4_phi = hzzmets[1].phi(); ev.met4_pt=hzzmets[1].pt(); }
    if(hzzmets.size()>2) { ev.met5_phi = hzzmets[2].phi(); ev.met5_pt=hzzmets[2].pt(); }
    if(hzzmets.size()>3) { ev.met6_phi = hzzmets[3].phi(); ev.met6_pt=hzzmets[3].pt(); }
    if(hzzmets.size()>4) { ev.met7_phi = hzzmets[4].phi(); ev.met7_pt=hzzmets[4].pt(); }

    //reduced met
    rmet_.compute(lepton1->p4(),lepton1pterr, lepton2->p4(),lepton2pterr, jetmomenta, met);
    float reducedMET=rmet_.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);

    //projected MET
    float projMet = pmet_.compute(lepton1->p4(),lepton2->p4(),met);
    float projChMet = pmet_.compute(lepton1->p4(),lepton2->p4(),met);
    float puffoMet = min(fabs(projMet),fabs(projChMet));

    //met control histograms
    controlHistos_.fillHisto("met",istream,met.pt(),weight);
    controlHistos_.fillHisto("chmet",istream,chmet.pt(),weight);
    controlHistos_.fillHisto("redmet",istream,reducedMET,weight);
    controlHistos_.fillHisto("projmet",istream,projMet,weight);
    controlHistos_.fillHisto("projchmet",istream,projChMet,weight);
    controlHistos_.fillHisto("puffomet",istream,puffoMet,weight);
    
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
  TString streams[]={"ee","mumu","emu"};
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      controlHistos_.fillHisto("cutflow",streams[istream],0.,ctrHandle->value);
      if(istream==0) controlHistos_.fillHisto("cutflow","all",0.,ctrHandle->value);
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      std::string inpt= std::string(streams[istream])+"SelectionCounter";
      iLumi.getByLabel(inpt, streamCtrHandle);
      controlHistos_.fillHisto("cutflow",streams[istream],1.,streamCtrHandle->value);
      controlHistos_.fillHisto("cutflow","all",1.,streamCtrHandle->value);
    } 
}


DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 
