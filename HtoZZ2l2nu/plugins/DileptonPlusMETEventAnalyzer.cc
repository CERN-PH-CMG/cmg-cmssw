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

  std::map<TString, TObject *>  results_;
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
    controlHistos_.addHistogram("met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("met2dilepton_dphi", ";#Delta #phi(#slash{E}_{T},vertex) [rad]; Events", 50, 0,3.2);
    controlHistos_.addHistogram("mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",50,0,500);
    controlHistos_.addHistogram("mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",50,0,1000);


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
  if(event.isRealData())  return weight;

  //pileup
  edm::Handle<float> puWeightHandle;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("puReweight"), puWeightHandle );
  if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
  ev.weight = weight;
 
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
      if(fabs(genpart->pdgId())==25) genHiggs=genpart->p4();
      ev.mcpx[ev.nmcparticles] = genpart->px();  
      ev.mcpy[ev.nmcparticles] = genpart->py();  
      ev.mcpz[ev.nmcparticles] = genpart->pz(); 
      ev.mcen[ev.nmcparticles]=genpart->energy();  
      ev.mcid[ev.nmcparticles]=genpart->pdgId();
      ev.nmcparticles++;
	    
      int igenpartdau(0);
      char buf[20];
      sprintf(buf,"gendaughter_%d",igenpart);
      for(pat::eventhypothesis::Looper<reco::GenParticle> genpartdau = evhyp.loopAs<reco::GenParticle>(buf); genpartdau; ++genpartdau)
	{
	  //higgs daughters level (ZZ/WW)
	  bool isZ(fabs(genpartdau->pdgId())==23);
	  LorentzVector zcand=genpartdau->p4();
	  ev.mcpx[ev.nmcparticles]=genpartdau->px();  
	  ev.mcpy[ev.nmcparticles]=genpartdau->py();  
	  ev.mcpz[ev.nmcparticles]=genpartdau->pz(); 
	  ev.mcen[ev.nmcparticles]=genpartdau->energy();  
	  ev.mcid[ev.nmcparticles]=genpartdau->pdgId();
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
	      ev.mcpx[ev.nmcparticles]=genpartgdau->px();  
	      ev.mcpy[ev.nmcparticles]=genpartgdau->py();  
	      ev.mcpz[ev.nmcparticles]=genpartgdau->pz(); 
	      ev.mcen[ev.nmcparticles]=genpartgdau->energy();  
	      ev.mcid[ev.nmcparticles]=genpartgdau->pdgId();
	      ev.nmcparticles++;
	    }
	  
	  if(isZ and nleptons==2) genZll=zcand;
	  
	  igenpartdau++;
	}
      igenpart++;
    }
  
  //gen MET (longitudinal momentum is included - do not use directly the energy afterwards)
  ev.mcpx[ev.nmcparticles]=genMET.px();
  ev.mcpy[ev.nmcparticles]=genMET.py();
  ev.mcpz[ev.nmcparticles]=genMET.pz();
  ev.mcen[ev.nmcparticles]=genMET.energy();
  ev.mcid[ev.nmcparticles]=0;
  ev.nmcparticles++;


  //add the generator level jets
  int ngenjets(0);
  edm::Handle<edm::View<reco::Candidate> > genJetsH;
  event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("genJets"), genJetsH );
  for(size_t ijet=0; ijet<genJetsH.product()->size(); ijet++)
    {
      reco::CandidatePtr gjIt = genJetsH->ptrAt(ijet);
      if(gjIt->pt()<15 /*|| fabs(gjIt->eta())>2.5*/) continue;
      
      //remove overlaps with leptons
      bool overlap(false);
      for(int imcpart=0; imcpart<ev.nmcparticles; imcpart++)
	{
	  int id=fabs(ev.mcid[imcpart]);
	  if(id!=11 && id!=13 && id!=15) continue;
	  LorentzVector p4(ev.mcpx[imcpart],ev.mcpy[imcpart],ev.mcpz[imcpart],ev.mcen[imcpart]);
	  double dr = deltaR(p4,gjIt->p4());
	  if(dr<0.4) overlap=true;
	}
      if(overlap) continue;
      
      ngenjets++;
      ev.mcpx[ev.nmcparticles]=gjIt->px();  
      ev.mcpy[ev.nmcparticles]=gjIt->py();  
      ev.mcpz[ev.nmcparticles]=gjIt->pz(); 
      ev.mcen[ev.nmcparticles]=gjIt->energy();  
      ev.mcid[ev.nmcparticles]=1;
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
    const pat::EventHypothesis &evhyp = (*(evHandle.product()))[0];

    //event summary to be filled
    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();

    //get the weight for the event
    float weight = addMCtruth(evhyp, event, iSetup );    

    //vertex info
    edm::Handle<std::vector<reco::Vertex> > vertexHandle;
    event.getByLabel(edm::InputTag("cleanEvent:selectedVertices"),vertexHandle);
    if(!vertexHandle.isValid()) 
      {
	cout << "No vertex selected" << endl;
	return;
      }

    //get other vertices
    std::vector<reco::VertexRef> selVertices;
    try{
      edm::Handle<reco::VertexCollection> hVtx;
      event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
      selVertices = vertex::filter(hVtx,objConfig_["Vertices"]);
    }catch(std::exception &e){
    }
        
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

    //average energy density
    edm::Handle< double > rho;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"), rho );

    //
    // VERTEX KINEMATICS
    //
    //MC truth on pileup (if available)
    controlHistos_.fillHisto("ngoodvertex","all",selVertices.size(),weight);
    
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
    const reco::Vertex *primVertex = &(*(vertexHandle.product()))[0];
    controlHistos_.fillHisto("ngoodvertex",istream,selVertices.size(),weight);
    controlHistos_.fillHisto("vertex_sumpt",istream,vertex::getVertexMomentumFlux(primVertex),weight);
    controlHistos_.fillHisto("vertex_pt",istream,primVertex->p4().pt(),weight);
    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++)
      {
	if(vit->get()->position()==primVertex->position()) continue;
	controlHistos_.fillHisto("othervertex_sumpt",istream,vertex::getVertexMomentumFlux(vit->get()),weight);
	controlHistos_.fillHisto("othervertex_pt",istream,vit->get()->p4().pt(),weight);
      }
        
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

    reco::CandidatePtr lepton2 = evhyp["leg2"];
    LorentzVector lepton2P(lepton2->p4());
    int l2id=lepton::getLeptonId(lepton2);
    double lepton2pterr=lepton::getPtErrorFor(lepton2);
    std::vector<double> lepton2iso=lepton::getLeptonIso(lepton2);
    genLepton=lepton::getLeptonGenMatch(lepton2);
    int genid2 = (genLepton==0? 0 : genLepton->pdgId());

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

    //veto-other isolated leptons
    int lepMult(2);
    for (pat::eventhypothesis::Looper<pat::Electron> ele = evhyp.loopAs<pat::Electron>("electron"); ele; ++ele) lepMult++;
    for (pat::eventhypothesis::Looper<pat::Muon> mu = evhyp.loopAs<pat::Muon>("muon"); mu; ++mu) lepMult++;
    controlHistos_.fillHisto("nleptons",istream,lepMult,weight);

    
    //
    // JET KINEMATICS
    //
    //count the jets in the event
    int njets(0),nbjets(0);
    std::vector<LorentzVector> jetmomenta;
    std::vector<const pat::Jet *> assocJets;
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) 
      {
	njets++;
	jetmomenta.push_back(jet->p4());
	assocJets.push_back( jet.get() );
	float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
	nbjets += (btag>1.7);

	controlHistos_.fillHisto("jetfassoc",istream,jet::fAssoc(jet.get(),primVertex),weight);
	controlHistos_.fillHisto("jetbtags",istream,btag,weight);
	controlHistos_.fillHisto("jetpt",istream,jet->pt(),weight);
	controlHistos_.fillHisto("jeteta",istream,jet->eta(),weight);

	//save for event summary
	ev.px[njets+3] = jet->px();  ev.py[njets+3]=jet->py();  ev.pz[njets+3]=jet->pz(); ev.en[njets+3]=jet->energy();  ev.id[njets+3]=1;
	const reco::Candidate *genParton = jet->genParton();
	ev.genid[njets+3] = genParton ? genParton->pdgId() : -9999;
	ev.info1[njets+3]=btag;
	ev.info2[njets+3]=jet->bDiscriminator("trackCountingHighPurBJetTags");
	ev.info3[njets+3]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	ev.info4[njets+3]=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");

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
	controlHistos_.fillHisto("pujetfassoc",istream,jet::fAssoc(jet.get(),primVertex),weight);
	controlHistos_.fillHisto("pujetbtags",istream,btag,weight);
        controlHistos_.fillHisto("pujetpt",istream,jet->pt(),weight);
	controlHistos_.fillHisto("pujeteta",istream,jet->eta(),weight);
      }
    controlHistos_.fillHisto("pujetmult",istream,npujets,weight);
    controlHistos_.fillHisto("pujetbmult",istream,npubjets,weight);

    //
    // MET kinematics
    //
    //the met
    const pat::MET *evmet = dynamic_cast<const pat::MET *>(evhyp["met"].get());
    LorentzVector met(evmet->px(),evmet->py(),0,evmet->pt());

    //reduced met
    rmet_.compute(lepton1->p4(),lepton1pterr, lepton2->p4(),lepton2pterr, jetmomenta, met);
    float reducedMET=rmet_.reducedMET();
    LorentzVector rmetP(rmet_.reducedMETComponents().first,rmet_.reducedMETComponents().second,0,reducedMET);
    std::pair<double, double> rmet_dil = rmet_.dileptonProjComponents();
    std::pair<double, double> rmet_delta = rmet_.dileptonPtCorrComponents();
    std::pair<double, double> rmet_rjet = rmet_.sumJetProjComponents();
    std::pair<double, double> rmet_met = rmet_.metProjComponents();
    std::pair<double, double> rmet_r = rmet_.recoilProjComponents();

    //projected MET
    float dphil2met[]={ fabs(deltaPhi(met.phi(),lepton1P.phi())), fabs(deltaPhi(met.phi(),lepton2P.phi())) };
    float dphimin=TMath::MinElement(sizeof(dphil2met)/sizeof(float),dphil2met);
    float relMET=met.pt();
    if(fabs(dphimin)<TMath::Pi()/2) relMET = relMET*TMath::Sin(dphimin);

    //correlation with dilepton
    float dphimet2zll = deltaPhi(met.phi(),dileptonP.phi());
    
    //transverse masses
    float mTlmet[]={ TMath::Sqrt(2*met.pt()*lepton1P.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*met.pt()*lepton2P.pt()*(1-TMath::Cos(dphil2met[1]))) };
    LorentzVector transvSum=dileptonP + met;
    float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.pt(),2)+pow(dileptonP.mass(),2))+TMath::Sqrt(TMath::Power(met.pt(),2)+pow(dileptonP.mass(),2)),2);
    transverseMass-=TMath::Power(transvSum.pt(),2);
    transverseMass=TMath::Sqrt(transverseMass);

    //final control histograms
    controlHistos_.fillHisto("met",istream,met.pt(),weight);
    controlHistos_.fillHisto("rmet",istream,reducedMET,weight);
    controlHistos_.fillHisto("met2dilepton_dphi",istream,fabs(dphimet2zll),weight);

    //save summary
    ev.run=event.id().run();
    ev.lumi=event.luminosityBlock();
    ev.event=event.id().event();
    ev.cat=selPath;
    ev.nvtx=selVertices.size();
    ev.rho=*rho;
    ev.nparticles=4+njets;
    ev.px[0] = lepton1P.px();    ev.py[0]=lepton1P.py();      ev.pz[0]=lepton1P.pz();     ev.en[0]=lepton1P.energy();  ev.id[0]=l1id; 
    ev.info1[0] = lepton1pterr;  ev.info2[0] = lepton1iso[0]; ev.info3[0]=lepton1iso[1];  ev.info4[0]=lepton1iso[2];   ev.genid[0] = genid1;
    ev.px[1] = lepton2P.px();    ev.py[1]=lepton2P.py();      ev.pz[1]=lepton2P.pz();     ev.en[1]=lepton2P.energy();  ev.id[1]=l2id; 
    ev.info1[1] = lepton2pterr;  ev.info2[1] = lepton2iso[0]; ev.info3[1]=lepton2iso[1];  ev.info4[1]=lepton2iso[2];   ev.genid[1] = genid2;
    ev.px[2] = met.px();         ev.py[2]=met.py();           ev.pz[2]=0;                 ev.en[2]=met.pt();           ev.id[2]=0; 
    ev.px[3]=primVertex->p4().px();  ev.py[3]=primVertex->p4().py();  ev.pz[3]=primVertex->p4().pz();  ev.en[3]=primVertex->p4().energy(); ev.id[3]=500;

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


 
