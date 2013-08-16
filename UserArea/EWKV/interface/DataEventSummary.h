#ifndef dataeventsummary_h
#define dataeventsummary_h

#include "TMath.h"
#include "Math/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"

//#include "TObject.h"

#define MAXDATAOBJECTS 1000

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

class DataEventSummary //: public TObject 
{
 public:

  Int_t run, lumi, event, cat;
  
  //filter information
  Bool_t f_bits;

  //trigger information
  Int_t tn;
  Bool_t t_bits[MAXDATAOBJECTS];
  Int_t t_prescale[MAXDATAOBJECTS];

  //pileup information
  Int_t nvtx;
  Float_t instLumi, rho,rho25;

  //PF candidates
  Int_t   pfn, pf_id[MAXDATAOBJECTS], pf_charge[MAXDATAOBJECTS];
  Float_t pf_px[MAXDATAOBJECTS], pf_py[MAXDATAOBJECTS], pf_pz[MAXDATAOBJECTS],pf_en[MAXDATAOBJECTS];

  //gen information
  Int_t ngenITpu, ngenOOTpu, ngenOOTpum1, ngenTruepu;
  Float_t pthat, genWeight, qscale, x1,x2;
  Int_t id1, id2, nup;

  //gen level event
  Int_t mcn, mc_id[MAXDATAOBJECTS], mc_status[MAXDATAOBJECTS];
  Float_t mc_px[MAXDATAOBJECTS],mc_py[MAXDATAOBJECTS],mc_pz[MAXDATAOBJECTS],mc_en[MAXDATAOBJECTS], mc_lxy[MAXDATAOBJECTS]; 

  //leptons
  Int_t ln;
  Int_t ln_id[MAXDATAOBJECTS],          ln_idbits[MAXDATAOBJECTS],    ln_pid[MAXDATAOBJECTS], ln_genid[MAXDATAOBJECTS];
  Float_t ln_px[MAXDATAOBJECTS],        ln_py[MAXDATAOBJECTS],        ln_pz[MAXDATAOBJECTS],        ln_en[MAXDATAOBJECTS];
  Float_t ln_genpx[MAXDATAOBJECTS],     ln_genpy[MAXDATAOBJECTS],     ln_genpz[MAXDATAOBJECTS],     ln_genen[MAXDATAOBJECTS];
  Float_t ln_ecalIso03[MAXDATAOBJECTS], ln_hcalIso03[MAXDATAOBJECTS], ln_trkIso03[MAXDATAOBJECTS];
  Float_t ln_gIso03[MAXDATAOBJECTS],    ln_chIso03[MAXDATAOBJECTS],   ln_puchIso03[MAXDATAOBJECTS], ln_nhIso03[MAXDATAOBJECTS]; 
  Float_t ln_ecalIso04[MAXDATAOBJECTS], ln_hcalIso04[MAXDATAOBJECTS], ln_trkIso04[MAXDATAOBJECTS];
  Float_t ln_gIso04[MAXDATAOBJECTS],    ln_chIso04[MAXDATAOBJECTS],   ln_puchIso04[MAXDATAOBJECTS], ln_nhIso04[MAXDATAOBJECTS]; 
  Float_t ln_d0[MAXDATAOBJECTS],        ln_dZ[MAXDATAOBJECTS],        ln_ip3d[MAXDATAOBJECTS],      ln_ip3dsig[MAXDATAOBJECTS];
  Float_t ln_trkpt[MAXDATAOBJECTS],     ln_trketa[MAXDATAOBJECTS],    ln_trkphi[MAXDATAOBJECTS],    ln_trkchi2[MAXDATAOBJECTS], ln_trkValidPixelHits[MAXDATAOBJECTS], ln_trkValidTrackerHits[MAXDATAOBJECTS], ln_trkLostInnerHits[MAXDATAOBJECTS], ln_trkPtErr[MAXDATAOBJECTS];

  //photons;
  Int_t gn, gn_idbits[MAXDATAOBJECTS], gn_pid[MAXDATAOBJECTS];
  Float_t gn_px[MAXDATAOBJECTS], gn_py[MAXDATAOBJECTS]  , gn_pz[MAXDATAOBJECTS]  , gn_en[MAXDATAOBJECTS];
  Float_t gn_ecalIso03[MAXDATAOBJECTS], gn_hcalIso03[MAXDATAOBJECTS], gn_trkIso03[MAXDATAOBJECTS];
  Float_t gn_gIso03[MAXDATAOBJECTS],    gn_chIso03[MAXDATAOBJECTS],   gn_puchIso03[MAXDATAOBJECTS], gn_nhIso03[MAXDATAOBJECTS];
  Float_t gn_ecalIso04[MAXDATAOBJECTS], gn_hcalIso04[MAXDATAOBJECTS], gn_trkIso04[MAXDATAOBJECTS];
  Float_t gn_gIso04[MAXDATAOBJECTS],    gn_chIso04[MAXDATAOBJECTS],   gn_puchIso04[MAXDATAOBJECTS], gn_nhIso04[MAXDATAOBJECTS];

  //e/gamma id variables
  Int_t   egn;
  Bool_t  egn_isConv[MAXDATAOBJECTS];
  Float_t egn_hoe[MAXDATAOBJECTS],egn_h2te[MAXDATAOBJECTS],egn_dphiin[MAXDATAOBJECTS],egn_detain[MAXDATAOBJECTS],egn_sihih[MAXDATAOBJECTS],egn_sipip[MAXDATAOBJECTS],egn_sihip[MAXDATAOBJECTS], egn_eopin[MAXDATAOBJECTS], egn_eopout[MAXDATAOBJECTS],egn_r9[MAXDATAOBJECTS],egn_fbrem[MAXDATAOBJECTS];
  Float_t egn_sce[MAXDATAOBJECTS],egn_sceta[MAXDATAOBJECTS],egn_scphi[MAXDATAOBJECTS], egn_ooemoop[MAXDATAOBJECTS];
  Float_t egn_mvatrigv0[MAXDATAOBJECTS], egn_mvanontrigv0[MAXDATAOBJECTS];

  //muon id variables
  Int_t   mn;
  Float_t mn_nMatches[MAXDATAOBJECTS], mn_nMatchedStations[MAXDATAOBJECTS], mn_validMuonHits[MAXDATAOBJECTS], mn_innerTrackChi2[MAXDATAOBJECTS], mn_trkLayersWithMeasurement[MAXDATAOBJECTS], mn_pixelLayersWithMeasurement[MAXDATAOBJECTS];

  //jets
  Int_t jn, jn_idbits[MAXDATAOBJECTS], jn_pfstart[MAXDATAOBJECTS], jn_pfend[MAXDATAOBJECTS];
  Float_t jn_px[MAXDATAOBJECTS],    jn_py[MAXDATAOBJECTS],      jn_pz[MAXDATAOBJECTS],          jn_en[MAXDATAOBJECTS], jn_torawsf[MAXDATAOBJECTS];
  Float_t jn_neutHadFrac[MAXDATAOBJECTS], jn_neutEmFrac[MAXDATAOBJECTS], jn_chHadFrac[MAXDATAOBJECTS], jn_area[MAXDATAOBJECTS];
  Float_t jn_tchp[MAXDATAOBJECTS], jn_jp[MAXDATAOBJECTS], jn_origcsv[MAXDATAOBJECTS], jn_csv[MAXDATAOBJECTS], jn_jpcsv[MAXDATAOBJECTS], jn_slcsv[MAXDATAOBJECTS], jn_supercsv[MAXDATAOBJECTS], jn_ssvhe[MAXDATAOBJECTS], jn_ivf[MAXDATAOBJECTS];
  Float_t jn_svxPx[MAXDATAOBJECTS], jn_svxPy[MAXDATAOBJECTS], jn_svxPz[MAXDATAOBJECTS], jn_svxM[MAXDATAOBJECTS], jn_svxNtrk[MAXDATAOBJECTS], jn_svxLxy[MAXDATAOBJECTS], jn_svxLxyErr[MAXDATAOBJECTS];
  Float_t jn_ivfPx[MAXDATAOBJECTS], jn_ivfPy[MAXDATAOBJECTS], jn_ivfPz[MAXDATAOBJECTS], jn_ivfM[MAXDATAOBJECTS], jn_ivfNtrk[MAXDATAOBJECTS], jn_ivfLxy[MAXDATAOBJECTS], jn_ivfLxyErr[MAXDATAOBJECTS];
  Float_t jn_puMVA[MAXDATAOBJECTS], jn_qgMVA[MAXDATAOBJECTS];
  Float_t jn_beta[MAXDATAOBJECTS],  jn_dRMean[MAXDATAOBJECTS], jn_ptRMS[MAXDATAOBJECTS], jn_etaW[MAXDATAOBJECTS], jn_phiW[MAXDATAOBJECTS];
  Int_t   jn_genflav[MAXDATAOBJECTS], jn_genid[MAXDATAOBJECTS];
  Float_t jn_genpx[MAXDATAOBJECTS], jn_genpy[MAXDATAOBJECTS], jn_genpz[MAXDATAOBJECTS], jn_genen[MAXDATAOBJECTS];
  Float_t jn_genjpx[MAXDATAOBJECTS], jn_genjpy[MAXDATAOBJECTS], jn_genjpz[MAXDATAOBJECTS], jn_genjen[MAXDATAOBJECTS];
  
  //met 
  Int_t metn;
  Float_t met_pt[MAXDATAOBJECTS], met_phi[MAXDATAOBJECTS],met_sig[MAXDATAOBJECTS],met_sigx2[MAXDATAOBJECTS],met_sigxy[MAXDATAOBJECTS],met_sigy2[MAXDATAOBJECTS];

  DataEventSummary() { } 
  ~DataEventSummary() { }

  //
  void reset()
  {
    run=0;    
    lumi=0;   
    event=0;  
    tn=0;
    pfn=0;
    mcn=0;
    ln=0; 
    egn=0;
    mn=0; 
    gn=0;
    jn=0;
    metn=0;
  }

  //  ClassDef(DataEventSummary,1)
};

#endif
