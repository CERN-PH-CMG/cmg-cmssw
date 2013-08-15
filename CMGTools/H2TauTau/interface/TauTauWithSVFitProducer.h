
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"
#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"

///New SVFit
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

#include <sstream>

class TauTauWithSVFitProducer : public edm::EDProducer {

public:
  // will template the producer later
  typedef std::vector< cmg::DiTau > DiTauCollection;

  explicit TauTauWithSVFitProducer(const edm::ParameterSet & iConfig);
  virtual ~TauTauWithSVFitProducer() {}
  
private:
 

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// source diobject inputtag
  edm::InputTag diTauSrc_;
  edm::InputTag metsigSrc_;

  unsigned warningNumbers_;
  bool verbose_;
  int SVFitVersion_;
};


TauTauWithSVFitProducer::TauTauWithSVFitProducer(const edm::ParameterSet & iConfig) : 
  diTauSrc_( iConfig.getParameter<edm::InputTag>("diTauSrc") ),
  metsigSrc_( iConfig.getParameter<edm::InputTag>("metsigSrc") ),
  warningNumbers_(0),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ),
  SVFitVersion_( iConfig.getParameter<int>("SVFitVersion") ) {

  // will produce a collection containing a copy of each di-object in input, 
  // with the SVFit mass set. 
  produces< std::vector< cmg::DiTau > >();
}


void TauTauWithSVFitProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle< DiTauCollection > diTauH;
  iEvent.getByLabel(diTauSrc_, diTauH);

  if(verbose_ && !diTauH->empty() ) {
    std::cout<<"TauTauWithSVFitProducer"<<std::endl;
    std::cout<<"+++"<<std::endl;
  }
    
  // get the MET significance
  std::vector< cmg::METSignificance > metsigs;
  bool vectorMetsigs = false;

  std::string warningMessage; 
  try {
    // In case we do recoil correction, we read a single PFMET significance
    edm::Handle< cmg::METSignificance > metsigh;
    iEvent.getByLabel(metsigSrc_, metsigh); 
    metsigs.push_back( *metsigh );
    warningMessage = "TauTauWithSVFitProducer: Recoil correction mode: reading single PFMET significance";
  }
  catch(...) {
    // In case we do MVAMET, we have a PFMET significance for each MVAMET value,
    // and thus for each di-tau
    edm::Handle< std::vector<cmg::METSignificance> > metsigh;
    iEvent.getByLabel(metsigSrc_, metsigh); 
    metsigs = *metsigh;
    vectorMetsigs = true;
    warningMessage = "TauTauWithSVFitProducer: MVA MET mode: reading vector PFMET significance";
  }

  const unsigned maxWarnings = 5;
  if(warningNumbers_<maxWarnings) {
    std::cout<<warningMessage<<std::endl;
    warningNumbers_ += 1;
  }
  
  if( vectorMetsigs )
    assert(metsigs.size()==diTauH->size());
  else
    assert(metsigs.size()==1);
    
  typedef std::auto_ptr< DiTauCollection >  OutPtr;
  OutPtr pOut( new DiTauCollection() );

  if(verbose_ && !diTauH->empty()) {
    std::cout<<"Looping on "<<diTauH->size()<<" input di-objects:"<<std::endl;
  }

  for( unsigned i=0; i<diTauH->size(); ++i) {
    const cmg::DiTau& diTau = diTauH->at(i);
    const reco::LeafCandidate met = diTau.met();
    cmg::METSignificance& metsig = metsigs[0];
    if(vectorMetsigs)
      metsig = metsigs[i];

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<diTau<<std::endl;
      std::cout<<"\t\tleg1: "<<diTau.leg1()<<std::endl;
      std::cout<<"\t\tleg2: "<<diTau.leg2()<<std::endl;
      std::cout<<"\t\tMET = "<<met.et()<<", phi_MET = "<<met.phi()<<std::endl;      
    }

    double massSVFit=0.;

    if(SVFitVersion_==1){
      //Note that this works only for di-objects where the tau is the leg1 and mu is leg2
      NSVfitStandalone2011::Vector measuredMET( met.p4().x(), met.p4().y(), 0);
      std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons;
      NSVfitStandalone2011::LorentzVector p1(diTau.leg1().p4());
      measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kHadDecay,p1));    
      NSVfitStandalone2011::LorentzVector p2(diTau.leg2().p4());
      measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kHadDecay,p2));
      NSVfitStandaloneAlgorithm2011 algo(measuredTauLeptons,measuredMET, metsig.significance(), 0);
      algo.maxObjFunctionCalls(5000);
      algo.fit();
      massSVFit = algo.fittedDiTauSystem().mass();
    }else if(SVFitVersion_==2){
      //Note that this works only for di-objects where the tau is the leg1 and mu is leg2
      std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
      measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, diTau.leg1().p4()));
      measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, diTau.leg2().p4()));
      NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met.p4().Vect(), *(metsig.significance()), 0);
      algo.addLogM(false);
      algo.integrate();
      massSVFit = algo.getMass();
    }else {
      std::cout<<" Unrecognized SVFitVersion !!!!!!!!!!!!"<<std::endl;    
    }




    pOut->push_back( diTau );
    pOut->back().setMassSVFit( massSVFit );
    
    if(verbose_) {
      std::cout<<"\tm_vis = "<<diTau.mass()<<", m_svfit = "<<massSVFit<<std::endl;
    }
  }
  
  iEvent.put( pOut ); 

  if(verbose_ && !diTauH->empty()) {
    std::cout<<"TauTauWithSVFitProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}


//#include "FWCore/Framework/interface/MakerMacros.h"

//DEFINE_FWK_MODULE(TauTauWithSVFitProducer);
