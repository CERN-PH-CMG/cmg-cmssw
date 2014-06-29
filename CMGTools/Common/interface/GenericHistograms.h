#ifndef GENERICHISTOGRAMS_H_
#define GENERICHISTOGRAMS_H_

#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CMGTools/Common/interface/HistogramCreator.h"

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "TAxis.h"

namespace cmg {

  template<class T>
  class GenericHistograms : public cmg::HistogramCreator<T> {
    protected:

        typedef StringObjectFunction<T> function;
        struct HistogramAxis {
            public:
                function fn_;
                std::string obs_;
                float low_;
                float high_;
                int nbins_;
                std::string title_;
                HistogramAxis(const std::string& obs,
                              const float low, const float high,
                              const int nbins, const std::string& title):
                  fn_(obs),
                  obs_(obs), low_(low), high_(high),nbins_(nbins), title_(title) {
                }
        };
        typedef std::map<std::string, std::vector<HistogramAxis> > Histograms;

    public:

        GenericHistograms(const edm::ParameterSet& ps):
            cmg::HistogramCreator<T>::HistogramCreator(ps),
            histos_(new Histograms){
                initHistograms(ps.getUntrackedParameter<edm::ParameterSet>("histograms",edm::ParameterSet()));
        }

        virtual void fill(const T& cand, const T& cand1, const T& cand2);
        virtual void fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){
            cmg::HistogramCreator<T>::fill(iEvent,iSetup);
        }

     protected:

        virtual void defineHistograms();
        void initHistograms(const edm::ParameterSet& ps);
        const std::auto_ptr<Histograms> histos_;

};

} // namespace cmg

template<class T>
void cmg::GenericHistograms<T>::initHistograms(const edm::ParameterSet& ps)
{

  std::vector<std::string> parameterNames = ps.getParameterNames();
  for(std::vector<std::string>::const_iterator n = parameterNames.begin();
      n != parameterNames.end(); n++) {
    std::string name = *n;
    std::vector<edm::ParameterSet> axes =
      ps.getUntrackedParameter<std::vector<edm::ParameterSet> >(name);
    std::vector<HistogramAxis> ax;
    for(std::vector<edm::ParameterSet>::const_iterator a = axes.begin(); a != axes.end(); a++) {
      // Parameters for the histogram.
      const std::string var = a->getUntrackedParameter<std::string>("var");
      const double low = a->getUntrackedParameter<double>("low");
      const double high = a->getUntrackedParameter<double>("high");
      const int nbins = a->getUntrackedParameter<int>("nbins");
      const std::string title = a->getUntrackedParameter<std::string>("title", var);
      ax.push_back(HistogramAxis(var, low, high, nbins, title));
    }
    (*histos_)[name] = ax;
  }
}

template<class T>
void cmg::GenericHistograms<T>::defineHistograms()
{

    if(cmg::HistogramCreator<T>::histosInitialized_)
      return;

    for(typename Histograms::const_iterator it = histos_->begin(); it != histos_->end(); it++) {
           const unsigned int size = it->second.size();
           switch(size) {
                case 1:
                    {
                        HistogramAxis axis = it->second.at(0);
                        this->add1DHistogram(it->first,it->first,
                                             axis.nbins_,axis.low_,axis.high_,
                                             cmg::HistogramCreator<T>::fs_.operator->());
                        
                        this->get1DHistogram(it->first)->GetXaxis()->SetTitle(axis.title_.c_str());
                        break;
                    }
                case 2:
                    {
                        HistogramAxis axisX = it->second.at(0);
                        HistogramAxis axisY = it->second.at(1);
                        this->add2DHistogram(it->first,it->first,
                                             axisX.nbins_,axisX.low_,axisX.high_,
                                             axisY.nbins_,axisY.low_,axisY.high_,
                                             cmg::HistogramCreator<T>::fs_.operator->());

                        this->get2DHistogram(it->first)->GetXaxis()->SetTitle(axisX.title_.c_str());
                        this->get2DHistogram(it->first)->GetYaxis()->SetTitle(axisY.title_.c_str());
                        break;
                    }
                case 3:
                    {
                        HistogramAxis axisX = it->second.at(0);
                        HistogramAxis axisY = it->second.at(1);
                        HistogramAxis axisZ = it->second.at(2);

                        this->add3DHistogram(it->first,it->first,
                                             axisX.nbins_,axisX.low_,axisX.high_,
                                             axisY.nbins_,axisY.low_,axisY.high_,
                                             axisZ.nbins_,axisZ.low_,axisZ.high_,
                                             cmg::HistogramCreator<T>::fs_.operator->());

                        this->get3DHistogram(it->first)->GetXaxis()->SetTitle(axisX.title_.c_str());
                        this->get3DHistogram(it->first)->GetYaxis()->SetTitle(axisY.title_.c_str());
                        this->get3DHistogram(it->first)->GetZaxis()->SetTitle(axisZ.title_.c_str());
                        break;
                    }
                default:
                        std::cerr << "Unsupported number of variables for histogram " << it->first << std::endl;
           }
    }
    cmg::HistogramCreator<T>::histosInitialized_ = true;
}

template<class T>
void cmg::GenericHistograms<T>::fill(const T& cand0, const T& cand1, const T& cand2){

  for(typename Histograms::const_iterator it = histos_->begin(); it != histos_->end(); it++) {
    const unsigned int size = it->second.size();
    switch(size) {
    case 1: {
      HistogramAxis axis = it->second.at(0);
      this->fill1DHistogram(it->first, axis.fn_(cand0));
      break;
    }
    case 2: {
      HistogramAxis axisX = it->second.at(0);
      HistogramAxis axisY = it->second.at(1);
      this->fill2DHistogram(it->first, axisX.fn_(cand0), axisY.fn_(cand1));
      break;
    }
    case 3: {
      HistogramAxis axisX = it->second.at(0);
      HistogramAxis axisY = it->second.at(1);
      HistogramAxis axisZ = it->second.at(2);
      this->fill3DHistogram(it->first, axisX.fn_(cand0), axisY.fn_(cand1), axisZ.fn_(cand2));
      break;
    }
    default:
      std::cerr << "Unsupported number of variables for histogram " << it->first << std::endl;
    }
  }
}

#endif /*GENERICHISTOGRAMS_H_*/
