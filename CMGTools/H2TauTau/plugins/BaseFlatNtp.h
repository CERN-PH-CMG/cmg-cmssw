#ifndef CMGH2TAUTAU_BaseFlatNtp_h
#define CMGH2TAUTAU_BaseFlatNtp_h

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
  bool trigObjMatch(float eta, float phi, std::string path, std::string filter);

  

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

 private:



};


#endif
