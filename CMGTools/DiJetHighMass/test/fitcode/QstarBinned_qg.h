#ifndef _QSTARBINNED_QG_H
#define _QSTARBINNED_QG_H


#include "Math/Interpolator.h"
#include "shape_qg.h"

   
//Resonance Shape of any mass value using Interpolation technique 
//Author: Sertac Ozturk
// sertac@fnal.gov,  sertac.ozturk@cern.ch

void Init(string resonance){

  //  qstarMass.clear();
  if (resonance.find("Q*") !=string::npos ) {
    TArrayD* dv500 = new TArrayD(50, QstarToJJ_M_700_TuneD6T_ak5_fat);
    TArrayD* dv700 = new TArrayD(50, QstarToJJ_M_700_TuneD6T_ak5_fat);
    TArrayD* dv1200 = new TArrayD(50, QstarToJJ_M_1200_TuneD6T_ak5_fat);
    TArrayD* dv2000 = new TArrayD(50, QstarToJJ_M_2000_TuneD6T_ak5_fat);
    TArrayD* dv3500 = new TArrayD(50, QstarToJJ_M_3500_TuneD6T_ak5_fat);
    TArrayD* dv4000 = new TArrayD(50, QstarToJJ_M_4000_TuneD6T_ak5_fat);

    TArrayD* dv500_den = new TArrayD(50, QstarToJJ_M_700_TuneD6T_ak5_fat);
    TArrayD* dv700_den = new TArrayD(50, QstarToJJ_M_700_TuneD6T_ak5_fat);
    TArrayD* dv1200_den = new TArrayD(50, QstarToJJ_M_1200_TuneD6T_ak5_fat);
    TArrayD* dv2000_den = new TArrayD(50, QstarToJJ_M_2000_TuneD6T_ak5_fat);
    TArrayD* dv3500_den = new TArrayD(50, QstarToJJ_M_3500_TuneD6T_ak5_fat);
    TArrayD* dv4000_den = new TArrayD(50, QstarToJJ_M_4000_TuneD6T_ak5_fat);


  }  if (resonance.find("ZBBbar_pf") !=string::npos ) {

    TArrayD* dv500 = new TArrayD(50, ZBBar_500_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv700 = new TArrayD(50, ZBBar_700_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv1200 = new TArrayD(50, ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv2000 = new TArrayD(50, ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv3500 = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv4000 = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_pf);

    TArrayD* dv500_den = new TArrayD(50, ZBBar_500_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv700_den = new TArrayD(50, ZBBar_700_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv1200_den = new TArrayD(50, ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv2000_den = new TArrayD(50, ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv3500_den = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_pf);
    TArrayD* dv4000_den = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_pf);

  }  if (resonance.find("ZBBbar_fat") !=string::npos ) {

    TArrayD* dv500 = new TArrayD(50, ZBBar_500_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv700 = new TArrayD(50, ZBBar_700_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv1200 = new TArrayD(50, ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv2000 = new TArrayD(50, ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv3500 = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv4000 = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat);

    TArrayD* dv500_den = new TArrayD(50, ZBBar_500_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv700_den = new TArrayD(50, ZBBar_700_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv1200_den = new TArrayD(50, ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv2000_den = new TArrayD(50, ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv3500_den = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv4000_den = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat);

  }  if (resonance.find("ZBBbar_mu10_fat") !=string::npos ) {

    TArrayD* dv500 = new TArrayD(50, ZBBar_500_withMuons_lowMassCut_ak5_lowMass_mu10_fat);
    TArrayD* dv700 = new TArrayD(50, ZBBar_700_withMuons_lowMassCut_ak5_lowMass_mu10_fat);
    TArrayD* dv1200 = new TArrayD(50, ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_mu10_fat);
    TArrayD* dv2000 = new TArrayD(50, ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_mu10_fat);
    TArrayD* dv3500 = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_mu10_fat);
    TArrayD* dv4000 = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_mu10_fat);

    TArrayD* dv500_den = new TArrayD(50, ZBBar_500_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv700_den = new TArrayD(50, ZBBar_700_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv1200_den = new TArrayD(50, ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv2000_den = new TArrayD(50, ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv3500_den = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat);
    TArrayD* dv4000_den = new TArrayD(50, ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat);


  }  if (resonance.find("qW") !=string::npos ) {
    dv750 = new TArrayD(50,  Qstar_qW_75TeV_ak5_fat);
    dv1500 = new TArrayD(50,  Qstar_qW_15TeV_ak5_fat);
    dv3000 = new TArrayD(50,  Qstar_qW_3TeV_ak5_fat);

  }  if (resonance.find("qZ") !=string::npos ) {
    dv750 = new TArrayD(50,  Qstar_qZ_75TeV_ak5_fat);
    dv1500 = new TArrayD(50,  Qstar_qZ_15TeV_ak5_fat);
    dv3000 = new TArrayD(50,  Qstar_qZ_3TeV_ak5_fat);

  }  if (resonance.find("WW") !=string::npos ) {
    dv1000 = new TArrayD(50,  RSGraviton_WW_1TeV_ak5_fat);
    dv1500 = new TArrayD(50,  RSGraviton_WW_15TeV_ak5_fat);
    dv2000 = new TArrayD(50,  RSGraviton_WW_2TeV_ak5_fat);
    dv3000 = new TArrayD(50,  RSGraviton_WW_3TeV_ak5_fat);

  }  if (resonance.find("ZZ") !=string::npos ) {
    dv1000 = new TArrayD(50,  RSGraviton_WZ_1TeV_ak5_fat);
    dv1500 = new TArrayD(50,  RSGraviton_WZ_15TeV_ak5_fat);
    dv2000 = new TArrayD(50,  RSGraviton_WZ_2TeV_ak5_fat);
    dv3000 = new TArrayD(50,  RSGraviton_WZ_3TeV_ak5_fat);

  }  if (resonance.find("WZ") !=string::npos ) {
    dv1000 = new TArrayD(50,  RSGraviton_ZZ_1TeV_ak5_fat);
    dv1500 = new TArrayD(50,  RSGraviton_ZZ_15TeV_ak5_fat);
    dv2000 = new TArrayD(50,  RSGraviton_ZZ_2TeV_ak5_fat);
    dv3000 = new TArrayD(50,  RSGraviton_ZZ_3TeV_ak5_fat);
  }

  if ((resonance.find("qW") !=string::npos )||
      (resonance.find("qZ") !=string::npos )){
    vy->Add((TObject*) dv750); vy->Add((TObject*) dv1500); vy->Add((TObject*) dv3000);
    vy_den->Add((TObject*) dv750); vy_den->Add((TObject*) dv1500); vy_den->Add((TObject*) dv3000);
    vy->Add((TObject*) dv3000); vy->Add((TObject*) dv3000); vy->Add((TObject*) dv3000);
    vy_den->Add((TObject*) dv3000); vy_den->Add((TObject*) dv3000); vy_den->Add((TObject*) dv3000);
  } else
  if ((resonance.find("WW") !=string::npos )||
      (resonance.find("ZZ") !=string::npos )||
      (resonance.find("WZ") !=string::npos )){
    vy->Add((TObject*) dv1000); vy->Add((TObject*) dv1500); vy->Add((TObject*) dv2000); vy->Add((TObject*) dv3000);
    vy_den->Add((TObject*) dv1000); vy_den->Add((TObject*) dv1500); vy_den->Add((TObject*) dv2000); vy_den->Add((TObject*) dv3000);
    vy->Add((TObject*) dv3000); vy->Add((TObject*) dv3000);
    vy_den->Add((TObject*) dv3000); vy_den->Add((TObject*) dv3000);
  } else
  {
  vy->Add((TObject*) dv500); 
  vy->Add((TObject*) dv700); vy->Add((TObject*) dv1200); vy->Add((TObject*) dv2000); 
  vy->Add((TObject*) dv3500); vy->Add((TObject*) dv4000);

  vy_den->Add((TObject*) dv500_den);
  vy_den->Add((TObject*) dv700_den); vy_den->Add((TObject*) dv1200_den); vy_den->Add((TObject*) dv2000_den); 
  vy_den->Add((TObject*) dv3500_den);   vy_den->Add((TObject*) dv4000_den); 
  }
 

  for (int bin = 0; bin < vy.GetEntries(); bin++){
    double sum = 0;
    double sum_den = 0;
    for (int k = 0; k < 50; k++){
      sum += ((TArrayD*) vy->At(bin))->GetAt(k);
      sum_den += ((TArrayD*) vy_den->At(bin))->GetAt(k);
    }

    cout << "effic bin " << bin << " is " << sum/sum_den << endl;
    effic.push_back(sum/sum_den);

    for (int k = 0; k < 50; k++){
      double val = ((TArrayD*) vy->At(bin))->GetAt(k);
      ((TArrayD*) vy->At(bin))->AddAt(k, val/sum);
    }
  }	



  /*
    qstarMass.push_back(b); 
    qstarMass.push_back(a); 
    qstarMass.push_back(a); 
    qstarMass.push_back(a); 
    qstarMass.push_back(a); 
  */
  /*
    ZBBbarMass_pf.clear();
    ZBBbarMass_pf.push_back(ZBBar_500_withMuons_lowMassCut_ak5_lowMass_pf); 
    ZBBbarMass_pf.push_back(ZBBar_700_withMuons_lowMassCut_ak5_lowMass_pf); 
    ZBBbarMass_pf.push_back(ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_pf); 
    ZBBbarMass_pf.push_back(ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_pf); 
    ZBBbarMass_pf.push_back(ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_pf); 


    ZBBbarMass_fat.clear();
    ZBBbarMass_fat.push_back(ZBBar_500_withMuons_lowMassCut_ak5_lowMass_fat); 
    ZBBbarMass_fat.push_back(ZBBar_700_withMuons_lowMassCut_ak5_lowMass_fat); 
    ZBBbarMass_fat.push_back(ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_fat); 
    ZBBbarMass_fat.push_back(ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_fat); 
    ZBBbarMass_fat.push_back(ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_fat); 


    ZBBbarMass_mu10_fat.clear();
    ZBBbarMass_mu10_fat.push_back(ZBBar_500_withMuons_lowMassCut_ak5_lowMass_mu10_fat); 
    ZBBbarMass_mu10_fat.push_back(ZBBar_700_withMuons_lowMassCut_ak5_lowMass_mu10_fat); 
    ZBBbarMass_mu10_fat.push_back(ZBBar_1200_withMuons_lowMassCut_ak5_lowMass_mu10_fat); 
    ZBBbarMass_mu10_fat.push_back(ZBBar_2000_withMuons_lowMassCut_ak5_lowMass_mu10_fat); 
    ZBBbarMass_mu10_fat.push_back(ZBBar_3500_withMuons_lowMassCut_ak5_lowMass_mu10_fat); 
  */
  /*
    if (resonance.find("Q*") !=string::npos ) vy = qstarMass;
    else if (resonance.find("ZBBbar_pf") !=string::npos ) vy = ZBBbarMass_pf;
    else if (resonance.find("ZBBbar_fat") !=string::npos ) vy = ZBBbarMass_fat;
    else if (resonance.find("ZBBbar_mu10_fat") !=string::npos ) vy = ZBBbarMass_mu10_fat;
    else 
  */
    
}



double QstarBinnedProb(double mjj, double mass){  

  double effic_analyse = 0;

  if(mjj<0.3*mass || mjj>1.3*mass) return 0;
  // Create a vector and fill it with variable x bins according to entered mass value
  for(int i=0;i<103; i++){
    massnew[i] = massBoundaries[i]/mass;
    if(massnew[i]<=1.32 && massnew[i]>=0.28){ // x distribution is from 0.3 to 1.3
      v.push_back(massnew[i]);
    }
  }
  
  const unsigned int s = v.size();
  
  // Definition of arrays with variable bins 
  double bins[s]; // variable bins for x distribution
  double m[s]; // variable mass bins for dijet mass distribution
  double dndm[s];
  double prob[s];


  // Fill the bin values to array
  for(unsigned int j=0; j<v.size(); j++){
    bins[j] = v[j];
    m[j] = v[j] * mass;
  }
  
 
  // Definition of histograms
  TH1F* h_new2 = new TH1F("h_new2","with variale bins2",s-1 , m); // Histogram for dijet mass distribution with variable mass bins

  if(fabs(mass-mqstar[0]) < 1e-5){ // If mass = 0.5 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] = ((TArrayD*) vy->At(0))->GetAt(k);
      effic_analyse = effic[0];
    }
  }

  if(fabs(mass-mqstar[1]) < 1e-5){ // If mass = 0.7 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] =  ((TArrayD*) vy->At(1))->GetAt(k);
      effic_analyse = effic[1];
    }
  }

  if(fabs(mass-mqstar[2]) < 1e-5){ // If mass = 1.2 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] =  ((TArrayD*) vy->At(2))->GetAt(k);
      effic_analyse = effic[2];
    }
  }

  if(fabs(mass-mqstar[3]) < 1e-5){ // If mass = 2 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] =  ((TArrayD*) vy->At(3))->GetAt(k);
      effic_analyse = effic[3];
    }
  }

  if(fabs(mass-mqstar[4]) < 1e-5){ // If mass = 3.5 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] =  ((TArrayD*) vy->At(4))->GetAt(k);
      effic_analyse = effic[4];
    }
  }
  
  if(fabs(mass-mqstar[5]) < 1e-5){ // If mass = 3.5 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] =  ((TArrayD*) vy->At(5))->GetAt(k);
      effic_analyse = effic[5];
    }
  }

  // Calculate and create new x distribution with fixed bins at entered mass value which is between 0.5 TeV and 0.7 TeV
  if((mass > mqstar[0] && mass < mqstar[1]) || mass < mqstar[0]){
    double Mjj1 = mqstar[0];
    double Mjj2 = mqstar[1];
    double Mq = mass;
    double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
 
    for(int k=0; k<n; k++){
      double bincont1 = ( (TArrayD*) vy->At(0))->GetAt(k);
      double bincont2 = ( (TArrayD*) vy->At(1))->GetAt(k);
      double eff = bincont2 - bincont1;
      double neff = eff * norm;
      double mqstar1 = bincont1 + neff;
      y[k] = mqstar1;
      x[k] = bincenter[k];}
    effic_analyse = effic[0] +  (effic[1]- effic[0])*norm;
  }

  // Calculate and create new x distribution with fixed bins at entered mass value which is between 0.7 TeV and 1.2 TeV. 
  if(mass > mqstar[1] && mass < mqstar[2]){
      double Mjj1 = mqstar[1];
      double Mjj2 = mqstar[2];
      double Mq = mass;
      double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);

    for(int k=0; k<n; k++){
      double bincont1 = ( (TArrayD*) vy->At(1))->GetAt(k);
      double bincont2 = ( (TArrayD*) vy->At(2))->GetAt(k);

      double eff = bincont2 - bincont1;
      double neff = eff * norm;
      double mqstar1 = bincont1 + neff;
      y[k] = mqstar1;
      x[k] = bincenter[k];}
    effic_analyse = effic[1] +  (effic[2]- effic[1])*norm;
  }
  
  // Calculate and create new x distribution with fixed bins at entered mass value which is between 1.2 TeV and 2 TeV. 
  if(mass > mqstar[2] && mass < mqstar[3]){

      double Mjj1 = mqstar[2];
      double Mjj2 = mqstar[3];
      double Mq = mass;
      double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);

    for(int k=0; k<n; k++){
      double bincont1 = ( (TArrayD*) vy->At(2))->GetAt(k);
      double bincont2 = ( (TArrayD*) vy->At(3))->GetAt(k);

      double eff = bincont2 - bincont1;
      double neff = eff * norm;
      double mqstar1 = bincont1 + neff;
      y[k] = mqstar1;
      x[k] = bincenter[k];}
    effic_analyse = effic[2] +  (effic[3]- effic[2])*norm;
  }

  // Calculate and create new x distribution with fixed bins at entered mass value which is between 2 TeV and 3.5 TeV. 
  if((mass > mqstar[3] && mass < mqstar[4])){
       double Mjj1 = mqstar[3];
      double Mjj2 = mqstar[4];
      double Mq = mass;
      double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);

   for(int k=0; k<n; k++){
      double bincont1 = ( (TArrayD*) vy->At(3))->GetAt(k);
      double bincont2 = ( (TArrayD*) vy->At(4))->GetAt(k);

      double eff = bincont2 - bincont1;
      double neff = eff * norm;
      double mqstar1 = bincont1 + neff;
      y[k] = mqstar1;
      x[k] = bincenter[k];}
    effic_analyse = effic[3] +  (effic[4]- effic[3])*norm;
  }

  // Calculate and create new x distribution with fixed bins at entered mass value which is between 3.5 TeV and 4 TeV. 
  if((mass > mqstar[4] && mass < mqstar[5]) || mass > mqstar[5])){
       double Mjj1 = mqstar[4];
      double Mjj2 = mqstar[5];
      double Mq = mass;
      double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);

   for(int k=0; k<n; k++){
      double bincont1 = ( (TArrayD*) vy->At(4))->GetAt(k);
      double bincont2 = ( (TArrayD*) vy->At(5))->GetAt(k);

      double eff = bincont2 - bincont1;
      double neff = eff * norm;
      double mqstar1 = bincont1 + neff;
      y[k] = mqstar1;
      x[k] = bincenter[k];}
    effic_analyse = effic[4] +  (effic[5]- effic[4])*norm;
  }


  // Interpolation tecnique is used to convert distribution in x to distribution in dijet mass with variable bins
  ROOT::Math::Interpolator inter(n, ROOT::Math::Interpolation::kCSPLINE); // If CSPLINE is not valid, please use kCSPLINE
  inter.SetData(n, x, y);
  
  // Generate the new distribution in x with variable bins
  for(int t=0; t<n; t++){
    f[t] = inter.Eval(x[t]);} 
  
  //  int bin_number = h_new->GetNbinsX();
  for(int z=0; z<s; z++){
    double bin = inter.Integ(v[z], v[z+1]); 
    dndm[z] = bin;
    h_new2->SetBinContent(z+1,bin); // histogram in dijet mass distribution with variable bins
  }

  v.clear();

  double integ = h_new2->Integral();
  cout << "Integral = " << integ << " effic analyse " <<  effic_analyse << endl;

  h_new2->Delete();

  for(int p=0; p<s; p++){
    double nor = dndm[p] / integ * effic_analyse;
    prob[p] = nor;
    if(m[p]<mjj && m[p+1]>=mjj){
      return prob[p];
    }
  }
};

#endif

