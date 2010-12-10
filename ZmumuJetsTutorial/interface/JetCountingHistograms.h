#ifndef JETCOUNTINGHISTOGRAMS_H_
#define JETCOUNTINGHISTOGRAMS_H_

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/HistogramCreator.h"
#include "DataFormats/Common/interface/View.h"

namespace cmg{
  class JetCountingHistograms : public cmg::HistogramCreator<cmg::PFJet>{
    public:
        JetCountingHistograms(const edm::ParameterSet& ps):
            cmg::HistogramCreator<type>::HistogramCreator(ps){
        }
        virtual void fill(const type& cand);
        virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);
     protected:
        virtual void defineHistograms();
};

}
#endif /*JETCOUNTINGHISTOGRAMS_H_*/
