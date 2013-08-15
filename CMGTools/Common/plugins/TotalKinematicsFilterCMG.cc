
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/GeneratorInterface/GenFilters/src/TotalKinematicsFilterCMG.cc?view=log
//http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/GeneratorInterface/GenFilters/interface/TotalKinematicsFilterCMG.h?view=log

class TotalKinematicsFilterCMG : public edm::EDFilter {

  public:

    explicit TotalKinematicsFilterCMG(const edm::ParameterSet & iConfig);
    ~TotalKinematicsFilterCMG() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    bool taggingMode_;
    bool debugOn_;
    edm::InputTag src_;
    double tolerance_;
    bool verbose_;

};


TotalKinematicsFilterCMG::TotalKinematicsFilterCMG(const edm::ParameterSet & iConfig) {

  debugOn_     = iConfig.getUntrackedParameter<bool>("debugOn",false);
  taggingMode_ = iConfig.getParameter<bool>("TaggingMode");
  src_         = iConfig.getParameter<edm::InputTag>("src");
  tolerance_   = iConfig.getParameter<double>("tolerance");

  produces<bool>("Result");
}


bool TotalKinematicsFilterCMG::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  float nEcms = 0.;
  unsigned int nInit = 0;

  std::vector<float> p4tot(4,0.);
  unsigned int nPart = 0;

  // Gather information on the reco::GenParticle collection
  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(src_, genParticles );
  
  for (reco::GenParticleCollection::const_iterator iter=genParticles->begin();iter!=genParticles->end();++iter){
    if ( nInit < 3 && (*iter).status() == 3 && (*iter).pdgId() == 2212 ) {
      nInit++;
      nEcms += (*iter).energy();
    }
    if ( (*iter).status() == 1) { 
      nPart++;
      if (debugOn_) {
        std::cout << "Status 1 part # " << std::setw(4) << std::fixed << nPart 
                  << std::setw(14) << std::fixed << (*iter).pdgId() 
                  << std::setw(14) << std::fixed << (*iter).px() 
                  << std::setw(14) << std::fixed << (*iter).py() 
                  << std::setw(14) << std::fixed << (*iter).pz() << std::endl;
      }
      p4tot[0] += (*iter).px();
      p4tot[1] += (*iter).py();
      p4tot[2] += (*iter).pz();
      p4tot[3] += std::sqrt( (*iter).px()*(*iter).px() + 
                           (*iter).py()*(*iter).py() + 
                           (*iter).pz()*(*iter).pz() + 
                           (*iter).mass()*(*iter).mass()) ; 
    }
  }

  if ( debugOn_ ) {
    std::cout << "Initial sqrt(s) = " << nEcms << std::endl;
    for (unsigned int i=0; i<4; i++) {
      std::cout << "p4tot["<<i<<"] = " << p4tot[i] << std::endl;
    }
  }

  bool pass = true;
  if ( std::abs(p4tot[0]) > tolerance_ || std::abs(p4tot[1]) > tolerance_ || std::abs(p4tot[2]) > tolerance_ || std::abs(p4tot[3]-nEcms) > tolerance_ ) 
    { pass = false; }


  bool result = pass;
  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  //if (!result)
  //   std::cout << "Failed TotalKinematicsFilterCMG: "
  // 	     << iEvent.id().run() << " : " << iEvent.id().luminosityBlock() << " : " << iEvent.id().event()
  //  	     << std::endl;

  if(taggingMode_) return true; else return result;  
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TotalKinematicsFilterCMG);
