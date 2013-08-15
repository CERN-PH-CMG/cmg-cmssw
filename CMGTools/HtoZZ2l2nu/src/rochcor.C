#include "CMGTools/HtoZZ2l2nu/interface/rochcor.h"
#include <TLorentzVector.h>

using namespace rochcor;

RochCor::~RochCor(){
}

RochCor::RochCor(){
  
  eran.SetSeed(123456);
  sran.SetSeed(1234);

  gscler_mc_dev=0;
  gscler_da_dev=0;

  for(int i=0; i<8; ++i){
      for(int j=0; j<8; ++j){
          mptsys_mc_dm[i][j]=0;
          mptsys_mc_da[i][j]=0;
          mptsys_da_dm[i][j]=0;
          mptsys_da_da[i][j]=0;
      }
  }
}

RochCor::RochCor(int seed){
  eran.SetSeed(123456);
  sran.SetSeed(seed);

  gscler_mc_dev=sran.Gaus(0.0, 1.0);
  gscler_da_dev=sran.Gaus(0.0, 1.0);

  for(int i=0; i<8; ++i){
      for(int j=0; j<8; ++j){
          mptsys_mc_dm[i][j]=sran.Gaus(0.0, 1.0);
          mptsys_mc_da[i][j]=sran.Gaus(0.0, 1.0);
          mptsys_da_dm[i][j]=sran.Gaus(0.0, 1.0);
          mptsys_da_da[i][j]=sran.Gaus(0.0, 1.0);
      }
  }
}

void RochCor::momcor_mc( TLorentzVector& mu, float charge, float sysdev, int runopt, float& qter){
  
  //sysdev == num : deviation = num

  float ptmu = mu.Pt();
  float muphi = mu.Phi();
  float mueta = mu.Eta(); // same with mu.Eta() in Root

  float px = mu.Px();
  float py = mu.Py();
  float pz = mu.Pz();
  float e = mu.E();

  int mu_phibin = phibin(muphi);
  int mu_etabin = etabin(mueta);

  //float mptsys = sran.Gaus(0.0,sysdev);
  
  float dm = 0.0;
  float da = 0.0;
  
  if(runopt == 0){
    dm = (mcor_bfA[mu_phibin][mu_etabin] + mptsys_mc_dm[mu_phibin][mu_etabin]*mcor_bfAer[mu_phibin][mu_etabin])/mmavgA[mu_phibin][mu_etabin];
    da = mcor_maA[mu_phibin][mu_etabin] + mptsys_mc_da[mu_phibin][mu_etabin]*mcor_maAer[mu_phibin][mu_etabin];
  }else if(runopt == 1){
    dm = (mcor_bfB[mu_phibin][mu_etabin] + mptsys_mc_dm[mu_phibin][mu_etabin]*mcor_bfBer[mu_phibin][mu_etabin])/mmavgB[mu_phibin][mu_etabin];
    da = mcor_maB[mu_phibin][mu_etabin] + mptsys_mc_da[mu_phibin][mu_etabin]*mcor_maBer[mu_phibin][mu_etabin];
  }
  
  float cor = 1.0/(1.0 + dm + charge*da*ptmu);
  
  //for the momentum tuning - eta,phi,Q correction
  px *= cor;
  py *= cor;
  pz *= cor;
  e  *= cor;
  
  float recm = 0.0;
  float drecm = 0.0; 
  float delta = 0.0;
  float sf = 0.0;

  float gscler = 0.0;
  float deltaer = 0.0;
  float sfer = 0.0;
  
  if(runopt==0){
    recm = recmA;
    drecm = drecmA;
    
    delta = deltaA;
    sf = sfA;
    
    gscler = TMath::Sqrt( TMath::Power(mgsclA_stat,2) + TMath::Power(mgsclA_syst,2) );
    deltaer = TMath::Sqrt( TMath::Power(deltaA_stat,2) + TMath::Power(deltaA_syst,2) );
    sfer = TMath::Sqrt( TMath::Power(sfA_stat,2) + TMath::Power(sfA_syst,2) );
  }else if(runopt==1){
    recm = recmB;
    drecm = drecmB;
    
    delta = deltaB;
    sf = sfB;
    
    gscler = TMath::Sqrt( TMath::Power(mgsclB_stat,2) + TMath::Power(mgsclB_syst,2) );
    deltaer = TMath::Sqrt( TMath::Power(deltaB_stat,2) + TMath::Power(deltaB_syst,2) );
    sfer = TMath::Sqrt( TMath::Power(sfB_stat,2) + TMath::Power(sfB_syst,2) );
  }
  
  float gscl = (genm_smr/recm);
  
  px *= (gscl + gscler_mc_dev*gscler);
  py *= (gscl + gscler_mc_dev*gscler);
  pz *= (gscl + gscler_mc_dev*gscler);
  e  *= (gscl + gscler_mc_dev*gscler);

  float momscl = sqrt(px*px + py*py)/ptmu;

  float tune = 1.0/(1.0 + (delta + sysdev*deltaer)*sqrt(px*px + py*py)*eran.Gaus(1.0,(sf + sysdev*sfer)));
  
  px *= (tune); 
  py *= (tune);  
  pz *= (tune);  
  e  *= (tune);   
      
  qter *= sqrt(momscl*momscl + (1.0-tune)*(1.0-tune));
  
  mu.SetPxPyPzE(px,py,pz,e);
  
}


void RochCor::momcor_data( TLorentzVector& mu, float charge, float sysdev, int runopt, float& qter){
  
  float ptmu = mu.Pt();

  float muphi = mu.Phi();
  float mueta = mu.Eta(); // same with mu.Eta() in Root

  float px = mu.Px();
  float py = mu.Py();
  float pz = mu.Pz();
  float e = mu.E();
  
  int mu_phibin = phibin(muphi);
  int mu_etabin = etabin(mueta);
  
  //float mptsys1 = sran.Gaus(0.0,sysdev);
  
  float dm = 0.0;
  float da = 0.0;
  
  if(runopt==0){
    dm = (dcor_bfA[mu_phibin][mu_etabin] + mptsys_da_dm[mu_phibin][mu_etabin]*dcor_bfAer[mu_phibin][mu_etabin])/dmavgA[mu_phibin][mu_etabin];
    da = dcor_maA[mu_phibin][mu_etabin] + mptsys_da_da[mu_phibin][mu_etabin]*dcor_maAer[mu_phibin][mu_etabin];
  }else if(runopt==1){
    dm = (dcor_bfB[mu_phibin][mu_etabin] + mptsys_da_dm[mu_phibin][mu_etabin]*dcor_bfBer[mu_phibin][mu_etabin])/dmavgB[mu_phibin][mu_etabin];
    da = dcor_maB[mu_phibin][mu_etabin] + mptsys_da_da[mu_phibin][mu_etabin]*dcor_maBer[mu_phibin][mu_etabin];
  }
  
  float cor = 1.0/(1.0 + dm + charge*da*ptmu);
  
  px *= cor;
  py *= cor;
  pz *= cor;
  e  *= cor;
  
  //after Z pt correction
  float recm = 0.0;
  float gscler = 0.0;
  
  if(runopt==0){
    recm = drecmA;
    gscler = TMath::Sqrt( TMath::Power(dgsclA_stat,2) + TMath::Power(dgsclA_syst,2) );
  }else if(runopt==1){
    recm = drecmB;
    gscler = TMath::Sqrt( TMath::Power(dgsclB_stat,2) + TMath::Power(dgsclB_syst,2) );
  }
  
  float gscl = (genm_smr/recm);
  
  px *= (gscl + gscler_da_dev*gscler);
  py *= (gscl + gscler_da_dev*gscler);
  pz *= (gscl + gscler_da_dev*gscler);
  e  *= (gscl + gscler_da_dev*gscler);
  
  float momscl = sqrt(px*px + py*py)/ptmu;
  qter *= momscl;

  mu.SetPxPyPzE(px,py,pz,e);
  
}


Int_t RochCor::phibin(float phi){
  
  int nphibin = -1;
  
  for(int i=0; i<8; i++){
    if(-pi+(2.0*pi/8.0)*i <= phi && -pi+(2.0*pi/8.0)*(i+1) > phi){
      nphibin = i;
      break;
    }
  }
  
  return nphibin;
}

Int_t RochCor::etabin(float eta){

  int nbin = -1;
  
  for(int i=0; i<8; i++){
    if(netabin[i] <= eta && netabin[i+1] > eta){
      nbin = i;
      break;
    }
  }
  
  return nbin;
}

float RochCor::zptcor(float gzpt) {
  int ibin( 0 );
  
  // mcptscl[] = 84 bins: [0] and [83] are the underflow and overflow
  if ( gzpt > ptlow[nptbins] ) return nptbins-1;
  if ( gzpt < ptlow[0      ] ) return 0;
  
  for ( int i=0; i<nptbins; ++i ) {
    if ( gzpt>=ptlow[i] && gzpt<ptlow[i+1] ) { ibin=i; break; }
  }

  float zptwt = zptscl[ibin];

  return zptwt;
}
