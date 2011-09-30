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

  //generator level info
  t_->Branch("ngenITpu",   &evSummary_.ngenITpu,   "ngenITpu/I");
  t_->Branch("ngenOOTpu",  &evSummary_.ngenOOTpu,  "ngenOOTpu/I");
  t_->Branch("weight",     &evSummary_.weight,     "weight/F");
  t_->Branch("normWeight", &evSummary_.normWeight, "normWeight/F");
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
  t_->Branch("l1_iso1",    &evSummary_.l1_iso1,    "l1_iso1/F");
  t_->Branch("l1_iso2",    &evSummary_.l1_iso2,    "l1_iso2/F");
  t_->Branch("l1_iso3",    &evSummary_.l1_iso3,    "l1_iso3/F");

  t_->Branch("l2_px",      &evSummary_.l2_px,      "l2_px/F");
  t_->Branch("l2_py",      &evSummary_.l2_py,      "l2_py/F");
  t_->Branch("l2_pz",      &evSummary_.l2_pz,      "l2_pz/F");
  t_->Branch("l2_en",      &evSummary_.l2_en,      "l2_en/F");
  t_->Branch("l2_pid",     &evSummary_.l2_pid,      "l2_pid/I");
  t_->Branch("l2_id",      &evSummary_.l2_id,      "l2_id/I");
  t_->Branch("l2_genid",   &evSummary_.l2_genid,   "l2_genid/I");
  t_->Branch("l2_ptErr",   &evSummary_.l2_ptErr,   "l2_ptErr/F");
  t_->Branch("l2_iso1",    &evSummary_.l2_iso1,    "l2_iso1/F");
  t_->Branch("l2_iso2",    &evSummary_.l2_iso2,    "l2_iso2/F");
  t_->Branch("l2_iso3",    &evSummary_.l2_iso3,    "l2_iso3/F");

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
  t_->Branch("ln_iso1",    evSummary_.ln_iso1,     "ln_iso1[ln]/F");
  t_->Branch("ln_iso2",    evSummary_.ln_iso2,     "ln_iso2[ln]/F");
  t_->Branch("ln_iso3",    evSummary_.ln_iso3,     "ln_iso3[ln]/F");

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
  t_->Branch("jn_btag3",   evSummary_.jn_btag3,    "jn_btag3[jn]/F");
  t_->Branch("jn_btag4",   evSummary_.jn_btag4,    "jn_btag4[jn]/F");
  t_->Branch("jn_vtxAssoc",evSummary_.jn_vtxAssoc, "jn_vtxAssoc[jn]/O");

  //primary vertex
  t_->Branch("vtx_px",    &evSummary_.vtx_px,      "vtx_px/F");
  t_->Branch("vtx_py",    &evSummary_.vtx_py,      "vtx_py/F");
  t_->Branch("vtx_pz",    &evSummary_.vtx_pz,      "vtx_pz/F");
  t_->Branch("vtx_en",    &evSummary_.vtx_en,      "vtx_en/F");

  //MET
  t_->Branch("met1_phi",     &evSummary_.met1_phi,     "met1_phi/F");
  t_->Branch("met1_pt",      &evSummary_.met1_pt,     "met1_pt/F");

  t_->Branch("met2_phi",      &evSummary_.met2_phi,     "met2_phi/F");
  t_->Branch("met2_pt",      &evSummary_.met2_pt,     "met2_pt/F");

  t_->Branch("met3_phi",      &evSummary_.met3_phi,     "met3_phi/F");
  t_->Branch("met3_pt",      &evSummary_.met3_pt,     "met3_pt/F");

  t_->Branch("met4_phi",      &evSummary_.met4_phi,     "met4_phi/F");
  t_->Branch("met4_pt",      &evSummary_.met4_pt,     "met4_pt/F");

  t_->Branch("met5_phi",      &evSummary_.met5_phi,     "met5_phi/F");
  t_->Branch("met5_pt",      &evSummary_.met5_pt,     "met5_pt/F");

  t_->Branch("met6_phi",      &evSummary_.met6_phi,     "met6_phi/F");
  t_->Branch("met6_pt",      &evSummary_.met6_pt,     "met6_pt/F");

  t_->Branch("met7_phi",      &evSummary_.met7_phi,     "met7_phi/F");
  t_->Branch("met7_pt",      &evSummary_.met7_pt,     "met7_pt/F");

  t_->Branch("met8_phi",      &evSummary_.met8_phi,     "met8_phi/F");
  t_->Branch("met8_pt",      &evSummary_.met8_pt,     "met8_pt/F");

  t_->Branch("met9_phi",      &evSummary_.met9_phi,     "met9_phi/F");
  t_->Branch("met9_pt",      &evSummary_.met9_pt,     "met9_pt/F");

  t_->Branch("met10_phi",      &evSummary_.met10_phi,     "met10_phi/F");
  t_->Branch("met10_pt",      &evSummary_.met10_pt,     "met10_pt/F");
  
  t_->Branch("met11_phi",      &evSummary_.met11_phi,     "met11_phi/F");
  t_->Branch("met11_pt",      &evSummary_.met11_pt,     "met11_pt/F");
  
  t_->Branch("met12_phi",      &evSummary_.met12_phi,     "met12_phi/F");
  t_->Branch("met12_pt",      &evSummary_.met12_pt,     "met12_pt/F");
  
  t_->Branch("met13_phi",      &evSummary_.met13_phi,     "met13_phi/F");
  t_->Branch("met13_pt",      &evSummary_.met13_pt,     "met13_pt/F");

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
  t_->Branch("g_px",      &evSummary_.g_px,        "g_px/F");
  t_->Branch("g_py",      &evSummary_.g_py,        "g_py/F");
  t_->Branch("g_pz",      &evSummary_.g_pz,        "g_pz/F");
  t_->Branch("g_en",      &evSummary_.g_en,        "g_en/F");
  t_->Branch("g_iso1",    &evSummary_.g_iso1,      "g_iso1/F");
  t_->Branch("g_iso2",    &evSummary_.g_iso2,      "g_iso2/F");
  t_->Branch("g_iso3",    &evSummary_.g_iso3,      "g_iso3/F");
  t_->Branch("g_sihih",   &evSummary_.g_sihih,     "g_sihih/F");
  t_->Branch("g_sipip",   &evSummary_.g_sipip,     "g_sipip/F");
  t_->Branch("g_r9",      &evSummary_.g_r9,        "g_r9/F");
  t_->Branch("g_eop",     &evSummary_.g_eop,       "g_eop/F");

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
  if(t_->GetBranch("mccat")) t_->GetBranch("mccat")->SetAddress(&evSummary_.mccat);

  //trigger bit
  if(t_->GetBranch("hasTrigger")) t_->GetBranch("hasTrigger")->SetAddress(&evSummary_.hasTrigger);

  //vertices and average energy density
  t_->GetBranch("nvtx")->SetAddress(&evSummary_.nvtx);
  t_->GetBranch("rho")->SetAddress( &evSummary_.rho );

  //generator level info
  t_->GetBranch("weight")->SetAddress(&evSummary_.weight);
  t_->GetBranch("normWeight")->SetAddress(&evSummary_.normWeight);
  t_->GetBranch("hptWeights")->SetAddress(evSummary_.hptWeights);
  t_->GetBranch("ngenITpu")->SetAddress(&evSummary_.ngenITpu);
  t_->GetBranch("ngenOOTpu")->SetAddress(&evSummary_.ngenOOTpu);
  t_->GetBranch("pthat")->SetAddress(&evSummary_.pthat);
  t_->GetBranch("genWeight")->SetAddress(&evSummary_.genWeight);
  t_->GetBranch("qscale")->SetAddress(&evSummary_.qscale);
  t_->GetBranch("x1")->SetAddress(&evSummary_.x1);
  t_->GetBranch("x2")->SetAddress(&evSummary_.x2);
  t_->GetBranch("id1")->SetAddress(&evSummary_.id1);
  t_->GetBranch("id2")->SetAddress(&evSummary_.id2);

  //selection info
  if(t_->GetBranch("pass")) t_->GetBranch("pass")->SetAddress(&evSummary_.pass);
  
  //selected di-leptons
  t_->GetBranch("l1_px")     ->SetAddress( &evSummary_.l1_px);
  t_->GetBranch("l1_py")     ->SetAddress( &evSummary_.l1_py);
  t_->GetBranch("l1_pz")     ->SetAddress( &evSummary_.l1_pz);
  t_->GetBranch("l1_en")     ->SetAddress( &evSummary_.l1_en);
  t_->GetBranch("l1_id")     ->SetAddress( &evSummary_.l1_id);
  if(t_->GetBranch("l1_pid")) t_->GetBranch("l1_pid")     ->SetAddress( &evSummary_.l1_pid);
  t_->GetBranch("l1_genid")  ->SetAddress( &evSummary_.l1_genid);
  t_->GetBranch("l1_ptErr")  ->SetAddress( &evSummary_.l1_ptErr);
  t_->GetBranch("l1_iso1")   ->SetAddress( &evSummary_.l1_iso1);
  t_->GetBranch("l1_iso2")   ->SetAddress( &evSummary_.l1_iso2);
  t_->GetBranch("l1_iso3")   ->SetAddress( &evSummary_.l1_iso3);
  
  t_->GetBranch("l2_px")     ->SetAddress( &evSummary_.l2_px);
  t_->GetBranch("l2_py")     ->SetAddress( &evSummary_.l2_py);
  t_->GetBranch("l2_pz")     ->SetAddress( &evSummary_.l2_pz);
  t_->GetBranch("l2_en")     ->SetAddress( &evSummary_.l2_en);
  if(t_->GetBranch("l2_pid")) t_->GetBranch("l2_pid")     ->SetAddress( &evSummary_.l2_pid);
  t_->GetBranch("l2_id")     ->SetAddress( &evSummary_.l2_id);
  t_->GetBranch("l2_genid")  ->SetAddress( &evSummary_.l2_genid);
  t_->GetBranch("l2_ptErr")  ->SetAddress( &evSummary_.l2_ptErr);
  t_->GetBranch("l2_iso1")   ->SetAddress( &evSummary_.l2_iso1);
  t_->GetBranch("l2_iso2")   ->SetAddress( &evSummary_.l2_iso2);
  t_->GetBranch("l2_iso3")   ->SetAddress( &evSummary_.l2_iso3);
  
  //selected additional-leptons
  t_->GetBranch("ln")        ->SetAddress( &evSummary_.ln);
  t_->GetBranch("ln_px")     ->SetAddress(evSummary_.ln_px);
  t_->GetBranch("ln_py")     ->SetAddress(evSummary_.ln_py);
  t_->GetBranch("ln_pz")     ->SetAddress(evSummary_.ln_pz);
  t_->GetBranch("ln_en")     ->SetAddress(evSummary_.ln_en);
  if(t_->GetBranch("ln_pid")) t_->GetBranch("ln_pid")     ->SetAddress(evSummary_.ln_pid);
  t_->GetBranch("ln_id")     ->SetAddress(evSummary_.ln_id);
  t_->GetBranch("ln_genid")  ->SetAddress(evSummary_.ln_genid);
  t_->GetBranch("ln_ptErr")  ->SetAddress(evSummary_.ln_ptErr);
  t_->GetBranch("ln_iso1")   ->SetAddress(evSummary_.ln_iso1);
  t_->GetBranch("ln_iso2")   ->SetAddress(evSummary_.ln_iso2);
  t_->GetBranch("ln_iso3")   ->SetAddress(evSummary_.ln_iso3);
  
  //selected jets
  t_->GetBranch("jn")        ->SetAddress( &evSummary_.jn);
  t_->GetBranch("jn_px")     ->SetAddress(evSummary_.jn_px);
  t_->GetBranch("jn_py")     ->SetAddress(evSummary_.jn_py);
  t_->GetBranch("jn_pz")     ->SetAddress(evSummary_.jn_pz);
  t_->GetBranch("jn_en")     ->SetAddress(evSummary_.jn_en);
  t_->GetBranch("jn_genid")  ->SetAddress(evSummary_.jn_genid);
  if(t_->GetBranch("jn_genflav")) t_->GetBranch("jn_genflav")->SetAddress(evSummary_.jn_genflav);
  t_->GetBranch("jn_btag1")  ->SetAddress(evSummary_.jn_btag1);
  t_->GetBranch("jn_btag2")  ->SetAddress(evSummary_.jn_btag2);
  t_->GetBranch("jn_btag3")  ->SetAddress(evSummary_.jn_btag3);
  t_->GetBranch("jn_btag4")  ->SetAddress(evSummary_.jn_btag4);
  t_->GetBranch("jn_vtxAssoc")->SetAddress(evSummary_.jn_vtxAssoc);
  
  //primary vertex
  t_->GetBranch("vtx_px")   ->SetAddress( &evSummary_.vtx_px);
  t_->GetBranch("vtx_py")   ->SetAddress( &evSummary_.vtx_py);
  t_->GetBranch("vtx_pz")   ->SetAddress( &evSummary_.vtx_pz);
  t_->GetBranch("vtx_en")   ->SetAddress( &evSummary_.vtx_en);
  
  //MET
  t_->GetBranch("met1_phi")  ->SetAddress( &evSummary_.met1_phi);
  t_->GetBranch("met1_pt")  ->SetAddress( &evSummary_.met1_pt);

  t_->GetBranch("met2_phi")  ->SetAddress( &evSummary_.met2_phi);
  t_->GetBranch("met2_pt")  ->SetAddress( &evSummary_.met2_pt);

  t_->GetBranch("met3_phi")  ->SetAddress( &evSummary_.met3_phi);
  t_->GetBranch("met3_pt")  ->SetAddress( &evSummary_.met3_pt);

  t_->GetBranch("met4_phi")  ->SetAddress( &evSummary_.met4_phi);
  t_->GetBranch("met4_pt")  ->SetAddress( &evSummary_.met4_pt);

  t_->GetBranch("met5_phi")  ->SetAddress( &evSummary_.met5_phi);
  t_->GetBranch("met5_pt")  ->SetAddress( &evSummary_.met5_pt);

  t_->GetBranch("met6_phi")  ->SetAddress( &evSummary_.met6_phi);
  t_->GetBranch("met6_pt")  ->SetAddress( &evSummary_.met6_pt);

  t_->GetBranch("met7_phi")  ->SetAddress( &evSummary_.met7_phi);
  t_->GetBranch("met7_pt")  ->SetAddress( &evSummary_.met7_pt);

  if(t_->GetBranch("met8_phi")) t_->GetBranch("met8_phi")->SetAddress( &evSummary_.met8_phi );
  if(t_->GetBranch("met8_pt"))  t_->GetBranch("met8_pt")->SetAddress( &evSummary_.met8_pt );

  if(t_->GetBranch("met9_phi")) t_->GetBranch("met9_phi")->SetAddress( &evSummary_.met9_phi );
  if(t_->GetBranch("met9_pt"))  t_->GetBranch("met9_pt")->SetAddress( &evSummary_.met9_pt );

  if(t_->GetBranch("met10_phi")) t_->GetBranch("met10_phi")->SetAddress( &evSummary_.met10_phi );
  if(t_->GetBranch("met10_pt"))  t_->GetBranch("met10_pt")->SetAddress( &evSummary_.met10_pt );

  if(t_->GetBranch("met11_phi")) t_->GetBranch("met11_phi")->SetAddress( &evSummary_.met11_phi );
  if(t_->GetBranch("met11_pt"))  t_->GetBranch("met11_pt")->SetAddress( &evSummary_.met11_pt );

  if(t_->GetBranch("met12_phi")) t_->GetBranch("met12_phi")->SetAddress( &evSummary_.met12_phi );
  if(t_->GetBranch("met12_pt"))  t_->GetBranch("met12_pt")->SetAddress( &evSummary_.met12_pt );

  if(t_->GetBranch("met13_phi")) t_->GetBranch("met13_phi")->SetAddress( &evSummary_.met13_phi );
  if(t_->GetBranch("met13_pt"))  t_->GetBranch("met13_pt")->SetAddress( &evSummary_.met13_pt );

  if(t_->GetBranch("sumEt"))                t_->GetBranch("sumEt")->SetAddress(&evSummary_.sumEt);
  if(t_->GetBranch("sumEtcentral"))         t_->GetBranch("sumEtcentral")->SetAddress(&evSummary_.sumEtcentral);
  if(t_->GetBranch("chsumEt"))              t_->GetBranch("chsumEt")->SetAddress(&evSummary_.chsumEt);
  if(t_->GetBranch("chsumEtcentral"))       t_->GetBranch("chsumEtcentral")->SetAddress(&evSummary_.chsumEtcentral);
  if(t_->GetBranch("neutsumEt"))            t_->GetBranch("neutsumEt")->SetAddress(&evSummary_.neutsumEt);
  if(t_->GetBranch("neutsumEtcentral"))     t_->GetBranch("neutsumEtcentral")->SetAddress(&evSummary_.neutsumEtcentral);
  if(t_->GetBranch("primVertexSumEt"))      t_->GetBranch("primVertexSumEt")->SetAddress(&evSummary_.primVertexSumEt);
  if(t_->GetBranch("otherVertexSumEt"))     t_->GetBranch("otherVertexSumEt")->SetAddress(&evSummary_.otherVertexSumEt);
  if(t_->GetBranch("primVertexChSumEt"))    t_->GetBranch("primVertexChSumEt")->SetAddress(&evSummary_.primVertexChSumEt);
  if(t_->GetBranch("otherVertexChSumEt"))   t_->GetBranch("otherVertexChSumEt")->SetAddress(&evSummary_.otherVertexChSumEt);
  if(t_->GetBranch("primVertexNeutSumEt"))  t_->GetBranch("primVertexNeutSumEt")->SetAddress(&evSummary_.primVertexNeutSumEt);
  if(t_->GetBranch("otherVertexNeutSumEt")) t_->GetBranch("otherVertexNeutSumEt")->SetAddress(&evSummary_.otherVertexNeutSumEt);

  
  //Gamma Info
  t_->GetBranch("g_px")     ->SetAddress( &evSummary_.g_px);
  t_->GetBranch("g_py")     ->SetAddress( &evSummary_.g_py);
  t_->GetBranch("g_pz")     ->SetAddress( &evSummary_.g_pz);
  t_->GetBranch("g_en")     ->SetAddress( &evSummary_.g_en);
  t_->GetBranch("g_iso1")   ->SetAddress( &evSummary_.g_iso1);
  t_->GetBranch("g_iso2")   ->SetAddress( &evSummary_.g_iso2);
  t_->GetBranch("g_iso3")   ->SetAddress( &evSummary_.g_iso3);
  t_->GetBranch("g_sihih")  ->SetAddress( &evSummary_.g_sihih);
  t_->GetBranch("g_sipip")  ->SetAddress( &evSummary_.g_sipip);
  t_->GetBranch("g_r9")     ->SetAddress( &evSummary_.g_r9);
  t_->GetBranch("g_eop")    ->SetAddress( &evSummary_.g_eop);
  
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
void ZZ2l2nuSummaryHandler::fillTree()
{
  if(t_) t_->Fill();
}

//
ZZ2l2nuSummaryHandler::~ZZ2l2nuSummaryHandler()
{
}
