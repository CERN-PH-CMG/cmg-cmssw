#include "CMGTools/H2TauTau/plugins/RootTupleMakerV2_PFJets.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"

RootTupleMakerV2_PFJets::RootTupleMakerV2_PFJets(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize (iConfig.getParameter<unsigned int> ("MaxSize")),
    jecUncPath(iConfig.getParameter<std::string>("JECUncertainty")),
    readJECuncertainty (iConfig.getParameter<bool>   ("ReadJECuncertainty"))
  
    //    applyResJEC (iConfig.getParameter<bool>     ("ApplyResidualJEC")),
    //    resJEC (iConfig.getParameter<std::string>   ("ResidualJEC"))
{
  produces <std::vector<double> > ( prefix + "Eta" + suffix );
  produces <std::vector<double> > ( prefix + "Phi" + suffix );
  produces <std::vector<double> > ( prefix + "Pt" + suffix );
  produces <std::vector<double> > ( prefix + "PtRaw" + suffix );
  produces <std::vector<double> > ( prefix + "Energy" + suffix );
  produces <std::vector<double> > ( prefix + "EnergyRaw" + suffix );
  produces <std::vector<double> > ( prefix + "JECUnc" + suffix );
  produces <std::vector<double> > ( prefix + "ResJEC" + suffix );
  produces <std::vector<int> >    ( prefix + "PartonFlavour" + suffix );
  produces <std::vector<double> > ( prefix + "ChargedEmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "ChargedHadronEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "ChargedMuEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "ElectronEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "MuonEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "NeutralEmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "NeutralHadronEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "PhotonEnergyFraction"  + suffix );
  produces <std::vector<int> >    ( prefix + "ChargedHadronMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "ChargedMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "ElectronMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "MuonMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "NeutralHadronMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "NeutralMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "PhotonMultiplicity"  + suffix );
  produces <std::vector<int> >    ( prefix + "NConstituents"  + suffix );
  produces <std::vector<double> > ( prefix + "TrackCountingHighEffBTag" + suffix );
  produces <std::vector<double> > ( prefix + "TrackCountingHighPurBTag" + suffix );
  produces <std::vector<double> > ( prefix + "SimpleSecondaryVertexHighEffBTag" + suffix );
  produces <std::vector<double> > ( prefix + "SimpleSecondaryVertexHighPurBTag" + suffix );
  produces <std::vector<double> > ( prefix + "JetProbabilityBTag" + suffix );
  produces <std::vector<double> > ( prefix + "JetBProbabilityBTag" + suffix );
  produces <std::vector<int> >    ( prefix + "PassLooseID" + suffix);
  produces <std::vector<int> >    ( prefix + "PassTightID" + suffix);
}


PFJetIDSelectionFunctor pfjetIDLoose( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );
PFJetIDSelectionFunctor pfjetIDTight( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::TIGHT );

pat::strbitset retpf = pfjetIDLoose.getBitTemplate();

void RootTupleMakerV2_PFJets::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >  eta  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  phi  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt_raw  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  energy  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  energy_raw ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  jecUnc_vec ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  resJEC_vec ( new std::vector<double>()  );
  std::auto_ptr<std::vector<int> >     partonFlavour  ( new std::vector<int>()  );
  std::auto_ptr<std::vector<double> >  chargedEmEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chargedHadronEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  chargedMuEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  electronEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  muonEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  neutralEmEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  neutralHadronEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  photonEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >     chargedHadronMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     chargedMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     electronMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     muonMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     neutralHadronMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     neutralMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     photonMultiplicity  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >     nConstituents  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  trackCountingHighEffBTag  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  trackCountingHighPurBTag  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  simpleSecondaryVertexHighEffBTag  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  simpleSecondaryVertexHighPurBTag  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  jetProbabilityBTag  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  jetBProbabilityBTag  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<int> >  passLooseID  ( new std::vector<int>()  );
  std::auto_ptr<std::vector<int> >  passTightID  ( new std::vector<int>()  );

  //-----------------------------------------------------------------
  /*
  //OLD
  edm::FileInPath fipUnc(jecUncPath);;
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(fipUnc.fullPath());

  JetCorrectorParameters *ResJetCorPar = 0;
  FactorizedJetCorrector *JEC = 0;  
  if(applyResJEC) {
    edm::FileInPath fipRes(resJEC);
    ResJetCorPar = new JetCorrectorParameters(fipRes.fullPath());
    std::vector<JetCorrectorParameters> vParam;
    vParam.push_back(*ResJetCorPar);
    JEC = new FactorizedJetCorrector(vParam);
    }
  */

  //JEC Uncertainties 
  JetCorrectionUncertainty *jecUnc = 0;
  /*
  if(readJECuncertainty) {
      //(See https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1075/1.html 
      // and https://hypernews.cern.ch/HyperNews/CMS/get/physTools/2367/1.html)
      // handle the jet corrector parameters collection
      edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
      // get the jet corrector parameters collection from the global tag
      iSetup.get<JetCorrectionsRecord>().get(jecUncPath,JetCorParColl);
      // get the uncertainty parameters from the collection
      JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
      // instantiate the jec uncertainty object
      jecUnc = new JetCorrectionUncertainty(JetCorPar);
  }   
  */
  // typedef pat::Jet Jet;
  typedef cmg::PFJet Jet;
  typedef std::vector<Jet> JetCollection; 

  edm::Handle<JetCollection > jets;
  iEvent.getByLabel(inputTag, jets);

  if(jets.isValid()) {
    edm::LogInfo("RootTupleMakerV2_PFJetsInfo") << "Total # PFJets: " << jets->size();

    for( JetCollection::const_iterator it = jets->begin(); it != jets->end(); ++it ) {
      //Store only jets with pT>10 GeV

      const Jet& jet = *it;
      if ( jet.pt() < 10 ) continue;

      // exit from loop when you reach the required number of jets      
      if(eta->size() >= maxSize)
        break;

      retpf.set(false);
      int passjetLoose =0;
      //       if(pfjetIDLoose( *it, retpf )) passjetLoose =1;
      //COLIN, access the loose jet id strbitset
      if( jet.getSelection("cuts_looseJetId") ) passjetLoose =1;

      retpf.set(false);
      int passjetTight = 0;
      if( jet.getSelection("cuts_tightJetId") ) passjetTight =1;

      //OLD
      //     double corr = 1.;
      //      if( applyResJEC && iEvent.isRealData() ) {
      //        JEC->setJetEta( jet.eta() );
      //        JEC->setJetPt( jet.pt() ); // here you put the L2L3 Corrected jet pt
      //        corr = JEC->getCorrection();
      //      }
      
      if (readJECuncertainty) {
	jecUnc->setJetEta( jet.eta() );
	jecUnc->setJetPt( jet.pt() ); // the uncertainty is a function of the corrected pt
      }

      // fill in all the vectors
      eta->push_back( jet.eta() );
      phi->push_back( jet.phi() );
      pt->push_back( jet.pt() );
//       pt_raw->push_back( jet.correctedJet("Uncorrected").pt() );
      pt_raw->push_back( jet.pt() * jet.rawFactor() );
      energy->push_back( jet.energy() );
      energy_raw->push_back( jet.energy() * jet.rawFactor() );
      if(readJECuncertainty)
	jecUnc_vec->push_back( jecUnc->getUncertainty(true) );
      else 
	jecUnc_vec->push_back( -999 );
      partonFlavour->push_back( jet.partonFlavour() );

      
      chargedEmEnergyFraction->push_back( jet.component(reco::PFCandidate::e).fraction() );
      chargedHadronEnergyFraction->push_back( jet.component(reco::PFCandidate::h).fraction()  );
      chargedMuEnergyFraction->push_back( jet.component(reco::PFCandidate::mu).fraction() );
      //COLIN
      //       electronEnergyFraction->push_back( jet.electronEnergy()/jet.energy() );
      //       muonEnergyFraction->push_back( jet.muonEnergyFraction() );
      neutralEmEnergyFraction->push_back( jet.component(reco::PFCandidate::gamma).fraction() );
      neutralHadronEnergyFraction->push_back( jet.component(reco::PFCandidate::h0).fraction() );
      //COLIN
      //       photonEnergyFraction->push_back( jet.photonEnergyFraction() );
      chargedHadronMultiplicity->push_back( jet.component(reco::PFCandidate::h).number() );
      //COLIN 
      //       chargedMultiplicity->push_back( jet.chargedMultiplicity() );
      electronMultiplicity->push_back( jet.component(reco::PFCandidate::e).number() );
      muonMultiplicity->push_back( jet.component(reco::PFCandidate::mu).number() );
      chargedMultiplicity->push_back( jet.component(reco::PFCandidate::e).number() +
				      jet.component(reco::PFCandidate::mu).number() + 
				      jet.component(reco::PFCandidate::h).number() );

      neutralHadronMultiplicity->push_back( jet.component(reco::PFCandidate::h0).number() );
      //COLIN
      //       neutralMultiplicity->push_back( jet.neutralMultiplicity() );
      photonMultiplicity->push_back( jet.component(reco::PFCandidate::gamma).number() );
      nConstituents->push_back( jet.nConstituents() );
      trackCountingHighEffBTag->push_back( jet.bDiscriminator("trackCountingHighEffBJetTags") );
      trackCountingHighPurBTag->push_back( jet.bDiscriminator("trackCountingHighPurBJetTags") );
      simpleSecondaryVertexHighEffBTag->push_back( jet.bDiscriminator("simpleSecondaryVertexHighEffBJetTags") );
      simpleSecondaryVertexHighPurBTag->push_back( jet.bDiscriminator("simpleSecondaryVertexHighPurBJetTags") );
      jetProbabilityBTag->push_back( jet.bDiscriminator("jetProbabilityBJetTags") );
      jetBProbabilityBTag->push_back( jet.bDiscriminator("jetBProbabilityBJetTags") );
      passLooseID->push_back( passjetLoose );
      passTightID->push_back( passjetTight );
    }
  } else {
    edm::LogError("RootTupleMakerV2_PFJetsError") << "Error! Can't get the product " << inputTag;
  }

  delete jecUnc;
  //  delete ResJetCorPar;
  //  delete JEC;

  //-----------------------------------------------------------------
  // put vectors in the event
  iEvent.put( eta, prefix + "Eta" + suffix );
  iEvent.put( phi, prefix + "Phi" + suffix );
  iEvent.put( pt, prefix + "Pt" + suffix );
  iEvent.put( pt_raw, prefix + "PtRaw" + suffix );
  iEvent.put( energy, prefix + "Energy" + suffix );
  iEvent.put( energy_raw, prefix + "EnergyRaw" + suffix );
  iEvent.put( jecUnc_vec, prefix + "JECUnc" + suffix );
  iEvent.put( resJEC_vec, prefix + "ResJEC" + suffix );
  iEvent.put( partonFlavour, prefix + "PartonFlavour" + suffix );
  iEvent.put( chargedEmEnergyFraction,  prefix + "ChargedEmEnergyFraction"  + suffix );
  iEvent.put( chargedHadronEnergyFraction,  prefix + "ChargedHadronEnergyFraction"  + suffix );
  iEvent.put( chargedMuEnergyFraction,  prefix + "ChargedMuEnergyFraction"  + suffix );
  iEvent.put( electronEnergyFraction,  prefix + "ElectronEnergyFraction"  + suffix );
  iEvent.put( muonEnergyFraction,  prefix + "MuonEnergyFraction"  + suffix );
  iEvent.put( neutralEmEnergyFraction,  prefix + "NeutralEmEnergyFraction"  + suffix );
  iEvent.put( neutralHadronEnergyFraction,  prefix + "NeutralHadronEnergyFraction"  + suffix );
  iEvent.put( photonEnergyFraction,  prefix + "PhotonEnergyFraction"  + suffix );
  iEvent.put( chargedHadronMultiplicity,  prefix + "ChargedHadronMultiplicity"  + suffix );
  iEvent.put( chargedMultiplicity,  prefix + "ChargedMultiplicity"  + suffix );
  iEvent.put( electronMultiplicity,  prefix + "ElectronMultiplicity"  + suffix );
  iEvent.put( muonMultiplicity,  prefix + "MuonMultiplicity"  + suffix );
  iEvent.put( neutralHadronMultiplicity,  prefix + "NeutralHadronMultiplicity"  + suffix );
  iEvent.put( neutralMultiplicity,  prefix + "NeutralMultiplicity"  + suffix );
  iEvent.put( photonMultiplicity,  prefix + "PhotonMultiplicity"  + suffix );
  iEvent.put( nConstituents,  prefix + "NConstituents"  + suffix );
  iEvent.put( trackCountingHighEffBTag, prefix + "TrackCountingHighEffBTag" + suffix );
  iEvent.put( trackCountingHighPurBTag, prefix + "TrackCountingHighPurBTag" + suffix );
  iEvent.put( simpleSecondaryVertexHighEffBTag, prefix + "SimpleSecondaryVertexHighEffBTag" + suffix );
  iEvent.put( simpleSecondaryVertexHighPurBTag, prefix + "SimpleSecondaryVertexHighPurBTag" + suffix );
  iEvent.put( jetProbabilityBTag, prefix + "JetProbabilityBTag" + suffix );
  iEvent.put( jetBProbabilityBTag, prefix + "JetBProbabilityBTag" + suffix );
  iEvent.put( passLooseID, prefix + "PassLooseID" + suffix);
  iEvent.put( passTightID, prefix + "PassTightID" + suffix);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RootTupleMakerV2_PFJets);
