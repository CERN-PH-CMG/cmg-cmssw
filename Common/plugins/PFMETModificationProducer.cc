
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include <sstream>
#include <algorithm>
#include <functional>

class PFMetModificationProducer : public edm::EDProducer {
 
public:
  // will template the filterr later
  typedef reco::PFMET  MET;
  typedef std::vector< MET > METCollection;
  
  explicit PFMetModificationProducer(const edm::ParameterSet & iConfig);
  virtual ~PFMetModificationProducer() {}
  
private:

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  edm::InputTag metSrc_;
  edm::InputTag candSrc_;
  const std::string operator_;
  bool verbose_;
};



PFMetModificationProducer::PFMetModificationProducer(const edm::ParameterSet & iConfig) : 
  metSrc_( iConfig.getParameter<edm::InputTag>("metSrc") ),
  candSrc_( iConfig.getParameter<edm::InputTag>("candSrc") ),
  operator_( iConfig.getParameter<std::string>("operator")),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) )
{
  produces< std::vector< reco::PFMET > >();
}


void PFMetModificationProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  if(verbose_) std::cout<<"LeptonRemover"<<std::endl;

  typedef reco::LeafCandidate::LorentzVector LorentzVector;

  edm::Handle< METCollection > metH;
  iEvent.getByLabel(metSrc_, metH);

  edm::Handle< edm::View< reco::LeafCandidate > > candH;
  iEvent.getByLabel(candSrc_, candH);

  typedef std::auto_ptr< METCollection >  OutPtr;
  OutPtr pOut( new METCollection() );


  for( unsigned i=0; i<metH->size(); ++i) {
    const MET& met = metH->at(i);

    double sumEx = 0;
    double sumEy = 0;
    double sumEt = 0;
    
    for( unsigned i=0; i<candH->size(); ++i) {
      const reco::LeafCandidate& cand = candH->at(i);
      double et = cand.et();
      sumEt += et;
      sumEx += et*cos(cand.phi());
      sumEy += et*sin(cand.phi());
    }
    
    const LorentzVector missingEt( -sumEx, -sumEy, 0, sqrt( sumEx*sumEx + sumEy*sumEy) );
    const LorentzVector metVector(met.p4());
    
    if(operator_ == "+"){
      /// we add the extra contribution           
      /// reco::PFMET bm(reco::LeafCandidate(metCharge+sumCharge,metVector+missingEt));
      reco::PFMET bm( met.getSpecific(), 
		      met.sumEt() + sumEt, 
		      metVector+missingEt, 
		      met.vertex() );
      pOut->push_back(bm);
    } 
    else if(operator_ == "-"){
      ///default is '-' 
      reco::PFMET bm( met.getSpecific(), 
		      met.sumEt() - sumEt, 
		      metVector - missingEt, 
		      met.vertex() );
      pOut->push_back(bm); 
    }
    else assert(false);
  }

  iEvent.put(pOut);
  
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFMetModificationProducer);
