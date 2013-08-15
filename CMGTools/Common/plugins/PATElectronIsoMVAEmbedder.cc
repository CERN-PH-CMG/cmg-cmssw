#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"
#include "EGamma/EGammaAnalysisTools/interface/EGammaMvaEleEstimator.h"

#include "Utilities/General/interface/FileInPath.h"



using namespace std;

class PATElectronIsoMVAEmbedder : public edm::EDProducer{
 public:
  PATElectronIsoMVAEmbedder(const edm::ParameterSet& ps);
  virtual ~PATElectronIsoMVAEmbedder();

  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:

  edm::InputTag src_;
  edm::InputTag srcRho_;
  edm::InputTag srcPF_;
  edm::InputTag srcMuons_;
  edm::InputTag srcElectrons_;
  edm::InputTag srcVertices_;
  
  EGammaMvaEleEstimator * fElectronIsoMVA_ ;


};

PATElectronIsoMVAEmbedder::PATElectronIsoMVAEmbedder(const edm::ParameterSet& ps):
  src_(ps.getParameter<edm::InputTag>("src")), 
  srcRho_(ps.getParameter<edm::InputTag>("srcRho")), 
  srcPF_(ps.getParameter<edm::InputTag>("srcPF")), 
  srcMuons_(ps.getParameter<edm::InputTag>("srcSelectedMuons")), 
  srcElectrons_(ps.getParameter<edm::InputTag>("srcSelectedElectrons")), 
  srcVertices_(ps.getParameter<edm::InputTag>("srcVertices")) 
{

  fElectronIsoMVA_ = new EGammaMvaEleEstimator () ;

  vector<string> eleiso_weightfiles ;

  edm::FileInPath a("EGamma/EGammaAnalysisToolsSiXie/data/ElectronIso_BDTG_V0_EndcapPt5To10.weights.xml");
  edm::FileInPath b("EGamma/EGammaAnalysisToolsSiXie/data/ElectronIso_BDTG_V0_EndcapPt5To10.weights.xml");
  edm::FileInPath c("EGamma/EGammaAnalysisToolsSiXie/data/ElectronIso_BDTG_V0_BarrelPt10ToInf.weights.xml");
  edm::FileInPath d("EGamma/EGammaAnalysisToolsSiXie/data/ElectronIso_BDTG_V0_EndcapPt10ToInf.weights.xml");

  eleiso_weightfiles.push_back (a.fullPath ()) ;
  eleiso_weightfiles.push_back (b.fullPath ()) ;
  eleiso_weightfiles.push_back (c.fullPath ()) ;
  eleiso_weightfiles.push_back (d.fullPath ()) ;

  fElectronIsoMVA_->initialize ("EleIso_BDTG_IsoRings",
                                 EGammaMvaEleEstimator::kIsoRings,
                                 true,
                                 eleiso_weightfiles) ;


  produces<pat::ElectronCollection>();

}


PATElectronIsoMVAEmbedder::~PATElectronIsoMVAEmbedder() {

}


void PATElectronIsoMVAEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<pat::ElectronCollection> src;
  iEvent.getByLabel(src_,src);
  
  edm::Handle<pat::ElectronCollection> srcElectrons;
  iEvent.getByLabel(srcElectrons_,srcElectrons);
  
  edm::Handle< reco::MuonCollection > srcMuons;
  iEvent.getByLabel(srcMuons_,srcMuons);
  // const reco::MuonCollection& selMuons = *srcMuons;
  
  edm::Handle<reco::PFCandidateCollection> srcPF;
  iEvent.getByLabel(srcPF_,srcPF);

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(srcVertices_,vertices);

  edm::Handle<double> rho;
  iEvent.getByLabel(srcRho_,rho);

  std::auto_ptr<pat::ElectronCollection> output(new pat::ElectronCollection) ;

  reco::Vertex dummy ;
  const reco::Vertex *pv = &dummy ;
  if (vertices->size () != 0) 
    {
      pv = &*vertices->begin () ;
    } 
      else 
    { // create a dummy PV
      reco::Vertex::Error e ;
      e (0, 0) = 0.0015 * 0.0015 ;
      e (1, 1) = 0.0015 * 0.0015 ;
      e (2, 2) = 15. * 15. ;
      reco::Vertex::Point p (0, 0, 0) ;
      dummy = reco::Vertex (p, e, 0, 0, 0) ;
    }


  for(unsigned int i=0;i<src->size();++i) {
    pat::Electron electron = src->at(i);

    Double_t tmpChargedIso_DR0p0To0p1  = 0 ;
    Double_t tmpChargedIso_DR0p1To0p2  = 0 ;
    Double_t tmpChargedIso_DR0p2To0p3  = 0 ;
    Double_t tmpChargedIso_DR0p3To0p4  = 0 ;
    Double_t tmpChargedIso_DR0p4To0p5  = 0 ;
    Double_t tmpGammaIso_DR0p0To0p1  = 0 ;
    Double_t tmpGammaIso_DR0p1To0p2  = 0 ;
    Double_t tmpGammaIso_DR0p2To0p3  = 0 ;
    Double_t tmpGammaIso_DR0p3To0p4  = 0 ;
    Double_t tmpGammaIso_DR0p4To0p5  = 0 ;
    Double_t tmpNeutralHadronIso_DR0p0To0p1  = 0 ;
    Double_t tmpNeutralHadronIso_DR0p1To0p2  = 0 ;
    Double_t tmpNeutralHadronIso_DR0p2To0p3  = 0 ;
    Double_t tmpNeutralHadronIso_DR0p3To0p4  = 0 ;
    Double_t tmpNeutralHadronIso_DR0p4To0p5  = 0 ;

    double electronTrackZ = 0 ;
    if (electron.gsfTrack ().isNonnull ()) 
      {
        electronTrackZ = electron.gsfTrack ()->dz (pv->position ()) ;
      } 
        else if (electron.closestCtfTrackRef ().isNonnull ()) 
      {
        electronTrackZ = electron.closestCtfTrackRef ()->dz (pv->position ()) ;
      }

    //PG loop over PFcandidates
    for (reco::PFCandidateCollection::const_iterator iCand = srcPF->begin () ; 
         iCand != srcPF->end () ; ++iCand) 
      {
        //exclude the electron itself
        if (iCand->gsfTrackRef ().isNonnull () && electron.gsfTrack ().isNonnull () &&
           refToPtr (iCand->gsfTrackRef ()) == refToPtr (electron.gsfTrack ())) continue ;
        if (iCand->trackRef ().isNonnull () && electron.closestCtfTrackRef ().isNonnull () &&
           refToPtr (iCand->trackRef ()) == refToPtr (electron.closestCtfTrackRef ())) continue ;      

        //
        // New Isolation Calculations
        //
        //PG FIXME speed up use squared distances
        double dr = sqrt (pow (iCand->eta () - electron.eta (),2) 
                          + pow (acos (cos (iCand->phi () - electron.phi ())),2)) ;
        //Double_t deta = (iCand->eta () - ele.eta ()) ;
        if (dr >= 1.0) continue ;
        Bool_t IsLeptonFootprint = kFALSE ;

        //
        // Lepton Footprint Removal
        //   
        //PG FIXME speed up with breaks at the first setting of the true condition
        //PG FIXME speed up use squared distances
        for (pat::ElectronCollection::const_iterator i_IDEle = srcElectrons->begin () ; 
             i_IDEle != srcElectrons->end () ; ++i_IDEle) 
          {
            //if pf candidate matches an electron passing ID cuts, then veto it
            if (iCand->gsfTrackRef ().isNonnull () && i_IDEle->gsfTrack ().isNonnull ()
                && refToPtr (iCand->gsfTrackRef ()) == refToPtr (i_IDEle->gsfTrack ())) IsLeptonFootprint = kTRUE ;
            if (iCand->trackRef ().isNonnull () && i_IDEle->closestCtfTrackRef ().isNonnull ()
                && refToPtr (iCand->trackRef ()) == refToPtr (i_IDEle->closestCtfTrackRef ())) IsLeptonFootprint = kTRUE ;

            //if pf candidate lies in veto regions of electron passing ID cuts, then veto it
            double tmpDR = sqrt (pow (iCand->eta () - i_IDEle->eta (),2) + pow (acos (cos (iCand->phi () - i_IDEle->phi ())),2)) ;
            if (iCand->trackRef ().isNonnull () //PG charged
                && fabs (i_IDEle->superCluster ()->eta ()) >= 1.479 
                && tmpDR < 0.015) IsLeptonFootprint = kTRUE ;
            if (iCand->particleId () == reco::PFCandidate::gamma //PG photons
                && fabs (i_IDEle->superCluster ()->eta ()) >= 1.479 
                && tmpDR < 0.08) IsLeptonFootprint = kTRUE ;
          }
//        for (reco::MuonCollection::const_iterator i_ID_Mu = selMuons.begin () ; 
//             i_ID_Mu != selMuons.end () ; ++i_ID_Mu) 
//          {
//            //if pf candidate matches an muon passing ID cuts, then veto it
//            if (iCand->trackRef ().isNonnull () 
//                && i_ID_Mu->innerTrack ().isNonnull () 
//                && refToPtr (iCand->trackRef ()) == refToPtr (i_ID_Mu->innerTrack ())) IsLeptonFootprint = kTRUE ;
//
//            //if pf candidate lies in veto regions of muon passing ID cuts, then veto it
//            double tmpDR = sqrt (pow (iCand->eta () - i_ID_Mu->eta (),2) + pow (acos (cos (iCand->phi () - i_ID_Mu->phi ())),2)) ;
//            if (iCand->trackRef ().isNonnull () 
//                && tmpDR < 0.01) IsLeptonFootprint = kTRUE ;
//          }

        if (IsLeptonFootprint) continue ;

        Bool_t passVeto = kTRUE ;
        //Charged
        if (iCand->trackRef ().isNonnull ()) 
          {             
            if (! (fabs (iCand->trackRef ()->dz (pv->position ()) - electronTrackZ) < 0.2)) passVeto = kFALSE ;

            // Veto any PFmuon, or PFEle
            if (iCand->particleId () == reco::PFCandidate::e 
                || iCand->particleId () == reco::PFCandidate::mu) passVeto = kFALSE ;

            // Footprint Veto
            if (fabs (electron.eta ()) > 1.479 && dr < 0.015) passVeto = kFALSE ;
            if (passVeto) 
              {
                if (dr < 0.1) tmpChargedIso_DR0p0To0p1 += iCand->pt () ;
                if (dr >= 0.1 && dr < 0.2) tmpChargedIso_DR0p1To0p2 += iCand->pt () ;
                if (dr >= 0.2 && dr < 0.3) tmpChargedIso_DR0p2To0p3 += iCand->pt () ;
                if (dr >= 0.3 && dr < 0.4) tmpChargedIso_DR0p3To0p4 += iCand->pt () ;
                if (dr >= 0.4 && dr < 0.5) tmpChargedIso_DR0p4To0p5 += iCand->pt () ;
              } //pass veto       
          }
        //Gamma
        else if (iCand->particleId () == reco::PFCandidate::gamma) 
          {
            // Footprint Veto
            if (fabs (electron.eta ()) > 1.479 && dr < 0.08) passVeto = kFALSE ;

            if (passVeto) 
              {
                if (dr < 0.1) tmpGammaIso_DR0p0To0p1 += iCand->pt () ;
                if (dr >= 0.1 && dr < 0.2) tmpGammaIso_DR0p1To0p2 += iCand->pt () ;
                if (dr >= 0.2 && dr < 0.3) tmpGammaIso_DR0p2To0p3 += iCand->pt () ;
                if (dr >= 0.3 && dr < 0.4) tmpGammaIso_DR0p3To0p4 += iCand->pt () ;
                if (dr >= 0.4 && dr < 0.5) tmpGammaIso_DR0p4To0p5 += iCand->pt () ;
              }
          }
        //NeutralHadron
        else 
          {
            if (dr < 0.1) tmpNeutralHadronIso_DR0p0To0p1 += iCand->pt () ;
            if (dr >= 0.1 && dr < 0.2) tmpNeutralHadronIso_DR0p1To0p2 += iCand->pt () ;
            if (dr >= 0.2 && dr < 0.3) tmpNeutralHadronIso_DR0p2To0p3 += iCand->pt () ;
            if (dr >= 0.3 && dr < 0.4) tmpNeutralHadronIso_DR0p3To0p4 += iCand->pt () ;
            if (dr >= 0.4 && dr < 0.5) tmpNeutralHadronIso_DR0p4To0p5 += iCand->pt () ;
          }
      } //loop over PF candidates

//      kEleEANoCorr,
//      kEleEAData2011,
//      kEleEASummer11MC,
//      kEleEAFall11MC

    double isomva = fElectronIsoMVA_->isoMvaValue (electron.pt (),
                                    electron.eta (),
                                    *rho,
                                    ElectronEffectiveArea::kEleEAData2011,
                                    tmpChargedIso_DR0p0To0p1,
                                    tmpChargedIso_DR0p1To0p2,
                                    tmpChargedIso_DR0p2To0p3,
                                    tmpChargedIso_DR0p3To0p4,
                                    tmpChargedIso_DR0p4To0p5,
                                    tmpGammaIso_DR0p0To0p1,
                                    tmpGammaIso_DR0p1To0p2,
                                    tmpGammaIso_DR0p2To0p3,
                                    tmpGammaIso_DR0p3To0p4,
                                    tmpGammaIso_DR0p4To0p5,
                                    tmpNeutralHadronIso_DR0p0To0p1,
                                    tmpNeutralHadronIso_DR0p1To0p2,
                                    tmpNeutralHadronIso_DR0p2To0p3,
                                    tmpNeutralHadronIso_DR0p3To0p4,
                                    tmpNeutralHadronIso_DR0p4To0p5,
                                    0) ; // print debug


    electron.addUserFloat("mvaIsoRings",isomva);

    //    printf("Test=%f\n",muon.userFloat("mvaIsoRings"));
    output->push_back(electron);
  }

  iEvent.put( output );
}


DEFINE_FWK_MODULE(PATElectronIsoMVAEmbedder);

