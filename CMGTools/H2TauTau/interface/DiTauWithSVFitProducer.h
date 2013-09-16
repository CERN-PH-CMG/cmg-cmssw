
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
  // edm::InputTag metsigSrc_;

  unsigned warningNumbers_;
  bool verbose_;
  int SVFitVersion_;
};


template< typename DiTauType >
DiTauWithSVFitProducer<DiTauType>::DiTauWithSVFitProducer(const edm::ParameterSet & iConfig) : 
  diTauSrc_( iConfig.getParameter<edm::InputTag>("diTauSrc") ),
  //   metSrc_( iConfig.getParameter<edm::InputTag>("metSrc") ),
  // metsigSrc_( iConfig.getParameter<edm::InputTag>("metsigSrc") ),
  warningNumbers_(0),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ),
  SVFitVersion_( iConfig.getParameter<int>("SVFitVersion") ) {

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
    
  // get the MET significance
  std::vector< cmg::METSignificance > metsigs;
  // bool vectorMetsigs = false;

  std::string warningMessage; 
  // try {
  //   // In case we do recoil correction, we read a single PFMET significance
  //   edm::Handle< cmg::METSignificance > metsigh;
  //   iEvent.getByLabel(metsigSrc_, metsigh); 
  //   metsigs.push_back( *metsigh );
  //   warningMessage = "DiTauWithSVFitProducer: Recoil correction mode: reading single PFMET significance";
  // }
  // catch(...) {
  //   // In case we do MVAMET, we have a PFMET significance for each MVAMET value,
  //   // and thus for each di-tau
  //   edm::Handle< std::vector<cmg::METSignificance> > metsigh;
  //   iEvent.getByLabel(metsigSrc_, metsigh); 
  //   metsigs = *metsigh;
  //   vectorMetsigs = true;
  //   warningMessage = "DiTauWithSVFitProducer: MVA MET mode: reading vector PFMET significance";
  // }

  NSVfitStandalone::kDecayType leg1type, leg2type;
  NSVfitStandalone2011::kDecayType leg1type2011, leg2type2011;
  if(typeid(typename DiTauType::type1)==typeid(cmg::Tau)) {
      leg1type=NSVfitStandalone::kHadDecay;
      leg1type2011=NSVfitStandalone2011::kHadDecay;
      warningMessage += " - first leg is hadronic tau";
  } else {
      leg1type=NSVfitStandalone::kLepDecay;
      leg1type2011=NSVfitStandalone2011::kLepDecay;
      warningMessage += " - first leg is leptonic tau";
  }
  if(typeid(typename DiTauType::type2)==typeid(cmg::Tau)) {
      leg2type=NSVfitStandalone::kHadDecay;
      leg2type2011=NSVfitStandalone2011::kHadDecay;
      warningMessage += " - second leg is hadronic tau";
  } else {
      leg2type=NSVfitStandalone::kLepDecay;
      leg2type2011=NSVfitStandalone2011::kLepDecay;
      warningMessage += " - second leg is leptonic tau";
  }

  const unsigned maxWarnings = 5;
  if(warningNumbers_<maxWarnings) {
    std::cout<<warningMessage<<std::endl;
    warningNumbers_ += 1;
  }
  
  // if( vectorMetsigs )
  //   assert(metsigs.size()==diTauH->size());
  // else
  //   assert(metsigs.size()==1);
    
  typedef std::auto_ptr< DiTauCollection >  OutPtr;
  OutPtr pOut( new DiTauCollection() );

  if(verbose_ && !diTauH->empty()) {
    std::cout<<"Looping on "<<diTauH->size()<<" input di-objects:"<<std::endl;
  }

  for( unsigned i=0; i<diTauH->size(); ++i) {
    DiTauType diTau(diTauH->at(i));
    const reco::LeafCandidate met = diTau.met();
    //cmg::METSignificance& metsig = metsigs[0];
    const cmg::METSignificance& metsig = diTau.metSig();
    const TMatrixD* tmsig = metsig.significance();
    // if(vectorMetsigs)
    //   metsig = metsigs[i];

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<diTau<<std::endl;
      std::cout<<"\t\tleg1: "<<diTau.leg1()<<std::endl;
      std::cout<<"\t\tleg2: "<<diTau.leg2()<<std::endl;
      std::cout<<"\t\tMET = "<<met.et()<<", phi_MET = "<<met.phi()<<std::endl;      
    }

    double massSVFit=0.;
    float det=tmsig->Determinant();
    if(det>1e-8) { 
      if(SVFitVersion_==1){
	//Note that this works only for di-objects where the tau is the leg1 and mu is leg2
	NSVfitStandalone2011::Vector measuredMET( met.p4().x(), met.p4().y(), 0);
	std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons;
	NSVfitStandalone2011::LorentzVector p1(diTau.leg1().p4());
	NSVfitStandalone2011::LorentzVector p2(diTau.leg2().p4());
	measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(leg2type2011,p2));
	measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(leg1type2011,p1));    
	NSVfitStandaloneAlgorithm2011 algo(measuredTauLeptons,measuredMET, tmsig, 0);
	algo.maxObjFunctionCalls(5000);
	algo.fit();
	massSVFit = algo.fittedDiTauSystem().mass();
      }else if(SVFitVersion_==2){
	//Note that this works only for di-objects where the tau is the leg1 and mu is leg2
	std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
	measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(leg2type, diTau.leg2().p4()));
	measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(leg1type, diTau.leg1().p4()));
	NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met.p4().Vect(), *(tmsig), 0);
	algo.addLogM(false);
	//algo.integrate();
        // algo.integrateMarkovChain();
        algo.integrateVEGAS();
        massSVFit = algo.mass();
        diTau.setMassErrSVFit( algo.massUncert() );
        diTau.setPtSVFit( algo.pt() );
        diTau.setPtErrSVFit( algo.ptUncert() );
      }else {
	std::cout<<" Unrecognized SVFitVersion !!!!!!!!!!!!"<<std::endl;    
      }
    }
    diTau.setMassSVFit( massSVFit );

    pOut->push_back( diTau );

    
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
