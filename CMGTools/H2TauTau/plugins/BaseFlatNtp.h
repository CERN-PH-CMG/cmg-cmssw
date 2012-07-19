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
//#include "CMGTools/H2TauTau/interface/TauRate.h"


using namespace std;

class BaseFlatNtp : public edm::EDAnalyzer{


 public:
  explicit BaseFlatNtp(const edm::ParameterSet&);
  ~BaseFlatNtp();

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;

protected:

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

  int dataPeriodFlag_;
  edm::InputTag verticesListTag_;
  edm::InputTag trigPathsListTag_;
  edm::InputTag trigObjsListTag_;
  edm::InputTag pupWeightName_;
  int firstRun_; 
  int lastRun_; 
  int dataType_;//0=MC, 1=Data, 2=EmbeddedData, 3=EmbeddedMC

  const edm::Event * iEvent_; 
  edm::Handle< std::vector<reco::Vertex> > vertices_;
  const reco::Vertex * PV_;
  std::vector<edm::InputTag *>  trigPaths_;


  edm::InputTag genParticlesTag_;
  edm::InputTag pfJetListTag_;

  std::vector<const cmg::PFJet * > pfJetList_;
  std::vector<const cmg::PFJet * > pfJetListLC_;
  std::vector<const cmg::PFJet * > pfJetListLepLC_;
  const cmg::PFJet * leadJet_;
  const cmg::PFJet * subleadJet_;

  std::vector<const cmg::PFJet * > pfJetListBTag_;
  std::vector<const cmg::PFJet * > pfJetListBTagLC_;
  const cmg::PFJet * leadBJet_;


  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;

  SelectionEfficiency selectionEff_;
  float selectionEffWeight_;



  edm::Service<TFileService> * file_;
  TTree * tree_;
  float pupWeight_;
  float eventweight_;
  int runnumber_;
  int lumiblock_;
  int eventid_;
  bool trigpass_;

  int npu_;
  int nvtx_;
  float vtxx_;
  float vtxy_;
  float vtxz_;

  
  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  void printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList);
  
  edm::Handle< std::vector<cmg::TriggerObject> > trigObjs_;
  bool trigObjMatch(float eta, float phi, std::string path, std::string filter, int pdgid=-1);

  

  bool checkPFJetId(const cmg::PFJet * jet){
    //Loose PF Jet id 
    ///https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    if(jet->component(5).fraction() < 0.99
       &&jet->component(4).fraction() < 0.99
       &&jet->nConstituents() > 1
       &&(jet->component(1).fraction() > 0 || abs(jet->eta()) > 2.4)
       &&(jet->component(1).number() > 0 || abs(jet->eta()) > 2.4)
       &&(jet->component(2).fraction() < 0.99 || abs(jet->eta()) > 2.4)        
       )return 1;
    else return 0;
  }


  
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



  TRandom2 randEngine_; 
  double randsigma_;
  




  int counterall_;
  int countertrig_;
  int counterruns_;
  int countergoodvtx_;




 private:



};


#endif
