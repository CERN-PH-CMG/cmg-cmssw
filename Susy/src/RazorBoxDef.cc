#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "CMGTools/Susy/interface/RazorBoxDef.h"

RazorBoxDef::RazorBoxDef(const edm::ParameterSet & iConfig) :
    tightElectrons_(iConfig.getParameter<edm::InputTag>("tightElectrons")),
    looseElectrons_(iConfig.getParameter<edm::InputTag>("looseElectrons")),
    tightMuons_(iConfig.getParameter<edm::InputTag>("tightMuons")),
    looseMuons_(iConfig.getParameter<edm::InputTag>("looseMuons")),
    jets_(iConfig.getParameter<edm::InputTag>("jets")),
    bjets_(iConfig.getParameter<edm::InputTag>("bjets")){

        produces<razor::RazorBox>();

}

void RazorBoxDef::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

    edm::Handle<CandidateView> tightElectronsH;
    iEvent.getByLabel(tightElectrons_, tightElectronsH);
    edm::Handle<CandidateView> looseElectronsH;
    iEvent.getByLabel(looseElectrons_, looseElectronsH);
 
    edm::Handle<CandidateView> tightMuonsH;
    iEvent.getByLabel(tightMuons_, tightMuonsH);
    edm::Handle<CandidateView> looseMuonsH;
    iEvent.getByLabel(looseMuons_, looseMuonsH); 

    edm::Handle<CandidateView> jetsH;
    iEvent.getByLabel(jets_, jetsH);
    edm::Handle<CandidateView> bjetsH;
    iEvent.getByLabel(bjets_, bjetsH);
    
    const unsigned int njets = jetsH->size();
    const unsigned int nbjets = bjetsH->size();
   
    const unsigned int nTMu10 = countThreshold(tightMuonsH, 10.0);
    const unsigned int nTEle20 = countThreshold(tightElectronsH, 20.0);
    
    const unsigned int nTMu15 = countThreshold(tightMuonsH, 15.0);
    const unsigned int nLMu10 = countThreshold(looseMuonsH, 10.0);
    
    const unsigned int nLEle10 = countThreshold(looseElectronsH, 10.0);
    
    razor::Boxes box = razor::Had;
    //see figure 2 of SUSY-11-008
    if( (nTMu10 > 0) && (nTEle20 > 0) ){
        box = razor::EleMu;
    }else if( (nLMu10 > 1) && (nTMu15 > 0) ){
        box = razor::MuMu;
    }else if( (nLEle10 > 1) && (nTEle20 > 0) ){
        box = razor::EleEle;
    }else if( nTMu10 > 0 ){
        box = razor::Mu;
    }else if( nTEle20 > 0 ){
        box = razor::Ele;
    }
    
    std::auto_ptr<razor::RazorBox> pOut(new razor::RazorBox(box,njets,nbjets) ); 
    iEvent.put( pOut );

}