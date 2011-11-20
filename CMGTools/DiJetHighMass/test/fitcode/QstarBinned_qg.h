#ifndef _QSTARBINNED_QG_H
#define _QSTARBINNED_QG_H


#include "Math/Interpolator.h"
#include "shape_qg.h"
   
//Resonance Shape of any mass value using Interpolation technique 
//Author: Sertac Ozturk
// sertac@fnal.gov,  sertac.ozturk@cern.ch


    double QstarBinnedProb(double mjj, double mass){  

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
  for(int j=0; j<v.size(); j++){
    bins[j] = v[j];
    m[j] = v[j] * mass;
  }
  
 
  // Definition of histograms
  TH1F* h_new2 = new TH1F("h_new2","with variale bins2",s-1 , m); // Histogram for dijet mass distribution with variable mass bins

  if(mass == mqstar[0]){ // If mass = 0.5 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] = y500[k];
    }
  }

  if(mass == mqstar[1]){ // If mass = 0.7 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] = y700[k];}
  }

 if(mass == mqstar[2]){ // If mass = 1.2 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] = y1200[k];}
 }

 if(mass == mqstar[3]){ // If mass = 2 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] = y2000[k];}
 }

  if(mass == mqstar[4]){ // If mass = 3.5 TeV
    for(int k=0; k<n; k++){
      x[k] = bincenter[k];
      y[k] = y3500[k];}
 }


  // Calculate and create new x distribution with fixed bins at entered mass value which is between 0.5 TeV and 0.7 TeV
  if((mass > mqstar[0] && mass < mqstar[1]) || mass < mqstar[0]){ 
	for(int k=0; k<n; k++){
        double bincont1 = y500[k];
        double bincont2 = y700[k];
        double Mjj1 = mqstar[0];
        double Mjj2 = mqstar[1];
        double Mq = mass;
        double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
        double eff = bincont2 - bincont1;
        double neff = eff * norm;
        double mqstar1 = bincont1 + neff;
        y[k] = mqstar1;
        x[k] = bincenter[k];}
   }

 // Calculate and create new x distribution with fixed bins at entered mass value which is between 0.7 TeV and 1.2 TeV. 
  if(mass > mqstar[1] && mass < mqstar[2]){
	for(int k=0; k<n; k++){
        double bincont1 = y700[k];
        double bincont2 = y1200[k];
        double Mjj1 = mqstar[1];
        double Mjj2 = mqstar[2];
        double Mq = mass;
        double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
        double eff = bincont2 - bincont1;
        double neff = eff * norm;
        double mqstar1 = bincont1 + neff;
        y[k] = mqstar1;
        x[k] = bincenter[k];}
   }
  
  // Calculate and create new x distribution with fixed bins at entered mass value which is between 1.2 TeV and 2 TeV. 
  if(mass > mqstar[2] && mass < mqstar[3]){
        for(int k=0; k<n; k++){
        double bincont1 = y1200[k];
        double bincont2 = y2000[k];
        double Mjj1 = mqstar[2];
        double Mjj2 = mqstar[3];
        double Mq = mass;
        double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
        double eff = bincont2 - bincont1;
        double neff = eff * norm;
        double mqstar1 = bincont1 + neff;
        y[k] = mqstar1;
        x[k] = bincenter[k];}
   }

   // Calculate and create new x distribution with fixed bins at entered mass value which is between 2 TeV and 3.5 TeV. 
  if((mass > mqstar[3] && mass < mqstar[4]) || mass > mqstar[4]){
        for(int k=0; k<n; k++){
        double bincont1 = y2000[k];
        double bincont2 = y3500[k];
        double Mjj1 = mqstar[3];
        double Mjj2 = mqstar[4];
        double Mq = mass;
        double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
        double eff = bincont2 - bincont1;
        double neff = eff * norm;
        double mqstar1 = bincont1 + neff;
        y[k] = mqstar1;
        x[k] = bincenter[k];}
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
  h_new2->Delete();

  for(int p=0; p<s; p++){
    double nor = dndm[p] / integ;
    prob[p] = nor;
    if(m[p]<mjj && m[p+1]>=mjj){
     return prob[p];
      }
   }
};

#endif

