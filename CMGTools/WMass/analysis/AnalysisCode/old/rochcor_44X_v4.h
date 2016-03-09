#include <iostream>
#include <TChain.h>
#include <TClonesArray.h>
#include <TString.h>
#include <map>

#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>


class rochcor_44X_v4 {
 public:
  rochcor_44X_v4();
  rochcor_44X_v4(int seed);
  ~rochcor_44X_v4();
  
  void momcor_mc(TLorentzVector&, float, float);
  void momcor_data(TLorentzVector&, float, float, int);
  
  void musclefit_data(TLorentzVector& , TLorentzVector&);
  
  float zptcor(float);
  int etabin(float);
  int letabin(float);
  int phibin(float);
  
 private:
  
  TRandom3 eran;
  TRandom3 sran;
  bool using_toys;
  
  //  static float netabin[9] = {-2.4,-2.1,-1.4,-0.7,0.0,0.7,1.4,2.1,2.4};
  static const double pi = 3.14159265358979323846;
  static const float netabin[23];
  static const float nletabin[9];
  
  static const float genm_smr = 9.09956e+01; //gen mass peak with eta dependent gaussian smearing => better match in Z mass profile vs. eta/phi
  static const float genm = 91.06; //gen mass peak without smearing => Z mass profile vs. eta/phi in CMS note
  
  static const float drecmA = 9.10218e+01; //rec mass peak in data (2011A)
  static const float dgsclA_stat = 0.0001; //stat. error of global factor for mass peak in data (2011A)
  static const float dgsclA_syst = 0.0008; //syst. error of global factor for mass peak in data (2011A)
  static const float drecmB = 9.09469e+01; //rec mass peak in data (2011B)
  static const float dgsclB_stat = 0.0001; //stat. error of global factor for mass peak in data (2011B)
  static const float dgsclB_syst = 0.0008; //syst. error of global factor for mass peak in data (2011B)
  
  /*
  //iteration2 after FSR : after Z Pt correction
  static const float deltaA = -2.85242e-06;
  static const float deltaA_stat = 7.74389e-07;
  static const float deltaA_syst = 6.992e-07;
  
  static const float sfA = 44.6463;
  static const float sfA_stat = 1.92224;
  static const float sfA_syst = 9.29;
  
  static const float deltaB = -5.68463e-06;
  static const float deltaB_stat = 8.21406e-07;
  static const float deltaB_syst = 1.4268e-06;
  
  static const float sfB = 23.8652;
  static const float sfB_stat = 0.941748;
  static const float sfB_syst = 4.86;
  */
  
  static const float apar = 1.0; //+- 0.002
  static const float bpar = -5.03313e-06; //+- 1.57968e-06
  static const float cpar = -4.41463e-05; //+- 1.92775e-06
  static const float d0par = -0.000148871; //+- 3.16301e-06
  static const float e0par = 1.59501; //+- 0.0249021
  static const float d1par = 7.95495e-05; //+- 1.12386e-05
  static const float e1par = -0.364823; //+- 0.17896
  static const float d2par = 0.000152032; //+- 5.68386e-06
  static const float e2par = 0.410195; //+- 0.0431732
 
  //---------------------------------------------------------------------------------------------
  
  static const float dcor_bfA[8][22];  
  static const float dcor_maA[8][22];
  static const float mcor_bfA[8][22];
  static const float mcor_maA[8][22];
  static const float dcor_bfAer[8][22];  
  static const float dcor_maAer[8][22];
  static const float mcor_bfAer[8][22];
  static const float mcor_maAer[8][22];

  static const float dcor_bfB[8][22];  
  static const float dcor_maB[8][22];
  static const float dcor_bfBer[8][22];  
  static const float dcor_maBer[8][22];

  static const float sfm[8];
  static const float sfp[8];
  static const float sfmer[8];
  static const float sfper[8];

  //=======================================================================================================
  
  static const float dmavgA[8][22];  
  static const float dpavgA[8][22];  
  static const float mmavgA[8][22];  
  static const float mpavgA[8][22];

  static const float dmavgB[8][22];  
  static const float dpavgB[8][22];  
  static const float mmavgB[8][22];  
  static const float mpavgB[8][22];

  //===============================================================================================
  //parameters for Z pt correction
  static const int nptbins=84;
  static const float ptlow[85];    
  
  static const float zptscl[84];
  static const float zptscler[84];

  float mptsys_mc_dm[8][22];
  float mptsys_mc_da[8][22];
  float mptsys_da_dm[8][22];
  float mptsys_da_da[8][22];


};
  
