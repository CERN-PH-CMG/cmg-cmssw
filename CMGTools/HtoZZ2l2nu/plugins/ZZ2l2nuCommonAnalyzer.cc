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

//
class ZZ2l2nuCommonAnalyzer : public edm::EDAnalyzer 
{
public:
  ZZ2l2nuCommonAnalyzer(const edm::ParameterSet &iConfig);
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
};


using namespace std;

//
ZZ2l2nuCommonAnalyzer::ZZ2l2nuCommonAnalyzer(const edm::ParameterSet &iConfig)
{
  try{
    
    edm::Service<TFileService> fs;
    
    objConfig_["Vertices"] = iConfig.getParameter<edm::ParameterSet>("Vertices");
    
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    
    TString streams[]={"ee","mumu","emu"};
    TString selsteps[]={"Reco","2 leptons","|M-M_{Z}|<15","p_{T}(#mu_{1})>20","p_{T}(#mu_{2})>20","b veto","p_{T}(#mu_{1}#mu_{2})","#slash_{E}_{T,rel}>80"};
    const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
    for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      TString cat=streams[istream];
      TFileDirectory newDir=baseDir.mkdir(cat.Data());
            
      results_[cat+"_cutflow"]=formatPlot( newDir.make<TH1F>(cat+"_cutflow", ";Steps; Events", nselsteps, 0.,nselsteps), 1,1,1,20,0,false,true,1,1,1);
      for(size_t istep=0; istep<nselsteps; istep++)
	{
	  ((TH1F *)results_[cat+"_cutflow"])->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
	}
    }
  }catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
void ZZ2l2nuCommonAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) {


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

    //require that a dilepton has benn selected
    if(selPath==0 or selStep<3) return;
    std::string istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";

    //basic dilepton kinematics
    reco::CandidatePtr lepton1 = evhyp["leg1"];
    TLorentzVector lepton1P(lepton1->px(),lepton1->py(),lepton1->pz(), lepton1->energy());
    reco::CandidatePtr lepton2 = evhyp["leg2"];
    TLorentzVector lepton2P(lepton2->px(),lepton2->py(),lepton2->pz(), lepton2->energy());
    TLorentzVector dileptonP=lepton1P+lepton2P;

    if(fabs(dileptonP.M()-91)>15) return;
    getHist(istream+"_cutflow")->Fill(2,weight);

    if(lepton1P.Pt()<20) return;
    getHist(istream+"_cutflow")->Fill(3,weight);

    if(lepton2P.Pt()<20) return;
    getHist(istream+"_cutflow")->Fill(4,weight);
    
    //count the b-tag multiplicity
    std::vector<reco::CandidatePtr> seljets= evhyp.all("jet");
    int nbjets(0);
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) {
      float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
      if(btag>2.0) nbjets+=1; //loose point
    }
    if(nbjets) return;
    getHist(istream+"_cutflow")->Fill(5,weight);

    //pT(Z)
    if(dileptonP.Pt()<50) return;
    getHist(istream+"_cutflow")->Fill(6,weight);

    //reduced MET
    const pat::MET *themet=evhyp.getAs<pat::MET>("met");
    TLorentzVector metP(themet->px(),themet->py(),0,themet->pt());
    float dphil2met[]={ fabs(metP.DeltaPhi(lepton1P)), fabs(metP.DeltaPhi(lepton2P)) };
    float dphimin=TMath::MinElement(sizeof(dphil2met)/sizeof(float),dphil2met);
    float relMET=metP.Pt();
    if(themet->pt()<80) return;
   getHist(istream+"_cutflow")->Fill(7,weight);

  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
  }

}

//
void ZZ2l2nuCommonAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  //  cout << "[ZZ2l2nuCommonAnalyzer::endLuminosityBlock]" << endl;
  TString streams[]={"ee","mumu","emu"};
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(0.,ctrHandle->value);
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      std::string inpt= std::string(streams[istream])+"Counter";
      iLumi.getByLabel(inpt, streamCtrHandle);
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(1.,streamCtrHandle->value);
    } 
}


DEFINE_FWK_MODULE(ZZ2l2nuCommonAnalyzer);


 
