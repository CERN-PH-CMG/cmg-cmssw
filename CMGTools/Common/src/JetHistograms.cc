#include "CMGTools/Common/interface/JetHistograms.h"

#include <iostream>



cmg::JetHistograms::JetHistograms(const edm::ParameterSet& ps) : 
  cmg::HistogramCreator<type>::HistogramCreator(ps) {

  
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "ChargedHadrons") );
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "Electrons") );
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "Muons") );
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "Photons") );
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "NeutralHadrons") );
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "HFHAD") );
  cHists_.push_back( JetComponentHistograms(fs_.operator->(), "HFEM") );
}


cmg::JetHistograms::~JetHistograms(){ 
}


void cmg::JetHistograms::defineHistograms(){

  using namespace std;
  
  if(!histosInitialized_){
    
   for( CHI chi = cHists_.begin(); chi!=cHists_.end(); ++chi) 
     chi->defineHistograms();
   
   histosInitialized_ = true;
  }
}


void cmg::JetHistograms::fill( const type& jet, const type& dummy1, const type& dummy2 ) {
  for( unsigned i=0; i<cHists_.size(); ++i) {
    cHists_[i].fill( jet.component(i+1), jet);
  }
}
