
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "CMGTools/Common/interface/ElectronIDMVA.h"

#include <sstream>

class CMGPATElectronProducer : public edm::EDProducer {

public:
  
  typedef pat::Electron  ObjectType;
  typedef std::vector< pat::Electron > CollectionType;

  explicit CMGPATElectronProducer(const edm::ParameterSet & iConfig);
  virtual ~CMGPATElectronProducer() {}
  
private:
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  edm::InputTag srcIT_;
  edm::InputTag redEBRecHitsIT_;
  edm::InputTag redEERecHitsIT_; 
  
  //weight files
  std::string  d0LowPt_;
  std::string  d1LowPt_;
  std::string  d2LowPt_;
  std::string  d0HighPt_;
  std::string  d1HighPt_;
  std::string  d2HighPt_;

  bool verbose_;

  ElectronIDMVA  fMVA_;
};


CMGPATElectronProducer::CMGPATElectronProducer(const edm::ParameterSet & iConfig) : 
  srcIT_( iConfig.getParameter<edm::InputTag>("src") ),
  redEBRecHitsIT_( iConfig.getParameter<edm::InputTag>("redEBRecHits") ),
  redEERecHitsIT_( iConfig.getParameter<edm::InputTag>("redEERecHits") ),
  d0LowPt_( iConfig.getParameter<std::string>("d0LowPt") ),
  d1LowPt_( iConfig.getParameter<std::string>("d1LowPt") ),
  d2LowPt_( iConfig.getParameter<std::string>("d2LowPt") ),
  d0HighPt_( iConfig.getParameter<std::string>("d0HighPt") ),
  d1HighPt_( iConfig.getParameter<std::string>("d1HighPt") ),
  d2HighPt_( iConfig.getParameter<std::string>("d2HighPt") ),
  
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) )

{
  
  fMVA_.Initialize("BDTG method",
		   d0LowPt_, 
		   d1LowPt_,
		   d2LowPt_,
		   d0HighPt_, 
		   d1HighPt_,
		   d2HighPt_,
                   ElectronIDMVA::kNoIPInfo);

  produces<CollectionType>();
}


void CMGPATElectronProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {


  using namespace std;

  edm::Handle< CollectionType > objectsH;
  iEvent.getByLabel(srcIT_, objectsH);

  std::auto_ptr<CollectionType> pOut(new CollectionType() ); 
  pOut->reserve( objectsH->size() );

  EcalClusterLazyTools ecalTools(iEvent, iSetup, redEBRecHitsIT_, redEERecHitsIT_ ); 

  typedef CollectionType::const_iterator IC;  
  for (IC icand = objectsH->begin(); icand != objectsH->end(); ++icand) {
    const ObjectType& object = *icand;
    pOut->push_back( object );
    double mva = fMVA_.MVAValue(&object,  ecalTools);
    // cout<<object.superCluster()->energy()<<endl;
    // cout<<object.superCluster()->seed()->energy()<<endl;
    // double mva = 7.;
    // cout<<"mva "<<mva<<endl;
    pOut->back().addUserFloat("ElectronMVA_MIT", mva);
  }

  iEvent.put( pOut ); 
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CMGPATElectronProducer);
