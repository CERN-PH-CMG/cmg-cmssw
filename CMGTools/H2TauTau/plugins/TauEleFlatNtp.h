#ifndef CMGH2TAUTAU_TauEleFlatNtp_h
#define CMGH2TAUTAU_TauEleFlatNtp_h

#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"


//for electron iso
#include "CMGTools/Common/interface/ElectronFactory.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"



class TauEleFlatNtp : public BaseFlatNtp {


public:
  explicit TauEleFlatNtp(const edm::ParameterSet&);
  ~TauEleFlatNtp();

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

protected:



  edm::Handle< std::vector<cmg::TauEle> > diTauList_;
  std::vector<cmg::TauEle> diTauSelList_;
  const cmg::TauEle * diTauSel_;


  float mumvaid_;
  float tauantiemva_;


private:

  edm::Handle< std::vector<cmg::Electron> > diLeptonVetoList_;
  bool vetoDiLepton();

  //electron id WP95
  bool electronIDWP95(const cmg::Electron * cand){
    if(!cand) return 0;
    ///need to apply missing hits and conv rejection outside
    if((*(cand->sourcePtr()))->isEB()){
      if(cand->sigmaIetaIeta()>=0.01)return 0;
      if(cand->deltaPhiSuperClusterTrackAtVtx()>=0.80)return 0;
      if(cand->deltaEtaSuperClusterTrackAtVtx()>=0.007)return 0;
      if(cand->hadronicOverEm()>=0.15)return 0;
    }else if((*(cand->sourcePtr()))->isEE()){
      if(cand->sigmaIetaIeta()>=0.03)return 0;
      if(cand->deltaPhiSuperClusterTrackAtVtx()>=0.70)return 0;
      if(cand->deltaEtaSuperClusterTrackAtVtx()>=0.01)return 0;
      //if(cand->hadronicOverEm()>=0.07)return 0;//dropped in HTT twiki
    } else return 0;

    return 1;
  }

  //custom electron isolation
  float electronRelIsoDBCorr(const cmg::Electron * cand){
    if(!cand) return 9999.;

    //these are ok in the cmgElectron
    float neutralhad=cand->neutralHadronIso();
    float puchhad=cand->puChargedHadronIso();

    //these two need to be recomputed with proper cone vetos
    float charged=0.;//cand->chargedAllIsoWithConeVeto();
    float photon=cand->photonIso();
    const pat::Electron * input= &(*(*(cand->sourcePtr())));

    //charged particle iso
    reco::isodeposit::AbsVetos allChargedVetoesCollection ;
    float coneSizeCh = 0.01 ;
    if (input->isEE()) coneSizeCh = 0.015 ;
    reco::isodeposit::ConeVeto allChargedVeto (reco::isodeposit::Direction(input->eta (), input->phi ()), coneSizeCh) ;
    allChargedVetoesCollection.push_back (&allChargedVeto) ;
    charged = (input->isoDeposit(pat::PfChargedAllIso)->depositAndCountWithin(0.4,allChargedVetoesCollection,false ).first);
    

    //photon iso
    reco::isodeposit::AbsVetos photonVetoesCollection ;
    float coneSizePhoton = 0.08 ;
    if (input->isEE()) coneSizePhoton = 0.08 ;
    reco::isodeposit::ConeVeto photonVeto (reco::isodeposit::Direction(input->eta (), input->phi ()), coneSizePhoton) ;
    photonVetoesCollection.push_back (&photonVeto) ;    
    photon = input->isoDeposit(pat::PfGammaIso)->depositAndCountWithin(0.4,photonVetoesCollection,false ).first ;
    
    //cout<<" "<<charged<<" "<<photon<<endl;

    if(cand->pt()>0.)
      return (charged + TMath::Max(neutralhad+photon - 0.5*puchhad,0.))/cand->pt();
    else return 9999.;
  }




  int counterev_;
  int counterveto_;
  int counterpresel_;
  int countertaueop_;
  int countertauvtx_;
  int countertaumuveto_;
  int countertaueveto_;
  int countertauiso_;
  int countertaumatch_;
  int countermuvtx_;
  int countermuid_;
  int countermuiso_;
  int countermumatch_;
  int countertruth_;

};


#endif

