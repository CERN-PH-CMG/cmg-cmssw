#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "Muon/MuonAnalysisTools/interface/MuonMVAEstimator.h"
#include "Utilities/General/interface/FileInPath.h"



using namespace std;

class PATMuonIsoMVAEmbedder : public edm::EDProducer{
 public:
  PATMuonIsoMVAEmbedder(const edm::ParameterSet& ps);
  virtual ~PATMuonIsoMVAEmbedder();

  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:

  edm::InputTag src_;
  edm::InputTag srcRho_;
  edm::InputTag srcPF_;
  edm::InputTag srcElectrons_;
  edm::InputTag srcMuons_;
  edm::InputTag srcVertices_;
  

  MuonMVAEstimator* mvaEstimator_;

};

PATMuonIsoMVAEmbedder::PATMuonIsoMVAEmbedder(const edm::ParameterSet& ps):
  src_(ps.getParameter<edm::InputTag>("src")), 
  srcRho_(ps.getParameter<edm::InputTag>("srcRho")), 
  srcPF_(ps.getParameter<edm::InputTag>("srcPF")), 
  srcElectrons_(ps.getParameter<edm::InputTag>("srcSelectedElectrons")), 
  srcMuons_(ps.getParameter<edm::InputTag>("srcSelectedMuons")), 
  srcVertices_(ps.getParameter<edm::InputTag>("srcVertices")) 
{

  mvaEstimator_ = new MuonMVAEstimator();

  vector<string> muoniso_weightfiles;

  edm::FileInPath a("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-BarrelPt5To10_V0_BDTG.weights.xml");
  edm::FileInPath b("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-EndcapPt5To10_V0_BDTG.weights.xml");
  edm::FileInPath c("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-BarrelPt10ToInf_V0_BDTG.weights.xml");
  edm::FileInPath d("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-EndcapPt10ToInf_V0_BDTG.weights.xml");
  edm::FileInPath e("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-Tracker_V0_BDTG.weights.xml");
  edm::FileInPath f("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-Global_V0_BDTG.weights.xml");


  muoniso_weightfiles.push_back(a.fullPath());
  muoniso_weightfiles.push_back(b.fullPath());
  muoniso_weightfiles.push_back(c.fullPath());
  muoniso_weightfiles.push_back(d.fullPath());                 
  muoniso_weightfiles.push_back(e.fullPath());
  muoniso_weightfiles.push_back(f.fullPath());
  mvaEstimator_->initialize("MuonIso_BDTG_IsoRings",
			  MuonMVAEstimator::kIsoRings,
			  kTRUE,
			  muoniso_weightfiles);
  mvaEstimator_->SetPrintMVADebug(kFALSE);


  produces<pat::MuonCollection>();

}


PATMuonIsoMVAEmbedder::~PATMuonIsoMVAEmbedder() {

}


void PATMuonIsoMVAEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<pat::MuonCollection> src;
  iEvent.getByLabel(src_,src);
  
  edm::Handle<pat::ElectronCollection> srcElectrons;
  iEvent.getByLabel(srcElectrons_,srcElectrons);

  //CONVERT TO RECO  COLLECTION.ARGHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!!
  reco::GsfElectronCollection selElectrons;
  for(unsigned int i=0;i<srcElectrons->size();++i) {
    const reco::GsfElectron *e = &(srcElectrons->at(i));
    if(e!=0)
      selElectrons.push_back(*e);
  }


  edm::Handle<pat::MuonCollection > srcMuons;
  iEvent.getByLabel(srcMuons_,srcMuons);

  reco::MuonCollection selMuons;
  for(unsigned int i=0;i<srcMuons->size();++i) {
    const reco::Muon *mu = &(srcMuons->at(i));
    if(mu!=0)
      selMuons.push_back(*mu);
  }


  edm::Handle<reco::PFCandidateCollection> srcPF;
  iEvent.getByLabel(srcPF_,srcPF);

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(srcVertices_,vertices);

  edm::Handle<double> rho;
  iEvent.getByLabel(srcRho_,rho);

  std::auto_ptr<pat::MuonCollection> output(new pat::MuonCollection);

  for(unsigned int i=0;i<src->size();++i) {
    pat::Muon muon = src->at(i);
    
    double isomva = mvaEstimator_->mvaValue( muon, vertices->at(0),
					   *srcPF, *rho, 
					   MuonEffectiveArea::kMuEAFall11MC, 
					   selElectrons, selMuons);
    muon.addUserFloat("mvaIsoRings",isomva);

    //    printf("Test=%f\n",muon.userFloat("mvaIsoRings"));
    output->push_back(muon);
  }

  iEvent.put( output );
}


DEFINE_FWK_MODULE(PATMuonIsoMVAEmbedder);

