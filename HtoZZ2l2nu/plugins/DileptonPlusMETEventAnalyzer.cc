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
#include "TLorentzVector.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"

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
};


using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
{
  try{
    
    edm::Service<TFileService> fs;

    objConfig_["Vertices"] = iConfig.getParameter<edm::ParameterSet>("Vertices");
    
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    
    TString streams[]={"ee","mumu","emu"};
    TString jetmult[]={"eq0jets","eq1jets","geq2jets"};
    TString selsteps[]={"Reco","2 leptons","2 good leptons","|M-M_{Z}|<15","|#Delta#phi(l+l)|<x","=0 jets","=1 jet","#geq 2 jets"};
    const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
    for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      TString cat=streams[istream];
      TFileDirectory newDir=baseDir.mkdir(cat.Data());
            
      results_[cat+"_cutflow"]=formatPlot( newDir.make<TH1F>(cat+"_cutflow", ";Steps; Events", nselsteps, 0.,nselsteps), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_cutflowlowmass"]=formatPlot( newDir.make<TH1F>(cat+"_cutflowlowmass", ";Steps; Events", nselsteps, 0.,nselsteps), 1,1,1,20,0,false,true,1,1,1);
      for(size_t istep=0; istep<nselsteps; istep++)
	{
	  ((TH1F *)results_[cat+"_cutflow"])->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
	  ((TH1F *)results_[cat+"_cutflowlowmass"])->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
	}

      //lepton control
      results_[cat+"_nleptons"]    = formatPlot( newDir.make<TH1F>(cat+"_nleptons",";Lepton multiplicity; Events",4,2,6), 1,1,1,20,0,false,true,1,1,1);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->GetNbins(); ibin++)
        {
          TString ilabel(""); ilabel+=(ibin+1);
          if(ibin==((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}
      
      //dilepton control
      results_[cat+"_dilepton_mass"]  =formatPlot( newDir.make<TH1F>(cat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_pt"]    = formatPlot( newDir.make<TH1F>(cat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_dphi"]  = formatPlot( newDir.make<TH1F>(cat+"_dilepton_dphi", ";|#Delta #phi| [rad]; Events", 50, 0.,3.2), 1,1,1,20,0,false,true,1,1,1);
	  
      //vertex control
      results_[cat+"_vertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_vertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngoodvertex"] = formatPlot( newDir.make<TH1F>(cat+"_ngoodvertex", ";Vertices; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);

      //genpileup
      results_[cat+"_ngenpileup"] = formatPlot( newDir.make<TH1F>(cat+"_ngenpileup", ";Pileup; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngoodvertex_ngenpileup"]  = formatPlot( newDir.make<TH2F>(cat+"_ngoodvertex_ngenpileup",";Pileup;Vertices; Events",25,0.,25.,25,0,25.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngenpileupOOT"] = formatPlot( newDir.make<TH1F>(cat+"_ngenpileupOOT", ";Out-of-time Pileup; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);
      
      //jets
      results_[cat+"_jetpt"]    = formatPlot( newDir.make<TH1F>(cat+"_jetpt",";p_{T} [GeV/c]; Jets",50,0,200), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_jeteta"]    = formatPlot( newDir.make<TH1F>(cat+"_jeteta",";#eta; Jets",50,-2.5,2.5), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_jetfassoc"]    = formatPlot( newDir.make<TH1F>(cat+"_jetfassoc",";f_{assoc}; Jets",50,0,1), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_njets"]    = formatPlot( newDir.make<TH1F>(cat+"_njets",";Jet multiplicity; Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_bmult"]    = formatPlot( newDir.make<TH1F>(cat+"_bmult",";b tag multiplicity (TCHEL); Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins(); ibin++)
	{
	  TString ilabel(""); ilabel+=(ibin-1);
	  if(ibin==((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_njets"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	  ((TH1F *)results_[cat+"_bmult"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}
      results_[cat+"_btags"]             = formatPlot( newDir.make<TH1F>(cat+"_btags",";b tags (TCHE); Jets",50,-5,45), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_met"]               = formatPlot( newDir.make<TH1F>(cat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_cormet"]               = formatPlot( newDir.make<TH1F>(cat+"_cormet", ";jet corrected #slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_tmet"]              = formatPlot( newDir.make<TH1F>(cat+"_tmet", ";track #slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_minmet"]            = formatPlot( newDir.make<TH1F>(cat+"_minmet", ";min(track #slash{E}_{T}, #slash{E}_{T}) [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_rmet"]              = formatPlot( newDir.make<TH1F>(cat+"_rmet", ";reduced #slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_relmet"]            = formatPlot( newDir.make<TH1F>(cat+"_relmet", ";relative #slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_mT_individualsum"]  = formatPlot( newDir.make<TH1F>(cat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_metsig"]            = formatPlot( newDir.make<TH1F>(cat+"_metsig", ";#slash{E}_{T} significance; Events", 50,  0.,100.), 1,1,1,20,0,false,true,1,1,1);

      results_[cat+"_othermT_individual"] = formatPlot( newDir.make<TH1F>(cat+"_othermT_individual",";Transverse mass(other leptons,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
      
      //split the analysis according to the jet multiplicity
      for(size_t jstream=0; jstream<sizeof(jetmult)/sizeof(TString); jstream++)
	{
	  TString subcat=cat+jetmult[jstream];
	  results_[subcat+"_dilepton_mass"]     = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_sumpt"]    = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_dphi"]    = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_dphi", ";|#Delta #phi| [rad]; Events", 50, 0.,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_pt"]       = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_["met2"+subcat+"_mindphi"]    = formatPlot( newDir.make<TH1F>("met2"+subcat+"_mindphi", ";min #Delta #phi(#slash{E}_{T},l) [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_met"]               = formatPlot( newDir.make<TH1F>(subcat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 50,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_metsig"]            = formatPlot( newDir.make<TH1F>(subcat+"_metsig", ";#slash{E}_{T} significance; Events", 50,  0.,100.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_rmet"]              = formatPlot( newDir.make<TH1F>(subcat+"_rmet", ";reduced #slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton2met_dphi"] = formatPlot( newDir.make<TH1F>(subcat+"dilepton2met_dphi", ";#Delta #phi [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_vertex2met_dphi"] = formatPlot( newDir.make<TH1F>(subcat+"vertex2met_dphi", ";#Delta #phi [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_othervertex2met_dphi"] = formatPlot( newDir.make<TH1F>(subcat+"othervertex2met_dphi", ";#Delta #phi [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_relmet"]            = formatPlot( newDir.make<TH1F>(subcat+"_relmet", ";#slash{E}_{T,rel} [GeV/c]; Events", 50,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT"]                = formatPlot( newDir.make<TH1F>(subcat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",50,0,1000), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_individual"]     = formatPlot( newDir.make<TH1F>(subcat+"_mT_individual",";Transverse mass(lepton,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_corr"]           = formatPlot( newDir.make<TH2F>(subcat+"_mT_corr",";Transverse mass(leading lepton,MET) [GeV/c^{2}];Transverse mass(trailer lepton,MET) [GeV/c^{2}]; Events",50,0,500,50,0,500), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_individualsum"]  = formatPlot( newDir.make<TH1F>(subcat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
	}
    }
  }catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) {


  try{

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

    //require that a dilepton has benn selected
    if(selPath==0 or selStep<3) return;
    std::string istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";

    getHist(istream+"_cutflow")->Fill(2,weight);
    getHist(istream+"_cutflowlowmass")->Fill(2,weight);

    //vertex quantities
    const reco::Vertex *primVertex = &(*(vertexHandle.product()))[0];
    getHist(istream+"_ngoodvertex")->Fill(selVertices.size(),weight);
    getHist(istream+"_vertex_sumpt")->Fill(vertex::getVertexMomentumFlux(primVertex),weight);
    getHist(istream+"_vertex_pt")->Fill(primVertex->p4().pt(),weight);
    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++)
      {
	if(vit->get()->position()==primVertex->position()) continue;
	getHist(istream+"_othervertex_sumpt")->Fill(vertex::getVertexMomentumFlux(vit->get()),weight);
	getHist(istream+"_othervertex_pt")->Fill(vit->get()->p4().pt(),weight);
      }

    //basic dilepton kinematics
    reco::CandidatePtr lepton1 = evhyp["leg1"];
    double lepton1pterr=dilepton::getPtErrorFor(lepton1);
    TLorentzVector lepton1P(lepton1->px(),lepton1->py(),lepton1->pz(), lepton1->energy());
    reco::CandidatePtr lepton2 = evhyp["leg2"];
    double lepton2pterr=dilepton::getPtErrorFor(lepton2);
    TLorentzVector lepton2P(lepton2->px(),lepton2->py(),lepton2->pz(), lepton2->energy());
    TLorentzVector dileptonP=lepton1P+lepton2P;
    double dphill=lepton1P.DeltaPhi(lepton2P);
    getHist(istream+"_dilepton_sumpt")->Fill(lepton1P.Pt()+lepton2P.Pt(),weight);
    getHist(istream+"_dilepton_pt")->Fill(dileptonP.Pt(),weight);
    getHist(istream+"_dilepton_mass")->Fill(dileptonP.M(),weight);
    getHist(istream+"_dilepton_dphi")->Fill( fabs(dphill) ,weight );

    if(fabs(dileptonP.M()-91)>15) return;
    getHist(istream+"_cutflow")->Fill(3,weight);
    getHist(istream+"_cutflowlowmass")->Fill(3,weight);
    
    if(fabs(dphill)>2.64) return;
    bool highmasscand(fabs(dphill)<2.25);
    getHist(istream+"_cutflowlowmass")->Fill(4,weight);
    if(highmasscand) getHist(istream+"_cutflow")->Fill(4,weight);

    //count the jets in the event
    std::vector<reco::CandidatePtr> seljets= evhyp.all("jet");
    int njets(0), nbjets(0);
    std::vector<LorentzVector> jetmomenta;
    TLorentzVector rejectedJetMomenta(0,0,0,0);
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) {
      float fassoc(jet::fAssoc(jet.get(),primVertex));
      if(fassoc>0.1) 
	jetmomenta.push_back(jet->p4());
      float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
      if(btag>1.74) nbjets+=1; //loose point
      getHist(istream+"_btags")->Fill(btag,weight);
      getHist(istream+"_jetpt")->Fill(jet->pt(),weight);
      getHist(istream+"_jeteta")->Fill(jet->eta(),weight);
      getHist(istream+"_jetfassoc")->Fill( fassoc ,weight);
    }
    njets=jetmomenta.size();
    getHist(istream+"_njets")->Fill(njets,weight);
    getHist(istream+"_bmult")->Fill(nbjets,weight);
    
    int jetbin=njets;
    if(jetbin>2)jetbin=2;
    
    //update the selection stream
    TString substream=istream; 
    if(njets==0)      substream = substream+"eq0jets";
    else if(njets==1) substream = substream+"eq1jets";
    else              substream = substream+"geq2jets";
    getHist(istream+"_cutflowlowmass")->Fill(5+jetbin,weight);
    if(highmasscand) getHist(istream+"_cutflow")->Fill(5+jetbin,weight);
    
    //base met kinematics
    const pat::MET *themet=evhyp.getAs<pat::MET>("met");
    TLorentzVector metP(themet->px(),themet->py(),0,themet->pt());
    float metsig(-1);
    try{
      const reco::MET *origMet = dynamic_cast<const reco::MET *>(themet->originalObject());
      metsig=origMet->significance();
    }catch(std::exception &e){
      metsig=themet->significance();
      cout << e.what() << endl;
    }

    //require MET back-to-back to dilepton
    float dphiZ2met=dileptonP.DeltaPhi(metP);

    //check other leptons in the event
    int lepMult(2);
    for (pat::eventhypothesis::Looper<pat::Electron> ele = evhyp.loopAs<pat::Electron>("electron"); ele; ++ele) 
      {
	lepMult++;
	TLorentzVector lepP(ele->px(),ele->py(),ele->pz(), ele->energy());
	float dphi=fabs(metP.DeltaPhi(lepP));
	float mTlm=TMath::Sqrt(2*metP.Pt()*lepP.Pt()*(1-TMath::Cos(dphi)));
	getHist(istream+"_othermT_individual")->Fill(mTlm,weight);
      }
    for (pat::eventhypothesis::Looper<pat::Muon> mu = evhyp.loopAs<pat::Muon>("muon"); mu; ++mu) 
      {
	lepMult++;
	TLorentzVector lepP(mu->px(),mu->py(),mu->pz(), mu->energy());
	float dphi=fabs(metP.DeltaPhi(lepP));
	float mTlm=TMath::Sqrt(2*metP.Pt()*lepP.Pt()*(1-TMath::Cos(dphi)));
	getHist(istream+"_othermT_individual")->Fill(mTlm,weight);
      }
    getHist(istream+"_nleptons")->Fill(lepMult,weight);

    //reduced met
    rmet_.compute(lepton1->p4(),lepton1pterr,
		  lepton2->p4(),lepton2pterr,
		  jetmomenta,
		  themet->p4()
		  );
    float reducedMET=rmet_.reducedMET();

    //charged candidates based met
    TLorentzVector tmet(0,0,0,0);
    if(pfCands.isValid())
      {
	TLorentzVector neutralSum_8(0,0,0,0), chSum(0,0,0,0);
	for(std::vector<reco::PFCandidate>::const_iterator it = pfCands->begin(); it != pfCands->end(); it++)
	  {
	    TLorentzVector candP(it->px(),it->py(),it->pz(),it->energy());
	    if(it->charge()==0 && candP.Pt()>8)                            neutralSum_8 -= candP;
	    else if(it->charge() && it->vertex()==primVertex->position() ) chSum -= candP;
	  }
	tmet = chSum + neutralSum_8;
      }

    //rejected jet corrected met
    LorentzVector thecormet = themet->p4()+LorentzVector(rejectedJetMomenta.Px(),
							 rejectedJetMomenta.Py(),
							 rejectedJetMomenta.Pz(),
							 rejectedJetMomenta.E());
    getHist(istream+"_cormet")->Fill( thecormet.pt() );
    getHist(istream+"_tmet")->Fill(tmet.Pt());
    getHist(istream+"_minmet")->Fill(min(tmet.Pt(),thecormet.pt()));

    //individual lepton vs MET kinematics
    float dphil2met[]={ fabs(metP.DeltaPhi(lepton1P)), fabs(metP.DeltaPhi(lepton2P)) };
    float mTlmet[]={ TMath::Sqrt(2*metP.Pt()*lepton1P.Pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*metP.Pt()*lepton2P.Pt()*(1-TMath::Cos(dphil2met[1]))) };
    float dphimin=TMath::MinElement(sizeof(dphil2met)/sizeof(float),dphil2met);
    float relMET=metP.Pt();
    if(fabs(dphimin)<TMath::Pi()/2) relMET = relMET*TMath::Sin(dphimin);
    
    //dilepton vs MET kinematics
    TLorentzVector transvSum=dileptonP + metP;
    float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.Pt(),2)+pow(dileptonP.M(),2))+TMath::Sqrt(TMath::Power(metP.Pt(),2)+pow(dileptonP.M(),2)),2);
    transverseMass-=TMath::Power(transvSum.Pt(),2);
    transverseMass=TMath::Sqrt(transverseMass);
    
    getHist(istream+"_met")->Fill(metP.Pt(),weight);
    getHist(istream+"_metsig")->Fill(metsig,weight);
    getHist(istream+"_rmet")->Fill(reducedMET,weight);
    getHist(istream+"_relmet")->Fill(relMET,weight);
    getHist(istream+"_mT_individualsum")->Fill(mTlmet[0]+mTlmet[1],weight);
    getHist(substream+"_met")->Fill(metP.Pt(),weight);
    getHist(substream+"_rmet")->Fill(reducedMET,weight);
    getHist(substream+"_dilepton2met_dphi")->Fill(fabs(dphiZ2met),weight);
    getHist(substream+"_metsig")->Fill(metsig,weight);
    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++)
      {
	TLorentzVector vrtxP(vit->get()->p4().px(),vit->get()->p4().py(),vit->get()->p4().pz(),vit->get()->p4().energy());
	float dphi=vrtxP.DeltaPhi(metP);
	TString hname=substream+"_othervertex2met_dphi";
	if(vit->get()->position()==primVertex->position()) hname=substream+"_vertex2met_dphi";
	getHist(hname)->Fill(dphi,weight);
      }
   
    //control histograms
    getHist("met2"+substream+"_mindphi")->Fill(dphimin,weight);
    getHist(substream+"_relmet")->Fill(relMET,weight);
    getHist(substream+"_mT_individual")->Fill(mTlmet[0],weight);
    getHist(substream+"_mT_individual")->Fill(mTlmet[1],weight);
    if(lepton1P.Pt()>lepton2P.Pt()) ((TH2 *)getHist(substream+"_mT_corr"))->Fill(mTlmet[0],mTlmet[1],weight);
    else ((TH2 *)getHist(substream+"_mT_corr"))->Fill(mTlmet[1],mTlmet[0],weight);
    getHist(substream+"_mT_individualsum")->Fill(mTlmet[0]+mTlmet[1],weight);
    getHist(substream+"_dilepton_mass")->Fill(dileptonP.M(),weight);
    getHist(substream+"_dilepton_dphi")->Fill( TMath::Abs( lepton1P.DeltaPhi(lepton2P) ) ,weight);
    getHist(substream+"_dilepton_sumpt")->Fill(lepton1P.Pt()+lepton2P.Pt(),weight);
    getHist(substream+"_dilepton_pt")->Fill(dileptonP.Pt(),weight);
    getHist(substream+"_mT")->Fill(transverseMass,weight);

    //if event is MC debug gen level event
    if(!event.isRealData())
      {
	edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
	event.getByType(puInfoH);
	if(puInfoH.isValid())
	  {
	    int npuOOT(0);
	    for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	      {
		TString puCat("");
		if(it->getBunchCrossing() ==0)
		  {
		    getHist(istream+"_ngenpileup")->Fill(it->getPU_NumInteractions(),weight);
		    ((TH2 *)getHist(istream+"_ngoodvertex_ngenpileup"))->Fill(it->getPU_NumInteractions(),selVertices.size(),weight);
		  }
		else npuOOT += it->getPU_NumInteractions();
	      }
	    getHist(istream+"_ngenpileupOOT")->Fill(npuOOT,weight);
	  }
      
	/*
	  cout << "\t Generator level event " << flush;
	  int igenpart(0);
	  for (pat::eventhypothesis::Looper<reco::GenParticle> genpart = h.loopAs<reco::GenParticle>("genparticle"); genpart; ++genpart) 
	  {
	  cout << "\t" << genpart->pdgId() << " -> " << flush;  
	  
	  int igenpartdau(0);
	  char buf[20];
	  sprintf(buf,"gendaughter_%d",igenpart);
	  for(pat::eventhypothesis::Looper<reco::GenParticle> genpartdau = h.loopAs<reco::GenParticle>(buf); genpartdau; ++genpartdau)
	  {
	  cout << genpartdau->pdgId() << " (" << flush;

	  char buf[20];
	  sprintf(buf,"gendaughter_%d_%d",igenpart,igenpartdau);
	  for(pat::eventhypothesis::Looper<reco::GenParticle> genpartgdau = h.loopAs<reco::GenParticle>(buf); genpartgdau; ++genpartgdau)
	  cout << genpartgdau->pdgId() << " " << flush;
	  
	  cout << ") " << flush;
	  igenpartdau++;
	  }
	  igenpart++;
	  }
	  cout << endl;
	*/
      }
    
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
      ((TH1F *)getHist(streams[istream]+"_cutflowlowmass"))->Fill(0.,ctrHandle->value);
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      std::string inpt= std::string(streams[istream])+"Counter";
      iLumi.getByLabel(inpt, streamCtrHandle);
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(1.,streamCtrHandle->value);
      ((TH1F *)getHist(streams[istream]+"_cutflowlowmass"))->Fill(1.,streamCtrHandle->value);
    } 
}


DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 
