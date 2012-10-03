
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

#include <sstream>
#include <algorithm>
#include <functional>

class MuonRemover : public edm::EDFilter {

public:
  // will template the filterr later
  typedef cmg::Muon  Lepton;
  typedef std::vector< Lepton > LeptonCollection;

  explicit MuonRemover(const edm::ParameterSet & iConfig);
  virtual ~MuonRemover() {}
  
private:
 

  bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// source diobject inputtag
  edm::InputTag leptonSrc_;
  bool verbose_;
  bool removeFirstLeg_;
};

typedef math::XYZTLorentzVector  LV; 
class Boson : public  LV {
public:
  Boson( unsigned i, unsigned j, 
	 const LV& p4, int charge): 
    LV(p4), i_(i), j_(j), charge_(charge) {}
  int charge() const {return charge_;}
  unsigned i_; 
  unsigned j_;
  int charge_;
  bool operator<(const Boson& other) const { return fabs(mass()-91.2) > fabs(other.mass()-91.2);}
};



MuonRemover::MuonRemover(const edm::ParameterSet & iConfig) : 
  leptonSrc_( iConfig.getParameter<edm::InputTag>("leptonSrc") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ),
  removeFirstLeg_(true) 
{


  produces< std::vector< Lepton > >();
  produces< std::vector< Lepton > >("removed");
}


bool MuonRemover::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle< LeptonCollection > leptonH;
  iEvent.getByLabel(leptonSrc_, leptonH);

  typedef std::auto_ptr< LeptonCollection >  OutPtr;

  if(verbose_) std::cout<<"LeptonRemover"<<std::endl;


  std::vector<Boson> zbosons; 
  for( unsigned i=0; i<leptonH->size(); ++i) {
    const Lepton& li = leptonH->at(i);
    if(abs(li.pdgId())!=13) continue;
    if(verbose_) std::cout<<"li "<<li.pdgId()<<", "<<li.pt()<<std::endl;
    for( unsigned j=i+1; j<leptonH->size(); ++j) {
      const Lepton& lj = leptonH->at(j);
      if(abs(lj.pdgId())!=13) continue;
      if( li.charge()*lj.charge() > 0 ) continue;
      LV zp4 = li.p4();
      zp4 += lj.p4();

      Boson zboson(i, j, zp4, lj.charge()+li.charge());
      if(verbose_) {
	std::cout<<"\tlj "<<lj.pdgId()<<", "<<lj.pt()<<std::endl;
	std::cout<<"\tZ "<<zboson.mass()<<", "<<zboson.charge()<<std::endl;
      } 
      // we don't want the gamma* component of the DY spectrum, as the W doesn't have any
      if (fabs(zboson.mass() - 91)>5.) continue;
      if(verbose_) std::cout<<"\t\tSELECTED Z"<<std::endl;
      zbosons.push_back( zboson );
    }
  }

  if( zbosons.empty() ) {
    // no z boson, nothing to remove. 
    // just copying out the input collection
    OutPtr pOut( new LeptonCollection(*leptonH) );
    OutPtr pOutRm( new LeptonCollection() );
    iEvent.put(pOut);
    iEvent.put(pOutRm, "removed");
    return false;
  }

  std::sort(zbosons.begin(), zbosons.end());
  OutPtr pOut( new LeptonCollection() );
  OutPtr pOutRm( new LeptonCollection() );
  // in case several Zs are found, take the one closest to the Z mass
  const Boson& theBoson = zbosons[0];
  for( unsigned i=0; i<leptonH->size(); ++i) {
    // remove either the first or second leg not to introduce a pT bias. 
    // using a deterministic algorithm for reproducible results
    if( removeFirstLeg_ && i==theBoson.i_) {
      pOutRm->push_back(leptonH->at(i));
    }
    else if( !removeFirstLeg_ && i==theBoson.j_ ) {
      pOutRm->push_back(leptonH->at(i));
    }
    else  pOut->push_back(leptonH->at(i));
  }
  // at next event, will remove the other leg. 
  if( removeFirstLeg_ ) removeFirstLeg_ = false;
  else removeFirstLeg_ = true;
  
  iEvent.put(pOut);
  iEvent.put(pOutRm, "removed");
  
  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MuonRemover);
