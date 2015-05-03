#define test_allEvt_cxx
#include "test_allEvt.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>


TLorentzVector muPosGen_status1,muNegGen_status1,ZNocorr,muPosGen,muNegGen,ZGen_status1,ZGen_status3;
HTransformToHelicityFrame *GoToHXframe;
double costh_HX = -1e10;
double phi_HX = -1e10;
double costh_HX_gen = -1e10;
double phi_HX_gen = -1e10;
HTransformToCS *GoToCSframe;
double costh_CS = -1e10;
double phi_CS = -1e10;
double costh_CS_gen = -1e10;
double phi_CS_gen = -1e10;

const int nrapbins = 2;
double rapbins[nrapbins+1] = {0,1,2.1};
TH1D *hrapbins = new TH1D("hrapbins","hrapbins",nrapbins,rapbins);
const int nptbins = 8;
double ptbins[nptbins+1] = {0,10,20,35,55,80,120,200,1000};
TH1D *hptbins = new TH1D("hptbins","hptbins",nptbins,ptbins);

// The data is binned in 2 bins in | Y(Z) | and 8 bins in PT(Z). The bins in | Y(Z) | are [0,1.0], [1.0,2.1]. 63
// The bins in PT(Z) are [0,10], [10,20], [20,35], [35,55], [55,80], [80,120], [120,200], [200,∞].   In 64
// cos(θ∗) and | φ∗| 12 equidistent bins are used in each case. 65

void test_allEvt::Loop()
{
  std::map<std::string, TH1D*> h_1d;
  std::map<std::string, TH2D*> h_2d;

  GoToHXframe = new HTransformToHelicityFrame();
  GoToCSframe = new HTransformToCS();
  
  TFile *fin_ewk_syst = new TFile("fout_invariant_mass_qed.root");
  TH1D *hvpt_ewk_syst = (TH1D*) fin_ewk_syst->Get("vpt_ewk_p8std_div_ewk_p8bad");
  TH1D *hmass_ewk_syst = (TH1D*) fin_ewk_syst->Get("ewk_p8std_div_ewk_p8bad");
  TH1D *hmass_ewk_syst_photos = (TH1D*) fin_ewk_syst->Get("ewk_p8photos_div_ewk_p8bad");
  
  // TH1D *hmass=new TH1D("hmass","hmass",150,50,200);
  // hmass->Sumw2();
  
  // TH1D *hvpt=new TH1D("hvpt","hvpt",300,0,300);
  // hvpt->Sumw2();
  
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  // for (Long64_t jentry=0; jentry<nentries;jentry++) {
  for (Long64_t jentry=0; jentry<10e6;jentry++) {
    // for (Long64_t jentry=0; jentry<10e3;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    if(jentry%100000==0) std::cout << "jentry= "<< jentry << "/" << nentries << std::endl;
    
    if(ZGen_PostFSR_mass>50
        && MuPosGenStatus1_pt>25
        && MuNegGenStatus1_pt>25
        && TMath::Abs(MuPosGenStatus1_eta)<2.5
        && TMath::Abs(MuNegGenStatus1_eta)<2.5
        ){
          
      muPosGen_status1.SetPtEtaPhiM(MuPosGenStatus1_pt,MuPosGenStatus1_eta,MuPosGenStatus1_phi,MuPosGenStatus1_mass);
      muNegGen_status1.SetPtEtaPhiM(MuNegGenStatus1_pt,MuNegGenStatus1_eta,MuNegGenStatus1_phi,MuNegGenStatus1_mass);
      ZGen_status1 = muPosGen_status1 + muNegGen_status1;
      // mtv = sqrt(2*ptl1*ptl2*(1d0-cos(delphi)))
      double dphi = MuPosGenStatus1_phi - MuNegGenStatus1_phi; 
      if ( dphi > TMath::Pi() ) {
        dphi -= 2.0*TMath::Pi();
      } else if ( dphi <= -TMath::Pi() ) {
        dphi += 2.0*TMath::Pi();
      }
      double ZGen_status1_mT = TMath::Sqrt(2*MuPosGenStatus1_pt*MuNegGenStatus1_pt*(1-TMath::Cos(dphi)));
      // cout << "ZGen_status1_mT= " << ZGen_status1_mT << " ZGen_status1.Mt()= " << ZGen_status1.Mt() << endl;
      
      double vpt_rew_factor = hvpt_ewk_syst->Interpolate(ZGen_status1.Pt());
      double vmass_std_rew_factor = hmass_ewk_syst->Interpolate(ZGen_status1.M());
      double vmass_photos_rew_factor = hmass_ewk_syst_photos->Interpolate(ZGen_status1.M());
          
      // hmass->Fill(ZGen_PostFSR_mass);
      common_stuff::plot1D("hmass", ZGen_PostFSR_mass, 1, h_1d, 150,50,200 );
      common_stuff::plot1D("hNegleptonpt", muNegGen_status1.Pt(), 1, h_1d, 160,25,65 );
      common_stuff::plot1D("hPosleptonpt", muPosGen_status1.Pt(), 1, h_1d, 160,25,65 );
      common_stuff::plot1D("hvpt", ZGen_status1.Pt(), 1, h_1d, 300,0,300 );
      common_stuff::plot1D("hrapidity", ZGen_status1.Rapidity(), 1, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hNegleptoneta", muNegGen_status1.Eta(), 1, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hPosleptoneta", muPosGen_status1.Eta(), 1, h_1d, 25,-2.5,2.5 );
      // common_stuff::plot1D("hmT", ZGen_status1.Mt(), 1, h_1d, 100,50,100 );
      common_stuff::plot1D("hmT", ZGen_status1_mT, 1, h_1d, 100,50,100 );
      
      common_stuff::plot1D("hmass_VpTRewTo_ewk_p8std", ZGen_PostFSR_mass, vpt_rew_factor, h_1d, 150,50,200 );
      common_stuff::plot1D("hNegleptonpt_VpTRewTo_ewk_p8std", muNegGen_status1.Pt(), vpt_rew_factor, h_1d, 160,25,65 );
      common_stuff::plot1D("hPosleptonpt_VpTRewTo_ewk_p8std", muPosGen_status1.Pt(), vpt_rew_factor, h_1d, 160,25,65 );
      common_stuff::plot1D("hvpt_VpTRewTo_ewk_p8std", ZGen_status1.Pt(), vpt_rew_factor, h_1d, 300,0,300 );
      common_stuff::plot1D("hrapidity_VpTRewTo_ewk_p8std", ZGen_status1.Rapidity(), vpt_rew_factor, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hNegleptoneta_VpTRewTo_ewk_p8std", muNegGen_status1.Eta(), vpt_rew_factor, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hPosleptoneta_VpTRewTo_ewk_p8std", muPosGen_status1.Eta(), vpt_rew_factor, h_1d, 25,-2.5,2.5 );
      // common_stuff::plot1D("hmT_VpTRewTo_ewk_p8std", ZGen_status1.Mt(), vpt_rew_factor, h_1d, 100,50,100 );
      common_stuff::plot1D("hmT_VpTRewTo_ewk_p8std", ZGen_status1_mT, vpt_rew_factor, h_1d, 100,50,100 );
      
      common_stuff::plot1D("hmass_VpTRewTo_ewk_p8std_massStd", ZGen_PostFSR_mass, vpt_rew_factor*vmass_std_rew_factor, h_1d, 150,50,200 );
      common_stuff::plot1D("hNegleptonpt_VpTRewTo_ewk_p8std_massStd", muNegGen_status1.Pt(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 160,25,65 );
      common_stuff::plot1D("hPosleptonpt_VpTRewTo_ewk_p8std_massStd", muPosGen_status1.Pt(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 160,25,65 );
      common_stuff::plot1D("hvpt_VpTRewTo_ewk_p8std_massStd", ZGen_status1.Pt(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 300,0,300 );
      common_stuff::plot1D("hrapidity_VpTRewTo_ewk_p8std_massStd", ZGen_status1.Rapidity(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hNegleptoneta_VpTRewTo_ewk_p8std_massStd", muNegGen_status1.Eta(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hPosleptoneta_VpTRewTo_ewk_p8std_massStd", muPosGen_status1.Eta(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 25,-2.5,2.5 );
      // common_stuff::plot1D("hmT_VpTRewTo_ewk_p8std_massStd", ZGen_status1.Mt(), vpt_rew_factor*vmass_std_rew_factor, h_1d, 100,50,100 );
      common_stuff::plot1D("hmT_VpTRewTo_ewk_p8std_massStd", ZGen_status1_mT, vpt_rew_factor*vmass_std_rew_factor, h_1d, 100,50,100 );
      
      common_stuff::plot1D("hmass_VpTRewTo_ewk_p8std_massPhotos", ZGen_PostFSR_mass, vpt_rew_factor*vmass_photos_rew_factor, h_1d, 150,50,200 );
      common_stuff::plot1D("hNegleptonpt_VpTRewTo_ewk_p8std_massPhotos", muNegGen_status1.Pt(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 160,25,65 );
      common_stuff::plot1D("hPosleptonpt_VpTRewTo_ewk_p8std_massPhotos", muPosGen_status1.Pt(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 160,25,65 );
      common_stuff::plot1D("hvpt_VpTRewTo_ewk_p8std_massPhotos", ZGen_status1.Pt(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 300,0,300 );
      common_stuff::plot1D("hrapidity_VpTRewTo_ewk_p8std_massPhotos", ZGen_status1.Rapidity(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hNegleptoneta_VpTRewTo_ewk_p8std_massPhotos", muNegGen_status1.Eta(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 25,-2.5,2.5 );
      common_stuff::plot1D("hPosleptoneta_VpTRewTo_ewk_p8std_massPhotos", muPosGen_status1.Eta(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 25,-2.5,2.5 );
      // common_stuff::plot1D("hmT_VpTRewTo_ewk_p8std_massPhotos", ZGen_status1.Mt(), vpt_rew_factor*vmass_photos_rew_factor, h_1d, 100,50,100 );
      common_stuff::plot1D("hmT_VpTRewTo_ewk_p8std_massPhotos", ZGen_status1_mT, vpt_rew_factor*vmass_photos_rew_factor, h_1d, 100,50,100 );
    
    }
    
    costh_HX = -1e10;     phi_HX = -1e10;
    costh_HX_gen = -1e10; phi_HX_gen = -1e10;
    costh_CS = -1e10;     phi_CS = -1e10;
    costh_CS_gen = -1e10; phi_CS_gen = -1e10;
    
    muPosGen.SetPtEtaPhiM(MuPosGen_pt,MuPosGen_eta,MuPosGen_phi,MuPosGen_mass);
    muNegGen.SetPtEtaPhiM(MuNegGen_pt,MuNegGen_eta,MuNegGen_phi,MuNegGen_mass);
    ZGen_status3 = muPosGen + muNegGen;

    ComputeHXVar(muPosGen,muNegGen,true);
    common_stuff::plot2D(Form("phi_vs_costh_HX_ptbin%d_rapbin%d",hptbins->FindBin(ZGen_status3.Pt()),hrapbins->FindBin(TMath::Abs(ZGen_status3.Rapidity()))),
                        costh_HX_gen,TMath::Abs(phi_HX_gen), 1, 
                        h_2d, 12,-1,1,12,0,TMath::Pi() );

    ComputeCSVar(muPosGen,muNegGen,true);
    common_stuff::plot2D(Form("phi_vs_costh_CS_ptbin%d_rapbin%d",hptbins->FindBin(ZGen_status3.Pt()),hrapbins->FindBin(TMath::Abs(ZGen_status3.Rapidity()))),
                        costh_CS_gen,TMath::Abs(phi_CS_gen), 1, 
                        h_2d, 12,-1,1,12,0,TMath::Pi() );
    
    ComputeAllVarPietro(muPosGen,muNegGen);
    common_stuff::plot2D(Form("phi_vs_costh_HX_pietro_ptbin%d_rapbin%d",hptbins->FindBin(ZGen_status3.Pt()),hrapbins->FindBin(TMath::Abs(ZGen_status3.Rapidity()))),
                        costh_HX,TMath::Abs(phi_HX), 1, 
                        h_2d, 12,-1,1,12,0,TMath::Pi() );
    common_stuff::plot2D(Form("phi_vs_costh_CS_pietro_ptbin%d_rapbin%d",hptbins->FindBin(ZGen_status3.Pt()),hrapbins->FindBin(TMath::Abs(ZGen_status3.Rapidity()))),
                        costh_CS,TMath::Abs(phi_CS), 1, 
                        h_2d, 12,-1,1,12,0,TMath::Pi() );
                        
    // cout 
    // << "costh_HX_gen= " << costh_HX_gen
    // << " phi_HX_gen= " << phi_HX_gen
    // << " costh_CS_gen= " << costh_CS_gen
    // << " phi_CS_gen= " << phi_CS_gen
    // << endl;
    // cout 
    // << "costh_HX= " << costh_HX
    // << " phi_HX= " << phi_HX
    // << " costh_CS= " << costh_CS
    // << " phi_CS= " << phi_CS
    // << endl;
    
  }
  
  TFile*fout = new TFile("fout_mass_allGenEvts.root","RECREATE");

  common_stuff::writeOutHistos( fout, h_1d, h_2d );

  // hmass->Write();
  fout->Write();
  fout->Close();
  
}

void test_allEvt::ComputeHXVar(TLorentzVector muPos,TLorentzVector muNeg, bool isGen){
  
  if(muPos.Pt()<0 || muNeg.Pt()<0) return;
  
  // if(muPos.Pt()>muNeg.Pt()){
  GoToHXframe->TransformToHelicityFrame(muPos,muNeg);
  // }else{
  // GoToHXframe->TransformToHelicityFrame(muNeg,muPos);
  // }

  costh_HX_gen = GoToHXframe->GetCosTheta();
  phi_HX_gen = GoToHXframe->GetPhi();
  
}

void test_allEvt::ComputeCSVar(TLorentzVector muPos,TLorentzVector muNeg, bool isGen){
  
  if(muPos.Pt()<0 || muNeg.Pt()<0) return;
  
  // if(muPos.Pt()>muNeg.Pt()){
  GoToCSframe->TransformToCS(muPos,muNeg);
  // }else{
  // GoToCSframe->TransformToCS(muNeg,muPos);
  // }
  
  costh_CS_gen = GoToCSframe->GetCosTheta();
  phi_CS_gen = GoToCSframe->GetPhi();
  
}

void test_allEvt::ComputeAllVarPietro(TLorentzVector lepP,TLorentzVector lepN){
  // Preliminary definitions:

  const double pbeam = 7000.; // exact number irrelevant as long as pbeam >> Mprot
  const double Mprot = 0.9382720;
  const double Mlepton = 0.10566;  // (muon)
  const double gPI = TMath::Pi();
  const double Ebeam = sqrt( pbeam*pbeam + Mprot*Mprot );
  TLorentzVector beam1_LAB( 0., 0., pbeam, Ebeam );
  TLorentzVector beam2_LAB( 0., 0., -pbeam, Ebeam );


  // assuming that we have a TTree "data" (data ntuple) containing the 4-vectors lepP and lepN of lepton and antilepton

  // event by event (in some loop over dilepton events in the data ntuple):


  // data->GetEvent( i_event );

  // double lepP_pT  = lepP->Pt();
  // double lepN_pT  = lepN->Pt();

  // double lepP_eta = lepP->PseudoRapidity();
  // double lepN_eta = lepN->PseudoRapidity();

  // dilepton 4-vector:

  TLorentzVector dilepton = lepP + lepN;
  double pT   = dilepton.Pt();
  double rap  = dilepton.Rapidity();
  double mass = dilepton.M();

  // calculation of decay angles in three polarization frames

  // reference directions to calculate angles:

  TVector3 lab_to_dilep = -dilepton.BoostVector();

  TLorentzVector beam1_DILEP = beam1_LAB;
  beam1_DILEP.Boost(lab_to_dilep);         // beam1 in the dilepton rest frame
  TLorentzVector beam2_DILEP = beam2_LAB;
  beam2_DILEP.Boost(lab_to_dilep);         // beam2 in the dilepton rest frame

  TVector3 beam1_direction     = beam1_DILEP.Vect().Unit();
  TVector3 beam2_direction     = beam2_DILEP.Vect().Unit();
  TVector3 dilep_direction     = dilepton.Vect().Unit();
  TVector3 beam1_beam2_bisect  = ( beam1_direction - beam2_direction ).Unit();


  // all polarization frames have the same Y axis = the normal to the plane formed by
  // the directions of the colliding hadrons:

  TVector3 Yaxis = ( beam1_direction.Cross( beam2_direction ) ).Unit();

  // flip of y axis with rapidity:

  if ( rap < 0. ) Yaxis = - Yaxis;

  TVector3 perpendicular_to_beam = ( beam1_beam2_bisect.Cross( Yaxis ) ).Unit();


  // positive lepton in the dilepton rest frame:

  TLorentzVector lepton_DILEP = lepP;
  lepton_DILEP.Boost(lab_to_dilep);

  // CS frame angles:

  TVector3 newZaxis = beam1_beam2_bisect;
  TVector3 newYaxis = Yaxis;
  TVector3 newXaxis = newYaxis.Cross( newZaxis );

  TRotation rotation;
  rotation.SetToIdentity();
  rotation.RotateAxes( newXaxis, newYaxis, newZaxis );
  rotation.Invert();  // transforms coordinates from the "xyz" frame to the new frame
  TVector3 lepton_DILEP_rotated = lepton_DILEP.Vect();
  lepton_DILEP_rotated.Transform(rotation);

  /* double */ costh_CS = lepton_DILEP_rotated.CosTheta();
  /* double */ phi_CS   = lepton_DILEP_rotated.Phi() * 180. / gPI;
  double phith_CS;
  if ( costh_CS < 0. ) phith_CS = phi_CS - 135.;
  if ( costh_CS > 0. ) phith_CS = phi_CS - 45.;
  if ( phith_CS < -180. ) phith_CS = 360. + phith_CS;
  phi_CS   = phi_CS / 180. * gPI;


  // HELICITY frame angles:

  newZaxis = dilep_direction;
  newYaxis = Yaxis;
  newXaxis = newYaxis.Cross( newZaxis );

  rotation.SetToIdentity();
  rotation.RotateAxes( newXaxis, newYaxis, newZaxis );
  rotation.Invert();
  lepton_DILEP_rotated = lepton_DILEP.Vect();
  lepton_DILEP_rotated.Transform(rotation);

  /* double */ costh_HX = lepton_DILEP_rotated.CosTheta();
  /* double */ phi_HX   = lepton_DILEP_rotated.Phi() * 180. / gPI;
  double phith_HX;
  if ( costh_HX < 0. ) phith_HX = phi_HX - 135.;
  if ( costh_HX > 0. ) phith_HX = phi_HX - 45.;
  if ( phith_HX < -180. ) phith_HX = 360. + phith_HX;
  phi_HX   = phi_HX / 180. * gPI;


  // // PERPENDICULAR HELICITY frame angles:

  // newZaxis = perpendicular_to_beam;
  // newYaxis = Yaxis;
  // newXaxis = newYaxis.Cross( newZaxis );

  // rotation.SetToIdentity();
  // rotation.RotateAxes( newXaxis, newYaxis, newZaxis );
  // rotation.Invert();
  // lepton_DILEP_rotated = lepton_DILEP.Vect();
  // lepton_DILEP_rotated.Transform(rotation);

  // double costh_PX = lepton_DILEP_rotated.CosTheta();
  // double phi_PX   = lepton_DILEP_rotated.Phi() * 180. / gPI;
  // double phith_PX;
  // if ( costh_PX < 0. ) phith_PX = phi_PX - 135.;
  // if ( costh_PX > 0. ) phith_PX = phi_PX - 45.;
  // if ( phith_PX < -180. ) phith_PX = 360. + phith_PX;
  // phi_PX   = phi_PX / 180. * gPI;



}
