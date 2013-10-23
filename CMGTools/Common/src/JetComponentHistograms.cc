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
    add1DHistogram( "PT", ";p_{T} (GeV/c)", 100, 0., 100, file_);
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

void cmg::JetComponentHistograms::fill( const cmg::PFJetComponent& comp,
				   const cmg::PFJet& jet) {
  
  fill1DHistogram( "R", comp.fraction() );
  fill1DHistogram( "E", comp.energy() );
  fill1DHistogram( "PT", comp.pt());
  fill1DHistogram( "N", comp.number() );
  fillProfile( "RvsEta", jet.eta(), comp.fraction() );

  
  if( fabs(jet.eta()) < 1.5 ) {
    fillProfile( "RvsPt_barrel", jet.pt(), comp.fraction() );
    fillProfile( "RvsPhi_barrel", jet.phi(), comp.fraction() );
  }
  if( fabs(jet.eta()) > 1.5 && fabs(jet.eta())<3 ) {
    fillProfile( "RvsPt_endcaps", jet.pt(), comp.fraction() );
    fillProfile( "RvsPhi_endcaps", jet.phi(), comp.fraction() );
  }
  else if( fabs(jet.eta())>3 ) {
    fillProfile( "RvsPt_HF", jet.pt(), comp.fraction() );
    fillProfile( "RvsPhi_HF", jet.phi(), comp.fraction() );
  }

}
