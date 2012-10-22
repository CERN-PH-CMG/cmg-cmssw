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
// $Id: TtbarLeptonJets.cc,v 1.5 2012/10/22 13:23:13 palencia Exp $
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
  TH1D *countAll_h, *mu_maxLxy_h, *e_maxLxy_h, *mu_pT_h, *e_pT_h, *mu_PV_h, *e_PV_h;
  
  FILE *outFile;
  ofstream myfile;

  
  //
  // constants, enums and typedefs
  //
  
  //
  // static data member definitions
  int nTot;
  int passMuoTrig, passTightMuoMJ, passLooseMuoMJ, passLooseEleMJ, pass4jetsMJ, passMJ;
  int passEleTrig, passTightEleEJ, passLooseMuoEJ, passLooseEleEJ, pass4jetsEJ, passEJ;
  
};


//

//
// constructors and destructor
//
TtbarLeptonJets::TtbarLeptonJets(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  countAll_h    = fs->make<TH1D>("countAll_h"  , "countAll_h"   , 5   ,   0. ,    5. );
  mu_maxLxy_h   = fs->make<TH1D>("mu_maxLxy_h" , "mu_maxLxy_h"  , 100 ,   0. ,   10. );
  mu_pT_h       = fs->make<TH1D>("mu_pT_h"     , "mu_pT_h"      ,  60 ,   0. ,  300. );
  mu_PV_h       = fs->make<TH1D>("mu_PV_h"     , "mu_PV_h"      ,  41 , -0.5 , 40.5  );
  e_maxLxy_h    = fs->make<TH1D>("e_maxLxy_h"  , "e_maxLxy_h"   , 100 ,   0. ,   10. );
  e_pT_h        = fs->make<TH1D>("e_pT_h"      , "e_pT_h"       ,  60 ,   0. ,  300. );
  e_PV_h        = fs->make<TH1D>("e_PV_h"      , "e_PV_h"       ,  41 , -0.5 , 40.5  );
  
  // set total counters
  nTot = 0;
  passMuoTrig = 0, passTightMuoMJ = 0; passLooseMuoMJ = 0; passLooseEleMJ = 0; pass4jetsMJ = 0; passMJ = 0;
  passEleTrig = 0, passTightEleEJ = 0; passLooseMuoEJ = 0; passLooseEleEJ = 0; pass4jetsEJ = 0; passEJ = 0;
  

  
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
  
  int verbose = 0;
  bool isData = true;
  
  if(verbose) std::cout << "\n" << iEvent.eventAuxiliary().id() << std::endl;

  edm::Handle<std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel("primaryVertexFilter", vertices);
  std::vector<reco::Vertex>::const_iterator PVTX;
  if(!vertices.isValid())     cerr << "  WARNING: PV is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Muon> > tightMuonMJ;
  iEvent.getByLabel("cmgTopTightMuonMuJetSel", tightMuonMJ); 
  std::vector<cmg::Muon>::const_iterator MUOtight;
  if(!tightMuonMJ.isValid())     cerr << "  WARNING: tightMuonMJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Muon> > looseMuonMJ;
  iEvent.getByLabel("cmgTopLooseMuonMuJetSel", looseMuonMJ); 
  std::vector<cmg::Muon>::const_iterator MUOlooseMJ;
  if(!looseMuonMJ.isValid())     cerr << "  WARNING: looseMuonMJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Electron> > looseElectronMJ;
  iEvent.getByLabel("cmgTopLooseElecMuJetSel", looseElectronMJ); 
  std::vector<cmg::Electron>::const_iterator ELElooseMJ;
  if(!looseElectronMJ.isValid())     cerr << " WARNING: looseElectronMJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::PFJet> > jetsMJ;
  iEvent.getByLabel("cmgTopJetMuJetSel", jetsMJ); 
  std::vector<cmg::PFJet>::const_iterator muJET;
  if(!jetsMJ.isValid())     cerr << "  WARNING: jetsMJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Electron> > tightElectronEJ;
  iEvent.getByLabel("cmgTopTightElecEleJetSel", tightElectronEJ); 
  std::vector<cmg::Electron>::const_iterator ELEtight;
  if(!tightElectronEJ.isValid())     cerr << " WARNING: tightElectronEJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Muon> > looseMuonEJ;
  iEvent.getByLabel("cmgTopLooseMuonEleJetSel", looseMuonEJ); 
  std::vector<cmg::Muon>::const_iterator MUOlooseEJ;
  if(!looseMuonEJ.isValid())     cerr << "  WARNING: looseMuonEJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::Electron> > looseElectronEJ;
  iEvent.getByLabel("cmgTopLooseElecEleJetSel", looseElectronEJ); 
  std::vector<cmg::Electron>::const_iterator ELElooseEJ;
  if(!looseElectronEJ.isValid())     cerr << " WARNING: looseElectronEJ is not valid! " << endl;
  
  edm::Handle<std::vector<cmg::PFJet> > jetsEJ;
  iEvent.getByLabel("cmgTopJetEleJetSel", jetsEJ); 
  std::vector<cmg::PFJet>::const_iterator eleJET;
  if(!jetsEJ.isValid())     cerr << "  WARNING: jetsEJ is not valid! " << endl;

  edm::Handle<std::vector<cmg::TriggerObject> > triggerPath;
  iEvent.getByLabel("cmgTriggerObjectSel", triggerPath); 
  std::vector<cmg::TriggerObject>::const_iterator TRIGGER;
  if(!triggerPath.isValid())     cerr << "  WARNING: trigger is not valid! " << endl;

  edm::Handle<double> puWeight2012AB;
  iEvent.getByLabel("vertexWeightSummer12MC53XHCPData", puWeight2012AB);
  if(!puWeight2012AB.isValid() && !isData) cerr << "  WARNING: puWeight2012AB is not valid! " << endl;

  // Info for PDFs weights
  edm::Handle<GenEventInfoProduct> pdfstuff;
  iEvent.getByLabel("generator", pdfstuff);
  if(!pdfstuff.isValid() && !isData)  cerr<<"  WARNING: genInfoProd handle invalid."<<endl;
  
  // Infor for PU weights
  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  if(!PupInfo.isValid() && !isData)     cerr << "  WARNING: PU is not valid! " << endl;
  
  
  
  
  nTot++;
  countAll_h ->Fill(1);
  
  double finalWeight;
  
  double puWeightCMG = 1.0;
  if(!isData){
     puWeightCMG = *puWeight2012AB;
     if(verbose) std::cout << "  pu weight = " << puWeightCMG << std::endl;
  }
  
  finalWeight = puWeightCMG;
  
  // Check triggers here: http://fwyzard.web.cern.ch/fwyzard/hlt/2012/summary
  // 2012A: HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30_v* 						       ---> 45, 45, 45, 20 jet pT cuts
  // 2012B: HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v* || HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v*   ---> 45, 45, 45, 20 jet pT cuts
  //# 2012C and D: HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet45_35_25_v*                                           ---> 55, 45, 35, 20 jet pT cuts
  // 2012A (190456<=run<=193621): HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30_v* with (45,45,45,20) jet pT cuts
  // 2012B (193834<=run<=194225): HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v1 with (45,45,45,20) jet pT cuts
  // 2012B (194270<=run<=196531): HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v1 with (45,45,35,20) jet pT cuts
  // 198022<=run<=199608:         HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v2 with (45,45,35,20) jet pT cuts
  // 199698<=run<=202504:         HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet45_35_25_v1 with (55,45,35,20) jet pT cuts
  // 202970<=run<=203002:         HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet45_35_25_v2 with (55,45,35,20) jet pT cuts

  int nMuoTrig = 0, nEleTrig= 0;
  if(triggerPath.isValid()){
     for(TRIGGER = triggerPath->begin(); TRIGGER != triggerPath->end(); ++TRIGGER) {
        if(isData){
	   if(iEvent.eventAuxiliary().run()>=190456 && iEvent.eventAuxiliary().run()<=193621){
              if ( TRIGGER->getSelectionRegExp("HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30_v*"))                         nMuoTrig++;
              if ( TRIGGER->getSelectionRegExp("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_v*")) nEleTrig++;
           }else if(iEvent.eventAuxiliary().run()>=193834 && iEvent.eventAuxiliary().run()<=199608){
              if ( TRIGGER->getSelectionRegExp("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v*") || TRIGGER->getSelectionRegExp("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v*") ) nMuoTrig++;
              if ( TRIGGER->getSelectionRegExp("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20_v*") )                                                       nEleTrig++;
	   }else if(iEvent.eventAuxiliary().run()>=199698){
              if ( TRIGGER->getSelectionRegExp("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet45_35_25_v*") )                           nMuoTrig++;
              if ( TRIGGER->getSelectionRegExp("HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet45_35_25_v*") ) nEleTrig++;
           } else std::cout << "  wrong run number???? = " << std::endl;
	}else{	 
	   // Last recommendation is to not apply HLT in MC
	   //if (TRIGGER->getSelectionRegExp("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30_v1")) nMuoTrig++;
	   //if (TRIGGER->getSelectionRegExp("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30_v5") || TRIGGER->getSelectionRegExp("HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30_v1")) nEleTrig++;	
	  nMuoTrig++;
	  nEleTrig++;
        }   
     }   
  }
 
  
  bool passMJsel = false, passEJsel = false;
  int  goodJetEJ = 0, goodJetLxyEJ = 0; 
  int  goodJetMJ = 0, goodJetLxyMJ = 0; 
  float maxLxyMJ = -999., maxLxyEJ = -999.;
  float muo_pT = -999., ele_pT = -999.;
  float jetMJ_pT[jetsMJ->size()], jetEJ_pT[jetsEJ->size()];
  float jetMJ_lxy[jetsMJ->size()], jetEJ_lxy[jetsEJ->size()];
  
  if(nMuoTrig == 1){
     passMuoTrig++;
     if(tightMuonMJ->size() == 1){
        passTightMuoMJ++;
        if(looseMuonMJ->size() == 0){
           passLooseMuoMJ++;
	   if(looseElectronMJ->size() == 0){ 
	      passLooseEleMJ++;
	      passMJsel = true;
	   }
        }   
     }   
  }   
  if(nEleTrig == 1){
     passEleTrig++;
     if(tightElectronEJ->size() == 1){
        passTightEleEJ++;
        if(looseElectronEJ->size() == 0){
           passLooseEleEJ++;
	   if(looseMuonEJ->size() == 0){ 
	      passLooseMuoEJ++;
	      passEJsel = true;
	   }
        }   
     }   
  }   
  
  
  if(passMJsel || passEJsel){
 
     if(vertices.isValid()){
        for(PVTX = vertices->begin(); PVTX != vertices->end(); ++PVTX) {
  	   if(verbose) std::cout << "  PV ndof = " << PVTX->ndof() << ", z = " << PVTX->z() << ", rho = " << PVTX->position().rho() << std::endl;
        }
     }
 
 
     if(tightMuonMJ.isValid() && passMJsel){
        for(MUOtight = tightMuonMJ->begin(); MUOtight != tightMuonMJ->end(); ++MUOtight) {
           if(verbose) std::cout << "  tight muon pt = " << MUOtight->pt() << ", eta = " << MUOtight->eta() << std::endl;
           muo_pT = MUOtight->pt();
	}  // tightMuonMJ loop
     }
  
  
  
     if(tightElectronEJ.isValid() && passEJsel){
        for(ELEtight = tightElectronEJ->begin(); ELEtight != tightElectronEJ->end(); ++ELEtight) {
           if(verbose) std::cout << " tight ele pt = " << ELEtight->pt() << ", eta = " << ELEtight->eta() << std::endl; 
           ele_pT = ELEtight->pt();
        }  // tightElectronEJ loop
     }
  
  
     int jetMuoIndex = 0;
     if(jetsMJ.isValid() && passMJsel){
        for(muJET = jetsMJ->begin(); muJET != jetsMJ->end(); ++muJET) {
	   jetMJ_pT[jetMuoIndex]  = muJET->pt();
	   jetMJ_lxy[jetMuoIndex] = muJET->Lxy();
           if(verbose) std::cout << "  mu-jet pt = " << muJET->pt() << ", eta = " << muJET->eta() << ", lxy = " << muJET->Lxy() << "  my mu-jet pt = " << jetMJ_pT[jetMuoIndex] <<  std::endl; 
	   jetMuoIndex++;
        } // muJET loop
     }
     if(jetMJ_pT[0]>=45 && jetMJ_pT[1]>=45 && jetMJ_pT[2]>=45 && jetMJ_pT[3]>=20) {
        goodJetMJ++;
        for(unsigned int i=0; i < jetsMJ->size(); ++i) {
	   if(jetMJ_pT[i]>=20 && jetMJ_lxy[i]>0) goodJetLxyMJ++;
	   if(jetMJ_lxy[i]>maxLxyMJ) maxLxyMJ = jetMJ_lxy[i] ;       
        }
     }
  
  
     int jetEleIndex = 0;
     if(jetsEJ.isValid() && passEJsel){
        for(eleJET = jetsEJ->begin(); eleJET != jetsEJ->end(); ++eleJET) {
	   jetEJ_pT[jetEleIndex]  = eleJET->pt();
	   jetEJ_lxy[jetEleIndex] = eleJET->Lxy();
           if(verbose) std::cout << "  ele-jet pt = " << eleJET->pt() << ", eta = " << eleJET->eta() << ", lxy = " << eleJET->Lxy() << std::endl;           
	   jetEleIndex++;
        } // eleJET loop
     }
     if(jetEJ_pT[0]>=45 && jetEJ_pT[1]>=45 && jetEJ_pT[2]>=45 && jetEJ_pT[3]>=20) {
        goodJetEJ++;
        for(unsigned int i=0; i < jetsEJ->size(); ++i) {
	   if(jetEJ_pT[i]>=20 && jetEJ_lxy[i]>0) goodJetLxyEJ++;
	   if(jetEJ_lxy[i]>maxLxyEJ) maxLxyEJ = jetEJ_lxy[i] ;       
        }
     }

  }// if(passMJsel || passEJsel)
  
  if (goodJetMJ>=1){
     pass4jetsMJ++;
     if (goodJetLxyMJ>0){
        passMJ++;
        //fprintf (outFile, "%i : %i\n", iEvent.eventAuxiliary().event(), iEvent.eventAuxiliary().luminosityBlock());
        //myfile << iEvent.eventAuxiliary().event() << " : " << iEvent.eventAuxiliary().luminosityBlock() << "\n" ;   
     }
  }
  if (goodJetEJ>=1){
     pass4jetsEJ++;
     if (goodJetLxyEJ>0){
        passEJ++;
     }
  }
  
  if (goodJetMJ<1 || goodJetLxyMJ<1) passMJsel = false;
  if (goodJetEJ<1 || goodJetLxyEJ<1) passEJsel = false;

  // Fill histograms after full selection
  if(passMJsel){
     mu_PV_h->Fill(vertices->size(), finalWeight);
     mu_maxLxy_h->Fill(maxLxyMJ, finalWeight);
     mu_pT_h->Fill(muo_pT, finalWeight);
  }
  if(passEJsel){
     e_PV_h->Fill(vertices->size(), finalWeight);
     e_maxLxy_h->Fill(maxLxyEJ, finalWeight);
     e_pT_h->Fill(ele_pT, finalWeight);
  }

  if(passMJsel && passEJsel) std::cout << " WARNING: event  " << iEvent.eventAuxiliary().event() << " passes both selections..." <<  std::endl;

}


// ------------ method called once each job just before starting event loop  ------------
void TtbarLeptonJets::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TtbarLeptonJets::endJob() 
{
  std::cout << "\nFilters\tmuoTrig\tTightMuo\tLooseMuo\tLooseEle\t4jets\tLxy" << std::endl;
  std::cout << nTot << "\t" << passMuoTrig << "\t" << passTightMuoMJ << "\t" << passLooseMuoMJ << "\t" << passLooseEleMJ << "\t" << pass4jetsMJ << "\t" << passMJ << std::endl;

  std::cout << "\nFilters\teleTrig\tTightEle\tLooseEle\tLooseMuo\t4jets\tLxy" << std::endl;
  std::cout << nTot << "\t" << passEleTrig << "\t" << passTightEleEJ << "\t" << passLooseEleEJ << "\t" << passLooseMuoEJ << "\t" << pass4jetsEJ << "\t" << passEJ << std::endl;



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
