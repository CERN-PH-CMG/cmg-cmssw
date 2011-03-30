#ifndef cleaneventanalysis_h
#define cleaneventanalysis_h

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <vector>
#include <map>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
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

#include "TFile.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

#endif


class CleanEventAnalysis  : public edm::BasicAnalyzer {
 public:
  CleanEventAnalysis(edm::ParameterSet const& params, TFileDirectory& fs);
  void beginJob() {};
  void endJob();
  void analyze(const edm::EventBase& event);
  virtual ~CleanEventAnalysis() {};

 private:
  std::map<TString, TObject *> results_;
};

#endif
