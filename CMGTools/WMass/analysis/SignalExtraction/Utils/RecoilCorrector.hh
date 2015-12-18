//================================================================================================
//
// Class and tools for recoil corrections
//
//  * Defines RecoilCorrector class to access and apply MET corrections based on Z recoil
//
//________________________________________________________________________________________________

#include <TFile.h>
#include <TF1.h>
#include <TH1D.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TRandom.h>
#include <TString.h>
#include <TVector2.h>
#include <TFitResult.h>
#include <iostream>


//--------------------------------------------------------------------------------------------------
class RecoilCorrector
{
public:
  RecoilCorrector(TString fname, Int_t iSeed=0xDEADBEEF);
  ~RecoilCorrector();
  void Correct(Double_t &pfmet, Double_t &pfmetphi,  // reference to variables to store corrected MET and phi(MET)
               Double_t genWPt, Double_t genWPhi,    // GEN W boson pT and phi
	       Double_t lepPt,  Double_t lepPhi,     // lepton pT and phi
	       Int_t nsigma=0);

protected:
  TF1 *fcnPFu1mean, *fcnPFu1sigma1, *fcnPFu1sigma2, *fcnPFu1sigma0;
  TF1 *fcnPFu2mean, *fcnPFu2sigma1, *fcnPFu2sigma2, *fcnPFu2sigma0;
  TH1D *hCorrPFu1u2;//,    *hCorrPFu2u1;
  TFitResult *fitresPFu1mean, *fitresPFu1sigma1, *fitresPFu1sigma2,  *fitresPFu1sigma0;
  TFitResult *fitresPFu2mean, *fitresPFu2sigma1, *fitresPFu2sigma2,  *fitresPFu2sigma0; 
};

//--------------------------------------------------------------------------------------------------
Double_t sigmaFunc(Double_t *x, Double_t *par) {
  // par[0]: cubic coefficient
  // par[1]: linear coefficient
  // par[2]: constant term
  // par[3]: transition point
  
  Double_t a = par[0];
  Double_t c = par[1];
  Double_t d = par[2];
  Double_t x0 = par[3]; 
  
  if(x[0]<x0) {
    return a*x[0]*x[0]*x[0] - 3.0*a*x0*x[0]*x[0] + c*x[0] + d;
  } else {
    return (c - 3.0*a*x0*x0)*x[0] + (d + a*x0*x0*x0);
  }
}

//--------------------------------------------------------------------------------------------------
Double_t dMean(const TF1 *fcn, const Double_t x, const TFitResultPtr fs) {
  Double_t df[2];
  df[0] = 1;
  df[1] = x;
  Double_t err2 = df[0]*df[0]*(fs->GetCovarianceMatrix()[0][0]) 
                  + df[1]*df[1]*(fs->GetCovarianceMatrix()[1][1]) 
		  + 2.0*df[0]*df[1]*(fs->GetCovarianceMatrix()[0][1]);
  assert(err2>=0);
  return sqrt(err2);
}

//--------------------------------------------------------------------------------------------------
Double_t dSigma(const TF1 *fcn, const Double_t x, const TFitResultPtr fs) {
  Double_t df[4];
  Double_t a  = fcn->GetParameter(0);
  Double_t c  = fcn->GetParameter(1);
  Double_t d  = fcn->GetParameter(2);
  Double_t x0 = fcn->GetParameter(3); 
  if(x < x0) {
    df[0] = x*x*x - 3.0*x0*x*x;
    df[1] = x;
    df[2] = 1;
    df[3] = -3.0*a*x*x;
  
  } else {
    df[0] = -3.0*x0*x0*x + x0*x0*x0;
    df[1] = 1;
    df[2] = 1;
    df[3] = -6.0*a*x0*x + 3.0*a*x0*x0;
  }
  
  Double_t err2=0;
  for(Int_t i=0; i<4; i++) {
    err2 += df[i]*df[i]*(fs->GetCovarianceMatrix()[i][i]);
    for(Int_t j=i+1; j<4; j++) {
      err2 += 2.0*df[i]*df[j]*(fs->GetCovarianceMatrix()[i][j]);
    }
  }
  assert(err2>=0);
  return sqrt(err2);
}


//==================================================================================================

RecoilCorrector::RecoilCorrector(TString fname, Int_t seed)
{
  gRandom->SetSeed(seed);

  TFile infile(fname);

  fcnPFu1mean  = (TF1*)(infile.Get("fcnPFu1mean")->Clone("fcnPFu1mean"));
  fcnPFu2mean  = (TF1*)(infile.Get("fcnPFu2mean")->Clone("fcnPFu2mean"));
        
  TF1* tmp;
  Double_t xmin,xmax;

  //
  // Load PF-recoil functions
  //
  tmp = (TF1*)infile.Get("fcnPFu1sigma1");
  tmp->GetRange(xmin,xmax);
  fcnPFu1sigma1 = new TF1("fcnPFu1sigma1",sigmaFunc,xmin,xmax,tmp->GetNpar()); 
  for(int i=0; i<tmp->GetNpar(); i++) 
    fcnPFu1sigma1->SetParameter(i,tmp->GetParameter(i));
  
  tmp = (TF1*)infile.Get("fcnPFu1sigma2");
  tmp->GetRange(xmin,xmax);
  fcnPFu1sigma2 = new TF1("fcnPFu1sigma2",sigmaFunc,xmin,xmax,tmp->GetNpar()); 
  for(int i=0; i<tmp->GetNpar(); i++) 
    fcnPFu1sigma2->SetParameter(i,tmp->GetParameter(i));
  
  tmp = (TF1*)infile.Get("fcnPFu1sigma0");
  tmp->GetRange(xmin,xmax);
  fcnPFu1sigma0 = new TF1("fcnPFu1sigma0",sigmaFunc,xmin,xmax,tmp->GetNpar()); 
  for(int i=0; i<tmp->GetNpar(); i++) 
    fcnPFu1sigma0->SetParameter(i,tmp->GetParameter(i));
  
  tmp = (TF1*)infile.Get("fcnPFu2sigma1");
  tmp->GetRange(xmin,xmax);
  fcnPFu2sigma1 = new TF1("fcnPFu2sigma1",sigmaFunc,xmin,xmax,tmp->GetNpar()); 
  for(int i=0; i<tmp->GetNpar(); i++) 
    fcnPFu2sigma1->SetParameter(i,tmp->GetParameter(i));
  
  tmp = (TF1*)infile.Get("fcnPFu2sigma2");
  tmp->GetRange(xmin,xmax);
  fcnPFu2sigma2 = new TF1("fcnPFu2sigma2",sigmaFunc,xmin,xmax,tmp->GetNpar()); 
  for(int i=0; i<tmp->GetNpar(); i++) 
    fcnPFu2sigma2->SetParameter(i,tmp->GetParameter(i));
  
  tmp = (TF1*)infile.Get("fcnPFu2sigma0");
  tmp->GetRange(xmin,xmax);
  fcnPFu2sigma0 = new TF1("fcnPFu2sigma0",sigmaFunc,xmin,xmax,tmp->GetNpar()); 
  for(int i=0; i<tmp->GetNpar(); i++) 
    fcnPFu2sigma0->SetParameter(i,tmp->GetParameter(i));        

  //
  // Load correlation factor histograms
  //
  hCorrPFu1u2    = (TH1D*)(infile.Get("hCorrPFu1u2")->Clone("hCorrPFu1u2"));       hCorrPFu1u2->SetDirectory(0);
//  hCorrPFu2u1    = (TH1D*)(infile.Get("hCorrPFu2u1")->Clone("hCorrPFu2u1"));       hCorrPFu2u1->SetDirectory(0);

  fitresPFu1mean  = (TFitResult*)(infile.Get("fitresPFu1mean")->Clone("fitresPFu1mean"));
  fitresPFu2mean  = (TFitResult*)(infile.Get("fitresPFu2mean")->Clone("fitresPFu2mean"));

  fitresPFu1sigma0  = (TFitResult*)(infile.Get("fitresPFu1sigma0")->Clone("fitresPFu1sigma0"));
  fitresPFu2sigma0  = (TFitResult*)(infile.Get("fitresPFu2sigma0")->Clone("fitresPFu2sigma0"));
  
  fitresPFu1sigma1  = (TFitResult*)(infile.Get("fitresPFu1sigma1")->Clone("fitresPFu1sigma1"));
  fitresPFu2sigma1  = (TFitResult*)(infile.Get("fitresPFu2sigma1")->Clone("fitresPFu2sigma1"));
  
  fitresPFu1sigma2  = (TFitResult*)(infile.Get("fitresPFu1sigma2")->Clone("fitresPFu1sigma2"));
  fitresPFu2sigma2  = (TFitResult*)(infile.Get("fitresPFu2sigma2")->Clone("fitresPFu2sigma2"));
    
  infile.Close();
}

RecoilCorrector::~RecoilCorrector()
{
  delete fcnPFu1mean;
  delete fcnPFu1sigma1;
  delete fcnPFu1sigma2;
  delete fcnPFu1sigma0;
  delete fcnPFu2mean;
  delete fcnPFu2sigma1;
  delete fcnPFu2sigma2;
  delete fcnPFu2sigma0;

  delete hCorrPFu1u2;
//  delete hCorrPFu2u1;

  delete fitresPFu1mean;
  delete fitresPFu1sigma1;
  delete fitresPFu1sigma2;
  delete fitresPFu1sigma0;
  delete fitresPFu2mean;
  delete fitresPFu2sigma1;
  delete fitresPFu2sigma2;
  delete fitresPFu2sigma0;
}

void RecoilCorrector::Correct(Double_t &pfmet, Double_t &pfmetphi,
                              Double_t genWPt, Double_t genWPhi,
			      Double_t lepPt,  Double_t lepPhi,
			      Int_t nsigma
) {
  //
  // Model for PF u1
  //
  Double_t pfu1mean   = fcnPFu1mean->Eval(genWPt);
  Double_t pfu1sigma1 = fcnPFu1sigma1->Eval(genWPt);
  Double_t pfu1sigma2 = fcnPFu1sigma2->Eval(genWPt);
  Double_t pfu1sigma0 = fcnPFu1sigma0->Eval(genWPt);
  if(nsigma!=0) {
    //pfu1mean   += nsigma*dMean(fcnPFu1mean,genWPt,fitresPFu1mean);
    pfu1sigma1 += nsigma*dSigma(fcnPFu1sigma1,genWPt,fitresPFu1sigma1);
    pfu1sigma2 += nsigma*dSigma(fcnPFu1sigma2,genWPt,fitresPFu1sigma2);
    pfu1sigma0 += nsigma*dSigma(fcnPFu1sigma0,genWPt,fitresPFu1sigma0);
  }
  Double_t pfu1frac2  = (pfu1sigma0 - pfu1sigma1)/(pfu1sigma2 - pfu1sigma1);
  
  //
  // Model for PF u2
  //
  Double_t pfu2mean   = fcnPFu2mean->Eval(genWPt);
  Double_t pfu2sigma1 = fcnPFu2sigma1->Eval(genWPt);
  Double_t pfu2sigma2 = fcnPFu2sigma2->Eval(genWPt);
  Double_t pfu2sigma0 = fcnPFu2sigma0->Eval(genWPt);
  if(nsigma!=0) {
    //pfu2mean   += nsigma*dMean(fcnPFu2mean,genWPt,fitresPFu2mean);
    pfu2sigma1 += nsigma*dSigma(fcnPFu2sigma1,genWPt,fitresPFu2sigma1);
    pfu2sigma2 += nsigma*dSigma(fcnPFu2sigma2,genWPt,fitresPFu2sigma2);
    pfu2sigma0 += nsigma*dSigma(fcnPFu2sigma0,genWPt,fitresPFu2sigma0);
  }
  Double_t pfu2frac2  = (pfu2sigma0 - pfu2sigma1)/(pfu2sigma2 - pfu2sigma1);

  
  Double_t z1 = gRandom->Gaus(0,1);
  Double_t z2 = gRandom->Gaus(0,1);

  Double_t pfu1 = (gRandom->Uniform(0,1) < pfu1frac2) ? z1*pfu1sigma2+pfu1mean : z1*pfu1sigma1+pfu1mean;
  Double_t pfu2 = (gRandom->Uniform(0,1) < pfu2frac2) ? z2*pfu2sigma2+pfu2mean : z2*pfu2sigma1+pfu2mean;
 
  TVector2 vpfmet(-pfu1*cos(genWPhi)+pfu2*sin(genWPhi)-lepPt*cos(lepPhi), -pfu1*sin(genWPhi)-pfu2*cos(genWPhi)-lepPt*sin(lepPhi));  
  pfmet    = vpfmet.Mod();
  pfmetphi = TVector2::Phi_mpi_pi(vpfmet.Phi());
}

