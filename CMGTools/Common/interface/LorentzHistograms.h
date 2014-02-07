#ifndef CMGTools_Common_LorentzHistograms_h
#define CMGTools_Common_LorentzHistograms_h

/** LorentzHistograms 
\brief holds lorentz vector histograms 

This class can be used for all lorentz vector physics objects, including MET
Specific histograms can be found in other histogram classes.

\author Colin
*/

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CMGTools/Common/interface/HistogramCreator.h"
#include "DataFormats/Common/interface/View.h"

namespace cmg{
  //NS change the file name. add FF everytime we have a name clash with previous system
  class LorentzHistogramsFF : public cmg::HistogramCreator<cmg::AbstractPhysicsObject>{
	public:
 		LorentzHistogramsFF(const edm::ParameterSet& ps):
 			cmg::HistogramCreator<type>::HistogramCreator(ps){
 		}
  		virtual void fill(const type& cand);
  		virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  			cmg::HistogramCreator<type>::fill(iEvent,iSetup);
  		}

	 protected:
  		virtual void defineHistograms();
};

}

#endif
