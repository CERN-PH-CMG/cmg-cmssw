#ifndef MUON_H_
#define MUON_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"


#include <vector>

namespace cmg
{

//forward def needed
class Muon;
class MuonFactory;

 class Muon : public cmg::Lepton<pat::MuonPtr>{
 public:
	
      Muon(){
      }
      Muon(const value& m):
	   cmg::Lepton<value>::Lepton(m),
	   isGlobal_(cmg::unset(cmg::TriBool())),
	   isTracker_(cmg::unset(cmg::TriBool())),
	   isPF_(cmg::unset(cmg::TriBool())),
	   muonID_(cmg::unset(cmg::TriBool())),
	   pixelHits_(UnSet(int)),
	   trackerHits_(UnSet(int)),
	   globalNormChi2_(UnSet(double)),
	   muonHits_(UnSet(int)),
	   nMatches_(UnSet(int)),
	   trackerLayersWithMeasurement_(UnSet(int)),
	   type_(0){
      }
      virtual ~Muon(){
	  }
	
     //getters for VBTF selection
     cmg::TriBool isGlobal() const{
       return isGlobal_;
     }
     cmg::TriBool isTracker() const{
       return isTracker_;
     }
     cmg::TriBool muonID() const{
       return muonID_;
     }
     Int_t numberOfValidPixelHits() const{
       return pixelHits_;
     }
     Int_t numberOfValidTrackerHits() const{
       return trackerHits_;
     }
     Double_t normalizedChi2() const{
       return globalNormChi2_;
     }
     Int_t numberOfValidMuonHits() const{
       return muonHits_;
     }
     Int_t numberOfMatches() const{
       return nMatches_;
     }
     Int_t trackerLayersWithMeasurement() const{
       return trackerLayersWithMeasurement_;
     }

     unsigned int type() const{
       return type_;
     }
     // override of method in base class reco::Candidate
     bool isMuon() const { return true; }
     bool isGlobalMuon()     const { return type_ & reco::Muon::GlobalMuon; }
     bool isTrackerMuon()    const { return type_ & reco::Muon::TrackerMuon; }
     bool isStandAloneMuon() const { return type_ & reco::Muon::StandAloneMuon; }
     bool isCaloMuon() const { return type_ & reco::Muon::CaloMuon; }
     bool isPFMuon() const {return isPF_;}
     //bool isRPCMuon() const {return type_ & reco::Muon::RPCMuon;}
	
    friend class cmg::MuonFactory;
	
 private:

     //@see https://twiki.cern.ch/twiki/bin/viewauth/CMS/VbtfWmunuBaselineSelection
     cmg::TriBool isGlobal_;
     cmg::TriBool isTracker_;
     cmg::TriBool isPF_;
     cmg::TriBool muonID_;
     Int_t pixelHits_;
     Int_t trackerHits_;
     //Dxy...
     Double_t globalNormChi2_;
     Int_t muonHits_;
     Int_t nMatches_;
     Int_t trackerLayersWithMeasurement_;
     
     //used for all the isMuon type flags
     unsigned int type_;
     

 };

}

#endif /*MUON_H_*/
