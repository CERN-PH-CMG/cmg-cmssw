#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TProfile2D.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"

class KalmanCalibrator {
 public:
  KalmanCalibrator(bool isData = false);
  // double getCorrectedPt(double,double,double,int);
  void getCorrectedPt(TLorentzVector &muon, int chrge);
  double smearGEN(double,double);
  // double smear(double pt,double eta,bool reverse=false);
  void smear(TLorentzVector &muon,bool reverse=false);
  double getCorrectedPtMag(double,double,double);
  double getCorrectedError(double pt,double eta,double error);
  int getN();
  void vary(int,int);
  void varyClosure(int);
  void reset();
  void randomize();

  ~KalmanCalibrator();

 private:
  double closure(double,double);
  TRandom * random_;
  void resetHisto(TH1*,const TH1* );
  int varyClosure_;

  bool isData_;
  TFile *file_;
  TH2F *magnetic; 
  TH3F *scale_A1; 
  TH3F *scale_A2; 
  TH3F *scale_e;
  TH3F *scale_B;

  TH3F *shifted_A1; 
  TH3F *shifted_A2; 
  TH3F *shifted_e;
  TH3F *shifted_B;


  TH3F *sigma_A_target;
  TH3F *sigma_B_target;
  TH3F *sigma_C_target;

  TH3F *sigma_A_ref;
  TH3F *sigma_B_ref;
  TH3F *sigma_C_ref;

  TH3F *ebe_A;
  TH3F *ebe_B;
  TH3F *ebe_C;

  TH3F *closure_;
  TMatrixDSym *cholesky_;
  TMatrixD *eigenvectors_;
  TVectorD *eigenvalues_;

  TH1I *covHistoMap_;
  TH1I *covBinMap_;

};
