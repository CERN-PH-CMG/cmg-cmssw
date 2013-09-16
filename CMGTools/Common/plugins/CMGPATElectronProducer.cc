
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
  edm::FileInPath  d0LowPt_;
  edm::FileInPath  d1LowPt_;
  edm::FileInPath  d2LowPt_;
  edm::FileInPath  d0HighPt_;
  edm::FileInPath  d1HighPt_;
  edm::FileInPath  d2HighPt_;

  bool verbose_;

  ElectronIDMVA  fMVA_;
};


CMGPATElectronProducer::CMGPATElectronProducer(const edm::ParameterSet & iConfig) : 
  srcIT_( iConfig.getParameter<edm::InputTag>("src") ),
  redEBRecHitsIT_( iConfig.getParameter<edm::InputTag>("redEBRecHits") ),
  redEERecHitsIT_( iConfig.getParameter<edm::InputTag>("redEERecHits") ),
  d0LowPt_( iConfig.getParameter<edm::FileInPath>("d0LowPt") ),
  d1LowPt_( iConfig.getParameter<edm::FileInPath>("d1LowPt") ),
  d2LowPt_( iConfig.getParameter<edm::FileInPath>("d2LowPt") ),
  d0HighPt_( iConfig.getParameter<edm::FileInPath>("d0HighPt") ),
  d1HighPt_( iConfig.getParameter<edm::FileInPath>("d1HighPt") ),
  d2HighPt_( iConfig.getParameter<edm::FileInPath>("d2HighPt") ),
  
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) )

{
  
  fMVA_.Initialize("BDTG method",
		   d0LowPt_.fullPath().data(),
		   d1LowPt_.fullPath().data(),
		   d2LowPt_.fullPath().data(),
		   d0HighPt_.fullPath().data(),
		   d1HighPt_.fullPath().data(),
		   d2HighPt_.fullPath().data(),
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
