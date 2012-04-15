#include "CMGTools/Common/interface/ElectronFactory.h"

cmg::ElectronFactory::event_ptr cmg::ElectronFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	
  edm::Handle<pat::ElectronCollection> electronCands;
  cmg::ElectronFactory::event_ptr result(new cmg::ElectronFactory::collection);
  iEvent.getByLabel(electronLabel_,electronCands);

  // Get primary vertices 
  edm::Handle< std::vector<reco::Vertex> > primaryVertices;
  bool found = iEvent.getByLabel(primaryVertexLabel_, primaryVertices);
  int nVertices = found ? primaryVertices->size() : 0;

  unsigned index = 0;
  for(pat::ElectronCollection::const_iterator e = electronCands->begin();
      e != electronCands->end(); ++e, ++index){
  			
    const pat::ElectronPtr electronPtr(electronCands, index);
    cmg::Electron elec(electronPtr);
    
    //set the generic quantities first
    leptonFactory_.set(electronPtr,&elec,iEvent,iSetup);
    leptonFactory_.set(electronPtr->gsfTrack(),&elec,iEvent,iSetup);
    
    //now the electron like ones
    set(electronPtr,&elec,nVertices);
   		
    result->push_back(elec);
  }
  return result;
}


void cmg::ElectronFactory::set(const pat::ElectronPtr& input, cmg::Electron* const output, int nVertices){

  output->dB3D_ = input->dB( pat::Electron::PV3D );
  output->edB3D_ = input->edB( pat::Electron::PV3D );
  
  output->mva_ = input->mva();
  output->mvaTrigV0_ = input->electronID("mvaTrigV0");
  output->mvaNonTrigV0_ = input->electronID("mvaNonTrigV0");
  output->mvaDaniele_ = mvaEstimator_.mva(*input,nVertices);
  output->mvaMIT_ = input->userFloat("ElectronMVA_MIT");
    
  //see http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/SelectorUtils/interface/SimpleCutBasedElectronIDSelectionFunctor.h?view=markup    
  output->sigmaIetaIeta_ = input->sigmaIetaIeta();
  output->deltaPhiSuperClusterTrackAtVtx_ = input->deltaPhiSuperClusterTrackAtVtx();
  output->deltaEtaSuperClusterTrackAtVtx_ = input->deltaEtaSuperClusterTrackAtVtx();
  output->hadronicOverEm_ = input->hadronicOverEm();
  if(input->gsfTrack().isNonnull() && input->gsfTrack().isAvailable()){
    output->numberOfHits_  = input->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
  }
  output->convDist_ = input->convDist();
  output->convDcot_ = input->convDcot();
  if(input->core().isNonnull() && input->core().isAvailable()){
    output->isEcalDriven_ = cmg::toTriBool(input->ecalDriven());
  }  
}
