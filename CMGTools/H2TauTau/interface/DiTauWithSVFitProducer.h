
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"
#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm.h"


#include <sstream>

template< typename DiTauType>
class DiTauWithSVFitProducer : public edm::EDProducer {

public:
  // will template the producer later
  typedef std::vector< DiTauType > DiTauCollection;

  explicit DiTauWithSVFitProducer(const edm::ParameterSet & iConfig);
  virtual ~DiTauWithSVFitProducer() {}
  
private:
 

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// source diobject inputtag
  edm::InputTag diTauSrc_;
//   edm::InputTag metSrc_;
  edm::InputTag metsigSrc_;

  bool verbose_;
};


template< typename DiTauType >
DiTauWithSVFitProducer<DiTauType>::DiTauWithSVFitProducer(const edm::ParameterSet & iConfig) : 
  diTauSrc_( iConfig.getParameter<edm::InputTag>("diTauSrc") ),
//   metSrc_( iConfig.getParameter<edm::InputTag>("metSrc") ),
  metsigSrc_( iConfig.getParameter<edm::InputTag>("metsigSrc") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ) {
  
  // will produce a collection containing a copy of each di-object in input, 
  // with the SVFit mass set. 
  produces< std::vector< DiTauType > >();
}


template< typename DiTauType >
void DiTauWithSVFitProducer<DiTauType>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle< DiTauCollection > diTauH;
  iEvent.getByLabel(diTauSrc_, diTauH);

  if(verbose_ && !diTauH->empty() ) {
    std::cout<<"DiTauWithSVFitProducer"<<std::endl;
    std::cout<<"+++"<<std::endl;
  }
    
  //get the MET significance
  edm::Handle< cmg::METSignificance > metsig;
  iEvent.getByLabel(metsigSrc_,metsig); 


  typedef std::auto_ptr< DiTauCollection >  OutPtr;
  OutPtr pOut( new DiTauCollection() );

  if(verbose_ && !diTauH->empty()) {
    std::cout<<"Looping on "<<diTauH->size()<<" input di-objects:"<<std::endl;
  }

  for( unsigned i=0; i<diTauH->size(); ++i) {
    const DiTauType& diTau = diTauH->at(i);
    const reco::LeafCandidate met = diTau.met();

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<diTau<<std::endl;
      std::cout<<"\t\tleg1: "<<diTau.leg1()<<std::endl;
      std::cout<<"\t\tleg2: "<<diTau.leg2()<<std::endl;
      std::cout<<"\t\tMET = "<<met.et()<<", phi_MET = "<<met.phi()<<std::endl;      
    }

    
    //Note that this works only for di-objects where the tau is the leg1 and mu is leg2
    NSVfitStandalone::Vector measuredMET( met.p4().x(), met.p4().y(), 0);
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    NSVfitStandalone::LorentzVector p1(diTau.leg1().p4());
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay,p1));    
    NSVfitStandalone::LorentzVector p2(diTau.leg2().p4());
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay,p2));
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons,measuredMET,metsig->significance(),0);
    algo.maxObjFunctionCalls(5000);
    algo.fit();
    double massSVFit = algo.fittedDiTauSystem().mass();

    pOut->push_back( diTau );
    pOut->back().setMassSVFit( massSVFit );
    
    if(verbose_) {
      std::cout<<"\tm_vis = "<<diTau.mass()<<", m_svfit = "<<massSVFit<<std::endl;
    }
  }
  
  iEvent.put( pOut ); 

  if(verbose_ && !diTauH->empty()) {
    std::cout<<"DiTauWithSVFitProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}


//#include "FWCore/Framework/interface/MakerMacros.h"

//DEFINE_FWK_MODULE(DiTauWithSVFitProducer);
