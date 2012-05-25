#include "CMGTools/Common/interface/ElectronFactory.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"


cmg::ElectronFactory::event_ptr cmg::ElectronFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	
  edm::Handle<pat::ElectronCollection> electronCands;
  cmg::ElectronFactory::event_ptr result(new cmg::ElectronFactory::collection);
  iEvent.getByLabel(electronLabel_,electronCands);

  // Get primary vertices 
  edm::Handle< std::vector<reco::Vertex> > primaryVertices;
  bool found = iEvent.getByLabel(primaryVertexLabel_, primaryVertices);
  int nVertices = found ? primaryVertices->size() : 0;

  //Get beamspot
  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel("offlineBeamSpot", bsHandle);
  const reco::BeamSpot &beamspot = *bsHandle.product();

  //Get conversions
  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);
  

  unsigned index = 0;
  for(pat::ElectronCollection::const_iterator e = electronCands->begin();
      e != electronCands->end(); ++e, ++index){
  			
    const pat::ElectronPtr electronPtr(electronCands, index);
    cmg::Electron elec(electronPtr);
    
    //set the generic quantities first
    leptonFactory_.set(electronPtr,&elec,iEvent,iSetup);
    leptonFactory_.set(electronPtr->gsfTrack(),&elec,iEvent,iSetup);
    
    //now the electron like ones
    set(electronPtr,&elec,nVertices, beamspot, hConversions);
   		
    result->push_back(elec);
  }
  return result;
}


void 
cmg::ElectronFactory::set(const pat::ElectronPtr& input, cmg::Electron* const output, int nVertices,
                          const reco::BeamSpot &beamspot, 
                          edm::Handle<reco::ConversionCollection> & hConversions){

  output->dB3D_ = input->dB( pat::Electron::PV3D );
  output->edB3D_ = input->edB( pat::Electron::PV3D );
  
  output->mva_ = input->mva();
  if( input->isElectronIDAvailable("mvaTrigV0") ){
    output->mvaTrigV0_ = input->electronID("mvaTrigV0");
  }else{
    output->mvaTrigV0_ = -999;
  }
  if( input->isElectronIDAvailable("mvaNonTrigV0") ){
    output->mvaNonTrigV0_ = input->electronID("mvaNonTrigV0");
  }else{
    output->mvaNonTrigV0_ = -999;
  }
    
  //see http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/SelectorUtils/interface/SimpleCutBasedElectronIDSelectionFunctor.h?view=markup    
  output->sigmaIetaIeta_ = input->sigmaIetaIeta();
  output->deltaPhiSuperClusterTrackAtVtx_ = input->deltaPhiSuperClusterTrackAtVtx();
  output->deltaEtaSuperClusterTrackAtVtx_ = input->deltaEtaSuperClusterTrackAtVtx();
  output->hadronicOverEm_ = input->hadronicOverEm();
  
  //PG FIXME there's some hardcoded parameters here
  reco::isodeposit::AbsVetos ilPennello ;
  reco::isodeposit::ConeVeto Cinghiale (reco::isodeposit::Direction(input->eta (), input->phi ()), 0.01) ;
  ilPennello.push_back (&Cinghiale) ;
  output->chargedAllIsoWithConeVeto_ = 
    (input->isoDeposit(pat::PfChargedAllIso)->depositAndCountWithin( 
      0.4, 
      ilPennello, 
      false ).first);
  
  if(input->gsfTrack().isNonnull() && input->gsfTrack().isAvailable()){
    output->numberOfHits_  = input->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
  }
  output->convDist_ = input->convDist();
  output->convDcot_ = input->convDcot();
  
//  output->passConversionVeto_ = input->passConversionVeto() ;
  output->passConversionVeto_ = !ConversionTools::hasMatchedConversion(
              *input, hConversions, beamspot.position (), true, 2.0, 1e-06, 0) ;
  
  if(input->core().isNonnull() && input->core().isAvailable()){
    output->isEcalDriven_ = cmg::toTriBool(input->ecalDriven());
  }  
}
