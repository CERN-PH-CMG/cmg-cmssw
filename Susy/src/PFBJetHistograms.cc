#include "CMGTools/Susy/interface/PFBJetHistograms.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <cmath>

bool cmg::PFBJetHistograms::isB(const reco::Candidate& gen) const{

    const int genId = gen.pdgId();
            
    const int p_h = abs(genId %1000/100);
    const int p_t = abs(genId %10000/1000);

    if( (p_h == 5) || (p_t ==5) ){
        return true;
    }
    
    for(unsigned int i = 0; i < gen.numberOfMothers(); i++){
        //recurse up the chain of mothers
        if(isB( *(gen.mother(i)) ) ) return true;
    }
    return false;
}

bool cmg::PFBJetHistograms::isBJet(const reco::GenJet& jet) const{
    
    bool result = false;
    const std::vector<const reco::GenParticle*> genParticles = jet.getGenConstituents();
    for(std::vector<const reco::GenParticle*>::const_iterator it = genParticles.begin();
        it != genParticles.end(); ++it){
            
            if(isB( *(*it) ) ){
                result = true;
                break;   
            }
    }
    return result;
}

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
    
    //MC Truth histograms
    add1DHistogram("NjetsBTagMC", "Exclusive Numbers of Jets with an MC B", 10, 0., 10, file_);
    add1DHistogram("NjetsNoBTagMC", "Exclusive Numbers of Jets without an MC B", 10, 0., 10, file_);
    add1DHistogram("PartonFlavour", "PartonFlavour", 10, 0., 10, file_);

    //eff histograms
    add2DHistogram("BTagEffMisLoose", "Eff vs Mis-tag (loose) from MC",
        24,0.,1.2,
        24,0.,1.2,
        file_);
    add2DHistogram("BTagEffMisMedium", "Eff vs Mis-tag (medium) from MC",
        24,0.,1.2,
        24,0.,1.2,
        file_);        
    add2DHistogram("BTagEffMisTight", "Eff vs Mis-tag (tight) from MC",
        24,0.,1.2,
        24,0.,1.2,
        file_);
    
    add1DHistogram("BTagEffLoose", "Eff (loose) from MC", 24,0.,1.2,file_);    
    add1DHistogram("BTagEffMedium", "Eff (medium) from MC", 24,0.,1.2,file_);
    add1DHistogram("BTagEffTight", "Eff (tight) from MC", 24,0.,1.2,file_);
    
    add1DHistogram("BTagMisLoose", "MisID (loose) from MC", 24,0.,1.2,file_);    
    add1DHistogram("BTagMisMedium", "MisID (medium) from MC", 24,0.,1.2,file_);
    add1DHistogram("BTagMisTight", "MisID (tight) from MC", 24,0.,1.2,file_);      

    histosInitialized_ = true;
  }
  
}

void cmg::PFBJetHistograms::fill(const edm::Event& iEvent, const edm::EventSetup&){

    edm::Handle<view> cands;
    iEvent.getByLabel(label_,cands);

    //the number of jets
    get1DHistogram("NjetsEx")->Fill(cands->size());
    
    int nBjetsMC = 0;
    int nBjetsLoose = 0;
    int nBjetsMedium = 0;
    int nBjetsTight = 0;
    
    for(view::const_iterator it = cands->begin(); it != cands->end(); ++it){
        
        if(it->sourcePtr()->isAvailable()){
             
            //fill the B tag distribution histograms   
            get1DHistogram("TrackCountingHighEff")->Fill( (*it->sourcePtr())->bDiscriminator("trackCountingHighEffBJetTags"));
            get1DHistogram("TrackCountingHighPur")->Fill( (*it->sourcePtr())->bDiscriminator("trackCountingHighPurBJetTags"));
            get1DHistogram("JetProbability")->Fill( (*it->sourcePtr())->bDiscriminator("jetProbabilityBJetTags"));
            get1DHistogram("JetBProbability")->Fill( (*it->sourcePtr())->bDiscriminator("jetBProbabilityBJetTags"));
            get1DHistogram("SimpleSecondaryVertexHighEff")->Fill( (*it->sourcePtr())->bDiscriminator("simpleSecondaryVertexHighEffBJetTags"));
            get1DHistogram("SimpleSecondaryVertexHighPur")->Fill( (*it->sourcePtr())->bDiscriminator("simpleSecondaryVertexHighPurBJetTags"));
            
            if( (*it->sourcePtr())->genJet() ){
                const bool isB = isBJet( *(*it->sourcePtr())->genJet() );
                if(isB){
                    nBjetsMC++;
                }
            }
            get1DHistogram("PartonFlavour")->Fill( abs( (*it->sourcePtr())->partonFlavour()) );
        }
        
        if(it->getSelection("cuts_btag_loose")) nBjetsLoose++;
        if(it->getSelection("cuts_btag_medium")) nBjetsMedium++;
        if(it->getSelection("cuts_btag_tight")) nBjetsTight++;
        
    }
    get1DHistogram("NjetsBTagMC")->Fill(nBjetsMC);
    get1DHistogram("NjetsNoBTagMC")->Fill(cands->size() - nBjetsMC);
    
    if(cands->size()){
    
        //loose
        const float effLoose = (1.*nBjetsMC) ? nBjetsLoose/(1.*nBjetsMC) : 1;
        get1DHistogram("BTagEffLoose")->Fill(effLoose);
        const float misLoose = (cands->size() - nBjetsMC) ? 1 - ((cands->size() - nBjetsLoose)/(1.*(cands->size() - nBjetsMC))) : 0;
        get1DHistogram("BTagMisLoose")->Fill(misLoose);
        get2DHistogram("BTagEffMisLoose")->Fill(effLoose,misLoose);
    
        //medium
        const float effMedium = (1.*nBjetsMC) ? nBjetsMedium/(1.*nBjetsMC) : 1;
        get1DHistogram("BTagEffMedium")->Fill(effMedium);
        const float misMedium = (cands->size() - nBjetsMC) ? 1 - ((cands->size() - nBjetsMedium)/(1.*(cands->size() - nBjetsMC))) : 0;
        get1DHistogram("BTagMisMedium")->Fill(misMedium);
        get2DHistogram("BTagEffMisMedium")->Fill(effMedium,misMedium);

        //tight
        const float effTight = (1.*nBjetsMC) ? nBjetsTight/(1.*nBjetsMC) : 1;
        get1DHistogram("BTagEffTight")->Fill(effTight);
        const float misTight = (cands->size() - nBjetsMC) ? 1 - ((cands->size() - nBjetsTight)/(1.*(cands->size() - nBjetsMC))) : 0;
        get1DHistogram("BTagMisTight")->Fill(misTight);       
        get2DHistogram("BTagEffMisTight")->Fill(effTight,misTight);
    
    }

}