
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <sstream>

#include "DataFormats/JetReco/interface/PFJet.h"

#include <TMatrixD.h>

class METSignificanceProducer : public edm::EDProducer {

public:
  
  explicit METSignificanceProducer(const edm::ParameterSet & iConfig);
  virtual ~METSignificanceProducer() {}
  
private:
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  edm::InputTag pfJetsIT_;
  
  bool verbose_;
};


METSignificanceProducer::METSignificanceProducer(const edm::ParameterSet & iConfig) : 
  pfJetsIT_( iConfig.getParameter<edm::InputTag>("pfJets") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) )
{
  produces< TMatrixD >();
}


void METSignificanceProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<edm::View<reco::PFJet> > pfJetH;
  iEvent.getByLabel(pfJetsIT_, pfJetH);

  std::auto_ptr< TMatrixD > pOut(new TMatrixD );
  iEvent.put( pOut ); 
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(METSignificanceProducer);
