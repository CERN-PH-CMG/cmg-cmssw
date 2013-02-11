
//#ifndef __CINT__
//#include "RooGlobalFunc.h"
//#endif
//#include "RooRealVar.h"
//#include "RooDataSet.h"
//#include "RooDataHist.h"
//#include "RooCBShape.h"
//#include "RooFitResult.h"
//#include "RooPlot.h"
//#include "RooBinning.h"
//#include "TCanvas.h"
//#include "TString.h"
//#include "TTree.h"
//#include "TH1.h"
//#include "TF1.h"
//#include "TFile.h"
//#include "TGraphErrors.h"

using namespace RooFit ;


const int NBINS=55;
Double_t BOUNDARIES[NBINS+1] = { 220, 244, 270, 296, 325, 354, 386, 419, 453,
				 489, 526, 565, 606, 649, 693, 740, 788, 838,
				 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383,
				 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132,
				 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147,
	                         3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869};

vector <double> Masses, Means, Sigmas, alphaHights, alphaLows, nHighs, nLows, fracs;


RooFitResult* FitHistWithCBShape(TH1* h, double mass)
{
  RooFitResult *res = 0;

  double avg = 0.97*mass;
  double rms = 0.05*mass;
  double xmin = 0.7*mass;
  // double xmax = 1.25*mass;
  // double xmin = 0.80*mass;
  double xmax = 1.25*mass;

  RooRealVar E("E","Normalised Resonance Mass", xmin, xmax, "GeV");
  RooDataHist dataSet("dataSet", "dataSet", E, h ); 

  RooPlot* frame = E.frame();
  dataSet.plotOn(frame);

  RooRealVar mean("mean","mean", avg, 0.92*mass, 1.2*mass) ;
  RooRealVar sigma("sigma","width", rms, 0.001*mass, 0.2*mass);
  RooRealVar alphaLow("alphaLow","alpha", 0.8, 0.0, 3.0);
  RooRealVar nLow("nLow","n", 20, 0.00001, 1000.0);
  RooRealVar alphaHigh("alphaHigh","alphaHigh", 2.0, 0.0, 3.0);
  RooRealVar nHigh("nHigh","nHigh", 100.0, 0.0000000001, 1000.0);

  RooFormulaVar minusE("minusE","Negate the data set","-1*E", E) ;
  RooFormulaVar minusMean("minusMean","Negate the mean","-1*mean", mean) ;


  RooCBShape CBallLow("CBallLow","Crystal Ball for lower tail", E, mean, sigma, alphaLow, nLow);
  RooCBShape CBallHigh("CBallHigh","Crystal Ball for highr tail", minusE, minusMean, sigma, alphaHigh, nHigh);  

  RooRealVar cb1frac("cb1frac","fraction of cb1", 0.5, 0, 1.0) ;

  RooAddPdf sum("sum","cb1+cb2",RooArgList(CBallLow, CBallHigh), cb1frac) ;

  res = sum.fitTo(dataSet, Save() );
  sum.plotOn(frame);
	
  frame->Draw();

  double sMean = ((RooAbsReal*) res->floatParsFinal().find("mean"))->getVal();
  double sSigma = ((RooAbsReal*) res->floatParsFinal().find("sigma"))->getVal();
  double sAlphaLow = ((RooAbsReal*) res->floatParsFinal().find("alphaLow"))->getVal();
  double sAlphaHigh = ((RooAbsReal*) res->floatParsFinal().find("alphaHigh"))->getVal();
  double sNLow = ((RooAbsReal*) res->floatParsFinal().find("nLow"))->getVal();
  double sNHigh = ((RooAbsReal*) res->floatParsFinal().find("nHigh"))->getVal();
  double sFrac = ((RooAbsReal*) res->floatParsFinal().find("cb1frac"))->getVal();

  Masses.push_back(mass); Means->push_back(sMean); Sigmas->push_back(sSigma);
  alphaHights->push_back(sAlphaHigh), alphaLows->push_back(sAlphaLow);
  nHighs->push_back(sNHigh), nLows->push_back(sNLow), fracs->push_back(sFrac);


  return res;

}


void FitterCBForSignificance(){



  //  TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_gg_TuneD6T_Emine2013.root");

  TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_qg_TuneD6T_Emine2013.root");



  //  TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_Qstar_2012_D6T_ak5_fat30_save.root");
  //  TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_RSGraviton_2012_D6T_ak5_GGtoGG_fat30_save.root");
  //TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_save.root");

  //TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_Qstar_ak5_fat30.root");
  for (int i = 1; i < 2; i++){
	   TH1F* shape = (TH1F*) _shapes->Get(Form("h_qg_%d;1", 1000+i*100));
	   TH1F* shape_largeBinning = new TH1F("shape_large_binning", "", NBINS, BOUNDARIES);

	   for (int j = 1; j < shape->GetNbinsX()+1; j++){
	     double binCenter = shape->GetBinCenter(j);
	     double weight = shape->GetBinContent(j);
	     double binWidth = shape->GetBinWidth(j);
	     shape_largeBinning->Fill(binCenter, weight);
	     //	     cout << "j = " << j << " binWidth = " << binWidth << " weight = " << weight << endl;
	     shape->SetBinContent(j, weight/binWidth);
	   }

	   for (int j = 1; j < shape_largeBinning->GetNbinsX()+1; j++){
	     double weight = shape_largeBinning->GetBinContent(j);
	     double binWidth = shape_largeBinning->GetBinWidth(j);
	     shape_largeBinning->SetBinContent(j, weight/binWidth*1000000);
	   }

	   shape->Draw();

	   shape_largeBinning->SetLineColor(kRed);
	   shape_largeBinning->Draw("SAME");

	   //   
	   FitHistWithCBShape(shape_largeBinning, 1000.+i*100);		
	}

	  cout << "mass\tsMean\tsSigma\tsAlphaHigh\tsAlphaLow\tsNHigh\tsNLow\tsFrac"<<endl;

	for (int i = 0; i < 40; i++){
	
	  cout << Masses[i] << "\t" << Means[i] << "\t" << Sigmas[i] << "\t"
	       << alphaHights[i] << "\t\t" << alphaLows[i] << "\t"
	       << nHighs[i] << "\t\t" << nLows[i] << "\t" << fracs[i] << endl;

	}
}
