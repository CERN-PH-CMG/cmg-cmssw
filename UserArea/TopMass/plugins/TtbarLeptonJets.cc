// -*- C++ -*-
//
// Package:    TtbarLeptonJets
// Class:      TtbarLeptonJets
// 
/**\class TtbarLeptonJets TtbarLeptonJets.cc CMGTools/TtbarLeptonJets/src/TtbarLeptonJets.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jose Enrique Palencia Cortezon
//         Created:  Tue May  1 15:53:55 CEST 2012
// $Id: TtbarLeptonJets.cc,v 1.1 2012/10/20 14:22:01 psilva Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
//#include "LHAPDF/LHAPDF.h"  //scram setup lhapdffull and update BuildFile

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//
// class declaration
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <TLorentzVector.h>
#include "DataFormats/Math/interface/LorentzVector.h"
#include <TObjArray.h> 
#include <TMath.h> 

//#include "KoPFA/CommonTools/interface/PatElectronAnalyzer.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Record.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
//#include "/uscmst1/prod/sw/cms/slc5_ia32_gcc434/external/lhapdf/5.6.0-cms2/share/lhapdf/PDFsets/../../../include/LHAPDF/LHAPDF.h"

#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include <vector>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <iostream>
#include <fstream>
#include "TH2.h"

class TtbarLeptonJets : public edm::EDAnalyzer {
public:
  explicit TtbarLeptonJets(const edm::ParameterSet&);
  ~TtbarLeptonJets();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
  // ----------member data ---------------------------
  TH1D *countAll_h;
  
  FILE *outFile;
  ofstream myfile;

  
  //
  // constants, enums and typedefs
  //
  
  //
  // static data member definitions
  int nTot;
  int nMuoTrigg, nStep0, nStep1, nStep2, nStep3, nStep4, nStep4a, nStep4b, nStep4c, nStep5, nStep6;
  int nEleTrigg, nStep0_eleSel, nStep1_eleSel, nStep2_eleSel, nStep3_eleSel, nStep4a_eleSel, nStep4b_eleSel, nStep5a_eleSel, nStep5b_eleSel, nStep5c_eleSel, nStep5d_eleSel, nStep6_eleSel;
  
};


//

//
// constructors and destructor
//
TtbarLeptonJets::TtbarLeptonJets(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  countAll_h               = fs->make<TH1D>("countAll_h"   , "countAll_h"   , 5 , 0. , 5. );
  
  // set total counters
  nTot = 0; nMuoTrigg = 0; nStep0 = 0; nStep1 = 0; nStep2 = 0; nStep3 = 0; nStep4 = 0; nStep4a = 0; nStep4b = 0; nStep4c = 0; nStep5 = 0; nStep6 = 0; 
  nTot = 0; nEleTrigg = 0; nStep0_eleSel = 0; nStep1_eleSel = 0; nStep2_eleSel = 0; nStep3_eleSel = 0; nStep4a_eleSel = 0; nStep4b_eleSel = 0; nStep5a_eleSel = 0; nStep5b_eleSel = 0; nStep5c_eleSel = 0; nStep5d_eleSel = 0; nStep6_eleSel = 0; 
  
  outFile = fopen ("outInfo.txt", "wu" );
  myfile.open ("example.txt");
}


TtbarLeptonJets::~TtbarLeptonJets()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called for each event  ------------
void TtbarLeptonJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  using namespace edm;
  using namespace reco;
  //using namespace isodeposit;
  
  int verbose = 1;
  
  if(verbose) std::cout << iEvent.eventAuxiliary().id() << std::endl;
  
  edm::Handle<std::vector<cmg::Muon> > muons;
  iEvent.getByLabel("cmgTopTightMuonMuJetSel", muons); 
  std::vector<cmg::Muon>::const_iterator MUO;
  if(!muons.isValid())     cerr << "  WARNING: muons is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Electron> > electrons;
  iEvent.getByLabel("cmgTopTightElecEleJetSel", electrons); 
  std::vector<cmg::Electron>::const_iterator ELE;
  if(!electrons.isValid())     cerr << " WARNING: electrons is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::PFJet> > eleJets;
  iEvent.getByLabel("cmgTopJetEleJetSel", eleJets); 
  std::vector<cmg::PFJet>::const_iterator eleJET;
  if(!eleJets.isValid())     cerr << "  WARNING: eleJets is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::PFJet> > muJets;
  iEvent.getByLabel("cmgTopJetMuJetSel", muJets); 
  std::vector<cmg::PFJet>::const_iterator muJET;
  if(!muJets.isValid())     cerr << "  WARNING: muJets is not valid! " << endl;

  edm::Handle<std::vector<cmg::TriggerObject> > triggerPath;
  iEvent.getByLabel("cmgTriggerObjectSel", triggerPath); 
  std::vector<cmg::TriggerObject>::const_iterator TRIGGER;
  if(!triggerPath.isValid())     cerr << "  WARNING: trigger is not valid! " << endl;

  edm::Handle<double> puWeight2012AB;
  iEvent.getByLabel("vertexWeightSummer12MC53XHCPData", puWeight2012AB);
  if(!puWeight2012AB.isValid()) cerr << "  WARNING: puWeight2012AB is not valid! " << endl;


  // Info for PDFs weights
  edm::Handle<GenEventInfoProduct> pdfstuff;
  iEvent.getByLabel("generator", pdfstuff);
  if(!pdfstuff.isValid())  cerr<<"genInfoProd handle invalid."<<endl;

  // Infor for PU weights
  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  if(!PupInfo.isValid())     cerr << "  WARNING: PU is not valid! " << endl;
  
  
  
  
  
  nTot++;
  countAll_h ->Fill(1);
  
  
  double puWeightCMG = *puWeight2012AB;
  if(verbose) std::cout << "  pu weight = " << puWeightCMG << std::endl;



  int passEleTrig = 0, passMuoTrig = 0;
  if(triggerPath.isValid()){
     for(TRIGGER = triggerPath->begin(); TRIGGER != triggerPath->end(); ++TRIGGER) {
	if (TRIGGER->getSelectionRegExp("HLT_IsoMu24_eta2p1_v*")) passMuoTrig++;
	if (TRIGGER->getSelectionRegExp("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20_v1")) passEleTrig++;	
     }   
  }
 
  if(muons.isValid()){
    for(MUO = muons->begin(); MUO != muons->end(); ++MUO) {
      if(verbose) std::cout << "  muon pt = " << MUO->pt() << ", eta = " << MUO->eta() << std::endl;
    }
  }
  
  
  
  if(electrons.isValid()){
    for(ELE = electrons->begin(); ELE != electrons->end(); ++ELE) {
      if(verbose) std::cout << "  ele pt = " << ELE->pt() << ", eta = " << ELE->eta() << std::endl; 
    }
  }
  
  
  
  
  if(muJets.isValid()){
    for(muJET = muJets->begin(); muJET != muJets->end(); ++muJET) {
      if(verbose) std::cout << "  mu-jet pt = " << muJET->pt() << ", eta = " << muJET->eta() << ", lxy = " << muJET->Lxy() <<  std::endl;           
    } 
  }
  
  
  if(eleJets.isValid()){
    for(eleJET = eleJets->begin(); eleJET != eleJets->end(); ++eleJET) {
      if(verbose) std::cout << "  ele-jet pt = " << eleJET->pt() << ", eta = " << eleJET->eta() << ", lxy = " << eleJET->Lxy() << std::endl;           
    } 
  }



}


// ------------ method called once each job just before starting event loop  ------------
void TtbarLeptonJets::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TtbarLeptonJets::endJob() 
{
  std::cout << "\nTot\tTrig\tstep0\tstep1\tstep2\tstep3\tstep4a\tstep4b\tstep4c\tstep5\tstep6" << std::endl;


  std::cout << "\nTot\tTrig\tstep0\tstep1\tstep2\tstep3\tstep4a\tstep4b\tstep5a\tstep5b\tstep5c\tstep5d\tstep6" << std::endl;

  myfile.close();
  fclose ( outFile ) ;
}

// ------------ method called when starting to processes a run  ------------
void 
TtbarLeptonJets::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TtbarLeptonJets::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TtbarLeptonJets::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TtbarLeptonJets::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TtbarLeptonJets::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TtbarLeptonJets);
