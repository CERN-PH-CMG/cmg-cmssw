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
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMGTools/TrackPairVertexFit/interface/CMGTrackPairVertexFitter.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include <DataFormats/MuonReco/interface/Muon.h>

#include "TH1F.h"
#include "TFile.h"
//
// class declaration
//

class JpsiVertexFitExample : public edm::EDProducer {
public:
  typedef cmg::Muon  Muon;
  typedef std::vector< Muon > MuonCollection;
  
  explicit JpsiVertexFitExample(const edm::ParameterSet& iConfig);
  virtual ~JpsiVertexFitExample();

  virtual void produce(edm::Event&, const edm::EventSetup&);

  // static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  virtual void beginJob() ;
  virtual void endJob() ;

private:

  // ----------member data ---------------------------
  CMGTrackPairVertexFitter *fitter;
  TFile *file; 
  TH1F *before; 
  TH1F *after; 
};

//
// constructors and destructor
//
JpsiVertexFitExample::JpsiVertexFitExample(const edm::ParameterSet& iConfig)
{
  // produces< std::vector<cmg::Muon> > ();
  produces< std::vector< Muon > >();

  //now do what ever initialization is needed

  file = new TFile("output.root","RECREATE");
  before = new TH1F("before","before",300,2.6,3.6);
  after = new TH1F("after","after",300,2.6,3.6);
  
  fitter = new CMGTrackPairVertexFitter();
  

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
JpsiVertexFitExample::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;

  double Jpsi_mass = 3.096916, mass_best_postfit=1e10,mass_best_prefit=1e10;
  unsigned int mu1_idx=0, mu2_idx=1;

  Handle< MuonCollection > muonsH;
  iEvent.getByLabel("cmgMuonSel",muonsH);

  typedef std::auto_ptr< MuonCollection >  OutPtr;

  if(muonsH->size()<2) return;
  
  for (unsigned int i=0;i<muonsH->size()-1;++i){
    for (unsigned int j=i+1;j<muonsH->size();++j) {
      const cmg::Muon& mu1= (*muonsH)[i];  
      const cmg::Muon& mu2= (*muonsH)[j];  
      if ((mu1.charge()+mu2.charge())!=0)
        continue;
      
      if (!(mu1.isTrackerMuon()&&mu2.isTrackerMuon()))
        continue;

      float m = (mu1.p4()+mu2.p4()).M();

      // printf("Mass before=%f \n",(mu1.p4()+mu2.p4()).M());
      if (!((*mu1.sourcePtr())->innerTrack().isNonnull() && (*mu1.sourcePtr())->innerTrack().isNonnull()))
        continue;
      
      if (fitter->fit((*mu1.sourcePtr())->innerTrack(),(*mu2.sourcePtr())->innerTrack(),iSetup)) {
        // printf("Fitted plus=%f %f %f\n",fitter->plus()->Pt(),fitter->plus()->Eta(),fitter->plus()->Phi());
        // printf("Fitted minus=%f %f %f\n",fitter->minus()->Pt(),fitter->minus()->Eta(),fitter->minus()->Phi());

        float m2 = ((*fitter->plus())+(*fitter->minus())).M();
        if(TMath::Abs(m2-Jpsi_mass)<TMath::Abs(mass_best_postfit-Jpsi_mass)){
          mu1_idx=i;
          mu2_idx=j;
          mass_best_prefit=m;
          mass_best_postfit=m2;
        }
      }

    }
  }
  
  // we have the best muon candidate pair
  
  if(mass_best_postfit>2.5 && mass_best_postfit<3.6 ){
    // printf("Mass after=%f \n",m2);
    before->Fill(mass_best_prefit);
    after->Fill(mass_best_postfit);
  }
  
  Muon mu1_best( muonsH->at(mu1_idx) );
  Muon mu2_best( muonsH->at(mu2_idx) );
  // std::cout << mu1_best << " " << mu2_best << std::endl;

  OutPtr pOut( new MuonCollection() );

  pOut->push_back(mu1_best);
  pOut->push_back(mu2_best);
  iEvent.put( pOut );

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

// // ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
// void
// JpsiVertexFitExample::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // //The following says we do not know what parameters are allowed so do no validation
  // // Please change this to state exactly what you do use, even if it is no parameters
  // edm::ParameterSetDescription desc;
  // desc.setUnknown();
  // descriptions.addDefault(desc);
// }

//define this as a plug-in
DEFINE_FWK_MODULE(JpsiVertexFitExample);
