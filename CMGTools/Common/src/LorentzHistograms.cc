#include "CMGTools/Common/interface/LorentzHistograms.h"

void cmg::LorentzHistogramsFF::defineHistograms(){

  using namespace std;

  if(!histosInitialized_){
   
    add1DHistogram( "E", ";E (GeV)", 100, 0., 500, file_);
    add1DHistogram( "pT", ";p_{T} (GeV/c)", 100, 0., 500, file_);
    add1DHistogram( "eta", ";#eta", 50, -5, 5, file_);
    add1DHistogram( "phi", ";#phi", 50, -3.2, 3.2, file_);
    add1DHistogram( "Mass", ";Mass (GeV)", 100, 0., 150, file_);
    add1DHistogram( "MT", ";M_{T} (GeV)", 100, 0., 150, file_);
    
    histosInitialized_ = true;
  }
  
}

void cmg::LorentzHistogramsFF::fill(const type& cand){
  fill1DHistogram( "E",  cand.energy() );
  fill1DHistogram( "pT", cand.pt() );
  fill1DHistogram( "eta", cand.eta() );
  fill1DHistogram( "phi", cand.phi() );
  fill1DHistogram( "Mass", cand.mass() );  
  fill1DHistogram( "MT", cand.mt() );  
}
