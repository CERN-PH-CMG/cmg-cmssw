#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

//
ZZ2l2nuSummaryHandler::ZZ2l2nuSummaryHandler()
{

}

/*
  Int_t run,lumi,event;
  Int_t cat;
  Int_t nvtx, ngenITpu,ngenOOTpu;
  Float_t weight,normWeight,hptWeights[5];
  Float_t rho;
  Float_t pthat,genWeight, qscale, x1,x2;
  Int_t id1,id2;
  Int_t pass;

  Float_t l1_px  ,l1_py  ,l1_pz  ,l1_en, l1_ptErr, l1_iso1, l1_iso2, l1_iso3; Int_t l1_id, l1_genid;
  Float_t l2_px  ,l2_py  ,l2_pz  ,l2_en, l2_ptErr, l2_iso1, l2_iso2, l2_iso3; Int_t l2_id, l2_genid;

  Int_t ln;
  Float_t ln_px[MAXPARTICLES],ln_py[MAXPARTICLES],ln_pz[MAXPARTICLES],ln_en[MAXPARTICLES], l1_ptErr[MAXPARTICLES], l1_iso1[MAXPARTICLES], l1_iso2[MAXPARTICLES], l1_iso3[MAXPARTICLES]; Int_t ln_id[MAXPARTICLES], ln_genid[MAXPARTICLES];

  Int_t jn;
  Float_t jn_px[MAXPARTICLES],jn_py[MAXPARTICLES],jn_pz[MAXPARTICLES],jn_en[MAXPARTICLES], jn_btag1[MAXPARTICLES], jn_btag2[MAXPARTICLES], jn_btag3[MAXPARTICLES], jn_btag4[MAXPARTICLES]; Int_t jn_genid[MAXPARTICLES];

  Float_t vtx_px  ,vtx_py  ,vtx_pz  ,vtx_en;
  Float_t met1_px  ,met1_py  ,met1_pt;
  Float_t met2_px  ,met2_py  ,met2_pt;
  Float_t met3_px  ,met3_py  ,met3_pt;
  Float_t met4_px  ,met4_py  ,met4_pt;
  Float_t met5_px  ,met5_py  ,met5_pt;
  Float_t met6_px  ,met6_py  ,met6_pt;

  Float_t hpx, hpy, hpz, hen;

  Float_t g_px  , g_py  , g_pz  , g_en, g_iso1, g_iso2, g_iso3, g_sihih, g_sipip, g_r9, g_hoe, g_eop;

  Int_t nmcparticles;
  Float_t mc_px[MAXPARTICLES],mc_py[MAXPARTICLES],mc_pz[MAXPARTICLES],mc_en[MAXPARTICLES]; Int_t mc_id[MAXPARTICLES];

*/

 
//
bool ZZ2l2nuSummaryHandler::initTree(TTree *t)
{
  if(t==0) return false;
  t_ = t;

  //event info
  t_->Branch("run",        &evSummary_.run,        "run/I");
  t_->Branch("lumi",       &evSummary_.lumi,       "lumi/I");
  t_->Branch("event",      &evSummary_.event,      "event/I");
  t_->Branch("cat",        &evSummary_.cat,        "cat/I");

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
  t_->Branch("met1_px",   &evSummary_.met1_px,     "met1_px/F");
  t_->Branch("met1_py",   &evSummary_.met1_py,     "met1_py/F");
  t_->Branch("met1_pt",   &evSummary_.met1_pt,     "met1_pt/F");

  t_->Branch("met2_px",   &evSummary_.met2_px,     "met2_px/F");
  t_->Branch("met2_py",   &evSummary_.met2_py,     "met2_py/F");
  t_->Branch("met2_pt",   &evSummary_.met2_pt,     "met2_pt/F");

  t_->Branch("met3_px",   &evSummary_.met3_px,     "met3_px/F");
  t_->Branch("met3_py",   &evSummary_.met3_py,     "met3_py/F");
  t_->Branch("met3_pt",   &evSummary_.met3_pt,     "met3_pt/F");

  t_->Branch("met4_px",   &evSummary_.met4_px,     "met4_px/F");
  t_->Branch("met4_py",   &evSummary_.met4_py,     "met4_py/F");
  t_->Branch("met4_pt",   &evSummary_.met4_pt,     "met4_pt/F");

  t_->Branch("met5_px",   &evSummary_.met5_px,     "met5_px/F");
  t_->Branch("met5_py",   &evSummary_.met5_py,     "met5_py/F");
  t_->Branch("met5_pt",   &evSummary_.met5_pt,     "met5_pt/F");

  t_->Branch("met6_px",   &evSummary_.met6_px,     "met6_px/F");
  t_->Branch("met6_py",   &evSummary_.met6_py,     "met6_py/F");
  t_->Branch("met6_pt",   &evSummary_.met6_pt,     "met6_pt/F");


  //Higgs Info
  t_->Branch("h_px",      &evSummary_.h_px,        "h_px/F");
  t_->Branch("h_py",      &evSummary_.h_py,        "h_py/F");
  t_->Branch("h_pz",      &evSummary_.h_pz,        "h_pz/F");
  t_->Branch("h_en",      &evSummary_.h_en,        "h_en/F");

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
  t_->GetBranch("pass")->SetAddress(&evSummary_.pass);



  //selected di-leptons
  t_->GetBranch("l1_px")     ->SetAddress( &evSummary_.l1_px);
  t_->GetBranch("l1_py")     ->SetAddress( &evSummary_.l1_py);
  t_->GetBranch("l1_pz")     ->SetAddress( &evSummary_.l1_pz);
  t_->GetBranch("l1_en")     ->SetAddress( &evSummary_.l1_en);
  t_->GetBranch("l1_id")     ->SetAddress( &evSummary_.l1_id);
  t_->GetBranch("l1_genid")  ->SetAddress( &evSummary_.l1_genid);
  t_->GetBranch("l1_ptErr")  ->SetAddress( &evSummary_.l1_ptErr);
  t_->GetBranch("l1_iso1")   ->SetAddress( &evSummary_.l1_iso1);
  t_->GetBranch("l1_iso2")   ->SetAddress( &evSummary_.l1_iso2);
  t_->GetBranch("l1_iso3")   ->SetAddress( &evSummary_.l1_iso3);

  t_->GetBranch("l2_px")     ->SetAddress( &evSummary_.l2_px);
  t_->GetBranch("l2_py")     ->SetAddress( &evSummary_.l2_py);
  t_->GetBranch("l2_pz")     ->SetAddress( &evSummary_.l2_pz);
  t_->GetBranch("l2_en")     ->SetAddress( &evSummary_.l2_en);
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
  t_->GetBranch("met1_px")  ->SetAddress( &evSummary_.met1_px);
  t_->GetBranch("met1_py")  ->SetAddress( &evSummary_.met1_py);
  t_->GetBranch("met1_pt")  ->SetAddress( &evSummary_.met1_pt);

  t_->GetBranch("met2_px")  ->SetAddress( &evSummary_.met2_px);
  t_->GetBranch("met2_py")  ->SetAddress( &evSummary_.met2_py);
  t_->GetBranch("met2_pt")  ->SetAddress( &evSummary_.met2_pt);

  t_->GetBranch("met3_px")  ->SetAddress( &evSummary_.met3_px);
  t_->GetBranch("met3_py")  ->SetAddress( &evSummary_.met3_py);
  t_->GetBranch("met3_pt")  ->SetAddress( &evSummary_.met3_pt);

  t_->GetBranch("met4_px")  ->SetAddress( &evSummary_.met4_px);
  t_->GetBranch("met4_py")  ->SetAddress( &evSummary_.met4_py);
  t_->GetBranch("met4_pt")  ->SetAddress( &evSummary_.met4_pt);

  t_->GetBranch("met5_px")  ->SetAddress( &evSummary_.met5_px);
  t_->GetBranch("met5_py")  ->SetAddress( &evSummary_.met5_py);
  t_->GetBranch("met5_pt")  ->SetAddress( &evSummary_.met5_pt);

  t_->GetBranch("met6_px")  ->SetAddress( &evSummary_.met6_px);
  t_->GetBranch("met6_py")  ->SetAddress( &evSummary_.met6_py);
  t_->GetBranch("met6_pt")  ->SetAddress( &evSummary_.met6_pt);


  //Higgs Info
  t_->GetBranch("h_px")     ->SetAddress( &evSummary_.h_px);
  t_->GetBranch("h_py")     ->SetAddress( &evSummary_.h_py);
  t_->GetBranch("h_pz")     ->SetAddress( &evSummary_.h_pz);
  t_->GetBranch("h_en")     ->SetAddress( &evSummary_.h_en);

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
