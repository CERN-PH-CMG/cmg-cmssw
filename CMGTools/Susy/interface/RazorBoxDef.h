#ifndef RAZORBOXDEF_H_
#define RAZORBOXDEF_H_

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>

namespace razor{

    enum Boxes{ EleMu = 0, MuMu, EleEle, Mu, Ele, Had};
    
    class RazorBox : public cmg::AbstractPhysicsObject{
        public:
            RazorBox():
                box_(Had),njet_(0),nbjet_(0){
            }
            RazorBox(const Boxes& b, const unsigned int nj, const unsigned int bj):
                box_(b),njet_(nj),nbjet_(bj){
            }
            
            Boxes box() const{
                return box_;
            }
            const char* name() const{
                switch(box_){
                 case EleMu:
                    return "EleMu";
                 case MuMu:
                    return "MuMu";
                 case EleEle:
                    return "EleEle";
                 case Mu:
                    return "Mu";
                 case Ele:
                    return "Ele";
                 case Had:
                 default:
                    return "Had";
                };
            }
            unsigned int nJets() const{
                return njet_;
            }
            unsigned int nBJets() const{
                return nbjet_;
            }
            
        private:
            Boxes box_;
            unsigned int njet_;
            unsigned int nbjet_;
    };
}

class RazorBoxDef : public edm::EDProducer {

public:
  
  explicit RazorBoxDef(const edm::ParameterSet & iConfig);
  virtual ~RazorBoxDef(){
  }
  
  typedef edm::View<reco::Candidate> CandidateView;
  typedef std::vector<razor::RazorBox> collection;
  unsigned int countThreshold(const edm::Handle<CandidateView>& handle, const double pt) const{
    unsigned int result = 0;
    for(CandidateView::const_iterator it = handle->begin(); it != handle->end(); ++it){
        if(it->pt() > pt) result++;
    }
    return result;
  }
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

private:
  
  edm::InputTag tightElectrons_;
  edm::InputTag looseElectrons_;
  
  edm::InputTag tightMuons_;
  edm::InputTag looseMuons_;
  
  edm::InputTag jets_;
  edm::InputTag bjets_;

};


#endif /*RAZORBOXDEF_H_*/
