#include "CMGTools/Common/interface/ElectronFactory.h"

cmg::ElectronFactory::event_ptr cmg::ElectronFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
	
  edm::Handle<pat::ElectronCollection> electronCands;
  cmg::ElectronFactory::event_ptr result(new cmg::ElectronFactory::collection);
  iEvent.getByLabel(electronLabel_,electronCands);

  unsigned index = 0;
  for(pat::ElectronCollection::const_iterator e = electronCands->begin();
      e != electronCands->end(); ++e, ++index){
  			
    const pat::ElectronPtr electronPtr(electronCands, index);
    cmg::Electron elec(electronPtr);
    
    //set the generic quantities first
    leptonFactory_.set(electronPtr,&elec,iEvent,iSetup);
    leptonFactory_.set(electronPtr->gsfTrack(),&elec,iEvent,iSetup);
    
    //now the electron like ones
    set(electronPtr,&elec);
   		
    result->push_back(elec);
  }
  return result;
}

void cmg::ElectronFactory::set(const pat::ElectronPtr& input, cmg::Electron* const output) const{

    output->mva_ = input->mva();
    
    //see http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/SelectorUtils/interface/SimpleCutBasedElectronIDSelectionFunctor.h?view=markup    
    output->sigmaIetaIeta_ = input->sigmaIetaIeta();
    output->deltaPhiSuperClusterTrackAtVtx_ = input->deltaPhiSuperClusterTrackAtVtx();
    output->deltaEtaSuperClusterTrackAtVtx_ = input->deltaEtaSuperClusterTrackAtVtx();
    output->hadronicOverEm_ = input->hadronicOverEm();
    if(input->gsfTrack().isNonnull()){
        output->numberOfHits_  = input->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
    }
    output->convDist_ = input->convDist();
    output->convDcot_ = input->convDcot();
    if(input->core().isNonnull()){
        output->isEcalDriven_ = cmg::toTriBool(input->ecalDriven());
    }
    
}
