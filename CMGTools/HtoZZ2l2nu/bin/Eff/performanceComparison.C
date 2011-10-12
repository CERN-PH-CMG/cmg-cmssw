/**
   gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
 */

#include <iostream>


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"

using namespace std;

int colors[]  = {1,  kRed,     kGreen+3, kBlue, kBlue, kRed+3, kRed, kRed, kOrange};
int styles[]  = {1,  2,        9,        9,     1,     1,      1,    1,    2};
int markers[] = {20, 24,       21,       25,    22,    24,     20,   24,   25};



TObject* GetObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false)
{
   TObject* toReturn = NULL;

   size_t pos = Path.find("/");
   if(pos < std::string::npos){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());
      toReturn =  GetObjectFromPath(TMP,endPart,GetACopy);
   }else{
      toReturn = File->Get(Path.c_str());
   }

   if(!toReturn)printf("BUG: %s\n",Path.c_str());

   if(GetACopy){ toReturn = toReturn->Clone();  }
   return toReturn;
}



double GetCut(TH1 *hist, double Eff){
   double Integral = hist->Integral();
   for(int i=hist->GetXaxis()->GetNbins()+1;i>=0;i--){
     if(hist->Integral(i,hist->GetXaxis()->GetNbins()+1)/Integral  > Eff)return hist->GetXaxis()->GetBinLowEdge(i+1);
   }
   return -1;
}


double GetEfficiency(TH1 *hist, double Cut){
   double Integral = hist->Integral();
   return hist->Integral(hist->GetXaxis()->FindBin(Cut),hist->GetXaxis()->GetNbins()) / Integral;
}

double GetYields(TH1 *hist, double Cut){
   return hist->Integral(hist->GetXaxis()->FindBin(Cut),hist->GetXaxis()->GetNbins());
}

TGraph* getCutGraph(TH1* hist){
   TGraph* toReturn = new TGraph(hist->GetXaxis()->GetNbins());
   double Integral = hist->Integral();
   for(int i=1;i<hist->GetXaxis()->GetNbins();i++){
      toReturn->SetPoint(i-1,hist->Integral(i,hist->GetXaxis()->GetNbins()+1)/Integral, hist->GetXaxis()->GetBinLowEdge(i+1));      
   }   
   return toReturn;
}


//
TGraphAsymmErrors *getEfficiencyCurve(TH1 *signalProc, TH1 *bckgProc, bool Reverse=false)
{ 
  TGraphAsymmErrors *effGr = new TGraphAsymmErrors;
	      
  //compute efficiency
  const int nbins=bckgProc->GetXaxis()->GetNbins();
  int start = 1; int end=nbins; int step=1;
//  if(Reverse){start=nbins; end=0; step=-1;}

  double bckgTotalError(0), signalTotalError(0);
  double bckgTotal=bckgProc->IntegralAndError(1,nbins,bckgTotalError,"width");
  double signalTotal=signalProc->IntegralAndError(1,nbins,signalTotalError,"width");

  for(int xbin=start; xbin<=end; xbin+=step)
    {
      Double_t signalPassError(0), bckgPassError(0);
      float signalPass, bckgPass;     

      if(!Reverse){
         signalPass = signalProc->IntegralAndError(xbin,nbins,signalPassError,"width");
         bckgPass   = bckgProc->IntegralAndError(xbin,nbins,bckgPassError,"width"); 
      }else{
         signalPass = signalProc->IntegralAndError(1, xbin,signalPassError,"width");
         bckgPass   = bckgProc->IntegralAndError(1, xbin,bckgPassError,"width");  
      }
		  
      //efficiency curve
      double effb=float(bckgPass/bckgTotal);
      double effbError=sqrt(pow(bckgPassError*bckgTotal,2)+pow(bckgPass*bckgTotalError,2))/pow(bckgTotal,2);
      double effsig=float(signalPass/signalTotal);
      double effsigError=sqrt(pow(signalPassError*signalTotal,2)+pow(signalPass*signalTotalError,2))/pow(signalTotal,2);

      int Bin = xbin-1;
      effGr->SetPoint(Bin,effsig,effb);
      effGr->SetPointError(Bin,effsigError,effsigError,effbError,effbError);
    }

  return effGr;
}


TGraph* Inverse(TGraph* graph){
   TGraph* toReturn = new TGraph(graph->GetN());
   for(int i=0;i<graph->GetN();i++){
      double x, y;
      graph->GetPoint(i,x,y);
      toReturn->SetPoint(i,y,x);
   }
   return toReturn;
}


struct stArray{
      std::string Title;
      double      Cut;
      double      SEff;   double SEffError;
      double      BEff;   double BEffError;
      double      S;      double SError;
      double      B;      double BError;
      double      SsqrtSB;double SsqrtSBError;
      double      SB;     double SBError;
};

stArray  makeArray(string name, string title, string signal, string background, TFile* f, double* PUReweightAvr){
      string evcat = "mumu_met_";
      TH2D *signalProc= (TH2D*) GetObjectFromPath(f, signal    +"#rightarrow VV" + "/"+evcat+name+"vspu", true);    
      TH2D *bckgProc=NULL;
      char* BckgSamples = new char [background.size()+1];  strcpy (BckgSamples, background.c_str());
      char* BckgSubSample = strtok (BckgSamples,"|");
      while (BckgSubSample != NULL){
         if(!bckgProc){bckgProc =    (TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+name+"vspu", true);
         }else{        bckgProc->Add((TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+name+"vspu", true));}
         BckgSubSample = strtok (NULL, "|");
      }
      //inclusive efficiency
      int nxbins=bckgProc->GetXaxis()->GetNbins();

      TH1*signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",1,nxbins);
      fixExtremities(signalProj,true,true);
      TH1*bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",1,nxbins);
      fixExtremities(bckgProj,true,true);

      TGraph* cutGraph = getCutGraph(signalProj);
      double SeffCut = 1.0;
      if(signal=="H(150)") SeffCut = 0.200;
      if(signal=="H(200)") SeffCut = 0.204;
      if(signal=="H(300)") SeffCut = 0.633;
      if(signal=="H(500)") SeffCut = 0.714;
      double Cut = cutGraph->Eval(SeffCut);

      TH1* signalProjPU;
      TH1* bckgProjPU;
      if(PUReweightAvr){
         signalProjPU = (TH1*)signalProj->Clone("signalProjPU"); signalProjPU->Reset();
         bckgProjPU   = (TH1*)bckgProj  ->Clone("bckgProjPU");   bckgProjPU->Reset();
         for(int p=1;p<nxbins;p++){
             TH1* tmpsignalProj = ((TH2 *)signalProc)->ProjectionY("tmp_sig_py",p,p);         
             TH1* tmpbckgProj   = ((TH2 *)bckgProc)->ProjectionY("tmp_bckg_py",p,p);
             signalProjPU->Add(tmpsignalProj,PUReweightAvr[p-1]);
             bckgProjPU->Add(tmpbckgProj,PUReweightAvr[p-1]);
         }
         fixExtremities(signalProjPU,true,true);
         fixExtremities(bckgProjPU  ,true,true);
         SeffCut = GetEfficiency(signalProjPU,Cut);
      }else{
        signalProjPU = signalProj;
        bckgProjPU   = bckgProj;
      }

      TGraphAsymmErrors *gr = getEfficiencyCurve(signalProjPU, bckgProjPU, name.find("metL")!=std::string::npos );
      gr->SetName((name+"_eff").c_str());

      double S = SeffCut*signalProjPU->Integral();
      double SError; signalProjPU->IntegralAndError(signalProjPU->GetXaxis()->FindBin(Cut), signalProjPU->GetXaxis()->GetNbins(), SError);
      double SIntegral = signalProjPU->Integral();
      SError = std::min(S, SError);
      double B = gr->Eval(SeffCut)* bckgProjPU->Integral();
      double BIntegral = bckgProjPU->Integral();
      double BError; bckgProjPU->IntegralAndError(bckgProjPU->GetXaxis()->FindBin(Cut), bckgProjPU->GetXaxis()->GetNbins(), BError);
      BError = std::min(B, BError);
      double SBError = fabs((1/sqrt(S+B) - 1/(S+B))) * SError + (1/(S+B))*BError;


      stArray toReturn;
      toReturn.Title   = title;
      toReturn.Cut     = Cut;
      toReturn.SEff    = SeffCut;            toReturn.SEffError = SError/SIntegral;
      toReturn.BEff    = gr->Eval(SeffCut);  toReturn.BEffError=gr->Eval(SeffCut)/BIntegral;
      toReturn.S       = S;                  toReturn.SError = SError;
      toReturn.B       = B;                  toReturn.BError = BError;
      toReturn.SsqrtSB = S/sqrt(S+B );       toReturn.SsqrtSBError = SBError;
      toReturn.SB      = S/B;                toReturn.SBError = SError/B + S*BError/(B*B);

     //printf("$%35s$ & $\\geq$%iGeV & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ \\\\ \n",toReturn.Title.c_str(), (int)toReturn.Cut, toReturn.SEff, toReturn.SEffError, toReturn.S, toReturn.SError, toReturn.B, toReturn.BError,  toReturn.SsqrtSB, toReturn.SsqrtSBError, toReturn.B, toReturn.BError,  toReturn.SsqrtSB, toReturn.SsqrtSBError );


//      delete signalProj; delete bckgProj;
//      if(PUReweightAvr)delete signalProjPU; delete bckgProjPU;
//      delete gr;
      return toReturn;
}


void performanceSummary(string OutDir, string evcat, string signal, string background,  string fname, const std::vector<string>& names, const std::vector<TString>& titles){
   int colors[]  = {1,  kRed,     kGreen+3, kBlue, kBlue, kRed+3, kRed, kRed, kOrange};
   int styles[]  = {1,  2,        9,        9,     1,     1,      1,    1,    2};
   int markers[] = {20, 24,       21,       25,    22,    24,     20,   24,   25};


   double PUReweightAvr02[] = {13.6966,12.3621,4.88648,1.91332,0.723482,0.25842,0.0902911,0.0294196,0.00933525,0.00278426,0.000785822,0.00021111,5.19039e-05,1.21975e-05,2.69609e-06,5.59881e-07,1.11159e-07,2.14596e-08,3.86702e-09,6.61371e-10,1.14148e-10,1.86341e-11,3.14171e-12,4.83165e-13,4.43481e-14};
   double PUReweightAvr10[] = {0.00459492,0.0207362,0.0409827,0.0802348,0.151695,0.27092,0.473292,0.771065,1.22335,1.82433,2.57448,3.45815,4.25113,4.99512,5.52051,5.73206,5.69024,5.49257,4.94881,4.23194,3.65202,2.98087,2.51287,1.93228,0.886787};
   double PUReweightAvr14[] = {05,0.000534372,0.00147858,0.00405261,0.0107268,0.0268206,0.0655972,0.149615,0.332324,0.693815,1.37074,2.57775,4.43637,7.29789,11.2917,16.4141,22.8122,30.8276,38.8859,46.5543,56.2447,64.2716,75.8534,81.6587,52.4662};
   double* PUScenaraio[] = {NULL,PUReweightAvr14};//,PUReweightAvr02};
   int ntouse = names.size();

  TFile *f=TFile::Open(fname.c_str());

  TMultiGraph* mgraphS   = new TMultiGraph();
  TMultiGraph* mgraphDY  = new TMultiGraph();
  TMultiGraph* mgraphSDY = new TMultiGraph();
  TMultiGraph* mgraphB   = new TMultiGraph();
  TMultiGraph* mgraphSB  = new TMultiGraph();
  TLegend* leg  = new TLegend(0.05,0.93,0.40,0.70, "NDC");
  leg->SetHeader("");leg->SetFillColor(0); leg->SetFillStyle(0); leg->SetLineColor(0);

  for(int pS=0;pS<sizeof(PUScenaraio)/sizeof(double);pS++){
     TGraphErrors* graphS   = new TGraphErrors(ntouse);  graphS  ->SetLineColor(colors[pS]); graphS  ->SetMarkerColor(colors[pS]); graphS  ->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphDY  = new TGraphErrors(ntouse);  graphDY ->SetLineColor(colors[pS]); graphDY ->SetMarkerColor(colors[pS]); graphDY ->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphSDY = new TGraphErrors(ntouse);  graphSDY->SetLineColor(colors[pS]); graphSDY->SetMarkerColor(colors[pS]); graphSDY->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphB   = new TGraphErrors(ntouse);  graphB  ->SetLineColor(colors[pS]); graphB  ->SetMarkerColor(colors[pS]); graphB  ->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphSB  = new TGraphErrors(ntouse);  graphSB ->SetLineColor(colors[pS]); graphSB ->SetMarkerColor(colors[pS]); graphSB ->SetMarkerStyle(markers[pS]);

     printf("####################### PU SCENARIO %i  #######################\n",pS);
     for(int i=0; i<ntouse; i++){
        stArray DY = makeArray(names[i], titles[i].Data(), signal, background, f, PUScenaraio[pS]);
        stArray B  = makeArray(names[i], titles[i].Data(), signal, "ZZ|WW|WZ|Single top|t#bar{t}|W+jets|Z-#gamma^{*}+jets#rightarrow ll", f, PUScenaraio[pS]);
        printf("$%35s$ & $\\geq$%iGeV & $%6.2E\\pm%6.2E$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ \\\\ \n",DY.Title.c_str(), (int)DY.Cut, DY.SEff, DY.SEffError, DY.S, DY.SError, DY.B, DY.BError,  DY.SB, DY.SBError, B.B, B.BError,  B.SB, B.SBError );

        graphS  ->SetPoint(i, i, DY.S);     graphS  ->SetPointError(i, 0.0, 0.0); //DY.SError);
        graphDY ->SetPoint(i, i, DY.B);     graphDY ->SetPointError(i, 0.0, 0.0); //DY.BError);
        graphSDY->SetPoint(i, i, DY.SB);    graphSDY->SetPointError(i, 0.0, 0.0); //DY.SBError);
        graphB  ->SetPoint(i, i, B .B);     graphB  ->SetPointError(i, 0.0, 0.0); //B .BError);
        graphSB ->SetPoint(i, i, B .SB);    graphSB ->SetPointError(i, 0.0, 0.0); //B .SBError);
     }

     TH1* tmpHisto = new TH1D("tmp","tmp",1,0,1);  tmpHisto->SetLineColor(colors[pS]); tmpHisto->SetMarkerColor(colors[pS]); tmpHisto->SetMarkerStyle(markers[pS]);
     if(pS==0){leg->AddEntry(tmpHisto,string("<PU> = 6").c_str(), "LP");}
     if(pS==1){leg->AddEntry(tmpHisto,string("<PU> =14").c_str(), "LP");}
     if(pS==2){leg->AddEntry(tmpHisto,string("<PU> = 2").c_str(), "LP");}

     mgraphS  ->Add(graphS  ,"CP");
     mgraphDY ->Add(graphDY ,"CP");
     mgraphSDY->Add(graphSDY,"CP");
     mgraphB  ->Add(graphB  ,"CP");
     mgraphSB ->Add(graphSB ,"CP");
  }

     TH1* tmphisto = new TH1D("tmphisto","tmphisto", ntouse,0,ntouse-1);
//     for(int i=0; i<ntouse; i++){tmphisto->GetXaxis()->SetBinLabel(i+1,titles[i]);}
//     tmphisto->SetMinimum(MinSB*0.9);   tmphisto->SetMaximum(MaxSB*1.1); 

  TVirtualPad* pad;
  TCanvas* c1 = new TCanvas("c1","c1", ntouse*100, 1000); 
  c1->Divide(1,3, 0, 0);
  pad = c1->cd(1);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphS  ->Draw("A"); tmphisto = mgraphS  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle(signal.c_str());  tmphisto->SetTitleOffset(0.35,"Y");
  leg->Draw("same");
  pad = c1->cd(2);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphDY ->Draw("A"); tmphisto = mgraphDY ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("DY");    tmphisto->SetTitleOffset(0.35,"Y"); 
  pad = c1->cd(3);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.35);  pad->SetLeftMargin(0.05); mgraphSDY->Draw("A"); tmphisto = mgraphSDY->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/DY");  tmphisto->SetTitleOffset(0.35,"Y");
//  pad = c1->cd(4);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphB  ->Draw("A"); tmphisto = mgraphB  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("B");     tmphisto->SetTitleOffset(0.35,"Y");     
//  pad = c1->cd(5);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.13);  pad->SetLeftMargin(0.05); mgraphSB ->Draw("A"); tmphisto = mgraphSB ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/B");   tmphisto->SetTitleOffset(0.35,"Y");
       for(int i=0; i<ntouse; i++){tmphisto->GetXaxis()->SetBinLabel(tmphisto->GetXaxis()->FindBin(i),titles[i]);} tmphisto->LabelsOption("d"); tmphisto->SetLabelSize(0.08,"X");
  pad = c1->cd(0);
  c1->SaveAs((OutDir + "summary_DY.png").c_str());
  c1->SaveAs((OutDir + "summary_DY.C").c_str());
  delete c1;

  c1 = new TCanvas("c1","c1", ntouse*100, 1000);
  c1->Divide(1,3, 0, 0);
  pad = c1->cd(1);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphS  ->Draw("A"); tmphisto = mgraphS  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle(signal.c_str());  tmphisto->SetTitleOffset(0.35,"Y");
  leg->Draw("same");
//  pad = c1->cd(2);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphDY ->Draw("A"); tmphisto = mgraphDY ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("DY");    tmphisto->SetTitleOffset(0.35,"Y");
//  pad = c1->cd(3);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphSDY->Draw("A"); tmphisto = mgraphSDY->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/DY");  tmphisto->SetTitleOffset(0.35,"Y");
  pad = c1->cd(2);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphB  ->Draw("A"); tmphisto = mgraphB  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("B");     tmphisto->SetTitleOffset(0.35,"Y");
  pad = c1->cd(3);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.35);  pad->SetLeftMargin(0.05); mgraphSB ->Draw("A"); tmphisto = mgraphSB ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/B");   tmphisto->SetTitleOffset(0.35,"Y");
       for(int i=0; i<ntouse; i++){tmphisto->GetXaxis()->SetBinLabel(tmphisto->GetXaxis()->FindBin(i),titles[i]);} tmphisto->LabelsOption("d"); tmphisto->SetLabelSize(0.08,"X");
  pad = c1->cd(0);
  c1->SaveAs((OutDir + "summary_ALL.png").c_str());
  c1->SaveAs((OutDir + "summary_ALL.C").c_str());
  delete c1;
  delete f;
}



void performancePU(string OutDir, string evcat, string signal, string background,  string fname, const std::vector<string>& names, const std::vector<TString>& titles)
{
   int colors[]  = {1,  kRed,     kGreen+3, kBlue, kBlue, kRed+3, kRed, kRed, kOrange, kOrange};
   int styles[]  = {1,  2,        9,        9,     1,     1,      1,    1,    2,       1};
   int markers[] = {20, 24,       21,       25,    22,    24,     20,   24,   25,      21};

  int ntouse = names.size();
  TString meanFitFunc("pol3");
  TString rmsFitFunc("pol3");

  TFile *f=TFile::Open(fname.c_str());
  std::vector<TGraphAsymmErrors*> bckgMeanList, bckgRMSList;
  std::vector<TGraphAsymmErrors *> incEffGrList;
  std::vector< std::vector<TGraphAsymmErrors *> > effGrList; 
  std::vector<std::pair<int,int> > effBins;
  effBins.push_back(std::pair<int,int>(1,3));
  effBins.push_back(std::pair<int,int>(4,6));  
  effBins.push_back(std::pair<int,int>(7,10));
  effBins.push_back(std::pair<int,int>(10,25));

  evcat += "_met";
  FILE* pFile = fopen((OutDir + "Eff.txt").c_str(),"w");
  for(int i=0; i<ntouse; i++)
    {
      string name=names[i];
      TString title=titles[i];
      TString idxStr("");
      idxStr += i;

      TH2D *signalProc= (TH2D*) GetObjectFromPath(f, signal    +"#rightarrow VV" + "/"+evcat+"_"+name+"vspu", true);    
      TH2D *bckgProc=NULL;
      char* BckgSamples = new char [background.size()+1];  strcpy (BckgSamples, background.c_str());
      char* BckgSubSample = strtok (BckgSamples,"|");
      while (BckgSubSample != NULL){
         if(!bckgProc){bckgProc =    (TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name+"vspu", true);
         }else{        bckgProc->Add((TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name+"vspu", true));}
         BckgSubSample = strtok (NULL, "|");
      }


      TH2D *AllbckgProc=NULL;
      char* AllBckgSamples = new char [150];  strcpy (AllBckgSamples,"ZZ|WW|WZ|Single top|t#bar{t}|W+jets|Z-#gamma^{*}+jets#rightarrow ll");
      char* AllBckgSubSample = strtok (AllBckgSamples,"|");
      while (AllBckgSubSample != NULL){
         if(!AllbckgProc){AllbckgProc =    (TH2D*)GetObjectFromPath(f, string(AllBckgSubSample)+"/"+evcat+"_"+name+"vspu", true);
         }else{           AllbckgProc->Add((TH2D*)GetObjectFromPath(f, string(AllBckgSubSample)+"/"+evcat+"_"+name+"vspu", true));}
         AllBckgSubSample = strtok (NULL, "|");
      }

      //inclusive efficiency
      int nxbins=bckgProc->GetXaxis()->GetNbins();
      TH1*signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",1,nxbins);
//      TH1*signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",1,11);
      fixExtremities(signalProj,true,true);
      TH1*bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",1,nxbins);
//      TH1*bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",1,11);
      fixExtremities(bckgProj,true,true);

      TGraphAsymmErrors *gr = getEfficiencyCurve(signalProj, bckgProj, names[i].find("metL")!=std::string::npos );
      gr->SetName((names[i]+"_eff").c_str());
      gr->SetTitle(titles[i]);
      gr->SetLineColor(colors[i]);
      gr->SetLineStyle(styles[i]);
      gr->SetFillStyle(0);
      gr->SetMarkerColor(colors[i]);
      gr->SetMarkerStyle(markers[i]);
      gr->SetLineWidth(2);
      incEffGrList.push_back(gr);
      delete signalProj; delete bckgProj;
     

      //efficiency per bins
      std::vector<TGraphAsymmErrors *> ieffGrList;
      for(size_t ibin=0; ibin<effBins.size(); ibin++){ 
	  signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",effBins[ibin].first,effBins[ibin].second);
	  fixExtremities(signalProj,true,true);
	  bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",effBins[ibin].first,effBins[ibin].second);
	  fixExtremities(bckgProj,true,true);

	  //compute efficiency
	  gr = getEfficiencyCurve(signalProj, bckgProj, names[i].find("metL")!=std::string::npos );
         delete signalProj; delete bckgProj;
	  gr->SetName((names[i]+"_eff").c_str());
	  gr->SetTitle(titles[i]);
	  gr->SetLineColor(colors[i]);
	  gr->SetLineStyle(styles[i]);
	  gr->SetFillStyle(0);
	  gr->SetMarkerColor(colors[i]);
	  gr->SetMarkerStyle(markers[i]);
	  gr->SetLineWidth(2);
	  ieffGrList.push_back( gr );
      }
      effGrList.push_back( ieffGrList );

      //built the performance curves 
      TGraphAsymmErrors *bckgMean = new TGraphAsymmErrors;
      bckgMean->SetName((names[i]+"_mean").c_str());
      bckgMean->SetTitle(titles[i]);
      bckgMean->SetLineColor(colors[i]);
      bckgMean->SetLineStyle(styles[i]);
      bckgMean->SetFillStyle(0);
      bckgMean->SetMarkerColor(colors[i]);
      bckgMean->SetMarkerStyle(markers[i]);
      bckgMean->SetLineWidth(2);
      bckgMeanList.push_back(bckgMean);

      TGraphAsymmErrors *bckgRMS = new TGraphAsymmErrors;
      bckgRMS->SetName((names[i]+"_rms").c_str());
      bckgRMS->SetTitle(titles[i]);
      bckgRMS->SetLineColor(colors[i]);
      bckgRMS->SetLineStyle(styles[i]);
      bckgRMS->SetFillStyle(0);
      bckgRMS->SetMarkerColor(colors[i]);
      bckgRMS->SetMarkerStyle(markers[i]);
      bckgRMS->SetLineWidth(2);
      bckgRMSList.push_back(bckgRMS);
      for(int ibin=1; ibin<=nxbins; ibin++) 
	{ 
	  signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",ibin,ibin); 
	  fixExtremities(signalProj,true,true);
	  bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",ibin,ibin); 
	  fixExtremities(bckgProj,true,true);

	  //check the evolution for pileup
	  bckgMean->SetPoint(ibin-1,ibin-1,bckgProj->GetMean()); 
	  bckgMean->SetPointError(ibin-1,0,0,bckgProj->GetMeanError(),bckgProj->GetMeanError());
	  
	  bckgRMS->SetPoint(ibin-1,ibin-1,bckgProj->GetRMS()); 
	  bckgRMS->SetPointError(ibin-1,0,0,bckgProj->GetRMSError(),bckgProj->GetRMSError()); 
          delete signalProj; delete bckgProj;
	}
      bckgMean->Fit(meanFitFunc,"Q+","",2,nxbins);
      bckgRMS->Fit(rmsFitFunc,"Q+","",2,nxbins);
    }
    fclose(pFile);

  //pileup robustness
  TCanvas *puC = new TCanvas("puC", "Pileup robustness", 1600,800);
//getNewCanvas("puC","Pileup robustness",false);
//puC->SetWindowSize(1000,500);
  puC->Divide(2,1);  

  TPad *p=(TPad *)puC->cd(1);
  for(size_t ip=0; ip<bckgMeanList.size(); ip++)    bckgMeanList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgMeanList[0]->GetXaxis()->SetTitle("Pileup");
  bckgMeanList[0]->GetYaxis()->SetTitle("Average");
  bckgMeanList[0]->SetMaximum(30);
  bckgMeanList[0]->SetMinimum(5);
  TLegend *leg=p->BuildLegend(0.20, 0.60, 0.60, 0.90);
  formatForCmsPublic(p,leg,"CMS simulation, Z#rightarrow ll",ntouse);

  p=(TPad *)puC->cd(2);
  for(size_t ip=0; ip<bckgRMSList.size(); ip++)    bckgRMSList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgRMSList[0]->GetXaxis()->SetTitle("Pileup");
  bckgRMSList[0]->GetYaxis()->SetTitle("RMS");
  bckgRMSList[0]->SetMaximum(20);
  bckgRMSList[0]->SetMinimum(5);

  TString legTit="CMS simulation "+TString(signal.c_str());
  legTit.ReplaceAll("H(","m_{H^{0}}=");
  legTit.ReplaceAll(")"," GeV/c^{2}");
  puC->SaveAs( (OutDir + "puC.png").c_str());
  delete puC;

  //inclusive efficiency
  TCanvas *effC =  new TCanvas("effC", "Inclusive efficiency", 800,800);//getNewCanvas("effC","Inclusive efficiency",false);
//  effC->SetWindowSize(1000,1000);
  effC->cd();
  effC->SetLogy();
  TGraph *effFrame=new TGraph;
  effFrame->SetPoint(0,1e-5,1e-5);
  effFrame->SetPoint(1,1e-5,1.01);
  effFrame->SetPoint(2,1.01,1.01);
  effFrame->SetPoint(3,1.01,1e-5);
  effFrame->SetMarkerStyle(1);
  effFrame->Draw("ap");
  effFrame->GetXaxis()->SetTitle("#varepsilon(signal)");
  effFrame->GetYaxis()->SetTitle("#varepsilon(background)");

  for(size_t ip=0; ip<incEffGrList.size(); ip++)   incEffGrList[ip]->Draw("pl");
  leg=effC->BuildLegend(0.20, 0.60, 0.60, 0.90);
  formatForCmsPublic(effC,leg,legTit,ntouse);
  effC->SaveAs( (OutDir + "effC.png").c_str());
  delete effC;

  //efficiency degradation  
  effC = new TCanvas("effpuC", "Differential efficiency", 800,800);//getNewCanvas("effpuC","Differential efficiency",false);
  effC->Divide(effBins.size()/2,2);
  for(size_t ip=0; ip<effBins.size(); ip++) 
   {
      p=(TPad *)effC->cd(ip+1);
      p->SetLogy();
      effFrame->Draw("ap");
      
      TString ybinTitle("");
      ybinTitle += effBins[ip].first;
      ybinTitle += "#leq PU #leq";
      ybinTitle += effBins[ip].second;
      TPaveText *pave = new TPaveText(0.05,0.96,0.5,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->AddText(ybinTitle);
      pave->Draw("same");

      for(int i=0; i<ntouse; i++) effGrList[i][ip]->Draw("pl");

      if(ip>0) continue;
      leg=p->BuildLegend(0.15, 0.60, 0.60, 0.95);
      formatForCmsPublic(p,leg,legTit,ntouse);
    }
    effC->SaveAs((OutDir + "effpuC.png").c_str());
    delete effC;

  //another view of efficiency degradation
//  effC = getNewCanvas("effpertypeC","Differential efficiency per type",false);
//  effC->SetWindowSize(400*ntouse,400);
//  effC->Divide(ntouse,1);
  for(int ip=0; ip<ntouse; ip++) 
    {
       effC = new TCanvas("effpertypeC","Differential efficiency per type",800,800);
       effC->SetLogy();
//      p=(TPad *)effC->cd(ip+1);
//      p->SetLogy();
      effFrame->Draw("ap");
      
      TPaveText *pave = new TPaveText(0.05,0.96,0.5,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->AddText(effGrList[ip][0]->GetTitle());
      pave->Draw("same");

      //
      for(int np=0; np<effGrList[ip][0]->GetN(); np++)
	{
	  TGraphAsymmErrors *dif_gr=new TGraphAsymmErrors;
	  for(size_t i=0; i<effBins.size(); i++) 
	    {
	      Double_t x(0),y(0);
	      effGrList[ip][i]->GetPoint(np,x,y);
	      dif_gr->SetPoint(i,x,y);
	    }
	  dif_gr->SetLineColor(6);
	  dif_gr->SetLineWidth(3);
	  dif_gr->Draw("l");
	}
      
      incEffGrList[ip]->Draw("p");
      effC->SaveAs( (OutDir + "effpertypeC_" + names[ip] + ".png").c_str());
      delete effC;
    }

//       if(ip>0) continue;
//       leg=p->BuildLegend();
//       formatForCmsPublic(p,leg,legTit,ntouse);


  effC = new TCanvas("effpertypeC","Differential efficiency per type",800,800);
  effC->SetLogy();
  effFrame->Draw("ap");
  for(int ip=0; ip<ntouse; ip++)
    {
//      p=(TPad *)effC->cd(ip+1);
//      p->SetLogy();
       
//      TPaveText *pave = new TPaveText(0.05,0.96,0.5,0.99,"NDC");
  //    pave->SetBorderSize(0); 
//      pave->SetFillStyle(0);
//      pave->AddText(effGrList[ip][0]->GetTitle());
//      pave->Draw("same");
  
      //
      for(int np=0; np<effGrList[ip][0]->GetN(); np++)
        { 
          TGraphAsymmErrors *dif_gr=new TGraphAsymmErrors;
          for(size_t i=0; i<effBins.size(); i++)
            {
              Double_t x(0),y(0);
              effGrList[ip][i]->GetPoint(np,x,y);
              dif_gr->SetPoint(i,x,y);
            }
          dif_gr->SetLineColor(6);
          dif_gr->SetLineWidth(3);
          dif_gr->Draw("l");
        }
      
      incEffGrList[ip]->Draw("lp");
    }
    //leg=effC->BuildLegend(0.15, 0.60, 0.60, 0.95);
    //leg->Draw("same");
    effC->SaveAs( (OutDir + "effpertypeC.png").c_str());
    delete effC;
}




//
void performanceComparison(string OutDir="Img", string evcat="mumu", string signal="H(200)", string background="Z-#gamma^{*}+jets#rightarrow ll",  string fname="../../test/plotter.root")
{
  setStyle();
  gStyle->SetOptFit(0);
  gStyle->SetPadTopMargin   (0.05);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadRightMargin (0.05);
  gStyle->SetPadLeftMargin  (0.15);
  system((string("mkdir -p ") + OutDir).c_str());

  
  // TString names[]={"metvspu",       "minMetvspu",                       "minClusteredMetvspu",     "unclusteredMetvspu",     "superMinMetvspu"};
//   TString titles[]={"E_{T}^{miss}", "min assoc. E_{T}^{miss}(charged)", "min assoc. E_{T}^{miss}", "min clean E_{T}^{miss}", "minimized E_{T}^{miss}"};
  
//  string names[]  ={"met_metvspu",      "met_assocMetvspu",       "met_cleanMetvspu", "met_minAssocChargedMetvspu", "met_minAssocMetvspu", "met_minCleanMetvspu", "met_superMinMetvspu", "met_redMetvspu", "met_redAssocMetvspu"};
//  TString titles[] ={"E_{T}^{miss}", "assoc-E_{T}^{miss}", "clean E_{T}^{miss}", "min{E_{T}^{miss},assoc E_{T}^{miss} (charged)}", "min{E_{T}^miss,assoc-E_{T}^{miss})", "min{E_{T}^{miss},clean-E_{T}^{miss}}", "min{E_{T}^{miss},assoc-E_{T}^{miss},clean-E_{T}^{miss},central-E_{T}^{miss}}", "red-E_{T}^{miss}", "red{E_{T}^miss,assoc-E_{T}^{miss})"};

//    string names[]  ={"metL_metvspu",      "metL_assocMetvspu",       "metL_cleanMetvspu", "metL_minAssocChargedMetvspu", "metL_minAssocMetvspu", "metL_minCleanMetvspu", "metL_superMinMetvspu", "met_redMetvspu"};
//    string names[]  ={"met_metgeq060zptvspu",      "met_assocMetgeq060zptvspu",       "met_cleanMetgeq060zptvspu", "met_minAssocChargedMetgeq060zptvspu", "met_minAssocMetgeq060zptvspu", "met_minCleanMetgeq060zptvspu", "met_superMinMetgeq060zptvspu", "met_redMetgeq060zptvspu"};
//  TString titles[] ={"E_{T}^{miss}", "assoc-E_{T}^{miss}", "clean E_{T}^{miss}", "min{E_{T}^{miss},assoc E_{T}^{miss} (charged)}", "min{E_{T}^miss,assoc-E_{T}^{miss})", "min{E_{T}^{miss},clean-E_{T}^{miss}}", "min{E_{T}^{miss},assoc-E_{T}^{miss},clean-E_{T}^{miss},central-E_{T}^{miss}}", "red-E_{T}^{miss}"};


//    string names[]  ={"met_minAssocMetvspu", "met_minAssocMetgeq40zptvspu", "met_minAssocMetgeq060zptvspu", "met_minAssocMetgeq080zptvspu" };
//  TString titles[] ={"min{E_{T}^miss,assoc-E_{T}^{miss})", "min{E_{T}^miss,assoc-E_{T}^{miss}) (> 0.4 zpT)", "min{E_{T}^miss,assoc-E_{T}^{miss}) (> 0.6 zpT)", "min{E_{T}^miss,assoc-E_{T}^{miss}) (> 0.8 zpT)" };

//    string names[]  ={"met_minAssocMetvspu", "met_assocMetleq120pfmetvspu", "met_assocMetleq140pfmetvspu", "met_assocMetleq160pfmetvspu" };
//  TString titles[] ={"min{E_{T}^miss,assoc-E_{T}^{miss})", "E_{T}^miss if assoc-E_{T}^{miss} < 1.2 E_{T}^miss", "E_{T}^miss if assoc-E_{T}^{miss} < 1.4 E_{T}^miss", "E_{T}^miss if assoc-E_{T}^{miss} < 1.6 E_{T}^miss" };

//    string names[]  ={"met_redMetvspu", "met_redMetgeq40zptvspu", "met_redMetgeq060zptvspu", "met_redMetgeq080zptvspu" };
//  TString titles[] ={"red-E_{T}^{miss}", "red-E_{T}^{miss} (> 0.4 zpT)", "red-E_{T}^{miss} (> 0.6 zpT)", "red-E_{T}^{miss} (> 0.8 zpT)" };

//     string names[]={"met_assocMetvspu",  "met_assocMet5vspu", "met_assocMet10vspu",   "met_assocFwdMetvspu", "met_assocFwdMet5vspu", "met_assocFwdMet10vspu"         };
//   TString titles[]={"assoc-E_{T}^{miss}  [PT>2]", "assoc-E_{T}^{miss}  [PT>5]", "assoc-E_{T}^{miss}  [PT>10]", "assoc-E_{T}^{miss} + Fwd  [PT>2]", "assoc-E_{T}^{miss} + Fwd  [PT>5]", "assoc-E_{T}^{miss} + Fwd  [PT>10]"};

//     string names[]={"met_assocMetvspu",  "met_assocMet5vspu", "met_assocMet10vspu"};
//   TString titles[]={"assoc-E_{T}^{miss}  [PT>2]", "assoc-E_{T}^{miss}  [PT>5]", "assoc-E_{T}^{miss}  [PT>10]", "assoc-E_{T}^{miss}"};

//     string names[]={"met_assocFwdMetvspu", "met_assocFwdMet5vspu", "met_assocFwdMet10vspu"         };
//   TString titles[]={"assoc-E_{T}^{miss} + Fwd  [PT>2]", "assoc-E_{T}^{miss} + Fwd  [PT>5]", "assoc-E_{T}^{miss} + Fwd  [PT>10]"};


//     string names[]={"met_metvspu",       "met_centralMetvspu",       "met_assocChargedMetvspu",          "met_assocMetvspu",       "met_assocFwdMetvspu"         };
//   TString titles[]={"E_{T}^{miss}", "central-E_{T}^{miss}", "assoc E_{T}^{miss} (charged)", "assoc-E_{T}^{miss}", "assoc-E_{T}^{miss} + Fwd"};

//     string names[]={"met_metvspu",       "met_minAssocChargedMetvspu"      , "met_minAssocMetvspu",    "met_redMetvspu"                              , "met_redMinAssocMetvspu", "met_minclusteredMetvspu" , "met_minclusteredAssocMetvspu"        };
//   TString titles[]={"E_{T}^{miss}",      "min assoc-E_{T}^{miss} (charged)", "min assoc-E_{T}^{miss}", "red [ E_{T}^{miss} , clustered E_{T}^{miss}]", "red [ min assoc-E_{T}^{miss} , clustered E_{T}^{miss}]" };
//   TString titles[]={"E_{T}^{miss}",      "min~assoc-E_{T}^{miss} (charged)", "min~assoc-E_{T}^{miss}", "red~[ E_{T}^{miss} , clustered~E_{T}^{miss}]", "red [ min~assoc-E_{T}^{miss},clust~E_{T}^{miss}]", "min~clust-E_{T}^{miss}", "min(clust-E_{T}^{miss},assoc E_{T}^{miss})" };


   std::vector<string> names;   std::vector<TString> titles;

//     string names[]={"met_metvspu",       "met_minAssocChargedMetvspu"      , "met_minAssocMetvspu",    "met_redMetvspu"                              , "met_redMinAssocMetvspu", "met_minclusteredMetvspu" , "met_minclusteredAssocMetvspu"        };
//   TString titles[]={"E_{T}^{miss}",      "min~assoc-E_{T}^{miss} (charged)", "min~assoc-E_{T}^{miss}", "red~[ E_{T}^{miss} , clustered~E_{T}^{miss}]", "red [ min~assoc-E_{T}^{miss},clust~E_{T}^{miss}]", "min~clust-E_{T}^{miss}", "min(clust-E_{T}^{miss},assoc E_{T}^{miss})" };


  names.clear();                             titles.clear();
  names.push_back("met"                 );   titles.push_back("E_{T}^{miss}");
//  names.push_back("assocChargedMet"     );   titles.push_back("assoc-E_{T}^{miss}(charged)");
//  names.push_back("assocMet"            );   titles.push_back("assoc-E_{T}^{miss}");
  names.push_back("clusteredMet"        );   titles.push_back("clustered-E_{T}^{miss}");
  names.push_back("minAssocChargedMet"  );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))");
  names.push_back("minAssocMet"         );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss})");
  names.push_back("minClusteredMet"     );   titles.push_back("min(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("minTAssocMet"        );   titles.push_back("min(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})");
  names.push_back("minTClusteredMet"    );   titles.push_back("min(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})");
  names.push_back("minAClusteredMet"    );   titles.push_back("min(assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("min3Met"             );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("min4Met"             );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redMet"              );   titles.push_back("red(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redAssocChargedMet"  );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss}(charged))");
  names.push_back("redAssocMet"         );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss})");
  names.push_back("redClusteredMet"     );   titles.push_back("red(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redTAssocMet"        );   titles.push_back("red(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})");
  names.push_back("redTClusteredMet"    );   titles.push_back("red(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})");
  names.push_back("redAClusteredMet"    );   titles.push_back("red(assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("red3Met"             );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redminAssocMet"      );   titles.push_back("red(min(E_{T}^{miss},assoc-E_{T}^{miss}),clustered E_{T}^{miss})");
  performanceSummary(OutDir, evcat, signal, background,  fname, names, titles);


  names.clear();                             titles.clear();
  names.push_back("met"                 );   titles.push_back("E_{T}^{miss}");
  names.push_back("minAssocChargedMet"  );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))");
  names.push_back("minAssocMet"         );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss})");
  names.push_back("minClusteredMet"     );   titles.push_back("min(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("minTAssocMet"        );   titles.push_back("min(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})");
  names.push_back("minTClusteredMet"    );   titles.push_back("min(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})");
  names.push_back("minAClusteredMet"    );   titles.push_back("min(assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("min3Met"             );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("min4Met"             );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  performancePU(OutDir+"min_", evcat, signal, background,  fname, names, titles);

  names.clear();                             titles.clear();
  names.push_back("met"                 );   titles.push_back("E_{T}^{miss}");
  names.push_back("minAssocChargedMet"  );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))");
  names.push_back("redAssocChargedMet"  );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss}(charged))");
  names.push_back("redAssocMet"         );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss})");
  names.push_back("redClusteredMet"     );   titles.push_back("red(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redTAssocMet"        );   titles.push_back("red(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})");
  names.push_back("redTClusteredMet"    );   titles.push_back("red(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})");
  names.push_back("redAClusteredMet"    );   titles.push_back("red(assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("red3Met"             );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redminAssocMet"      );   titles.push_back("red(min(E_{T}^{miss},assoc-E_{T}^{miss}),clustered E_{T}^{miss})");
  performancePU(OutDir+"red_", evcat, signal, background,  fname, names, titles);
  return;
}

