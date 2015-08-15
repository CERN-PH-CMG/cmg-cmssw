#include "TFile.h"
#include "TLorentzVector.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TProfile2D.h"
#include "TRandom3.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
class KalmanCalibratorParam {
 public:
  KalmanCalibratorParam(bool isData = false);
  void getCorrectedPt(TLorentzVector &muon, int chrge);
  void smear(TLorentzVector &muon);
  double getCorrectedPtMag(double,double,double);
  double getCorrectedError(double pt,double eta,double error);
  int getN();
  void vary(int,int);
  void varyClosure(int);
  void reset();
  void randomize();


  ~KalmanCalibratorParam();



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
  TH3F *scale_L; 
  TH3F *scale_e;
  TH3F *scale_B0;
  TH3F *scale_B1;
  TH3F *scale_B2;
  TH3F *scale_C1;
  TH3F *scale_C2;

  TH3F *shifted_A1; 
  TH3F *shifted_A2; 
  TH3F *shifted_L; 
  TH3F *shifted_e;
  TH3F *shifted_B0;
  TH3F *shifted_B1;
  TH3F *shifted_B2;
  TH3F *shifted_C1;
  TH3F *shifted_C2;

  TH1F* a2_;
  TH1F* b2_;


  TH3F* smearing_;

  TH3F *closure_;
  TMatrixDSym *cholesky_;
  TMatrixD *eigenvectors_;
  TVectorD *eigenvalues_;

  TH1I *covHistoMap_;
  TH1I *covBinMap_;
};
