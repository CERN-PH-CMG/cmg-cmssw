// imported from RecoEgamma/ElectronIdentification/plugins/ElectronMVAEstimatorRun2Spring15NonTrig.cc and renamed, with modifications to run in FWLite

#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimatorSpring15NonTrig.h"
#include <TFile.h>
#include "TMath.h"
#include <cmath>
#include <vector>
#include <cstdio>
#include <zlib.h>
#include "TMVA/MethodBase.h"
#include "TMVA/MethodBDT.h"

EGammaMvaEleEstimatorSpring15NonTrig::EGammaMvaEleEstimatorSpring15NonTrig(){}

void EGammaMvaEleEstimatorSpring15NonTrig::initialize(std::string methodName, std::vector<std::string> weightFileNames){

  _MethodName = methodName;

  if( (int)(weightFileNames.size()) != nCategories )
    throw cms::Exception("MVA config failure: ")
      << "wrong number of weightfiles" << std::endl;

  _gbrForests.clear();

  // Create a TMVA reader object for each category
  for(int i=0; i<nCategories; i++){

    // Use unique_ptr so that all readers are properly cleaned up
    // when the vector clear() is called in the destructor

    _gbrForests.push_back( createSingleReader(i, weightFileNames[i] ) );

  }

}

EGammaMvaEleEstimatorSpring15NonTrig::
~EGammaMvaEleEstimatorSpring15NonTrig(){
}

float EGammaMvaEleEstimatorSpring15NonTrig::
mvaValue( const pat::Electron& particle, const fwlite::EventBase& eventbase, bool printDebug) const {
  
  // beamspot
  edm::InputTag beamSpotTag("offlineBeamSpot");
  edm::Handle<reco::BeamSpot> beamspot;
  eventbase.getByLabel(beamSpotTag, beamspot);

  // conversions
  edm::InputTag convTag("reducedEgamma:reducedConversions");
  edm::Handle<reco::ConversionCollection> conversions;
  eventbase.getByLabel(convTag, conversions);

  const int iCategory = findCategory( particle );
  const std::vector<float> vars = std::move( fillMVAVariables( particle, *(beamspot.product()), conversions ) );  
  const float result = _gbrForests.at(iCategory)->GetClassifier(vars.data());

  if(printDebug) {
    std::cout << " *** Inside the class _MethodName " << _MethodName << std::endl;
    std::cout << " bin " << iCategory
	      << " fbrem " <<  vars[11]
	      << " kfchi2 " << vars[9]
	      << " mykfhits " << vars[8]
	      << " gsfchi2 " << vars[10]
	      << " deta " <<  vars[18]
	      << " dphi " << vars[19]
	      << " detacalo " << vars[20]
	      << " see " << vars[0]
	      << " spp " << vars[1]
	      << " etawidth " << vars[4]
	      << " phiwidth " << vars[5]
	      << " OneMinusE1x5E5x5 " << vars[2]
	      << " R9 " << vars[3]
	      << " HoE " << vars[6]
	      << " EoP " << vars[15]
	      << " IoEmIoP " << vars[17]
	      << " eleEoPout " << vars[16]
	      << " eta " << vars[24]
	      << " pt " << vars[21] << std::endl;
    std::cout << " ### MVA " << result << std::endl;
  }

  return result;
}

int EGammaMvaEleEstimatorSpring15NonTrig::findCategory( const pat::Electron& particle) const {
  
  float pt = particle.pt();
  float eta = particle.superCluster()->eta();

  //
  // Determine the category
  //
  int  iCategory = UNDEFINED;
  const float ptSplit = 10;   // we have above and below 10 GeV categories
  const float ebSplit = 0.800;// barrel is split into two regions
  const float ebeeSplit = 1.479; // division between barrel and endcap

  if (pt < ptSplit && std::abs(eta) < ebSplit)  
    iCategory = CAT_EB1_PT5to10;

  if (pt < ptSplit && std::abs(eta) >= ebSplit && std::abs(eta) < ebeeSplit)
    iCategory = CAT_EB2_PT5to10;

  if (pt < ptSplit && std::abs(eta) >= ebeeSplit) 
    iCategory = CAT_EE_PT5to10;

  if (pt >= ptSplit && std::abs(eta) < ebSplit) 
    iCategory = CAT_EB1_PT10plus;

  if (pt >= ptSplit && std::abs(eta) >= ebSplit && std::abs(eta) < ebeeSplit)
    iCategory = CAT_EB2_PT10plus;

  if (pt >= ptSplit && std::abs(eta) >= ebeeSplit) 
    iCategory = CAT_EE_PT10plus;
  
  return iCategory;
}

bool EGammaMvaEleEstimatorSpring15NonTrig::
isEndcapCategory(int category ) const {

  bool isEndcap = false;
  if( category == CAT_EE_PT5to10 || category == CAT_EE_PT10plus )
    isEndcap = true;

  return isEndcap;
}


std::unique_ptr<const GBRForest> EGammaMvaEleEstimatorSpring15NonTrig::
createSingleReader(const int iCategory, const std::string weightFile){

  //
  // Create the reader  
  //
  TMVA::Reader tmpTMVAReader( "!Color:Silent:!Error" );

  //
  // Configure all variables and spectators. Note: the order and names
  // must match what is found in the xml weights file!
  //
  // Pure ECAL -> shower shapes
  tmpTMVAReader.AddVariable("ele_oldsigmaietaieta", &_allMVAVars.see);
  tmpTMVAReader.AddVariable("ele_oldsigmaiphiiphi", &_allMVAVars.spp);
  tmpTMVAReader.AddVariable("ele_oldcircularity",   &_allMVAVars.OneMinusE1x5E5x5);
  tmpTMVAReader.AddVariable("ele_oldr9",            &_allMVAVars.R9);
  tmpTMVAReader.AddVariable("ele_scletawidth",      &_allMVAVars.etawidth);
  tmpTMVAReader.AddVariable("ele_sclphiwidth",      &_allMVAVars.phiwidth);
  tmpTMVAReader.AddVariable("ele_he",               &_allMVAVars.HoE);
  // Endcap only variables
  if( isEndcapCategory(iCategory) )
    tmpTMVAReader.AddVariable("ele_psEoverEraw",    &_allMVAVars.PreShowerOverRaw);
  
  //Pure tracking variables
  tmpTMVAReader.AddVariable("ele_kfhits",           &_allMVAVars.kfhits);
  tmpTMVAReader.AddVariable("ele_kfchi2",           &_allMVAVars.kfchi2);
  tmpTMVAReader.AddVariable("ele_gsfchi2",        &_allMVAVars.gsfchi2);

  // Energy matching
  tmpTMVAReader.AddVariable("ele_fbrem",           &_allMVAVars.fbrem);

  tmpTMVAReader.AddVariable("ele_gsfhits",         &_allMVAVars.gsfhits);
  tmpTMVAReader.AddVariable("ele_expected_inner_hits",             &_allMVAVars.expectedMissingInnerHits);
  tmpTMVAReader.AddVariable("ele_conversionVertexFitProbability",  &_allMVAVars.convVtxFitProbability);

  tmpTMVAReader.AddVariable("ele_ep",              &_allMVAVars.EoP);
  tmpTMVAReader.AddVariable("ele_eelepout",        &_allMVAVars.eleEoPout);
  tmpTMVAReader.AddVariable("ele_IoEmIop",         &_allMVAVars.IoEmIoP);
  
  // Geometrical matchings
  tmpTMVAReader.AddVariable("ele_deltaetain",      &_allMVAVars.deta);
  tmpTMVAReader.AddVariable("ele_deltaphiin",      &_allMVAVars.dphi);
  tmpTMVAReader.AddVariable("ele_deltaetaseed",    &_allMVAVars.detacalo);
  
  // Spectator variables  
  tmpTMVAReader.AddSpectator("ele_pT",             &_allMVAVars.pt);
  tmpTMVAReader.AddSpectator("ele_isbarrel",       &_allMVAVars.isBarrel);
  tmpTMVAReader.AddSpectator("ele_isendcap",       &_allMVAVars.isEndcap);
  tmpTMVAReader.AddSpectator("scl_eta",            &_allMVAVars.SCeta);

  tmpTMVAReader.AddSpectator("ele_eClass",                 &_allMVAVars.eClass);
  tmpTMVAReader.AddSpectator("ele_pfRelIso",               &_allMVAVars.pfRelIso);
  tmpTMVAReader.AddSpectator("ele_expected_inner_hits",    &_allMVAVars.expectedInnerHits);
  tmpTMVAReader.AddSpectator("ele_vtxconv",                &_allMVAVars.vtxconv);
  tmpTMVAReader.AddSpectator("mc_event_weight",            &_allMVAVars.mcEventWeight);
  tmpTMVAReader.AddSpectator("mc_ele_CBmatching_category", &_allMVAVars.mcCBmatchingCategory);

  //
  // Book the method and set up the weights file
  //
  std::unique_ptr<TMVA::IMethod> temp( tmpTMVAReader.BookMVA(_MethodName , weightFile ) );

  return std::unique_ptr<const GBRForest> ( new GBRForest( dynamic_cast<TMVA::MethodBDT*>( tmpTMVAReader.FindMVA(_MethodName) ) ) );
}

// A function that should work on both pat and reco objects
std::vector<float> EGammaMvaEleEstimatorSpring15NonTrig::
fillMVAVariables(const pat::Electron& particle, const reco::BeamSpot& theBeamSpot, const edm::Handle<reco::ConversionCollection>& conversions) const {

  // Both pat and reco particles have exactly the same accessors, so we use a reco ptr 
  // throughout the code, with a single exception as of this writing, handled separately below.
  auto superCluster = particle.superCluster();
  
  AllVariables allMVAVars;

  // Pure ECAL -> shower shapes
  allMVAVars.see            = particle.full5x5_sigmaIetaIeta();
  allMVAVars.spp            = particle.full5x5_sigmaIphiIphi();
  allMVAVars.OneMinusE1x5E5x5 = 1. - particle.full5x5_e1x5() / particle.full5x5_e5x5();
  allMVAVars.R9             = particle.full5x5_r9();
  allMVAVars.etawidth       = superCluster->etaWidth();
  allMVAVars.phiwidth       = superCluster->phiWidth();
  allMVAVars.HoE            = particle.hadronicOverEm();
  // Endcap only variables
  allMVAVars.PreShowerOverRaw  = superCluster->preshowerEnergy() / superCluster->rawEnergy();

  // To get to CTF track information in pat::Electron, we have to have the pointer
  // to pat::Electron, it is not accessible from the pointer to reco::GsfElectron.
  // This behavior is reported and is expected to change in the future (post-7.4.5 some time).
  bool validKF= false; 
  reco::TrackRef myTrackRef = particle.closestCtfTrackRef();
  validKF = (myTrackRef.isAvailable() && (myTrackRef.isNonnull()) );  

  //Pure tracking variables
  allMVAVars.kfhits         = (validKF) ? myTrackRef->hitPattern().trackerLayersWithMeasurement() : -1. ;
  allMVAVars.kfchi2          = (validKF) ? myTrackRef->normalizedChi2() : 0;
  allMVAVars.gsfchi2         = particle.gsfTrack()->normalizedChi2();

  // Energy matching
  allMVAVars.fbrem           = particle.fbrem();

  allMVAVars.gsfhits         = particle.gsfTrack()->hitPattern().trackerLayersWithMeasurement();
  allMVAVars.expectedMissingInnerHits = particle.gsfTrack()
    ->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);

  reco::ConversionRef conv_ref = ConversionTools::matchedConversion(particle,
								    conversions, 
								    theBeamSpot.position());
  double vertexFitProbability = -1.; 
  if(!conv_ref.isNull()) {
    const reco::Vertex &vtx = conv_ref.get()->conversionVertex(); if (vtx.isValid()) {
      vertexFitProbability = TMath::Prob( vtx.chi2(), vtx.ndof());
    } 
  }
  allMVAVars.convVtxFitProbability    = vertexFitProbability;

  allMVAVars.EoP             = particle.eSuperClusterOverP();
  allMVAVars.eleEoPout       = particle.eEleClusterOverPout();
  float pAtVertex            = particle.trackMomentumAtVtx().R();
  allMVAVars.IoEmIoP         = (1.0/particle.ecalEnergy()) - (1.0 / pAtVertex );

  // Geometrical matchings
  allMVAVars.deta            = particle.deltaEtaSuperClusterTrackAtVtx();
  allMVAVars.dphi            = particle.deltaPhiSuperClusterTrackAtVtx();
  allMVAVars.detacalo        = particle.deltaEtaSeedClusterTrackAtCalo();

  // Spectator variables  
  allMVAVars.pt              = particle.pt();
  float scEta = superCluster->eta();
  constexpr float ebeeSplit = 1.479;
  allMVAVars.isBarrel        = ( std::abs(scEta) < ebeeSplit );
  allMVAVars.isEndcap        = ( std::abs(scEta) >= ebeeSplit );
  allMVAVars.SCeta           = scEta;
  // The spectator variables below were examined for training, but
  // are not necessary for evaluating the discriminator, so they are
  // given dummy values (the specator variables above are also unimportant).
  // They are introduced only to match the definition of the discriminator 
  // in the weights file.
  constexpr unsigned nines = 999;
  allMVAVars.eClass               = nines;
  allMVAVars.pfRelIso             = nines;
  allMVAVars.expectedInnerHits    = nines;
  allMVAVars.vtxconv              = nines;
  allMVAVars.mcEventWeight        = nines;
  allMVAVars.mcCBmatchingCategory = nines;

  constrainMVAVariables(allMVAVars);

  std::vector<float> vars;

  if( isEndcapCategory( findCategory( particle ) ) ) {
    vars.push_back(allMVAVars.see);
    vars.push_back(allMVAVars.spp);
    vars.push_back(allMVAVars.OneMinusE1x5E5x5);
    vars.push_back(allMVAVars.R9);
    vars.push_back(allMVAVars.etawidth);
    vars.push_back(allMVAVars.phiwidth);
    vars.push_back(allMVAVars.HoE);
    // Endcap only variables
    vars.push_back(allMVAVars.PreShowerOverRaw);                                       
    //Pure tracking variables
    vars.push_back(allMVAVars.kfhits);
    vars.push_back(allMVAVars.kfchi2);
    vars.push_back(allMVAVars.gsfchi2);                                       
    // Energy matching
    vars.push_back(allMVAVars.fbrem);                                       
    vars.push_back(allMVAVars.gsfhits);
    vars.push_back(allMVAVars.expectedMissingInnerHits);
    vars.push_back(allMVAVars.convVtxFitProbability);
    vars.push_back(allMVAVars.EoP);
    vars.push_back(allMVAVars.eleEoPout);
    vars.push_back(allMVAVars.IoEmIoP);                                       
    // Geometrical matchings
    vars.push_back(allMVAVars.deta);
    vars.push_back(allMVAVars.dphi);
    vars.push_back(allMVAVars.detacalo);                                       
    // Spectator variables  
    vars.push_back(allMVAVars.pt);
    vars.push_back(allMVAVars.isBarrel);
    vars.push_back(allMVAVars.isEndcap);
    vars.push_back(allMVAVars.SCeta);                                       
    vars.push_back(allMVAVars.eClass);
    vars.push_back(allMVAVars.pfRelIso);
    vars.push_back(allMVAVars.expectedInnerHits);
    vars.push_back(allMVAVars.vtxconv);
    vars.push_back(allMVAVars.mcEventWeight);
    vars.push_back(allMVAVars.mcCBmatchingCategory);
  } else {
    vars.push_back(allMVAVars.see);
    vars.push_back(allMVAVars.spp);
    vars.push_back(allMVAVars.OneMinusE1x5E5x5);
    vars.push_back(allMVAVars.R9);
    vars.push_back(allMVAVars.etawidth);
    vars.push_back(allMVAVars.phiwidth);
    vars.push_back(allMVAVars.HoE);
    //Pure tracking variables
    vars.push_back(allMVAVars.kfhits);
    vars.push_back(allMVAVars.kfchi2);
    vars.push_back(allMVAVars.gsfchi2);                                       
    // Energy matching
    vars.push_back(allMVAVars.fbrem);                                       
    vars.push_back(allMVAVars.gsfhits);
    vars.push_back(allMVAVars.expectedMissingInnerHits);
    vars.push_back(allMVAVars.convVtxFitProbability);
    vars.push_back(allMVAVars.EoP);
    vars.push_back(allMVAVars.eleEoPout);
    vars.push_back(allMVAVars.IoEmIoP);                                       
    // Geometrical matchings
    vars.push_back(allMVAVars.deta);
    vars.push_back(allMVAVars.dphi);
    vars.push_back(allMVAVars.detacalo);                                       
    // Spectator variables  
    vars.push_back(allMVAVars.pt);
    vars.push_back(allMVAVars.isBarrel);
    vars.push_back(allMVAVars.isEndcap);
    vars.push_back(allMVAVars.SCeta);                                       
    vars.push_back(allMVAVars.eClass);
    vars.push_back(allMVAVars.pfRelIso);
    vars.push_back(allMVAVars.expectedInnerHits);
    vars.push_back(allMVAVars.vtxconv);
    vars.push_back(allMVAVars.mcEventWeight);
    vars.push_back(allMVAVars.mcCBmatchingCategory);
  }
  return vars;
}

void EGammaMvaEleEstimatorSpring15NonTrig::constrainMVAVariables(AllVariables& allMVAVars) const {

  // Check that variables do not have crazy values

  if(allMVAVars.fbrem < -1.)
    allMVAVars.fbrem = -1.;
  
  allMVAVars.deta = fabs(allMVAVars.deta);
  if(allMVAVars.deta > 0.06)
    allMVAVars.deta = 0.06;
  
  
  allMVAVars.dphi = fabs(allMVAVars.dphi);
  if(allMVAVars.dphi > 0.6)
    allMVAVars.dphi = 0.6;
  

  if(allMVAVars.EoP > 20.)
    allMVAVars.EoP = 20.;
  
  if(allMVAVars.eleEoPout > 20.)
    allMVAVars.eleEoPout = 20.;
  
  
  allMVAVars.detacalo = fabs(allMVAVars.detacalo);
  if(allMVAVars.detacalo > 0.2)
    allMVAVars.detacalo = 0.2;
  
  if(allMVAVars.OneMinusE1x5E5x5 < -1.)
    allMVAVars.OneMinusE1x5E5x5 = -1;
  
  if(allMVAVars.OneMinusE1x5E5x5 > 2.)
    allMVAVars.OneMinusE1x5E5x5 = 2.; 
  
  
  
  if(allMVAVars.R9 > 5)
    allMVAVars.R9 = 5;
  
  if(allMVAVars.gsfchi2 > 200.)
    allMVAVars.gsfchi2 = 200;
  
  
  if(allMVAVars.kfchi2 > 10.)
    allMVAVars.kfchi2 = 10.;
  

}

