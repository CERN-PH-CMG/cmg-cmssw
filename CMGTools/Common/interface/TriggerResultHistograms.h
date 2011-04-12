#ifndef CMGTOOLS_COMMON_TRIGGERRESULTHISTOGRAMS_H_
#define CMGTOOLS_COMMON_TRIGGERRESULTHISTOGRAMS_H_

#include "CMGTools/Common/interface/HistogramCreator.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cmg {

///A class for plotting whether a particular Trigger has passed or failed
class TriggerResultHistograms: public cmg::HistogramCreator<edm::TriggerResults> {
 public:

  TriggerResultHistograms(const edm::ParameterSet& ps);

  virtual ~TriggerResultHistograms(){
  }

  virtual void fill(const type& cand){
  }
  virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup);

 private:

  virtual void defineHistograms(){
    //do nothing as we need the names
  }
  void defineHistograms(const std::vector<std::string>&);
  
  const std::vector<std::string> names_;

};

}

#endif /*CMGTOOLS_COMMON_TRIGGERRESULTHISTOGRAMS_H_*/
