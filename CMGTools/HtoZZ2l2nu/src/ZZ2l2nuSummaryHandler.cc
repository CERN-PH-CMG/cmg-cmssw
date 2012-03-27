#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

using namespace std;

//
ZZ2l2nuSummaryHandler::ZZ2l2nuSummaryHandler()
{

}

//
bool ZZ2l2nuSummaryHandler::initTree(TTree *t, bool needsToRecreate)
{
  if(t==0) return false;
  t_ = t;
  if(!needsToRecreate) return true;
  
  //event info
  t_->Branch("run",        &evSummary_.run,        "run/I");
  t_->Branch("lumi",       &evSummary_.lumi,       "lumi/I");
  t_->Branch("event",      &evSummary_.event,      "event/I");
  t_->Branch("cat",        &evSummary_.cat,        "cat/I");
  t_->Branch("mccat",        &evSummary_.mccat,        "mccat/I");
  t_->Branch("hasTrigger",        &evSummary_.hasTrigger,        "hasTrigger/O");

  //vertices and average energy density
  t_->Branch("nvtx",       &evSummary_.nvtx,       "nvtx/I");
  t_->Branch("rho",        &evSummary_.rho,        "rho/F");
  t_->Branch("rho25",        &evSummary_.rho25,        "rho25/F");

  //generator level info
  t_->Branch("ngenITpu",   &evSummary_.ngenITpu,   "ngenITpu/I");
  t_->Branch("ngenOOTpu",  &evSummary_.ngenOOTpu,  "ngenOOTpu/I");
  t_->Branch("ngenOOTpum1",  &evSummary_.ngenOOTpum1,  "ngenOOTpum1/I");
  t_->Branch("puweight",     &evSummary_.puWeight,     "puweight/F");
  t_->Branch("hptWeights", evSummary_.hptWeights,  "hptWeights[5]/F");
  t_->Branch("pthat",      &evSummary_.pthat,      "pthat/F");
  t_->Branch("genWeight",  &evSummary_.genWeight,  "genWeight/F");
  t_->Branch("qscale",     &evSummary_.qscale,     "qscale/F");
  t_->Branch("x1",         &evSummary_.x1,         "x1/F");
  t_->Branch("x2",         &evSummary_.x2,         "x2/F");
  t_->Branch("id1",        &evSummary_.id1,        "id1/I");
  t_->Branch("id2",        &evSummary_.id2,        "id2/I");

  //selection info
  t_->Branch("pass",       &evSummary_.pass,       "pass/I");

  //selected di-leptons
  t_->Branch("l1_px",      &evSummary_.l1_px,      "l1_px/F");
  t_->Branch("l1_py",      &evSummary_.l1_py,      "l1_py/F");
  t_->Branch("l1_pz",      &evSummary_.l1_pz,      "l1_pz/F");
  t_->Branch("l1_en",      &evSummary_.l1_en,      "l1_en/F");
  t_->Branch("l1_pid",     &evSummary_.l1_pid,      "l1_pid/I");  
  t_->Branch("l1_id",      &evSummary_.l1_id,      "l1_id/I");
  t_->Branch("l1_genid",   &evSummary_.l1_genid,   "l1_genid/I");
  t_->Branch("l1_ptErr",   &evSummary_.l1_ptErr,   "l1_ptErr/F");
  t_->Branch("l1_ecalIso",    &evSummary_.l1_ecalIso,    "l1_ecalIso/F");
  t_->Branch("l1_hcalIso",    &evSummary_.l1_hcalIso,    "l1_hcalIso/F");
  t_->Branch("l1_trkIso",    &evSummary_.l1_trkIso,    "l1_trkIso/F");
  t_->Branch("l1_gIso",    &evSummary_.l1_gIso,    "l1_gIso/F");
  t_->Branch("l1_chIso",    &evSummary_.l1_chIso,    "l1_chIso/F");
  t_->Branch("l1_puchIso",    &evSummary_.l1_puchIso,    "l1_puchIso/F");
  t_->Branch("l1_nhIso",    &evSummary_.l1_nhIso,    "l1_nhIso/F");

  t_->Branch("l2_px",      &evSummary_.l2_px,      "l2_px/F");
  t_->Branch("l2_py",      &evSummary_.l2_py,      "l2_py/F");
  t_->Branch("l2_pz",      &evSummary_.l2_pz,      "l2_pz/F");
  t_->Branch("l2_en",      &evSummary_.l2_en,      "l2_en/F");
  t_->Branch("l2_pid",     &evSummary_.l2_pid,      "l2_pid/I");
  t_->Branch("l2_id",      &evSummary_.l2_id,      "l2_id/I");
  t_->Branch("l2_genid",   &evSummary_.l2_genid,   "l2_genid/I");
  t_->Branch("l2_ptErr",   &evSummary_.l2_ptErr,   "l2_ptErr/F");
  t_->Branch("l2_ecalIso",    &evSummary_.l2_ecalIso,    "l2_ecalIso/F");
  t_->Branch("l2_hcalIso",    &evSummary_.l2_hcalIso,    "l2_hcalIso/F");
  t_->Branch("l2_trkIso",    &evSummary_.l2_trkIso,    "l2_trkIso/F");
  t_->Branch("l2_gIso",    &evSummary_.l2_gIso,    "l2_gIso/F");
  t_->Branch("l2_chIso",    &evSummary_.l2_chIso,    "l2_chIso/F");
  t_->Branch("l2_puchIso",    &evSummary_.l2_puchIso,    "l2_puchIso/F");
  t_->Branch("l2_nhIso",    &evSummary_.l2_nhIso,    "l2_nhIso/F");

  //selected additional-leptons
  t_->Branch("ln",         &evSummary_.ln,         "ln/I");
  t_->Branch("ln_px",      evSummary_.ln_px,       "ln_px[ln]/F");
  t_->Branch("ln_py",      evSummary_.ln_py,       "ln_py[ln]/F");
  t_->Branch("ln_pz",      evSummary_.ln_pz,       "ln_pz[ln]/F");
  t_->Branch("ln_en",      evSummary_.ln_en,       "ln_en[ln]/F");
  t_->Branch("ln_pid",     evSummary_.ln_pid,      "ln_pid[ln]/I");
  t_->Branch("ln_id",      evSummary_.ln_id,       "ln_id[ln]/I");
  t_->Branch("ln_genid",   evSummary_.ln_genid,    "ln_genid[ln]/I");
  t_->Branch("ln_ptErr",   evSummary_.ln_ptErr,    "ln_ptErr[ln]/F");
  t_->Branch("ln_ecalIso",    evSummary_.ln_ecalIso,     "ln_ecalIso[ln]/F");
  t_->Branch("ln_hcalIso",    evSummary_.ln_hcalIso,     "ln_hcalIso[ln]/F");
  t_->Branch("ln_trkIso",    evSummary_.ln_trkIso,     "ln_trkIso[ln]/F");
  t_->Branch("ln_gIso",    evSummary_.ln_gIso,    "l_gIso[ln]/F");
  t_->Branch("ln_chIso",    evSummary_.ln_chIso,    "l_chIso[ln]/F");
  t_->Branch("ln_puchIso",   evSummary_.ln_puchIso,    "l_puchIso[ln]/F");
  t_->Branch("ln_nhIso",    evSummary_.ln_nhIso,    "l_nhIso[ln]/F");

  //selected jets
  t_->Branch("jn",         &evSummary_.jn,         "jn/I");
  t_->Branch("jn_px",      evSummary_.jn_px,       "jn_px[jn]/F");
  t_->Branch("jn_py",      evSummary_.jn_py,       "jn_py[jn]/F");
  t_->Branch("jn_pz",      evSummary_.jn_pz,       "jn_pz[jn]/F");
  t_->Branch("jn_en",      evSummary_.jn_en,       "jn_en[jn]/F");
  t_->Branch("jn_genid",   evSummary_.jn_genid,    "jn_genid[jn]/I");
  t_->Branch("jn_genflav",   evSummary_.jn_genflav,    "jn_genflav[jn]/I");
  t_->Branch("jn_btag1",   evSummary_.jn_btag1,    "jn_btag1[jn]/F");
  t_->Branch("jn_btag2",   evSummary_.jn_btag2,    "jn_btag2[jn]/F");
  t_->Branch("jn_neutHadFrac",  evSummary_.jn_neutHadFrac,  "jn_neutHadFrac[jn]/F");
  t_->Branch("jn_neutEmFrac",   evSummary_.jn_neutEmFrac,   "jn_neutEmFrac[jn]/F");
  t_->Branch("jn_chHadFrac",   evSummary_.jn_chHadFrac,    "jn_chHadFrac[jn]/F");
  t_->Branch("jn_pid",   evSummary_.jn_pid,    "jn_pid[jn]/F");
  t_->Branch("jn_genpt",   evSummary_.jn_genpt,    "jn_genpt[jn]/F");
  t_->Branch("jn_pumva",   evSummary_.jn_pumva,    "jn_pumva[jn]/F");
  t_->Branch("htvec_px",   &evSummary_.htvec_px, "htvec_px/F");
  t_->Branch("htvec_py",   &evSummary_.htvec_py, "htvec_py/F");
  t_->Branch("htvec_pz",   &evSummary_.htvec_pz, "htvec_pz/F");
  t_->Branch("htvec_en",   &evSummary_.htvec_en, "htvec_en/F");

  //selected associated jets 
  t_->Branch("ajn",         &evSummary_.ajn,         "ajn/I");
  t_->Branch("ajn_px",      evSummary_.ajn_px,       "ajn_px[ajn]/F");
  t_->Branch("ajn_py",      evSummary_.ajn_py,       "ajn_py[ajn]/F");
  t_->Branch("ajn_pz",      evSummary_.ajn_pz,       "ajn_pz[ajn]/F");
  t_->Branch("ajn_en",      evSummary_.ajn_en,       "ajn_en[ajn]/F");
  t_->Branch("ajn_genid",   evSummary_.ajn_genid,    "ajn_genid[ajn]/I");
  t_->Branch("ajn_genflav",   evSummary_.ajn_genflav,    "ajn_genflav[ajn]/I");
  t_->Branch("ajn_btag1",   evSummary_.ajn_btag1,    "ajn_btag1[ajn]/F");
  t_->Branch("ajn_btag2",   evSummary_.ajn_btag2,    "ajn_btag2[ajn]/F");
  t_->Branch("ajn_neutHadFrac",  evSummary_.ajn_neutHadFrac,  "ajn_neutHadFrac[ajn]/F");
  t_->Branch("ajn_neutEmFrac",   evSummary_.ajn_neutEmFrac,   "ajn_neutEmFrac[ajn]/F");
  t_->Branch("ajn_chHadFrac",   evSummary_.ajn_chHadFrac,    "ajn_chHadFrac[ajn]/F");
  t_->Branch("ajn_pid",   evSummary_.ajn_pid,    "ajn_pid[ajn]/F");
  t_->Branch("ajn_pumva",   evSummary_.ajn_pumva,    "ajn_pumva[jn]/F");

  //primary vertex
  t_->Branch("vtx_px",    &evSummary_.vtx_px,      "vtx_px/F");
  t_->Branch("vtx_py",    &evSummary_.vtx_py,      "vtx_py/F");
  t_->Branch("vtx_pz",    &evSummary_.vtx_pz,      "vtx_pz/F");
  t_->Branch("vtx_en",    &evSummary_.vtx_en,      "vtx_en/F");

  //MET
  t_->Branch("nmet",  &evSummary_.nmet, "nmet/I");
  t_->Branch("met_phi",     evSummary_.met_phi,    "met_phi[nmet]/F");
  t_->Branch("met_pt",      evSummary_.met_pt,     "met_pt[nmet]/F");

  t_->Branch("sumEt",               &evSummary_.sumEt,            "sumEt/F");
  t_->Branch("sumEtcentral",        &evSummary_.sumEtcentral,     "sumEtcentral/F");
  t_->Branch("chsumEt",             &evSummary_.chsumEt,          "chsumEt/F");
  t_->Branch("chsumEtcentral",      &evSummary_.chsumEtcentral,   "chsumEtcentral/F");
  t_->Branch("neutsumEt",           &evSummary_.neutsumEt,        "neutsumEt/F");
  t_->Branch("neutsumEtcentral",    &evSummary_.neutsumEtcentral, "neutsumEtcentral/F");

  t_->Branch("primVertexSumEt",         &evSummary_.primVertexSumEt,      "primVertexSumEt/F");
  t_->Branch("otherVertexSumEt",        &evSummary_.otherVertexSumEt,     "otherVertexSumEt/F");
  t_->Branch("primVertexChSumEt",       &evSummary_.primVertexChSumEt,    "primVertexChSumEt/F");
  t_->Branch("otherVertexChSumEt",      &evSummary_.otherVertexChSumEt,   "otherVertexChSumEt/F");
  t_->Branch("primVertexNeutSumEt",     &evSummary_.primVertexNeutSumEt,  "primVertexNeutSumEt/F");
  t_->Branch("otherVertexNeutSumEt",    &evSummary_.otherVertexNeutSumEt, "otherVertexNeutSumEt/F");

  //Gamma Info
  t_->Branch("gn",        &evSummary_.gn,         "gn/I"); 
  t_->Branch("g_px",      evSummary_.g_px,        "g_px[gn]/F");
  t_->Branch("g_py",      evSummary_.g_py,        "g_py[gn]/F");
  t_->Branch("g_pz",      evSummary_.g_pz,        "g_pz[gn]/F");
  t_->Branch("g_en",      evSummary_.g_en,        "g_en[gn]/F");
  t_->Branch("g_iso1",    evSummary_.g_iso1,      "g_iso1[gn]/F");
  t_->Branch("g_iso2",    evSummary_.g_iso2,      "g_iso2[gn]/F");
  t_->Branch("g_iso3",    evSummary_.g_iso3,      "g_iso3[gn]/F");
  t_->Branch("g_sihih",   evSummary_.g_sihih,     "g_sihih[gn]/F");
  t_->Branch("g_r9",      evSummary_.g_r9,        "g_r9[gn]/F");
  t_->Branch("g_mva",      evSummary_.g_mva,        "g_mva[gn]/F");
  t_->Branch("g_mvaNeut",      evSummary_.g_mvaNeut,        "g_mvaNeut[gn]/F");
  t_->Branch("g_trkVeto",  evSummary_.g_trkVeto, "g_trkVeto[gn]/F");
  t_->Branch("g_conv",    evSummary_.g_conv,      "g_conv[gn]/O");
  t_->Branch("g_conv_px", evSummary_.g_conv_px,   "g_conv_px[gn]/F");
  t_->Branch("g_conv_py", evSummary_.g_conv_py,   "g_conv_py[gn]/F");
  t_->Branch("g_conv_pz", evSummary_.g_conv_pz,   "g_conv_pz[gn]/F");
  t_->Branch("g_conv_en", evSummary_.g_conv_en,   "g_conv_en[gn]/F");

  //Higgs Info
  t_->Branch("h_px",      &evSummary_.h_px,        "h_px/F");
  t_->Branch("h_py",      &evSummary_.h_py,        "h_py/F");
  t_->Branch("h_pz",      &evSummary_.h_pz,        "h_pz/F");
  t_->Branch("h_en",      &evSummary_.h_en,        "h_en/F");

  //mc truth
  t_->Branch("nmcparticles", &evSummary_.nmcparticles, "nmcparticles/I");
  t_->Branch("mc_px", evSummary_.mc_px, "mc_px[nmcparticles]/F");
  t_->Branch("mc_py", evSummary_.mc_py, "mc_py[nmcparticles]/F");
  t_->Branch("mc_pz", evSummary_.mc_pz, "mc_pz[nmcparticles]/F");
  t_->Branch("mc_en", evSummary_.mc_en, "mc_en[nmcparticles]/F");
  t_->Branch("mc_id", evSummary_.mc_id, "mc_id[nmcparticles]/I");


  return true;
}

//
bool ZZ2l2nuSummaryHandler::attachToTree(TTree *t)
{
  if(t==0) return false;
  t_ = t;

  //event info
  t_->GetBranch("run")->SetAddress(&evSummary_.run);
  t_->GetBranch("lumi")->SetAddress(&evSummary_.lumi);
  t_->GetBranch("event")->SetAddress(&evSummary_.event);
  t_->GetBranch("cat")->SetAddress(&evSummary_.cat);
  t_->GetBranch("mccat")->SetAddress(&evSummary_.mccat);

  //trigger bit
  t_->GetBranch("hasTrigger")->SetAddress(&evSummary_.hasTrigger);

  //vertices and average energy density
  t_->GetBranch("nvtx")->SetAddress(&evSummary_.nvtx);
  t_->GetBranch("rho")->SetAddress( &evSummary_.rho );
  if(t_->GetBranch("rho25")) t_->GetBranch("rho25")->SetAddress( &evSummary_.rho25 );

  //generator level info
  t_->GetBranch("puweight")->SetAddress(&evSummary_.puWeight);
  t_->GetBranch("hptWeights")->SetAddress(evSummary_.hptWeights);
  t_->GetBranch("ngenITpu")->SetAddress(&evSummary_.ngenITpu);
  t_->GetBranch("ngenOOTpu")->SetAddress(&evSummary_.ngenOOTpu);
  t_->GetBranch("ngenOOTpum1")->SetAddress(&evSummary_.ngenOOTpum1);
  t_->GetBranch("pthat")->SetAddress(&evSummary_.pthat);
  t_->GetBranch("genWeight")->SetAddress(&evSummary_.genWeight);
  t_->GetBranch("qscale")->SetAddress(&evSummary_.qscale);
  t_->GetBranch("x1")->SetAddress(&evSummary_.x1);
  t_->GetBranch("x2")->SetAddress(&evSummary_.x2);
  t_->GetBranch("id1")->SetAddress(&evSummary_.id1);
  t_->GetBranch("id2")->SetAddress(&evSummary_.id2);

  //selection info
  t_->GetBranch("pass")->SetAddress(&evSummary_.pass);
  
  //selected di-leptons
  t_->GetBranch("l1_px")     ->SetAddress( &evSummary_.l1_px);
  t_->GetBranch("l1_py")     ->SetAddress( &evSummary_.l1_py);
  t_->GetBranch("l1_pz")     ->SetAddress( &evSummary_.l1_pz);
  t_->GetBranch("l1_en")     ->SetAddress( &evSummary_.l1_en);
  t_->GetBranch("l1_id")     ->SetAddress( &evSummary_.l1_id);
  t_->GetBranch("l1_pid")     ->SetAddress( &evSummary_.l1_pid);
  t_->GetBranch("l1_genid")  ->SetAddress( &evSummary_.l1_genid);
  t_->GetBranch("l1_ptErr")  ->SetAddress( &evSummary_.l1_ptErr);
  t_->GetBranch("l1_ecalIso")   ->SetAddress( &evSummary_.l1_ecalIso);
  t_->GetBranch("l1_hcalIso")   ->SetAddress( &evSummary_.l1_hcalIso);
  t_->GetBranch("l1_trkIso")   ->SetAddress( &evSummary_.l1_trkIso);
  t_->GetBranch("l1_gIso")   ->SetAddress( &evSummary_.l1_gIso);
  t_->GetBranch("l1_chIso")   ->SetAddress( &evSummary_.l1_chIso);
  t_->GetBranch("l1_puchIso")   ->SetAddress( &evSummary_.l1_puchIso);
  t_->GetBranch("l1_nhIso")   ->SetAddress( &evSummary_.l1_nhIso);
  
  t_->GetBranch("l2_px")     ->SetAddress( &evSummary_.l2_px);
  t_->GetBranch("l2_py")     ->SetAddress( &evSummary_.l2_py);
  t_->GetBranch("l2_pz")     ->SetAddress( &evSummary_.l2_pz);
  t_->GetBranch("l2_en")     ->SetAddress( &evSummary_.l2_en);
  t_->GetBranch("l2_pid")     ->SetAddress( &evSummary_.l2_pid);
  t_->GetBranch("l2_id")     ->SetAddress( &evSummary_.l2_id);
  t_->GetBranch("l2_genid")  ->SetAddress( &evSummary_.l2_genid);
  t_->GetBranch("l2_ptErr")  ->SetAddress( &evSummary_.l2_ptErr);
  t_->GetBranch("l2_ecalIso")   ->SetAddress( &evSummary_.l2_ecalIso);
  t_->GetBranch("l2_hcalIso")   ->SetAddress( &evSummary_.l2_hcalIso);
  t_->GetBranch("l2_trkIso")   ->SetAddress( &evSummary_.l2_trkIso);
  t_->GetBranch("l2_gIso")   ->SetAddress( &evSummary_.l2_gIso);
  t_->GetBranch("l2_chIso")   ->SetAddress( &evSummary_.l2_chIso);
  t_->GetBranch("l2_puchIso")   ->SetAddress( &evSummary_.l2_puchIso);
  t_->GetBranch("l2_nhIso")   ->SetAddress( &evSummary_.l2_nhIso);  

  //selected additional-leptons
  t_->GetBranch("ln")        ->SetAddress( &evSummary_.ln);
  t_->GetBranch("ln_px")     ->SetAddress(evSummary_.ln_px);
  t_->GetBranch("ln_py")     ->SetAddress(evSummary_.ln_py);
  t_->GetBranch("ln_pz")     ->SetAddress(evSummary_.ln_pz);
  t_->GetBranch("ln_en")     ->SetAddress(evSummary_.ln_en);
  t_->GetBranch("ln_pid")     ->SetAddress(evSummary_.ln_pid);
  t_->GetBranch("ln_id")     ->SetAddress(evSummary_.ln_id);
  t_->GetBranch("ln_genid")  ->SetAddress(evSummary_.ln_genid);
  t_->GetBranch("ln_ptErr")  ->SetAddress(evSummary_.ln_ptErr);
  t_->GetBranch("ln_ecalIso")   ->SetAddress(evSummary_.ln_ecalIso);
  t_->GetBranch("ln_hcalIso")   ->SetAddress(evSummary_.ln_hcalIso);
  t_->GetBranch("ln_trkIso")   ->SetAddress(evSummary_.ln_trkIso);
  t_->GetBranch("ln_gIso")   ->SetAddress(evSummary_.ln_gIso);
  t_->GetBranch("ln_chIso")   ->SetAddress( &evSummary_.ln_chIso);
  t_->GetBranch("ln_puchIso")   ->SetAddress( &evSummary_.ln_puchIso);
  t_->GetBranch("ln_nhIso")   ->SetAddress( &evSummary_.ln_nhIso);
  
  //selected jets
  t_->GetBranch("jn")        ->SetAddress( &evSummary_.jn);
  t_->GetBranch("jn_px")     ->SetAddress(evSummary_.jn_px);
  t_->GetBranch("jn_py")     ->SetAddress(evSummary_.jn_py);
  t_->GetBranch("jn_pz")     ->SetAddress(evSummary_.jn_pz);
  t_->GetBranch("jn_en")     ->SetAddress(evSummary_.jn_en);
  t_->GetBranch("jn_genid")  ->SetAddress(evSummary_.jn_genid);
  t_->GetBranch("jn_genflav")->SetAddress(evSummary_.jn_genflav);
  t_->GetBranch("jn_btag1")  ->SetAddress(evSummary_.jn_btag1);
  t_->GetBranch("jn_btag2")  ->SetAddress(evSummary_.jn_btag2);
  t_->GetBranch("jn_neutHadFrac") ->SetAddress(evSummary_.jn_neutHadFrac);
  t_->GetBranch("jn_neutEmFrac") ->SetAddress(evSummary_.jn_neutEmFrac);
  t_->GetBranch("jn_chHadFrac") ->SetAddress(evSummary_.jn_chHadFrac);
  t_->GetBranch("jn_pid") ->SetAddress(evSummary_.jn_pid);
  if(t_->GetBranch("jn_genpt")) t_->GetBranch("jn_genpt")->SetAddress(evSummary_.jn_genpt);
  if(t_->GetBranch("jn_pumva")) t_->GetBranch("jn_pumva")->SetAddress(evSummary_.jn_pumva);
  if(t_->GetBranch("htvec_px")) t_->GetBranch("htvec_px")->SetAddress(&evSummary_.htvec_px);
  if(t_->GetBranch("htvec_py")) t_->GetBranch("htvec_py")->SetAddress(&evSummary_.htvec_py);
  if(t_->GetBranch("htvec_pz")) t_->GetBranch("htvec_pz")->SetAddress(&evSummary_.htvec_pz);
  if(t_->GetBranch("htvec_en")) t_->GetBranch("htvec_en")->SetAddress(&evSummary_.htvec_en);

  //selected jets
  t_->GetBranch("ajn")        ->SetAddress( &evSummary_.ajn);
  t_->GetBranch("ajn_px")     ->SetAddress(evSummary_.ajn_px);
  t_->GetBranch("ajn_py")     ->SetAddress(evSummary_.ajn_py);
  t_->GetBranch("ajn_pz")     ->SetAddress(evSummary_.ajn_pz);
  t_->GetBranch("ajn_en")     ->SetAddress(evSummary_.ajn_en);
  t_->GetBranch("ajn_genid")  ->SetAddress(evSummary_.ajn_genid);
  t_->GetBranch("ajn_genflav")->SetAddress(evSummary_.ajn_genflav);
  t_->GetBranch("ajn_btag1")  ->SetAddress(evSummary_.ajn_btag1);
  t_->GetBranch("ajn_btag2")  ->SetAddress(evSummary_.ajn_btag2);
  t_->GetBranch("ajn_neutHadFrac") ->SetAddress(evSummary_.ajn_neutHadFrac);
  t_->GetBranch("ajn_neutEmFrac") ->SetAddress(evSummary_.ajn_neutEmFrac);
  t_->GetBranch("ajn_chHadFrac") ->SetAddress(evSummary_.ajn_chHadFrac);
  t_->GetBranch("ajn_pid") ->SetAddress(evSummary_.ajn_pid);
  if(t_->GetBranch("ajn_genpt")) t_->GetBranch("ajn_genpt")->SetAddress(evSummary_.ajn_genpt);
  if(t_->GetBranch("ajn_pumva")) t_->GetBranch("ajn_pumva")->SetAddress(evSummary_.ajn_pumva);
  
  //primary vertex
  t_->GetBranch("vtx_px")   ->SetAddress( &evSummary_.vtx_px);
  t_->GetBranch("vtx_py")   ->SetAddress( &evSummary_.vtx_py);
  t_->GetBranch("vtx_pz")   ->SetAddress( &evSummary_.vtx_pz);
  t_->GetBranch("vtx_en")   ->SetAddress( &evSummary_.vtx_en);
  
  //MET
  t_->GetBranch("nmet")  ->SetAddress( &evSummary_.nmet);
  t_->GetBranch("met_phi")  ->SetAddress( evSummary_.met_phi);
  t_->GetBranch("met_pt")  ->SetAddress( evSummary_.met_pt);

  t_->GetBranch("sumEt")->SetAddress(&evSummary_.sumEt);
  t_->GetBranch("sumEtcentral")->SetAddress(&evSummary_.sumEtcentral);
  t_->GetBranch("chsumEt")->SetAddress(&evSummary_.chsumEt);
  t_->GetBranch("chsumEtcentral")->SetAddress(&evSummary_.chsumEtcentral);
  t_->GetBranch("neutsumEt")->SetAddress(&evSummary_.neutsumEt);
  t_->GetBranch("neutsumEtcentral")->SetAddress(&evSummary_.neutsumEtcentral);
  t_->GetBranch("primVertexSumEt")->SetAddress(&evSummary_.primVertexSumEt);
  t_->GetBranch("otherVertexSumEt")->SetAddress(&evSummary_.otherVertexSumEt);
  t_->GetBranch("primVertexChSumEt")->SetAddress(&evSummary_.primVertexChSumEt);
  t_->GetBranch("otherVertexChSumEt")->SetAddress(&evSummary_.otherVertexChSumEt);
  t_->GetBranch("primVertexNeutSumEt")->SetAddress(&evSummary_.primVertexNeutSumEt);
  t_->GetBranch("otherVertexNeutSumEt")->SetAddress(&evSummary_.otherVertexNeutSumEt);
  
  //Gamma Info
  t_->GetBranch("gn")->SetAddress(&evSummary_.gn);
  t_->GetBranch("g_px")->SetAddress(evSummary_.g_px);
  t_->GetBranch("g_py")->SetAddress(evSummary_.g_py);
  t_->GetBranch("g_pz")->SetAddress(evSummary_.g_pz);
  t_->GetBranch("g_en")->SetAddress(evSummary_.g_en);
  t_->GetBranch("g_iso1")->SetAddress(evSummary_.g_iso1);
  t_->GetBranch("g_iso2")->SetAddress(evSummary_.g_iso2);
  t_->GetBranch("g_iso3")->SetAddress(evSummary_.g_iso3);
  t_->GetBranch("g_sihih")->SetAddress(evSummary_.g_sihih);
  t_->GetBranch("g_r9")->SetAddress(evSummary_.g_r9);
  if(t_->GetBranch("g_mva"))     t_->GetBranch("g_mva")->SetAddress(evSummary_.g_mva);
  if(t_->GetBranch("g_mvaNeut")) t_->GetBranch("g_mvaNeut")->SetAddress(evSummary_.g_mvaNeut);
  if(t_->GetBranch("g_trkVeto")) t_->GetBranch("g_trkVeto")->SetAddress(evSummary_.g_trkVeto);
  t_->GetBranch("g_conv")->SetAddress( evSummary_.g_conv );
  t_->GetBranch("g_conv_px")->SetAddress( evSummary_.g_conv_px );
  t_->GetBranch("g_conv_py")->SetAddress( evSummary_.g_conv_py );
  t_->GetBranch("g_conv_pz")->SetAddress( evSummary_.g_conv_pz );
  t_->GetBranch("g_conv_en")->SetAddress( evSummary_.g_conv_en );

  //Higgs Info
  t_->GetBranch("h_px")     ->SetAddress( &evSummary_.h_px);
  t_->GetBranch("h_py")     ->SetAddress( &evSummary_.h_py);
  t_->GetBranch("h_pz")     ->SetAddress( &evSummary_.h_pz);
  t_->GetBranch("h_en")     ->SetAddress( &evSummary_.h_en);
  
  //mc truth
  t_->GetBranch("nmcparticles")->SetAddress( &evSummary_.nmcparticles);
  t_->GetBranch("mc_px")->SetAddress(evSummary_.mc_px);
  t_->GetBranch("mc_py")->SetAddress(evSummary_.mc_py);
  t_->GetBranch("mc_pz")->SetAddress(evSummary_.mc_pz);
  t_->GetBranch("mc_en")->SetAddress(evSummary_.mc_en);
  t_->GetBranch("mc_id")->SetAddress(evSummary_.mc_id);
  
  return true;
}

//
void ZZ2l2nuSummaryHandler::resetStruct()
{
  evSummary_.nmcparticles=0;
  evSummary_.run=0;    evSummary_.lumi=0;   evSummary_.event=0;  evSummary_.cat=0;
  evSummary_.l1_px=0;  evSummary_.l1_py=0;  evSummary_.l1_pz=0;  evSummary_.l1_en=0;
  evSummary_.l2_px=0;  evSummary_.l2_py=0;  evSummary_.l2_pz=0;  evSummary_.l2_en=0;
  evSummary_.ln=0;
  evSummary_.jn=0;   evSummary_.ajn=0;
  evSummary_.gn=0;
  evSummary_.nmet=0;
}

//
void ZZ2l2nuSummaryHandler::fillTree()
{
  if(t_) t_->Fill();
}

//
ZZ2l2nuSummaryHandler::~ZZ2l2nuSummaryHandler()
{
}
