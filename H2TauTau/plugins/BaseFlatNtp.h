#ifndef CMGH2TAUTAU_BaseFlatNtp_h
#define CMGH2TAUTAU_BaseFlatNtp_h

#include <TRandom2.h>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <TROOT.h>
#include <TNamed.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TF1.h>
#include <TVector3.h>


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"


#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/SelectionEfficiency.h"
#include "CMGTools/Utilities/interface/RecoilCorrector.h"

#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

#include "CMGTools/H2TauTau/interface/BTagEfficiency.h"
#include "CMGTools/H2TauTau/interface/BTagWeight.h"
#include "CMGTools/RootTools/interface/BTagSF.h"


//#include "TMVA/Reader.h"
#include "CMGTools/H2TauTau/interface/VBFMVA.h"
#include "CMGTools/H2TauTau/interface/VBFMVA2012.h"

using namespace std;

class BaseFlatNtp : public edm::EDAnalyzer{


 public:
  explicit BaseFlatNtp(const edm::ParameterSet&);
  ~BaseFlatNtp();

  virtual void beginJob() ;
  //virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;

protected:

  edm::Service<TFileService> * file_;
  TTree * tree_;

  int printSelectionPass_;

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

  int dataPeriodFlag_;
  edm::InputTag verticesListTag_;
  edm::InputTag trigPathsListTag_;
  edm::InputTag trigObjsListTag_;
  int firstRun_; 
  int lastRun_; 
  int dataType_;//0=MC, 1=Data, 2=EmbeddedData, 3=EmbeddedMC

   //configurable selections
  float muPtCut_;
  float tauPtCut_;
  float muEtaCut_;
  float tauEtaCut_;
  int metType_;
  float metscale_;
  int runSVFit_;
  std::string tauAntiMuDisc_;
  std::string tauAntiEDisc_;

  
  float smearSVFitMass0pi0_;
  float smearVisMass0pi0_;
  float smearSVFitMass1pi0_;
  float smearVisMass1pi0_;

  //event info
  const edm::Event * iEvent_; 

  edm::Handle< std::vector<reco::Vertex> > vertices_;
  const reco::Vertex * PV_;

  edm::Handle< std::vector<cmg::TriggerObject> > trig_;
  std::vector<edm::InputTag *>  trigPaths_;
  std::vector<edm::InputTag *>  trigPathsTest_;

  edm::InputTag genParticlesTag_;
  const reco::GenParticle * genBoson_;
  const reco::GenParticle * genBosonL1_;
  const reco::GenParticle * genBosonL2_;
  int sampleGenEventType_;
  int sampleTruthEventType_;
  int genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu
  int truthEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZOther 11=WToENu, 13=WToMuNu, 15=WToENu, 16=WOther
  float deltaRTruth_;
  float sampleGenMassMin_;
  float sampleGenMassMax_;


  edm::InputTag diTauTag_;

  edm::InputTag unscaledTauTag_;
  edm::Handle< std::vector<cmg::Tau> > unscaledTauList_;

  
  edm::InputTag muonVetoListTag_;
  edm::Handle< std::vector<cmg::Muon> > leptonVetoListMuon_;
  edm::InputTag electronVetoListTag_;
  edm::Handle< std::vector<cmg::Electron> > leptonVetoListElectron_;
  edm::InputTag tauVetoListTag_;
  edm::Handle< std::vector<cmg::Tau> > leptonVetoListTau_;

  edm::InputTag pfJetListTag_;
  std::vector<const cmg::PFJet * > fullJetList_;
  std::vector<const cmg::PFJet * > pfJetList_;
  std::vector<const cmg::PFJet * > pfJetListLC_;
  std::vector<const cmg::PFJet * > pfJetListLepLC_;
  const cmg::PFJet * leadJet_;
  const cmg::PFJet * subleadJet_;

  std::vector<const cmg::PFJet * > pfJetList20_;//Jets pf pT>20
  std::vector<const cmg::PFJet * > pfJetList20LC_;

  std::vector<const cmg::PFJet * > pfJetListBTagLC_;
  const cmg::PFJet * leadBJet_;

  std::vector<const cmg::PFJet * > pfJetListBTagLCLoose_;

  std::string pujetidname_;

  const cmg::METSignificance * metSig_;

  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;
  float triggerEffWeightMu_;
  float triggerEffWeightTau_;
  float triggerEffWeightsMu_[5];
  float triggerEffWeightsTau_[5];

  SelectionEfficiency selectionEff_;
  float selectionEffWeight_;
  float selectionEffWeightId_;
  float selectionEffWeightIso_;
  float selectionEffWeightsId_[5];
  float selectionEffWeightsIso_[5];

  //HQT weights for 2011 signal samples
  std::string signalWeightDir_;
  std::string signalWeightMass_;
  TH1F* signalWeightHisto_;
  TH1F* signalWeightHistoLow_;
  TH1F* signalWeightHistoHigh_;
  float signalWeight_;
  float signalWeightLow_;
  float signalWeightHigh_;


  //for MSSM
  float btagWP_;

  BTagSF btagsf;

  std::vector<const cmg::PFJet * > pfJetListBTagWeight_;
  BTagWeight btagWeight_;
  BTagEfficiency btagEff_;
  float btagEffWeight_;

  //generator variables
  int saveLHENUP_;
  int lhenup_;
  float genbosonmass_;
  float genbosonpt_;
  float genbosoneta_;
  float genbosonphi_;

  //event variables
  edm::InputTag pupWeightName_;
  float pupWeight_;
  float eventweight_;
  int embeddedWeightType_;
  std::string embeddedKinWeightFile_;
  float embeddedGenWeight_;//for tau embedded samples
  float embeddedGenWeights_[8];
  float embeddedLeptonWeight_;

  edm::InputTag pupWeightNames_[5];
  float pupWeights_[5];

  int runnumber_;
  int lumiblock_;
  int eventid_;
  int trigPath_[10];
  int trigTest_[10];

  int npu_;
  int nvtx_;
  float vtxx_;
  float vtxy_;
  float vtxz_;


  //di-tau variables
  int   nditau_;
  float ditaumass_;
  int   ditaucharge_;
  float ditaueta_;
  float ditaupt_;
  float ditauphi_;
  float svfitmass_;
  float mutaucostheta_;
  float ditaudeltaR_;
  float ditaudeltaEta_;
  float ditaudeltaPhi_;
  float ditaumetpt_;
  float ditaumetphi_;
  float ditaumetsvfitpt_;
  float ditaumetsvfiteta_;
  float ditaumetsvfitphi_;

  //lepton variables
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
  int   mujetmatch_;//0= no match, 1 lead jet, 2=subleading jet //check which jet list is being used !
  float mujetpt_;
  float mujeteta_;
  int   mutruth_; 
  int   mutruthstatus_;
  float mutruthpt_;
  float mutrutheta_;
  math::XYZTLorentzVector mup4_;
  int   mucharge_;
  

  //tau variables
  float taumass_;
  float taupt_;
  float taupx_;
  float taupy_;
  float taueta_;
  float tautheta_;
  float tauphi_;
  int   tautruth_;
  int   tautruthstatus_;
  float tautruthpt_;
  float tautrutheta_;
  float tauehop_;
  float taueop_;
  float tauhoe_;
  int   taudecaymode_;
  float taudz_;
  float taudxy_;
  float taux_;
  float tauy_;
  float tauz_;
  float tauzimpact_;

  int   tauantie_;//old discs
  int   tauantiemva_;//old MVA trained on old Medium
  float tauantiemva2raw_;//
  float tauantiemva3raw_;//
  int   tauantiemva3cat_;
  int   tauantiemva3_;//Loose,Medium,Tight,VTight
  int   tauantiemva3new_;//Loose,Medium,Tight,VTight
  int   tauinecalcrack_;
  int   tauantimu_;
  int   tauantimu2_;

  int   tauiso_;
  float tauisomvaraw_;
  float tauisomva2raw_;
  float tauiso3hitraw_;

  float taujetpt_;
  float taujeteta_;
  float taujetrefpt_;
  float taujetrefeta_;
  float tauleadpt_;
  float tauleadhcal_;
  float tauleadecal_;
  math::XYZTLorentzVector taup4_;
  int   taucharge_;

  //met variables
  float pftransversemass_;
  double pfmetpt_;
  double pfmetphi_;
  float transversemass_;
  double metptraw_;//save met without recoil correction
  double metphiraw_;
  double metpt_;//double needed by recoil corrector
  double metphi_;
  float  metsigcov00_;
  float  metsigcov01_;
  float  metsigcov10_;
  float  metsigcov11_;
  float  pZeta_;
  float  pZetaVis_;
  reco::Candidate::PolarLorentzVector metP4_;

  //mva met inputs
  float  pfMetForRegression_;
  float  tkMet_;
  float  nopuMet_;
  float  puMet_;
  float  pcMet_;


  //jet variables
  int njet_;
  float leadJetPt_;
  float leadJetEta_;
  float leadJetRawFactor_;
  float leadJetPUIdMva_;
  float subleadJetPt_;
  float subleadJetEta_;
  float subleadJetRawFactor_;
  float diJetMass_;
  float diJetPt_;
  float diJetDeltaEta_;
  float diJetEta1Eta2_;
  int   njetingap_;


  //need up to 4 jets (pT20): pt, eta, phi, flavor
  int njet20_;
  float jet20pt1_;
  float jet20eta1_;
  float jet20phi1_;
  float jet20mass1_;
  float jet20tagprob1_;
  int jet20flavor1_;
  float jet20pt2_;
  float jet20eta2_;
  float jet20phi2_;
  float jet20mass2_;
  float jet20tagprob2_;
  int jet20flavor2_;
  float jet20pt3_;
  float jet20eta3_;
  float jet20phi3_;
  float jet20mass3_;
  float jet20tagprob3_;
  int jet20flavor3_;
  float jet20pt4_;
  float jet20eta4_;
  float jet20phi4_;
  float jet20mass4_;
  float jet20tagprob4_;
  int jet20flavor4_;

  int nbjet_;
  float leadBJetBTagProb_;
  float leadBJetPt_;
  float leadBJetEta_;

  int nbjetLoose_;

  int njetLepLC_;
  float muLCleadJetPt_;//jets where only the muon has been removed
  float muLCleadJetEta_;

  int categoryIso_;//
  int categorySM_;//SM search 
  int categorySM2012_;//SM search 

  //selection variables
  int runrangepass_;
  int trigpass_;
  int nvtxpass_;
  int geneventtypepass_;
  int baseeventpass_;
  int dilepvetopass_;
  int ptetapass_;
  int muvtxpass_;
  int muidpass_;
  int muisopass_;
  int mumatchpass_;
  int taueoppass_;
  int tauvtxpass_;
  int taumuvetopass_;
  int tauelevetopass_;
  int tauisopass_;
  int taumatchpass_;
  int ditauisopass_;


  //event counters
  int counterall_;
  int countertrig_;
  int counterruns_;
  int countergoodvtx_;
  int countergen_;
  int countergenmass_;

  cmg::BaseMET diobjectmet_;
  int diobjectindex_;
  edm::InputTag mvaMETTag_;
  edm::InputTag mvaMETSigTag_;

  RecoilCorrector corrector_;
  int recoilCorreciton_;
  double recoiliScale_;
  std::string fileZmmData_;
  std::string fileZmmMC_;

  TRandom2 randEngine_; 
  double randsigma_;
  
  void fillTruthEventType(float taueta,float tauphi, float mueta, float muphi);
  void fillTau(const cmg::Tau * tau);
  void fillMET();
  void fillDiTauVars();
  void fillDiTauMETVars();
  bool thirdLeptonVeto();

  void fillJetVariables();
  void fillBJetVariables();
  void fillJetVariables20();
  void fillBTagWeight();
  void runSVFit();

  float vbfmva_;
  double vbfvars_[8];
  std::string mvaWeights_ ;
  VBFMVA reader_;
  void fillVBFMVA();

  float vbfmva2012_;
  double vbfvars2012_[8];
  std::string mvaWeights2012_;
  VBFMVA2012 reader2012_;
  void fillVBFMVA2012();


  int truthMatchLeg(float legeta, float legphi,float& truthpt,float& trutheta,int& truthstatus);
  
  edm::Handle< std::vector<cmg::TriggerObject> > trigObjs_;
  const cmg::TriggerObject * trigObjMatch(float eta, float phi, std::string path, std::string filter, int pdgid=-1);

  float electronRelIsoDBCorr(const cmg::Electron * cand);
  bool electronIDWP95(const cmg::Electron * cand);
  bool electronMVATight(const cmg::Electron * cand);
  bool electronMVALoose(const cmg::Electron * cand);
  bool muonIDPFMuonTight(const cmg::Muon * cand);
  bool checkPUJetId(const cmg::PFJet * jet);
  bool checkPFJetId(const cmg::PFJet * jet);


  const cmg::PFJet * findJet(std::vector<const cmg::PFJet * > * fulllist, float eta, float phi);
  void fillPFJetList(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetList20(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetListLC(float leg1eta, float leg1phi, float leg2eta, float leg2phi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);
  void fillPFJetListLepLC(float lepeta, float lepphi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);
  void fillPFJetListBTag(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetListBTagLoose(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);

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

  float computeDxy(reco::TrackBase::Point vtx, math::XYZTLorentzVector p4){
    //methods from here http://cmslxr.fnal.gov/lxr/source/DataFormats/TrackReco/interface/TrackBase.h#063
    return ( - (vtx.x()-PV_->position().x()) *  p4.y() + (vtx.y()-PV_->position().y()) *  p4.x() ) /  p4.pt();
  }
  float computeDz(reco::TrackBase::Point vtx, math::XYZTLorentzVector p4){
    //methods from here http://cmslxr.fnal.gov/lxr/source/DataFormats/TrackReco/interface/TrackBase.h#063
    return (vtx.z()-PV_->position().z()) - ((vtx.x()-PV_->position().x()) * p4.x()+(vtx.y()-PV_->position().y())*  p4.y())/ p4.pt() *  p4.z()/ p4.pt();
  }


  //pZeta computation //Code from Josh  
  //void compZeta(const cmg::Muon * leg1, const cmg::Tau * leg2, double metPx, double metPy, float * pZ, float * pZV){
  void compZeta(math::XYZTLorentzVector leg1, math::XYZTLorentzVector leg2, double metPx, double metPy, float * pZ, float * pZV){
    //leg1 is the muon and leg2 is the tau

    double leg1x = cos(leg1.phi());
    double leg1y = sin(leg1.phi());
    double leg2x = cos(leg2.phi());
    double leg2y = sin(leg2.phi());
    double zetaX = leg1x + leg2x;
    double zetaY = leg1y + leg2y;
    double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
    if ( zetaR > 0. ) {
      zetaX /= zetaR;
      zetaY /= zetaR;
    }

    double visPx = leg1.px() + leg2.px();
    double visPy = leg1.py() + leg2.py();
    //double pZetaVis = visPx*zetaX + visPy*zetaY;
    *pZV = visPx*zetaX + visPy*zetaY;
    
    double px = visPx + metPx;
    double py = visPy + metPy;
    //double pZeta = px*zetaX + py*zetaY;
    *pZ = px*zetaX + py*zetaY;
  }


  bool electronVertexCut(const cmg::Electron * cand){
    if(!((*(cand->sourcePtr()))->gsfTrack().isNonnull()))return 0;
    if(!((*(cand->sourcePtr()))->gsfTrack().isAvailable()))return 0;     
    if(fabs((*(cand->sourcePtr()))->gsfTrack()->dxy(PV_->position())) > 0.045 ) return 0;
    if(fabs((*(cand->sourcePtr()))->gsfTrack()->dz(PV_->position()))  > 0.2 ) return 0;
    return 1;
  }
  bool muonVertexCut(const cmg::Muon * cand){
    if(!((*(cand->sourcePtr()))->innerTrack().isNonnull()))return 0;
    if(!((*(cand->sourcePtr()))->innerTrack().isAvailable()))return 0;
    if(fabs((*(cand->sourcePtr()))->innerTrack()->dxy(PV_->position())) > 0.045 ) return 0;
    if(fabs((*(cand->sourcePtr()))->innerTrack()->dz(PV_->position()))  > 0.2 ) return 0;
    return 1;
  }
  bool tauVertexCut(const cmg::Tau * cand){
    if(fabs(computeDxy(cand->leadChargedHadrVertex(),cand->p4()))>0.045)return 0;
    if(fabs(computeDz(cand->leadChargedHadrVertex(),cand->p4()))>0.2)return 0;
    return 1;
  }


  //new anti-electron functions from Ivo:https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2013#Tau_Electron_MVA3_WP
  //bool passAntiEMVA(int iCat, float raw, TString WP="Medium") {
  bool passAntiEMVA(int iCat, float raw,int WP) {

    if(iCat<0)return false;
    if(iCat>15) return true;

    float cutsLoose[16]={0.835,0.831,0.849,0.859,0.873,0.823,0.85,0.855,0.816,0.861,0.862,0.847,0.893,0.82,0.845,0.851};
    float cutsMedium[16]={0.933,0.921,0.944,0.945,0.918,0.941,0.981,0.943,0.956,0.947,0.951,0.95,0.897,0.958,0.955,0.942};
    float cutsTight[16]={ 0.96,0.968,0.971,0.972,0.969,0.959,0.981,0.965,0.975,0.972,0.974,0.971,0.897,0.971,0.961,0.97};
    float cutsVeryTight[16]={0.978,0.98,0.982,0.985,0.977,0.974,0.989,0.977,0.986,0.983,0.984,0.983,0.971,0.987,0.977,0.981};
    float cut=0;

    if(WP==0) cut = cutsLoose[iCat];
    if(WP==1) cut = cutsMedium[iCat];
    if(WP==2) cut = cutsTight[iCat];
    if(WP==3) cut = cutsVeryTight[iCat];

    return (raw>cut);
  }
  bool isInEcalCrack(double eta) 
  {
    eta = fabs(eta);
    return (eta < 0.018 ||
	    (eta>0.423 && eta<0.461) ||
	    (eta>0.770 && eta<0.806) ||
	    (eta>1.127 && eta<1.163) ||
	    (eta>1.460 && eta<1.558));
  }



  ////Debugging functions:
  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  void printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList);
  void printMuonInfo(const cmg::Muon * cand);
  void printElectronInfo(const cmg::Electron * cand);
  void printTauInfo(const cmg::Tau * cand);
  void printJetInfo(const cmg::PFJet * cand, int level=0);
  void printMETInfo();


private:


  //some gen level quantities filled before any selections and  written out to log file
  TH1F * HGenBosonPt_;
  TH1F * HGenBosonMass_;
  TH1F * HGenBosonEta_;
  TH1F * HGenBosonPhi_;

};


#endif
