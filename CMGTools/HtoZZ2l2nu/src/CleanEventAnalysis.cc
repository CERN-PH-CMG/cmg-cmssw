#include "CMGTools/HtoZZ2l2nu/interface/CleanEventAnalysis.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

/// default constructor
CleanEventAnalysis::CleanEventAnalysis(const edm::ParameterSet& cfg, TFileDirectory& fs): 
  edm::BasicAnalyzer::BasicAnalyzer(cfg, fs)
{
  try{
    
    objConfig_["Vertices"] = cfg.getParameter<edm::ParameterSet>("Vertices");
    
    TFileDirectory baseDir=fs.mkdir(cfg.getParameter<std::string>("dtag"));    
    TString streams[]={"ee","mumu","emu"};
    TString jetmult[]={"eq0jets","eq1jets","geq2jets"};
    for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      TString cat=streams[istream];
      TFileDirectory newDir=baseDir.mkdir(cat.Data());

      results_[cat+"_cutflow"]=formatPlot( newDir.make<TH1F>(cat+"_cutflow", ";Steps; Events", 10, 0.,10.), 1,1,1,20,0,false,true,1,1,1);

      //dilepton control
      results_[cat+"_dilepton_mass"]=formatPlot( newDir.make<TH1F>(cat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_mass"]=formatPlot( newDir.make<TH1F>(cat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_sumpt"]= formatPlot( newDir.make<TH1F>(cat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_dilepton_pt"] = formatPlot( newDir.make<TH1F>(cat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);

      //vertex control
      results_[cat+"_vertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_vertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngoodvertex"] = formatPlot( newDir.make<TH1F>(cat+"_ngoodvertex", ";Vertices; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);
      
      //jets
      results_[cat+"_jetpt"]    = formatPlot( newDir.make<TH1F>(cat+"_jetpt",";p_{T} [GeV/c]; Jets",100,0,200), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_njets"]    = formatPlot( newDir.make<TH1F>(cat+"_njets",";Jet multiplicity; Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_bmult"]    = formatPlot( newDir.make<TH1F>(cat+"_bmult",";b tag multiplicity (TCHEL); Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins(); ibin++)
	{
	  TString ilabel(""); ilabel+=(ibin-1);
	  if(ibin==((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_njets"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	  ((TH1F *)results_[cat+"_bmult"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}
      results_[cat+"_btags"]             = formatPlot( newDir.make<TH1F>(cat+"_btags",";b tags (TCHE); Jets",100,-0.5,2), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_met"]               = formatPlot( newDir.make<TH1F>(cat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 30,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_metsig"]            = formatPlot( newDir.make<TH1F>(cat+"_metsig", ";#slash{E}_{T} significance; Events", 100,  0.,100.), 1,1,1,20,0,false,true,1,1,1);
      
      //split the analysis according to the jet multiplicity
      for(size_t jstream=0; jstream<sizeof(jetmult)/sizeof(TString); jstream++)
	{
	  TString subcat=cat+jetmult[jstream];
	  results_[subcat+"_dilepton_mass"]     = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_sumpt"]    = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_pt"]       = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_["met2"+subcat+"_mindphi"]    = formatPlot( newDir.make<TH1F>("met2"+subcat+"_mindphi", ";min #Delta #phi(#slash{E}_{T},l) [rad]; Events", 100, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_met"]               = formatPlot( newDir.make<TH1F>(subcat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_metsig"]            = formatPlot( newDir.make<TH1F>(subcat+"_metsig", ";#slash{E}_{T} significance; Events", 100,  0.,100.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton2met_dphi"] = formatPlot( newDir.make<TH1F>(subcat+"dilepton2met_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_relmet"]            = formatPlot( newDir.make<TH1F>(subcat+"_relmet", ";#slash{E}_{T,rel} [GeV/c]; Events", 100,  0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT"]                = formatPlot( newDir.make<TH1F>(subcat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",100,0,1000), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_individual"]     = formatPlot( newDir.make<TH1F>(subcat+"_mT_individual",";Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_corr"]           = formatPlot( newDir.make<TH2F>(subcat+"_mT_corr",";Transverse mass(leading lepton,MET) [GeV/c^{2}];Transverse mass(trailer lepton,MET) [GeV/c^{2}]; Events",50,0,500,50,0,500), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_individualsum"]  = formatPlot( newDir.make<TH1F>(subcat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1,1,1,20,0,false,true,1,1,1);
	}
    }
  }catch(std::exception &e){
  }  
}

/// everything that needs to be done during the event loop
void CleanEventAnalysis::analyze(const edm::EventBase& event)
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

      //require that a dilepton has benn selected
    if(selPath==0 or selStep<3) return;
    std::string istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";
    getHist(istream+"_cutflow")->Fill(2);

    //count the jets in the event
    std::vector<reco::CandidatePtr> seljets= evhyp.all("jet");
    int njets( seljets.size() ), nbjets(0);
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) {
      float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
      if(btag>1.74) nbjets+=1; //loose point
      getHist(istream+"_btags")->Fill(btag);
      getHist(istream+"_jetpt")->Fill(jet->pt());
    }
    getHist(istream+"_njets")->Fill(njets);
    getHist(istream+"_bmult")->Fill(nbjets);
    
    int jetbin=njets;
    if(jetbin>2)jetbin=2;

    //update the selection stream
    TString substream=istream; 
    if(njets==0)      substream = substream+"eq0jets";
    else if(njets==1) substream = substream+"eq1jets";
    else              substream = substream+"geq2jets";

    
    //vertex quantities
    //const reco::Vertex *primVertex = &(*(vertexHandle.product()))[0];
    getHist(istream+"_ngoodvertex")->Fill(selVertices.size());
    //getHist(istream+"_vertex_sumpt")->Fill(vertex::getVertexMomentumFlux(primVertex));
    //getHist(istream+"_vertex_pt")->Fill(primVertex->p4().pt());
    //    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++)
    //{
    //	getHist(istream+"_othervertex_sumpt")->Fill(vertex::getVertexMomentumFlux(vit->get()));
    //	getHist(istream+"_othervertex_pt")->Fill(vit->get()->p4().pt());
    //      }

    //basic dilepton kinematics
    reco::CandidatePtr lepton1 = evhyp["leg1"];
    TLorentzVector lepton1P(lepton1->px(),lepton1->py(),lepton1->pz(), lepton1->energy());
    reco::CandidatePtr lepton2 = evhyp["leg2"];
    TLorentzVector lepton2P(lepton2->px(),lepton2->py(),lepton2->pz(), lepton2->energy());
    TLorentzVector dileptonP=lepton1P+lepton2P;
    getHist(istream+"_dilepton_sumpt")->Fill(lepton1P.Pt()+lepton2P.Pt());
    getHist(istream+"_dilepton_pt")->Fill(dileptonP.Pt());
    getHist(istream+"_dilepton_mass")->Fill(dileptonP.M());
    
    //base met kinematics
    const pat::MET *themet=evhyp.getAs<pat::MET>("met");
    TLorentzVector metP(themet->px(),themet->py(),0,themet->pt());
    float metsig(-1);
    try{
      const reco::MET *origMet = dynamic_cast<const reco::MET *>(themet->originalObject());
      metsig=origMet->significance();
    }catch(std::exception &e){
    }
    getHist(istream+"_met")->Fill(metP.Pt());
    getHist(istream+"_metsig")->Fill(metsig);

    //require MET back-to-back to dilepton
    float dphiZ2met=dileptonP.DeltaPhi(metP);
    if(fabs(dphiZ2met)<TMath::Pi()/2) return;
    getHist(istream+"_cutflow")->Fill(3);
    getHist(istream+"_cutflow")->Fill(4+jetbin);
    getHist(substream+"_met")->Fill(metP.Pt());
    getHist(substream+"_dilepton2met_dphi")->Fill(abs(dphiZ2met));
    getHist(substream+"_metsig")->Fill(metsig);
    
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
    
    //control histograms
    getHist("met2"+substream+"_mindphi")->Fill(dphimin);
    getHist(substream+"_relmet")->Fill(relMET);
    getHist(substream+"_mT_individual")->Fill(mTlmet[0]);
    getHist(substream+"_mT_individual")->Fill(mTlmet[1]);
    if(lepton1P.Pt()>lepton2P.Pt()) getHist(substream+"_mT_corr")->Fill(mTlmet[0],mTlmet[1]);
    else getHist(substream+"_mT_corr")->Fill(mTlmet[1],mTlmet[0]);
    getHist(substream+"_mT_individualsum")->Fill(mTlmet[0]+mTlmet[1]);
    getHist(substream+"_dilepton_mass")->Fill(dileptonP.M());
    getHist(substream+"_dilepton_sumpt")->Fill(lepton1P.Pt()+lepton2P.Pt());
    getHist(substream+"_dilepton_pt")->Fill(dileptonP.Pt());
    getHist(substream+"_mT")->Fill(transverseMass);
    
  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
  }

}

//
void CleanEventAnalysis::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  cout << "[CleanEventAnalysis::endLuminosityBlock]" << endl;
  TString streams[]={"ee","mumu","emu"};
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(0.,ctrHandle->value);
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      iLumi.getByLabel("startCounter", streamCtrHandle);
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(1.,streamCtrHandle->value);
    } 
}

