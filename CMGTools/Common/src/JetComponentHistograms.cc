#include <iostream>

#include "CMGTools/Common/interface/JetComponentHistograms.h"

using namespace std;

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
cmg::JetComponentHistograms::JetComponentHistograms(TFileDirectory *myDir, const std::string & name){
  AnalysisHistograms::init(myDir,name);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
cmg::JetComponentHistograms::~JetComponentHistograms(){ 
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void cmg::JetComponentHistograms::defineHistograms(){

  using namespace std;

  if(!histosInitialized_){

    ///Here we define 1D histogram. The histograms are assigned to the TDirectory 
    /// pointed by the file argument 

   
    add1DHistogram( "R", ";fraction", 100, 0., 1.1, file_);
    add1DHistogram( "E", ";E (GeV)", 100, 0., 100, file_);
    //add1DHistogram( "PT", ";p_{T} (GeV/c)", 100, 0., 100, file_);
    add1DHistogram( "N", ";number", 21, -0.5, 20.5, file_);

    addProfile( "RvsEta", ";#eta;fraction", 20, -5, 5, file_ );
    addProfile( "RvsPt_barrel", "Barrel;p_{T} (GeV/c);fraction", 50, 0, 200, file_ );
    addProfile( "RvsPt_endcaps", "Endcaps;p_{T} (GeV/c);fraction", 50, 0, 200, file_ );
    addProfile( "RvsPt_HF", "HF;p_{T} (GeV/c);fraction", 50, 0, 200, file_ );
    addProfile( "RvsPhi_barrel", "Barrel;#phi;fraction", 50, -3.2, 3.2, file_ );
    addProfile( "RvsPhi_endcaps", "Endcaps;#phi;fraction", 50, -3.2, 3.2, file_ );
    addProfile( "RvsPhi_HF", "HF;#phi;fraction", 50, -3.2, 3.2, file_ );

    histosInitialized_ = true;
  }
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

void cmg::JetComponentHistograms::fill( const reco::PFCandidate::ParticleType& component,
				   const pat::Jet& jet) {
  
  float fraction, energy, number;
  switch(component) {
      case reco::PFCandidate::h:
          fraction = jet.chargedHadronEnergyFraction();
          energy = jet.chargedHadronEnergy();
          number = jet.chargedHadronMultiplicity();
          break;
      case reco::PFCandidate::e:
          fraction = jet.electronEnergyFraction();
          energy = jet.electronEnergy();
          number = jet.electronMultiplicity();
          break;
      case reco::PFCandidate::mu:
          fraction = jet.muonEnergyFraction();
          energy = jet.muonEnergy();
          number = jet.muonMultiplicity();
          break;
      case reco::PFCandidate::gamma:
          fraction = jet.photonEnergyFraction();
          energy = jet.photonEnergy();
          number = jet.photonMultiplicity();
          break;
      case reco::PFCandidate::h0:
          fraction = jet.neutralHadronEnergyFraction();
          energy = jet.neutralHadronEnergy();
          number = jet.neutralHadronMultiplicity();
          break;
      case reco::PFCandidate::h_HF:
          fraction = jet.HFHadronEnergyFraction();
          energy = jet.HFHadronEnergy();
          number = jet.HFHadronMultiplicity();
          break;
      case reco::PFCandidate::egamma_HF:
          fraction = jet.HFEMEnergyFraction();
          energy = jet.HFEMEnergy();
          number = jet.HFEMMultiplicity();
          break;
      default:
          throw cms::Exception("LogicError", "Filling for a component that is not foreseen\n");
  };
  
  fill1DHistogram( "R", fraction );
  fill1DHistogram( "E", energy );
  //fill1DHistogram( "PT", comp.pt());
  fill1DHistogram( "N", number );
  fillProfile( "RvsEta", jet.eta(), fraction );

  
  if( fabs(jet.eta()) < 1.5 ) {
    fillProfile( "RvsPt_barrel", jet.pt(), fraction );
    fillProfile( "RvsPhi_barrel", jet.phi(), fraction );
  }
  if( fabs(jet.eta()) > 1.5 && fabs(jet.eta())<3 ) {
    fillProfile( "RvsPt_endcaps", jet.pt(), fraction );
    fillProfile( "RvsPhi_endcaps", jet.phi(), fraction );
  }
  else if( fabs(jet.eta())>3 ) {
    fillProfile( "RvsPt_HF", jet.pt(), fraction );
    fillProfile( "RvsPhi_HF", jet.phi(), fraction );
  }

}
