#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

//
class GammaPlusJetsEventAnalyzer : public edm::EDAnalyzer 
{
public:
  GammaPlusJetsEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:

  std::map<std::string, edm::ParameterSet> objConfig_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
};

using namespace std;

//
GammaPlusJetsEventAnalyzer::GammaPlusJetsEventAnalyzer(const edm::ParameterSet &iConfig)
  : controlHistos_( iConfig.getParameter<std::string>("dtag") )
{
  try{
    
    edm::Service<TFileService> fs;
    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    

    //configuration for objects
    objConfig_["Photons"] = iConfig.getParameter<edm::ParameterSet>("Photons");
    objConfig_["Jets"] = iConfig.getParameter<edm::ParameterSet>("Jets");
    objConfig_["MET"] = iConfig.getParameter<edm::ParameterSet>("MET");
    objConfig_["Trigger"] = iConfig.getParameter<edm::ParameterSet>("Trigger");

    //book histograms

    controlHistos_.addHistogram("cutflow",";Selection step; Events",5,0,5);
    TH1 *h=controlHistos_.getHisto("cutflow");
    h->GetXaxis()->SetBinLabel(1,"Reco");
    h->GetXaxis()->SetBinLabel(2,"#geq 1 vertex");
    h->GetXaxis()->SetBinLabel(3,"#geq 1 photon");

    //photons
    controlHistos_.addHistogram("photonpt",";p_{T} [GeV/c]; Photons",50,0,200);
    controlHistos_.addHistogram("photoneta",";#eta; Photons",100,-3,3);
    controlHistos_.addHistogram("photonr9",";R_{9}; Photons",100,0,1);
    controlHistos_.addHistogram("photonsihih",";#sigma_{i#eta,i#eta}; Photons",100,0,0.3);
    controlHistos_.addHistogram("photonhoe",";h/e; Photons",100,0,1);
    controlHistos_.addHistogram("photontrkiso",";Tracker isolation; Photons",100,0,15);
    controlHistos_.addHistogram("photonecaliso",";ECAL isolation; Photons",100,0,15);
    controlHistos_.addHistogram("photonhcaliso",";HCAL isolation; Photons",100,0,15);

    //jets
    controlHistos_.addHistogram("fassoc",";#beta_{vtx}(Jet); Jets",50,0,1.0);
    controlHistos_.addHistogram("pt",";p_{T} [GeV/c]; Jets",50,0,200);
    controlHistos_.addHistogram("eta",";#eta; Jets",50,-2.5,2.5);
    controlHistos_.addHistogram("mult",";Jet multiplicity; Events",4,0,4);
    h=controlHistos_.getHisto("mult");
    for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
      {
	TString ilabel(""); ilabel+=(ibin-1);
	if(ibin==h->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	controlHistos_.getHisto("mult")->GetXaxis()->SetBinLabel(ibin,ilabel);
      }
    
    //MET
    controlHistos_.addHistogram("met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,500.);

    controlHistos_.initMonitorForStep("photon");
  }
  catch(std::exception &e){
    cout << e.what() << endl;
  }  
}


//
void GammaPlusJetsEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) 
{

  try{
    
    //event summary to be filled
    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();

    //get the weight for the event
    float weight(1.0);
    edm::Handle<float> puWeightHandle;
    event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("puReweight"), puWeightHandle );
    if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());

    //select the vertex
    edm::Handle<reco::VertexCollection> hVtx;
    event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
    std::vector<reco::VertexRef> selVertices = vertex::filter(hVtx,objConfig_["Vertices"]);
    if(selVertices.size()==0) return;
    reco::VertexRef primVertex = selVertices[0]; 
    controlHistos_.fillHisto("cutflow","photon",1.,weight);

    //average energy density
    edm::Handle< double > rho;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"), rho );

    //photon
    edm::Handle<edm::View<reco::Candidate> > hPhoton;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("source"), hPhoton );
    edm::Handle<EcalRecHitCollection> ebrechits;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"), ebrechits );
    EcalClusterLazyTools lazyTool(event,iSetup,
				  objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"),
				  objConfig_["Photons"].getParameter<edm::InputTag>("eerechits"));
    CandidateCollection isolPhotons=photon::filter(hPhoton,lazyTool,ebrechits,objConfig_["Photons"]);
    CandidateWithVertexCollection gammaCandidate;
    for(size_t ipho=0; ipho<isolPhotons.size(); ipho++)
      {
	const pat::Photon *photon = dynamic_cast<const pat::Photon *>( isolPhotons[ipho].get() );
	controlHistos_.fillHisto("photonpt","photon",photon->pt(),weight);
	controlHistos_.fillHisto("photoneta","photon",photon->superCluster()->eta(),weight);
	controlHistos_.fillHisto("photonr9","photon",photon->r9(),weight);
	controlHistos_.fillHisto("photonsihih","photon",photon->sigmaIetaIeta(),weight);
	controlHistos_.fillHisto("photonhoe","photon",photon->hadronicOverEm(),weight);
	controlHistos_.fillHisto("photontrkiso","photon",photon->trkSumPtSolidConeDR04(),weight);
	controlHistos_.fillHisto("photonecaliso","photon",photon->ecalRecHitSumEtConeDR04(),weight);
	controlHistos_.fillHisto("photonhcaliso","photon",photon->hcalTowerSumEtConeDR04(),weight);

	if(gammaCandidate.size()==0) gammaCandidate.push_back( CandidateWithVertex(isolPhotons[ipho], primVertex) );
	else
	  {
	    if(gammaCandidate[0].first->pt() > isolPhotons[ipho]->pt()) continue;
	    gammaCandidate[0].first = isolPhotons[ipho];
	  }
      }
    if(gammaCandidate.size()==0) return;
    controlHistos_.fillHisto("cutflow","photon",2.,weight);

    //Jets
    edm::Handle<edm::View<reco::Candidate> > hJet;
    event.getByLabel(objConfig_["Jets"].getParameter<edm::InputTag>("source"), hJet);
    CandidateWithVertexCollection selJets = jet::filter(hJet, gammaCandidate, selVertices, objConfig_["Jets"]);

    //add the met                                                                                                                                                                                        
    edm::Handle<edm::View<reco::Candidate> > hMET;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("source"), hMET);
    reco::CandidatePtr met = hMET->ptrAt(0);

    //save summary
    ev.run=event.id().run();
    ev.lumi=event.luminosityBlock();
    ev.event=event.id().event();
    ev.cat=22;
    ev.nvtx=selVertices.size();
    ev.rho=*rho;
    ev.nparticles=3+selJets.size();

    const pat::Photon *photon = dynamic_cast<const pat::Photon *>( gammaCandidate[0].first.get() );
    ev.px[0] = photon->px();   ev.py[0]=photon->py();     ev.pz[0]=photon->pz();      ev.en[0]=photon->energy();              ev.id[0]=22; 
    ev.info1[0] = photon->superCluster()->eta();          ev.info2[0] = photon->r9(); ev.info3[0]=photon->hadronicOverEm();  ev.info4[0]=photon->sigmaIetaIeta(); 

    ev.px[1] = met->px();    ev.py[1]=met->py();          ev.pz[1]=0;                 ev.en[1]=met->pt();                     ev.id[1]=0;

    ev.px[3]=primVertex->p4().px();  ev.py[3]=primVertex->p4().py();  ev.pz[3]=primVertex->p4().pz();  ev.en[3]=primVertex->p4().energy(); ev.id[3]=500;

    for(size_t ijet=0; ijet<selJets.size(); ijet++)
      {
	int ipart(4+ijet);
	const pat::Jet *jet = dynamic_cast<const pat::Jet *>( selJets[ijet].first.get() );
	ev.px[ipart] = jet->px();  ev.py[ipart]=jet->py();  ev.pz[ipart]=jet->pz(); ev.en[ipart]=jet->energy();  ev.id[ipart]=1;
        const reco::Candidate *genParton = jet->genParton();
        ev.genid[ipart] = genParton ? genParton->pdgId() : -9999;
        ev.info1[ipart]=jet->bDiscriminator("trackCountingHighEffBJetTags");
	ev.info2[ipart]=jet->bDiscriminator("trackCountingHighPurBJetTags");
        ev.info3[ipart]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
        ev.info4[ipart]=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
      }

    summaryHandler_.fillTree();

  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
  }

}

//
void GammaPlusJetsEventAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  //  cout << "[GammaPlusJetsEventAnalyzer::endLuminosityBlock]" << endl;
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  controlHistos_.fillHisto("cutflow","photon",0.,ctrHandle->value);
}


DEFINE_FWK_MODULE(GammaPlusJetsEventAnalyzer);


 
