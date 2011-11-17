#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>

namespace cmg{

/**
 * A producer that makes a copy of the inputCollection, vetoing objects that
 * lie within minDeltaR of *any* object in the vetoCollection.
 * 
 * Algorithm is order N^2, so please use reduced collections as input
 */
template <class T>
class DeltaRVetoProducer : public edm::EDProducer {

public:
  
  explicit DeltaRVetoProducer(const edm::ParameterSet & iConfig);
  virtual ~DeltaRVetoProducer() {}
  
private:
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  const edm::InputTag inputCollection_;
  const edm::InputTag vetoCollection_;
  const double minDeltaR_;
  const bool verbose_;
  
  typedef std::vector<T> collection;
};

}

template <class T>
cmg::DeltaRVetoProducer<T>::DeltaRVetoProducer(const edm::ParameterSet & iConfig) : 
  inputCollection_(iConfig.getParameter<edm::InputTag>("inputCollection")),
  vetoCollection_(iConfig.getParameter<edm::InputTag>("vetoCollection")),
  minDeltaR_(iConfig.getParameter<double>("minDeltaR")),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose",false ) ){
  produces<collection>();
}

template <class T>
void cmg::DeltaRVetoProducer<T>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

    edm::Handle<collection> input;
    iEvent.getByLabel(inputCollection_, input);

    edm::Handle<edm::View<reco::Candidate> > vetos;
    iEvent.getByLabel(vetoCollection_, vetos);

    std::auto_ptr<collection> result(new collection); 
    //Order N^2 so lets hope there aren't too many veto objects. Will be fine for any sensible use
    unsigned int key = 0;
    for(typename collection::const_iterator it = input->begin(); it != input->end(); ++it, ++key){
        bool accept = true;
        for(edm::View<reco::Candidate>::const_iterator jt = vetos->begin(); jt != vetos->end(); ++jt){
            const double dR = reco::deltaR(*it,*jt);
            const bool reject = (dR < minDeltaR_);
            if(verbose_){
                std::cout << "DeltaRVetoProducer: pt1/pt2/deltaR " << it->pt() << "/" << jt->pt() << "/" << dR << " Reject: " << reject << std::endl;
            }
            if(reject){
                accept = false;
                break;
            }
        }
        
        if(accept){
            //add to event and set the source pointer for the TopProjector
            T accepted(*it);
            accepted.setSourceCandidatePtr( reco::CandidatePtr( input, key ) );
            result->push_back(accepted);
        }
    }
    iEvent.put(result); 
}


#include "FWCore/Framework/interface/MakerMacros.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
typedef cmg::DeltaRVetoProducer<cmg::BaseJet> DeltaRVetoProducerBaseJet;
typedef cmg::DeltaRVetoProducer<cmg::PFJet> DeltaRVetoProducerPFJet;

DEFINE_FWK_MODULE(DeltaRVetoProducerBaseJet);
DEFINE_FWK_MODULE(DeltaRVetoProducerPFJet);
