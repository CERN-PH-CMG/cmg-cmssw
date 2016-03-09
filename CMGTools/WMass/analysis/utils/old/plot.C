#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TString.h"
#include "TObject.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "THStack.h"
#include "TMath.h"
#include "TLatex.h"
#include "TPaveStats.h"

#include <stdio.h>
#include <fstream>
#include <iostream>

bool debug=true;

double round_fn(double pre_round, int round_dig)
{
  pre_round *= pow(10.,round_dig);
  int the_floor = (int) TMath::Floor(pre_round);
  double remainder = pre_round - the_floor;
  if(remainder >= 0.5) the_floor++;
  pre_round = the_floor / pow(10.,round_dig);
  return pre_round;
}


void plotPreliminary(TString text, float posX ,float posY){

  //  TString leg = "L = ";  
                                                                                                                                          
  //  leg += text.data(); 
                                                                                                                                             

  TLatex latexLabel;

  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  latexLabel.DrawLatex(posX, posY+0.04, "CMS Preliminary");
  latexLabel.DrawLatex(posX, posY, "#sqrt{s} = 8 TeV");
  latexLabel.DrawLatex(posX, posY-0.04, text.Data());

}

void plotLeg(string tagger, double weight, TH1* h1,float posX, float posY, TString drawOption){

  TString leg = "";
  leg += tagger.data();

  TLegend* this_leg = new TLegend(posX,posY,posX+0.1,posY+0.05);
  this_leg->SetFillColor(0);
  this_leg->SetBorderSize(0);
  this_leg->SetTextColor(h1->GetLineColor());
  //  this_leg->SetTextSize(0.1);
  this_leg->SetTextSize(0.04);
  this_leg->AddEntry(h1, leg, drawOption);
  this_leg->Draw();

}

void plotLegMarker(string tagger, double weight, TGraph* h1,float posX, float posY, TString drawOption) {

  TString leg = "";
  leg += tagger.data();

  TLegend* this_leg = new TLegend(posX,posY,posX+0.1,posY+0.05);
  this_leg->SetFillColor(0);
  this_leg->SetBorderSize(0);
  this_leg->SetTextSize(0.04);
  this_leg->AddEntry(h1, leg, drawOption);
  this_leg->Draw();

}

void drawLine(int X, int lowY, int highY) {

  TLine *line = new TLine(X,lowY,X,highY);
  line->SetLineColor(11);
  line->Draw();

}


TH1 * getHisto (TString fileName, TString dirName, TString histoName, int color, int style, int rebin, bool is2d=false) {

  TH1 * h_=0;

  TFile *file_ = TFile::Open(fileName);

  if(!file_) return h_;

  TDirectory * dir;

  if(dirName.Contains("0")) {
    //    file_->ls();  
                                                                                                                                                   
    if(!is2d) h_ = (TH1F*) file_->Get(histoName);
    if(is2d) h_ = (TH2F*) file_->Get(histoName);
  } else {
    //    file_->ls();  
                                                                                                                                                   
    dir = (TDirectory*) file_->Get(dirName);
    //    dir->ls();  
                                                                                                                                                     
    if(!is2d) h_ = (TH1F*) dir->Get(histoName);
    if(is2d) h_ = (TH2F*) dir->Get(histoName);
  }

  if(is2d && h_) return h_;

  if(h_) h_->SetMarkerColor(color);
  if(h_) h_->SetLineColor(color);
  if(h_) h_->SetLineWidth(3);

  if(fileName.Contains("data")) {

    if(h_) h_->SetMarkerColor(color);

  } else {

    if(h_) h_->SetLineColor(color);
    if(h_) h_->SetLineStyle(style);
    if(h_) h_->SetLineWidth(3);

    if(h_) h_->SetMarkerColor(color);
    if(h_) h_->SetMarkerStyle(1);

    //    if(h_) h_->SetFillColor(color); 
    //    if(h_) h_->SetFillStyle(style);   
    //    if(color==2)h_->SetLineWidth(3);  
                                                                                                                               

  }

  if(debug){
    if(h_) cout << "FOUND fileName " << fileName << " dirName "  << dirName << " histoName " << histoName << endl;
    if(!h_) cout << "NOTFOUND fileName " << fileName << " dirName "  << dirName << " histoName " << histoName << endl;
  }

  if(h_) {

    Int_t nBinX= h_->GetXaxis()->GetNbins();  
                                                                                                                             
    // overFlow 
    h_->SetBinContent(nBinX,h_->GetBinContent(nBinX)+h_->GetBinContent(nBinX+1)); 
    h_->SetBinContent(nBinX+1,0);     
    // underFlow
    h_->SetBinContent(1,h_->GetBinContent(0)+h_->GetBinContent(1));  
    h_->SetBinContent(0,0);      
   
  }

  //  if(h_) h_->Scale(1./h_->Integral()); 

  if(h_) h_->Rebin(rebin);

  if(h_) h_->SetDirectory(0);
  file_->Close();

  //  if(h_) cout << "found " << h_ << endl;
                                                                                                                               
  if(h_) return h_;

  return 0;

}

//TGraph * makeGraph(int binMax, double[100] x, double[100] RMS_y, TH1 * histo, int style) {
TGraph * makeGraph(int binMax, double* x, double* RMS_y, TH1 * histo, int style) {
  
  TGraph * gr = new TGraph(binMax,x,RMS_y);
  if(gr && histo) gr->SetLineColor(histo->GetLineColor());
  if(gr && histo) gr->SetMarkerColor(histo->GetLineColor());
  if(gr) gr->SetMarkerStyle(style);

  return gr;

}

TGraph * getGraph(TString fileName, TString dirName, TString histoName, int color, int style, int rebin, bool doZPt, bool doRMS, bool is2d=false) {

  bool doFit = false;
  bool dotk = true;
  bool doTail =false;

  Double_t x[100];
  Double_t RMS_data[100];

  int binMax=13; //pt 
  //  int binMax=10; //eta
  //  if(!doZPt) binMax=20; // vtx 
  if(!doZPt) binMax=35; // vtx 

  if(fileName.Contains("7TeV")) binMax=20;
  if(fileName.Contains("output_DYJetsMadSig")) binMax=20;

  TString histo = "";

  for(int iBin=1; iBin<=binMax; iBin++) {

    TString Binning="";

    if(doZPt) {
      /*
      if ( iBin==1 ) Binning = "_Zyinf20";
      else if  ( iBin==2 ) Binning = "_Zy2015";
      else if  ( iBin==3 ) Binning = "_Zy1510"; 
      else if  ( iBin==4 ) Binning = "_Zy1005";
      else if  ( iBin==5 ) Binning = "_Zy0500"; 
      else if  ( iBin==6 ) Binning = "_Zy0005"; 
      else if  ( iBin==7 ) Binning = "_Zy0510"; 
      else if  ( iBin==8 ) Binning = "_Zy1015";
      else if  ( iBin==9 ) Binning = "_Zy1520";
      else if  ( iBin==10 ) Binning = "_Zy20inf";
      */

      if ( iBin==1 ) Binning = "_Zpt02";
      else if  ( iBin==2 ) Binning = "_Zpt24";
      else if  ( iBin==3 ) Binning = "_Zpt46";
      else if  ( iBin==4 ) Binning = "_Zpt68";
      else if  ( iBin==5 ) Binning = "_Zpt810";
      else if  ( iBin==6 ) Binning = "_Zpt1012";
      else if  ( iBin==7 ) Binning = "_Zpt1214";
      else if  ( iBin==8 ) Binning = "_Zpt1416";
      else if  ( iBin==9 ) Binning = "_Zpt1618";
      else if  ( iBin==10 ) Binning = "_Zpt1820";
      else if  ( iBin==11 ) Binning = "_Zpt2030";
      else if  ( iBin==12 ) Binning = "_Zpt3050";
      else if  ( iBin==13 ) Binning = "_Zpt50";

    } else {

      Binning = "_VTX";
      Binning += iBin;

    }

    histo = histoName;
    histo += Binning.Data();
    if(dotk) histo += "_tk";

    TString dirName = "0";

    TH1 *data = getHisto(fileName,dirName,histo,1,1,rebin);

    if(doZPt) x[iBin] = iBin*2-1;
    if(!doZPt) x[iBin] = iBin;


    if(!doFit) {

      if(doRMS && data) RMS_data[iBin] = data->GetRMS();

    } else {

      if(data) RMS_data[iBin] = getFit(data, doRMS);

      if(doRMS && doTail && MC_corr) RMS_data[iBin] = getTail(data,3);

      if(!doRMS && data) RMS_data[iBin] = data->GetMean();

    }

  } // end binning


  cout << " out of the loop 1111 " << endl;

  for (int i=0; i<binMax ; i++ ) {
    cout << RMS_data[i] << endl;
  }

  TGraph * gr = new TGraph(binMax,x,RMS_data);
  if(gr && data) gr->SetLineColor(color);
  if(gr && data) gr->SetMarkerColor(color);
  if(gr) gr->SetMarkerStyle(style);

  cout << " before return  " << endl;
  if(gr) return gr;

}

TH2F * getHisto2D(TString fileName, TString dirName, TString histoName) {

  TH2F * h_=0;

  TFile *file_ = TFile::Open(fileName);

  if(!file_) return h_;

  cout << "filename " << fileName << " histoName " << histoName << endl;

  TDirectory * dir;

  if(dirName.Contains("0")) {
    //    file_->ls();
                                                                                                                              
    h_ = (TH2F*) file_->Get(histoName);
  } else {
    dir = (TDirectory*) file_->Get(dirName);
    //    dir->ls(); 
                                                                                                                              
    h_ = (TH2F*) dir->Get(histoName);
  }

  if(h_) return h_;

  return 0;

}



////$$$$$$$$$
////$$$$$$$$$
////$$$    up to here the Utils
////$$$    BELOW THE    
////$$$$$$$$$
////$$$$$$$$$


void getCorrelation()
{

  TString fileNameDATA = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_DATA_pol3_rapBin1.root";
  TString fileNameMCmad = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_MC_MADGRAPH_pol3_PDF-1_rapBin1.root";
  TString fileNameMCpow = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_MC_POWHEG_pol3_PDF-1_rapBin1.root";

  TString dirName = "0";

  Double_t x[100];
  Double_t corr_data[100], corr_mad[100], corr_pow[100];

  int binMax=10;
  int n=0;

  TLatex latexLabel;
  
  latexLabel.SetTextSize(0.05);
  latexLabel.SetNDC();

  TCanvas c1;
  c1.Divide(2,2);

  for(int iC=1; iC<=4; iC++) {

    for(int iBin=1; iBin<=binMax; iBin++) {
      
      TString Binning="";
      
      TString histo ="hu1vsu2";
      if ( iBin==1 ) Binning = "_02";
      else if  ( iBin==2 ) Binning = "_24";
      else if  ( iBin==3 ) Binning = "_46";
      else if  ( iBin==4 ) Binning = "_68";
      else if  ( iBin==5 ) Binning = "_810";
      else if  ( iBin==6 ) Binning = "_1012";
      else if  ( iBin==7 ) Binning = "_1214";
      else if  ( iBin==8 ) Binning = "_1416";
      else if  ( iBin==9 ) Binning = "_1618";
      else if  ( iBin==10 ) Binning = "_1820";
      
      histo+=Binning.Data();
      //    histo+="_tk";
      
      TH2F *data = getHisto2D(fileNameDATA,dirName,histo);
      TH2F *MC_mad = getHisto2D(fileNameMCmad,dirName,histo);
      TH2F *MC_pow = getHisto2D(fileNameMCpow,dirName,histo);
      
      int nBin = data->GetNbinsX();
      
      TH2D *empty_data = data->Clone();
      TH2D *empty_mad = MC_mad->Clone();
      TH2D *empty_pow = MC_pow->Clone();
       
      if(iC==1 || iC==2) {
	// put at zero the lower half
	for(int j=0; j<(nBin/2)+1; j++) {
	  for(int i=0; i<(nBin)+1; i++) {
	    empty_data->SetBinContent(i,j,0);
	    empty_mad->SetBinContent(i,j,0);
	    empty_pow->SetBinContent(i,j,0);
	  }
	}


	for(int j=(nBin/2)+1; j<(nBin)+1; j++) {
	  if(iC==1) {
	    // set at zero the second quadrant
	    for(int i=(nBin/2)+1; i<(nBin)+1; i++) { 
		empty_data->SetBinContent(i,j,0);
		empty_mad->SetBinContent(i,j,0);
		empty_pow->SetBinContent(i,j,0);
	      }
	  } else {
	    // set at zero the first quadrant
	    for(int i=0; i<(nBin/2)+1; i++) { 
	      empty_data->SetBinContent(i,j,0);
	      empty_mad->SetBinContent(i,j,0);
	      empty_pow->SetBinContent(i,j,0);
	    }
	  }
	}
      }


      if(iC==3 || iC==4) {
	// put at zero the upper half
	for(int j=(nBin/2)+1; j<(nBin)+1; j++) {
	  for(int i=0; i<(nBin)+1; i++) {
	    empty_data->SetBinContent(i,j,0);
	    empty_mad->SetBinContent(i,j,0);
	    empty_pow->SetBinContent(i,j,0);
	  }
	}

	for(int j=0; j<(nBin/2)+1; j++) {
	// set at zero the second quadrant
	  if(iC==3) {
	    for(int i=(nBin/2)+1; i<(nBin)+1; i++) { 
		empty_data->SetBinContent(i,j,0);
		empty_mad->SetBinContent(i,j,0);
		empty_pow->SetBinContent(i,j,0);
	      }
	  } else {
	// set at zero the first quadrant
	    for(int i=0; i<(nBin/2)+1; i++) { 
	      empty_data->SetBinContent(i,j,0);
	      empty_mad->SetBinContent(i,j,0);
	      empty_pow->SetBinContent(i,j,0);
	    }
	  }
	}
      }

  
      /*    
      data->Draw("colz");
      c1.Update();
      cin.get(); 
      */

      /*	 
		 empty_data->Draw("colz");
		 c1->Update();
		 cin.get();
      */
      
      x[iBin] = iBin*2-1;
      
      //    if(data) corr_data[iBin] = data->GetCorrelationFactor() ;
      //    if(MC_raw) corr_mc[iBin] = MC_raw->GetCorrelationFactor();
      
      if(data) corr_data[iBin] = empty_data->GetCorrelationFactor() ;
      if(MC_mad) corr_mad[iBin] = empty_mad->GetCorrelationFactor();
      if(MC_pow) corr_pow[iBin] = empty_pow->GetCorrelationFactor();
      
      //    data->GetRMS();
      //    MC_raw->GetRMS();
      //    if(data) cout << "data " << histo.Data() << " " << corr_data[iBin] << endl;
      //    if(MC_raw) cout << "MC " << histo.Data() << " " << corr_mc[iBin] << endl;
      
      n++;
    }
    
    TGraph * gr_mc_mad = new TGraph(n, x, corr_mad);
    TGraph * gr_mc_pow = new TGraph(n, x, corr_pow);
    TGraph * gr_data = new TGraph(n,   x, corr_data);
    
    gr_mc_mad->SetMarkerColor(2);
    gr_mc_mad->SetLineColor(2);
    gr_mc_mad->SetMarkerStyle(21);
    
    gr_mc_pow->SetMarkerColor(4);
    gr_mc_pow->SetLineColor(4);
    gr_mc_pow->SetMarkerStyle(21);
    
    gr_data->SetMarkerStyle(20);


    //  TCanvas * c = new TCanvas("c","c",700,700); 
    //  c->cd();
    //  gr_data->Draw("A C P");
    //  gr_mc_raw->Draw("A C P");
    
    c1.cd(iC);   
 
    TMultiGraph *mg = new TMultiGraph();
    
    mg->Add(gr_mc_mad,"c p ");
    mg->Add(gr_mc_pow,"c p ");
    mg->Add(gr_data,"c p ");
    
    mg->Draw("A");

    mg->GetXaxis()->SetTitle("Z p_{T}");    
    mg->GetYaxis()->SetTitle("coeff corr u1 u2");    

    gPad->Modified();
    gPad->SetGridy(1);

    if(iC==1 || iC==4) mg->SetMaximum(0.);
    if(iC==1 || iC==4) mg->SetMinimum(-0.30);

    if(iC==2 || iC==3) mg->SetMaximum(0.30);
    if(iC==2 || iC==3) mg->SetMinimum(0.);
    
    if(iC==1) latexLabel.DrawLatex(0.55, 0.85, "U1<0 U2>0");
    if(iC==2) latexLabel.DrawLatex(0.55, 0.85, "U1>0 U2>0");
    if(iC==3) latexLabel.DrawLatex(0.55, 0.85, "U1<0 U2<0");
    if(iC==4) latexLabel.DrawLatex(0.55, 0.85, "U1>0 U2<0");

    if(gr_data) plotLegMarker("data",1.,gr_data,0.35,0.1+0.05,"p");
    if(gr_mc_mad) plotLegMarker("MC mad",1.,gr_mc_mad,0.35,0.1+2*0.05,"p");
    if(gr_mc_pow) plotLegMarker("MC pow",1.,gr_mc_pow,0.35,0.1+3*0.05,"p");

    c1.Update();
    
  }

  c1.SaveAs("Correlation.png");
  
  
  cin.get();


}


void trendGraph() {

  for(int iType=0; iType<3; iType++) {

    TLatex latexLabel;

    latexLabel.SetTextSize(0.04);
    latexLabel.SetNDC();

    for(int iVar=0; iVar<2; iVar++) {

      cout << "doing iType: " << iType << " iVar "<< iVar << endl;

      gSystem->Exec("rm Scale.txt; rm R1.txt; rm R2.txt");

      if(iVar==0 && iType==0) gSystem->Exec("./getResult | grep +/- | grep SCALE | grep Wlike | grep Mt | awk '{print $2,$6;}' > Scale.txt");
      if(iVar==0 && iType==0) gSystem->Exec("./getResult | grep +/- | grep RESOLU1 | grep Wlike | grep Mt | awk '{print $2,$6;}' > R1.txt");
      if(iVar==0 && iType==0) gSystem->Exec("./getResult | grep +/- | grep RESOLU2 | grep Wlike | grep Mt | awk '{print $2,$6;}' > R2.txt");

      if(iVar==1 && iType==0) gSystem->Exec("./getResult | grep +/- | grep SCALE | grep Wlike | grep MET | awk '{print $2,$6;}' > Scale.txt");
      if(iVar==1 && iType==0) gSystem->Exec("./getResult | grep +/- | grep RESOLU1 | grep Wlike | grep MET | awk '{print $2,$6;}' > R1.txt");
      if(iVar==1 && iType==0) gSystem->Exec("./getResult | grep +/- | grep RESOLU2 | grep Wlike | grep MET | awk '{print $2,$6;}' > R2.txt");

      if(iVar==0 && iType==1) gSystem->Exec("./getResult | grep +/- | grep SCALE | grep 'W Pos' | grep Mt | awk '{print $2,$6;}' > Scale.txt");
      if(iVar==0 && iType==1) gSystem->Exec("./getResult | grep +/- | grep RESOLU1 | grep 'W Pos' | grep Mt | awk '{print $2,$6;}' > R1.txt");
      if(iVar==0 && iType==1) gSystem->Exec("./getResult | grep +/- | grep RESOLU2 | grep 'W Pos' | grep Mt | awk '{print $2,$6;}' > R2.txt");


      if(iVar==1 && iType==1) gSystem->Exec("./getResult | grep +/- | grep SCALE | grep 'W Pos' | grep MET | awk '{print $2,$6;}' > Scale.txt");
      if(iVar==1 && iType==1) gSystem->Exec("./getResult | grep +/- | grep RESOLU1 | grep 'W Pos' | grep MET | awk '{print $2,$6;}' > R1.txt");
      if(iVar==1 && iType==1) gSystem->Exec("./getResult | grep +/- | grep RESOLU2 | grep 'W Pos' | grep MET | awk '{print $2,$6;}' > R2.txt");

      if(iVar==0 && iType==2) gSystem->Exec("./getResult | grep +/- | grep SCALE | grep 'W Neg' | grep Mt | awk '{print $2,$6;}' > Scale.txt");
      if(iVar==0 && iType==2) gSystem->Exec("./getResult | grep +/- | grep RESOLU1 | grep 'W Neg' | grep Mt | awk '{print $2,$6;}' > R1.txt");
      if(iVar==0 && iType==2) gSystem->Exec("./getResult | grep +/- | grep RESOLU2 | grep 'W Neg' | grep Mt | awk '{print $2,$6;}' > R2.txt");

      if(iVar==1 && iType==2) gSystem->Exec("./getResult | grep +/- | grep SCALE | grep 'W Neg' | grep MET | awk '{print $2,$6;}' > Scale.txt");
      if(iVar==1 && iType==2) gSystem->Exec("./getResult | grep +/- | grep RESOLU1 | grep 'W Neg' | grep MET | awk '{print $2,$6;}' > R1.txt");
      if(iVar==1 && iType==2) gSystem->Exec("./getResult | grep +/- | grep RESOLU2 | grep 'W Neg' | grep MET | awk '{print $2,$6;}' > R2.txt");

      TGraph * s1 = new TGraph("Scale.txt", "%lg %lg");
      s1->SetMarkerColor(2);
      s1->SetLineColor(2);
      s1->SetLineWidth(3);

      TGraph * g1 = new TGraph("R1.txt", "%lg %lg");
      g1->SetMarkerColor(4);
      g1->SetLineColor(4);
      g1->SetLineWidth(3);

      TGraph * g2 = new TGraph("R2.txt", "%lg %lg");
      g2->SetMarkerColor(kGreen+1);
      g2->SetLineColor(kGreen+1);
      g2->SetLineWidth(3);

      TMultiGraph *mg = new TMultiGraph();

      mg->Add(g1,"lp");
      mg->Add(g2,"lp");
      mg->Add(s1,"lp");

      if(iVar==0) mg->SetMaximum(15.);
      if(iVar==0) mg->SetMinimum(-15.);
      if(iVar==1) mg->SetMaximum(35.);
      if(iVar==1) mg->SetMinimum(-35.);

      TCanvas * c1 = new TCanvas("c1","c1",700,700);
      c1->Draw();
      c1->SetGridy(1);
      c1->SetGridx(1);

      mg->Draw("A");

      mg->GetXaxis()->SetTitle("# sigma");
      mg->GetYaxis()->SetTitle("uncertainty");
      mg->GetXaxis()->SetAxisColor(12);
      mg->GetYaxis()->SetAxisColor(12);
      c1->RedrawAxis();

      if(iVar==0) latexLabel.DrawLatex(0.25, 0.75, "M_{T}");
      if(iVar==1) latexLabel.DrawLatex(0.25, 0.75, "MET");

      if(iType==0) latexLabel.DrawLatex(0.25, 0.65, "Wlike Pos");
      if(iType==1) latexLabel.DrawLatex(0.25, 0.65, "W Pos");
      if(iType==2) latexLabel.DrawLatex(0.25, 0.65, "W Neg");

      if(s1) plotLegMarker("Scale U1",1.,s1,0.55,0.35+0.05,"p");
      if(g1) plotLegMarker("Resol U1",1.,g1,0.55,0.3+0.05,"p");
      if(g2) plotLegMarker("Resol U2",1.,g2,0.55,0.25+0.05,"p");

      TString fileToPrint="trend";
      if(iVar==0) fileToPrint + = "_MT";
      if(iVar==1) fileToPrint + = "_MET";
      if(iType==0) fileToPrint + = "_WlikePos";
      if(iType==1) fileToPrint + = "_WPos";
      if(iType==2) fileToPrint + = "_WNeg";
      fileToPrint + = ".pdf";

      c1->Print(fileToPrint.Data());

    }

  }

}


void drawGraph(bool doU1=false, bool doZ=true)
{

  gStyle->SetOptStat(0000000);

  TString dirName="/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/";  
  //  TString dirName="/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/";

  TString fileNameMC = dirName.Data();  
  fileNameMC += "file2Dfit_ZMC_MADGRAPH_Y1_PDF-1_3G.root";
  //  fileNameMC += "file2Dfit_ZMC_MADGRAPH_Y1_PDF-1.root";
  TString fileNamePOW = dirName.Data();
  fileNamePOW += "file2Dfit_ZMC_POWHEG_Y1_PDF-1_3G.root";
  //  fileNamePOW += "file2Dfit_ZMC_POWHEG_Y1_PDF-1.root";
  TString fileNameDATA = dirName.Data(); 
  fileNameDATA += "file2Dfit_DATA_Y1_PDF-1_3G.root";
  //  fileNameDATA += "file2Dfit_DATA_Y1_PDF-1.root";

  TString fileNameWposPOW = dirName.Data();
  fileNameWposPOW += "file2Dfit_WposMC_POWHEG_Y1_PDF-1_3G.root";
  TString fileNameWnegMAD = dirName.Data();
  fileNameWnegMAD += "file2Dfit_WnegMC_MADGRAPH_Y1_PDF-1_3G.root";
  TString fileNameWposMAD = dirName.Data();
  fileNameWposMAD += "file2Dfit_WposMC_MADGRAPH_Y1_PDF-1_3G.root";

  /*
  Form("%s/%s",dirName.Data(),"file2Dfit_ZMC_MADGRAPH_Y1_PDF-1_3G.root");
  Form("%s/%s",dirName.Data(),"file2Dfit_ZMC_POWHEG_Y1_PDF-1_3G.root");
  Form("%s/%s",dirName.Data(),"file2Dfit_DATA_Y1_PDF-1_3G.root");

  Form("%s/%s",dirName.Data(),"file2Dfit_WposMC_MADGRAPH_Y1_PDF-1_3G.root");
  Form("%s/%s",dirName.Data(),"file2Dfit_WnegMC_MADGRAPH_Y1_PDF-1_3G.root");
  Form("%s/%s",dirName.Data(),"file2Dfit_WposMC_POWHEG_Y1_PDF-1_3G.root");
  */


  TString histoName="chi2";
  if(doU1) histoName += "_U1";
  if(!doU1) histoName += "_U2";

  TFile *fileDATA_ = TFile::Open(fileNameDATA);
  TGraph* gDATA_ = (TGraph*) fileDATA_->Get(histoName.Data());
  gDATA_->SetMarkerColor(1);
  gDATA_->SetLineColor(1);
  gDATA_->SetLineWidth(3);
  gDATA_->SetMarkerStyle(kFullCircle);

  TFile *filePOW_ = TFile::Open(fileNamePOW);
  TGraph* gPOW_ = (TGraph*) filePOW_->Get(histoName.Data());
  if(gPOW_) gPOW_->SetMarkerColor(2);
  if(gPOW_) gPOW_->SetLineColor(2);
  if(gPOW_) gPOW_->SetLineWidth(3);
  if(gPOW_) gPOW_->SetMarkerStyle(kFullCircle);

  TFile *fileMAD_ = TFile::Open(fileNameMC);
  TGraph* gMAD_ = (TGraph*) fileMAD_->Get(histoName.Data());
  if(gMAD_) gMAD_->SetMarkerColor(4);
  if(gMAD_) gMAD_->SetLineColor(4);
  if(gMAD_) gMAD_->SetLineWidth(3);
  if(gMAD_) gMAD_->SetMarkerStyle(kFullCircle);

  TFile *fileWposPOW_ = TFile::Open(fileNameWposPOW);
  TGraph* gWposPOW_ = (TGraph*) fileWposPOW_->Get(histoName.Data());
  if(gWposPOW_) gWposPOW_->SetMarkerColor(1);
  if(gWposPOW_) gWposPOW_->SetLineColor(1);
  if(gWposPOW_) gWposPOW_->SetLineWidth(3);
  if(gWposPOW_) gWposPOW_->SetMarkerStyle(kFullCircle);

  TFile *fileWposMAD_ = TFile::Open(fileNameWposMAD);
  TGraph* gWposMAD_ = (TGraph*) fileWposMAD_->Get(histoName.Data());
  if(gWposMAD_) gWposMAD_->SetMarkerColor(2);
  if(gWposMAD_) gWposMAD_->SetLineColor(2);
  if(gWposMAD_) gWposMAD_->SetLineWidth(3);
  if(gWposMAD_) gWposMAD_->SetMarkerStyle(kFullCircle);

  TFile *fileWnegMAD_ = TFile::Open(fileNameWnegMAD);
  TGraph* gWnegMAD_ = (TGraph*) fileWnegMAD_->Get(histoName.Data());
  if(gWnegMAD_) gWnegMAD_->SetMarkerColor(4);
  if(gWnegMAD_) gWnegMAD_->SetLineColor(4);
  if(gWnegMAD_) gWnegMAD_->SetLineWidth(3);
  if(gWnegMAD_)gWnegMAD_->SetMarkerStyle(kFullCircle);

  if(doZ) {

    if(gPOW_) gPOW_->Draw("A L P ");
    if(gDATA_) gDATA_->Draw("L P same");
    if(gMAD_) gMAD_->Draw("L P same");
    
    if(gDATA_) plotLegMarker("DATA",1.,gDATA_,0.2,0.80,"l");
    if(gPOW_) plotLegMarker("POWHEG",1.,gPOW_, 0.2,0.75,"l");
    if(gMAD_) plotLegMarker("MAD",1.,gMAD_, 0.2,0.7,"l");

  } else {

    if(gWposPOW_) gWposPOW_->Draw("A L P ");
    if(gWposMAD_) gWposMAD_->Draw("L P same");
    if(gWnegMAD_) gWnegMAD_->Draw("L P same");
    
    if(gWposPOW_) plotLegMarker("W^{+} powheg",1.,gWposPOW_,0.2,0.80,"l");
    if(gWposMAD_) plotLegMarker("W^{+} mad"   ,1.,gWposMAD_,0.2,0.75,"l");
    if(gWnegMAD_) plotLegMarker("W^{-} mad"   ,1.,gWnegMAD_,0.2,0.7,"l");
  }

}


void fitDiffHisto(int Zpt, bool doU1=false, bool doClosure=false)
{

  gStyle->SetOptStat(0000000);

  TString dirName="/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/";  
  //  TString dirName="/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/";

  TString fileNameMC = dirName.Data();  
  fileNameMC += "file2Dfit_ZMC_MADGRAPH_Y1_PDF-1_3G.root";
  TString fileNamePOW = dirName.Data();
  fileNamePOW += "file2Dfit_ZMC_POWHEG_Y1_PDF-1_3G.root";
  TString fileNameDATA = dirName.Data(); 
  fileNameDATA += "file2Dfit_DATA_Y1_PDF-1_3G.root";

  TString fileNamePOWsmeared = dirName.Data();
  fileNamePOWsmeared += "file2Dfit_ZMC_POWHEG_ITERATIVE_Y1_PDF-1_3G.root";

  double rebin=10;

  TString histoName = "hh_";
  if(!doClosure)  histoName += "plotOn_";
  if(doU1) { histoName += "U1";
  } else {
    histoName += "U2";
  } 
  histoName += "_1_Zpt";
  histoName += Zpt;

  TString histoNameFit = histoName.Data();
  histoName += "__XVar";
  histoNameFit += "_Fit__XVar";

  TCanvas * c1 = new TCanvas("c1","c1",700,700);
  c1->Draw();
  c1->cd();
  c1->SetGridx(1);

  TH1 *MC_ptMC = getHisto(fileNameMC,"0",histoName,4,1, rebin);
  TH1 *MC_ptMC_fit = getHisto(fileNameMC,"0",histoNameFit,4,1, rebin);
  TH1 *MC_ptPOW = getHisto(fileNamePOW,"0",histoName,2,1, rebin);
  TH1 *MC_ptPOW_fit = getHisto(fileNamePOW,"0",histoNameFit,2,1, rebin);
  TH1 *MC_ptDATA = getHisto(fileNameDATA,"0",histoName,1,1,rebin);
  TH1 *MC_ptDATA_fit = getHisto(fileNameDATA,"0",histoNameFit,1,1,rebin);

  TH1 *MC_ptPOW_smeared = getHisto(fileNamePOWsmeared,"0",histoName,2,3, rebin);

  if(!doClosure) { 

    //  TH1 *MC_ptMC_ = MC_ptMC->Clone();
    //  TH1 *MC_ptPOW_ = MC_ptPOW->Clone();
    //  TH1 *MC_ptDATA_ = MC_ptDATA->Clone();
    
    if(MC_ptMC_fit) MC_ptMC_fit->Scale(-1./MC_ptMC_fit->Integral());
    if(MC_ptMC) MC_ptMC->Scale(1./MC_ptMC->Integral());
    //  if(MC_ptMC) MC_ptMC_->Scale(-1./MC_ptMC->Integral());
    if(MC_ptMC && MC_ptMC_fit) MC_ptMC->Add(MC_ptMC_fit);
    //  MC_ptMC->Add(MC_ptDATA);
    
    if(MC_ptPOW_fit) MC_ptPOW_fit->Scale(-1./MC_ptPOW_fit->Integral());
    if(MC_ptPOW) MC_ptPOW->Scale(1./MC_ptPOW->Integral());
    //  if(MC_ptPOW_) MC_ptPOW_->Scale(-1./MC_ptPOW->Integral());
    if(MC_ptPOW) MC_ptPOW->Add(MC_ptPOW_fit);
    //  MC_ptPOW->Add(MC_ptDATA);
    
    if(MC_ptDATA_fit) MC_ptDATA_fit->Scale(-1./MC_ptDATA_fit->Integral());
    if(MC_ptDATA) MC_ptDATA->Scale(1./MC_ptDATA->Integral());
    //  if(MC_ptDATA) MC_ptDATA_->Scale(-1./MC_ptDATA->Integral());
    if(MC_ptDATA && MC_ptDATA_fit) MC_ptDATA->Add(MC_ptDATA_fit);
    //  MC_ptDATA->Add(MC_ptDATA);
    
    if(MC_ptDATA) MC_ptDATA->GetXaxis()->SetTitle("[U-UmeanFit]/UsigmaFit");
    if(MC_ptDATA) MC_ptDATA->GetYaxis()->SetTitle("dataset - fit");
    if(MC_ptDATA) MC_ptDATA->GetXaxis()->SetRangeUser(-5.,5.);
    if(MC_ptDATA) MC_ptDATA->GetYaxis()->SetRangeUser(-0.02,0.02);
    //  MC_ptDATA->GetYaxis()->SetRangeUser(-1.,1.);
    
    if(MC_ptDATA) MC_ptDATA->SetTitle("");
    if(MC_ptDATA) MC_ptDATA->Draw("hist");
    if(MC_ptPOW) MC_ptPOW->Draw("hist same");
    if(MC_ptMC) MC_ptMC->Draw("hist same");
    
    if(MC_ptDATA) plotLeg("DATA",1.,MC_ptDATA,0.2,0.80,"l");
    if(MC_ptPOW) plotLeg("POWHEG",1.,MC_ptPOW, 0.2,0.75,"l");
    if(MC_ptMC) plotLeg("MAD",1.,MC_ptMC, 0.2,0.7,"l");
    
  } else {

    if(MC_ptDATA) MC_ptDATA->Draw("hist");
    if(MC_ptPOW) MC_ptPOW->Draw("hist same");
    if(MC_ptPOW_smeared) MC_ptPOW_smeared->Draw("hist same");
  }

  TString stringToPrint = "Zpt=";
  stringToPrint + = Zpt;
  if(doU1) stringToPrint + = "_U1";
  if(!doU1) stringToPrint + = "_U2";
  
  TLatex latexLabel;
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  latexLabel.DrawLatex(0.25, 0.2, stringToPrint.Data());
  
  TString fileToPrint="residual";
  fileToPrint + = stringToPrint.Data();
  TString fileToSave = fileToPrint.Data();
  fileToSave + = ".png";
  fileToPrint + = ".pdf";
  
  c1->Print(fileToPrint.Data());
  c1->SaveAs(fileToSave.Data());
  
}

void bosonPTratio()
{

  TString fileNameDATA8 = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_DATA_pol3_rapBin1_runD_8TeV.root";
  TString fileNameDATA8C = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_DATA_pol3_rapBin1_runC_8TeV.root";
  TString fileNameDATA7 = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_DATA_pol3_rapBin1_7TeV.root";

  TString fileNameMC8 = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_MC_MADGRAPH_pol3_PDF-1_rapBin1_8TeV.root";
  TString fileNameMC7 = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_MC_MADGRAPH_pol3_PDF-1_rapBin1_7TeV.root";

  TString fileNameMCpow7 = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_MC_POWHEG_pol3_PDF-1_rapBin1_7TeV.root";

  double rebin=1;

  TH1 *MC_ptMC8 = getHisto(fileNameMC8,"0","hZpt",1,1, rebin);
  TH1 *MC_ptMC7 = getHisto(fileNameMC7,"0","hZpt",2,1, rebin);
  TH1 *MC_ptMCpow7 = getHisto(fileNameMCpow7,"0","hZpt",3,1, rebin);

  TH1 *MC_ptDATA8 = getHisto(fileNameDATA8,"0","hZpt",4,1,rebin);
  TH1 *MC_ptDATA8C = getHisto(fileNameDATA8C,"0","hZpt",6,1, rebin);
  MC_ptDATA8->Add(MC_ptDATA8C);
  TH1 *MC_ptDATA7 = getHisto(fileNameDATA7,"0","hZpt",7,1,rebin);

  MC_ptMC8->DrawNormalized("hist");
  MC_ptMC7->DrawNormalized("hist same");
  MC_ptMCpow7->DrawNormalized("hist same");

  MC_ptDATA7->DrawNormalized("hist same");
  MC_ptDATA8->DrawNormalized("hist same");

  if(MC_ptMC7) plotLeg("MC (7TeV)", 1., MC_ptMC7, 0.45, 0.80, "l");
  if(MC_ptMC8) plotLeg("MC (8TeV)", 1., MC_ptMC8, 0.45, 0.75, "l");
  if(MC_ptMCpow7) plotLeg("POW (7TeV)", 1., MC_ptMCpow7, 0.45, 0.70, "l");

  if(MC_ptDATA7) plotLeg("DATA (7TeV)", 1., MC_ptDATA7, 0.45, 0.65, "l");
  if(MC_ptDATA8) plotLeg("DATA (8TeV)", 1., MC_ptDATA8, 0.45, 0.60, "l");

  /*
  MC_ptMC->Scale(1./MC_ptMC->Integral());
  MC_ptDATA->Scale(1./MC_ptDATA->Integral());
 
  MC_ptMC->Divide(MC_ptDATA);
 
  MC_ptMC->Draw("hist");
  */

  //Minimizer is Linear
  //Chi2                      =     0.547781
  //NDf                       =          448
  //p0                        =     0.928271   +/-   0.0931938   
  //p1                        =   0.00525191   +/-   0.00368778  


}


void CompareMT()
{

  gStyle->SetOptStat(0000000);
  double rebin=1;
  TString fileNameMC = "histo_WplusMCpowheg_53X.root";
  TString dirName="0";

  TH1 *MC_genPFMET = getHisto(fileNameMC,dirName,"h_mt_genPFMET",1,1, rebin);
  TH1 *MC_pfMET = getHisto(fileNameMC,dirName,"h_mt_pfMET",4,1,rebin);

  TH1 *MC_genTkMET = getHisto(fileNameMC,dirName,"h_mt_genTkMET",2,1, rebin);
  TH1 *MC_pfTKMETall = getHisto(fileNameMC,dirName,"h_mt_pfTKMETall",kGreen+1,1,rebin);
  TH1 *MC_pfTKMET = getHisto(fileNameMC,dirName,"h_mt_pfTKMET",kMagenta,1, rebin);

  if(MC_genPFMET) MC_genPFMET->GetYaxis()->SetTitle("A.U.");
  if(MC_genPFMET) MC_genPFMET->GetXaxis()->SetTitle("M_{T} W^{+}");

  if(MC_genPFMET) MC_genPFMET->GetXaxis()->SetRangeUser(30.,100.);
  if(MC_genPFMET) MC_genPFMET->SetTitle("");


  TCanvas * c1 = new TCanvas("c1","c1",700,700);
  c1->Draw();
  c1->cd();

  if(MC_genPFMET) MC_genPFMET->DrawNormalized("hist");
  if(MC_pfMET) MC_pfMET->DrawNormalized("hist same");

  if(MC_genTkMET) MC_genTkMET->DrawNormalized("hist same");
  if(MC_pfTKMETall) MC_pfTKMETall->DrawNormalized("hist same");
  if(MC_pfTKMET) MC_pfTKMET->DrawNormalized("hist same");

  if(MC_genPFMET) plotLeg("gen PFMET",1.,MC_genPFMET,0.2,0.80,"l");
  if(MC_pfMET) plotLeg("reco PFMET",1.,MC_pfMET, 0.2,0.75,"l");

  if(MC_genTkMET) plotLeg("gen TKMET",1.,MC_genTkMET,0.2,0.70,"l");
  if(MC_pfTKMETall) plotLeg("reco TKMET all",1., MC_pfTKMETall, 0.2,0.65,"l");
  if(MC_pfTKMET) plotLeg("reco TKMET dz<0.1",1., MC_pfTKMET, 0.2,0.60,"l");

  TLatex latexLabel;

  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  latexLabel.DrawLatex(0.25, 0.5, "7TeV");
  latexLabel.DrawLatex(0.25, 0.45, "powheg 53X");

  c1->Print("MT_defMET.pdf");
  c1->SaveAs("MT_defMET.png");

}

double getTail(TH1* h, int Nsigma) {

  //  double mean=h->GetMean();
  //  double rms=h->GetRMS();

  int binNPlusSigma= h->FindBin(h->GetMean() + Nsigma*h->GetRMS());
  int binNMinusSigma= h->FindBin(h->GetMean() - Nsigma*h->GetRMS());

  double fractionTail = 100 * ( h->Integral()- h->Integral(binNMinusSigma,binNPlusSigma ) ) / h->Integral();

  cout << "fractionTail " << fractionTail << endl;

  return fractionTail;

}

double getFit(TH1* h, bool getRMS) {

  double minf=-50.0, maxf=50.;
  double minf2=-100.0, maxf2=100.;

  TF1 *gfit = new TF1("Gaussian","gaus",minf,maxf); // Create the fit function                                                                                                                       
  TF1 *g2fit = new TF1("DiGaussian","gaus(0)+gaus(3)",minf2,maxf2); // Create the fit function                                                                                                       

  TF1 *g1    = new TF1("g1","gaus",h->GetMean(),maxf2);
  TF1 *g2    = new TF1("g2","gaus",minf2,h->GetMean());
  TF1 *gasym = new TF1("agauss","gaus(0)+gaus(3)",minf2,maxf2);

  //  TF1 *gasym = new TF1("AsymGaus",AsymGaus,1.0,2.0,4);                                                                                                                                           
  //  gasym->SetParNames("Constant","Mean","Sigma_g","Sigma_d");                                                                                                                                     

  // norm                                                                                                                                                                                            
  //  g2fit->SetParameter(0,0.9*0.1);                                                                                                                                                                
  g2fit->SetParameter(1,h->GetMean());
  g2fit->SetParameter(2,h->GetRMS()*0.5);
  // norm                                                                                                                                                                                            
  //  g2fit->SetParameter(3,0.1*0.1);                                                                                                                                                                
  //  g2fit->SetParameter(4,h->GetMean());                                                                                                                                                           
  //  g2fit->SetParameter(4,h->GetMean());                                                                                                                                                           
  g2fit->FixParameter(4,g2fit->GetParameter(1));
  g2fit->SetParameter(5,g2fit->GetParameter(2)*2.);

  Double_t par[6];

  g1->SetParameter(1,h->GetMean());
  g2->FixParameter(1,g1->GetParameter(1));
  h->Fit(g1,"RQ");
  h->Fit(g2,"RQ+");
  g1->GetParameters(&par[0]);
  g2->GetParameters(&par[3]);
  // Use the parameters on the sum                                                                                                                                                                   
  gasym->SetParameters(par);
  h->Fit(gasym,"RQ");

  h->Fit(gfit,"RQ+");
  h->Fit(g2fit,"RQ+");


  //  TF1 *gfit = new TF1("DoubleGaussian","gaus+gaus",minf,maxf); // Create the fit function                                                                                                        
  //  h->Fit(gfit,"RQ");                                                                                                                                                                             

  double chisq=gfit->GetChisquare();
  double ndf=gfit->GetNDF();
  double chisqdf=chisq/ndf;

  double chisq2=g2fit->GetChisquare();
  double ndf2=g2fit->GetNDF();
  double chisqdf2=chisq2/ndf2;

  double chisqAsym=gasym->GetChisquare();
  double ndfAsym=gasym->GetNDF();
  double chisqdfAsym=chisqAsym/ndfAsym;

  if(debug)  cout << "Chisquare: " << chisq << "/" << ndf << " : " << chisqdf << endl;

  //  Double_t amp = gfit->GetParameter(0); //value of 0th parameter                                                                                                                                     
  //  Double_t eamp = gfit->GetParError(0); //error on 0th parameter                                                                                                                                     
  Double_t mean = gfit->GetParameter(1); //value of 1st parameter                                                                                                                                    
  Double_t emean = gfit->GetParError(1); //error on 1st parameter                                                                                                                                    
  Double_t rms = gfit->GetParameter(2); //value of 1st parameter                                                                                                                                     
  Double_t erms = gfit->GetParError(2); //error on 1st parameter                                                                                                                                     

  if(debug) cout << "Fit: mean " << mean << " +- " << emean << "; rms"  << rms << " +- " << erms << endl;
  if(debug) cout << "h: mean " << h->GetMean() << " +- " << h->GetMeanError() << "; rms"  << h->GetRMS() << " +- " << h->GetRMSError() << endl;

  g2fit->SetLineColor(kRed);
  g2fit->SetLineWidth(5);
  g2fit->SetLineStyle(1);
  g2fit->Draw("sames");

  //  gfit->SetLineColor(kBlue);                                                                                                                                                                     
  gfit->SetLineColor(kBlack);
  gfit->SetLineWidth(5);
  gfit->SetLineStyle(1);
  gfit->Draw("sames");

  gasym->SetLineColor(kMagenta);
  gasym->SetLineWidth(5);
  gasym->SetLineStyle(1);
  gasym->Draw("sames");

  /*                                                                                                                                                                                                 
  TString meanLeg = "mean=";                                                                                                                                                                         
  meanLeg += round_fn(mean,2);                                                                                                                                                                       
  TString RMSLeg = "rms=";                                                                                                                                                                           
  RMSLeg += round_fn(mean,2);                                                                                                                                                                        
  */
  TString chi2Leg = "gauss #chi^{2}/ndof=";
  chi2Leg += round_fn(chisqdf,2);
  TString chi2Leg2 = "di-gauss #chi^{2}/ndof=";
  chi2Leg2 += round_fn(chisqdf2,2);
  TString chi2LegA = "gauss-Asym #chi^{2}/ndof=";
  chi2LegA += round_fn(chisqdfAsym,2);

  TLatex latexLabel;

  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  //  latexLabel.DrawLatex(0.65, 0.55, meanLeg.Data());                                                                                                                                              
  //  latexLabel.DrawLatex(0.65, 0.50, RMSLeg.Data());                                                                                                                                               
  latexLabel.DrawLatex(0.65, 0.5, chi2Leg2.Data());
  latexLabel.DrawLatex(0.65, 0.45, chi2Leg.Data());
  latexLabel.DrawLatex(0.65, 0.4, chi2LegA.Data());

  if(!getRMS) return mean;
  if(getRMS) return rms;
  return -9999.;

}

void drawRatio(const char * histoName) {

  //h_u2_Zpt810_tk

  TString dirName = "0";
  double rebin=1;
  TString histo = histoName;
  
  TString fileNameDATA  = "JobOutputs/test_7TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameMCmad = "JobOutputs/test_7TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";
  TString fileNameMCpow = "JobOutputs/test_7TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";  

  TH1 *data = getHisto(fileNameDATA,dirName,histo,1,1,rebin);
  TH1 *MC_raw = getHisto(fileNameMCmad,dirName,histo,2,1,rebin);
  TH1 *MC_pow = getHisto(fileNameMCpow,dirName,histo,4,1,rebin);
  
  TH1 * normdata=data->Clone();
  TH1 * normMC_raw=MC_raw->Clone();
  TH1 * normMC_pow=MC_pow->Clone();

  TCanvas * c1 = new TCanvas("c1","c1",700,700);
  c1->Draw();
  c1->cd();

  TPad * plotpad=new TPad("plotpad","plotpad",0,0,1,0.8);
  //  if(histoName->Contains("pttrk")) plotpad->SetLogy(1);
                                                                                                                                               
  //  if(doLog) plotpad->SetLogy(1);
  plotpad->Draw();
  plotpad->cd();

  normdata->DrawNormalized("hist");
  normMC_raw->DrawNormalized("hist same");
  normMC_pow->DrawNormalized("hist same");

  c1->cd();
  TPad *respad = new TPad("respad","respad",0,0.8,1,1);
  respad->Draw();
  respad->cd();
  gPad->SetGridx(1); 

  TH1 * hdata1 = normdata->Clone();
  TH1 * hdata2 = normdata->Clone();
  TH1 * hMC1   = normMC_raw->Clone();
  TH1 * hMC2   = normMC_pow->Clone();

  hdata1->Rebin(4);
  hdata2->Rebin(4);
  hMC1->Rebin(4);
  hMC2->Rebin(4);

  if(hdata1) hdata1->Scale(1./hdata1->Integral());
  if(hdata2) hdata2->Scale(1./hdata2->Integral());
  if(hMC1) hMC1->Scale(1./hMC1->Integral());
  if(hMC2) hMC2->Scale(1./hMC2->Integral());

  hMC1->Scale(-1);
  hMC2->Scale(-1);
  hdata1->Add(hMC1);
  hdata2->Add(hMC2);
  hMC1->Scale(-1);
  hMC2->Scale(-1);
  hdata1->Divide(hMC1);
  hdata2->Divide(hMC2);

  hdata1->SetLineColor(MC_raw->GetLineColor());
  hdata2->SetLineColor(MC_pow->GetLineColor());

  hdata1->GetXaxis()->SetTitleSize(0.1);
  hdata1->GetYaxis()->SetTitleSize(0.2);
  hdata1->GetYaxis()->SetTitleOffset(0.4);
  hdata1->GetXaxis()->SetTitleOffset(0.5);

  //  hdata1->GetYaxis()->SetTitle("data/MC");
  hdata1->GetYaxis()->SetTitle("(data-MC)/MC");
  hdata1->GetYaxis()->SetLabelSize(0.1);
  hdata1->GetXaxis()->SetLabelSize(0.1);

  //  hdata1->GetYaxis()->SetRangeUser(-1.,1.);
  hdata1->Draw("hist p e");
  hdata2->Draw("hist p e same");

  plotLeg("data/madgraph",1., hdata1, 0.7, 0.80-1*0.05, "l");
  plotLeg("data/powheg",1., hdata2, 0.7, 0.80-4*0.05, "l");

  hdata1->GetYaxis()->SetRangeUser(-1.,1.);

  TLine *line = new TLine(-100.,0,100.,0);
  line->SetLineColor(11);
  line->Draw();

}


// this does:
// the PF vs TK met vs nVTX
// the 7TeV vs 8 TeV

TPad * testClosureRMS(const char * histoName, int rebin, bool doLog, int type, bool doZPt=true, bool doRMS=true) {

  gStyle->SetFillStyle(0);
  gStyle->SetGridColor(kGray+1);

  //  bool doZPt=false;
  //  bool doFit = false; 
  //  bool dotk = false;

  bool doFit = false;
  bool dotk = true;
  bool print78 = true;
  bool printPF = false;
  bool printPT = false;

  bool doTail =false;

  /////
  /////
  /////

  TString fileNameDATA_8TeV_A  = "JobOutputs/test_8TeV_runA_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameDATA_8TeV_B  = "JobOutputs/test_8TeV_runB_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameDATA_8TeV_C  = "JobOutputs/test_8TeV_runC_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameDATA_8TeV_D  = "JobOutputs/test_8TeV_runD_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";

  TString fileNameDATA_8TeV  = "JobOutputs/test_8TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameDATA  = "JobOutputs/test_7TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";

//  TString fileNameDATA  = "JobOutputs/test_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameMC    = "JobOutputs/test_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCmad = "JobOutputs/test_7TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";
  TString fileNameMCmad_8TeV = "JobOutputs/test_8TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";

  TString fileNameDATA_pf  = "JobOutputs/test_pfmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root";
  TString fileNameMC_pf    = "JobOutputs/test_pfmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";

  TString fileNameMCcorr    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCcorrScaleUp    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_Scale_U1_1_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCcorrScaleDWN    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_Scale_U1_-1_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCcorrU1Up    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_Resol_U1_1_U2_0_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCcorrU1DWN    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_Resol_U1_-1_U2_0_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCcorrU2Up    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_Resol_U1_0_U2_1_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";
  TString fileNameMCcorrU2DWN    = "JobOutputs/test_tkmet_RochCorr_RecoilCorr_Resol_U1_0_U2_-1_EffHeinerSFCorr_PileupSFCorr/output_DYJetsPow/ZanalysisOnDATA.root";

  TString fileNameMC_pt0    = "JobOutputs/test_8TeV_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";
  TString fileNameMC_pt050  = "JobOutputs/test_8TeV_tkmet050_tkmet050_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";
  TString fileNameMC_pt075  = "JobOutputs/test_8TeV_tkmet075_tkmet075_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";
  TString fileNameMC_pt100  = "JobOutputs/test_8TeV_tkmet100_tkmet100_RochCorr_EffHeinerSFCorr_PileupSFCorr/output_DYJetsMadSig/ZanalysisOnDATA.root";

  /////
  /////
  /////

  TString dirName = "0";
  TString histo=histoName;

  cout << " out of the loop " << endl;

  /// DATA
  TGraph * gr_data = getGraph(fileNameDATA, dirName, histo, kBlack, kOpenCircle, rebin, doZPt, doRMS);
  TGraph * gr_data_8TeV = getGraph(fileNameDATA_8TeV, dirName, histo, kBlue, kOpenTriangleUp, rebin, doZPt, doRMS);

  TGraph * gr_data_8TeV_A = getGraph(fileNameDATA_8TeV_A, dirName, histo, kBlue+1, kOpenTriangleUp, rebin, doZPt, doRMS);
  TGraph * gr_data_8TeV_B = getGraph(fileNameDATA_8TeV_B, dirName, histo, kBlue+2, kOpenTriangleUp, rebin, doZPt, doRMS);
  TGraph * gr_data_8TeV_C = getGraph(fileNameDATA_8TeV_C, dirName, histo, kBlue+3, kOpenTriangleUp, rebin, doZPt, doRMS);
  TGraph * gr_data_8TeV_D = getGraph(fileNameDATA_8TeV_D, dirName, histo, kBlue+4, kOpenTriangleUp, rebin, doZPt, doRMS);

  // MC central value
  TGraph * gr_mc_raw = getGraph(fileNameMC, dirName, histo, kRed, kFullCircle, rebin, doZPt, doRMS);
  TGraph * gr_mc_mad_raw = getGraph(fileNameMCmad, dirName, histoName, 1, kFullCircle, rebin, doZPt, doRMS);
  TGraph * gr_mc_mad_raw_8TeV = getGraph(fileNameMCmad_8TeV, dirName, histoName, kBlue, kFullTriangleUp, rebin, doZPt,doRMS);

  //pfmet 7TeV (full Open square)
  TGraph * gr_data_pf = getGraph(fileNameDATA_pf, dirName, histo, kBlack, kOpenSquare, rebin, doZPt, doRMS);
  TGraph * gr_mc_raw_pf = getGraph(fileNameMC_pf, dirName, histo, kMagenta+2, kFullSquare, rebin, doZPt, doRMS);

  // MC powheg corrected
  TGraph * gr_mc_corr = getGraph(fileNameMCcorr,dirName,histo,kGreen+1,kFullCircle,rebin, doZPt, doRMS);
  TGraph * gr_mc_corrPlus = getGraph(fileNameMCcorrScaleUp,dirName,histo,kGreen+3,2,kFullCircle,rebin, doZPt, doRMS);
  TGraph * gr_mc_corrMinus = getGraph(fileNameMCcorrScaleDWN,dirName,histo,kGreen-5,3,kFullCircle,rebin, doZPt, doRMS);
  
  // MC madgraph corrected
  TGraph * gr_mc_corr_mad = getGraph(fileNameMCmad,dirName,histo,kGreen+1,1,rebin, doZPt, doRMS);
  TGraph * gr_mc_corrPlus_mad = getGraph(fileNameMCmad,dirName,histo,kGreen+3,2,rebin, doZPt, doRMS);
  TGraph * gr_mc_corrMinus_mad = getGraph(fileNameMCmad,dirName,histo,kGreen-5,3,rebin, doZPt, doRMS);

  /*
  if(!print78) {

    // powheg
    TGraph * gr_mc_raw = makeGraph(binMax,x, RMS_mc_raw, MC_raw, kFullCircle);
    TGraph * gr_mc_corr = makeGraph(binMax,x, RMS_mc_corr, MC_corr, kFullCircle);
    TGraph * gr_mc_corrPlus = makeGraph(binMax,x, RMS_mc_corrPlus, MC_corrPlus, 2);
    TGraph * gr_mc_corrMinus = makeGraph(binMax,x, RMS_mc_corrMinus, MC_corrMinus, 5);
    
    // madgraph
    TGraph * gr_mc_raw_mad = makeGraph(binMax,x,RMS_mc_raw_mad, MC_raw_mad, 20);
    
    //tkmet mad 8TeV
    TGraph * gr_mad_pt0 = makeGraph(binMax,x,RMS_mad_pt0,MC_mad_pt0,kFullCircle);
    TGraph * gr_mad_pt050 = makeGraph(binMax,x,RMS_mad_pt050,MC_mad_pt050,kFullCircle);
    TGraph * gr_mad_pt100 = makeGraph(binMax,x,RMS_mad_pt100,MC_mad_pt100,kFullCircle);
    TGraph * gr_mad_pt075 = makeGraph(binMax,x,RMS_mad_pt075,MC_mad_pt075,kFullCircle);
  }
  */

  //////
  //////
  //////

  TString titleY = "";
  if(doRMS) titleY = "RMS";
  if(!doRMS) titleY = "MEAN";

  if(doFit) titleY += " - gauss Fit -";
  if(!doFit) titleY += " - histo - ";

  if(histo.Contains("h_met")) titleY += " met";
  if(histo.Contains("h_u2")) titleY += " u2";
  if(histo.Contains("h_u1") && !histo.Contains("h_u1_Residual")) titleY += " u1";
  if(histo.Contains("h_u1_Residual")) titleY += " u1+Zpt";

  gr_data_8TeV->GetYaxis()->SetTitle(titleY);
  if(doZPt) gr_data_8TeV->GetXaxis()->SetTitle("Z pt");
  if(!doZPt) gr_data_8TeV->GetXaxis()->SetTitle("Num vtx");
  gr_data_8TeV->SetTitle("");

  double maxRMS=15.;
  double minRMS=0.;
  if(print78) maxRMS=9.;
  if(print78) minRMS=3.;

  double maxScale=15.;
  double minScale=5.0;

  // this is for PFmet
  if(dotk && printPF ) maxRMS=30.;
  if(dotk && printPF ) maxScale=5.;
  if(dotk && printPF ) minScale=-5.;

  if(doRMS && doZPt) gr_data_8TeV->SetMaximum(15.);
  if(doRMS && doZPt) gr_data_8TeV->SetMinimum(0.);
  if(!doRMS && doZPt) gr_data_8TeV->SetMaximum(maxScale);
  if(!doRMS && doZPt) gr_data_8TeV->SetMinimum(minScale);

  if(!doZPt && doRMS) gr_data_8TeV->SetMaximum(maxRMS);
  if(!doZPt && doRMS) gr_data_8TeV->SetMinimum(minRMS);
  if(!doZPt && !doRMS) gr_data_8TeV->SetMaximum(maxScale);
  if(!doZPt && !doRMS) gr_data_8TeV->SetMinimum(minScale);

  /////
  /////

  TCanvas * c = new TCanvas("c","c",700,700);
  c->cd();
  c->SetGridy(1);


  float doYPos=0.35;
  if(printPF) doYPos=0.8;

  TLatex latexLabel;
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();

  if(printPT) {

    MC_mad_pt0->Draw("A P");
    MC_mad_pt050->Draw("P same");
    MC_mad_pt075->Draw("P same");
    MC_mad_pt100->Draw("P same");
 
  } else  if(!print78) {

    gr_data->Draw("A P");
    if(gr_mc_raw) gr_mc_raw->Draw("P same");
    
    if(printPF && gr_mc_raw_pf) gr_mc_raw_pf->Draw(" P  same");
    if(printPF && gr_data_pf)  gr_data_pf->Draw(" P same");
    
    if(printPF && gr_data_pf) plotLegMarker("PFmet data",1.,gr_data_pf,0.5,doYPos-0*0.05,"p");
    if(printPF && gr_mc_raw_pf) plotLegMarker("PFmet DY powheg",1., gr_mc_raw_pf, 0.5,doYPos-1*0.05,"p");
    
    if(gr_data) plotLegMarker("TKmet  data",1.,gr_data,0.5,doYPos-2*0.05,"p");
    if(gr_mc_raw) plotLegMarker("TKmet DY powheg",1., gr_mc_raw, 0.5,doYPos-3*0.05,"p");

    if(printPF && gr_data_pf) plotLegMarker("PFmet data",1.,gr_data_pf,0.5,doYPos-0*0.05,"p");
    if(printPF && gr_mc_raw_pf) plotLegMarker("PFmet DY powheg",1., gr_mc_raw_pf, 0.5,doYPos-1*0.05,"p");
    
    if(!printPF) {

      gr_mc_corr->Draw(" P same");
      gr_mc_corrPlus->Draw(" L P same");
      gr_mc_corrMinus->Draw(" L P same");
      if(gr_mc_corr) plotLegMarker("DY MC corr",1., gr_mc_corr, 0.5,doYPos-4*0.05,"p");
      if(gr_mc_corrPlus) plotLegMarker("DY MC corr",1., gr_mc_corrPlus, 0.5,doYPos-4*0.05,"p");
      if(gr_mc_corrMinus) plotLegMarker("DY MC corr",1., gr_mc_corrMinus, 0.5,doYPos-4*0.05,"p");
    }

    gr_data->Draw("P same");

    if(dotk && printPF) latexLabel.DrawLatex(0.2, 0.65, "7TeV");
    //  if(dotk && printPF) latexLabel.DrawLatex(0.2, 0.6, "powheg 53X");
    if(dotk && printPF) latexLabel.DrawLatex(0.2, 0.6, "53X");

    if(dotk && !printPF) latexLabel.DrawLatex(0.2, 0.7, "TKmet");
    if(dotk && !printPF) latexLabel.DrawLatex(0.2, 0.7, "TKmet");

  } else {

    if(gr_data_8TeV) gr_data_8TeV->Draw("A P");
    if(gr_data) gr_data->Draw("P same");

    if(gr_mc_mad_raw) gr_mc_mad_raw->Draw("P same");
    if(gr_mc_mad_raw_8TeV) gr_mc_mad_raw_8TeV->Draw("P same");

    if(gr_data) plotLegMarker("data - 7TeV",1.,gr_data,0.5,doYPos-2*0.05,"p");
    if(gr_data_8TeV) plotLegMarker("data - 8TeV",1., gr_data_8TeV, 0.5,doYPos-3*0.05,"p");

    if(gr_mc_mad_raw) plotLegMarker("DY mad - 7TeV",1.,gr_mc_mad_raw,0.5,doYPos-4*0.05,"p");
    if(gr_mc_mad_raw_8TeV) plotLegMarker("DY mad - 8TeV",1., gr_mc_mad_raw_8TeV, 0.5,doYPos-5*0.05,"p");

    /*
      gr_data_8TeV_A->Draw("L P same");
      gr_data_8TeV_B->Draw("L P same");
      gr_data_8TeV_C->Draw("L P same");
      gr_data_8TeV_D->Draw("L P same");
      
      if(gr_data_8TeV_A) plotLegMarker("data - 8TeV runA",1., gr_data_8TeV_A, 0.5,doYPos-4*0.05,"p");
      if(gr_data_8TeV_B) plotLegMarker("data - 8TeV runB",1., gr_data_8TeV_B, 0.5,doYPos-5*0.05,"p");
      if(gr_data_8TeV_C) plotLegMarker("data - 8TeV runC",1., gr_data_8TeV_C, 0.5,doYPos-6*0.05,"p");
      if(gr_data_8TeV_D) plotLegMarker("data - 8TeV runD",1., gr_data_8TeV_D, 0.5,doYPos-7*0.05,"p");
    */
    
    if(dotk && !printPF) latexLabel.DrawLatex(0.2, 0.7, "TKmet");
    if(dotk && printPF) latexLabel.DrawLatex(0.2, 0.6, "53X");
    
  }

  ////// 
  //////          
  //////
  ////// 
  //////          
  //////

  if(doRMS && doZPt) drawLine(20, 10, 20);
  if(doRMS && doZPt) drawLine(22, 10, 20);
  if(doRMS && doZPt) drawLine(24, 10, 20);

  if(!doRMS && doZPt) drawLine(20, -3, 3);
  if(!doRMS && doZPt) drawLine(22, -3, 3);
  if(!doRMS && doZPt) drawLine(24, -3, 3);

  /*
  TString corrType="corr type ";
  corrType += type;
  // MARIA just for now 
  latexLabel.DrawLatex(0.2, 0.8, corrType.Data());
  */                                                                                                                                                                             


  c->Update();
  //  c->Print("testClosure.pdf"); 
                                                                                                                                                                  
  c->Print("/tmp/dalfonso/testClosure.pdf");
  c->Print("/tmp/dalfonso/testClosure_summary.pdf");

  TString file="";

  if(histo.Contains("h_met")) file="PLOTclosure/met";
  if(histo.Contains("h_u2")) file="PLOTclosure/u2";
  if(histo.Contains("h_u1_Residual")) file="PLOTclosure/u1";

  if(doRMS) file+="_RMS";
  if(!doRMS) file+="_MEAN";

  if(doZPt) file+="_Zpt";
  if(!doZPt) file+="_vtx";

  if(printPF) {
    file+="_TKPFcomp.pdf";
  } else {
    file+=".pdf";
  }
  c->Print(file);

  /* 
    if(histo.Contains("h_u2")) c1->Print("u2.pdf"); 
    if(histo.Contains("h_u1") && !histo.Contains("h_u1_Residual")) c1->Print("u1.pdf");  
    if(histo.Contains("h_u1_Residual")) c1->Print("u1_Residual.pdf");     
  */

  return c;

}


void printClosure() {

  TCanvas * ca = new TCanvas("ca","ca",800,800);
  ca->Print("/tmp/dalfonso/testClosure_summary.pdf(");
  ca->Print("/tmp/dalfonso/testClosure_summary.pdf");
  ca->Print("/tmp/dalfonso/testClosure_summary.pdf");

  TCanvas * c2 = new TCanvas("c2","c2",800,800);
  c2->Print("/tmp/dalfonso/testClosure.pdf(");
  c2->Print("/tmp/dalfonso/testClosure.pdf");
  c2->Print("/tmp/dalfonso/testClosure.pdf");


  /*
  //// MET 
                                                                                                                                                                                          
  TPad * c1_met_RMS_pt = testClosureRMS("h_met", 1, true, 1, true, true);
  TPad * c1_met_MEAN_pt = testClosureRMS("h_met", 1, true, 1, true, false); 
  */

  TPad * c1_met_RMS_vtx = testClosureRMS("h_met", 1, true, 1, false, true);
  //  TPad * c1_met_MEAN_vtx = testClosureRMS("h_met", 1, true, 1, false, false);

  //// u2 
  TPad * c1_u2_RMS_pt = testClosureRMS("h_u2", 1, true, 1, true, true);
  TPad * c1_u2_MEAN_pt = testClosureRMS("h_u2", 1, true, 1, true, false);
  TPad * c1_u2_RMS_vtx = testClosureRMS("h_u2", 1, true, 1, false, true);
  TPad * c1_u2_MEAN_vtx = testClosureRMS("h_u2", 1, true, 1, false, false);

  //// u1
  TPad * c1_u1_RMS_pt = testClosureRMS("h_u1_Residual", 1, true, 1, true, true);
  TPad * c1_u1_MEAN_pt = testClosureRMS("h_u1_Residual", 1, true, 1, true, false);
  TPad * c1_u1_RMS_vtx = testClosureRMS("h_u1_Residual", 1, true, 1, false, true);
  TPad * c1_u1_MEAN_vtx = testClosureRMS("h_u1_Residual", 1, true, 1, false, false);

  c2->Print("/tmp/dalfonso/testClosure.pdf)");

  ca->Print("/tmp/dalfonso/testClosure_summary.pdf)");

  cout << "CIAO " << endl;


}


void checkPDF()
{

  gStyle->SetOptStat(0000000);
  
  TString dirName = "0";

  ofstream myfile;
  myfile.open("example.txt");
  
  double rebin=1;

  TCanvas * c1 = new TCanvas("c1","c1", 1200, 800);
  c1->Divide(3,2);
  c1->Draw();
  
  for(int jGEN=0; jGEN<=1; jGEN++) {

    bool doMad=true;
    if(jGEN==0) doMad=false;
    
    if(doMad) myfile << "---- MADGRAPH" << endl;
    if(!doMad) myfile << "---- POWHEG" << endl;
    
    for(int kBos=1; kBos<=3; kBos++) {
      
      myfile << "--------------" << endl;
      for(int iBin=0; iBin<=5; iBin++) {
	
	TString fileNameMC_Z  = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_MC";
	TString fileNameMC_Wp = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_WposMC";
	TString fileNameMC_Wm = "RecoilCode/RecoilCorrector_v6/fitcode/fileHisto_WnegMC";
	
	if(doMad) fileNameMC_Z  += "_MADGRAPH_pol3_PDF";
	if(doMad)  fileNameMC_Wp  += "_MADGRAPH_pol3_PDF";
	if(doMad) fileNameMC_Wm  += "_MADGRAPH_pol3_PDF";
	
	if(!doMad) fileNameMC_Z  += "_POWHEG_pol3_PDF";
	if(!doMad)  fileNameMC_Wp  += "_POWHEG_pol3_PDF";
	if(!doMad) fileNameMC_Wm  += "_POWHEG_pol3_PDF";
	
	//	TString histo ="Vpt";
	//    TString histo ="Vy"; 
	TString histo ="sumEt"; 
	
	TString Binning="";
	
	if ( iBin==0 ) Binning = "0"; 
	else if ( iBin==1 ) Binning = "1";
	else if  ( iBin==2 ) Binning = "2";
	else if  ( iBin==3 ) Binning = "3";
	else if  ( iBin==4 ) Binning = "4";
	else if  ( iBin==5 ) Binning = "5";
	else if  ( iBin==6 ) Binning = "10";
	
	fileNameMC_Z += Binning.Data(); 
	fileNameMC_Wp += Binning.Data();  
	fileNameMC_Wm += Binning.Data(); 
	
	fileNameMC_Z += "_rapBin1.root";
	fileNameMC_Wp += "_rapBin1.root";
	fileNameMC_Wm += "_rapBin1.root";
	
	histo += Binning.Data();
	int color=iBin;
	if(iBin==0) color=11;
	if(iBin==10) color=20;
	TH1 *MC_Wp = getHisto(fileNameMC_Wp,dirName,histo,color,1,rebin);
	TH1 *MC_Z = getHisto(fileNameMC_Z,dirName,histo,color,1,rebin);
	TH1 *MC_Wm = getHisto(fileNameMC_Wm,dirName,histo,color,1,rebin);
	
	TH1 *h_=0;
	if(MC_Wp && kBos==1) h_=(TH1*) MC_Wp->Clone();
	if(MC_Wm && kBos==2) h_=(TH1*) MC_Wm->Clone();
	if(MC_Z  && kBos==3) h_=(TH1*) MC_Z->Clone();
	
	if(iBin==0 && h_) h_->SetMaximum(30000);
	if(iBin==0 && h_) h_->SetTitle("");
	if(iBin==0 && h_ && !histo.Contains("sumEt")) h_->GetXaxis()->SetRangeUser(0,45);
	if(iBin==0 && h_ && histo.Contains("sumEt")) h_->GetXaxis()->SetRangeUser(0,200);
	if(iBin==0 && h_) h_->GetXaxis()->SetTitle("Z p_{T}");
	if(iBin==0 && h_ && histo.Contains("sumEt")) h_->GetXaxis()->SetTitle("sum E_{T}");
	
	c1->cd(jGEN*3+kBos);
	if(iBin==0 && h_) h_->DrawNormalized("hist");
	if(iBin!=0 && h_) h_->DrawNormalized("hist same");
	
	//   if(MC_Z) MC_Z->Draw("hist e same");
	//   if(MC_Wm) MC_Wm->Draw("hist e same");
	
	if(h_) {

	  if(iBin==0) plotLeg("gluon",1.,h_,0.55,0.80-iBin*0.05,"l");
	  if(iBin==1 && kBos!=3) plotLeg("main valence",1.,h_,0.55,0.80-iBin*0.05,"l");
	  if(iBin==2 && kBos!=3) plotLeg("subleading valence",1.,h_,0.55,0.80-iBin*0.05,"l");
	  
	  if(iBin==1 && kBos==3) plotLeg("u ubar",1.,h_,0.55,0.80-iBin*0.05,"l");
	  if(iBin==2 && kBos==3) plotLeg("d dbar",1.,h_,0.55,0.80-iBin*0.05,"l");
	  if(iBin==3 && kBos==3) plotLeg("s sbar",1.,h_,0.55,0.80-iBin*0.05,"l");
	  if(iBin==4 && kBos==3) plotLeg("c cbar",1.,h_,0.55,0.80-iBin*0.05,"l");
	  //      if(iBin==5 && kBos==3) plotLeg("b bbar",1.,h_,0.55,0.80-iBin*0.05,"l");  
	}

	if(kBos==1 && h_) myfile << "Wplus: Bin: " << iBin << " " << h_->Integral() << endl;
	if(kBos==2 && h_) myfile << "Wmins: Bin: " << iBin << " " << h_->Integral() << endl;
	if(kBos==3 && h_) myfile << "Z: Bin: " << iBin << " " << h_->Integral() << endl;

	TLatex latexLabel;
	latexLabel.SetTextSize(0.06);
	latexLabel.SetNDC();
	if(doMad) latexLabel.DrawLatex(0.25, 0.85, "madgraph");
	if(!doMad) latexLabel.DrawLatex(0.25, 0.85, "powheg");

	if(iBin==1 && kBos==1) latexLabel.DrawLatex(0.25, 0.78, "W^{+}");
	if(iBin==1 && kBos==2) latexLabel.DrawLatex(0.25, 0.78, "W^{-}");
	if(iBin==1 && kBos==3) latexLabel.DrawLatex(0.25, 0.78, "Z");

	latexLabel.DrawLatex(0.25, 0.7, "|Y(V)|<1");

      }
    }

  }

  myfile.close();

}
