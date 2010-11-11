#ifndef _CMGTools_Common_JetComponentHistograms_h_
#define _CMGTools_Common_JetComponentHistograms_h_

/** JetComponentHistograms 
\brief holds histograms specific to jet components.

Used e.g. to make the famous fraction vs eta plot 
\author Colin
*/

#include "AnalysisDataFormats/CMGTools/interface/PFJetComponent.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/AnalysisHistograms.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include <map>

namespace cmg {

class JetComponentHistograms: public AnalysisHistograms {
 public:
  JetComponentHistograms(TFileDirectory *myDir, const std::string & name="");

  virtual ~JetComponentHistograms();

  void fill(const cmg::PFJetComponent& component, const cmg::PFJet& jet);

  virtual void defineHistograms();

 private:


};

}

#endif
