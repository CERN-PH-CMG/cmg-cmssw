#ifndef _CMGTools_Common_JetHistograms_h_
#define _CMGTools_Common_JetHistograms_h_

/** JetHistograms 
\brief holds jet specific histograms.
 
\author Colin
*/

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/HistogramCreator.h"
#include "CMGTools/Common/interface/JetComponentHistograms.h"

#include <vector>

namespace cmg {

class JetHistograms: public cmg::HistogramCreator<cmg::PFJet> {
 public:

  JetHistograms(const edm::ParameterSet& ps);

  virtual ~JetHistograms();

  virtual void fill(const type& cand, const type& dummy1, const type& dummy2);
  virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    cmg::HistogramCreator<type>::fill(iEvent,iSetup);
  }

 private:

  virtual void defineHistograms();

  typedef std::vector<JetComponentHistograms> CHists;
  typedef CHists::iterator CHI;
  
  /// vector of JetComponentHistograms, for each component (charged hadrons, photons, ...)
  CHists  cHists_;

};

}

#endif
