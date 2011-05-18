#include "CMGTools/Common/interface/TauFactory.h"

cmg::TauFactory::event_ptr cmg::TauFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
	
  edm::Handle<pat::TauCollection> tauCands;
  cmg::TauFactory::event_ptr result(new cmg::TauFactory::collection);
  iEvent.getByLabel(tauLabel_,tauCands);

  unsigned index = 0;
  for(pat::TauCollection::const_iterator m = tauCands->begin(); m != tauCands->end(); ++m, ++index){
    pat::TauPtr tauPtr(tauCands, index);
    cmg::Tau tau(tauPtr);
    set(tauPtr,&tau,iEvent,iSetup);
    result->push_back(tau);
  }
  return result;
}

void cmg::TauFactory::set(const pat::TauPtr& input, cmg::Tau* const output, const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
        
  //set the generic quantities first
  leptonFactory_.set(input,output,iEvent,iSetup);

  //  
  output->leadHadrHCalEnergy =  input->leadPFChargedHadrCand()->hcalEnergy();
  output->leadHadrECalEnergy = input->leadPFChargedHadrCand()->ecalEnergy();
  output->leadCandHCalEnergy = input->leadPFCand()->hcalEnergy();
  output->leadChargedHadrMvaEPi = input->leadPFChargedHadrCand()->mva_e_pi();
  output->leadCandMvaEPi = input->leadPFCand()->mva_e_pi();
  output->leadChargedHadrTrkPt = input->leadPFChargedHadrCand()->pt();//  output->leadChargedHadrTrkPt = input->leadPFChargedHadrCand()->trackRef()->pt();
  output->numberChargedHadr =  input->signalPFChargedHadrCands().size();
  output->numberGamma = input->signalPFGammaCands().size() ;

  output->tauIDHPSagainstElectronLoose= input->tauID("againstElectronLoose");
  output->tauIDHPSagainstElectronMedium= input->tauID("againstElectronMedium");
  output->tauIDHPSagainstElectronTight= input->tauID("againstElectronTight");
  output->tauIDHPSagainstMuonLoose= input->tauID("againstMuonLoose");
  output->tauIDHPSagainstMuonTight= input->tauID("againstMuonTight");
  output->tauIDHPSbyLooseIsolation= input->tauID("byLooseIsolation");
  output->tauIDHPSbyMediumIsolation= input->tauID("byMediumIsolation");
  output->tauIDHPSbyTightIsolation= input->tauID("byTightIsolation");
  output->tauIDHPSbyVLooseIsolation= input->tauID("byVLooseIsolation");
  output->tauIDHPSdecayModeFinding= input->tauID("decayModeFinding");

  //other variables
  output->decayMode = input->decayMode();

}


