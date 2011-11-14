
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// #include "DataFormats/Candidate/interface/LeafCandidate.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/Math/interface/deltaR.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
// #include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
// #include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

// #include "CMGTools/Common/interface/RecoilCorrector.h"

#include <sstream>

class DiTauWithSVFitProducer : public edm::EDProducer {

public:
  // will template the producer later
  typedef cmg::TauMu DiTauType;
  typedef std::vector< DiTauType > DiTauCollection;

  explicit DiTauWithSVFitProducer(const edm::ParameterSet & iConfig);
  virtual ~DiTauWithSVFitProducer() {}
  
private:
 

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// source diobject inputtag
  edm::InputTag diTauSrc_;

  bool verbose_;
};


DiTauWithSVFitProducer::DiTauWithSVFitProducer(const edm::ParameterSet & iConfig) : 
  diTauSrc_( iConfig.getParameter<edm::InputTag>("diTauSrc") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ) {
  
  // will produce a collection containing a copy of each di-object in input, 
  // with the SVFit mass set. 
  produces< std::vector< DiTauType > >();
}


void DiTauWithSVFitProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  
  if(verbose_) {
    std::cout<<"DiTauWithSVFitProducer"<<std::endl;
    std::cout<<"+++"<<std::endl;
  }

  edm::Handle< DiTauCollection > diTauH;
  iEvent.getByLabel(diTauSrc_, diTauH);
  
  typedef std::auto_ptr< DiTauCollection >  OutPtr;
  OutPtr pOut( new DiTauCollection() );

  if(verbose_) {
    std::cout<<"Looping on "<<diTauH->size()<<"input di-objects:"<<std::endl;
  }

  for( unsigned i=0; i<diTauH->size(); ++i) {
    const DiTauType& diTau = diTauH->at(i);

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<diTau<<std::endl;
      std::cout<<"\t\tleg1: "<<diTau.leg1()<<std::endl;
      std::cout<<"\t\tleg2: "<<diTau.leg2()<<std::endl;
    }

    // here, copy input diTau, and set SVFit mass
    // now setting it to twice the mass

    pOut->push_back( diTau );
    pOut->back().setMassSVFit( 2*diTau.mass() );
  }
  
  iEvent.put( pOut ); 

  if(verbose_) {
    std::cout<<"DiTauWithSVFitProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DiTauWithSVFitProducer);
