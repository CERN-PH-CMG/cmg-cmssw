#ifndef cleaneventanalysis_h
#define cleaneventanalysis_h

#include <vector>
#include <map>
#include <string>

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/EventHypothesisLooper.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

class CleanEventAnalysis : public edm::BasicAnalyzer {

 public:
  /// default constructor
  CleanEventAnalysis(const edm::ParameterSet& cfg, TFileDirectory& fs);
  /// default destructor
  virtual ~CleanEventAnalysis(){};
  /// everything that needs to be done before the event loop
  void beginJob(){};
  /// everything that needs to be done after the event loop
  void endJob(){};
  /// everything that needs to be done during the event loop
  void analyze(const edm::EventBase& event);
  ///
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);

 private:
  
  inline TH1 *getHist(TString key)
    {
      if(results_.find(key)==results_.end()) return 0;
      return (TH1 *)results_[key];
    }
    

  std::map<TString, TObject *>  results_;
  std::map<std::string, edm::ParameterSet> objConfig_;

};

#endif
