#ifndef PFMULTIJETHISTOGRAMS_H_
#define PFMULTIJETHISTOGRAMS_H_

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/HistogramCreator.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include <string>

namespace cmg{
  class PFMultiJetHistograms : public cmg::HistogramCreator<cmg::PFJet>{
    public:
        PFMultiJetHistograms(const edm::ParameterSet& ps):
            cmg::HistogramCreator<type>::HistogramCreator(ps){
        }
        virtual void fill(const type& cand){}
        virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
        
     protected:
        virtual void defineHistograms();
        
        std::string indexedHistogram(const std::string& name, unsigned int index) const;
        
        
};

}

#endif /*PFMULTIJETHISTOGRAMS_H_*/
