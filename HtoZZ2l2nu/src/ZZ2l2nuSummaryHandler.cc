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
  t_->Branch("curAvgInstLumi",      &evSummary_.curAvgInstLumi,      "curAvgInstLumi/F");
  t_->Branch("curIntegLumi",      &evSummary_.curIntegLumi,      "curIntegLumi/F");

  t_->Branch("cat",          &evSummary_.cat,          "cat/I");
  t_->Branch("mccat",        &evSummary_.mccat,        "mccat/I");
  t_->Branch("hasTrigger",   &evSummary_.hasTrigger,   "hasTrigger/O");
  t_->Branch("triggerType",  &evSummary_.triggerType,  "triggerType/I");
  
  //vertices and average energy density
  t_->Branch("nvtx",           &evSummary_.nvtx,           "nvtx/I");
  t_->Branch("rho",            &evSummary_.rho,            "rho/F");
  t_->Branch("rho25",          &evSummary_.rho25,          "rho25/F");
  t_->Branch("rho25Neut", &evSummary_.rho25Neut, "rho25Neut/F");

  //generator level info
  t_->Branch("ngenITpu",   &evSummary_.ngenITpu,   "ngenITpu/I");
  t_->Branch("ngenOOTpu",  &evSummary_.ngenOOTpu,  "ngenOOTpu/I");
  t_->Branch("ngenOOTpum1",  &evSummary_.ngenOOTpum1,  "ngenOOTpum1/I");
  t_->Branch("ngenTruepu",  &evSummary_.ngenTruepu,  "ngenTruepu/I");
  t_->Branch("puweight",     &evSummary_.puWeight,     "puweight/F");
  t_->Branch("hptWeights", evSummary_.hptWeights,  "hptWeights[5]/F");
  t_->Branch("pthat",      &evSummary_.pthat,      "pthat/F");
  t_->Branch("genWeight",  &evSummary_.genWeight,  "genWeight/F");
  t_->Branch("qscale",     &evSummary_.qscale,     "qscale/F");
  t_->Branch("x1",         &evSummary_.x1,         "x1/F");
  t_->Branch("x2",         &evSummary_.x2,         "x2/F");
  t_->Branch("id1",        &evSummary_.id1,        "id1/I");
  t_->Branch("id2",        &evSummary_.id2,        "id2/I");

  //selected di-leptons
  t_->Branch("l1_px",                  &evSummary_.l1_px,                  "l1_px/F");
  t_->Branch("l1_py",                  &evSummary_.l1_py,                  "l1_py/F");
  t_->Branch("l1_pz",                  &evSummary_.l1_pz,                  "l1_pz/F");
  t_->Branch("l1_en",                  &evSummary_.l1_en,                  "l1_en/F");
  t_->Branch("l1_ptErr",               &evSummary_.l1_ptErr,               "l1_ptErr/F");
  t_->Branch("l1_ecalIso",             &evSummary_.l1_ecalIso,             "l1_ecalIso/F");
  t_->Branch("l1_hcalIso",             &evSummary_.l1_hcalIso,             "l1_hcalIso/F");
  t_->Branch("l1_trkIso",              &evSummary_.l1_trkIso,              "l1_trkIso/F");
  t_->Branch("l1_gIso",                &evSummary_.l1_gIso,                "l1_gIso/F");
  t_->Branch("l1_chIso",               &evSummary_.l1_chIso,               "l1_chIso/F");
  t_->Branch("l1_puchIso",             &evSummary_.l1_puchIso,             "l1_puchIso/F");
  t_->Branch("l1_nhIso",               &evSummary_.l1_nhIso,               "l1_nhIso/F");
  t_->Branch("l1_id",                  &evSummary_.l1_id,                  "l1_id/I");
  t_->Branch("l1_genid",               &evSummary_.l1_genid,               "l1_genid/I");
  t_->Branch("l1_pid",                 &evSummary_.l1_pid,                 "l1_pid/I");  
  t_->Branch("l1_ensf",                &evSummary_.l1_ensf,                "l1_ensf/F");
  t_->Branch("l1_ensferr",             &evSummary_.l1_ensferr,             "l1_ensferr/F");
  t_->Branch("l1_d0",                  &evSummary_.l1_d0,                  "l1_d0/F");
  t_->Branch("l1_dZ",                  &evSummary_.l1_dZ,                  "l1_dZ/F");
  t_->Branch("l1_ip3d",                &evSummary_.l1_ip3d,                "l1_ip3d/F");
  t_->Branch("l1_ip3dsig",             &evSummary_.l1_ip3dsig,             "l1_ip3dsig/F");
  t_->Branch("l1_trkpt",               &evSummary_.l1_trkpt,               "l1_trkpt/F");
  t_->Branch("l1_trketa",              &evSummary_.l1_trketa,              "l1_trketa/F");
  t_->Branch("l1_trkphi",              &evSummary_.l1_trkphi,              "l1_trkphi/F");
  t_->Branch("l1_trkchi2",             &evSummary_.l1_trkchi2,             "l1_trkchi2/F");
  t_->Branch("l1_trkValidPixelHits",   &evSummary_.l1_trkValidPixelHits,   "l1_trkValidPixelHits/F");
  t_->Branch("l1_trkValidTrackerHits", &evSummary_.l1_trkValidTrackerHits, "l1_trkValidTrackerHits/F");
  t_->Branch("l1_trkLostInnerHits",    &evSummary_.l1_trkLostInnerHits,    "l1_trkLostInnerHits/F");

  t_->Branch("l2_px",                  &evSummary_.l2_px,                  "l2_px/F");
  t_->Branch("l2_py",                  &evSummary_.l2_py,                  "l2_py/F");
  t_->Branch("l2_pz",                  &evSummary_.l2_pz,                  "l2_pz/F");
  t_->Branch("l2_en",                  &evSummary_.l2_en,                  "l2_en/F");
  t_->Branch("l2_ptErr",               &evSummary_.l2_ptErr,               "l2_ptErr/F");
  t_->Branch("l2_ecalIso",             &evSummary_.l2_ecalIso,             "l2_ecalIso/F");
  t_->Branch("l2_hcalIso",             &evSummary_.l2_hcalIso,             "l2_hcalIso/F");
  t_->Branch("l2_trkIso",              &evSummary_.l2_trkIso,              "l2_trkIso/F");
  t_->Branch("l2_gIso",                &evSummary_.l2_gIso,                "l2_gIso/F");
  t_->Branch("l2_chIso",               &evSummary_.l2_chIso,               "l2_chIso/F");
  t_->Branch("l2_puchIso",             &evSummary_.l2_puchIso,             "l2_puchIso/F");
  t_->Branch("l2_nhIso",               &evSummary_.l2_nhIso,               "l2_nhIso/F");
  t_->Branch("l2_id",                  &evSummary_.l2_id,                  "l2_id/I");
  t_->Branch("l2_genid",               &evSummary_.l2_genid,               "l2_genid/I");
  t_->Branch("l2_pid",                 &evSummary_.l2_pid,                 "l2_pid/I");  
  t_->Branch("l2_ensf",                &evSummary_.l2_ensf,                "l2_ensf/F");
  t_->Branch("l2_ensferr",             &evSummary_.l2_ensferr,             "l2_ensferr/F");
  t_->Branch("l2_d0",                  &evSummary_.l2_d0,                  "l2_d0/F");
  t_->Branch("l2_dZ",                  &evSummary_.l2_dZ,                  "l2_dZ/F");
  t_->Branch("l2_ip3d",                &evSummary_.l2_ip3d,                "l2_ip3d/F");
  t_->Branch("l2_ip3dsig",             &evSummary_.l2_ip3dsig,             "l2_ip3dsig/F");
  t_->Branch("l2_trkpt",               &evSummary_.l2_trkpt,               "l2_trkpt/F");
  t_->Branch("l2_trketa",              &evSummary_.l2_trketa,              "l2_trketa/F");
  t_->Branch("l2_trkphi",              &evSummary_.l2_trkphi,              "l2_trkphi/F");
  t_->Branch("l2_trkchi2",             &evSummary_.l2_trkchi2,             "l2_trkchi2/F");
  t_->Branch("l2_trkValidPixelHits",   &evSummary_.l2_trkValidPixelHits,   "l2_trkValidPixelHits/F");
  t_->Branch("l2_trkValidTrackerHits", &evSummary_.l2_trkValidTrackerHits, "l2_trkValidTrackerHits/F");
  t_->Branch("l2_trkLostInnerHits",    &evSummary_.l2_trkLostInnerHits,    "l2_trkLostInnerHits/F");

  //selected additional-leptons
  t_->Branch("ln",                     &evSummary_.ln,                    "ln/I");
  t_->Branch("ln_px",                  evSummary_.ln_px,                  "ln_px[ln]/F");
  t_->Branch("ln_py",                  evSummary_.ln_py,                  "ln_py[ln]/F");
  t_->Branch("ln_pz",                  evSummary_.ln_pz,                  "ln_pz[ln]/F");
  t_->Branch("ln_en",                  evSummary_.ln_en,                  "ln_en[ln]/F");
  t_->Branch("ln_ptErr",               evSummary_.ln_ptErr,               "ln_ptErr[ln]/F");
  t_->Branch("ln_ecalIso",             evSummary_.ln_ecalIso,             "ln_ecalIso[ln]/F");
  t_->Branch("ln_hcalIso",             evSummary_.ln_hcalIso,             "ln_hcalIso[ln]/F");
  t_->Branch("ln_trkIso",              evSummary_.ln_trkIso,              "ln_trkIso[ln]/F");
  t_->Branch("ln_gIso",                evSummary_.ln_gIso,                "ln_gIso[ln]/F");
  t_->Branch("ln_chIso",               evSummary_.ln_chIso,               "ln_chIso[ln]/F");
  t_->Branch("ln_puchIso",             evSummary_.ln_puchIso,             "ln_puchIso[ln]/F");
  t_->Branch("ln_nhIso",               evSummary_.ln_nhIso,               "ln_nhIso[ln]/F");
  t_->Branch("ln_id",                  evSummary_.ln_id,                  "ln_id[ln]/I");
  t_->Branch("ln_genid",               evSummary_.ln_genid,               "ln_genid[ln]/I");
  t_->Branch("ln_pid",                 evSummary_.ln_pid,                 "ln_pid[ln]/I");  
  t_->Branch("ln_ensf",                evSummary_.ln_ensf,                "ln_ensf[ln]/F");
  t_->Branch("ln_ensferr",             evSummary_.ln_ensferr,             "ln_ensferr[ln]/F");
  t_->Branch("ln_d0",                  evSummary_.ln_d0,                  "ln_d0[ln]/F");
  t_->Branch("ln_dZ",                  evSummary_.ln_dZ,                  "ln_dZ[ln]/F");
  t_->Branch("ln_ip3d",                evSummary_.ln_ip3d,                "ln_ip3d[ln]/F");
  t_->Branch("ln_ip3dsig",             evSummary_.ln_ip3dsig,             "ln_ip3dsig[ln]/F");
  t_->Branch("ln_trkpt",               evSummary_.ln_trkpt,               "ln_trkpt[ln]/F");
  t_->Branch("ln_trketa",              evSummary_.ln_trketa,              "ln_trketa[ln]/F");
  t_->Branch("ln_trkphi",              evSummary_.ln_trkphi,              "ln_trkphi[ln]/F");
  t_->Branch("ln_trkchi2",             evSummary_.ln_trkchi2,             "ln_trkchi2[ln]/F");
  t_->Branch("ln_trkValidPixelHits",   evSummary_.ln_trkValidPixelHits,   "ln_trkValidPixelHits[ln]/F");
  t_->Branch("ln_trkValidTrackerHits", evSummary_.ln_trkValidTrackerHits, "ln_trkValidTrackerHits[ln]/F");
  t_->Branch("ln_trkLostInnerHits",    evSummary_.ln_trkLostInnerHits,    "ln_trkLostInnerHits[ln]/F");

  //lepton id fields
  t_->Branch("en",         &evSummary_.en,         "en/I");
  t_->Branch("en_idbits",  evSummary_.en_idbits,   "en_idbits[en]/I");
  t_->Branch("en_hoe",     evSummary_.en_hoe ,     "en_hoe[en]/F");
  t_->Branch("en_hoebc",     evSummary_.en_hoebc ,     "en_hoebc[en]/F");
  t_->Branch("en_dphiin",  evSummary_.en_dphiin ,  "en_dphiin[en]/F");
  t_->Branch("en_detain",  evSummary_.en_detain ,  "en_detain[en]/F");
  t_->Branch("en_sihih",   evSummary_.en_sihih ,   "en_sihih[en]/F");
  t_->Branch("en_sipip",   evSummary_.en_sipip ,   "en_sipip[en]/F");
  t_->Branch("en_r9",      evSummary_.en_r9,       "en_r9[en]/F");
  t_->Branch("en_sce",     evSummary_.en_sce ,     "en_sce[en]/F");
  t_->Branch("en_sceta",   evSummary_.en_sceta ,   "en_sceta[en]/F");
  t_->Branch("en_scphi",   evSummary_.en_scphi ,   "en_scphi[en]/F");
  t_->Branch("en_e2x5max", evSummary_.en_e2x5max , "en_e2x5max[en]/F");
  t_->Branch("en_e1x5",    evSummary_.en_e1x5 ,    "en_e1x5[en]/F");
  t_->Branch("en_e5x5",    evSummary_.en_e5x5 ,    "en_e5x5[en]/F");
  t_->Branch("en_h2te",    evSummary_.en_h2te ,    "en_h2te[en]/F");
  t_->Branch("en_h2tebc",  evSummary_.en_h2tebc ,  "en_h2tebc[en]/F");
  t_->Branch("en_ooemoop", evSummary_.en_ooemoop,  "en_ooemoop[en]/F");
  t_->Branch("en_fbrem",   evSummary_.en_fbrem,    "en_fbrem[en]/F");
  t_->Branch("en_eopin",   evSummary_.en_eopin,    "en_eopin[en]/F");
  t_->Branch("en_dEtaCalo",   evSummary_.en_dEtaCalo,    "en_dEtaCalo[en]/F");
  t_->Branch("en_dPhiCalo",   evSummary_.en_dPhiCalo,    "en_dPhiCalo[en]/F");
  t_->Branch("en_kfchi2",   evSummary_.en_kfchi2,    "en_kfchi2[en]/F");
  t_->Branch("en_kfhits",   evSummary_.en_kfhits,    "en_kfhits[en]/F");
  t_->Branch("en_kfhitsall",   evSummary_.en_kfhitsall,    "en_kfhitsall[en]/F");
  t_->Branch("en_sihip",   evSummary_.en_sihip,    "en_sihip[en]/F");
  t_->Branch("en_nbrems",   evSummary_.en_nbrems,    "en_nbrems[en]/F");
  t_->Branch("en_etawidth",   evSummary_.en_etawidth,    "en_etawidth[en]/F");
  t_->Branch("en_phiwidth",   evSummary_.en_phiwidth,    "en_phiwidth[en]/F");
  t_->Branch("en_e1x5e5x5",   evSummary_.en_e1x5e5x5,    "en_e1x5e5x5[en]/F");
  t_->Branch("en_preShowerOverRaw",   evSummary_.en_preShowerOverRaw,    "en_preShowerOverRaw[en]/F");
  t_->Branch("en_eopout",   evSummary_.en_eopout,    "en_eopout[en]/F");

  t_->Branch("mn",               &evSummary_.mn,              "mn/I");
  t_->Branch("mn_idbits",        evSummary_.mn_idbits,        "mn_idbits[mn]/I");
  t_->Branch("mn_nMatches",      evSummary_.mn_nMatches ,     "mn_nMatches[mn]/F");
  t_->Branch("mn_nMatchedStations", evSummary_.mn_nMatchedStations ,     "mn_nMatchedStations[mn]/F");
  t_->Branch("mn_validMuonHits", evSummary_.mn_validMuonHits, "mn_validMuonHits[mn]/F");
  t_->Branch("mn_innerTrackChi2",evSummary_.mn_innerTrackChi2,"mn_innerTrackChi2[mn]/F");
  t_->Branch("mn_trkLayersWithMeasurement",evSummary_.mn_trkLayersWithMeasurement,"mn_trkLayersWithMeasurement[mn]/F");
  t_->Branch("mn_pixelLayersWithMeasurement",evSummary_.mn_pixelLayersWithMeasurement,"mn_pixelLayersWithMeasurement[mn]/F");

  //selected CHS jets
  t_->Branch("jn",             &evSummary_.jn,            "jn/I");
  t_->Branch("jn_px",          evSummary_.jn_px,          "jn_px[jn]/F");
  t_->Branch("jn_py",          evSummary_.jn_py,          "jn_py[jn]/F");
  t_->Branch("jn_pz",          evSummary_.jn_pz,          "jn_pz[jn]/F");
  t_->Branch("jn_en",          evSummary_.jn_en,          "jn_en[jn]/F");
  t_->Branch("jn_rawsf",       evSummary_.jn_rawsf,       "jn_rawsf[jn]/F");
  t_->Branch("jn_genid",       evSummary_.jn_genid,       "jn_genid[jn]/I");
  t_->Branch("jn_genflav",     evSummary_.jn_genflav,     "jn_genflav[jn]/I");
  t_->Branch("jn_btag1",       evSummary_.jn_btag1,       "jn_btag1[jn]/F");
  t_->Branch("jn_btag2",       evSummary_.jn_btag2,       "jn_btag2[jn]/F");
  t_->Branch("jn_btag3",       evSummary_.jn_btag3,       "jn_btag3[jn]/F");
  t_->Branch("jn_btag4",       evSummary_.jn_btag4,       "jn_btag4[jn]/F");
  t_->Branch("jn_neutHadFrac", evSummary_.jn_neutHadFrac, "jn_neutHadFrac[jn]/F");
  t_->Branch("jn_neutEmFrac",  evSummary_.jn_neutEmFrac,  "jn_neutEmFrac[jn]/F");
  t_->Branch("jn_chHadFrac",   evSummary_.jn_chHadFrac,   "jn_chHadFrac[jn]/F");
  t_->Branch("jn_genpt",       evSummary_.jn_genpt,       "jn_genpt[jn]/F");
  t_->Branch("jn_pumva",       evSummary_.jn_pumva,       "jn_pumva[jn]/F");
  t_->Branch("jn_puminmva",    evSummary_.jn_puminmva,    "jn_puminmva[jn]/F");
  t_->Branch("jn_beta",        evSummary_.jn_beta,        "jn_beta[jn]/F");
  t_->Branch("jn_betaStar",    evSummary_.jn_betaStar,    "jn_betaStar[jn]/F");
  t_->Branch("jn_dRMean",      evSummary_.jn_dRMean,      "jn_dRMean[jn]/F");
  t_->Branch("jn_ptD",         evSummary_.jn_ptD,         "jn_ptD[jn]/F");
  t_->Branch("jn_ptRMS",       evSummary_.jn_ptRMS,       "jn_ptRMS[jn]/F");
  t_->Branch("jn_etaW",        evSummary_.jn_etaW,        "jn_etaW[jn]/F");
  t_->Branch("jn_phiW",        evSummary_.jn_phiW,        "jn_phiW[jn]/F");
  t_->Branch("jn_idbits",      evSummary_.jn_idbits,      "jn_idbits[jn]/I");

  //selected PF jets 
  t_->Branch("ajn",             &evSummary_.ajn,            "ajn/I");
  t_->Branch("ajn_px",          evSummary_.ajn_px,          "ajn_px[ajn]/F");
  t_->Branch("ajn_py",          evSummary_.ajn_py,          "ajn_py[ajn]/F");
  t_->Branch("ajn_pz",          evSummary_.ajn_pz,          "ajn_pz[ajn]/F");
  t_->Branch("ajn_en",          evSummary_.ajn_en,          "ajn_en[ajn]/F");
  t_->Branch("ajn_rawsf",       evSummary_.ajn_rawsf,       "ajn_rawsf[ajn]/F");
  t_->Branch("ajn_genid",       evSummary_.ajn_genid,       "ajn_genid[ajn]/I");
  t_->Branch("ajn_genflav",     evSummary_.ajn_genflav,     "ajn_genflav[ajn]/I");
  t_->Branch("ajn_btag1",       evSummary_.ajn_btag1,       "ajn_btag1[ajn]/F");
  t_->Branch("ajn_btag2",       evSummary_.ajn_btag2,       "ajn_btag2[ajn]/F");
  t_->Branch("ajn_btag3",       evSummary_.ajn_btag3,       "ajn_btag3[ajn]/F");
  t_->Branch("ajn_btag4",       evSummary_.ajn_btag4,       "ajn_btag4[ajn]/F");
  t_->Branch("ajn_neutHadFrac", evSummary_.ajn_neutHadFrac, "ajn_neutHadFrac[ajn]/F");
  t_->Branch("ajn_neutEmFrac",  evSummary_.ajn_neutEmFrac,  "ajn_neutEmFrac[ajn]/F");
  t_->Branch("ajn_chHadFrac",   evSummary_.ajn_chHadFrac,   "ajn_chHadFrac[ajn]/F");
  t_->Branch("ajn_genpt",       evSummary_.ajn_genpt,       "ajn_genpt[ajn]/F");
  t_->Branch("ajn_pumva",       evSummary_.ajn_pumva,       "ajn_pumva[ajn]/F");
  t_->Branch("ajn_puminmva",    evSummary_.ajn_puminmva,    "ajn_puminmva[jn]/F");  
  t_->Branch("ajn_beta",        evSummary_.ajn_beta,        "ajn_beta[ajn]/F");
  t_->Branch("ajn_betaStar",    evSummary_.ajn_betaStar,    "ajn_betaStar[ajn]/F");
  t_->Branch("ajn_dRMean",      evSummary_.ajn_dRMean,      "ajn_dRMean[ajn]/F");
  t_->Branch("ajn_ptD",         evSummary_.ajn_ptD,         "ajn_ptD[ajn]/F");
  t_->Branch("ajn_ptRMS",       evSummary_.ajn_ptRMS,       "ajn_ptRMS[ajn]/F");
  t_->Branch("ajn_etaW",        evSummary_.ajn_etaW,        "ajn_etaW[jn]/F");
  t_->Branch("ajn_phiW",        evSummary_.ajn_phiW,        "ajn_phiW[jn]/F");
  t_->Branch("ajn_idbits",      evSummary_.ajn_idbits,      "ajn_idbits[ajn]/I");

  //HT
  t_->Branch("htvec_px",   &evSummary_.htvec_px, "htvec_px/F");
  t_->Branch("htvec_py",   &evSummary_.htvec_py, "htvec_py/F");
  t_->Branch("htvec_pz",   &evSummary_.htvec_pz, "htvec_pz/F");
  t_->Branch("htvec_en",   &evSummary_.htvec_en, "htvec_en/F");


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
  t_->Branch("gn",          &evSummary_.gn,         "gn/I"); 
  t_->Branch("g_px",        evSummary_.g_px,        "g_px[gn]/F");
  t_->Branch("g_py",        evSummary_.g_py,        "g_py[gn]/F");
  t_->Branch("g_pz",        evSummary_.g_pz,        "g_pz[gn]/F");
  t_->Branch("g_en",        evSummary_.g_en,        "g_en[gn]/F");
  t_->Branch("g_iso1",      evSummary_.g_iso1,      "g_iso1[gn]/F");
  t_->Branch("g_iso2",      evSummary_.g_iso2,      "g_iso2[gn]/F");
  t_->Branch("g_iso3",      evSummary_.g_iso3,      "g_iso3[gn]/F");
  t_->Branch("g_sihih",     evSummary_.g_sihih,     "g_sihih[gn]/F");
  t_->Branch("g_sipip",     evSummary_.g_sipip,     "g_sipip[gn]/F");
  t_->Branch("g_r9",        evSummary_.g_r9,        "g_r9[gn]/F");
  t_->Branch("g_hoe",       evSummary_.g_hoe,       "g_hoe[gn]/F");
  t_->Branch("g_htoe",      evSummary_.g_htoe,      "g_htoe[gn]/F");
  t_->Branch("g_corren",    evSummary_.g_corren,    "g_corren[gn]/F");
  t_->Branch("g_correnerr", evSummary_.g_correnerr, "g_correnerr[gn]/F");
  t_->Branch("g_idbits",    evSummary_.g_idbits,    "g_idbits[gn]/I");

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

  t_->Branch("nmeasurements", &evSummary_.nmeasurements, "nmeasurements/I");
  t_->Branch("measurements", evSummary_.measurements, "measurements[nmeasurements]/I");

  return true;
}

//
bool ZZ2l2nuSummaryHandler::attachToTree(TTree *t, bool full)
{
  if(t==0) return false;
  t_ = t;

  //event info
  t_->GetBranch("run")->SetAddress(&evSummary_.run);
  t_->GetBranch("lumi")->SetAddress(&evSummary_.lumi);
  t_->GetBranch("event")->SetAddress(&evSummary_.event);
  if(t_->GetBranch("curAvgInstLumi")) t_->GetBranch("curAvgInstLumi")->SetAddress(&evSummary_.curAvgInstLumi);
  if(t_->GetBranch("curIntegLumi"))   t_->GetBranch("curIntegLumi")->SetAddress(&evSummary_.curIntegLumi);
  t_->GetBranch("cat")->SetAddress(&evSummary_.cat);
  t_->GetBranch("mccat")->SetAddress(&evSummary_.mccat);

  //trigger bit
  t_->GetBranch("hasTrigger")->SetAddress(&evSummary_.hasTrigger);
  if(t_->GetBranch("triggerType")) t_->GetBranch("triggerType")->SetAddress(&evSummary_.triggerType);
  
  //vertices and average energy density
  t_->GetBranch("nvtx")->SetAddress(&evSummary_.nvtx);
  t_->GetBranch("rho")->SetAddress( &evSummary_.rho );
  t_->GetBranch("rho25")->SetAddress( &evSummary_.rho25 );
  if(t_->GetBranch("rho25Neut")) t_->GetBranch("rho25Neut")->SetAddress(&evSummary_.rho25Neut);

  //generator level info
  t_->GetBranch("puweight")->SetAddress(&evSummary_.puWeight);
  t_->GetBranch("hptWeights")->SetAddress(evSummary_.hptWeights);
  t_->GetBranch("ngenITpu")->SetAddress(&evSummary_.ngenITpu);
  t_->GetBranch("ngenOOTpu")->SetAddress(&evSummary_.ngenOOTpu);
  t_->GetBranch("ngenOOTpum1")->SetAddress(&evSummary_.ngenOOTpum1);
  if(t_->GetBranch("ngenTruepu")) t_->GetBranch("ngenTruepu")->SetAddress(&evSummary_.ngenTruepu);
  t_->GetBranch("pthat")->SetAddress(&evSummary_.pthat);
  t_->GetBranch("genWeight")->SetAddress(&evSummary_.genWeight);
  t_->GetBranch("qscale")->SetAddress(&evSummary_.qscale);
  t_->GetBranch("x1")->SetAddress(&evSummary_.x1);
  t_->GetBranch("x2")->SetAddress(&evSummary_.x2);
  t_->GetBranch("id1")->SetAddress(&evSummary_.id1);
  t_->GetBranch("id2")->SetAddress(&evSummary_.id2);

  //selected di-leptons
  t_->GetBranch("l1_px"                  )->SetAddress( &evSummary_.l1_px);
  t_->GetBranch("l1_py"                  )->SetAddress( &evSummary_.l1_py);
  t_->GetBranch("l1_pz"                  )->SetAddress( &evSummary_.l1_pz);
  t_->GetBranch("l1_en"                  )->SetAddress( &evSummary_.l1_en);
  t_->GetBranch("l1_id"                  )->SetAddress( &evSummary_.l1_id);
  t_->GetBranch("l1_pid"                 )->SetAddress( &evSummary_.l1_pid);
  t_->GetBranch("l1_genid"               )->SetAddress( &evSummary_.l1_genid);
  t_->GetBranch("l1_ptErr"               )->SetAddress( &evSummary_.l1_ptErr);
  t_->GetBranch("l1_ecalIso"             )->SetAddress( &evSummary_.l1_ecalIso);
  t_->GetBranch("l1_hcalIso"             )->SetAddress( &evSummary_.l1_hcalIso);
  t_->GetBranch("l1_trkIso"              )->SetAddress( &evSummary_.l1_trkIso);
  t_->GetBranch("l1_gIso"                )->SetAddress( &evSummary_.l1_gIso);
  t_->GetBranch("l1_chIso"               )->SetAddress( &evSummary_.l1_chIso);
  t_->GetBranch("l1_puchIso"             )->SetAddress( &evSummary_.l1_puchIso);
  t_->GetBranch("l1_nhIso"               )->SetAddress( &evSummary_.l1_nhIso);
  t_->GetBranch("l1_ensf"                )->SetAddress(&evSummary_.l1_ensf);
  t_->GetBranch("l1_ensferr"             )->SetAddress(&evSummary_.l1_ensferr);
  t_->GetBranch("l1_d0"                  )->SetAddress(&evSummary_.l1_d0);
  t_->GetBranch("l1_dZ"                  )->SetAddress(&evSummary_.l1_dZ);
  t_->GetBranch("l1_trkpt"               )->SetAddress(&evSummary_.l1_trkpt);
  t_->GetBranch("l1_trketa"              )->SetAddress(&evSummary_.l1_trketa);
  t_->GetBranch("l1_trkphi"              )->SetAddress(&evSummary_.l1_trkphi);
  t_->GetBranch("l1_trkchi2"             )->SetAddress(&evSummary_.l1_trkchi2);
  t_->GetBranch("l1_trkValidPixelHits"   )->SetAddress(&evSummary_.l1_trkValidPixelHits);
  t_->GetBranch("l1_trkValidTrackerHits" )->SetAddress(&evSummary_.l1_trkValidTrackerHits);
  t_->GetBranch("l1_trkLostInnerHits"    )->SetAddress(&evSummary_.l1_trkLostInnerHits);

  t_->GetBranch("l2_px"                  )->SetAddress( &evSummary_.l2_px);
  t_->GetBranch("l2_py"                  )->SetAddress( &evSummary_.l2_py);
  t_->GetBranch("l2_pz"                  )->SetAddress( &evSummary_.l2_pz);
  t_->GetBranch("l2_en"                  )->SetAddress( &evSummary_.l2_en);
  t_->GetBranch("l2_id"                  )->SetAddress( &evSummary_.l2_id);
  t_->GetBranch("l2_pid"                 )->SetAddress( &evSummary_.l2_pid);
  t_->GetBranch("l2_genid"               )->SetAddress( &evSummary_.l2_genid);
  t_->GetBranch("l2_ptErr"               )->SetAddress( &evSummary_.l2_ptErr);
  t_->GetBranch("l2_ecalIso"             )->SetAddress( &evSummary_.l2_ecalIso);
  t_->GetBranch("l2_hcalIso"             )->SetAddress( &evSummary_.l2_hcalIso);
  t_->GetBranch("l2_trkIso"              )->SetAddress( &evSummary_.l2_trkIso);
  t_->GetBranch("l2_gIso"                )->SetAddress( &evSummary_.l2_gIso);
  t_->GetBranch("l2_chIso"               )->SetAddress( &evSummary_.l2_chIso);
  t_->GetBranch("l2_puchIso"             )->SetAddress( &evSummary_.l2_puchIso);
  t_->GetBranch("l2_nhIso"               )->SetAddress( &evSummary_.l2_nhIso);
  t_->GetBranch("l2_ensf"                )->SetAddress(&evSummary_.l2_ensf);
  t_->GetBranch("l2_ensferr"             )->SetAddress(&evSummary_.l2_ensferr);
  t_->GetBranch("l2_d0"                  )->SetAddress(&evSummary_.l2_d0);
  t_->GetBranch("l2_dZ"                  )->SetAddress(&evSummary_.l2_dZ);
  t_->GetBranch("l2_trkpt"               )->SetAddress(&evSummary_.l2_trkpt);
  t_->GetBranch("l2_trketa"              )->SetAddress(&evSummary_.l2_trketa);
  t_->GetBranch("l2_trkphi"              )->SetAddress(&evSummary_.l2_trkphi);
  t_->GetBranch("l2_trkchi2"             )->SetAddress(&evSummary_.l2_trkchi2);
  t_->GetBranch("l2_trkValidPixelHits"   )->SetAddress(&evSummary_.l2_trkValidPixelHits);
  t_->GetBranch("l2_trkValidTrackerHits" )->SetAddress(&evSummary_.l2_trkValidTrackerHits);
  t_->GetBranch("l2_trkLostInnerHits"    )->SetAddress(&evSummary_.l2_trkLostInnerHits);


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
  t_->GetBranch("ln")          ->SetAddress( &evSummary_.ln);
  t_->GetBranch("ln_px")       ->SetAddress(evSummary_.ln_px);
  t_->GetBranch("ln_py")       ->SetAddress(evSummary_.ln_py);
  t_->GetBranch("ln_pz")       ->SetAddress(evSummary_.ln_pz);
  t_->GetBranch("ln_en")       ->SetAddress(evSummary_.ln_en);
  t_->GetBranch("ln_pid")       ->SetAddress(evSummary_.ln_pid);
  t_->GetBranch("ln_id")       ->SetAddress(evSummary_.ln_id);
  t_->GetBranch("ln_genid")    ->SetAddress(evSummary_.ln_genid);
  t_->GetBranch("ln_ptErr")    ->SetAddress(evSummary_.ln_ptErr);
  t_->GetBranch("ln_ecalIso")  ->SetAddress(evSummary_.ln_ecalIso);
  t_->GetBranch("ln_hcalIso")  ->SetAddress(evSummary_.ln_hcalIso);
  t_->GetBranch("ln_trkIso")   ->SetAddress(evSummary_.ln_trkIso);
  t_->GetBranch("ln_gIso")     ->SetAddress(evSummary_.ln_gIso);
  t_->GetBranch("ln_chIso")    ->SetAddress( &evSummary_.ln_chIso);
  t_->GetBranch("ln_puchIso")  ->SetAddress( &evSummary_.ln_puchIso);
  t_->GetBranch("ln_nhIso")    ->SetAddress( &evSummary_.ln_nhIso);

  t_->GetBranch("en"              )->SetAddress(&evSummary_.en);
  t_->GetBranch("en_idbits"       )->SetAddress(evSummary_.en_idbits);
  t_->GetBranch("en_hoe"          )->SetAddress(evSummary_.en_hoe);
  if(t_->GetBranch("en_hoebc")    ) t_->GetBranch("en_hoebc")->SetAddress(evSummary_.en_hoebc);
  t_->GetBranch("en_dphiin"       )->SetAddress(evSummary_.en_dphiin);
  t_->GetBranch("en_detain"       )->SetAddress(evSummary_.en_detain);
  t_->GetBranch("en_sihih"        )->SetAddress(evSummary_.en_sihih);
  t_->GetBranch("en_sipip"        )->SetAddress(evSummary_.en_sipip);
  t_->GetBranch("en_r9"           )->SetAddress(evSummary_.en_r9);
  t_->GetBranch("en_sce"          )->SetAddress(evSummary_.en_sce);
  t_->GetBranch("en_sceta"        )->SetAddress(evSummary_.en_sceta);
  t_->GetBranch("en_scphi"        )->SetAddress(evSummary_.en_scphi);
  t_->GetBranch("en_e2x5max"      )->SetAddress(evSummary_.en_e2x5max);
  t_->GetBranch("en_e1x5"         )->SetAddress(evSummary_.en_e1x5);
  t_->GetBranch("en_e5x5"         )->SetAddress(evSummary_.en_e5x5);
  t_->GetBranch("en_h2te"         )->SetAddress(evSummary_.en_h2te);
  t_->GetBranch("en_h2tebc"       )->SetAddress(evSummary_.en_h2tebc);
  t_->GetBranch("en_ooemoop"      )->SetAddress(evSummary_.en_ooemoop);
  t_->GetBranch("en_fbrem"        )->SetAddress(evSummary_.en_fbrem);
  t_->GetBranch("en_eopin"        )->SetAddress(evSummary_.en_eopin);
  t_->GetBranch("en_dEtaCalo"     )->SetAddress(evSummary_.en_dEtaCalo);
  t_->GetBranch("en_dPhiCalo"     )->SetAddress(evSummary_.en_dPhiCalo);
  t_->GetBranch("en_kfchi2"       )->SetAddress(evSummary_.en_kfchi2);
  t_->GetBranch("en_kfhits"       )->SetAddress(evSummary_.en_kfhits);
  t_->GetBranch("en_kfhitsall"    )->SetAddress(evSummary_.en_kfhitsall);
  t_->GetBranch("en_sihip"        )->SetAddress(evSummary_.en_sihip);
  t_->GetBranch("en_nbrems"       )->SetAddress(evSummary_.en_nbrems);
  t_->GetBranch("en_etawidth"     )->SetAddress(evSummary_.en_etawidth);
  t_->GetBranch("en_phiwidth"     )->SetAddress(evSummary_.en_phiwidth);
  t_->GetBranch("en_e1x5e5x5"     )->SetAddress(evSummary_.en_e1x5e5x5);
  t_->GetBranch("en_preShowerOverRaw")->SetAddress(evSummary_.en_preShowerOverRaw);
  t_->GetBranch("en_eopout"        )->SetAddress(evSummary_.en_eopout);
  t_->GetBranch("mn"              )->SetAddress(&evSummary_.mn);
  t_->GetBranch("mn_idbits"       )->SetAddress(evSummary_.mn_idbits);
  t_->GetBranch("mn_nMatches"     )->SetAddress(evSummary_.mn_nMatches);
  t_->GetBranch("mn_nMatchedStations")->SetAddress(evSummary_.mn_nMatchedStations);
  t_->GetBranch("mn_validMuonHits")->SetAddress(evSummary_.mn_validMuonHits);
  if(t_->GetBranch("mn_innerTrackChi2"))             t_->GetBranch("mn_innerTrackChi2")->SetAddress(evSummary_.mn_innerTrackChi2);
  if(t_->GetBranch("mn_trkLayersWithMeasurement"))   t_->GetBranch("mn_trkLayersWithMeasurement")->SetAddress(evSummary_.mn_trkLayersWithMeasurement);
  if(t_->GetBranch("mn_pixelLayersWithMeasurement")) t_->GetBranch("mn_pixelLayersWithMeasurement")->SetAddress(evSummary_.mn_pixelLayersWithMeasurement);

  //selected jets
  t_->GetBranch("jn")             ->SetAddress(&evSummary_.jn);
  t_->GetBranch("jn_px")          ->SetAddress(evSummary_.jn_px);
  t_->GetBranch("jn_py")          ->SetAddress(evSummary_.jn_py);
  t_->GetBranch("jn_pz")          ->SetAddress(evSummary_.jn_pz);
  t_->GetBranch("jn_en")          ->SetAddress(evSummary_.jn_en);
  t_->GetBranch("jn_genid")       ->SetAddress(evSummary_.jn_genid);
  t_->GetBranch("jn_genflav")     ->SetAddress(evSummary_.jn_genflav);
  t_->GetBranch("jn_btag1")       ->SetAddress(evSummary_.jn_btag1);
  t_->GetBranch("jn_btag2")       ->SetAddress(evSummary_.jn_btag2);
  t_->GetBranch("jn_btag3")       ->SetAddress(evSummary_.jn_btag3);
  if(t_->GetBranch("jn_btag4")) t_->GetBranch("jn_btag4")->SetAddress(evSummary_.jn_btag4);
  t_->GetBranch("jn_neutHadFrac") ->SetAddress(evSummary_.jn_neutHadFrac);
  t_->GetBranch("jn_neutEmFrac")  ->SetAddress(evSummary_.jn_neutEmFrac);
  t_->GetBranch("jn_chHadFrac")   ->SetAddress(evSummary_.jn_chHadFrac);
  t_->GetBranch("jn_genpt")       ->SetAddress(evSummary_.jn_genpt);
  t_->GetBranch("jn_pumva")       ->SetAddress(evSummary_.jn_pumva);
  if(t_->GetBranch("jn_puminmva"))    t_->GetBranch("jn_puminmva")       ->SetAddress(evSummary_.jn_puminmva);
  t_->GetBranch("jn_beta")        ->SetAddress(evSummary_.jn_beta);
  t_->GetBranch("jn_betaStar")    ->SetAddress(evSummary_.jn_betaStar);
  t_->GetBranch("jn_dRMean")      ->SetAddress(evSummary_.jn_dRMean);
  t_->GetBranch("jn_ptD")         ->SetAddress(evSummary_.jn_ptD);
  t_->GetBranch("jn_ptRMS")       ->SetAddress(evSummary_.jn_ptRMS);
  if(t_->GetBranch("jn_etaW"))  t_->GetBranch("jn_etaW")->SetAddress(evSummary_.jn_etaW);
  if(t_->GetBranch("jn_phiW"))  t_->GetBranch("jn_phiW")->SetAddress(evSummary_.jn_phiW);
  t_->GetBranch("jn_idbits")->SetAddress(evSummary_.jn_idbits);
  
  t_->GetBranch("htvec_px")->SetAddress(&evSummary_.htvec_px);
  t_->GetBranch("htvec_py")->SetAddress(&evSummary_.htvec_py);
  t_->GetBranch("htvec_pz")->SetAddress(&evSummary_.htvec_pz);
  t_->GetBranch("htvec_en")->SetAddress(&evSummary_.htvec_en);

  t_->GetBranch("ajn")             ->SetAddress(&evSummary_.ajn);
  t_->GetBranch("ajn_px")          ->SetAddress(evSummary_.ajn_px);
  t_->GetBranch("ajn_py")          ->SetAddress(evSummary_.ajn_py);
  t_->GetBranch("ajn_pz")          ->SetAddress(evSummary_.ajn_pz);
  t_->GetBranch("ajn_en")          ->SetAddress(evSummary_.ajn_en);
  t_->GetBranch("ajn_genid")       ->SetAddress(evSummary_.ajn_genid);
  t_->GetBranch("ajn_genflav")     ->SetAddress(evSummary_.ajn_genflav);
  t_->GetBranch("ajn_btag1")       ->SetAddress(evSummary_.ajn_btag1);
  t_->GetBranch("ajn_btag2")       ->SetAddress(evSummary_.ajn_btag2);
  t_->GetBranch("ajn_btag3")       ->SetAddress(evSummary_.ajn_btag3);
  if(t_->GetBranch("ajn_btag4"))  t_->GetBranch("ajn_btag4")       ->SetAddress(evSummary_.ajn_btag4);
  t_->GetBranch("ajn_neutHadFrac") ->SetAddress(evSummary_.ajn_neutHadFrac);
  t_->GetBranch("ajn_neutEmFrac")  ->SetAddress(evSummary_.ajn_neutEmFrac);
  t_->GetBranch("ajn_chHadFrac")   ->SetAddress(evSummary_.ajn_chHadFrac);
  t_->GetBranch("ajn_genpt")       ->SetAddress(evSummary_.ajn_genpt);
  t_->GetBranch("ajn_pumva")       ->SetAddress(evSummary_.ajn_pumva);
  if(t_->GetBranch("ajn_puminmva"))    t_->GetBranch("ajn_puminmva")       ->SetAddress(evSummary_.ajn_puminmva);  
  t_->GetBranch("ajn_beta")        ->SetAddress(evSummary_.ajn_beta);
  t_->GetBranch("ajn_betaStar")    ->SetAddress(evSummary_.ajn_betaStar);
  t_->GetBranch("ajn_dRMean")      ->SetAddress(evSummary_.ajn_dRMean);
  t_->GetBranch("ajn_ptD")         ->SetAddress(evSummary_.ajn_ptD);
  t_->GetBranch("ajn_ptRMS")       ->SetAddress(evSummary_.ajn_ptRMS);
  if(t_->GetBranch("ajn_etaW"))  t_->GetBranch("ajn_etaW")->SetAddress(evSummary_.ajn_etaW);
  if(t_->GetBranch("ajn_phiW"))  t_->GetBranch("ajn_phiW")->SetAddress(evSummary_.ajn_phiW);
  t_->GetBranch("ajn_idbits")      ->SetAddress(evSummary_.ajn_idbits);

  //primary vertex
  t_->GetBranch("vtx_px")->SetAddress( &evSummary_.vtx_px);
  t_->GetBranch("vtx_py")->SetAddress( &evSummary_.vtx_py);
  t_->GetBranch("vtx_pz")->SetAddress( &evSummary_.vtx_pz);
  t_->GetBranch("vtx_en")->SetAddress( &evSummary_.vtx_en);
  
  //MET
  t_->GetBranch("nmet")    ->SetAddress( &evSummary_.nmet);
  t_->GetBranch("met_phi") ->SetAddress( evSummary_.met_phi);
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
  t_->GetBranch("gn")          ->SetAddress(&evSummary_.gn);
  t_->GetBranch("g_px")        ->SetAddress(evSummary_.g_px);
  t_->GetBranch("g_py")        ->SetAddress(evSummary_.g_py);
  t_->GetBranch("g_pz")        ->SetAddress(evSummary_.g_pz);
  t_->GetBranch("g_en")        ->SetAddress(evSummary_.g_en);
  t_->GetBranch("g_iso1")      ->SetAddress(evSummary_.g_iso1);
  t_->GetBranch("g_iso2")      ->SetAddress(evSummary_.g_iso2);
  t_->GetBranch("g_iso3")      ->SetAddress(evSummary_.g_iso3);
  t_->GetBranch("g_sihih")     ->SetAddress(evSummary_.g_sihih);
  t_->GetBranch("g_sipip")     ->SetAddress(evSummary_.g_sipip);
  t_->GetBranch("g_r9")        ->SetAddress(evSummary_.g_r9);
  t_->GetBranch("g_hoe")       ->SetAddress(evSummary_.g_hoe);
  if(t_->GetBranch("g_htoe"))  t_->GetBranch("g_htoe")->SetAddress(evSummary_.g_htoe);
  t_->GetBranch("g_corren")    ->SetAddress(evSummary_.g_corren);
  t_->GetBranch("g_correnerr") ->SetAddress(evSummary_.g_correnerr);
  t_->GetBranch("g_idbits")    ->SetAddress(evSummary_.g_idbits);
  
  //Higgs Info
  t_->GetBranch("h_px")->SetAddress(&evSummary_.h_px);
  t_->GetBranch("h_py")->SetAddress(&evSummary_.h_py);
  t_->GetBranch("h_pz")->SetAddress(&evSummary_.h_pz);
  t_->GetBranch("h_en")->SetAddress(&evSummary_.h_en);
  
  //mc truth
  t_->GetBranch("nmcparticles") ->SetAddress(&evSummary_.nmcparticles);
  t_->GetBranch("mc_px")        ->SetAddress(evSummary_.mc_px);
  t_->GetBranch("mc_py")        ->SetAddress(evSummary_.mc_py);
  t_->GetBranch("mc_pz")        ->SetAddress(evSummary_.mc_pz);
  t_->GetBranch("mc_en")        ->SetAddress(evSummary_.mc_en);
  t_->GetBranch("mc_id")        ->SetAddress(evSummary_.mc_id);

  //additional measurements you may want to add  
  if( t_->GetBranch("nmeasurements") )  t_->GetBranch("nmeasurements")->SetAddress(&evSummary_.nmeasurements);
  if( t_->GetBranch("measurements") )   t_->GetBranch("measurements")->SetAddress(evSummary_.measurements);
  
  return true;
}

//
void ZZ2l2nuSummaryHandler::resetStruct()
{
  evSummary_.nmcparticles=0;
  evSummary_.run=0;    evSummary_.lumi=0;   evSummary_.event=0;  evSummary_.cat=0;
  evSummary_.l1_px=0;  evSummary_.l1_py=0;  evSummary_.l1_pz=0;  evSummary_.l1_en=0; evSummary_.l1_id=0;
  evSummary_.l2_px=0;  evSummary_.l2_py=0;  evSummary_.l2_pz=0;  evSummary_.l2_en=0; evSummary_.l2_id=0;
  evSummary_.ln=0;     evSummary_.mn=0;     evSummary_.en=0;
  evSummary_.jn=0;     evSummary_.ajn=0;
  evSummary_.gn=0;
  evSummary_.nmet=0;
}

//
void ZZ2l2nuSummaryHandler::fillTree()
{
  if(t_) t_->Fill();
}


bool ZZ2l2nuSummaryHandler::hasSpoilerAlert(bool isData)
{
  if(!isData) return false;
  if(evSummary_.cat!=1 && evSummary_.cat!=2)return false; //make sure that we clean only EE or MUMU events

  //compute the invariant mass
  LorentzVector l1(evSummary_.l1_px,evSummary_.l1_py,evSummary_.l1_pz,evSummary_.l1_en);
  LorentzVector l2(evSummary_.l2_px,evSummary_.l2_py,evSummary_.l2_pz,evSummary_.l2_en);
  LorentzVector visible(l1+l2);
  LorentzVector invisible(evSummary_.met_pt[0]*cos(evSummary_.met_phi[0]),evSummary_.met_pt[0]*sin(evSummary_.met_phi[0]),0,evSummary_.met_pt[0]);
  LorentzVector sum=visible+invisible;
  Double_t tMass = TMath::Power(TMath::Sqrt(TMath::Power(visible.pt(),2)+pow(visible.mass(),2))+TMath::Sqrt(TMath::Power(invisible.pt(),2)+pow(visible.mass(),2)),2);
  tMass-=TMath::Power(sum.pt(),2);
  return (TMath::Sqrt(tMass)>250);
}

//
ZZ2l2nuSummaryHandler::~ZZ2l2nuSummaryHandler()
{
}
