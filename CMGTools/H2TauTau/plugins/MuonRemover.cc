
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

#include <TRandom.h>
// #include <TMath.h>
#include "Math/VectorUtil.h"

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

  TRandom rand_;
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
  produces< std::vector< Lepton > >("corrected");
  produces< std::vector< Lepton > >("correction");
}


std::pair<LV, LV> ZToWCorrection(const LV& lv1, const LV& lv2) {
  
  // uncomment to deactivate correction
  // return std::make_pair(lv1, lv2);

  LV boson = lv1;
  boson += lv2; 
  double energy = boson.E();

  ROOT::Math::Boost boostToCM(-boson.Px()/energy, -boson.Py()/energy, -boson.Pz()/energy);
  LV lv1cm = boostToCM(lv1);
  LV lv2cm = boostToCM(lv2);
  double mW = 80.4;
  double mZ = 91.2;
  double ratio = mW / mZ;
  lv1cm *= ratio;
  lv2cm *= ratio;

  double newMass = (lv1cm+lv2cm).mass();
  double energyW = sqrt( boson.Px()*boson.Px() + 
			 boson.Py()*boson.Py() + 
			 boson.Pz()*boson.Pz() + newMass*newMass);

  ROOT::Math::Boost boostToLabW(boson.Px()/energyW, boson.Py()/energyW, boson.Pz()/energyW);
  
  ROOT::Math::Boost boostToLab(boson.Px()/energy, boson.Py()/energy, boson.Pz()/energy);
  LV lv1cor = boostToLabW(lv1cm);
  LV lv2cor = boostToLabW(lv2cm);

  return std::make_pair(lv1cor, lv2cor);
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

  // sorting according to the distance to mZ
  std::sort(zbosons.begin(), zbosons.end());
  // in case several Zs are found, take the one closest to the Z mass
  const Boson& theBoson = zbosons[0];
  const LV& li = leptonH->at(theBoson.i_).p4();
  const LV& lj = leptonH->at(theBoson.j_).p4();
  std::pair<LV, LV> corrected = ZToWCorrection(li, lj);
  const LV& lci = corrected.first;
  const LV& lcj = corrected.second;
  LV corrbos = lci + lcj;

  if (verbose_) {
    std::cout<<"boson "<<theBoson.mass()<<", "
	     <<theBoson.pt()<<", "<<theBoson.eta()<<std::endl;
    std::cout<<"\t"
	     <<li.pt()<<", "<<li.eta()<<" | cor: "
	     <<lci.pt()<<", "<<lci.eta()<<std::endl;
    std::cout<<"\t naive "<<li.pt()*80.4 / 91.2<<", "<<li.eta()<<std::endl;
    std::cout<<"\t"
	     <<lj.pt()<<", "<<lj.eta()<<" | cor: "
	     <<lcj.pt()<<", "<<lcj.eta()<<std::endl;
    std::cout<<"\t naive "<<lj.pt()*80.4 / 91.2<<", "<<lj.eta()<<std::endl;
    std::cout<<"corrected boson: "<<corrbos.mass()<<", "
	     <<corrbos.pt()<<", "<<corrbos.eta()<<std::endl;
  }
  // pseudo random choice of the leg to be removed.
  unsigned toRemove = theBoson.i_;
  unsigned toCorrect = theBoson.j_;
  LV lcrm = lci;
  LV lccor = lcj;

  double randNum = rand_.Rndm();
    // if(!removeFirstLeg_) {
  if(randNum<0.5){
    if(verbose_)
      std::cout<<"remove second leg"<<std::endl;
    toRemove = theBoson.j_;
    toCorrect = theBoson.i_;
    lcrm = lcj;
    lccor = lci;
    removeFirstLeg_ = true;
  }
  else {
    if(verbose_)
      std::cout<<"remove first leg"<<std::endl;
    removeFirstLeg_ = false;
  }
  if (verbose_)
    std::cout<<"rem, cor indices "<<toRemove<<" "<<toCorrect<<std::endl;

  OutPtr pOut( new LeptonCollection() );
  OutPtr pOutRm( new LeptonCollection() );
  OutPtr pOutCorrect( new LeptonCollection() );
  OutPtr pOutCorrection( new LeptonCollection() );
  for( unsigned i=0; i<leptonH->size(); ++i) {
    if(verbose_) std::cout<<"2dn lepton loop, "<<i<<" "<<leptonH->at(i).pt()<<std::endl;
    // remove either the first or second leg not to introduce a pT bias. 
    // using a deterministic algorithm for reproducible results
    if(i==toRemove) {
      // will be used to correct the MET and clean up other physics objects
      // need to put a corrected lepton here. 
      // don't mess up the indices... 
      Lepton newlep( leptonH->at(i) );
      newlep.setP4(lcrm);
      if(verbose_) std::cout<<"rem "<<newlep.pt()<<std::endl;
      pOutRm->push_back(newlep);
      // pOut->push_back(newlep); that was a bug...
    }
    else if(i==toCorrect) {
      // this one will be used to correct the MET
      Lepton newlep( leptonH->at(i) );
      newlep.setP4(lccor);
      if(verbose_) std::cout<<"cor "<<newlep.pt()<<std::endl;
      pOutCorrect->push_back(newlep);
      pOut->push_back(newlep);
      Lepton correction( leptonH->at(i) );
      correction.setP4(leptonH->at(i).p4() - lccor);
      pOutCorrection->push_back(correction);
    }
    else {
      pOut->push_back(leptonH->at(i));
    }
  }
  
  iEvent.put(pOut);
  iEvent.put(pOutRm, "removed");
  iEvent.put(pOutCorrect, "corrected");
  iEvent.put(pOutCorrection, "correction");
  
  return true;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MuonRemover);
