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
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:
  inline TH1 *getHist(TString key)
  {
    if(results_.find(key)==results_.end()) return 0;
    return (TH1 *)results_[key];
  }
  std::map<TString, TObject *>  results_;
  std::map<std::string, edm::ParameterSet> objConfig_;
  ReducedMETComputer rmet_;
  ZZ2l2nuSummaryHandler summaryHandler_;
};


using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
  : rmet_(1.0,1.0,0.0,0.0,1.0)
{
  try{
    
    edm::Service<TFileService> fs;
    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    

    //generated pileup (use directly loose selection)
    objConfig_["Vertices"] = iConfig.getParameter<edm::ParameterSet>("Vertices");
    results_["ngenpileup"] = formatPlot( baseDir.make<TH1F>("ngenpileup", ";Pileup; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);
    results_["ngoodvertex_ngenpileup"]  = formatPlot( baseDir.make<TH2F>("ngoodvertex_ngenpileup",";Pileup;Vertices; Events",25,0.,25.,25,0,25.), 1,1,1,20,0,false,true,1,1,1);
    results_["ngenpileupOOT"] = formatPlot( baseDir.make<TH1F>("ngenpileupOOT", ";Out-of-time Pileup; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);
    results_["ngoodvertex"] = formatPlot( baseDir.make<TH1F>("ngoodvertex", ";Vertices; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);

    //selection streams
    TString streams[]={"ee","mumu","emu"};
    TString selsteps[]={"Reco","2 leptons","2 good leptons","|M-M_{Z}|<15","3^{rd} lepton veto"};
    const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
    for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      TString cat=streams[istream];
      TFileDirectory newDir=baseDir.mkdir(cat.Data());
            
      results_[cat+"_cutflow"]=formatPlot( newDir.make<TH1F>(cat+"_cutflow", ";Steps; Events", nselsteps, 0.,nselsteps), 1,1,1,20,0,false,true,1,1,1);
      if(istream==0) results_["cutflow"]=formatPlot( baseDir.make<TH1F>("cutflow", ";Steps; Events", nselsteps, 0.,nselsteps), 1,1,1,20,0,false,true,1,1,1);
      for(size_t istep=0; istep<nselsteps; istep++)
	{
	  ((TH1F *)results_[cat+"_cutflow"])->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
	  if(istream==0)  ((TH1F *)results_["cutflow"])->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
	}
  
      //vertex control
      results_[cat+"_vertex_mindz"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_mindz", ";min #Delta z [cm]; Events", 100, -25.,25.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_vertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_vertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_vertex_rank"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_rank", ";Vertex rank; Events", 10, 0.,10.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngoodvertex"] = formatPlot( newDir.make<TH1F>(cat+"_ngoodvertex", ";Vertices; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);

      //lepton control
      results_[cat+"_nleptons"]    = formatPlot( newDir.make<TH1F>(cat+"_nleptons",";Lepton multiplicity; Events",4,2,6), 1,1,1,20,0,false,true,1,1,1);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->GetNbins(); ibin++)
        {
          TString ilabel(""); ilabel+=(ibin+1);
          if(ibin==((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}

      //dilepton control plots
      TString dilTypes[2]={"ss","os"};
      for(size_t idilType=0; idilType<2; idilType++)
	{
	  results_[cat+"_"+dilTypes[idilType]+"dilepton_mass"]     = formatPlot( newDir.make<TH1F>(cat+"_"+dilTypes[idilType]+"dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+dilTypes[idilType]+"dilepton_sumpt"]    = formatPlot( newDir.make<TH1F>(cat+"_"+dilTypes[idilType]+"dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+dilTypes[idilType]+"dilepton_dphi"]    = formatPlot( newDir.make<TH1F>(cat+"_"+dilTypes[idilType]+"dilepton_dphi", ";|#Delta #phi| [rad]; Events", 50, 0.,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+dilTypes[idilType]+"dilepton_pt"]       = formatPlot( newDir.make<TH1F>(cat+"_"+dilTypes[idilType]+"dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	}
      
      results_[cat+"_dilepton_atres"]     = formatPlot( newDir.make<TH1F>(cat+"_dilepton_atres", ";a_{#perp} difference [GeV]; Events", 100, -50.,50.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_alres"]     = formatPlot( newDir.make<TH1F>(cat+"_dilepton_alres", ";a_{#parallel} difference [GeV]; Events", 100, -50.,50.), 1,1,1,20,0,false,true,1,1,1);

      //jets
      TString jetTypes[2] ={"jet","pujet"};
      for(size_t ijetType=0; ijetType<2; ijetType++)
	{
	  results_[cat+"_"+jetTypes[ijetType]+"fassoc"] = formatPlot( newDir.make<TH1F>(cat+"_"+jetTypes[ijetType]+"fassoc",";#beta_{vtx}(Jet); Jets",50,0,1.0), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+jetTypes[ijetType]+"pt"]     = formatPlot( newDir.make<TH1F>(cat+"_"+jetTypes[ijetType]+"pt",";p_{T} [GeV/c]; Jets",50,0,200), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+jetTypes[ijetType]+"eta"]    = formatPlot( newDir.make<TH1F>(cat+"_"+jetTypes[ijetType]+"eta",";#eta; Jets",50,-2.5,2.5), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+jetTypes[ijetType]+"btags"]  = formatPlot( newDir.make<TH1F>(cat+"_"+jetTypes[ijetType]+"btags",";b tags (TCHE); Jets",50,-5,45), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+jetTypes[ijetType]+"mult"]   = formatPlot( newDir.make<TH1F>(cat+"_"+jetTypes[ijetType]+"mult",";Jet multiplicity; Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
	  results_[cat+"_"+jetTypes[ijetType]+"bmult"]  = formatPlot( newDir.make<TH1F>(cat+"_"+jetTypes[ijetType]+"bmult",";b tag multiplicity (TCHEL); Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
	  for(int ibin=1; ibin<=((TH1F *)results_[cat+"_"+jetTypes[ijetType]+"mult"])->GetXaxis()->GetNbins(); ibin++)
	    {
	      TString ilabel(""); ilabel+=(ibin-1);
	      if(ibin==((TH1F *)results_[cat+"_"+jetTypes[ijetType]+"mult"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	      ((TH1F *)results_[cat+"_"+jetTypes[ijetType]+"mult"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	      ((TH1F *)results_[cat+"_"+jetTypes[ijetType]+"bmult"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	    }
	}

      //MET
      results_[cat+"_met"]               = formatPlot( newDir.make<TH1F>(cat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,500.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_jesmet"]            = formatPlot( newDir.make<TH1F>(cat+"_jesmet", ";#slash{E}_{T} (JES corrected) [GeV/c]; Events", 100,  0.,500.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_jesmetnopu"]        = formatPlot( newDir.make<TH1F>(cat+"_jesmetnopu", ";#slash{E}_{T} (JES corrected, no PU) [GeV/c]; Events", 100,  0.,500.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_metresol"]          = formatPlot( newDir.make<TH1F>(cat+"_metresol", ";#slash{E}_{T}-|#sum_{#nu}#vec{p}_{T}| [GeV/c]; Events", 50,  -100.,100.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_metsig"]            = formatPlot( newDir.make<TH1F>(cat+"_metsig", ";#slash{E}_{T} significance; Events", 50,  0.,100.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_tmet"]              = formatPlot( newDir.make<TH1F>(cat+"_tmet", ";track #slash{E}_{T} [GeV/c]; Events", 100,0.,500.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_rmet"]              = formatPlot( newDir.make<TH1F>(cat+"_rmet", ";reduced #slash{E}_{T} [GeV/c]; Events", 100,0.,500.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_relmet"]            = formatPlot( newDir.make<TH1F>(cat+"_relmet", ";relative #slash{E}_{T} [GeV/c]; Events", 100,0.,500.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_met2dilepton_dphi"] = formatPlot( newDir.make<TH1F>(cat+"_met2dilepton_dphi", ";#Delta #phi(#slash{E}_{T},vertex) [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_mT_individualsum"]  = formatPlot( newDir.make<TH1F>(cat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_mT"]                = formatPlot( newDir.make<TH1F>(cat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",50,0,1000), 1,1,1,20,0,false,true,1,1,1);
    }
  }catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) {


  try{

    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();

    //get the weight for the event
    float weight=1;
    if(!event.isRealData())
      {
	edm::Handle<float> puWeightHandle;
	event.getByLabel("puWeights","puWeight",puWeightHandle);
	if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
      }

    //get objects for this event
    edm::Handle<std::vector<pat::EventHypothesis> > evHandle;
    event.getByLabel(edm::InputTag("cleanEvent:selectedEvent"),evHandle);
    if(!evHandle.isValid()) 
      {
	cout << "No valid event hypothesis" << endl;
	return;
      }
    const pat::EventHypothesis &evhyp = (*(evHandle.product()))[0];
    
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
        
    edm::Handle< std::vector<int> > selInfo;
    event.getByLabel(edm::InputTag("cleanEvent:selectionInfo"),selInfo);
    if(!selInfo.isValid()) 
      {
	cout << "No valid selection info" << endl;
	return;
      }
    int selPath = (*(selInfo.product()))[0];
    int selStep = (*(selInfo.product()))[1];

    edm::Handle< double > rho;
    event.getByLabel(edm::InputTag("kt6PFJets:rho"),rho);

    edm::Handle< std::vector<reco::PFCandidate> > pfCands;
    event.getByLabel(edm::InputTag("particleFlow"),pfCands);
    
    edm::Handle<std::vector<reco::PFMET> > hPfMET; 
    event.getByLabel(edm::InputTag("pfMet"), hPfMET);
    const reco::PFMET &pfmet = (*hPfMET)[0];
    reco::CandidatePtr evmet = evhyp["met"];
    LorentzVector met(evmet->px(),evmet->py(),0,evmet->pt());

    //
    // VERTEX KINEMATICS
    //
    //MC truth on pileup (if available)
    int npuOOT(0),npuIT(0);
    getHist("ngoodvertex")->Fill(selVertices.size(),weight);
    if(!event.isRealData())
      {
	edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
	event.getByType(puInfoH);
	if(puInfoH.isValid())
	  {
	    for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	      {
		if(it->getBunchCrossing()==0) npuIT += it->getPU_NumInteractions();
		else npuOOT += it->getPU_NumInteractions();
	      }
	    getHist("ngenpileup")->Fill(npuIT,weight);
	    getHist("ngenpileupOOT")->Fill(npuOOT,weight);
	    ((TH2 *)getHist("ngoodvertex_ngenpileup"))->Fill(npuIT,selVertices.size(),weight);
	  }
      }
    
    //require that a dilepton has been selected
    if(selPath==0 or selStep<3) return;
    std::string istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";
    getHist("cutflow")->Fill(2,weight);
    getHist(istream+"_cutflow")->Fill(2,weight);

    //
    // VERTEX KINEMATICS (get primary vertex selected)
    //
    const reco::Vertex *primVertex = &(*(vertexHandle.product()))[0];
    getHist(istream+"_ngoodvertex")->Fill(selVertices.size(),weight);
    getHist(istream+"_vertex_sumpt")->Fill(vertex::getVertexMomentumFlux(primVertex),weight);
    getHist(istream+"_vertex_pt")->Fill(primVertex->p4().pt(),weight);
    int irank(0),ivtx(0);
    float mindz(1000);
    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++,ivtx++)
      {
	if(vit->get()->position()==primVertex->position())
	  {
	    irank=ivtx;
	    continue;
	  }
	float dz = vit->get()->position().z()-primVertex->position().z();
	if(fabs(dz)<fabs(mindz)) mindz=dz;

	getHist(istream+"_othervertex_sumpt")->Fill(vertex::getVertexMomentumFlux(vit->get()),weight);
	getHist(istream+"_othervertex_pt")->Fill(vit->get()->p4().pt(),weight);
      }
    getHist(istream+"_vertex_mindz")->Fill(mindz,weight);
    getHist(istream+"_vertex_rank")->Fill(irank,weight);


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
    getHist(istream+"_"+dilCat+"_mass")->Fill(dileptonP.mass(),weight);
    if(dileptonP.mass()<40) return;
    if(fabs(l1id)==fabs(l2id) && fabs(dileptonP.mass()-91)>15) return;
    getHist("cutflow")->Fill(3,weight);
    getHist(istream+"_cutflow")->Fill(3,weight);

    double dphill=deltaPhi(lepton1P.phi(),lepton2P.phi());
    getHist(istream+"_"+dilCat+"_sumpt")->Fill(lepton1P.pt()+lepton2P.pt(),weight);
    getHist(istream+"_"+dilCat+"_pt")->Fill(dileptonP.pt(),weight);
    getHist(istream+"_"+dilCat+"_dphi")->Fill( fabs(dphill) ,weight );

    //veto-other isolated leptons
    int lepMult(2);
    for (pat::eventhypothesis::Looper<pat::Electron> ele = evhyp.loopAs<pat::Electron>("electron"); ele; ++ele) lepMult++;
    for (pat::eventhypothesis::Looper<pat::Muon> mu = evhyp.loopAs<pat::Muon>("muon"); mu; ++mu) lepMult++;
    getHist(istream+"_nleptons")->Fill(lepMult,weight);

    if(lepMult>2) return;
    getHist("cutflow")->Fill(4,weight);
    getHist(istream+"_cutflow")->Fill(4,weight);

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

	getHist(istream+"_jetfassoc")->Fill(jet::fAssoc(jet.get(),primVertex),weight);
	getHist(istream+"_jetbtags")->Fill(btag,weight);
	getHist(istream+"_jetpt")->Fill(jet->pt(),weight);
	getHist(istream+"_jeteta")->Fill(jet->eta(),weight);

	//save for event summary
	ev.px[njets+3] = jet->px();  ev.py[njets+3]=jet->py();  ev.pz[njets+3]=jet->pz(); ev.en[njets+3]=jet->energy();  ev.id[njets+3]=1;
	const reco::Candidate *genParton = jet->genParton();
	ev.genid[njets+3] = genParton ? genParton->pdgId() : -9999;
	ev.info1[njets+3]=btag;
	ev.info2[njets+3]=jet->bDiscriminator("trackCountingHighPurBJetTags");
	ev.info3[njets+3]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	ev.info4[njets+3]=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");

      }
    getHist(istream+"_jetmult")->Fill(njets,weight);
    getHist(istream+"_jetbmult")->Fill(nbjets,weight);	
    
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
	getHist(istream+"_pujetfassoc")->Fill(jet::fAssoc(jet.get(),primVertex),weight);
	getHist(istream+"_pujetbtags")->Fill(btag,weight);
        getHist(istream+"_pujetpt")->Fill(jet->pt(),weight);
	getHist(istream+"_pujeteta")->Fill(jet->eta(),weight);
      }
    getHist(istream+"_pujetmult")->Fill(npujets,weight);
    getHist(istream+"_pujetbmult")->Fill(npubjets,weight);

        
    //
    // MET kinematics
    //
    //the met
    float metsig( pfmet.significance() );
    LorentzVectorCollection corMets = met::filter(met,assocJets,puJets,false);
    LorentzVector rawMetP(pfmet.p4());
    LorentzVector jesMetP(met);
    LorentzVector jesMetNopuP(corMets[met::CORRECTED_TYPEIMET]);

    //MC truth on MET
    LorentzVector genHiggs(0,0,0,0);
    LorentzVector genMET(0,0,0,0),genZll(0,0,0,0);
    if(!event.isRealData())
      {     
	int igenpart(0);
	for (pat::eventhypothesis::Looper<reco::GenParticle> genpart = evhyp.loopAs<reco::GenParticle>("genparticle"); genpart; ++genpart) 
	  {
	    //higgs level (H)
	    if(fabs(genpart->pdgId())==25) genHiggs=genpart->p4();
	    
	    int igenpartdau(0);
	    char buf[20];
	    sprintf(buf,"gendaughter_%d",igenpart);
	    for(pat::eventhypothesis::Looper<reco::GenParticle> genpartdau = evhyp.loopAs<reco::GenParticle>(buf); genpartdau; ++genpartdau)
	      {
		//higgs daughters level (ZZ/WW)
		//cout << genpartdau->pdgId() << " (" << flush;
		bool isZ(fabs(genpartdau->pdgId())==23);
		LorentzVector zcand=genpartdau->p4();

		char buf[20];
		sprintf(buf,"gendaughter_%d_%d",igenpart,igenpartdau);
		int nleptons(0);
		for(pat::eventhypothesis::Looper<reco::GenParticle> genpartgdau = evhyp.loopAs<reco::GenParticle>(buf); genpartgdau; ++genpartgdau)
		  {
		    //final state products (leptons, jets, neutrinos)
		    //cout << genpartgdau->pdgId() << " " << flush;

		    int pdgid=fabs(genpartgdau->pdgId());
		    if(pdgid==12||pdgid==14||pdgid==16) genMET += genpartgdau->p4();
		    if(pdgid==11||pdgid==13||pdgid==15) nleptons++;
		  }
		
		if(isZ and nleptons==2) genZll=zcand;

		//cout << ") " << flush;
		igenpartdau++;
	      }
	    igenpart++;
	  }
	//cout << endl;
      }

    //reduced met
    rmet_.defineThrust(lepton1->p4(),lepton1pterr,lepton2->p4(),lepton2pterr);
    TVector2 dilProj=rmet_.project(dileptonP);
    TVector2 genDilProj=rmet_.project(genZll);
    getHist(istream+"_dilepton_alres")->Fill(dilProj.Px()-genDilProj.Px(),weight);
    getHist(istream+"_dilepton_atres")->Fill(dilProj.Py()-genDilProj.Py(),weight);

    rmet_.compute(lepton1->p4(),lepton1pterr, lepton2->p4(),lepton2pterr, jetmomenta, jesMetNopuP );
    float reducedMET=rmet_.reducedMET();
    LorentzVector rmetP(rmet_.reducedMETComponents().first,rmet_.reducedMETComponents().second,0,reducedMET);
    std::pair<double, double> rmet_dil = rmet_.dileptonProjComponents();
    std::pair<double, double> rmet_delta = rmet_.dileptonPtCorrComponents();
    std::pair<double, double> rmet_rjet = rmet_.sumJetProjComponents();
    std::pair<double, double> rmet_met = rmet_.metProjComponents();
    std::pair<double, double> rmet_r = rmet_.recoilProjComponents();

    //projected MET
    float dphil2met[]={ fabs(deltaPhi(jesMetNopuP.phi(),lepton1P.phi())), fabs(deltaPhi(jesMetNopuP.phi(),lepton2P.phi())) };
    float dphimin=TMath::MinElement(sizeof(dphil2met)/sizeof(float),dphil2met);
    float relMET=jesMetNopuP.pt();
    if(fabs(dphimin)<TMath::Pi()/2) relMET = relMET*TMath::Sin(dphimin);

    //charged candidates based met 
    //(use only charged candidates associated to the primary vertex and neutral candidates with pT>5 GeV)
    LorentzVector tmet(0,0,0,0);
    if(pfCands.isValid())
      {
	LorentzVector neutralSum(0,0,0,0), chSum(0,0,0,0);
	for(std::vector<reco::PFCandidate>::const_iterator it = pfCands->begin(); it != pfCands->end(); it++)
	  {
	    LorentzVector candP(it->p4());
	    if(it->charge()==0 && candP.pt()>5 && fabs(candP.eta())<3)     neutralSum -= candP;
	    else if(it->charge())
	      {
		math::XYZPointD vec( it->vertex().x()-primVertex->position().x(),
				     it->vertex().y()-primVertex->position().y(),
				     it->vertex().z()-primVertex->position().z() );
		if( vec.R()>0.1) continue;
		chSum -= candP;
	      }
	  }
	tmet = chSum + neutralSum;
      }
    
    //correlation with dilepton
    float dphimet2zll = deltaPhi(jesMetNopuP.phi(),dileptonP.phi());
    
    //transverse masses
    float mTlmet[]={ TMath::Sqrt(2*jesMetNopuP.pt()*lepton1P.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*jesMetNopuP.pt()*lepton2P.pt()*(1-TMath::Cos(dphil2met[1]))) };
    LorentzVector transvSum=dileptonP + jesMetNopuP;
    float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.pt(),2)+pow(dileptonP.mass(),2))+TMath::Sqrt(TMath::Power(jesMetNopuP.pt(),2)+pow(dileptonP.mass(),2)),2);
    transverseMass-=TMath::Power(transvSum.pt(),2);
    transverseMass=TMath::Sqrt(transverseMass);

    //final control histograms
    getHist(istream+"_met")->Fill(rawMetP.pt(),weight);
    getHist(istream+"_jesmet")->Fill(jesMetP.pt(),weight);
    getHist(istream+"_jesmetnopu")->Fill(jesMetNopuP.pt(),weight);
    getHist(istream+"_metresol")->Fill(jesMetNopuP.pt()-genMET.pt(),weight);
    getHist(istream+"_metsig")->Fill(metsig,weight);
    getHist(istream+"_rmet")->Fill(reducedMET,weight);
    getHist(istream+"_relmet")->Fill(relMET,weight);
    getHist(istream+"_tmet")->Fill(tmet.pt());
    getHist(istream+"_met2dilepton_dphi")->Fill(fabs(dphimet2zll),weight);
    getHist(istream+"_mT_individualsum")->Fill(mTlmet[0]+mTlmet[1],weight);
    getHist(istream+"_mT")->Fill(transverseMass,weight);

    //save summary
    ev.run=event.id().run();
    ev.lumi=event.luminosityBlock();
    ev.event=event.id().event();
    ev.cat=selPath;
    ev.nvtx=selVertices.size();
    ev.ngenpu=npuIT;
    ev.njets=njets;
    ev.nbtags=nbjets;
    ev.weight=weight;
    ev.redmet_pt=rmetP.pt();              ev.redmet_phi=rmetP.phi();
    ev.redmet_dilL = rmet_dil.first;      ev.redmet_dilT = rmet_dil.second;
    ev.redmet_deltaL = rmet_delta.first;  ev.redmet_deltaT = rmet_delta.second;
    ev.redmet_rjetL = rmet_rjet.first;    ev.redmet_rjetT = rmet_rjet.second;
    ev.redmet_metL = rmet_met.first;      ev.redmet_metT = rmet_met.second;
    ev.redmet_rL = rmet_r.first;          ev.redmet_rT = rmet_r.second;
    ev.projmet_pt=relMET;
    ev.tkmet_pt=tmet.pt();       ev.tkmet_phi=tmet.phi();
    ev.genmet_pt=genMET.pt();    ev.genmet_phi=genMET.phi();
    ev.rho=*rho;
    ev.nparticles=4+njets;
    ev.px[0] = lepton1P.px();    ev.py[0]=lepton1P.py();      ev.pz[0]=lepton1P.pz();     ev.en[0]=lepton1P.energy();  ev.id[0]=l1id; 
    ev.info1[0] = lepton1pterr;  ev.info2[0] = lepton1iso[0]; ev.info3[0]=lepton1iso[1];  ev.info4[0]=lepton1iso[2];   ev.genid[0] = genid1;
    ev.px[1] = lepton2P.px();    ev.py[1]=lepton2P.py();      ev.pz[1]=lepton2P.pz();     ev.en[1]=lepton2P.energy();  ev.id[1]=l2id; 
    ev.info1[1] = lepton2pterr;  ev.info2[1] = lepton2iso[0]; ev.info3[1]=lepton2iso[1];  ev.info4[1]=lepton2iso[2];   ev.genid[1] = genid2;

    ev.px[2] = jesMetP.px(); ev.py[2]=jesMetP.py();   ev.pz[2]=0;  ev.en[2]=jesMetP.pt(); ev.id[2]=0; 
    ev.info1[2] = jesMetNopuP.px();  ev.info2[2] = jesMetNopuP.py();  ev.info3[3]=genMET.pt();
    

    ev.px[3]=primVertex->p4().px();
    ev.py[3]=primVertex->p4().py();
    ev.pz[3]=primVertex->p4().pz();
    ev.en[3]=primVertex->p4().energy();
    ev.id[3]=500;

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
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(0.,ctrHandle->value);
      if(istream==0) ((TH1F *)getHist("cutflow"))->Fill(0.,ctrHandle->value);
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      std::string inpt= std::string(streams[istream])+"Counter";
      iLumi.getByLabel(inpt, streamCtrHandle);
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(1.,streamCtrHandle->value);
      ((TH1F *)getHist("cutflow"))->Fill(1.,ctrHandle->value);
    } 
}


DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 
