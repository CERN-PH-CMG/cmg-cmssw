
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class CMGGenEventTypeFilter : public edm::EDFilter {

public:

  explicit CMGGenEventTypeFilter(const edm::ParameterSet & iConfig);
  ~CMGGenEventTypeFilter() {}

private:

  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

  edm::InputTag genParticles_;
  unsigned int eventType_;
  int charge_;

};


CMGGenEventTypeFilter::CMGGenEventTypeFilter(const edm::ParameterSet & iConfig):
  genParticles_(iConfig.getParameter<edm::InputTag>("GenParticleList")),
  eventType_(iConfig.getUntrackedParameter<unsigned int>("GenEventType",0)),
  charge_(iConfig.getUntrackedParameter<int>("GenEventTypeCharge",0)){
  
}


bool CMGGenEventTypeFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle< std::vector<reco::GenParticle> > genParticles;
  iEvent.getByLabel(genParticles_, genParticles);

  if(eventType_==0)//unset
    throw cms::Exception("CMGGenEventTypeFilter")<<"eventType not set"<<std::endl;

  //a Z --> l l
  if(eventType_==1 || eventType_==3 || eventType_==5 ){

    const reco::GenParticle * Z=0;
    const reco::GenParticle * lp=0;
    const reco::GenParticle * lm=0;
    for (std::vector<reco::GenParticle>::const_iterator gen= genParticles->begin(); gen != genParticles->end(); ++gen) {
      if(abs(gen->pdgId())==23)
	Z=&(*gen);
      
      if(Z){
	if(gen->mother()==Z && gen->pdgId()==(int)(10+eventType_)) lp=&(*gen);
	if(gen->mother()==Z && gen->pdgId()==-(int)(10+eventType_)) lm=&(*gen);
      }
    }    
    if(lp&&lm) return 1;

  }

  //a W--> l nu
  if(eventType_==11 || eventType_==13 || eventType_==15 ){
    const reco::GenParticle * W=0;
    const reco::GenParticle * l=0;
    for (std::vector<reco::GenParticle>::const_iterator gen= genParticles->begin(); gen != genParticles->end(); ++gen) {
      if(abs(gen->pdgId())==24)
	W=&(*gen);
      
      if(W){
	if(gen->mother()==W && gen->pdgId()==(-1)*charge_*(int)(10+eventType_)) l=&(*gen);//pdgId of lepton sign is flipped w.r.t charge
      }
    }    
    if(l) return 1;
  }
  

  //decay mode not found  
  return 0;  
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CMGGenEventTypeFilter);
