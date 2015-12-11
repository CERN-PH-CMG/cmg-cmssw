#include <iostream>

#include "TFile.h"
#include <RooFitResult.h>
#include <TMatrixDSymEigen.h>


using namespace std;

const string filepath = "~/WMass/analysis/RecoilCode/NOV30/recoilfit_NOV30_\
genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root";
//DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root";
//genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root";

int matrix_extractor()
{
  cout << filepath << endl;
  TFile* f = new TFile(filepath.c_str());
  
  for (int unum = 1; unum <= 2; ++unum) {
    for (int Ynum = 1; Ynum <=2; ++Ynum) {
      RooFitResult* res = (RooFitResult*) f->Get(Form("fitresult_AddU%dY%d_Crapsky0_U%d_2D", unum, Ynum, unum));
      TMatrixDSymEigen eigen(res->covarianceMatrix());
      const TMatrixD eigvecs = eigen.GetEigenVectors();
      
      cout << "U" << unum << " - Y" << Ynum << endl << endl;
      
      for (int i = eigvecs.GetRowLwb(); i <= eigvecs.GetRowUpb(); ++i) {
        for (int j = eigvecs.GetColLwb(); j <= eigvecs.GetColUpb(); ++j) {
          cout << eigvecs[i][j] << "\t";
        }
        cout << endl;
      }
      cout << endl << endl;
    }
  }
  return 0;
}

int main()
{
  return matrix_extractor();
}
