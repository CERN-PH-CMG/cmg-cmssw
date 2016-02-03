#include <iostream>
// #include <fstream>
// #include <cmath>
// #include <ctime>

#include "TH1.h"
// #include "TH2.h"
// #include "TH3.h"
// #include "TCanvas.h"
// #include "TRandom3.h"
#include "TFile.h"
// #include "TGraph.h"
#include "TString.h"

using namespace std;

int macro()
{
	TString folderpath = "JobOutputs/jackknife_muPos_tkmet_ewk0_polariz1_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr/";
	TString templfolder = "output_DYJetsPow/";
	TString infolder    = "output_DYJetsPow_jackknife/";
	TString outfolder   = "output_jackknife_rescaled/";
	
	const int fitvars = 4;
	TString fitvar_str[fitvars] = {"Pt", "Mt", "MET", "MtLin"};
	
	TFile* ftempl = new TFile(folderpath + templfolder + "ZanalysisOnDATA.root");
	double int_templ[fitvars];
	
	for (int fitvar = 0; fitvar < fitvars; ++fitvar) {
		TH1D* htempl = (TH1D*)ftempl->Get(Form("hWlikePos_%sNonScaled_8_JetCut_pdf229800-0_eta0p9_91188", fitvar_str[fitvar].Data()));
		int_templ[fitvar] = htempl->Integral(); 
	}
	ftempl->Close();
	
	const int jkhistos = 2092;
	for (int jkhisto = 1; jkhisto <= jkhistos; ++jkhisto) {
		TFile* fin  = new TFile(folderpath +  infolder + Form("Zanalysis_%d.root", jkhisto),"OPEN"    );
		TFile* fout = new TFile(folderpath + outfolder + Form("Zanalysis_%d.root", jkhisto),"RECREATE");
		fout->cd();
		cout << "Num:\t" << jkhisto << endl;
		for (int fitvar = 0; fitvar < fitvars; ++fitvar) {
			TH1D* hjk = (TH1D*)fin->Get(Form("hWlikePos_%sNonScaled_8_JetCut_pdf229800-0_eta0p9_91188", fitvar_str[fitvar].Data()));
			double scale_factor = int_templ[fitvar]/hjk->Integral();
			cout << fitvar_str[fitvar] << ":\t" << scale_factor << endl;
			hjk->Scale(scale_factor);
			hjk->Write();
		}
		fout->Close();
	}
	
	return 0;
}

int main(int, char**)
{
	return macro();
}
