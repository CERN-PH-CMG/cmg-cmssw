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
	
   
    //getters for recommended ID selection
    //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
   
    ///////getters for Loose selection  
    cmg::TriBool isGlobal() const{
      return isGlobal_;
    }
    cmg::TriBool isTracker() const{
      return isTracker_;
    }
    bool isPF() const{
      //return this->sourcePtr()->isPFMuon(); //this is not functional in release < 52X
      assert ((*(this->sourcePtr()))->hasUserFloat("isPFMuon")); //this is normally added by PATPFMuonEmbedder module. see comment above
      return (*(this->sourcePtr()))->userFloat("isPFMuon"); 
    }
    
    ///////getters for Soft selection 
    
    //TMOneStationTight selector
    bool isTMOST() const{
      return (*(this->sourcePtr()))->muonID("TMOneStationTight");
    }
    Int_t trackerLayersWithMeasurement() const{
      return trackerLayersWithMeasurement_;
    }
    Int_t pixelLayersWithMeasurement() const{
      reco::TrackRef track = (*(this->sourcePtr()))->innerTrack();
      if(track.isNonnull() && track.isAvailable()){
	return track->hitPattern().numberOfValidPixelHits();
      } else {
	return UnSet(int);
      }
    }
    Double_t tkNormalizedChi2() const{
      reco::TrackRef track = (*(this->sourcePtr()))->innerTrack();
      if(track.isNonnull() && track.isAvailable()){
	return track->normalizedChi2();
      } else {
	return UnSet(double);
      }
    }

    ///////getters for Tight selection 

    Double_t normalizedChi2() const{
      return globalNormChi2_;
    }
    Int_t numberOfValidMuonHits() const{
      return muonHits_;
    }    
    Int_t numberOfValidPixelHits() const{
      return pixelHits_;
    }
    Int_t numberOfMatchedStations() const{
      return (*(this->sourcePtr()))->numberOfMatchedStations(); 
    }

    //not used in the recommended ID
    Int_t numberOfValidTrackerHits() const{
      return trackerHits_;
    }
    cmg::TriBool muonID() const{
      return muonID_;
    }
    Int_t numberOfMatches() const{
      return nMatches_;
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
    //bool isPFMuon() const {return type_ & reco::Muon::PFMuon; }     //WOULD NOT WORK IN <5.2.X (and is not a member of reco::Candidate)
    //bool isRPCMuon() const {return type_ & reco::Muon::RPCMuon;}

    /// charged hadron vetoes    
    reco::isodeposit::AbsVetos chargedHadronVetos() const;
    /// charged particle vetoes (e, mu, h+-)
    reco::isodeposit::AbsVetos chargedAllVetos() const;
    /// pile-up charged hadron vetoes, used for dbeta corrections
    reco::isodeposit::AbsVetos puChargedHadronVetos() const;
    /// neutral hadron vetoes
    reco::isodeposit::AbsVetos neutralHadronVetos() const;
    /// photon vetoes
    reco::isodeposit::AbsVetos photonVetos() const;


    AlgebraicSymMatrix66 covarianceMatrix() { return covarianceMatrix_; }

	
    friend class cmg::MuonFactory;
	
  private:

    cmg::TriBool isGlobal_;
    cmg::TriBool isTracker_;
    cmg::TriBool muonID_;
    Int_t pixelHits_;
    Int_t trackerHits_;
    Double_t globalNormChi2_;
    Int_t muonHits_;
    Int_t nMatches_;
    Int_t trackerLayersWithMeasurement_;
     
    //used for all the isMuon type flags
    unsigned int type_;

    //Adding Cov Matrix for momentum resolution
    //Different than the track 5D curvilinear one
    AlgebraicSymMatrix66 covarianceMatrix_;
   
  };

}

#endif /*MUON_H_*/
