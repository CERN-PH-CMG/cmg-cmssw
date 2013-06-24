//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     Thu May 31 21:49:25 2012 by mkntanalyzer.py
// Author:      Andreas Hinzmann
//-----------------------------------------------------------------------------
#include "substructure_pas.h"

#ifdef PROJECT_NAME
#include "PhysicsTools/TheNtupleMaker/interface/pdg.h"
#else
#include "pdg.h"
#endif

#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

using namespace std;

Double_t DeltaRfun(Double_t eta1,Double_t eta2,Double_t phi1,Double_t phi2)
{
  return TMath::Sqrt(pow((eta1-eta2),2)+pow(deltaPhi(phi1,phi2),2));
}

Double_t fnc_dscb(Double_t *xx, Double_t *pp)
{
  Double_t x   = xx[0];
  Double_t N   = pp[0];
  Double_t mu  = pp[1];
  Double_t sig = pp[2];
  Double_t a1  = pp[3];
  Double_t p1  = pp[4];
  Double_t a2  = pp[5];
  Double_t p2  = pp[6];

  Double_t u   = (x-mu)/sig;
  Double_t A1  = TMath::Power(p1/TMath::Abs(a1),p1)*TMath::Exp(-a1*a1/2);
  Double_t A2  = TMath::Power(p2/TMath::Abs(a2),p2)*TMath::Exp(-a2*a2/2);
  Double_t B1  = p1/TMath::Abs(a1) - TMath::Abs(a1);
  Double_t B2  = p2/TMath::Abs(a2) - TMath::Abs(a2);

  Double_t result=N;
  if (u<-a1)
      result *= A1*TMath::Power(B1-u,-p1);
  else if (u<a2)
      result *= TMath::Exp(-u*u/2);
  else
      result *= A2*TMath::Power(B2+u,-p2);
  return result;
}

    //////////////////////////////////
    //// P A P E R   4 - V E C T O R   D E F I N I T I O N   O F   P H I   A N D   P H I 1
    //////////////////////////////////
void computeAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4Z2, TLorentzVector thep4M21, TLorentzVector thep4M22, double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1){
       
        ///////////////////////////////////////////////
        // check for z1/z2 convention, redefine all 4 vectors with convention
        ///////////////////////////////////////////////	
    TLorentzVector p4H, p4Z1, p4M11, p4M12, p4Z2, p4M21, p4M22;
    p4H = thep4H;
        
    p4Z1 = thep4Z1; p4M11 = thep4M11; p4M12 = thep4M12;
    p4Z2 = thep4Z2; p4M21 = thep4M21; p4M22 = thep4M22;
        //// costhetastar
	TVector3 boostX = -(thep4H.BoostVector());
	TLorentzVector thep4Z1inXFrame( p4Z1 );
	TLorentzVector thep4Z2inXFrame( p4Z2 );	
	thep4Z1inXFrame.Boost( boostX );
	thep4Z2inXFrame.Boost( boostX );
	TVector3 theZ1X_p3 = TVector3( thep4Z1inXFrame.X(), thep4Z1inXFrame.Y(), thep4Z1inXFrame.Z() );
	TVector3 theZ2X_p3 = TVector3( thep4Z2inXFrame.X(), thep4Z2inXFrame.Y(), thep4Z2inXFrame.Z() );    
    costhetastar = theZ1X_p3.CosTheta();

        //// --------------------------- costheta1
    TVector3 boostV1 = -(thep4Z1.BoostVector());
    TLorentzVector p4M11_BV1( p4M11 );
	TLorentzVector p4M12_BV1( p4M12 );	
    TLorentzVector p4M21_BV1( p4M21 );
	TLorentzVector p4M22_BV1( p4M22 );
    p4M11_BV1.Boost( boostV1 );
	p4M12_BV1.Boost( boostV1 );
	p4M21_BV1.Boost( boostV1 );
	p4M22_BV1.Boost( boostV1 );
    
    TLorentzVector p4V2_BV1 = p4M21_BV1 + p4M22_BV1;
        //// costheta1
    costheta1 = -p4V2_BV1.Vect().Dot( p4M11_BV1.Vect() )/p4V2_BV1.Vect().Mag()/p4M11_BV1.Vect().Mag();

        //// --------------------------- costheta2
    TVector3 boostV2 = -(thep4Z2.BoostVector());
    TLorentzVector p4M11_BV2( p4M11 );
	TLorentzVector p4M12_BV2( p4M12 );	
    TLorentzVector p4M21_BV2( p4M21 );
	TLorentzVector p4M22_BV2( p4M22 );
    p4M11_BV2.Boost( boostV2 );
	p4M12_BV2.Boost( boostV2 );
	p4M21_BV2.Boost( boostV2 );
	p4M22_BV2.Boost( boostV2 );
    
    TLorentzVector p4V1_BV2 = p4M11_BV2 + p4M12_BV2;
        //// costheta2
    costheta2 = -p4V1_BV2.Vect().Dot( p4M21_BV2.Vect() )/p4V1_BV2.Vect().Mag()/p4M21_BV2.Vect().Mag();
    
        //// --------------------------- Phi and Phi1
//    TVector3 boostX = -(thep4H.BoostVector());
    TLorentzVector p4M11_BX( p4M11 );
	TLorentzVector p4M12_BX( p4M12 );	
    TLorentzVector p4M21_BX( p4M21 );
	TLorentzVector p4M22_BX( p4M22 );	
    
	p4M11_BX.Boost( boostX );
	p4M12_BX.Boost( boostX );
	p4M21_BX.Boost( boostX );
	p4M22_BX.Boost( boostX );
    
    TVector3 tmp1 = p4M11_BX.Vect().Cross( p4M12_BX.Vect() );
    TVector3 tmp2 = p4M21_BX.Vect().Cross( p4M22_BX.Vect() );    
    
    TVector3 normal1_BX( tmp1.X()/tmp1.Mag(), tmp1.Y()/tmp1.Mag(), tmp1.Z()/tmp1.Mag() ); 
    TVector3 normal2_BX( tmp2.X()/tmp2.Mag(), tmp2.Y()/tmp2.Mag(), tmp2.Z()/tmp2.Mag() ); 

        //// Phi
    TLorentzVector p4Z1_BX = p4M11_BX + p4M12_BX;    
    double tmpSgnPhi = p4Z1_BX.Vect().Dot( normal1_BX.Cross( normal2_BX) );
    double sgnPhi = tmpSgnPhi/fabs(tmpSgnPhi);
    Phi = sgnPhi * acos( -1.*normal1_BX.Dot( normal2_BX) );
    
    
        //////////////
    
    TVector3 beamAxis(0,0,1);
    TVector3 tmp3 = (p4M11_BX + p4M12_BX).Vect();
    
    TVector3 p3V1_BX( tmp3.X()/tmp3.Mag(), tmp3.Y()/tmp3.Mag(), tmp3.Z()/tmp3.Mag() );
    TVector3 tmp4 = beamAxis.Cross( p3V1_BX );
    TVector3 normalSC_BX( tmp4.X()/tmp4.Mag(), tmp4.Y()/tmp4.Mag(), tmp4.Z()/tmp4.Mag() );
        
        //// Phi1
    double tmpSgnPhi1 = p4Z1_BX.Vect().Dot( normal1_BX.Cross( normalSC_BX) );
    double sgnPhi1 = tmpSgnPhi1/fabs(tmpSgnPhi1);    
    Phi1 = sgnPhi1 * acos( normal1_BX.Dot( normalSC_BX) );    
    
//    std::cout << "extractAngles: " << std::endl;
//    std::cout << "costhetastar = " << costhetastar << ", costheta1 = " << costheta1 << ", costheta2 = " << costheta2 << std::endl;
//    std::cout << "Phi = " << Phi << ", Phi1 = " << Phi1 << std::endl;    

}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  cout << "Start program" << endl;

  commandLine cmdline;
  decodeCommandLine(argc, argv, cmdline);

  // Get names of ntuple files to be processed and open chain of ntuples

  vector<string> filenames = getFilenames(cmdline.filelist);
  cout << "Open file stream" << endl;
  itreestream stream(filenames, "Events");
  if ( !stream.good() ) error("unable to open ntuple file(s)");

  // Get number of events to be read

  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;

  // Select variables to be read

  //selectVariables(stream);

  stream.select("edmEventHelper_info.bunchCrossing", eventhelper_bunchCrossing);
  stream.select("edmEventHelper_info.event", eventhelper_event);
  stream.select("edmEventHelper_info.isRealData", eventhelper_isRealData);
  stream.select("edmEventHelper_info.luminosityBlock", eventhelper_luminosityBlock);
  stream.select("edmEventHelper_info.orbitNumber", eventhelper_orbitNumber);
  stream.select("edmEventHelper_info.run", eventhelper_run);
  /*stream.select("edmEventHelperExtra_info.dijetCHS_invmass", eventhelperextra_dijetCHS_invmass);
  stream.select("edmEventHelperExtra_info.dijet_invmass", eventhelperextra_dijet_invmass);
  stream.select("edmEventHelperExtra_info.wj1CHS_energy", eventhelperextra_wj1CHS_energy);
  stream.select("edmEventHelperExtra_info.wj1CHS_eta", eventhelperextra_wj1CHS_eta);
  stream.select("edmEventHelperExtra_info.wj1CHS_mass", eventhelperextra_wj1CHS_mass);
  stream.select("edmEventHelperExtra_info.wj1CHS_nconst", eventhelperextra_wj1CHS_nconst);
  stream.select("edmEventHelperExtra_info.wj1CHS_phi", eventhelperextra_wj1CHS_phi);
  stream.select("edmEventHelperExtra_info.wj1CHS_pt", eventhelperextra_wj1CHS_pt);
  stream.select("edmEventHelperExtra_info.wj1_energy", eventhelperextra_wj1_energy);
  stream.select("edmEventHelperExtra_info.wj1_eta", eventhelperextra_wj1_eta);
  stream.select("edmEventHelperExtra_info.wj1_mass", eventhelperextra_wj1_mass);
  stream.select("edmEventHelperExtra_info.wj1_nconst", eventhelperextra_wj1_nconst);
  stream.select("edmEventHelperExtra_info.wj1_phi", eventhelperextra_wj1_phi);
  stream.select("edmEventHelperExtra_info.wj1_pt", eventhelperextra_wj1_pt);
  stream.select("edmEventHelperExtra_info.wj1wj2CHS_invmass", eventhelperextra_wj1wj2CHS_invmass);
  stream.select("edmEventHelperExtra_info.wj1wj2CHS_nconst", eventhelperextra_wj1wj2CHS_nconst);
  stream.select("edmEventHelperExtra_info.wj1wj2_invmass", eventhelperextra_wj1wj2_invmass);
  stream.select("edmEventHelperExtra_info.wj1wj2_nconst", eventhelperextra_wj1wj2_nconst);
  stream.select("edmEventHelperExtra_info.wj2CHS_energy", eventhelperextra_wj2CHS_energy);
  stream.select("edmEventHelperExtra_info.wj2CHS_eta", eventhelperextra_wj2CHS_eta);
  stream.select("edmEventHelperExtra_info.wj2CHS_mass", eventhelperextra_wj2CHS_mass);
  stream.select("edmEventHelperExtra_info.wj2CHS_nconst", eventhelperextra_wj2CHS_nconst);
  stream.select("edmEventHelperExtra_info.wj2CHS_phi", eventhelperextra_wj2CHS_phi);
  stream.select("edmEventHelperExtra_info.wj2CHS_pt", eventhelperextra_wj2CHS_pt);
  stream.select("edmEventHelperExtra_info.wj2_energy", eventhelperextra_wj2_energy);
  stream.select("edmEventHelperExtra_info.wj2_eta", eventhelperextra_wj2_eta);
  stream.select("edmEventHelperExtra_info.wj2_mass", eventhelperextra_wj2_mass);
  stream.select("edmEventHelperExtra_info.wj2_nconst", eventhelperextra_wj2_nconst);
  stream.select("edmEventHelperExtra_info.wj2_phi", eventhelperextra_wj2_phi);
  stream.select("edmEventHelperExtra_info.wj2_pt", eventhelperextra_wj2_pt);
  stream.select("edmEventHelperExtra_info.wj2_pt", eventhelperextra_wj2_pt);
  */
  stream.select("edmEventHelperExtra_info.numberOfPrimaryVertices", eventhelperextra_numberOfPrimaryVertices);
  
  stream.select("patJetHelper_patJetsWithVar.chargedEmEnergyFraction", jethelperNoCHS_chargedEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.chargedHadronEnergyFraction", jethelperNoCHS_chargedHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.chargedMultiplicity", jethelperNoCHS_chargedMultiplicity);
  stream.select("patJetHelper_patJetsWithVar.energy", jethelperNoCHS_energy);
  stream.select("patJetHelper_patJetsWithVar.eta", jethelperNoCHS_eta);
  stream.select("patJetHelper_patJetsWithVar.rapidity", jethelperNoCHS_rapidity);
  stream.select("patJetHelper_patJetsWithVar.mass", jethelperNoCHS_mass);
  stream.select("patJetHelper_patJetsWithVar.muonEnergyFraction", jethelperNoCHS_muonEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.nConstituents", jethelperNoCHS_nConstituents);
  stream.select("patJetHelper_patJetsWithVar.neutralEmEnergyFraction", jethelperNoCHS_neutralEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.neutralHadronEnergyFraction", jethelperNoCHS_neutralHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.phi", jethelperNoCHS_phi);
  stream.select("patJetHelper_patJetsWithVar.pt", jethelperNoCHS_pt);
  stream.select("patJetHelper_patJetsWithVarCHS.chargedEmEnergyFraction", jethelper_chargedEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.chargedHadronEnergyFraction", jethelper_chargedHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.chargedMultiplicity", jethelper_chargedMultiplicity);
  stream.select("patJetHelper_patJetsWithVarCHS.combinedSecondaryVertexBJetTags", jethelper_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.combinedSecondaryVertexMVABJetTags", jethelper_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.energy", jethelper_energy);
  stream.select("patJetHelper_patJetsWithVarCHS.eta", jethelper_eta);
  stream.select("patJetHelper_patJetsWithVarCHS.rapidity", jethelper_rapidity);
  stream.select("patJetHelper_patJetsWithVarCHS.jetBProbabilityBJetTags", jethelper_jetBProbabilityBJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.jetProbabilityBJetTags", jethelper_jetProbabilityBJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.mass", jethelper_mass);
  stream.select("patJetHelper_patJetsWithVarCHS.muonEnergyFraction", jethelper_muonEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.nConstituents", jethelper_nConstituents);
  stream.select("patJetHelper_patJetsWithVarCHS.neutralEmEnergyFraction", jethelper_neutralEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.neutralHadronEnergyFraction", jethelper_neutralHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.phi", jethelper_phi);
  stream.select("patJetHelper_patJetsWithVarCHS.pt", jethelper_pt);
  
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.combinedSecondaryVertexBJetTags", jethelperCA8_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.combinedSecondaryVertexMVABJetTags", jethelperCA8_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.energy", jethelperCA8_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.et", jethelperCA8_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.eta", jethelperCA8_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetArea", jethelperCA8_jetArea);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetBProbabilityBJetTags", jethelperCA8_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetProbabilityBJetTags", jethelperCA8_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.mass", jethelperCA8_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.nConstituents", jethelperCA8_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.partonFlavour", jethelperCA8_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.chargedEmEnergyFraction", jethelperCA8_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.chargedHadronEnergyFraction", jethelperCA8_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.chargedMultiplicity", jethelperCA8_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.muonEnergyFraction", jethelperCA8_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.neutralEmEnergyFraction", jethelperCA8_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.neutralHadronEnergyFraction", jethelperCA8_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.phi", jethelperCA8_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.pt", jethelperCA8_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.trackCountingHighEffBJetTags", jethelperCA8_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.trackCountingHighPurBJetTags", jethelperCA8_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_energy", jethelperCA8_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_et", jethelperCA8_uncor_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_pt", jethelperCA8_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.tau1", jethelperCA8_tau1);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.tau2", jethelperCA8_tau2);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.C2beta15", jethelperCA8_C2beta15);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.C2beta17", jethelperCA8_C2beta17);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.C2beta20", jethelperCA8_C2beta20);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetCharge03", jethelperCA8_jetCharge03);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetCharge05", jethelperCA8_jetCharge05);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetCharge10", jethelperCA8_jetCharge10);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.getNcharged01", jethelperCA8_Ncharged01);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.getNneutral01", jethelperCA8_Nneutral01);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.getChargedPt2", jethelperCA8_ChargedPt2);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.getPt2", jethelperCA8_Pt2);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.mass", jethelperCA8pruned_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.pt", jethelperCA8pruned_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.uncor_pt", jethelperCA8pruned_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.numberOfDaughters", jethelperCA8pruned_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_energy", jethelperCA8pruned_daughter_0_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_eta", jethelperCA8pruned_daughter_0_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_mass", jethelperCA8pruned_daughter_0_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_phi", jethelperCA8pruned_daughter_0_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_pt", jethelperCA8pruned_daughter_0_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_energy", jethelperCA8pruned_daughter_1_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_eta", jethelperCA8pruned_daughter_1_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_mass", jethelperCA8pruned_daughter_1_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_phi", jethelperCA8pruned_daughter_1_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_pt", jethelperCA8pruned_daughter_1_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.tau1", jethelperCA8pruned_tau1);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.tau2", jethelperCA8pruned_tau2);

  stream.select("patJetHelper_patGenJetsCA8CHS.energy", jethelperGenCA8_energy);
  stream.select("patJetHelper_patGenJetsCA8CHS.et", jethelperGenCA8_et);
  stream.select("patJetHelper_patGenJetsCA8CHS.eta", jethelperGenCA8_eta);
  stream.select("patJetHelper_patGenJetsCA8CHS.jetArea", jethelperGenCA8_jetArea);
  stream.select("patJetHelper_patGenJetsCA8CHS.mass", jethelperGenCA8_mass);
  stream.select("patJetHelper_patGenJetsCA8CHS.nConstituents", jethelperGenCA8_nConstituents);
  stream.select("patJetHelper_patGenJetsCA8CHS.partonFlavour", jethelperGenCA8_partonFlavour);
  stream.select("patJetHelper_patGenJetsCA8CHS.phi", jethelperGenCA8_phi);
  stream.select("patJetHelper_patGenJetsCA8CHS.pt", jethelperGenCA8_pt);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau1", jethelperGenCA8_tau1);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau2", jethelperGenCA8_tau2);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau1Pt2", jethelperGenCA8_tau1Pt2);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau2Pt2", jethelperGenCA8_tau2Pt2);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau1CHS", jethelperGenCA8_tau1CHS);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau2CHS", jethelperGenCA8_tau2CHS);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau21PUcorrected", jethelperGenCA8_tau21PUcorrected);
  stream.select("patJetHelper_patGenJetsCA8CHS.genTau21PUcorrectedCHS", jethelperGenCA8_tau21PUcorrectedCHS);
  stream.select("patJetHelper_patGenJetsCA8CHS.genNCHS", jethelperGenCA8_NCHS);
  stream.select("patJetHelper_patGenJetsCA8CHS.genC2beta15", jethelperGenCA8_C2beta15);
  stream.select("patJetHelper_patGenJetsCA8CHS.genC2beta17", jethelperGenCA8_C2beta17);
  stream.select("patJetHelper_patGenJetsCA8CHS.genC2beta20", jethelperGenCA8_C2beta20);
  stream.select("patJetHelper_patGenJetsCA8CHS.genC2beta17CHS", jethelperGenCA8_C2beta17CHS);
  stream.select("patJetHelper_patGenJetsCA8CHS.genJetCharge03", jethelperGenCA8_jetCharge03);
  stream.select("patJetHelper_patGenJetsCA8CHS.genJetCharge05", jethelperGenCA8_jetCharge05);
  stream.select("patJetHelper_patGenJetsCA8CHS.genJetCharge10", jethelperGenCA8_jetCharge10);
  stream.select("patJetHelper_patGenJetsCA8CHS.getNcharged01", jethelperGenCA8_Ncharged01);
  stream.select("patJetHelper_patGenJetsCA8CHS.getNneutral01", jethelperGenCA8_Nneutral01);
  stream.select("patJetHelper_patGenJetsCA8CHS.getChargedPt2", jethelperGenCA8_ChargedPt2);
  stream.select("patJetHelper_patGenJetsCA8CHS.getPt2", jethelperGenCA8_Pt2);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.mass", jethelperGenCA8pruned_mass);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.pt", jethelperGenCA8pruned_pt);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.numberOfDaughters", jethelperGenCA8pruned_numberOfDaughters);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_0_energy", jethelperGenCA8pruned_daughter_0_energy);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_0_eta", jethelperGenCA8pruned_daughter_0_eta);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_0_mass", jethelperGenCA8pruned_daughter_0_mass);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_0_phi", jethelperGenCA8pruned_daughter_0_phi);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_0_pt", jethelperGenCA8pruned_daughter_0_pt);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_1_energy", jethelperGenCA8pruned_daughter_1_energy);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_1_eta", jethelperGenCA8pruned_daughter_1_eta);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_1_mass", jethelperGenCA8pruned_daughter_1_mass);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_1_phi", jethelperGenCA8pruned_daughter_1_phi);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.daughter_1_pt", jethelperGenCA8pruned_daughter_1_pt);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.genTau1", jethelperGenCA8pruned_tau1);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.genTau2", jethelperGenCA8pruned_tau2);

  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.combinedSecondaryVertexBJetTags", jethelperAK7_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.combinedSecondaryVertexMVABJetTags", jethelperAK7_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.energy", jethelperAK7_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.et", jethelperAK7_et);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.eta", jethelperAK7_eta);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.jetArea", jethelperAK7_jetArea);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.jetBProbabilityBJetTags", jethelperAK7_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.jetProbabilityBJetTags", jethelperAK7_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.mass", jethelperAK7_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.nConstituents", jethelperAK7_nConstituents);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.partonFlavour", jethelperAK7_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.phi", jethelperAK7_phi);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.pt", jethelperAK7_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.trackCountingHighEffBJetTags", jethelperAK7_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.trackCountingHighPurBJetTags", jethelperAK7_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.uncor_energy", jethelperAK7_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.uncor_et", jethelperAK7_uncor_et);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.uncor_pt", jethelperAK7_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.tau1", jethelperAK7_tau1);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.tau2", jethelperAK7_tau2);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.C2beta15", jethelperAK7_C2beta15);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.C2beta17", jethelperAK7_C2beta17);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.C2beta20", jethelperAK7_C2beta20);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.mass", jethelperAK7pruned_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.pt", jethelperAK7pruned_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.uncor_pt", jethelperAK7pruned_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.numberOfDaughters", jethelperAK7pruned_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_energy", jethelperAK7pruned_daughter_0_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_eta", jethelperAK7pruned_daughter_0_eta);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_mass", jethelperAK7pruned_daughter_0_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_phi", jethelperAK7pruned_daughter_0_phi);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_pt", jethelperAK7pruned_daughter_0_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_energy", jethelperAK7pruned_daughter_1_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_eta", jethelperAK7pruned_daughter_1_eta);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_mass", jethelperAK7pruned_daughter_1_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_phi", jethelperAK7pruned_daughter_1_phi);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_pt", jethelperAK7pruned_daughter_1_pt);

  stream.select("patMET_patMETsRaw.et", met2_et);
  stream.select("patMET_patMETsRaw.sumEt", met2_sumEt);
  stream.select("nrecoVertex_offlinePrimaryVertices", nvertex);
  stream.select("nPileupSummaryInfo_addPileupInfo", nPileupSummaryInfo);
  stream.select("PileupSummaryInfo_addPileupInfo.getBunchCrossing", pileupsummaryinfo_getBunchCrossing);
  stream.select("PileupSummaryInfo_addPileupInfo.getPU_NumInteractions", pileupsummaryinfo_getPU_NumInteractions);
  stream.select("PileupSummaryInfo_addPileupInfo.getTrueNumInteractions", pileupsummaryinfo_getTrueNumInteractions);
  stream.select("sdouble_kt6PFJets_rho.value", sdouble_kt6PFJets_rho_value);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v1", triggerresultshelper_HLT_HT500_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v2", triggerresultshelper_HLT_HT500_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v3", triggerresultshelper_HLT_HT500_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v1", triggerresultshelper_HLT_HT550_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v2", triggerresultshelper_HLT_HT550_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v3", triggerresultshelper_HLT_HT550_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v1", triggerresultshelper_HLT_HT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v2", triggerresultshelper_HLT_HT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v3", triggerresultshelper_HLT_HT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v1", triggerresultshelper_HLT_HT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v2", triggerresultshelper_HLT_HT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v3", triggerresultshelper_HLT_HT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v1", triggerresultshelper_HLT_PFHT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v2", triggerresultshelper_HLT_PFHT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v3", triggerresultshelper_HLT_PFHT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v4", triggerresultshelper_HLT_PFHT650_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v5", triggerresultshelper_HLT_PFHT650_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v6", triggerresultshelper_HLT_PFHT650_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v7", triggerresultshelper_HLT_PFHT650_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v8", triggerresultshelper_HLT_PFHT650_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v1", triggerresultshelper_HLT_PFHT700_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v2", triggerresultshelper_HLT_PFHT700_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v3", triggerresultshelper_HLT_PFHT700_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v4", triggerresultshelper_HLT_PFHT700_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v5", triggerresultshelper_HLT_PFHT700_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v6", triggerresultshelper_HLT_PFHT700_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v1", triggerresultshelper_HLT_PFHT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v2", triggerresultshelper_HLT_PFHT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v3", triggerresultshelper_HLT_PFHT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v4", triggerresultshelper_HLT_PFHT750_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v5", triggerresultshelper_HLT_PFHT750_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v6", triggerresultshelper_HLT_PFHT750_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.hcalLaserEventFilterPath", triggerresultshelper_hcalLaserEventFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.noscrapingFilterPath", triggerresultshelper_noscrapingFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.primaryVertexFilterPath", triggerresultshelper_primaryVertexFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.trackingFailureFilterPath", triggerresultshelper_trackingFailureFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.CSCTightHaloFilterPath", triggerresultshelper_CSCTightHaloFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.EcalDeadCellBoundaryEnergyFilterPath", triggerresultshelper_EcalDeadCellBoundaryEnergyFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.EcalDeadCellTriggerPrimitiveFilterPath", triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.HBHENoiseFilterPath", triggerresultshelper_HBHENoiseFilterPath);
  stream.select("sint_hcallasereventfilter2012.value", triggerresultshelper_hcallasereventfilter2012);

  stream.select("nrecoGenParticleHelper_genParticles", ngenparticlehelper);
  stream.select("recoGenParticleHelper_genParticles.firstDaughter", genparticlehelper_firstDaughter);
  stream.select("recoGenParticleHelper_genParticles.firstMother", genparticlehelper_firstMother);
  stream.select("recoGenParticleHelper_genParticles.lastDaughter", genparticlehelper_lastDaughter);
  stream.select("recoGenParticleHelper_genParticles.lastMother", genparticlehelper_lastMother);
  stream.select("recoGenParticleHelper_genParticles.pdgId", genparticlehelper_pdgId);
  stream.select("recoGenParticleHelper_genParticles.status", genparticlehelper_status);
  stream.select("recoGenParticleHelper_genParticles.charge", genparticlehelper_charge);
  stream.select("recoGenParticleHelper_genParticles.eta", genparticlehelper_eta);
  stream.select("recoGenParticleHelper_genParticles.phi", genparticlehelper_phi);
  stream.select("recoGenParticleHelper_genParticles.pt", genparticlehelper_pt);
  stream.select("recoGenParticleHelper_genParticles.mass", genparticlehelper_mass);

  stream.select("sdouble_vertexWeightSummer12MC53X2012ABCDData.value", vertexWeight);
/*
  stream.select("cmgPFJet_cmgPFJetSel.energy", jethelperCMG_energy);
  stream.select("cmgPFJet_cmgPFJetSel.eta", jethelperCMG_eta);
  stream.select("cmgPFJet_cmgPFJetSel.rapidity", jethelperCMG_rapidity);
  stream.select("cmgPFJet_cmgPFJetSel.mass", jethelperCMG_mass);
  stream.select("cmgPFJet_cmgPFJetSel.phi", jethelperCMG_phi);
  stream.select("cmgPFJet_cmgPFJetSel.pt", jethelperCMG_pt);
*/
  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the following
  // line

  TApplication app("analyzer", &argc, argv);

  /*
	 Notes:
	
	 1. Use
	   ofile = outputFile(cmdline.outputfile, stream)
	
	 to skim events to output file in addition to writing out histograms.
	
	 2. Use
	   ofile.addEvent(event-weight)
	
	 to specify that the current event is to be added to the output file.
	 If omitted, the event-weight is defaulted to 1.
	
	 3. Use
	    ofile.count(cut-name, event-weight)
	
	 to keep track, in the count histogram, of the number of events
	 passing a given cut. If omitted, the event-weight is taken to be 1.
	 If you want the counts in the count histogram to appear in a given
	 order, specify the order, before entering the event loop, as in
	 the example below
	 
	    ofile.count("NoCuts", 0)
		ofile.count("GoodEvent", 0)
		ofile.count("Vertex", 0)
		ofile.count("MET", 0)
  */
  
  outputFile ofile(cmdline.outputfilename);

  //---------------------------------------------------------------------------
  // Declare histograms
  //---------------------------------------------------------------------------

  const int NBINS = 104;
  Double_t BOUNDARIES[NBINS] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325,
  354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687,
  1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509,
  4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7060, 7320, 7589, 7866, 8152, 8447, 8752, 9067, 9391, 9726, 10072, 10430, 
  10798, 11179, 11571, 11977, 12395, 12827, 13272, 13732, 14000};

  double nJetsCA8;
  double nGenJetsCA8;
  double weight;
  double Jet1CA8MassDrop;
  double Jet1CA8Nsub;
  double Jet1CA8NsubPruned;
  double Jet1GenCA8MassDrop;
  double Jet1GenCA8Nsub;
  double Jet1GenCA8NsubPruned;
  double Jet1GenCA8NsubPt2;
  double Jet1GenCA8NsubCHS;
  double DijetMassCA8=0;
  double deta=0;
  double GenDijetMassCA8=0;
  double Gendeta=0;
  
  int genWcharge=0;
  double costheta1=0;
  double costheta2=0;
  double Phi=0;
  double costhetastar=0;
  double Phi1=0;

  TTree *dijetWtag = new TTree("dijetWtag", "dijetWtag");
  dijetWtag->Branch("nPU",&pileupsummaryinfo_getPU_NumInteractions[0],"nPU/I");
  dijetWtag->Branch("nJets",&nJetsCA8,"nJets/D");
  dijetWtag->Branch("deta",&deta,"deta/D");
  dijetWtag->Branch("DijetMass",&DijetMassCA8,"DijetMass/D");
  dijetWtag->Branch("Jet1pt",&jethelperCA8_pt[0],"Jet1pt/D");
  dijetWtag->Branch("Jet1eta",&jethelperCA8_eta[0],"Jet1eta/D");
  dijetWtag->Branch("Jet1phi",&jethelperCA8_phi[0],"Jet1phi/D");
  dijetWtag->Branch("Jet1Mass",&jethelperCA8pruned_mass[0],"Jet1Mass/D");
  dijetWtag->Branch("Jet1MassDrop",&Jet1CA8MassDrop,"Jet1MassDrop/D");
  dijetWtag->Branch("Jet1Nsub",&Jet1CA8Nsub,"Jet1Nsub/D");
  dijetWtag->Branch("Jet1NsubPruned",&Jet1CA8NsubPruned,"Jet1NsubPruned/D");
  //dijetWtag->Branch("Jet1C2beta15",&jethelperCA8_C2beta15[0],"Jet1C2beta15/D");
  dijetWtag->Branch("Jet1C2beta17",&jethelperCA8_C2beta17[0],"Jet1C2beta17/D");
  //dijetWtag->Branch("Jet1C2beta20",&jethelperCA8_C2beta20[0],"Jet1C2beta20/D");
  dijetWtag->Branch("Jet1jetCharge03",&jethelperCA8_jetCharge03[0],"Jet1jetCharge03/D");
  dijetWtag->Branch("Jet1jetCharge05",&jethelperCA8_jetCharge05[0],"Jet1jetCharge05/D");
  dijetWtag->Branch("Jet1jetCharge10",&jethelperCA8_jetCharge10[0],"Jet1jetCharge10/D");
  dijetWtag->Branch("Jet1nConstituents",&jethelperCA8_nConstituents[0],"Jet1nConstituents/I");
  dijetWtag->Branch("Jet1Ncharged01",&jethelperCA8_Ncharged01[0],"Jet1Ncharged01/D");
  dijetWtag->Branch("Jet1Nneutral01",&jethelperCA8_Nneutral01[0],"Jet1Nneutral01/D");
  dijetWtag->Branch("Jet1ChargedPt2",&jethelperCA8_ChargedPt2[0],"Jet1ChargedPt2/D");
  dijetWtag->Branch("Jet1Pt2",&jethelperCA8_Pt2[0],"Jet1Pt2/D");

  dijetWtag->Branch("Jet1genWcharge",&genWcharge,"Jet1genWcharge/I");
  dijetWtag->Branch("costheta1",&costheta1,"costheta1/D");
  dijetWtag->Branch("costheta2",&costheta2,"costheta2/D");
  dijetWtag->Branch("Phi",&Phi,"Phi/D");
  dijetWtag->Branch("costhetastar",&costhetastar,"costhetastar/D");
  dijetWtag->Branch("Phi1",&Phi1,"Phi1/D");

  dijetWtag->Branch("nGenJets",&nGenJetsCA8,"nGenJets/D");
  dijetWtag->Branch("Gendeta",&Gendeta,"Gendeta/D");
  dijetWtag->Branch("GenDijetMass",&GenDijetMassCA8,"GenDijetMass/D");
  dijetWtag->Branch("GenJet1pt",&jethelperGenCA8_pt[0],"GenJet1pt/D");
  dijetWtag->Branch("GenJet1eta",&jethelperGenCA8_eta[0],"GenJet1eta/D");
  dijetWtag->Branch("GenJet1phi",&jethelperGenCA8_phi[0],"GenJet1phi/D");
  dijetWtag->Branch("GenJet1Mass",&jethelperGenCA8pruned_mass[0],"GenJet1Mass/D");
  dijetWtag->Branch("GenJet1MassDrop",&Jet1GenCA8MassDrop,"GenJet1MassDrop/D");
  dijetWtag->Branch("GenJet1Nsub",&Jet1GenCA8Nsub,"GenJet1Nsub/D");
  dijetWtag->Branch("GenJet1NsubPruned",&Jet1GenCA8NsubPruned,"GenJet1NsubPruned/D");
  dijetWtag->Branch("GenJet1NsubPt2",&Jet1GenCA8NsubPt2,"GenJet1NsubPt2/D");
  dijetWtag->Branch("GenJet1NsubCHS",&Jet1GenCA8NsubCHS,"GenJet1NsubCHS/D");
  dijetWtag->Branch("GenJet1NsubPUcorrected",&jethelperGenCA8_tau21PUcorrected[0],"GenJet1NsubPUcorrected/D");
  dijetWtag->Branch("GenJet1NsubPUcorrectedCHS",&jethelperGenCA8_tau21PUcorrectedCHS[0],"GenJet1NsubPUcorrectedCHS/D");
  dijetWtag->Branch("GenJet1NCHS",&jethelperGenCA8_NCHS[0],"GenJet1NCHS/I");
  //dijetWtag->Branch("GenJet1C2beta15",&jethelperGenCA8_C2beta15[0],"GenJet1C2beta15/D");
  dijetWtag->Branch("GenJet1C2beta17",&jethelperGenCA8_C2beta17[0],"GenJet1C2beta17/D");
  //dijetWtag->Branch("GenJet1C2beta20",&jethelperGenCA8_C2beta20[0],"GenJet1C2beta20/D");
  dijetWtag->Branch("GenJet1C2beta17CHS",&jethelperGenCA8_C2beta17CHS[0],"GenJet1C2beta17CHS/D");
  dijetWtag->Branch("GenJet1jetCharge03",&jethelperGenCA8_jetCharge03[0],"GenJet1jetCharge03/D");
  dijetWtag->Branch("GenJet1jetCharge05",&jethelperGenCA8_jetCharge05[0],"GenJet1jetCharge05/D");
  dijetWtag->Branch("GenJet1jetCharge10",&jethelperGenCA8_jetCharge10[0],"GenJet1jetCharge10/D");
  dijetWtag->Branch("GenJet1nConstituents",&jethelperGenCA8_nConstituents[0],"GenJet1nConstituents/I");
  dijetWtag->Branch("GenJet1Ncharged01",&jethelperGenCA8_Ncharged01[0],"GenJet1Ncharged01/D");
  dijetWtag->Branch("GenJet1Nneutral01",&jethelperGenCA8_Nneutral01[0],"GenJet1Nneutral01/D");
  dijetWtag->Branch("GenJet1ChargedPt2",&jethelperGenCA8_ChargedPt2[0],"GenJet1ChargedPt2/D");
  dijetWtag->Branch("GenJet1Pt2",&jethelperGenCA8_Pt2[0],"GenJet1Pt2/D");
  dijetWtag->Branch("weight",&weight,"weight/D");
  dijetWtag->Branch("vertexWeight",&vertexWeight,"vertexWeight/D");
  dijetWtag->Branch("numberOfPrimaryVertices",&eventhelperextra_numberOfPrimaryVertices,"numberOfPrimaryVertices/I");

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  bool hcallasereventfilter2012active=false;
  bool datafiltersactive=false;

  for(int entry=0; entry < nevents; ++entry)
	{
	  // Read event into memory
	  stream.read(entry);

	  // Uncomment the following line if you wish to copy variables into
	  // structs. See the header file analyzer.h to find out what structs
	  // are available. Each struct contains the field "selected", which
	  // can be set as needed. Call saveSelectedObjects() before a call to
	  // addEvent if you wish to save only the selected objects.
	  
	  // fillObjects();
	  
	  // ---------------------
	  // -- event selection --
	  // ---------------------

          nJetsCA8=0;
	  Jet1CA8MassDrop=0;
	  Jet1CA8Nsub=0;
	  if(jethelperCA8_pt.size()>=1)
          {
	  for(unsigned n=0;n<jethelperCA8_pt.size();n++)
	     if(jethelperCA8_pt[n]>100)
	         nJetsCA8++;
          Jet1CA8MassDrop = max(jethelperCA8pruned_daughter_0_mass[0],jethelperCA8pruned_daughter_1_mass[0])/jethelperCA8pruned_mass[0]/jethelperCA8pruned_uncor_pt[0]*jethelperCA8pruned_pt[0];
	  if ((jethelperCA8pruned_daughter_0_mass[0]<0.0001)||(jethelperCA8pruned_daughter_1_mass[0]<0.0001)) Jet1CA8MassDrop = 2;
          Jet1CA8Nsub = jethelperCA8_tau2[0]/jethelperCA8_tau1[0];
          Jet1CA8NsubPruned = jethelperCA8pruned_tau2[0]/jethelperCA8pruned_tau1[0];
          }           

          nGenJetsCA8=0;
	  Jet1GenCA8MassDrop=0;
	  Jet1GenCA8Nsub=0;
	  Jet1GenCA8NsubPt2=0;
	  Jet1GenCA8NsubCHS=0;
	  if(jethelperGenCA8_pt.size()>=1)
          {
	  for(unsigned n=0;n<jethelperGenCA8_pt.size();n++)
	     if(jethelperGenCA8_pt[n]>100)
	         nGenJetsCA8++;
          Jet1GenCA8MassDrop = max(jethelperGenCA8pruned_daughter_0_mass[0],jethelperGenCA8pruned_daughter_1_mass[0])/jethelperGenCA8pruned_mass[0]/jethelperGenCA8pruned_pt[0]*jethelperGenCA8pruned_pt[0];
	  if ((jethelperGenCA8pruned_daughter_0_mass[0]<0.0001)||(jethelperGenCA8pruned_daughter_1_mass[0]<0.0001)) Jet1GenCA8MassDrop = 2;
          Jet1GenCA8Nsub = jethelperGenCA8_tau2[0]/jethelperGenCA8_tau1[0];
          Jet1GenCA8NsubPruned = jethelperGenCA8pruned_tau2[0]/jethelperGenCA8pruned_tau1[0];
          Jet1GenCA8NsubPt2 = jethelperGenCA8_tau2Pt2[0]/jethelperGenCA8_tau1Pt2[0];
          Jet1GenCA8NsubCHS = jethelperGenCA8_tau2CHS[0]/jethelperGenCA8_tau1CHS[0];
          }
	  
          for(int i=0;i<ngenparticlehelper;++i)
	  {
if((DeltaRfun(genparticlehelper_eta[i],jethelperCA8_eta[0],genparticlehelper_phi[i],jethelperCA8_phi[0])<0.2))
//   std::cerr << genparticlehelper_pdgId[i] << "," << genparticlehelper_charge[i] << "," << DeltaRfun(genparticlehelper_eta[i],jethelperCA8_eta[0],genparticlehelper_phi[i],jethelperCA8_phi[0]) << std::endl;
	      if((abs(genparticlehelper_pdgId[i])==24)&&(genparticlehelper_charge[i]<0)&&(DeltaRfun(genparticlehelper_eta[i],jethelperCA8_eta[0],genparticlehelper_phi[i],jethelperCA8_phi[0])<0.2))
                  genWcharge=-1;
if((abs(genparticlehelper_pdgId[i])==24)&&(genparticlehelper_charge[i]>0)&&(DeltaRfun(genparticlehelper_eta[i],jethelperCA8_eta[0],genparticlehelper_phi[i],jethelperCA8_phi[0])<0.2))
                  genWcharge=1;
          }

          TLorentzVector H,W1,W2,j11,j12,j21,j22;
          for(int i=0;i<ngenparticlehelper;++i)
	  {    
	      if((abs(genparticlehelper_pdgId[i])>=23)&&(abs(genparticlehelper_pdgId[i])<=25)&&(genparticlehelper_charge[i]<0))
	      {
                  W1.SetPtEtaPhiM(genparticlehelper_pt[i],genparticlehelper_eta[i],genparticlehelper_phi[i],genparticlehelper_mass[i]);
                   j11.SetPtEtaPhiM(genparticlehelper_pt[genparticlehelper_firstDaughter[i]],genparticlehelper_eta[genparticlehelper_firstDaughter[i]],genparticlehelper_phi[genparticlehelper_firstDaughter[i]],genparticlehelper_mass[genparticlehelper_firstDaughter[i]]);
                   j12.SetPtEtaPhiM(genparticlehelper_pt[genparticlehelper_lastDaughter[i]],genparticlehelper_eta[genparticlehelper_lastDaughter[i]],genparticlehelper_phi[genparticlehelper_lastDaughter[i]],genparticlehelper_mass[genparticlehelper_lastDaughter[i]]);
              }
	      if((abs(genparticlehelper_pdgId[i])>=23)&&(abs(genparticlehelper_pdgId[i])<=25)&&(genparticlehelper_charge[i]>0))
              {
	          W2.SetPtEtaPhiM(genparticlehelper_pt[i],genparticlehelper_eta[i],genparticlehelper_phi[i],genparticlehelper_mass[i]);
                   j21.SetPtEtaPhiM(genparticlehelper_pt[genparticlehelper_firstDaughter[i]],genparticlehelper_eta[genparticlehelper_firstDaughter[i]],genparticlehelper_phi[genparticlehelper_firstDaughter[i]],genparticlehelper_mass[genparticlehelper_firstDaughter[i]]);
                   j22.SetPtEtaPhiM(genparticlehelper_pt[genparticlehelper_lastDaughter[i]],genparticlehelper_eta[genparticlehelper_lastDaughter[i]],genparticlehelper_phi[genparticlehelper_lastDaughter[i]],genparticlehelper_mass[genparticlehelper_lastDaughter[i]]);
              }
          }
          H=W1+W2;
	  //std::cerr << H.Pt() << "," << W1.Pt() <<  "," << W2.Pt() <<  "," << j11.Pt() <<  "," << j12.Pt() <<  "," << j21.Pt() <<  "," << j22.Pt() <<  std::endl;
          computeAngles(H, W1, j11, j12, W2, j21, j22, costheta1, costheta2, Phi, costhetastar, Phi1);

          if (triggerresultshelper_hcallasereventfilter2012!=0)
	     hcallasereventfilter2012active=true;

	  if((triggerresultshelper_primaryVertexFilterPath!=0)&&
	     (triggerresultshelper_noscrapingFilterPath!=0)&&
	     (triggerresultshelper_trackingFailureFilterPath!=0)&&
	     (triggerresultshelper_hcalLaserEventFilterPath!=0)&&
	     (triggerresultshelper_HBHENoiseFilterPath!=0)&&
	     (triggerresultshelper_CSCTightHaloFilterPath!=0)&&
	     (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0))
	     datafiltersactive=true;

          DijetMassCA8=0;
	  deta=0;
          if(jethelperCA8_pt.size()>=2)
	  {
              TLorentzVector Jet1;
              TLorentzVector Jet2;
              Jet1.SetPtEtaPhiE(jethelperCA8_pt[0],jethelperCA8_eta[0],jethelperCA8_phi[0],jethelperCA8_energy[0]);
              Jet2.SetPtEtaPhiE(jethelperCA8_pt[1],jethelperCA8_eta[1],jethelperCA8_phi[1],jethelperCA8_energy[1]);
              DijetMassCA8 = (Jet1+Jet2).M();
              deta = fabs(jethelperCA8_eta[0]-jethelperCA8_eta[1]);
          }
          GenDijetMassCA8=0;
	  Gendeta=0;
          if(jethelperGenCA8_pt.size()>=2)
	  {
              TLorentzVector Jet1;
              TLorentzVector Jet2;
              Jet1.SetPtEtaPhiE(jethelperGenCA8_pt[0],jethelperGenCA8_eta[0],jethelperGenCA8_phi[0],jethelperGenCA8_energy[0]);
              Jet2.SetPtEtaPhiE(jethelperGenCA8_pt[1],jethelperGenCA8_eta[1],jethelperGenCA8_phi[1],jethelperGenCA8_energy[1]);
              GenDijetMassCA8 = (Jet1+Jet2).M();
              Gendeta = fabs(jethelperGenCA8_eta[0]-jethelperGenCA8_eta[1]);
          }

          //std::cerr << jethelperCA8_pt.size() << "," << jethelperCA8_pt[0]  << "," << (fabs(jethelperCA8_eta[0])) << std::endl;

	  if(!((((jethelperCA8_pt.size()>=2)&&(jethelperCA8_pt[0]>400)&&(fabs(jethelperCA8_eta[0])<2.5))||
	        ((jethelperGenCA8_pt.size()>=2)&&(jethelperGenCA8_pt[0]>400)&&(fabs(jethelperGenCA8_eta[0])<2.5)))&&
	     
	   (((jethelperCA8_muonEnergyFraction[0]<0.80)&&
	     //(jethelper_neutralHadronEnergyFraction[0]<0.99)&&
	     (jethelperCA8_neutralHadronEnergyFraction[0]<0.90)&&
	     //(jethelper_neutralEmEnergyFraction[0]<0.99)&&
	     (jethelperCA8_neutralEmEnergyFraction[0]<0.90)&&
	     (jethelperCA8_nConstituents[0]>1)&&
	     ((fabs(jethelperCA8_eta[0])>2.4)||
	      ((jethelperCA8_chargedHadronEnergyFraction[0]>0.01)&&
	       (jethelperCA8_chargedMultiplicity[0]>0)&&
	       (jethelperCA8_chargedEmEnergyFraction[0]<0.99)))&&
	     (((sdouble_kt6PFJets_rho_value<40)&&
	       (triggerresultshelper_primaryVertexFilterPath!=0)&&
	       (triggerresultshelper_noscrapingFilterPath!=0)&&
	       (triggerresultshelper_trackingFailureFilterPath!=0)&&
	       (triggerresultshelper_hcalLaserEventFilterPath!=0)&&
	       (triggerresultshelper_HBHENoiseFilterPath!=0)&&
	       (triggerresultshelper_CSCTightHaloFilterPath!=0)&&
	       (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0))||
	      (datafiltersactive==false))&&
	     ((triggerresultshelper_hcallasereventfilter2012!=0)||(hcallasereventfilter2012active==false))
	     )||((jethelperGenCA8_pt.size()>=1)))
	    )) continue;

	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

          if(geneventinfoproduct_weight>0)
	      weight=geneventinfoproduct_weight;
	  else
	      weight=1;
          if(vertexWeight==0)
	      vertexWeight=1;

          dijetWtag->Fill();

	}

  stream.close();
  ofile.close();

  return 0;
}
