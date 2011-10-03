#include "CMGTools/Susy/interface/PFBJetHistograms.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <cmath>

void cmg::PFBJetHistograms::defineHistograms(){

  if(!histosInitialized_){
    
    //jet counting histograms
    add1DHistogram("NjetsEx", "Exclusive Numbers of Jets", 10, 0., 10, file_);

    //btag output histograms
    add1DHistogram("TrackCountingHighEff", "TrackCountingHighEff", 50, 0., 25., file_);
    add1DHistogram("TrackCountingHighPur", "TrackCountingHighPur", 50, 0., 10., file_);
    add1DHistogram("JetProbability", "JetProbability", 50, 0., 3., file_);
    add1DHistogram("JetBProbability", "JetBProbability", 50, 0., 10., file_);
    add1DHistogram("SimpleSecondaryVertexHighEff", "SimpleSecondaryVertexHighEff", 50, 0., 6., file_);
    add1DHistogram("SimpleSecondaryVertexHighPur", "SimpleSecondaryVertexHighPur", 50, 0., 6., file_);
    
    //with MC truth requirements
    add1DHistogram("TrackCountingHighEffMCTruthB", "TrackCountingHighEff", 50, 0., 25., file_);
    add1DHistogram("TrackCountingHighPurMCTruthB", "TrackCountingHighPur", 50, 0., 10., file_);
    add1DHistogram("JetProbabilityMCTruthB", "JetProbability", 50, 0., 3., file_);
    add1DHistogram("JetBProbabilityMCTruthB", "JetBProbability", 50, 0., 10., file_);
    add1DHistogram("SimpleSecondaryVertexHighEffMCTruthB", "SimpleSecondaryVertexHighEff", 50, 0., 6., file_);
    add1DHistogram("SimpleSecondaryVertexHighPurMCTruthB", "SimpleSecondaryVertexHighPur", 50, 0., 6., file_);
    
    //MC Truth histograms
    add1DHistogram("NjetsBTagMC", "Exclusive Numbers of Jets with an MC B", 10, 0., 10, file_);
    add1DHistogram("NjetsNoBTagMC", "Exclusive Numbers of Jets without an MC B", 10, 0., 10, file_);
    add1DHistogram("PartonFlavour", "PartonFlavour", 10, 0., 10, file_);

    
    

    histosInitialized_ = true;
  }
  
}

void cmg::PFBJetHistograms::fill(const edm::Event& iEvent, const edm::EventSetup&){

    edm::Handle<view> cands;
    iEvent.getByLabel(labelX_,cands);

    //the number of jets
    get1DHistogram("NjetsEx")->Fill(cands->size());
    
    int nBjetsMC = 0;
    for(view::const_iterator it = cands->begin(); it != cands->end(); ++it){
        
        //fill the B tag distribution histograms   
        get1DHistogram("TrackCountingHighEff")->Fill( it->btag("trackCountingHighEffBJetTags") );
        get1DHistogram("TrackCountingHighPur")->Fill( it->btag("trackCountingHighPurBJetTags") );
        get1DHistogram("JetProbability")->Fill( it->btag("jetProbabilityBJetTags") );
        get1DHistogram("JetBProbability")->Fill( it->btag("jetBProbabilityBJetTags") );
        get1DHistogram("SimpleSecondaryVertexHighEff")->Fill( it->btag("simpleSecondaryVertexHighEffBJetTags") );
        get1DHistogram("SimpleSecondaryVertexHighPur")->Fill( it->btag("simpleSecondaryVertexHighPurBJetTags") );

        if( it->hasSelection("cuts_btag_mcbparton") ){
            const bool isB = it->hasSelection("cuts_btag_mcbparton");
            if(isB){
                nBjetsMC++;
                
                get1DHistogram("TrackCountingHighEffMCTruthB")->Fill( it->btag("trackCountingHighEffBJetTags") );
                get1DHistogram("TrackCountingHighPurMCTruthB")->Fill( it->btag("trackCountingHighPurBJetTags") );
                get1DHistogram("JetProbabilityMCTruthB")->Fill( it->btag("jetProbabilityBJetTags") );
                get1DHistogram("JetBProbabilityMCTruthB")->Fill( it->btag("jetBProbabilityBJetTags") );
                get1DHistogram("SimpleSecondaryVertexHighEffMCTruthB")->Fill( it->btag("simpleSecondaryVertexHighEffBJetTags") );
                get1DHistogram("SimpleSecondaryVertexHighPurMCTruthB")->Fill( it->btag("simpleSecondaryVertexHighPurBJetTags") );                
                
            }
        }

        get1DHistogram("PartonFlavour")->Fill( abs( it->partonFlavour() ) );
        
    }
    get1DHistogram("NjetsBTagMC")->Fill(nBjetsMC);
    get1DHistogram("NjetsNoBTagMC")->Fill(cands->size() - nBjetsMC);


}
