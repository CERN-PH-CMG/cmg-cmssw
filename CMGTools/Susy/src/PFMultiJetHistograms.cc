#include "CMGTools/Susy/interface/PFMultiJetHistograms.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <cmath>
#include <memory>
#include <sstream>
#include "TString.h"

void cmg::PFMultiJetHistograms::defineHistograms(){

  if(!histosInitialized_){
    
    //jet counting histograms
    add1DHistogram("NjetsEx", "Exclusive Numbers of Jets", 10, 0., 10, file_);

    for(int i = 0; i < 15; i++){
        const std::string pt = indexedHistogram("hPT",i);
        add1DHistogram(pt.c_str(), pt.c_str(), 200, 0., 1000, file_);
        
        const std::string eta = indexedHistogram("hEta",i);
        add1DHistogram(eta.c_str(), eta.c_str(), 62, -3.15, 3.15, file_);
        
        const std::string phi = indexedHistogram("hPhi",i);
        add1DHistogram(phi.c_str(), phi.c_str(), 63, -3.15, 3.15, file_);
        
        const std::string flavour = indexedHistogram("hPartonFlavour",i);
        add1DHistogram(flavour.c_str(), flavour.c_str(), 6, 0, 6, file_);
        
        const std::string tag = indexedHistogram("hBTag",i);
        add1DHistogram(tag.c_str(), tag.c_str(), 10, 0, 10, file_);
        
    }

    
    

    histosInitialized_ = true;
  }
  
}

std::string cmg::PFMultiJetHistograms::indexedHistogram(const std::string& name, unsigned int index) const{
    std::ostringstream os;
    os << name << "_" << index;
    return os.str();
}

void cmg::PFMultiJetHistograms::fill(const edm::Event& iEvent, const edm::EventSetup&){

    edm::Handle<view> cands;
    iEvent.getByLabel(labelX_,cands);

    //the number of jets
    get1DHistogram("NjetsEx")->Fill(cands->size());
    
    int nBjetsMC = 0;
    unsigned int count = 0;
    for(view::const_iterator it = cands->begin(); it != cands->end(); ++it){
        if(count > 15) break;  

        const std::string pt = indexedHistogram("hPT",count);
        get1DHistogram(pt.c_str())->Fill( it->pt() );
        
        const std::string eta = indexedHistogram("hEta",count);
        get1DHistogram(eta.c_str())->Fill( it->eta() );
        
        const std::string phi = indexedHistogram("hPhi",count);
        get1DHistogram(phi.c_str())->Fill( it->phi() );
        
        const std::string flavour = indexedHistogram("hPartonFlavour",count);
        get1DHistogram(flavour.c_str())->Fill( abs(it->partonFlavour()) );
        
        const std::string tag = indexedHistogram("hBTag",count);
        get1DHistogram(tag.c_str())->Fill( it->btag() );
        
        count++;
    }

}
