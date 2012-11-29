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
  t_->Branch("en_mvatrigv0",   evSummary_.en_mvatrigv0,    "en_mvatrigv0[en]/F");
  t_->Branch("en_mvanontrigv0",   evSummary_.en_mvanontrigv0,    "en_mvanontrigv0[en]/F");
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
  t_->Branch("jn_btag5",       evSummary_.jn_btag5,       "jn_btag5[jn]/F");
  t_->Branch("jn_btag6",       evSummary_.jn_btag6,       "jn_btag6[jn]/F");
  t_->Branch("jn_btag7",       evSummary_.jn_btag7,       "jn_btag7[jn]/F");
  t_->Branch("jn_btag8",       evSummary_.jn_btag8,       "jn_btag8[jn]/F");
  t_->Branch("jn_btag9",       evSummary_.jn_btag9,       "jn_btag9[jn]/F");
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
  t_->Branch("jn_isCfromGsplit", evSummary_.jn_isCfromGsplit, "jn_isCfromGsplit[jn]/O");
  t_->Branch("jn_isBfromGsplit", evSummary_.jn_isBfromGsplit, "jn_isBfromGsplit[jn]/O");
  t_->Branch("jn_lxy",           evSummary_.jn_lxy,           "jn_lxy[jn]/F");
  t_->Branch("jn_lxyErr",        evSummary_.jn_lxyErr,        "jn_lxyErr[jn]/F");
  t_->Branch("jn_svmass",        evSummary_.jn_svmass,        "jn_svmass[jn]/F");
  t_->Branch("jn_svpt",        evSummary_.jn_svpt,        "jn_svpt[jn]/F");
  t_->Branch("jn_svdr",        evSummary_.jn_svdr,        "jn_svdr[jn]/F");

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
  t_->Branch("ajn_btag5",       evSummary_.ajn_btag5,       "ajn_btag5[ajn]/F");
  t_->Branch("ajn_btag6",       evSummary_.ajn_btag6,       "ajn_btag6[ajn]/F");
  t_->Branch("ajn_btag7",       evSummary_.ajn_btag7,       "ajn_btag7[ajn]/F");
  t_->Branch("ajn_btag8",       evSummary_.ajn_btag8,       "ajn_btag8[ajn]/F");
  t_->Branch("ajn_btag9",       evSummary_.ajn_btag9,       "ajn_btag9[ajn]/F");
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
  t_->Branch("ajn_isCfromGsplit", evSummary_.ajn_isCfromGsplit, "ajn_isCfromGsplit[ajn]/O");
  t_->Branch("ajn_isBfromGsplit", evSummary_.ajn_isBfromGsplit, "ajn_isBfromGsplit[ajn]/O");
  t_->Branch("ajn_lxy",           evSummary_.ajn_lxy,           "ajn_lxy[ajn]/F");
  t_->Branch("ajn_lxyErr",        evSummary_.ajn_lxyErr,        "ajn_lxyErr[ajn]/F");
  t_->Branch("ajn_svmass",        evSummary_.ajn_svmass,        "ajn_svmass[ajn]/F");
  t_->Branch("ajn_svpt",        evSummary_.ajn_svpt,        "ajn_svpt[jn]/F");
  t_->Branch("ajn_svdr",        evSummary_.ajn_svdr,        "ajn_svdr[jn]/F");


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
  t_->Branch("gn_prescale",          evSummary_.gn_prescale,         "gn_prescale[16]/I"); 
  t_->Branch("gn_triggerWord",       &evSummary_.gn_triggerWord,      "gn_triggerWord/I"); 
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
  t_->Branch("mc_nup", &evSummary_.mc_nup, "mc_nup/I");
  t_->Branch("mc_px", evSummary_.mc_px, "mc_px[nmcparticles]/F");
  t_->Branch("mc_py", evSummary_.mc_py, "mc_py[nmcparticles]/F");
  t_->Branch("mc_pz", evSummary_.mc_pz, "mc_pz[nmcparticles]/F");
  t_->Branch("mc_en", evSummary_.mc_en, "mc_en[nmcparticles]/F");
  t_->Branch("mc_lxy", evSummary_.mc_lxy, "mc_lxy[nmcparticles]/F");
  t_->Branch("mc_id", evSummary_.mc_id, "mc_id[nmcparticles]/I");
  t_->Branch("mc_status", evSummary_.mc_status, "mc_status[nmcparticles]/I");

  t_->Branch("nmeasurements", &evSummary_.nmeasurements, "nmeasurements/I");
  t_->Branch("measurements", evSummary_.measurements, "measurements[nmeasurements]/F");

  return true;
}

//
bool ZZ2l2nuSummaryHandler::attachToTree(TTree *t, bool full)
{
  if(t==0) return false;
  t_ = t;

  //event info
  t_->SetBranchAddress("run",&evSummary_.run);
  t_->SetBranchAddress("lumi",&evSummary_.lumi);
  t_->SetBranchAddress("event",&evSummary_.event);
  t_->SetBranchAddress("curAvgInstLumi",&evSummary_.curAvgInstLumi);
  t_->SetBranchAddress("curIntegLumi",&evSummary_.curIntegLumi);
  t_->SetBranchAddress("cat",&evSummary_.cat);
  t_->SetBranchAddress("mccat",&evSummary_.mccat);

  //trigger bit
  t_->SetBranchAddress("hasTrigger",&evSummary_.hasTrigger);
  t_->SetBranchAddress("triggerType",&evSummary_.triggerType);
  
  //vertices and average energy density
  t_->SetBranchAddress("nvtx",&evSummary_.nvtx);
  t_->SetBranchAddress("rho", &evSummary_.rho );
  t_->SetBranchAddress("rho25", &evSummary_.rho25 );
  t_->SetBranchAddress("rho25Neut",&evSummary_.rho25Neut);

  //generator level info
  t_->SetBranchAddress("puweight",&evSummary_.puWeight);
  t_->SetBranchAddress("hptWeights",evSummary_.hptWeights);
  t_->SetBranchAddress("ngenITpu",&evSummary_.ngenITpu);
  t_->SetBranchAddress("ngenOOTpu",&evSummary_.ngenOOTpu);
  t_->SetBranchAddress("ngenOOTpum1",&evSummary_.ngenOOTpum1);
  t_->SetBranchAddress("ngenTruepu",&evSummary_.ngenTruepu);
  t_->SetBranchAddress("pthat",&evSummary_.pthat);
  t_->SetBranchAddress("genWeight",&evSummary_.genWeight);
  t_->SetBranchAddress("qscale",&evSummary_.qscale);
  t_->SetBranchAddress("x1",&evSummary_.x1);
  t_->SetBranchAddress("x2",&evSummary_.x2);
  t_->SetBranchAddress("id1",&evSummary_.id1);
  t_->SetBranchAddress("id2",&evSummary_.id2);

  //selected di-leptons
  t_->SetBranchAddress("l1_px"                  , &evSummary_.l1_px);
  t_->SetBranchAddress("l1_py"                  , &evSummary_.l1_py);
  t_->SetBranchAddress("l1_pz"                  , &evSummary_.l1_pz);
  t_->SetBranchAddress("l1_en"                  , &evSummary_.l1_en);
  t_->SetBranchAddress("l1_id"                  , &evSummary_.l1_id);
  t_->SetBranchAddress("l1_pid"                 , &evSummary_.l1_pid);
  t_->SetBranchAddress("l1_genid"               , &evSummary_.l1_genid);
  t_->SetBranchAddress("l1_ptErr"               , &evSummary_.l1_ptErr);
  t_->SetBranchAddress("l1_ecalIso"             , &evSummary_.l1_ecalIso);
  t_->SetBranchAddress("l1_hcalIso"             , &evSummary_.l1_hcalIso);
  t_->SetBranchAddress("l1_trkIso"              , &evSummary_.l1_trkIso);
  t_->SetBranchAddress("l1_gIso"                , &evSummary_.l1_gIso);
  t_->SetBranchAddress("l1_chIso"               , &evSummary_.l1_chIso);
  t_->SetBranchAddress("l1_puchIso"             , &evSummary_.l1_puchIso);
  t_->SetBranchAddress("l1_nhIso"               , &evSummary_.l1_nhIso);
  t_->SetBranchAddress("l1_ensf"                ,&evSummary_.l1_ensf);
  t_->SetBranchAddress("l1_ensferr"             ,&evSummary_.l1_ensferr);
  t_->SetBranchAddress("l1_d0"                  ,&evSummary_.l1_d0);
  t_->SetBranchAddress("l1_dZ"                  ,&evSummary_.l1_dZ);
  t_->SetBranchAddress("l1_trkpt"               ,&evSummary_.l1_trkpt);
  t_->SetBranchAddress("l1_trketa"              ,&evSummary_.l1_trketa);
  t_->SetBranchAddress("l1_trkphi"              ,&evSummary_.l1_trkphi);
  t_->SetBranchAddress("l1_trkchi2"             ,&evSummary_.l1_trkchi2);
  t_->SetBranchAddress("l1_trkValidPixelHits"   ,&evSummary_.l1_trkValidPixelHits);
  t_->SetBranchAddress("l1_trkValidTrackerHits" ,&evSummary_.l1_trkValidTrackerHits);
  t_->SetBranchAddress("l1_trkLostInnerHits"    ,&evSummary_.l1_trkLostInnerHits);

  t_->SetBranchAddress("l2_px"                  , &evSummary_.l2_px);
  t_->SetBranchAddress("l2_py"                  , &evSummary_.l2_py);
  t_->SetBranchAddress("l2_pz"                  , &evSummary_.l2_pz);
  t_->SetBranchAddress("l2_en"                  , &evSummary_.l2_en);
  t_->SetBranchAddress("l2_id"                  , &evSummary_.l2_id);
  t_->SetBranchAddress("l2_pid"                 , &evSummary_.l2_pid);
  t_->SetBranchAddress("l2_genid"               , &evSummary_.l2_genid);
  t_->SetBranchAddress("l2_ptErr"               , &evSummary_.l2_ptErr);
  t_->SetBranchAddress("l2_ecalIso"             , &evSummary_.l2_ecalIso);
  t_->SetBranchAddress("l2_hcalIso"             , &evSummary_.l2_hcalIso);
  t_->SetBranchAddress("l2_trkIso"              , &evSummary_.l2_trkIso);
  t_->SetBranchAddress("l2_gIso"                , &evSummary_.l2_gIso);
  t_->SetBranchAddress("l2_chIso"               , &evSummary_.l2_chIso);
  t_->SetBranchAddress("l2_puchIso"             , &evSummary_.l2_puchIso);
  t_->SetBranchAddress("l2_nhIso"               , &evSummary_.l2_nhIso);
  t_->SetBranchAddress("l2_ensf"                ,&evSummary_.l2_ensf);
  t_->SetBranchAddress("l2_ensferr"             ,&evSummary_.l2_ensferr);
  t_->SetBranchAddress("l2_d0"                  ,&evSummary_.l2_d0);
  t_->SetBranchAddress("l2_dZ"                  ,&evSummary_.l2_dZ);
  t_->SetBranchAddress("l2_trkpt"               ,&evSummary_.l2_trkpt);
  t_->SetBranchAddress("l2_trketa"              ,&evSummary_.l2_trketa);
  t_->SetBranchAddress("l2_trkphi"              ,&evSummary_.l2_trkphi);
  t_->SetBranchAddress("l2_trkchi2"             ,&evSummary_.l2_trkchi2);
  t_->SetBranchAddress("l2_trkValidPixelHits"   ,&evSummary_.l2_trkValidPixelHits);
  t_->SetBranchAddress("l2_trkValidTrackerHits" ,&evSummary_.l2_trkValidTrackerHits);
  t_->SetBranchAddress("l2_trkLostInnerHits"    ,&evSummary_.l2_trkLostInnerHits);

  //selected additional-leptons
  t_->SetBranchAddress("ln",          &evSummary_.ln);
  t_->SetBranchAddress("ln_px",       evSummary_.ln_px);
  t_->SetBranchAddress("ln_py",       evSummary_.ln_py);
  t_->SetBranchAddress("ln_pz",       evSummary_.ln_pz);
  t_->SetBranchAddress("ln_en",       evSummary_.ln_en);
  t_->SetBranchAddress("ln_pid",      evSummary_.ln_pid);
  t_->SetBranchAddress("ln_id",       evSummary_.ln_id);
  t_->SetBranchAddress("ln_genid",    evSummary_.ln_genid);
  t_->SetBranchAddress("ln_ptErr",    evSummary_.ln_ptErr);
  t_->SetBranchAddress("ln_ecalIso",  evSummary_.ln_ecalIso);
  t_->SetBranchAddress("ln_hcalIso",  evSummary_.ln_hcalIso);
  t_->SetBranchAddress("ln_trkIso",   evSummary_.ln_trkIso);
  t_->SetBranchAddress("ln_gIso",     evSummary_.ln_gIso);
  t_->SetBranchAddress("ln_chIso",     &evSummary_.ln_chIso);
  t_->SetBranchAddress("ln_puchIso",   &evSummary_.ln_puchIso);
  t_->SetBranchAddress("ln_nhIso",    &evSummary_.ln_nhIso);
  t_->SetBranchAddress("ln_ptErr",               evSummary_.ln_ptErr);
  t_->SetBranchAddress("ln_ensf",                evSummary_.ln_ensf);
  t_->SetBranchAddress("ln_ensferr",             evSummary_.ln_ensferr);
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

  //lepton id branches
  t_->SetBranchAddress("en"              ,&evSummary_.en);
  t_->SetBranchAddress("en_idbits"       ,evSummary_.en_idbits);
  t_->SetBranchAddress("en_hoe"          ,evSummary_.en_hoe);
  t_->SetBranchAddress("en_hoebc"        ,evSummary_.en_hoebc);
  t_->SetBranchAddress("en_dphiin"       ,evSummary_.en_dphiin);
  t_->SetBranchAddress("en_detain"       ,evSummary_.en_detain);
  t_->SetBranchAddress("en_sihih"        ,evSummary_.en_sihih);
  t_->SetBranchAddress("en_sipip"        ,evSummary_.en_sipip);
  t_->SetBranchAddress("en_r9"           ,evSummary_.en_r9);
  t_->SetBranchAddress("en_sce"          ,evSummary_.en_sce);
  t_->SetBranchAddress("en_sceta"        ,evSummary_.en_sceta);
  t_->SetBranchAddress("en_scphi"        ,evSummary_.en_scphi);
  t_->SetBranchAddress("en_e2x5max"      ,evSummary_.en_e2x5max);
  t_->SetBranchAddress("en_e1x5"         ,evSummary_.en_e1x5);
  t_->SetBranchAddress("en_e5x5"         ,evSummary_.en_e5x5);
  t_->SetBranchAddress("en_h2te"         ,evSummary_.en_h2te);
  t_->SetBranchAddress("en_h2tebc"       ,evSummary_.en_h2tebc);
  t_->SetBranchAddress("en_ooemoop"      ,evSummary_.en_ooemoop);
  t_->SetBranchAddress("en_fbrem"        ,evSummary_.en_fbrem);
  t_->SetBranchAddress("en_eopin"        ,evSummary_.en_eopin);
  t_->SetBranchAddress("en_dEtaCalo"     ,evSummary_.en_dEtaCalo);
  t_->SetBranchAddress("en_dPhiCalo"     ,evSummary_.en_dPhiCalo);
  t_->SetBranchAddress("en_kfchi2"       ,evSummary_.en_kfchi2);
  t_->SetBranchAddress("en_kfhits"       ,evSummary_.en_kfhits);
  t_->SetBranchAddress("en_kfhitsall"    ,evSummary_.en_kfhitsall);
  t_->SetBranchAddress("en_sihip"        ,evSummary_.en_sihip);
  t_->SetBranchAddress("en_nbrems"       ,evSummary_.en_nbrems);
  t_->SetBranchAddress("en_etawidth"     ,evSummary_.en_etawidth);
  t_->SetBranchAddress("en_phiwidth"     ,evSummary_.en_phiwidth);
  t_->SetBranchAddress("en_e1x5e5x5"     ,evSummary_.en_e1x5e5x5);
  t_->SetBranchAddress("en_preShowerOverRaw",evSummary_.en_preShowerOverRaw);
  t_->SetBranchAddress("en_eopout"        ,evSummary_.en_eopout);
  if(t_->GetBranch("en_mvatrigv0"))    t_->SetBranchAddress("en_mvatrigv0",     evSummary_.en_mvatrigv0);
  if(t_->GetBranch("en_mvanontrigv0")) t_->SetBranchAddress("en_mvanontrigv0",  evSummary_.en_mvanontrigv0);
  t_->SetBranchAddress("mn"              ,&evSummary_.mn);
  t_->SetBranchAddress("mn_idbits"       ,evSummary_.mn_idbits);
  t_->SetBranchAddress("mn_nMatches"     ,evSummary_.mn_nMatches);
  t_->SetBranchAddress("mn_nMatchedStations",evSummary_.mn_nMatchedStations);
  t_->SetBranchAddress("mn_validMuonHits",evSummary_.mn_validMuonHits);
  t_->SetBranchAddress("mn_innerTrackChi2",evSummary_.mn_innerTrackChi2);
  t_->SetBranchAddress("mn_trkLayersWithMeasurement",evSummary_.mn_trkLayersWithMeasurement);
  t_->SetBranchAddress("mn_pixelLayersWithMeasurement",evSummary_.mn_pixelLayersWithMeasurement);

  //selected jets
  t_->SetBranchAddress("jn",             &evSummary_.jn);
  t_->SetBranchAddress("jn_px",          evSummary_.jn_px);
  t_->SetBranchAddress("jn_py",          evSummary_.jn_py);
  t_->SetBranchAddress("jn_pz",          evSummary_.jn_pz);
  t_->SetBranchAddress("jn_en",          evSummary_.jn_en);
  t_->SetBranchAddress("jn_rawsf",       evSummary_.jn_rawsf);
  t_->SetBranchAddress("jn_genid",       evSummary_.jn_genid);
  t_->SetBranchAddress("jn_genflav",     evSummary_.jn_genflav);
  t_->SetBranchAddress("jn_btag1",       evSummary_.jn_btag1);
  t_->SetBranchAddress("jn_btag2",       evSummary_.jn_btag2);
  t_->SetBranchAddress("jn_btag3",       evSummary_.jn_btag3);
  t_->SetBranchAddress("jn_btag4",       evSummary_.jn_btag4);
  if(t_->GetBranch("jn_btag5"))  t_->SetBranchAddress("jn_btag5",       evSummary_.jn_btag5);
  if(t_->GetBranch("jn_btag6"))  t_->SetBranchAddress("jn_btag6",       evSummary_.jn_btag6);
  if(t_->GetBranch("jn_btag7"))  t_->SetBranchAddress("jn_btag7",       evSummary_.jn_btag7);
  if(t_->GetBranch("jn_btag8"))  t_->SetBranchAddress("jn_btag8",       evSummary_.jn_btag8);
  if(t_->GetBranch("jn_btag9"))  t_->SetBranchAddress("jn_btag9",       evSummary_.jn_btag9);
  t_->SetBranchAddress("jn_neutHadFrac", evSummary_.jn_neutHadFrac);
  t_->SetBranchAddress("jn_neutEmFrac",  evSummary_.jn_neutEmFrac);
  t_->SetBranchAddress("jn_chHadFrac",   evSummary_.jn_chHadFrac);
  t_->SetBranchAddress("jn_genpt",       evSummary_.jn_genpt);
  t_->SetBranchAddress("jn_pumva",       evSummary_.jn_pumva);
  t_->SetBranchAddress("jn_puminmva",    evSummary_.jn_puminmva);
  t_->SetBranchAddress("jn_beta",        evSummary_.jn_beta);
  t_->SetBranchAddress("jn_betaStar",    evSummary_.jn_betaStar);
  t_->SetBranchAddress("jn_dRMean",      evSummary_.jn_dRMean);
  t_->SetBranchAddress("jn_ptD",         evSummary_.jn_ptD);
  t_->SetBranchAddress("jn_ptRMS",       evSummary_.jn_ptRMS);
  t_->SetBranchAddress("jn_etaW",evSummary_.jn_etaW);
  t_->SetBranchAddress("jn_phiW",evSummary_.jn_phiW);
  t_->SetBranchAddress("jn_idbits",evSummary_.jn_idbits);
  if(t_->GetBranch("jn_isCfromGsplit")) t_->SetBranchAddress("jn_isCfromGsplit", evSummary_.jn_isCfromGsplit);
  if(t_->GetBranch("jn_isBfromGsplit")) t_->SetBranchAddress("jn_isBfromGsplit", evSummary_.jn_isBfromGsplit);
  if(t_->GetBranch("jn_lxy"))           t_->SetBranchAddress("jn_lxy",           evSummary_.jn_lxy);
  if(t_->GetBranch("jn_lxyErr"))        t_->SetBranchAddress("jn_lxyErr",        evSummary_.jn_lxyErr);
  if(t_->GetBranch("jn_svmass"))        t_->SetBranchAddress("jn_svmass",        evSummary_.jn_svmass);
  if(t_->GetBranch("jn_svpt"))          t_->SetBranchAddress("jn_svpt",          evSummary_.jn_svpt);
  if(t_->GetBranch("jn_svdr"))          t_->SetBranchAddress("jn_svdr",          evSummary_.jn_svdr);
  
  t_->SetBranchAddress("htvec_px",&evSummary_.htvec_px);
  t_->SetBranchAddress("htvec_py",&evSummary_.htvec_py);
  t_->SetBranchAddress("htvec_pz",&evSummary_.htvec_pz);
  t_->SetBranchAddress("htvec_en",&evSummary_.htvec_en);

  t_->SetBranchAddress("ajn",             &evSummary_.ajn);
  t_->SetBranchAddress("ajn_px",          evSummary_.ajn_px);
  t_->SetBranchAddress("ajn_py",          evSummary_.ajn_py);
  t_->SetBranchAddress("ajn_pz",          evSummary_.ajn_pz);
  t_->SetBranchAddress("ajn_en",          evSummary_.ajn_en);
  t_->SetBranchAddress("ajn_rawsf",       evSummary_.ajn_rawsf);
  t_->SetBranchAddress("ajn_genid",       evSummary_.ajn_genid);
  t_->SetBranchAddress("ajn_genflav",     evSummary_.ajn_genflav);
  t_->SetBranchAddress("ajn_btag1",       evSummary_.ajn_btag1);
  t_->SetBranchAddress("ajn_btag2",       evSummary_.ajn_btag2);
  t_->SetBranchAddress("ajn_btag3",       evSummary_.ajn_btag3);
  t_->SetBranchAddress("ajn_btag4",       evSummary_.ajn_btag4);
  if(t_->GetBranch("ajn_btag5"))  t_->SetBranchAddress("ajn_btag5",       evSummary_.ajn_btag5);
  if(t_->GetBranch("ajn_btag6"))  t_->SetBranchAddress("ajn_btag6",       evSummary_.ajn_btag6);
  if(t_->GetBranch("ajn_btag7"))  t_->SetBranchAddress("ajn_btag7",       evSummary_.ajn_btag7);
  if(t_->GetBranch("ajn_btag8"))  t_->SetBranchAddress("ajn_btag8",       evSummary_.ajn_btag8);
  if(t_->GetBranch("ajn_btag9"))  t_->SetBranchAddress("ajn_btag9",       evSummary_.ajn_btag9);
  t_->SetBranchAddress("ajn_neutHadFrac", evSummary_.ajn_neutHadFrac);
  t_->SetBranchAddress("ajn_neutEmFrac",  evSummary_.ajn_neutEmFrac);
  t_->SetBranchAddress("ajn_chHadFrac",   evSummary_.ajn_chHadFrac);
  t_->SetBranchAddress("ajn_genpt",       evSummary_.ajn_genpt);
  t_->SetBranchAddress("ajn_pumva",       evSummary_.ajn_pumva);
  t_->SetBranchAddress("ajn_puminmva",    evSummary_.ajn_puminmva);  
  t_->SetBranchAddress("ajn_beta",        evSummary_.ajn_beta);
  t_->SetBranchAddress("ajn_betaStar",    evSummary_.ajn_betaStar);
  t_->SetBranchAddress("ajn_dRMean",      evSummary_.ajn_dRMean);
  t_->SetBranchAddress("ajn_ptD",         evSummary_.ajn_ptD);
  t_->SetBranchAddress("ajn_ptRMS",       evSummary_.ajn_ptRMS);
  t_->SetBranchAddress("ajn_etaW",        evSummary_.ajn_etaW);
  t_->SetBranchAddress("ajn_phiW",        evSummary_.ajn_phiW);
  t_->SetBranchAddress("ajn_idbits",      evSummary_.ajn_idbits);
  if(t_->GetBranch("ajn_isCfromGsplit")) t_->SetBranchAddress("ajn_isCfromGsplit", evSummary_.ajn_isCfromGsplit);
  if(t_->GetBranch("ajn_isBfromGsplit")) t_->SetBranchAddress("ajn_isBfromGsplit", evSummary_.ajn_isBfromGsplit);
  if(t_->GetBranch("ajn_lxy"))           t_->SetBranchAddress("ajn_lxy",           evSummary_.ajn_lxy);
  if(t_->GetBranch("ajn_lxyErr"))        t_->SetBranchAddress("ajn_lxyErr",        evSummary_.ajn_lxyErr);
  if(t_->GetBranch("ajn_svmass"))        t_->SetBranchAddress("ajn_svmass",        evSummary_.ajn_svmass);
  if(t_->GetBranch("ajn_svpt"))          t_->SetBranchAddress("ajn_svpt",          evSummary_.ajn_svpt);
  if(t_->GetBranch("ajn_svdr"))          t_->SetBranchAddress("ajn_svdr",          evSummary_.ajn_svdr);

  //primary vertex
  t_->SetBranchAddress("vtx_px", &evSummary_.vtx_px);
  t_->SetBranchAddress("vtx_py", &evSummary_.vtx_py);
  t_->SetBranchAddress("vtx_pz", &evSummary_.vtx_pz);
  t_->SetBranchAddress("vtx_en", &evSummary_.vtx_en);
  
  //MET
  t_->SetBranchAddress("nmet",     &evSummary_.nmet);
  t_->SetBranchAddress("met_phi",  evSummary_.met_phi);
  t_->SetBranchAddress("met_pt",   evSummary_.met_pt);

  t_->SetBranchAddress("sumEt",&evSummary_.sumEt);
  t_->SetBranchAddress("sumEtcentral",&evSummary_.sumEtcentral);
  t_->SetBranchAddress("chsumEt",&evSummary_.chsumEt);
  t_->SetBranchAddress("chsumEtcentral",&evSummary_.chsumEtcentral);
  t_->SetBranchAddress("neutsumEt",&evSummary_.neutsumEt);
  t_->SetBranchAddress("neutsumEtcentral",&evSummary_.neutsumEtcentral);
  t_->SetBranchAddress("primVertexSumEt",&evSummary_.primVertexSumEt);
  t_->SetBranchAddress("otherVertexSumEt",&evSummary_.otherVertexSumEt);
  t_->SetBranchAddress("primVertexChSumEt",&evSummary_.primVertexChSumEt);
  t_->SetBranchAddress("otherVertexChSumEt",&evSummary_.otherVertexChSumEt);
  t_->SetBranchAddress("primVertexNeutSumEt",&evSummary_.primVertexNeutSumEt);
  t_->SetBranchAddress("otherVertexNeutSumEt",&evSummary_.otherVertexNeutSumEt);
  
  //Gamma Info
  t_->SetBranchAddress("gn",          &evSummary_.gn);
  if(t_->GetBranch("gn_prescale")) t_->SetBranchAddress("gn_prescale",          evSummary_.gn_prescale);
  if(t_->GetBranch("gn_triggerWord")) t_->SetBranchAddress("gn_triggerWord",       &evSummary_.gn_triggerWord);
  t_->SetBranchAddress("g_px",        evSummary_.g_px);
  t_->SetBranchAddress("g_py",        evSummary_.g_py);
  t_->SetBranchAddress("g_pz",        evSummary_.g_pz);
  t_->SetBranchAddress("g_en",        evSummary_.g_en);
  t_->SetBranchAddress("g_iso1",      evSummary_.g_iso1);
  t_->SetBranchAddress("g_iso2",      evSummary_.g_iso2);
  t_->SetBranchAddress("g_iso3",      evSummary_.g_iso3);
  t_->SetBranchAddress("g_sihih",     evSummary_.g_sihih);
  t_->SetBranchAddress("g_sipip",     evSummary_.g_sipip);
  t_->SetBranchAddress("g_r9",        evSummary_.g_r9);
  t_->SetBranchAddress("g_hoe",       evSummary_.g_hoe);
  t_->SetBranchAddress("g_htoe",      evSummary_.g_htoe);
  t_->SetBranchAddress("g_corren",    evSummary_.g_corren);
  t_->SetBranchAddress("g_correnerr", evSummary_.g_correnerr);
  t_->SetBranchAddress("g_idbits",    evSummary_.g_idbits);
  
  //Higgs Info
  t_->SetBranchAddress("h_px",&evSummary_.h_px);
  t_->SetBranchAddress("h_py",&evSummary_.h_py);
  t_->SetBranchAddress("h_pz",&evSummary_.h_pz);
  t_->SetBranchAddress("h_en",&evSummary_.h_en);
  
  //mc truth
  t_->SetBranchAddress("nmcparticles", &evSummary_.nmcparticles);
  t_->SetBranchAddress("mc_px",        evSummary_.mc_px);
  t_->SetBranchAddress("mc_py",        evSummary_.mc_py);
  t_->SetBranchAddress("mc_pz",        evSummary_.mc_pz);
  t_->SetBranchAddress("mc_en",        evSummary_.mc_en);
  t_->SetBranchAddress("mc_id",        evSummary_.mc_id);
  if(t_->GetBranch("mc_nup"))    t_->SetBranchAddress("mc_nup", &evSummary_.mc_nup);
  if(t_->GetBranch("mc_lxy"))    t_->SetBranchAddress("mc_lxy", &evSummary_.mc_lxy);
  if(t_->GetBranch("mc_status")) t_->SetBranchAddress("mc_status", &evSummary_.mc_status);

  //additional measurements you may want to add  
  t_->SetBranchAddress("nmeasurements",&evSummary_.nmeasurements);
  t_->SetBranchAddress("measurements",evSummary_.measurements);
  
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
  //  LorentzVector invisible(evSummary_.met_pt[0]*cos(evSummary_.met_phi[0]),evSummary_.met_pt[0]*sin(evSummary_.met_phi[0]),0,evSummary_.met_pt[0]);
  LorentzVector invisible(evSummary_.met_pt[2]*cos(evSummary_.met_phi[2]),evSummary_.met_pt[2]*sin(evSummary_.met_phi[2]),0,evSummary_.met_pt[2]);
  LorentzVector sum=visible+invisible;
  Double_t tMass = TMath::Power(TMath::Sqrt(TMath::Power(visible.pt(),2)+pow(visible.mass(),2))+TMath::Sqrt(TMath::Power(invisible.pt(),2)+pow(visible.mass(),2)),2);
  tMass-=TMath::Power(sum.pt(),2);
  return (TMath::Sqrt(tMass)>250);
}

//
ZZ2l2nuSummaryHandler::~ZZ2l2nuSummaryHandler()
{
}
