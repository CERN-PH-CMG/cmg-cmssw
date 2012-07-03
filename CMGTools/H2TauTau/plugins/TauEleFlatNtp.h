#ifndef CMGH2TAUTAU_TauEleFlatNtp_h
#define CMGH2TAUTAU_TauEleFlatNtp_h

#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/SelectionEfficiency.h"
//#include "CMGTools/H2TauTau/interface/TauRate.h"

#include "CMGTools/H2TauTau/interface/BTagEfficiency.h"
#include "CMGTools/H2TauTau/interface/BTagWeight.h"

#include "CMGTools/Common/interface/RecoilCorrector.h"

#include <TRandom2.h>

#include "CMGTools/H2TauTau/interface/VBFMVA.h"

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

  edm::InputTag diTauTag_;
  edm::InputTag genParticlesTag_;
  edm::InputTag pfJetListTag_;
  edm::InputTag diMuonVetoListTag_;
  edm::Handle< std::vector<cmg::TauEle> > diTauList_;
  std::vector<cmg::TauEle> diTauSelList_;
  const cmg::TauEle * diTauSel_;
  std::vector<const cmg::PFJet * > pfJetList_;
  std::vector<const cmg::PFJet * > pfJetListLC_;
  std::vector<const cmg::PFJet * > pfJetListLepLC_;
  const cmg::PFJet * leadJet_;
  const cmg::PFJet * subleadJet_;



  std::vector<const cmg::PFJet * > pfJetListBTag_;
  std::vector<const cmg::PFJet * > pfJetListBTagLC_;


  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;
  SelectionEfficiency selectionEff_;
  float selectionEffWeight_;

  float embeddedGenWeight_;//for tau embedded samples

  //for MSSM
  float btagWP_;
  std::vector<const cmg::PFJet * > pfJetListBTagWeight_;
  BTagWeight btagWeight_;
  BTagEfficiency btagEff_;
  float btagEffWeight_;

  //generator variables
  float genbosonmass_;
  float genbosonpt_;
  float genbosonphi_;

  int   nditau_;//number of candidates before best candidate selection
  float ditaumass_;
  int   ditaucharge_;
  float ditaueta_;
  float ditaupt_;
  float svfitmass_;

  float taumass_;
  float taupt_;
  float taueta_;
  float tauphi_;
  int   tautruth_;
  float tauehop_;
  float taueop_;
  int   taudecaymode_;
  float taudz_;
  float taudxy_;
  float taux_;
  float tauy_;
  float tauz_;
  int   tauantie_;
  int   tauantiemva_;
  int   tauantimu_;
  int   tauisodisc_;
  int   tauisodiscmva_;
  float tauiso_;
  float tauisomva_;
  float taujetpt_;
  float taujeteta_;

  float mupt_;
  float mueta_;
  float muphi_;
  float muiso_;
  float muisomva_;
  float mudz_;
  float mudxy_;
  float mux_;
  float muy_;
  float muz_;
  int   mutruth_;
  int   mujetmatch_;//0= no match, 1 lead jet, 2=subleading jet //check which jet list is being used !
  float mujetpt_;
  float mujeteta_;
  float mumvaid_;

  float pftransversemass_;
  float pfmetpt_;
  float pfmetphi_;
  float transversemass_;
  float  metsigcov00_;
  float  metsigcov01_;
  float  metsigcov10_;
  float  metsigcov11_;
  double metpt_;//double needed by recoil corrector
  double metphi_;
  
  float  pZeta_;
  float  pZetaVis_;


  int njet_;
  float leadJetPt_;
  float leadJetEta_;
  float leadJetRawFactor_;
  float subleadJetPt_;
  float subleadJetEta_;
  float subleadJetRawFactor_;
  float diJetMass_;
  float diJetPt_;
  float diJetDeltaEta_;
  float diJetEta1Eta2_;
  int   njetingap_;
  int nbjet_;
  float leadBJetBTagProb_;
  float leadBJetPt_;
  float leadBJetEta_;



  float muLCleadJetPt_;//jets where only the muon has been removed
  float muLCleadJetEta_;

  int categoryCh_;//
  int categoryMT_;//
  int categoryIso_;//
  int categorySM_;//SM search 
  int categorySM2012_;//SM search 

private:
  

  int sampleGenEventType_;
  int sampleTruthEventType_;
  int genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu
  int truthEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZOther 11=WToENu, 13=WToMuNu, 15=WToENu, 16=WOther
  float deltaRTruth_;

  const reco::GenParticle * genBoson_;
  const reco::GenParticle * genBosonL1_;
  const reco::GenParticle * genBosonL2_;

  edm::Handle< std::vector<cmg::Electron> > diLeptonVetoList_;

  void fillPFJetListLC(const cmg::TauEle * cand, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);
  void fillPFJetListLepLC(const cmg::TauEle * cand, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);
  bool vetoDiLepton();
  int truthMatchTau();
  int truthMatchEle();


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
    //code from Pietro
//      //electron iso:
//      dBetaFactor = 0.5
//        if dBetaFactor>0 and self.puChargedHadronIso()<0:
//      return -1
//        neutralIso = self.neutralHadronIso()+self.photonIso()       
//        corNeutralIso = neutralIso - dBetaFactor * self.puChargedHadronIso();
//      charged = self.chargedAllIsoWithConeVeto()       
//        abs = (charged + max(corNeutralIso,0)) / self.pt()

// //PG FIXME there's some hardcoded parameters here
//   reco::isodeposit::AbsVetos allChargedVetoesCollection ;
//   float coneSize = 0.01 ;
//   if (input->isEE ()) coneSize = 0.015 ;
//   reco::isodeposit::ConeVeto allChargedVeto (reco::isodeposit::Direction(input->eta (), input->phi ()), coneSize) ;
//   allChargedVetoesCollection.push_back (&allChargedVeto) ;
//   output->chargedAllIsoWithConeVeto_ =
//     (input->isoDeposit(pat::PfChargedAllIso)->depositAndCountWithin(
//       0.4,
//       allChargedVetoesCollection,
//       false ).first);
 
//   //PG try for the photon, to have a working recipe
//   reco::isodeposit::AbsVetos photonVetoesCollection ;
//   coneSize = 0.00 ;
//   if (input->isEE ()) coneSize = 0.08 ;
//   reco::isodeposit::ConeVeto photonVeto (reco::isodeposit::Direction(input->eta (), input->phi ()), coneSize) ;
//   photonVetoesCollection.push_back (&photonVeto) ;

//   float photonsIsoWithTauEleConeVeto = input->isoDeposit(pat::PfGammaIso)->depositAndCountWithin(
//       0.4,
//       photonVetoesCollection,
//       false ).first ;
//   //PG FIXME I cannot commit this!
//   output->photonIso_ = photonsIsoWithTauEleConeVeto ;

    //these are ok in the cmgElectron
    float neutralhad=cand->neutralHadronIso();
    float puchhad=cand->puChargedHadronIso();

    //these two need to be recomputed with proper cone vetos
    float charged=cand->chargedAllIsoWithConeVeto();
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


  TRandom2 randEngine_; 
  double randsigma_;
  
  
  RecoilCorrector corrector_;
  int recoilCorreciton_;
  double recoiliScale_;
  std::string fileCorrectTo_;
  std::string fileZmmData_;
  std::string fileZmmMC_;

  int metType_;
  int runSVFit_;

  float vbfmva_;  
  double vbfvars_[8];
  std::string mvaWeights_ ;
  VBFMVA reader_;


  //function definitions from Matthews mva
  Double_t deltaPhi(Double_t phi1, Double_t phi2){
    Double_t dphi = fabs(phi1 - phi2);
    return dphi <= TMath::Pi() ? dphi : 2*TMath::Pi() - dphi; 
  }
  Double_t massPtEtaPhiM(Double_t pt1, Double_t eta1, Double_t phi1, Double_t m1,
                         Double_t pt2, Double_t eta2, Double_t phi2, Double_t m2)
  {
    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<Double_t> > mom1(pt1, eta1, phi1, m1);
    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<Double_t> > mom2(pt2, eta2, phi2, m2);
    return (mom1+mom2).M();
  }  
  
  //pZeta computation //Code from Josh
  void compZeta(const cmg::Electron * leg1, const cmg::Tau * leg2, double metPx, double metPy, float * pZ, float * pZV){
    //leg1 is the muon and leg2 is the tau

    double leg1x = cos(leg1->phi());
    double leg1y = sin(leg1->phi());
    double leg2x = cos(leg2->phi());
    double leg2y = sin(leg2->phi());
    double zetaX = leg1x + leg2x;
    double zetaY = leg1y + leg2y;
    double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
    if ( zetaR > 0. ) {
      zetaX /= zetaR;
      zetaY /= zetaR;
    }

    double visPx = leg1->px() + leg2->px();
    double visPy = leg1->py() + leg2->py();
    //double pZetaVis = visPx*zetaX + visPy*zetaY;
    *pZV = visPx*zetaX + visPy*zetaY;
    
    double px = visPx + metPx;
    double py = visPy + metPy;
    //double pZeta = px*zetaX + py*zetaY;
    *pZ = px*zetaX + py*zetaY;
  }



  int counterall_;
  int counterev_;
  int countergen_;
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
  int counterditau_;
  int counterbestcand_;
  int countertruth_;
  int counter_;

};


#endif

