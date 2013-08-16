// -*- C++ -*-
//
// Package:    LxyMapProducer
// Class:      LxyMapProducer
// 
/**\class LxyMapProducer LxyMapProducer.cc 

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Juerg Eugster,40 1-B06,+41227674534,
//         Created:  Fri Jan 11 17:33:17 CET 2013
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1.h"
#include "TTree.h"


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

#include <iostream>
#include <fstream>
#include "TH2.h"
#include "TLorentzVector.h"
#include <vector>
#include <cmath>

//
// class declaration
//

class LxyMapProducer : public edm::EDAnalyzer {
   public:
      explicit LxyMapProducer(const edm::ParameterSet&);
      ~LxyMapProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  

  void clearTreeVariables();

      // ----------member data ---------------------------

  std::string treeName_;    
  TTree*      tree_;
  int fnjets, fnpu;
  float flxy, flxyerr, fpt, fsecvtxmass, feta,fbtag,fgamma;
  float fdR; 
  float fpuW;
  
  float fgenpt, fgeneta, frecopt, frecoeta;


  bool isData;



};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
LxyMapProducer::LxyMapProducer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

  isData = false;

}


LxyMapProducer::~LxyMapProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
LxyMapProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace reco;

   int verbose = 0;


   // clear the variables which get filled in the tree
   clearTreeVariables();


   // primary vertex filter
   edm::Handle<std::vector<reco::Vertex> > vertices;
   iEvent.getByLabel("primaryVertexFilter", vertices);
   std::vector<reco::Vertex>::const_iterator PVTX;
   if(!vertices.isValid())     cerr << "  WARNING: PV is not valid! " << endl;
   
   // PF jets
   edm::Handle<std::vector<cmg::PFJet> > jets;
   //iEvent.getByLabel("cmgPFJetSel", jets); 
   iEvent.getByLabel("cmgTopJetPreSel", jets); 
   std::vector<cmg::PFJet>::const_iterator jetT;
   if(!jets.isValid())     cerr << "  WARNING: jets is not valid! " << endl;
   
   // gen jets
   edm::Handle<std::vector<cmg::PhysicsObjectWithPtr<edm::Ptr<reco::GenJet> > > >genJets;
   iEvent.getByLabel("genJetSel", genJets); 
   std::vector<cmg::PhysicsObjectWithPtr<edm::Ptr<reco::GenJet> > >::const_iterator genJET;
   if(!genJets.isValid())     cerr << "  WARNING: genJets is not valid! " << endl;

   // gen particles
   edm::Handle< std::vector<reco::GenParticle> > genParticles;
   iEvent.getByLabel("genParticlesPruned", genParticles);
   std::vector<reco::GenParticle>::const_iterator genParticle;
   if(!genParticles.isValid())     cerr << "  WARNING: genParticles is not valid! " << endl;
   
   // pu weights
   edm::Handle<double> puWeight2012AB;
   iEvent.getByLabel("vertexWeightSummer12MC53XHCPData", puWeight2012AB);
   if(!puWeight2012AB.isValid() && !isData) cerr << "  WARNING: puWeight2012AB is not valid! " << endl;
   
   // Info for PU weights
   edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
   iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
   std::vector<PileupSummaryInfo>::const_iterator PVI;
   if(!PupInfo.isValid() && !isData)     cerr << "  WARNING: PU is not valid! " << endl;
  

   float finalWeight;
  
   float puWeightCMG = 1.0;
   if(!isData){
     puWeightCMG = *puWeight2012AB;
     if(verbose) std::cout << "  pu weight = " << puWeightCMG << std::endl;
   }
   finalWeight = puWeightCMG;

   fpuW = finalWeight;

   if(verbose) {
     std::cout << "number of PF jets: " <<jets->size() << std::endl;
   }
  

   //PupInfo
   for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
     fnpu = PVI->getPU_NumInteractions();
     if(verbose) std::cout << "number of gen PU: " << PVI->getPU_NumInteractions() << std::endl;
     //std::cout << PVI->TrueNumInteractions() << std::endl;
   }


   // bool leptonEvent = false;

   // loop over gen particles
   vector<TLorentzVector> b_quarks;
   for(genParticle = genParticles->begin(); genParticle != genParticles->end(); ++genParticle) {
     // if(abs(genParticle->pdgId()) == 11 && abs(genParticle->mother()->pdgId()) == 24) {
     //   leptonEvent = true;
     // }
     if(abs(genParticle->pdgId()) == 5 /*&& abs(genParticle->mother()->pdgId()) == 5*/ && genParticle->status() == 2) {
       TLorentzVector b_quark;
       b_quark.SetPxPyPzE(genParticle->px(),  genParticle->py(),  genParticle->pz(),  genParticle->energy());
       b_quarks.push_back(b_quark);
       if(verbose) {
	 std::cout << "genParticle: " << genParticle->pt() << " " << genParticle->eta() << " " << genParticle->phi()<< std::endl;
	 std::cout << "genParticle: " << genParticle->px() << " " << genParticle->py() << " " << genParticle->pz() << " " << genParticle->energy()<< std::endl;
       }
     }
   }

   if(verbose) {
     std::cout << "number of b-quarks found: " << b_quarks.size() << endl;
   } 

   // // loop over gen jets
   // vector<TLorentzVector> gen_jets;
   // for(genJET = genJets->begin(); genJET != genJets->end(); ++genJET) {
   //   TLorentzVector gen_jet;
   //   gen_jet.SetPxPyPzE(genJET->px(), genJET->py(), genJET->pz(), genJET->energy());
   //   gen_jets.push_back(gen_jet);
   //   std::cout << "gen jet: " << genJET->pt() << " " << genJET->eta() << " " << genJET->phi() << std::endl;
   //   std::cout << "vertex: " << genJET->vertex() << std::endl;
   //   //std::cout << "gen jet: " << genJET->pdgId() << std::endl;
   //   //std::cout << "gen jet: " << genJET->sourcePtr()->get()->pdgId() << std::endl;
   // }
   // std::cout << "number of gen-jets found: " << gen_jets.size() << endl;

   // // match the gen_jets to the b-quarks
   // vector<TLorentzVector> selected_gen_jets;
   // for(unsigned int j=0; j<b_quarks.size(); ++j) {
   //   float deltaR = 999.999;
   //   TLorentzVector selected_gen_jet;
   //   for(unsigned int i=0; i<gen_jets.size(); ++i) {
   //     std::cout << gen_jets[i].Pt() << std::endl;
   //     std::cout << b_quarks[j].DeltaR(gen_jets[i]) << std::endl;
   //     if(abs(b_quarks[j].DeltaR(gen_jets[i])) < deltaR) {
   // 	 selected_gen_jet = gen_jets[i];
   //     }
   //   }
   //   selected_gen_jets.push_back(selected_gen_jet);
   // }
   // std::cout << "number of selected gen jets: " << selected_gen_jets.size() << std::endl;




   // loop over PFjets
   // get the jet with maxlxy
   // b-jets only...
   int njets = 0;
   float maxlxy = -999.999,maxlxyErr = 0.;
   float deltaR = 9999.99;
   float bq_pt = -999.99, reco_pt = -999.99;
   float bq_eta = -999.99, reco_eta = -999.99;
   float bq_gamma = -999.99;
   for(std::vector<cmg::PFJet>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet) {
     // only b-jets
     if(abs(jet->partonFlavour()) != 5) continue;
     if (jet->Lxy() <= 0.) continue;
     if(jet->Lxy() > maxlxy) {
       maxlxy = jet->Lxy();
       maxlxyErr = jet->LxyErr();
       reco_pt = jet->pt(); 
       reco_eta = jet->eta(); 
       TLorentzVector pfjet;
       pfjet.SetPxPyPzE(jet->px(), jet->py(), jet->pz(), jet->energy());
       // match the jet to one of the b-quarks
       if(verbose) std::cout << "------------------" << std::endl;
       for(unsigned int i=0; i<b_quarks.size(); ++i) {
       //for(unsigned int i=0; i<2; ++i) {
	 if(pfjet.DeltaR(b_quarks[i]) < deltaR) {
	   deltaR = pfjet.DeltaR(b_quarks[i]);
	   if(verbose) std::cout << deltaR << std::endl;
	   bq_pt  = b_quarks[i].Pt();
	   bq_eta = b_quarks[i].Eta();
	   bq_gamma = b_quarks[i].Gamma();
	 }
       }
       // flxy = jet->Lxy();
       // flxyerr = jet->LxyErr();
       fsecvtxmass = jet->secvtxMass();
       // fpt  = jet->pt();
       // feta = jet->eta();
       // fpt  = pt;
       // feta = eta;
       // fgamma = gamma;
       fbtag = jet->btag(6);
     }
     njets++;
   }
   flxy = maxlxy;
   flxyerr = maxlxyErr;
   frecopt = reco_pt;
   frecoeta = reco_eta;
   fpt  = bq_pt;
   feta = bq_eta;
   fgamma = bq_gamma;
   fdR = deltaR;
   fnjets = njets;
   
   

   tree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
LxyMapProducer::beginJob()
{
  // create FileService
  edm::Service<TFileService> fileService;
  tree_ = fileService->make<TTree>("map", "map");
  tree_->Branch("njets"      ,&fnjets        ,"njets/I");
  tree_->Branch("npu"        ,&fnpu          ,"npu/I");
  tree_->Branch("secvtxmass" ,&fsecvtxmass   ,"secvtxmass/F");
  tree_->Branch("lxy"        ,&flxy          ,"lxy/F");
  tree_->Branch("lxyerr"     ,&flxyerr       ,"lxyerr/F");
  tree_->Branch("pt"         ,&fpt           ,"pt/F");
  tree_->Branch("eta"        ,&feta          ,"eta/F");
  tree_->Branch("btag"       ,&fbtag         ,"btag/F");
  tree_->Branch("puW"        ,&fpuW          ,"puW/F");
  tree_->Branch("gamma"      ,&fgamma        ,"gamma/F");
  tree_->Branch("dR"         ,&fdR           ,"dR/F");
  tree_->Branch("recopt"     ,&frecopt       ,"recopt/F");
  tree_->Branch("recoeta"    ,&frecoeta      ,"recoeta/F");



}

// ------------ method called once each job just after ending the event loop  ------------
void 
LxyMapProducer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
LxyMapProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
LxyMapProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
LxyMapProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
LxyMapProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LxyMapProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
LxyMapProducer::clearTreeVariables() {
  // integers
  fnjets = -999;
  //doubles 
  fpuW  = 1.;
  flxy  = -999.999;
  frecopt   = -999.999;
  frecoeta  = -999.999;
  fpt   = -999.999;
  feta  = -999.999;
  fbtag = -999.999;
  fgamma = -999.999;
  fdR = -999.999;
}




//define this as a plug-in
DEFINE_FWK_MODULE(LxyMapProducer);
