#ifndef Mela_h
#define Mela_h

/** \class Mela
 *
 *  MELA - cf. http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/sbologne/MELAproject/
 *
 *  This class is adapted from:
 *  UserCode/scasasso/HZZ4lAnalysis/HZZ4lCommon/plugins/LDProducer.h tag V00-00-00
 *
 *  $Date: 2012/08/19 20:46:50 $
 *  $Revision: 1.4 $
 *  \author S. Bolognesi - JHU
 */

//#include <memory>
//#include "CommonTools/UtilAlgos/interface/TFileService.h"

// #include "RooDataSet.h"
// #include "RooRealVar.h"
// #include "RooDataSet.h"
// #include "RooDataHist.h"
// #include "RooHistPdf.h"
// #include "RooGaussian.h"

// #include "TCanvas.h"
// #include "TString.h"
// #include "TAxis.h"
// #include "RooPlot.h"
//#include "TLegend.h"
//#include "TGraphErrors.h"
//#include "TChain.h"
//#include "TF1.h"
// #include "TLorentzVector.h"
// #include <sstream>

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include <vector>
#include <string>

class Mela { 
public:
  Mela();

  ~Mela();




void computeLD(double mzz, double m1, double m2, 
	       double costhetastar,
	       double costheta1, 
	       double costheta2,
	       double phi,
	       double phistar1,
	       double& ld, 
	       double& psig,
	       double& pbkg);
 
void calculateAngles(TLorentzVector thep4H, TLorentzVector thep4Z1, TLorentzVector thep4Lep11, TLorentzVector thep4Lep12, TLorentzVector thep4Z2, TLorentzVector thep4Lep21, TLorentzVector thep4Lep22, double& costheta1, double& costheta2, double& phi, double& costhetastar, double& phistar1, double& phistar2, double& phistar12, double& phi1, double& phi2);

private:	
  std::vector<double> my8DTemplate(bool normalized,double mZZ, double m1, double m2, double costhetastar, double costheta1, double costheta2, double phi, double phistar1);

  std::pair<double,double> likelihoodDiscriminant (double mZZ, double m1, double m2, double costhetastar, double costheta1, double costheta2, double phi, double phistar1,  double scaleFactor=5.0);

  void checkZorder(double& z1mass, double& z2mass, double& costhetastar, double& costheta1, double& costheta2, double& phi, double& phistar1);
  TFile *f;
  TH1F *h_mzz;
  TH3F *h_mzzm1m2;
  TH2F *h_mzzcosthetastar;
  TH2F *h_mzzcostheta1;
  TH2F *h_mzzcostheta2;
  TH2F *h_mzzphi1;
  TH2F *h_mzzphi;




  ClassDef(Mela, 1);

 
};



#endif
