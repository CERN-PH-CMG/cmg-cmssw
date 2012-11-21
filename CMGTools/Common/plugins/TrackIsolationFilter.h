#ifndef _CMGTOOLS_COMMON_TRACKISOLATIONFILTER_H_
#define _CMGTOOLS_COMMON_TRACKISOLATIONFILTER_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TrackIsolationFilter : public edm::EDFilter{
 public:

  TrackIsolationFilter(const edm::ParameterSet& ps):
    pfcands_trkiso_tag_(ps.getParameter<edm::InputTag>("pfcands_trkiso")),
    pfcands_pt_tag_(ps.getParameter<edm::InputTag>("pfcands_pt")),
    pfcands_chg_tag_(ps.getParameter<edm::InputTag>("pfcands_chg")),
    pt_cut_(ps.getParameter<double>("pt_cut")),
    relIso_cut_(ps.getParameter<double>("relIso_cut")){
  }

  virtual bool filter(edm::Event&, const edm::EventSetup&);

 private:

  edm::InputTag pfcands_trkiso_tag_;
  edm::InputTag pfcands_pt_tag_;
  edm::InputTag pfcands_chg_tag_;

  double pt_cut_;
  double relIso_cut_;

  
};


#endif /*_CMGTOOLS_COMMON_TRACKISOLATIONFILTER_H_*/
