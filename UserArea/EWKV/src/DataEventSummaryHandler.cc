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

  //filter bits
  t_->Branch("f_bits",   &evSummary_.f_bits,    "f_bits/I");

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
  t_->Branch("pf_px",     &evSummary_.pf_px,     "pf_px[pfn]/F");
  t_->Branch("pf_py",     &evSummary_.pf_py,     "pf_py[pfn]/F");
  t_->Branch("pf_pz",     &evSummary_.pf_pz,     "pf_pz[pfn]/F");
  t_->Branch("pf_en",     &evSummary_.pf_en,     "pf_en[pfn]/F");

  //generator level info
  t_->Branch("ngenITpu",    &evSummary_.ngenITpu,    "ngenITpu/I");
  t_->Branch("ngenOOTpu",   &evSummary_.ngenOOTpu,   "ngenOOTpu/I");
  t_->Branch("ngenOOTpum1", &evSummary_.ngenOOTpum1, "ngenOOTpum1/I");
  t_->Branch("ngenTruepu",  &evSummary_.ngenTruepu,  "ngenTruepu/I");
  t_->Branch("pthat",       &evSummary_.pthat,       "pthat/F");
  t_->Branch("genWeight",   &evSummary_.genWeight,   "genWeight/F");
  t_->Branch("qscale",      &evSummary_.qscale,      "qscale/F");
  t_->Branch("x1",          &evSummary_.x1,          "x1/F");
  t_->Branch("x2",          &evSummary_.x2,          "x2/F");
  t_->Branch("id1",         &evSummary_.id1,         "id1/I");
  t_->Branch("id2",         &evSummary_.id2,         "id2/I");
  t_->Branch("nup",         &evSummary_.nup,         "nup/I");

  //mc truth
  t_->Branch("mcn",         &evSummary_.mcn,         "mcn/I");
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
  t_->Branch("ln_genid",      evSummary_.ln_genid,     "ln_genid[ln]/I");
  t_->Branch("ln_px",         evSummary_.ln_px,        "ln_px[ln]/F");
  t_->Branch("ln_py",         evSummary_.ln_py,        "ln_py[ln]/F");
  t_->Branch("ln_pz",         evSummary_.ln_pz,        "ln_pz[ln]/F");
  t_->Branch("ln_en",         evSummary_.ln_en,        "ln_en[ln]/F");
  t_->Branch("ln_genpx",      evSummary_.ln_genpx,     "ln_genpx[ln]/F");
  t_->Branch("ln_genpy",      evSummary_.ln_genpy,     "ln_genpy[ln]/F");
  t_->Branch("ln_genpz",      evSummary_.ln_genpz,     "ln_genpz[ln]/F");
  t_->Branch("ln_genen",      evSummary_.ln_genen,     "ln_genen[ln]/F");
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
  t_->Branch("gn_idbits",   evSummary_.gn_idbits,    "gn_idbits[gn]/I");
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
  t_->Branch("egn_fbrem",        evSummary_.egn_fbrem,   "egn_fbrem[egn]/F");
  t_->Branch("egn_sce",          evSummary_.egn_sce,     "egn_sce[egn]/F");
  t_->Branch("egn_sceta",        evSummary_.egn_sceta,   "egn_sceta[egn]/F");
  t_->Branch("egn_scphi",        evSummary_.egn_scphi,   "egn_scphi[egn]/F");
  t_->Branch("egn_ooemoop",      evSummary_.egn_ooemoop, "egn_ooemoop[egn]/F");
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
  t_->Branch("jn_pfstart",     evSummary_.jn_pfstart,         "jn_pfstart[jn]/I");
  t_->Branch("jn_pfend",       evSummary_.jn_pfend,           "jn_pfend[jn]/I");
  t_->Branch("jn_px",          evSummary_.jn_px,              "jn_px[jn]/F");
  t_->Branch("jn_py",          evSummary_.jn_py,              "jn_py[jn]/F");
  t_->Branch("jn_pz",          evSummary_.jn_pz,              "jn_pz[jn]/F");
  t_->Branch("jn_en",          evSummary_.jn_en,              "jn_en[jn]/F");
  t_->Branch("jn_torawsf",     evSummary_.jn_torawsf,         "jn_torawsf[jn]/F");
  t_->Branch("jn_neutHadFrac", evSummary_.jn_neutHadFrac,     "jn_neutHadFrac[jn]/F");
  t_->Branch("jn_neutEmFrac",  evSummary_.jn_neutEmFrac,      "jn_neutEmFrac[jn]/F");
  t_->Branch("jn_chHadFrac",   evSummary_.jn_chHadFrac,       "jn_chHadFrac[jn]/F");
  t_->Branch("jn_area",        evSummary_.jn_area,            "jn_area[jn]/F");
  t_->Branch("jn_tchp",        evSummary_.jn_tchp,            "jn_tchp[jn]/F");
  t_->Branch("jn_jp",          evSummary_.jn_jp,              "jn_jp[jn]/F");
  t_->Branch("jn_origcsv",     evSummary_.jn_origcsv,         "jn_oringcsv[jn]/F");
  t_->Branch("jn_csv",         evSummary_.jn_csv,             "jn_csv[jn]/F");
  t_->Branch("jn_jpcsv",       evSummary_.jn_jpcsv,           "jn_jpcsv[jn]/F");
  t_->Branch("jn_slcsv",       evSummary_.jn_slcsv,           "jn_slcsv[jn]/F");
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
  t_->Branch("jn_genid",       evSummary_.jn_genid,           "jn_genid[jn]/I");
  t_->Branch("jn_genpx",       evSummary_.jn_genpx,           "jn_genpx[jn]/F");
  t_->Branch("jn_genpy",       evSummary_.jn_genpy,           "jn_genpy[jn]/F");
  t_->Branch("jn_genpz",       evSummary_.jn_genpz,           "jn_genpz[jn]/F");
  t_->Branch("jn_genen",       evSummary_.jn_genen,           "jn_genen[jn]/F");
  t_->Branch("jn_genjpx",      evSummary_.jn_genjpx,          "jn_genjpx[jn]/F");
  t_->Branch("jn_genjpy",      evSummary_.jn_genjpy,          "jn_genjpy[jn]/F");
  t_->Branch("jn_genjpz",      evSummary_.jn_genjpz,          "jn_genjpz[jn]/F");
  t_->Branch("jn_genjen",      evSummary_.jn_genjen,          "jn_genjen[jn]/F");
 
  //MET
  t_->Branch("metn",        &evSummary_.metn,       "metn/I");
  t_->Branch("met_phi",     evSummary_.met_phi,     "met_phi[metn]/F");
  t_->Branch("met_pt",      evSummary_.met_pt,      "met_pt[metn]/F");
  t_->Branch("met_sig",     evSummary_.met_sig,     "met_pt[metn]/F");
  t_->Branch("met_sigx2",   evSummary_.met_sigx2,   "met_sigx2[metn]/F");
  t_->Branch("met_sigxy",   evSummary_.met_sigxy,   "met_sigxy[metn]/F");
  t_->Branch("met_sigy2",   evSummary_.met_sigy2,   "met_sigy2[metn]/F");

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
  t_->SetBranchAddress("ln_trkPtErr",            evSummary_.ln_trkPtErr);
  t_->SetBranchAddress("ln_genid",               evSummary_.ln_genid);
  t_->SetBranchAddress("ln_genpx",               evSummary_.ln_genpx);
  t_->SetBranchAddress("ln_genpy",               evSummary_.ln_genpy);
  t_->SetBranchAddress("ln_genpz",               evSummary_.ln_genpz);
  t_->SetBranchAddress("ln_genen",               evSummary_.ln_genen);
  

  //Gamma Info
  t_->SetBranchAddress("gn",          &evSummary_.gn);
  t_->SetBranchAddress("gn_idbits",   evSummary_.gn_idbits);
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
  t_->SetBranchAddress("jn_pfstart",     evSummary_.jn_pfstart);
  t_->SetBranchAddress("jn_pfend",       evSummary_.jn_pfend);
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
  t_->SetBranchAddress("jn_origcsv",     evSummary_.jn_origcsv);
  t_->SetBranchAddress("jn_jpcsv",       evSummary_.jn_jpcsv);
  t_->SetBranchAddress("jn_slcsv",       evSummary_.jn_slcsv);
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
  t_->SetBranchAddress("jn_genid",       evSummary_.jn_genid);
  t_->SetBranchAddress("jn_genpx",       evSummary_.jn_genpx);
  t_->SetBranchAddress("jn_genpy",       evSummary_.jn_genpy);
  t_->SetBranchAddress("jn_genpz",       evSummary_.jn_genpz);
  t_->SetBranchAddress("jn_genen",       evSummary_.jn_genen);
  t_->SetBranchAddress("jn_genjpx",      evSummary_.jn_genjpx);
  t_->SetBranchAddress("jn_genjpy",      evSummary_.jn_genjpy);
  t_->SetBranchAddress("jn_genjpz",      evSummary_.jn_genjpz);
  t_->SetBranchAddress("jn_genjen",      evSummary_.jn_genjen);
 
  //MET
  t_->SetBranchAddress("metn",     &evSummary_.metn);
  t_->SetBranchAddress("met_phi",  evSummary_.met_phi);
  t_->SetBranchAddress("met_pt",   evSummary_.met_pt);
  
  return true;
}

//
data::PhysicsObjectCollection_t DataEventSummaryHandler::getPhysicsObject(int code)
{
  data::PhysicsObjectCollection_t coll;
  switch(code)
    {
    case LEPTONS:
      for(Int_t ilep=0; ilep<evSummary_.ln; ilep++)
	{
	  data::PhysicsObject_t lep(evSummary_.ln_px[ilep],evSummary_.ln_py[ilep],evSummary_.ln_pz[ilep],evSummary_.ln_en[ilep]);
	  lep.set("id",evSummary_.ln_id[ilep]);
	  lep.set("idbits",evSummary_.ln_idbits[ilep]);
	  lep.setVal("gIso03",    evSummary_.ln_gIso03[ilep]);  lep.setVal("chIso03",   evSummary_.ln_chIso03[ilep]);  lep.setVal("puchIso03", evSummary_.ln_puchIso03[ilep]); lep.setVal("nhIso03",   evSummary_.ln_nhIso03[ilep]);
	  lep.setVal("gIso04",    evSummary_.ln_gIso04[ilep]);  lep.setVal("chIso04",   evSummary_.ln_chIso04[ilep]);  lep.setVal("puchIso04", evSummary_.ln_puchIso04[ilep]); lep.setVal("nhIso04",   evSummary_.ln_nhIso04[ilep]);
	  lep.setVal("ecalIso03",    evSummary_.ln_ecalIso03[ilep]);  lep.setVal("trk03",   evSummary_.ln_trkIso03[ilep]);  lep.setVal("hcalIso03",   evSummary_.ln_hcalIso03[ilep]);
	  lep.setVal("ecalIso04",    evSummary_.ln_ecalIso04[ilep]);  lep.setVal("trk04",   evSummary_.ln_trkIso04[ilep]);  lep.setVal("hcalIso04",   evSummary_.ln_hcalIso04[ilep]);
	  lep.setVal("tk_d0",               evSummary_.ln_d0[ilep]);
	  lep.setVal("tk_dz",               evSummary_.ln_dZ[ilep]);
	  lep.setVal("tk_ip3d",             evSummary_.ln_ip3d[ilep]);
	  lep.setVal("tk_ip3dsig",          evSummary_.ln_ip3dsig[ilep]);
	  lep.setVal("tk_pt",               evSummary_.ln_trkpt[ilep]);
	  lep.setVal("tk_eta",              evSummary_.ln_trketa[ilep]);
	  lep.setVal("tk_phi",              evSummary_.ln_trkphi[ilep]);
	  lep.setVal("tk_chi2",             evSummary_.ln_trkchi2[ilep]);
	  lep.setVal("tk_validPixelHits",   evSummary_.ln_trkValidPixelHits[ilep]);
	  lep.setVal("tk_validTrackerHits", evSummary_.ln_trkValidTrackerHits[ilep]);
	  lep.setVal("tk_lostInnerHits",    evSummary_.ln_trkLostInnerHits[ilep]);
	  lep.setVal("tk_ptErr",            evSummary_.ln_trkPtErr[ilep]);
	  
	  if(abs(evSummary_.ln_id[ilep])==11)
	    {
	      int idx=evSummary_.ln_pid[ilep];
	      lep.setFlag("isconv", evSummary_.egn_isConv[idx]);
	      lep.setVal("scen", evSummary_.egn_sce[idx]);
	      lep.setVal("sceta", evSummary_.egn_sceta[idx]);
	      lep.setVal("scphi", evSummary_.egn_scphi[idx]);
	      lep.setVal("hoe", evSummary_.egn_hoe[idx]);
	      lep.setVal("h2te", evSummary_.egn_h2te[idx]);
	      lep.setVal("detain", evSummary_.egn_detain[idx]);
	      lep.setVal("dphiin", evSummary_.egn_dphiin[idx]);
	      lep.setVal("sihih", evSummary_.egn_sihih[idx]);
	      lep.setVal("sipip", evSummary_.egn_sipip[idx]);
	      lep.setVal("sihip", evSummary_.egn_sihip[idx]);
	      lep.setVal("eopin", evSummary_.egn_eopin[idx]);
	      lep.setVal("eopout", evSummary_.egn_eopout[idx]);
	      lep.setVal("r9", evSummary_.egn_r9[idx]);
	      lep.setVal("mvatrig",evSummary_.egn_mvatrigv0[idx]);
	      lep.setVal("mvanontrig",evSummary_.egn_mvanontrigv0[idx]);
	    }
	  else if(abs(evSummary_.ln_id[ilep])==13)
	    {
	      int idx=evSummary_.ln_pid[ilep];
	      lep.setVal("nMatches",                   evSummary_.mn_nMatches[idx]);
	      lep.setVal("nMatchedStations",           evSummary_.mn_nMatchedStations[idx]);
	      lep.setVal("validMuonHits",              evSummary_.mn_validMuonHits[idx]);
	      lep.setVal("innerTrackChi2",             evSummary_.mn_innerTrackChi2[idx]);
	      lep.setVal("trkLayersWithMeasurement",   evSummary_.mn_trkLayersWithMeasurement[idx]);
	      lep.setVal("pixelLayersWithMeasurement", evSummary_.mn_pixelLayersWithMeasurement[idx]);
	    }

	  data::PhysicsObject_t genLep(evSummary_.ln_genpx[ilep],evSummary_.ln_genpy[ilep],evSummary_.ln_genpz[ilep],evSummary_.ln_genen[ilep]);
	  genLep.set("id",evSummary_.ln_genid[ilep]);
	  lep.setObject("gen",genLep);
	  coll.push_back(lep);
	}
      break;
    case JETS:
      for(Int_t ijet=0; ijet<evSummary_.jn; ijet++)
	{
	  data::PhysicsObject_t jet(evSummary_.jn_px[ijet],evSummary_.jn_py[ijet],evSummary_.jn_pz[ijet],evSummary_.jn_en[ijet]);
	  jet.setVal("torawsf", evSummary_.jn_torawsf[ijet]);
	  jet.set("pfstart", evSummary_.jn_pfstart[ijet]);
	  jet.set("pfend",   evSummary_.jn_pfend[ijet]);
	  jet.set("idbits",  evSummary_.jn_idbits[ijet]);
	  jet.setVal("neutHadFrac",    evSummary_.jn_neutHadFrac[ijet]);
	  jet.setVal("neutEmFrac",     evSummary_.jn_neutEmFrac[ijet]);
	  jet.setVal("chHadFrac",      evSummary_.jn_chHadFrac[ijet]);
	  jet.setVal("area",           evSummary_.jn_area[ijet]);
	  jet.setVal("tchp",           evSummary_.jn_tchp[ijet]);
	  jet.setVal("jp",             evSummary_.jn_jp[ijet]);
	  jet.setVal("origcsv",        evSummary_.jn_origcsv[ijet]);
	  jet.setVal("csv",            evSummary_.jn_csv[ijet]);
	  jet.setVal("jpcsv",          evSummary_.jn_jpcsv[ijet]);
	  jet.setVal("slcsv",          evSummary_.jn_slcsv[ijet]);
	  jet.setVal("supercsv",       evSummary_.jn_supercsv[ijet]);
	  jet.setVal("ssvhe",          evSummary_.jn_ssvhe[ijet]);
	  jet.setVal("ivf",            evSummary_.jn_ivf[ijet]);
	  jet.setVal("puMVA",           evSummary_.jn_puMVA[ijet]);
	  jet.setVal("qgMVA",           evSummary_.jn_qgMVA[ijet]);
	  jet.setVal("beta",            evSummary_.jn_beta[ijet]);
	  jet.setVal("dRMean",          evSummary_.jn_dRMean[ijet]);
	  jet.setVal("ptRMS",           evSummary_.jn_ptRMS[ijet]);
	  jet.setVal("etaW",            evSummary_.jn_etaW[ijet]);
	  jet.setVal("phiW",            evSummary_.jn_phiW[ijet]);
 
	  float svxEn=sqrt(pow(evSummary_.jn_svxPx[ijet],2)+pow(evSummary_.jn_svxPy[ijet],2)+pow(evSummary_.jn_svxPz[ijet],2)+pow(evSummary_.jn_svxM[ijet],2));
	  data::PhysicsObject_t svx(evSummary_.jn_svxPx[ijet],evSummary_.jn_svxPy[ijet],evSummary_.jn_svxPz[ijet],svxEn);
	  svx.set("ntrk",evSummary_.jn_svxNtrk[ijet]);
	  svx.setVal("lxy",evSummary_.jn_svxLxy[ijet]);
	  svx.setVal("lxyErr",evSummary_.jn_svxLxyErr[ijet]);
	  jet.setObject("svx",svx);

	  float ivfEn=sqrt(pow(evSummary_.jn_ivfPx[ijet],2)+pow(evSummary_.jn_ivfPy[ijet],2)+pow(evSummary_.jn_ivfPz[ijet],2)+pow(evSummary_.jn_ivfM[ijet],2));
	  data::PhysicsObject_t ivf(evSummary_.jn_ivfPx[ijet],evSummary_.jn_ivfPy[ijet],evSummary_.jn_ivfPz[ijet],ivfEn);
	  ivf.set("ntrk",evSummary_.jn_ivfNtrk[ijet]);
	  ivf.setVal("lxy",evSummary_.jn_ivfLxy[ijet]);
	  ivf.setVal("lxyErr",evSummary_.jn_ivfLxyErr[ijet]);
	  jet.setObject("ivf",ivf);
  
	  data::PhysicsObject_t genParton(evSummary_.jn_genpx[ijet],evSummary_.jn_genpy[ijet],evSummary_.jn_genpz[ijet],evSummary_.jn_genen[ijet]);
	  genParton.set("id",evSummary_.jn_genid[ijet]);
	  jet.setObject("gen",genParton);
	  data::PhysicsObject_t genJet(evSummary_.jn_genjpx[ijet],evSummary_.jn_genjpy[ijet],evSummary_.jn_genjpz[ijet],evSummary_.jn_genjen[ijet]);
	  genJet.set("id",evSummary_.jn_genflav[ijet]);
	  jet.setObject("genJet",genJet);
	  coll.push_back(jet);
	}
      break;
    case PHOTONS:
      for(Int_t ig=0; ig<evSummary_.gn; ig++)
	{
	  data::PhysicsObject_t photon(evSummary_.gn_px[ig],evSummary_.gn_py[ig],evSummary_.gn_pz[ig],evSummary_.gn_en[ig]);
	  photon.set("id",  evSummary_.gn_idbits[ig]);
	  photon.setVal("gIso03",    evSummary_.gn_gIso03[ig]);        photon.setVal("chIso03",   evSummary_.gn_chIso03[ig]);  photon.setVal("puchIso03", evSummary_.gn_puchIso03[ig]);   photon.setVal("nhIso03",   evSummary_.gn_nhIso03[ig]);
	  photon.setVal("gIso04",    evSummary_.gn_gIso04[ig]);        photon.setVal("chIso04",   evSummary_.gn_chIso04[ig]);  photon.setVal("puchIso04", evSummary_.gn_puchIso04[ig]);   photon.setVal("nhIso04",   evSummary_.gn_nhIso04[ig]);
	  photon.setVal("ecalIso03",    evSummary_.gn_ecalIso03[ig]);  photon.setVal("trk03",   evSummary_.gn_trkIso03[ig]);   photon.setVal("hcalIso03",   evSummary_.gn_hcalIso03[ig]);
	  photon.setVal("ecalIso04",    evSummary_.gn_ecalIso04[ig]);  photon.setVal("trk04",   evSummary_.gn_trkIso04[ig]);   photon.setVal("hcalIso04",   evSummary_.gn_hcalIso04[ig]);
	  Int_t idx=evSummary_.gn_pid[ig];
	  photon.setFlag("isconv", evSummary_.egn_isConv[idx]);
	  photon.setVal("scen",    evSummary_.egn_sce[idx]);
	  photon.setVal("sceta",   evSummary_.egn_sceta[idx]);
	  photon.setVal("scphi",   evSummary_.egn_scphi[idx]);
	  photon.setVal("hoe",     evSummary_.egn_hoe[idx]);
	  photon.setVal("h2te",    evSummary_.egn_h2te[idx]);
	  photon.setVal("detain",  evSummary_.egn_detain[idx]);
	  photon.setVal("dphiin",  evSummary_.egn_dphiin[idx]);
	  photon.setVal("sihih",   evSummary_.egn_sihih[idx]);
	  photon.setVal("sipip",   evSummary_.egn_sipip[idx]);
	  photon.setVal("sihip",   evSummary_.egn_sihip[idx]);
	  photon.setVal("eopin",   evSummary_.egn_eopin[idx]);
	  photon.setVal("eopout",  evSummary_.egn_eopout[idx]);
	  photon.setVal("r9",      evSummary_.egn_r9[idx]);
	  //photon.setVal("mvatrig",evSummary_.egn_mvatrigv0[idx]);
	  //photon.setVal("mvanontrig",evSummary_.egn_mvanontrigv0[idx]);
	  coll.push_back(photon);
	}
      break;
    case MET:
      for(int imet=0; imet<evSummary_.metn; imet++)
	{
	  float px=evSummary_.met_pt[imet]*TMath::Cos(evSummary_.met_phi[imet]);
	  float py=evSummary_.met_pt[imet]*TMath::Sin(evSummary_.met_phi[imet]);
	  float en=sqrt(px*px+py*py);
	  data::PhysicsObject_t met(px,py,0.,en);
	  met.set("id",0);
	  coll.push_back(met);
	}
      break;
    case PFCANDIDATES:
      for(int ipf=0; ipf<evSummary_.pfn; ipf++)
	{
	  data::PhysicsObject_t pf(evSummary_.pf_px[ipf],evSummary_.pf_py[ipf],evSummary_.pf_pz[ipf],evSummary_.pf_en[ipf]);
	  pf.set("id",evSummary_.pf_id[ipf]);
	  pf.set("charge",evSummary_.pf_charge[ipf]);
	  coll.push_back(pf);
	}
      break;
    case GENPARTICLES:
      for(int imc=0; imc<evSummary_.mcn; imc++)
	{
	  data::PhysicsObject_t p(evSummary_.mc_px[imc],evSummary_.mc_py[imc],evSummary_.mc_pz[imc],evSummary_.mc_en[imc]);
	  p.set("id",evSummary_.mc_id[imc]);
	  p.set("status",evSummary_.mc_status[imc]);
	  p.setVal("lxy",evSummary_.mc_lxy[imc]);
	  coll.push_back(p);
	}
      break;
    default:
      break;
    }
  return coll;
}
