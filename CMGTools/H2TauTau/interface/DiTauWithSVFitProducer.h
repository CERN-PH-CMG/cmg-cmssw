
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"

///New SVFit
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

#include <sstream>


template< typename T, typename U>
class DiTauWithSVFitProducer : public edm::EDProducer {

typedef pat::CompositeCandidate DiTauObject;

public:
  // will template the producer later
  typedef std::vector< DiTauObject > DiTauCollection;

  explicit DiTauWithSVFitProducer(const edm::ParameterSet & iConfig);
  virtual ~DiTauWithSVFitProducer() {}
  
private:

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// source diobject inputtag
  edm::InputTag diTauSrc_;

  unsigned warningNumbers_;
  bool verbose_;
  int SVFitVersion_;
  std::string fitAlgo_;
};


template< typename T, typename U >
DiTauWithSVFitProducer<T, U>::DiTauWithSVFitProducer(const edm::ParameterSet & iConfig) : 
  diTauSrc_( iConfig.getParameter<edm::InputTag>("diTauSrc") ),
  warningNumbers_(0),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ),
  SVFitVersion_( iConfig.getParameter<int>("SVFitVersion") ),
  fitAlgo_(iConfig.getParameter<std::string>("fitAlgo")) {

  // will produce a collection containing a copy of each di-object in input, 
  // with the SVFit mass set. 
  produces< std::vector< DiTauObject > >();
}


template< typename T, typename U >
void DiTauWithSVFitProducer<T, U>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle< DiTauCollection > diTauH;
  iEvent.getByLabel(diTauSrc_, diTauH);

  if(verbose_ && !diTauH->empty() ) {
    std::cout<<"DiTauWithSVFitProducer"<<std::endl;
    std::cout<<"+++"<<std::endl;
  }
    
  std::string warningMessage; 

  NSVfitStandalone::kDecayType leg1type, leg2type;
  NSVfitStandalone2011::kDecayType leg1type2011, leg2type2011;
  if(typeid(T)==typeid(pat::Tau)) {
      leg1type=NSVfitStandalone::kHadDecay;
      leg1type2011=NSVfitStandalone2011::kHadDecay;
      warningMessage += " - first leg is hadronic tau";
  } else {
      leg1type=NSVfitStandalone::kLepDecay;
      leg1type2011=NSVfitStandalone2011::kLepDecay;
      warningMessage += " - first leg is leptonic tau";
  }
  if(typeid(U)==typeid(pat::Tau)) {
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
    
  typedef std::auto_ptr< DiTauCollection >  OutPtr;
  OutPtr pOut( new DiTauCollection() );

  if(verbose_ && !diTauH->empty()) {
    std::cout<<"Looping on "<<diTauH->size()<<" input di-objects:"<<std::endl;
  }

  for (size_t i=0; i<diTauH->size(); ++i) {
    DiTauObject diTau(diTauH->at(i));
    const reco::MET& met(dynamic_cast<const reco::MET&>(*diTau.daughter(2)));

    const auto& smsig = met.getSignificanceMatrix();

    TMatrixD tmsig(2, 2);
    // tmsig.SetMatrixArray(smsig.Array());
    // Set elements by hand to avoid array gymnastics/assumptions
    tmsig(0,0) = smsig(0,0);
    tmsig(0,1) = smsig(0,1);
    tmsig(1,0) = smsig(1,0);
    tmsig(1,1) = smsig(1,1);

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<diTau<<std::endl;
      std::cout<<"\t\tleg1: "<<diTau.daughter(0)<<std::endl;
      std::cout<<"\t\tleg2: "<<diTau.daughter(1)<<std::endl;
      std::cout<<"\t\tMET = "<<met.et()<<", phi_MET = "<<met.phi()<<std::endl;      
    }

    double massSVFit=0.;
    float det=tmsig.Determinant();
    if(det>1e-8) { 
      if(SVFitVersion_==1) {
        //Note that this works only for di-objects where the tau is the leg1 and mu is leg2
        NSVfitStandalone2011::Vector measuredMET( met.p4().x(), met.p4().y(), 0);
        std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons;
        NSVfitStandalone2011::LorentzVector p1(diTau.daughter(0)->p4());
        NSVfitStandalone2011::LorentzVector p2(diTau.daughter(1)->p4());
        measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(leg2type2011,p2));
        measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(leg1type2011,p1));    
        NSVfitStandaloneAlgorithm2011 algo(measuredTauLeptons,measuredMET, &tmsig, 0);
        algo.maxObjFunctionCalls(5000);
        algo.fit();
        massSVFit = algo.fittedDiTauSystem().mass();
        } else if (SVFitVersion_==2) {
        //Note that this works only for di-objects where the tau is the leg1 and mu is leg2
        std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
        measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(leg2type, diTau.daughter(1)->p4()));
        measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(leg1type, diTau.daughter(0)->p4()));
        NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met.p4().Vect(), tmsig, 0);
        algo.addLogM(false);
        
        if (fitAlgo_ == "VEGAS")
          algo.integrateVEGAS();
        else if (fitAlgo_ == "MC")
          algo.integrateMarkovChain();
        else 
          algo.integrate();
        
        massSVFit = algo.mass();
        // Add more fit results as user floats
        diTau.addUserFloat("massUncert", algo.massUncert());
        diTau.addUserFloat("pt", algo.pt());
        diTau.addUserFloat("ptUncert", algo.ptUncert());
      } else {
        std::cout << " Unrecognized SVFitVersion !!!!!!!!!!!!" << std::endl;    
      }
    }
    // This is now handled via the user floats so we can keep the visible mass
    diTau.addUserFloat("mass", massSVFit);
    // diTau.setMass( massSVFit );

    pOut->push_back( diTau );
    
    if(verbose_) {
      std::cout<<"\tm_vis = "<<diTau.mass()<<", m_svfit = "<<massSVFit<<std::endl;
    }
  }
  
  iEvent.put(pOut); 

  if(verbose_ && !diTauH->empty()) {
    std::cout << "DiTauWithSVFitProducer done" << std::endl;
    std::cout << "***" << std::endl;
  }
}


//#include "FWCore/Framework/interface/MakerMacros.h"

//DEFINE_FWK_MODULE(DiTauWithSVFitProducer);
