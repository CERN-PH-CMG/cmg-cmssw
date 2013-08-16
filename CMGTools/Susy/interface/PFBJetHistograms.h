#ifndef PFBJETHISTOGRAMS_H_
#define PFBJETHISTOGRAMS_H_

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/HistogramCreator.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

namespace cmg{
  class PFBJetHistograms : public cmg::HistogramCreator<cmg::PFJet>{
    public:
        PFBJetHistograms(const edm::ParameterSet& ps):
            cmg::HistogramCreator<type>::HistogramCreator(ps){
        }
        virtual void fill(const type& cand){}
        virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
        
     protected:
        virtual void defineHistograms();
};

}

#endif /*PFBJETHISTOGRAMS_H_*/
