
#include <algorithm>

void JetIdEfficiencyScaleFactors(string idType  = "cutbasedId",
				 string idLevel = "Loose")
{

  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1); 
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleBorderSize(0);
    

  // [0] : MC
  // [1] : data
  TFile* f[4][2];
  f[0][0] = new TFile("V00-02-09/52X_newJec/histos_DYJetsToLL_pfjets_TK_pt20to100.root"); //MC
  f[1][0] = new TFile("V00-02-09/52X_newJec/histos_DYJetsToLL_pfjets_HEin_pt20to100.root"); //MC
  f[2][0] = new TFile("V00-02-09/52X_newJec/histos_DYJetsToLL_pfjets_HEout_pt20to100.root"); //MC
  f[3][0] = new TFile("V00-02-09/52X_newJec/histos_DYJetsToLL_pfjets_HF_pt20to100.root"); //MC

  f[0][1] = new TFile("V00-02-09/52X_newJec/histos_DoubleMu2012_pfjets_TK_pt20to100.root"); //DATA
  f[1][1] = new TFile("V00-02-09/52X_newJec/histos_DoubleMu2012_pfjets_HEin_pt20to100.root"); //DATA
  f[2][1] = new TFile("V00-02-09/52X_newJec/histos_DoubleMu2012_pfjets_HEout_pt20to100.root"); //DATA
  f[3][1] = new TFile("V00-02-09/52X_newJec/histos_DoubleMu2012_pfjets_HF_pt20to100.root"); //DATA
  
  TH1F *hEff_vs_JetPt[4][2];
  TH1F *hEff_vs_JetPt_matched[4][2]; 
  
  int rebinPt   = 10;

  // if variable size bins
  double xbins[8] = {0.,10.,20.,30.,40.,50.,70.,100.};


  char hname[200];
  std::string etaRegion[4] = {"TK","HEin","HEout","HF"};
  
  for ( int ifile = 0; ifile < 2 ; ifile++ ){
    for ( int eta = 0; eta < 4 ; eta++ ){
    
      //eff plots
      sprintf(hname,"efficiency/hEff_vs_JetPt_bkgSub_%s_%s", idType.c_str(), idLevel.c_str());
      hEff_vs_JetPt[eta][ifile] = (TH1F*)f[eta][ifile]->Get(hname);
      hEff_vs_JetPt[eta][ifile] ->Rebin(rebinPt);
      hEff_vs_JetPt[eta][ifile] ->Scale(1./rebinPt);
      
      if (ifile==0){
	hEff_vs_JetPt[eta][ifile] ->SetLineColor(kPink+1);
	hEff_vs_JetPt[eta][ifile] ->SetMarkerColor(kPink+1);
	hEff_vs_JetPt[eta][ifile] ->SetMarkerStyle(21);
	//hEff_vs_JetPt[eta][ifile] ->SetFillColor(kPink+1);
	//hEff_vs_JetPt[eta][ifile] ->SetFillStyle(3001);
      }
      
      if (ifile==1){

	hEff_vs_JetPt[eta][ifile] ->SetLineColor(1);
	hEff_vs_JetPt[eta][ifile] ->SetMarkerStyle(20);
	hEff_vs_JetPt[eta][ifile] ->SetMarkerSize(0.8);
      }

      // true eff plots
      if (ifile==0){
	sprintf(hname,"efficiency/hEff_vs_JetPt_matched_%s_%s", idType.c_str(), idLevel.c_str());
	hEff_vs_JetPt_matched[eta][ifile] = (TH1F*)f[eta][ifile]->Get(hname);
	hEff_vs_JetPt_matched[eta][ifile] ->Rebin(rebinPt);
	hEff_vs_JetPt_matched[eta][ifile] ->Scale(1./rebinPt);
      }
    }
  }
  
  // data to MC ratio
  TH1F *hRatio_vs_JetPt[4];
  for ( int eta = 0; eta < 4 ; eta++ ){
    sprintf(hname,"hRatio_vs_JetPt_%s", etaRegion[eta].c_str());
    hRatio_vs_JetPt[eta]= (TH1F*)hEff_vs_JetPt[eta][1]->Clone( hname );
    hRatio_vs_JetPt[eta]->Divide(hEff_vs_JetPt[eta][0]);

    for (int ibin = 1; ibin < hRatio_vs_JetPt[eta]->GetNbinsX()+1; ibin++ ){
      // mc err : diff MC/MCtruth
      float diffMc  = fabs(hEff_vs_JetPt[eta][0]-> GetBinContent(ibin)-hEff_vs_JetPt_matched[eta][0]-> GetBinContent(ibin));
      float errMc   = (diffMc > (hEff_vs_JetPt[eta][0]-> GetBinError(ibin))) ? diffMc : (hEff_vs_JetPt[eta][0]-> GetBinError(ibin));
      float errDa   = hEff_vs_JetPt[eta][1] -> GetBinError(ibin);
      float effMc   = hEff_vs_JetPt[eta][0]-> GetBinContent(ibin);
      float effDa   = hEff_vs_JetPt[eta][1]-> GetBinContent(ibin);
      float err     = (effDa/effMc)*sqrt(pow(errDa/effDa,2)+pow(errMc/effMc,2));
      hRatio_vs_JetPt[eta] -> SetBinError(ibin,err);
    }
   
  }


  TCanvas *cEffJetPt[4];
  char cnamePt[200];

  for ( int eta = 0; eta < 4 ; eta++ ){
    // -- vs Pt
    sprintf(cnamePt,"cEff_vs_JetPt_%s", etaRegion[eta].c_str());
    cEffJetPt[eta] = new TCanvas(cnamePt,cnamePt);
    
    TPad *cLowerPt  = new TPad("pad_0","pad_0",0.00,0.00,1.00,0.45);
    TPad *cUpperPt  = new TPad("pad_2","pad_2",0.00,0.45,1.00,1.00);
    cLowerPt->SetBottomMargin(0.25); 
    cLowerPt->SetTopMargin(0.1);
    cUpperPt->SetBottomMargin(0.13); 
    cUpperPt->SetTopMargin(0.1);
    cLowerPt->Draw();
    cUpperPt->Draw();
    
    float FontSCF  = cUpperPt->GetHNDC()/cLowerPt->GetHNDC(); 
    float tYoffset = 0.6; 
    float labSize  = 0.06;
    
    cUpperPt-> cd();
    hEff_vs_JetPt[eta][0] -> GetXaxis()->SetLabelSize(labSize);
    hEff_vs_JetPt[eta][0] -> GetXaxis()->SetTitleSize(labSize);
    hEff_vs_JetPt[eta][0] -> GetYaxis()->SetLabelSize(labSize);
    hEff_vs_JetPt[eta][0] -> GetYaxis()->SetTitleSize(labSize);
    hEff_vs_JetPt[eta][0] -> GetYaxis()->SetTitleOffset(tYoffset);
    hEff_vs_JetPt[eta][0] -> GetXaxis()->SetRangeUser(10.,100.);
    hEff_vs_JetPt[eta][0] -> GetYaxis()->SetRangeUser(0.6,1.2);
    hEff_vs_JetPt[eta][0] -> Draw("pl");
    hEff_vs_JetPt_matched[eta][0] -> Draw("esame");
    hEff_vs_JetPt[eta][1] -> Draw("e1same");
      
    cLowerPt-> cd();
    cLowerPt-> SetGridy();
    hRatio_vs_JetPt[eta] ->SetNdivisions(505);
    hRatio_vs_JetPt[eta] ->SetTitle("");
    hRatio_vs_JetPt[eta] ->GetXaxis()->SetLabelSize(labSize*FontSCF);
    hRatio_vs_JetPt[eta] ->GetXaxis()->SetTitleSize(labSize*FontSCF);
    hRatio_vs_JetPt[eta] ->GetYaxis()->SetLabelSize(labSize*FontSCF);
    hRatio_vs_JetPt[eta] ->GetYaxis()->SetTitleSize(labSize*FontSCF);
    hRatio_vs_JetPt[eta] ->GetYaxis()->SetTitleOffset(tYoffset/FontSCF);
    hRatio_vs_JetPt[eta] ->GetXaxis()->SetRangeUser(10.,100.);
    hRatio_vs_JetPt[eta] ->GetYaxis()->SetRangeUser(0.8,1.2);
    hRatio_vs_JetPt[eta] ->GetYaxis()->SetTitle("data/MC");
    hRatio_vs_JetPt[eta] ->Draw("e");

    
    TFile *fileout = new TFile("JetIdScaleFactor_ZmumuJets.root","recreate");
    hRatio_vs_JetPt[0]->Write("hJetIdScaleFactor_TK");
    hRatio_vs_JetPt[1]->Write("hJetIdScaleFactor_HEin");
    hRatio_vs_JetPt[2]->Write("hJetIdScaleFactor_HEout");
    hRatio_vs_JetPt[3]->Write("hJetIdScaleFactor_HF");
    fileout->Close();
  }
  
  

}
