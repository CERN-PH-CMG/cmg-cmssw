#ifndef HISTOGRAMCREATOR_H_
#define HISTOGRAMCREATOR_H_

#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CMGTools/Common/interface/AnalysisHistograms.h"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace cmg{

  template <class T>
    class HistogramCreator : public AnalysisHistograms{
  public:
    HistogramCreator(const edm::ParameterSet& ps):
      AnalysisHistograms::AnalysisHistograms(),
      labelX_(ps.getParameter<edm::InputTag>("inputCollection"))
        {
          labelY_ = ps.getUntrackedParameter<edm::InputTag>("inputCollectionY",
                                                            edm::InputTag("", "", ""));
          labelZ_ = ps.getUntrackedParameter<edm::InputTag>("inputCollectionZ",
                                                            edm::InputTag("", "", ""));
        }

      virtual ~HistogramCreator(){
      }

      void init(){
        AnalysisHistograms::init( fs_.operator->() );
      }

      // Fill the histograms after getting a collection from the event.
      virtual void fill(const edm::Event& iEvent, const edm::EventSetup&) {
        edm::Handle<view> candsX;
        iEvent.getByLabel(labelX_, candsX);
        if (labelY_.encode() == "") {
          // Only X-collection given.
          for(typename view::const_iterator it = candsX->begin(); it != candsX->end(); ++it) {
            fill(*it, *it, *it);
          }
        } else if (labelZ_.encode() == "") {
          // X- and Y-collections given, no Z-collection.
          edm::Handle<view> candsY;
          iEvent.getByLabel(labelY_, candsY);
          size_t sizeX = candsX->size();
          size_t sizeY = candsY->size();
          size_t sizeMin = std::min(sizeX, sizeY);
          T dummy;
          for (size_t i = 0; i != sizeMin; ++i) {
            fill(candsX->at(i), candsY->at(i), dummy);
          }
        } else {
          // All three input collections given.
          edm::Handle<view> candsY;
          iEvent.getByLabel(labelY_, candsY);
          edm::Handle<view> candsZ;
          iEvent.getByLabel(labelZ_, candsZ);
          size_t sizeX = candsX->size();
          size_t sizeY = candsY->size();
          size_t sizeZ = candsZ->size();
          size_t sizeMin = std::min(std::min(sizeX, sizeY), sizeZ);
          for (size_t i = 0; i != sizeMin; ++i) {
            fill(candsX->at(i), candsY->at(i), candsZ->at(i));
          }
        }
      }

      // Fill on a per-PhysicsObject basis.
      virtual void fill(const T& cand0, const T& cand1, const T& cand2)
      {}

      typedef T type;
      typedef edm::View<T> view;

  protected:

      // Can define any histograms here.
      virtual void defineHistograms() = 0;

      const edm::Service<TFileService> fs_;
      const edm::InputTag labelX_;
      edm::InputTag labelY_;
      edm::InputTag labelZ_;
      const std::string name_;
  };

}

#endif /*HISTOGRAMCREATOR_H_*/
