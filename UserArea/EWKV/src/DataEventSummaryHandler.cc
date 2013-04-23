#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"

using namespace std;

//
bool DataEventSummaryHandler::init(TTree *t, bool needsToRecreate)
{
  if(t==0) return false;
  t_ = t;
  if(!needsToRecreate) return true;
  
  //event info
  t_->Branch("run",      &evSummary_.run,      "run/I");
  t_->Branch("lumi",     &evSummary_.lumi,     "lumi/I");
  t_->Branch("event",    &evSummary_.event,    "event/I");
  t_->Branch("cat",      &evSummary_.cat,      "cat/I");

  //trigger bit
  t_->Branch("tn",          &evSummary_.tn,        "tn/I");
  t_->Branch("t_bits",      evSummary_.t_bits,     "t_bits[tn]/O");
  t_->Branch("t_prescale",  evSummary_.t_prescale, "t_prescale[tn]/I");
  
  //pileup related observables
  t_->Branch("nvtx",     &evSummary_.nvtx,       "nvtx/I");
  t_->Branch("instLumi", &evSummary_.instLumi,   "instLumi/F");
  t_->Branch("rho",      &evSummary_.rho,        "rho/F");
  t_->Branch("rho25",    &evSummary_.rho25,      "rho25/F");

  //pf candidates
  t_->Branch("pfn",       &evSummary_.pfn,       "pfn/I");
  t_->Branch("pf_id",     &evSummary_.pf_id,     "pf_id[pfn]/I");
  t_->Branch("pf_charge", &evSummary_.pf_charge, "pf_charge[pfn]/I");
  t_->Branch("pf_px",     &evSummary_.pf_px,     "pf_px[pfn]/I");
  t_->Branch("pf_py",     &evSummary_.pf_py,     "pf_py[pfn]/I");
  t_->Branch("pf_pz",     &evSummary_.pf_pz,     "pf_pz[pfn]/I");

  //generator level info
  t_->Branch("mccat",       &evSummary_.mccat,       "mccat/I");
  t_->Branch("ngenITpu",    &evSummary_.ngenITpu,    "ngenITpu/I");
  t_->Branch("ngenOOTpu",   &evSummary_.ngenOOTpu,   "ngenOOTpu/I");
  t_->Branch("ngenOOTpum1", &evSummary_.ngenOOTpum1, "ngenOOTpum1/I");
  t_->Branch("ngenTruepu",  &evSummary_.ngenTruepu,  "ngenTruepu/I");
  t_->Branch("pthat",       &evSummary_.pthat,       "pthat/I");
  t_->Branch("genWeight",   &evSummary_.genWeight,   "genWeight/I");
  t_->Branch("qscale",      &evSummary_.qscale,      "qscale/I");
  t_->Branch("x1",          &evSummary_.x1,          "x1/I");
  t_->Branch("x2",          &evSummary_.x2,          "x2/I");
  t_->Branch("id1",         &evSummary_.id1,         "id1/I");
  t_->Branch("id2",         &evSummary_.id2,         "id2/I");
  t_->Branch("nup",         &evSummary_.nup,         "nup/I");

  //mc truth
  t_->Branch("mcn",         &evSummary_.mcn,         "mcn");
  t_->Branch("mc_id",       evSummary_.mc_id,        "mc_id[mcn]/I");
  t_->Branch("mc_status",   evSummary_.mc_status,    "mc_status[mcn]/I");
  t_->Branch("mc_px",       evSummary_.mc_px,        "mc_px[mcn]/F");
  t_->Branch("mc_py",       evSummary_.mc_py,        "mc_py[mcn]/F");
  t_->Branch("mc_pz",       evSummary_.mc_pz,        "mc_pz[mcn]/F");
  t_->Branch("mc_en",       evSummary_.mc_en,        "mc_en[mcn]/F");
  t_->Branch("mc_lxy",      evSummary_.mc_lxy,       "mc_lxy[mcn]/F");  

  //selected leptons
  t_->Branch("ln",            &evSummary_.ln,          "ln/I");
  t_->Branch("ln_id",         evSummary_.ln_id,        "ln_id[ln]/I");
  t_->Branch("ln_idbits",     evSummary_.ln_idbits,    "ln_idbits[ln]/I");
  t_->Branch("ln_pid",        evSummary_.ln_pid,       "ln_pid[ln]/I");
  t_->Branch("ln_px",         evSummary_.ln_px,        "ln_px[ln]/F");
  t_->Branch("ln_py",         evSummary_.ln_py,        "ln_py[ln]/F");
  t_->Branch("ln_pz",         evSummary_.ln_pz,        "ln_pz[ln]/F");
  t_->Branch("ln_en",         evSummary_.ln_en,        "ln_en[ln]/F");
  t_->Branch("ln_ecalIso03",  evSummary_.ln_ecalIso03, "ln_ecalIso03[ln]/F");
  t_->Branch("ln_hcalIso03",  evSummary_.ln_hcalIso03, "ln_hcalIso03[ln]/F");
  t_->Branch("ln_trkIso03",   evSummary_.ln_trkIso03,  "ln_trkIso03[ln]/F");
  t_->Branch("ln_gIso03",     evSummary_.ln_gIso03,    "ln_gIso03[ln]/F");
  t_->Branch("ln_chIso03",    evSummary_.ln_chIso03,   "ln_chIso03[ln]/F");
  t_->Branch("ln_puchIso03",  evSummary_.ln_puchIso03, "ln_puchIso03[ln]/F");
  t_->Branch("ln_nhIso03",    evSummary_.ln_nhIso03,   "ln_nhIso03[ln]/F");
  t_->Branch("ln_ecalIso04",  evSummary_.ln_ecalIso04, "ln_ecalIso04[ln]/F");
  t_->Branch("ln_hcalIso04",  evSummary_.ln_hcalIso04, "ln_hcalIso04[ln]/F");
  t_->Branch("ln_trkIso04",   evSummary_.ln_trkIso04,  "ln_trkIso04[ln]/F");
  t_->Branch("ln_gIso04",     evSummary_.ln_gIso04,    "ln_gIso04[ln]/F");
  t_->Branch("ln_chIso04",    evSummary_.ln_chIso04,   "ln_chIso04[ln]/F");
  t_->Branch("ln_puchIso04",  evSummary_.ln_puchIso04, "ln_puchIso04[ln]/F");
  t_->Branch("ln_nhIso04",    evSummary_.ln_nhIso04,   "ln_nhIso04[ln]/F");
  t_->Branch("ln_d0",                  evSummary_.ln_d0,          "ln_d0[ln]/F");
  t_->Branch("ln_dZ",                  evSummary_.ln_dZ,          "ln_dZ[ln]/F");
  t_->Branch("ln_ip3d",                evSummary_.ln_ip3d,        "ln_ip3d[ln]/F");
  t_->Branch("ln_ip3dsig",             evSummary_.ln_ip3dsig,     "ln_ip3dsig[ln]/F");
  t_->Branch("ln_trkpt",               evSummary_.ln_trkpt,       "ln_trkpt[ln]/F");
  t_->Branch("ln_trketa",              evSummary_.ln_trketa,      "ln_trketa[ln]/F");
  t_->Branch("ln_trkphi",              evSummary_.ln_trkphi,      "ln_trkphi[ln]/F");
  t_->Branch("ln_trkchi2",             evSummary_.ln_trkchi2,     "ln_trkchi2[ln]/F");
  t_->Branch("ln_trkValidPixelHits",   evSummary_.ln_trkValidPixelHits,  "ln_trkValidPixelHits[ln]/F");
  t_->Branch("ln_trkValidTrackerHits", evSummary_.ln_trkValidTrackerHits,"ln_trkValidTrackerHits[ln]/F");
  t_->Branch("ln_trkLostInnerHits",    evSummary_.ln_trkLostInnerHits,   "ln_trkLostInnerHits[ln]/F");
  t_->Branch("ln_trkPtErr",               evSummary_.ln_trkPtErr,              "ln_trkPtErr[ln]/F");

  //Gamma Info
  t_->Branch("gn",          &evSummary_.gn,          "gn/I");
  t_->Branch("gn_pidbits",  evSummary_.gn_pidbits,   "gn_pidbits[gn]/I");
  t_->Branch("gn_pid",      evSummary_.gn_pid,       "gn_pid[gn]/I");
  t_->Branch("gn_px",       evSummary_.gn_px,        "gn_px[gn]/F");
  t_->Branch("gn_py",       evSummary_.gn_py,        "gn_py[gn]/F");
  t_->Branch("gn_pz",       evSummary_.gn_pz,        "gn_pz[gn]/F");
  t_->Branch("gn_en",       evSummary_.gn_en,        "gn_en[gn]/F");
  t_->Branch("gn_ecalIso03",  evSummary_.gn_ecalIso03, "gn_ecalIso03[gn]/F");
  t_->Branch("gn_hcalIso03",  evSummary_.gn_hcalIso03, "gn_hcalIso03[gn]/F");
  t_->Branch("gn_trkIso03",   evSummary_.gn_trkIso03,  "gn_trkIso03[gn]/F");
  t_->Branch("gn_gIso03",     evSummary_.gn_gIso03,    "gn_gIso03[gn]/F");
  t_->Branch("gn_chIso03",    evSummary_.gn_chIso03,   "gn_chIso03[gn]/F");
  t_->Branch("gn_puchIso03",  evSummary_.gn_puchIso03, "gn_puchIso03[gn]/F");
  t_->Branch("gn_nhIso03",    evSummary_.gn_nhIso03,   "gn_nhIso03[gn]/F");
  t_->Branch("gn_ecalIso04",  evSummary_.gn_ecalIso04, "gn_ecalIso04[gn]/F");
  t_->Branch("gn_hcalIso04",  evSummary_.gn_hcalIso04, "gn_hcalIso04[gn]/F");
  t_->Branch("gn_trkIso04",   evSummary_.gn_trkIso04,  "gn_trkIso04[gn]/F");
  t_->Branch("gn_gIso04",     evSummary_.gn_gIso04,    "gn_gIso04[gn]/F");
  t_->Branch("gn_chIso04",    evSummary_.gn_chIso04,   "gn_chIso04[gn]/F");
  t_->Branch("gn_puchIso04",  evSummary_.gn_puchIso04, "gn_puchIso04[gn]/F");
  t_->Branch("gn_nhIso04",    evSummary_.gn_nhIso04,   "gn_nhIso04[gn]/F");

  //e/gamma id branches
  t_->Branch("egn",              &evSummary_.egn,        "egn/I");
  t_->Branch("egn_isConv",       evSummary_.egn_isConv,  "egn_isConv[egn]/O");
  t_->Branch("egn_hoe",          evSummary_.egn_hoe,     "egn_hoe[egn]/F");
  t_->Branch("egn_h2te",         evSummary_.egn_h2te,    "egn_h2te[egn]/F");
  t_->Branch("egn_dphiin",       evSummary_.egn_dphiin,  "egn_dphiin[egn]/F");
  t_->Branch("egn_detain",       evSummary_.egn_detain,  "egn_detain[egn]/F");
  t_->Branch("egn_sihih",        evSummary_.egn_sihih,   "egn_sihih[egn]/F");
  t_->Branch("egn_sipip",        evSummary_.egn_sipip,   "egn_sipip[egn]/F");
  t_->Branch("egn_sihip",        evSummary_.egn_sihip,   "egn_sihip[egn]/F");
  t_->Branch("egn_eopin",        evSummary_.egn_eopin,   "egn_eopin[egn]/F");
  t_->Branch("egn_eopout",       evSummary_.egn_eopout,  "egn_eopout[egn]/F");
  t_->Branch("egn_r9",           evSummary_.egn_r9,      "egn_r9[egn]/F");
  t_->Branch("egn_sce",          evSummary_.egn_sce,     "egn_sce[egn]/F");
  t_->Branch("egn_sceta",        evSummary_.egn_sceta,   "egn_sceta[egn]/F");
  t_->Branch("egn_scphi",        evSummary_.egn_scphi,   "egn_scphi[egn]/F");
  t_->Branch("egn_mvatrigv0",    evSummary_.egn_mvatrigv0, "egn_mvatrigv0[egn]/F");
  t_->Branch("egn_mvanontrigv0", evSummary_.egn_mvanontrigv0, "egn_mvanontrigv0[egn]/F");

  //muon id branches
  t_->Branch("mn",                            &evSummary_.mn,                             "mn/I");
  t_->Branch("mn_nMatches",                   evSummary_.mn_nMatches,                     "mn_nMatches[mn]/F");
  t_->Branch("mn_nMatchedStations",           evSummary_.mn_nMatchedStations,             "mn_nMatchedStations[mn]/F");
  t_->Branch("mn_validMuonHits",              evSummary_.mn_validMuonHits,                "mn_validMuonHits[mn]/F");
  t_->Branch("mn_innerTrackChi2",             evSummary_.mn_innerTrackChi2,               "mn_innerTrackChi2[mn]/F");
  t_->Branch("mn_trkLayersWithMeasurement",   evSummary_.mn_trkLayersWithMeasurement,     "mn_trkLayersWithMeasurement[mn]/F");
  t_->Branch("mn_pixelLayersWithMeasurement", evSummary_.mn_pixelLayersWithMeasurement,   "mn_pixelLayersWithMeasurement[mn]/F");

  //selected jets
  t_->Branch("jn",             &evSummary_.jn,                "jn/I");
  t_->Branch("jn_idbits",      evSummary_.jn_idbits,          "jn_idbits[jn]/I");
  t_->Branch("jn_px",          evSummary_.jn_px,              "jn_px[jn]/F");
  t_->Branch("jn_py",          evSummary_.jn_py,              "jn_py[jn]/F");
  t_->Branch("jn_pz",          evSummary_.jn_pz,              "jn_pz[jn]/F");
  t_->Branch("jn_en",          evSummary_.jn_en,              "jn_en[jn]/F");
  t_->Branch("jn_torawsf",     evSummary_.jn_torawsf,         "jn_torawsf[jn]/F");
  t_->Branch("jn_neutHadFrac", evSummary_.jn_neutHadFrac,     "jn_neutHadFrac[jn]/F");
  t_->Branch("jn_neutEmFrac",  evSummary_.jn_neutEmFrac,      "jn_neutEmFrac[jn]/F");
  t_->Branch("jn_chHadFrac",   evSummary_.jn_chHadFrac,       "jn_chHadFrac[jn]/F");
  t_->Branch("jn_tchp",        evSummary_.jn_tchp,            "jn_tchp[jn]/F");
  t_->Branch("jn_jp",          evSummary_.jn_jp,              "jn_jp[jn]/F");
  t_->Branch("jn_csv",         evSummary_.jn_csv,             "jn_csv[jn]/F");
  t_->Branch("jn_supercsv",    evSummary_.jn_supercsv,        "jn_supercsv[jn]/F");
  t_->Branch("jn_ssvhe",       evSummary_.jn_ssvhe,           "jn_ssvhe[jn]/F");
  t_->Branch("jn_ivf",         evSummary_.jn_ivf,             "jn_ivf[jn]/F");
  t_->Branch("jn_svxPx",       evSummary_.jn_svxPx,           "jn_svxPx[jn]/F");
  t_->Branch("jn_svxPy",       evSummary_.jn_svxPy,           "jn_svxPy[jn]/F");
  t_->Branch("jn_svxPz",       evSummary_.jn_svxPz,           "jn_svxPz[jn]/F");
  t_->Branch("jn_svxM",        evSummary_.jn_svxM,            "jn_svxM[jn]/F");
  t_->Branch("jn_svxNtrk",     evSummary_.jn_svxNtrk,         "jn_svxNtrk[jn]/F");
  t_->Branch("jn_svxLxy",      evSummary_.jn_svxLxy,          "jn_svxLxy[jn]/F");
  t_->Branch("jn_svxLxyErr",   evSummary_.jn_svxLxyErr,       "jn_svxLxyErr[jn]/F");
  t_->Branch("jn_ivfPx",       evSummary_.jn_ivfPx,           "jn_ivfPx[jn]/F");
  t_->Branch("jn_ivfPy",       evSummary_.jn_ivfPy,           "jn_ivfPy[jn]/F");
  t_->Branch("jn_ivfPz",       evSummary_.jn_ivfPz,           "jn_ivfPz[jn]/F");
  t_->Branch("jn_ivfM",        evSummary_.jn_ivfM,            "jn_ivfM[jn]/F");
  t_->Branch("jn_ivfNtrk",     evSummary_.jn_ivfNtrk,         "jn_ivfNtrk[jn]/F");
  t_->Branch("jn_ivfLxy",      evSummary_.jn_ivfLxy,          "jn_ivfLxy[jn]/F");
  t_->Branch("jn_ivfLxyErr",   evSummary_.jn_ivfLxyErr,       "jn_ivfLxyErr[jn]/F");
  t_->Branch("jn_puMVA",       evSummary_.jn_puMVA,           "jn_puMVA[jn]/F");
  t_->Branch("jn_qgMVA",       evSummary_.jn_qgMVA,           "jn_qgMVA[jn]/F");
  t_->Branch("jn_beta",        evSummary_.jn_beta,            "jn_beta[jn]/F");
  t_->Branch("jn_dRMean",      evSummary_.jn_dRMean,          "jn_dRMean[jn]/F");
  t_->Branch("jn_ptRMS",       evSummary_.jn_ptRMS,           "jn_ptRMS[jn]/F");
  t_->Branch("jn_etaW",        evSummary_.jn_etaW,            "jn_etaW[jn]/F");
  t_->Branch("jn_phiW",        evSummary_.jn_phiW,            "jn_phiW[jn]/F");
  t_->Branch("jn_genflav",     evSummary_.jn_genflav,         "jn_genflav[jn]/I");
  t_->Branch("jn_genPx",       evSummary_.jn_genPx,           "jn_genPx[jn]/F");
  t_->Branch("jn_genPy",       evSummary_.jn_genPy,           "jn_genPy[jn]/F");
  t_->Branch("jn_genPz",       evSummary_.jn_genPz,           "jn_genPz[jn]/F");
  t_->Branch("jn_genEn",       evSummary_.jn_genEn,           "jn_genEn[jn]/F");
 
  //MET
  t_->Branch("metn",     &evSummary_.metn,    "metn/I");
  t_->Branch("met_phi",  evSummary_.met_phi,  "met_phi[metn]/F");
  t_->Branch("met_pt",   evSummary_.met_pt,   "met_pt[metn]/F");

  return true;
}

//
bool DataEventSummaryHandler::attach(TTree *t)
{
  if(t==0) return false;
  t_ = t;

  //event info
  t_->SetBranchAddress("run",      &evSummary_.run);
  t_->SetBranchAddress("lumi",     &evSummary_.lumi);
  t_->SetBranchAddress("event",    &evSummary_.event);
  t_->SetBranchAddress("cat",      &evSummary_.cat);

  //trigger bit
  t_->SetBranchAddress("tn",         &evSummary_.tn);
  t_->SetBranchAddress("t_bits",    evSummary_.t_bits);
  t_->SetBranchAddress("t_prescale", evSummary_.t_prescale);
  
  //pileup related observables
  t_->SetBranchAddress("nvtx",     &evSummary_.nvtx);
  t_->SetBranchAddress("instLumi", &evSummary_.instLumi);
  t_->SetBranchAddress("rho",      &evSummary_.rho );
  t_->SetBranchAddress("rho25",    &evSummary_.rho25 );

  //pf candidates
  t_->SetBranchAddress("pfn",       &evSummary_.pfn);
  t_->SetBranchAddress("pf_id",     &evSummary_.pf_id);
  t_->SetBranchAddress("pf_charge", &evSummary_.pf_charge);
  t_->SetBranchAddress("pf_px",     &evSummary_.pf_px);
  t_->SetBranchAddress("pf_py",     &evSummary_.pf_py);
  t_->SetBranchAddress("pf_pz",     &evSummary_.pf_pz);


  //generator level info
  t_->SetBranchAddress("mccat",       &evSummary_.mccat);
  t_->SetBranchAddress("ngenITpu",    &evSummary_.ngenITpu);
  t_->SetBranchAddress("ngenOOTpu",   &evSummary_.ngenOOTpu);
  t_->SetBranchAddress("ngenOOTpum1", &evSummary_.ngenOOTpum1);
  t_->SetBranchAddress("ngenTruepu",  &evSummary_.ngenTruepu);
  t_->SetBranchAddress("pthat",       &evSummary_.pthat);
  t_->SetBranchAddress("genWeight",   &evSummary_.genWeight);
  t_->SetBranchAddress("qscale",      &evSummary_.qscale);
  t_->SetBranchAddress("x1",          &evSummary_.x1);
  t_->SetBranchAddress("x2",          &evSummary_.x2);
  t_->SetBranchAddress("id1",         &evSummary_.id1);
  t_->SetBranchAddress("id2",         &evSummary_.id2);
  t_->SetBranchAddress("nup",         &evSummary_.nup);

  //mc truth
  t_->SetBranchAddress("mcn",         &evSummary_.mcn);
  t_->SetBranchAddress("mc_id",       evSummary_.mc_id);
  t_->SetBranchAddress("mc_status",   evSummary_.mc_status);
  t_->SetBranchAddress("mc_px",       evSummary_.mc_px);
  t_->SetBranchAddress("mc_py",       evSummary_.mc_py);
  t_->SetBranchAddress("mc_pz",       evSummary_.mc_pz);
  t_->SetBranchAddress("mc_en",       evSummary_.mc_en);
  t_->SetBranchAddress("mc_lxy",      evSummary_.mc_lxy);  



  //selected leptons
  t_->SetBranchAddress("ln",            &evSummary_.ln);
  t_->SetBranchAddress("ln_id",         evSummary_.ln_id);
  t_->SetBranchAddress("ln_idbits",     evSummary_.ln_idbits);
  t_->SetBranchAddress("ln_pid",        evSummary_.ln_pid);
  t_->SetBranchAddress("ln_px",         evSummary_.ln_px);
  t_->SetBranchAddress("ln_py",         evSummary_.ln_py);
  t_->SetBranchAddress("ln_pz",         evSummary_.ln_pz);
  t_->SetBranchAddress("ln_en",         evSummary_.ln_en);
  t_->SetBranchAddress("ln_ecalIso03",  evSummary_.ln_ecalIso03);
  t_->SetBranchAddress("ln_hcalIso03",  evSummary_.ln_hcalIso03);
  t_->SetBranchAddress("ln_trkIso03",   evSummary_.ln_trkIso03);
  t_->SetBranchAddress("ln_gIso03",     evSummary_.ln_gIso03);
  t_->SetBranchAddress("ln_chIso03",    evSummary_.ln_chIso03);
  t_->SetBranchAddress("ln_puchIso03",  evSummary_.ln_puchIso03);
  t_->SetBranchAddress("ln_nhIso03",    evSummary_.ln_nhIso03);
  t_->SetBranchAddress("ln_ecalIso04",  evSummary_.ln_ecalIso04);
  t_->SetBranchAddress("ln_hcalIso04",  evSummary_.ln_hcalIso04);
  t_->SetBranchAddress("ln_trkIso04",   evSummary_.ln_trkIso04);
  t_->SetBranchAddress("ln_gIso04",     evSummary_.ln_gIso04);
  t_->SetBranchAddress("ln_chIso04",    evSummary_.ln_chIso04);
  t_->SetBranchAddress("ln_puchIso04",  evSummary_.ln_puchIso04);
  t_->SetBranchAddress("ln_nhIso04",    evSummary_.ln_nhIso04);
  t_->SetBranchAddress("ln_d0",                  evSummary_.ln_d0);
  t_->SetBranchAddress("ln_dZ",                  evSummary_.ln_dZ);
  t_->SetBranchAddress("ln_ip3d",                evSummary_.ln_ip3d);
  t_->SetBranchAddress("ln_ip3dsig",             evSummary_.ln_ip3dsig);
  t_->SetBranchAddress("ln_trkpt",               evSummary_.ln_trkpt);
  t_->SetBranchAddress("ln_trketa",              evSummary_.ln_trketa);
  t_->SetBranchAddress("ln_trkphi",              evSummary_.ln_trkphi);
  t_->SetBranchAddress("ln_trkchi2",             evSummary_.ln_trkchi2);
  t_->SetBranchAddress("ln_trkValidPixelHits",   evSummary_.ln_trkValidPixelHits);
  t_->SetBranchAddress("ln_trkValidTrackerHits", evSummary_.ln_trkValidTrackerHits);
  t_->SetBranchAddress("ln_trkLostInnerHits",    evSummary_.ln_trkLostInnerHits);
  t_->SetBranchAddress("ln_trkPtErr",               evSummary_.ln_trkPtErr);

  //Gamma Info
  t_->SetBranchAddress("gn",          &evSummary_.gn);
  t_->SetBranchAddress("gn_pidbits",  evSummary_.gn_pidbits);
  t_->SetBranchAddress("gn_pid",      evSummary_.gn_pid);
  t_->SetBranchAddress("gn_px",       evSummary_.gn_px);
  t_->SetBranchAddress("gn_py",       evSummary_.gn_py);
  t_->SetBranchAddress("gn_pz",       evSummary_.gn_pz);
  t_->SetBranchAddress("gn_en",       evSummary_.gn_en);
  t_->SetBranchAddress("gn_ecalIso03",  evSummary_.gn_ecalIso03);
  t_->SetBranchAddress("gn_hcalIso03",  evSummary_.gn_hcalIso03);
  t_->SetBranchAddress("gn_trkIso03",   evSummary_.gn_trkIso03);
  t_->SetBranchAddress("gn_gIso03",     evSummary_.gn_gIso03);
  t_->SetBranchAddress("gn_chIso03",    evSummary_.gn_chIso03);
  t_->SetBranchAddress("gn_puchIso03",  evSummary_.gn_puchIso03);
  t_->SetBranchAddress("gn_nhIso03",    evSummary_.gn_nhIso03);
  t_->SetBranchAddress("gn_ecalIso04",  evSummary_.gn_ecalIso04);
  t_->SetBranchAddress("gn_hcalIso04",  evSummary_.gn_hcalIso04);
  t_->SetBranchAddress("gn_trkIso04",   evSummary_.gn_trkIso04);
  t_->SetBranchAddress("gn_gIso04",     evSummary_.gn_gIso04);
  t_->SetBranchAddress("gn_chIso04",    evSummary_.gn_chIso04);
  t_->SetBranchAddress("gn_puchIso04",  evSummary_.gn_puchIso04);
  t_->SetBranchAddress("gn_nhIso04",    evSummary_.gn_nhIso04);

  //e/gamma id branches
  t_->SetBranchAddress("egn",              &evSummary_.egn);
  t_->SetBranchAddress("egn_isConv",       evSummary_.egn_isConv);
  t_->SetBranchAddress("egn_hoe",          evSummary_.egn_hoe);
  t_->SetBranchAddress("egn_h2te",         evSummary_.egn_h2te);
  t_->SetBranchAddress("egn_dphiin",       evSummary_.egn_dphiin);
  t_->SetBranchAddress("egn_detain",       evSummary_.egn_detain);
  t_->SetBranchAddress("egn_sihih",        evSummary_.egn_sihih);
  t_->SetBranchAddress("egn_sipip",        evSummary_.egn_sipip);
  t_->SetBranchAddress("egn_sihip",        evSummary_.egn_sihip);
  t_->SetBranchAddress("egn_eopin",        evSummary_.egn_eopin);
  t_->SetBranchAddress("egn_eopout",       evSummary_.egn_eopout);
  t_->SetBranchAddress("egn_r9",           evSummary_.egn_r9);
  t_->SetBranchAddress("egn_sce",          evSummary_.egn_sce);
  t_->SetBranchAddress("egn_sceta",        evSummary_.egn_sceta);
  t_->SetBranchAddress("egn_scphi",        evSummary_.egn_scphi);
  t_->SetBranchAddress("egn_mvatrigv0",    evSummary_.egn_mvatrigv0);
  t_->SetBranchAddress("egn_mvanontrigv0", evSummary_.egn_mvanontrigv0);

  //muon id branches
  t_->SetBranchAddress("mn",                            &evSummary_.mn);
  t_->SetBranchAddress("mn_nMatches",                   evSummary_.mn_nMatches);
  t_->SetBranchAddress("mn_nMatchedStations",           evSummary_.mn_nMatchedStations);
  t_->SetBranchAddress("mn_validMuonHits",              evSummary_.mn_validMuonHits);
  t_->SetBranchAddress("mn_innerTrackChi2",             evSummary_.mn_innerTrackChi2);
  t_->SetBranchAddress("mn_trkLayersWithMeasurement",   evSummary_.mn_trkLayersWithMeasurement);
  t_->SetBranchAddress("mn_pixelLayersWithMeasurement", evSummary_.mn_pixelLayersWithMeasurement);

  //selected jets
  t_->SetBranchAddress("jn",             &evSummary_.jn);
  t_->SetBranchAddress("jn_idbits",      evSummary_.jn_idbits);
  t_->SetBranchAddress("jn_px",          evSummary_.jn_px);
  t_->SetBranchAddress("jn_py",          evSummary_.jn_py);
  t_->SetBranchAddress("jn_pz",          evSummary_.jn_pz);
  t_->SetBranchAddress("jn_en",          evSummary_.jn_en);
  t_->SetBranchAddress("jn_torawsf",     evSummary_.jn_torawsf);
  t_->SetBranchAddress("jn_neutHadFrac", evSummary_.jn_neutHadFrac);
  t_->SetBranchAddress("jn_neutEmFrac",  evSummary_.jn_neutEmFrac);
  t_->SetBranchAddress("jn_chHadFrac",   evSummary_.jn_chHadFrac);
  t_->SetBranchAddress("jn_tchp",        evSummary_.jn_tchp);
  t_->SetBranchAddress("jn_jp",          evSummary_.jn_jp);
  t_->SetBranchAddress("jn_csv",         evSummary_.jn_csv);
  t_->SetBranchAddress("jn_supercsv",    evSummary_.jn_supercsv);
  t_->SetBranchAddress("jn_ssvhe",       evSummary_.jn_ssvhe);
  t_->SetBranchAddress("jn_ivf",         evSummary_.jn_ivf);
  t_->SetBranchAddress("jn_svxPx",       evSummary_.jn_svxPx);
  t_->SetBranchAddress("jn_svxPy",       evSummary_.jn_svxPy);
  t_->SetBranchAddress("jn_svxPz",       evSummary_.jn_svxPz);
  t_->SetBranchAddress("jn_svxNtrk",     evSummary_.jn_svxNtrk);
  t_->SetBranchAddress("jn_svxM",        evSummary_.jn_svxM);
  t_->SetBranchAddress("jn_svxLxy",      evSummary_.jn_svxLxy);
  t_->SetBranchAddress("jn_svxLxyErr",   evSummary_.jn_svxLxyErr);
  t_->SetBranchAddress("jn_ivfPx",       evSummary_.jn_ivfPx);
  t_->SetBranchAddress("jn_ivfPy",       evSummary_.jn_ivfPy);
  t_->SetBranchAddress("jn_ivfPz",       evSummary_.jn_ivfPz);
  t_->SetBranchAddress("jn_ivfM",        evSummary_.jn_ivfM);
  t_->SetBranchAddress("jn_ivfNtrk",     evSummary_.jn_ivfNtrk);
  t_->SetBranchAddress("jn_ivfLxy",      evSummary_.jn_ivfLxy);
  t_->SetBranchAddress("jn_ivfLxyErr",   evSummary_.jn_ivfLxyErr);
  t_->SetBranchAddress("jn_puMVA",       evSummary_.jn_puMVA);
  t_->SetBranchAddress("jn_qgMVA",       evSummary_.jn_qgMVA);
  t_->SetBranchAddress("jn_beta",        evSummary_.jn_beta);
  t_->SetBranchAddress("jn_dRMean",      evSummary_.jn_dRMean);
  t_->SetBranchAddress("jn_ptRMS",       evSummary_.jn_ptRMS);
  t_->SetBranchAddress("jn_etaW",        evSummary_.jn_etaW);
  t_->SetBranchAddress("jn_phiW",        evSummary_.jn_phiW);
  t_->SetBranchAddress("jn_genflav",     evSummary_.jn_genflav);
  t_->SetBranchAddress("jn_genPx",       evSummary_.jn_genPx);
  t_->SetBranchAddress("jn_genPy",       evSummary_.jn_genPy);
  t_->SetBranchAddress("jn_genPz",       evSummary_.jn_genPz);
  t_->SetBranchAddress("jn_genEn",       evSummary_.jn_genEn);
 
  //MET
  t_->SetBranchAddress("metn",     &evSummary_.metn);
  t_->SetBranchAddress("met_phi",  evSummary_.met_phi);
  t_->SetBranchAddress("met_pt",   evSummary_.met_pt);
  
  return true;
}

