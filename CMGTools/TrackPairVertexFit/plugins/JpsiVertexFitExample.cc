// -*- C++ -*-
//
// Package:    JpsiVertexFitExample
// Class:      JpsiVertexFitExample
// 
/**\class JpsiVertexFitExample JpsiVertexFitExample.cc CMGTools/JpsiVertexFitExample/src/JpsiVertexFitExample.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michail Bachtis,40 1-B08,+41227678176,
//         Created:  Mon Mar  2 16:53:15 CET 2015
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
#include "CMGTools/TrackPairVertexFit/interface/CMGTrackPairVertexFitter.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "TH1F.h"
#include "TFile.h"
//
// class declaration
//

class JpsiVertexFitExample : public edm::EDAnalyzer {
   public:
      explicit JpsiVertexFitExample(const edm::ParameterSet&);
      ~JpsiVertexFitExample();

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
  CMGTrackPairVertexFitter *fitter;
  TFile *file; 
  TH1F *before; 
  TH1F *after; 
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
JpsiVertexFitExample::JpsiVertexFitExample(const edm::ParameterSet& iConfig)

{

  file = new TFile("output.root","RECREATE");
  before = new TH1F("before","before",1000,2,12);
  after = new TH1F("after","after",1000,2,12);
  
  fitter = new CMGTrackPairVertexFitter();
  
 //now do what ever initialization is needed

}


JpsiVertexFitExample::~JpsiVertexFitExample()
{
 
  if (fitter)
    delete fitter;
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)


  if (before)
    delete before;
  if (after)
    delete after;
  if (file)
    delete file;

}


//
// member functions
//

// ------------ method called for each event  ------------
void
JpsiVertexFitExample::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



   Handle<std::vector<cmg::Muon> > muonsH;
   iEvent.getByLabel("cmgMuonSel",muonsH);

   for (unsigned int i=0;i<muonsH->size()-1;++i)
     for (unsigned int j=i+1;j<muonsH->size();++j) {
       const cmg::Muon& mu1= (*muonsH)[i];  
       const cmg::Muon& mu2= (*muonsH)[j];  
       if ((mu1.charge()+mu2.charge())!=0)
	 continue;
       
       if (!(mu1.isTrackerMuon()&&mu2.isTrackerMuon()))
	 continue;

       float m = (mu1.p4()+mu2.p4()).M();

       //       if (m<2.8 || m>3.3)
       //	 continue;

       printf("Mass before=%f \n",(mu1.p4()+mu2.p4()).M());
       before->Fill(m);
       if (!((*mu1.sourcePtr())->innerTrack().isNonnull() && (*mu1.sourcePtr())->innerTrack().isNonnull()))
	 continue;
       if (fitter->fit((*mu1.sourcePtr())->innerTrack(),(*mu2.sourcePtr())->innerTrack(),iSetup)) {
	 printf("Fitted plus=%f %f %f\n",fitter->plus()->Pt(),fitter->plus()->Eta(),fitter->plus()->Phi());
	 printf("Fitted minus=%f %f %f\n",fitter->minus()->Pt(),fitter->minus()->Eta(),fitter->minus()->Phi());
       }

       float m2 = ((*fitter->plus())+(*fitter->minus())).M();

       printf("Mass after=%f \n",m2);
       after->Fill(m2);

     }

}


// ------------ method called once each job just before starting event loop  ------------
void 
JpsiVertexFitExample::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JpsiVertexFitExample::endJob() 
{
  file->cd();
  before->Write();
  after->Write();
  file->Close();

}

// ------------ method called when starting to processes a run  ------------
void 
JpsiVertexFitExample::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JpsiVertexFitExample::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JpsiVertexFitExample::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JpsiVertexFitExample::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JpsiVertexFitExample::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JpsiVertexFitExample);
