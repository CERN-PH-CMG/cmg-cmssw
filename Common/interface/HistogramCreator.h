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

namespace cmg{

   template <class T>
   class HistogramCreator : public AnalysisHistograms{
    public:
        HistogramCreator(const edm::ParameterSet& ps):
            AnalysisHistograms::AnalysisHistograms(),
            label_(ps.getParameter<edm::InputTag>("inputCollection")){
        }
		
        virtual ~HistogramCreator(){
        }

        void init(){
	       AnalysisHistograms::init( fs_.operator->() );
        }

        // fill the histograms after getting a collection from the event
        virtual void fill(const edm::Event& iEvent, const edm::EventSetup&){
	       edm::Handle<view> cands;
	       iEvent.getByLabel(label_,cands);
	       for(typename view::const_iterator it = cands->begin(); it != cands->end(); ++it){
	           fill(*it);	
	       }
        }
		
        // fill on a per PhysicsObject basis
        virtual void fill(const T& cand){
        }
		
        typedef T type;
        typedef edm::View<T> view;
		
    protected:
	
        // can define any histograms here
        virtual void defineHistograms() = 0;
	
        const edm::Service<TFileService> fs_;
        const edm::InputTag label_;
        const std::string name_;
  };	
	
}


#endif /*HISTOGRAMCREATOR_H_*/
