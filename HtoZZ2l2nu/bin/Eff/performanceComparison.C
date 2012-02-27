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


void GetWeights(double NewPU, double* OldPU, double* array, double N){
   double Integral = 0.0;
   for(int i=0;i<N;i++){Integral+=OldPU[i];}
   for(int i=0;i<N;i++){OldPU[i] =OldPU[i]/Integral;}

   Integral = 0.0;
   for(int i=0;i<N;i++){
      array[i] = std::min(TMath::PoissonI(i, NewPU) / OldPU[i] , 1000.0);
      Integral+=array[i];
      printf("%3i - %6.5f --> %6.5f  (ratio=%6.5f)\n",i,OldPU[i],array[i],array[i]/OldPU[i]);
   }printf("Integral = %f\n",Integral);

//   for(int i=0;i<N;i++){array[i] = array[i]/Integral;}
}



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


double GetEfficiency(TH1 *hist, double Cut){
   int cutBin = hist->GetXaxis()->FindBin(Cut);
   double TotalIntegral = hist->Integral();
   double Integral = hist->Integral(cutBin+1,hist->GetXaxis()->GetNbins()+1);
   Integral += hist->GetBinContent(cutBin) * ((hist->GetXaxis()->GetBinUpEdge(cutBin) - Cut) / hist->GetXaxis()->GetBinWidth(cutBin));
   return Integral / TotalIntegral;
}


double GetCut(TH1 *hist, double Eff){
   double TotalIntegral = hist->Integral();
   for(int i=hist->GetXaxis()->GetNbins()+1;i>=0;i--){
     if(hist->Integral(i,hist->GetXaxis()->GetNbins()+1)/TotalIntegral  > Eff){
        double ClosestEfficiency = 100;  double bestdC;
        for(double dC =hist->GetXaxis()->GetBinWidth(i); dC>0;dC-=0.01){
           double tmpEff = GetEfficiency(hist, hist->GetXaxis()->GetBinLowEdge(i)+dC);
           if(fabs(tmpEff-Eff)<ClosestEfficiency){ ClosestEfficiency = fabs(tmpEff-Eff); bestdC=dC; }           
        }
        return hist->GetXaxis()->GetBinLowEdge(i)+ bestdC;
     }
   }
   return -1;
}


double GetYields(TH1 *hist, double Cut){
   return hist->Integral(hist->GetXaxis()->FindBin(Cut),hist->GetXaxis()->GetNbins());
}

TGraph* getCutGraph(TH1* hist){
   TGraph* toReturn = new TGraph(hist->GetXaxis()->GetNbins()+2);
   double Integral = hist->Integral();
   for(int i=0;i<hist->GetXaxis()->GetNbins()+1;i++){
      toReturn->SetPoint(i,hist->Integral(i,hist->GetXaxis()->GetNbins()+1)/Integral, hist->GetXaxis()->GetBinLowEdge(i));      
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
      TH1*        SHist;  TH1*   BHist;
};

stArray  makeArray(string evcat, string name, string title, string signal, string background, TFile* f, double* PUReweightAvr, TFile* GJ=NULL){
      char pbuffer[256]; sprintf(pbuffer,"%p",PUReweightAvr);
      evcat += "_met";

      TH2D *signalProc= NULL;
      signalProc = (TH2D*) GetObjectFromPath(f, signal    +"#rightarrow VV" + "/"+evcat+"_"+name+"_vspu", true);    
      std::vector<TH2D*> bckgProcVector;
      TH2D* bckgProc = NULL;
      char* BckgSamples = new char [background.size()+1];  strcpy (BckgSamples, background.c_str());
      char* BckgSubSample = strtok (BckgSamples,"|");
      bckgProcVector.clear();
      while (BckgSubSample != NULL){
         TH2D* tmpHisto = (TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name+"_vspu", true);
         tmpHisto->SetName((string(BckgSubSample) + tmpHisto->GetName()).c_str());

         if(GJ && string(BckgSubSample)==string("Z-#gamma^{*}+jets#rightarrow ll")){
            //std::cout << "Use Gamma+Jet\n";
            double ExpDY = tmpHisto->Integral();delete tmpHisto;
            tmpHisto = (TH2D*) GetObjectFromPath(GJ, "gammasum_"+name+"_vspu", true);
            //printf("Integral = %f vs %f --> ",ExpDY,tmpHisto->Integral());
            tmpHisto->Scale(ExpDY/tmpHisto->Integral());
            //printf("%f\n",tmpHisto->Integral());
            //tmpHisto->Scale(0.0);
         }
               
         if(!bckgProc){bckgProc =    tmpHisto;
         }else{        bckgProc->Add(tmpHisto);}
         bckgProcVector.push_back((TH2D*)tmpHisto->Clone((string(BckgSubSample) + tmpHisto->GetName()).c_str()));
         BckgSubSample = strtok (NULL, "|");
      }

      //inclusive efficiency
      int nxbins=bckgProc->GetXaxis()->GetNbins();

      TH1*signalProj = ((TH2 *)signalProc)->ProjectionY((string("sig_py") + signal + pbuffer).c_str(),1,nxbins);
      printf("Integral : %f\n",signalProj->Integral());
      fixExtremities(signalProj,true,true);
      TH1*bckgProj   = ((TH2 *)bckgProc)->ProjectionY((string("bckg_py")+ background + pbuffer).c_str(),1,nxbins);
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
         signalProjPU = (TH1*)signalProj->Clone((string("signalProjPU") + signal + pbuffer).c_str()); signalProjPU->Reset();
         for(int p=1;p<=nxbins;p++){
             TH1* tmpsignalProj = ((TH2 *)signalProc)->ProjectionY("tmp_sig_py",p,p);
             signalProjPU->Add(tmpsignalProj,PUReweightAvr[p-1]);
         }
         //make sure that the inegral is unchanged
         signalProjPU->Scale(signalProj->Integral()/signalProjPU->Integral());
         fixExtremities(signalProjPU,true,true);


         bckgProjPU   = (TH1*)bckgProj  ->Clone((string("bckgProjPU") + background + pbuffer).c_str());   bckgProjPU->Reset();
         for(unsigned int b=0;b<bckgProcVector.size();b++){
            TH1* bckgProjPUtmp   = (TH1*)bckgProcVector[b]->ProjectionY((string("bckgProjPUtmp") + background + pbuffer).c_str());   bckgProjPUtmp->Reset();
   	    for(int p=1;p<=nxbins;p++){
                TH1* tmpbckgProj   = bckgProcVector[b]->ProjectionY("tmpPUProjection",p,p);
                bckgProjPUtmp->Add(tmpbckgProj,PUReweightAvr[p-1]);
            }
            //printf("getting integral %f instead of %f   (%s)\n",bckgProjPUtmp->Integral(), bckgProcVector[b]->Integral(), bckgProcVector[b]->GetName() );
            bckgProjPUtmp->Scale(bckgProcVector[b]->Integral()/bckgProjPUtmp->Integral());
            bckgProjPU->Add(bckgProjPUtmp, 1.0);
            delete bckgProjPUtmp;
            delete bckgProcVector[b];
         }
         fixExtremities(bckgProjPU  ,true,true);
         SeffCut = GetEfficiency(signalProjPU,Cut);
      }else{
        signalProjPU = signalProj;
        bckgProjPU   = bckgProj;
      }

      TGraphAsymmErrors *gr = getEfficiencyCurve(signalProjPU, bckgProjPU, name.find("metL")!=std::string::npos );
      gr->SetName((name+"_eff").c_str());

      printf("Seff = %f & Integral=%f  --> %f\n",SeffCut,signalProjPU->Integral(),SeffCut*signalProjPU->Integral());


      double S = SeffCut*signalProjPU->Integral();
      double SError; signalProjPU->IntegralAndError(signalProjPU->GetXaxis()->FindBin(Cut), signalProjPU->GetXaxis()->GetNbins(), SError);
      double SIntegral = signalProjPU->Integral();
      SError = std::min(S, SError);
      double B = gr->Eval(SeffCut)* bckgProjPU->Integral();
      double BIntegral = bckgProjPU->Integral();
      double BError; bckgProjPU->IntegralAndError(bckgProjPU->GetXaxis()->FindBin(Cut), bckgProjPU->GetXaxis()->GetNbins(), BError);
      BError = std::min(B, BError);
      double SBError = fabs((1/sqrt(S+B) - 1/(S+B))) * SError + (1/(S+B))*BError;

      signalProjPU->SetFillColor(0);
      bckgProjPU->SetFillColor(0);

      stArray toReturn;
      toReturn.Title   = title;
      toReturn.Cut     = Cut;
      toReturn.SEff    = SeffCut;            toReturn.SEffError = SError/SIntegral;
      toReturn.BEff    = gr->Eval(SeffCut);  toReturn.BEffError=gr->Eval(SeffCut)/BIntegral;
      toReturn.S       = S;                  toReturn.SError = SError;
      toReturn.B       = B;                  toReturn.BError = BError;
      toReturn.SsqrtSB = S/sqrt(S+B );       toReturn.SsqrtSBError = SBError;
      toReturn.SB      = S/B;                toReturn.SBError = SError/B + S*BError/(B*B);
      toReturn.SHist   = signalProjPU;       toReturn.BHist   = bckgProjPU;

     //printf("$%35s$ & $\\geq$%iGeV & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ \\\\ \n",toReturn.Title.c_str(), (int)toReturn.Cut, toReturn.SEff, toReturn.SEffError, toReturn.S, toReturn.SError, toReturn.B, toReturn.BError,  toReturn.SsqrtSB, toReturn.SsqrtSBError, toReturn.B, toReturn.BError,  toReturn.SsqrtSB, toReturn.SsqrtSBError );


//      delete signalProj; delete bckgProj;
//      if(PUReweightAvr)delete signalProjPU; delete bckgProjPU;
//      delete gr;
      return toReturn;
}


void performanceSummary(string OutDir, string evcat, string signal, string background,  string fname, const std::vector<string>& names, const std::vector<TString>& titles){
   int colors[]  = {4,  kGreen+3, kRed,     kGreen+3, kBlue, kBlue, kRed+3, kRed, kRed, kOrange};
   int styles[]  = {1,  9,        2,        9,        9,     1,     1,      1,    1,    2};
   int markers[] = {20, 22,       21,       25,    22,    24,     20,   24,   25};

   double PUdata[] = {1.344651e+07,   5.90653e+07,   1.409027e+08,   2.413012e+08,   3.337449e+08,   3.98711e+08,   4.301064e+08,   4.32283e+08,   4.138202e+08,   3.82846e+08,   3.451637e+08,   3.043438e+08,   2.62555e+08,   2.213308e+08,   1.819826e+08,   1.456898e+08,   1.134134e+08,   8.577886e+07,   6.301239e+07,   4.495959e+07,   3.116904e+07,   2.100786e+07,   1.377588e+07,   8796407,   5474418,   3323776,   1970638,   1142040,   647538.6,   359547.2,   195673.2,   104459.9,   54745.15,   28185.57,   28005.55,   0.008};

   double PUReweightAvr15[36];  GetWeights(15 ,PUdata,PUReweightAvr15, 36);
   double PUReweightAvr25[36];  GetWeights(25 ,PUdata,PUReweightAvr25, 36);

   double* PUScenaraio[] = {PUReweightAvr15, PUReweightAvr25};
   string  PUName[] = {"<PU> = 15", "<PU> = 25"};

   int ntouse = names.size();

  TFile *f=TFile::Open(fname.c_str());
//  TFile *g=TFile::Open("gammaTemplates.root");
  TFile *g=NULL;//TFile::Open("gammaTemplates_data.root");

  TMultiGraph* mgraphS   = new TMultiGraph();
  TMultiGraph* mgraphDY  = new TMultiGraph();
  TMultiGraph* mgraphSDY = new TMultiGraph();
  TMultiGraph* mgraphB   = new TMultiGraph();
  TMultiGraph* mgraphSB  = new TMultiGraph();
  TLegend* leg  = new TLegend(0.20,0.73,0.55,0.50, "NDC");
  leg->SetHeader("");leg->SetFillColor(0); leg->SetFillStyle(0); leg->SetLineColor(0);

  TH1D** SHists = new TH1D*[(sizeof(PUScenaraio)/sizeof(double)) * ntouse];
  TH1D** BHists = new TH1D*[(sizeof(PUScenaraio)/sizeof(double)) * ntouse];
  double* Cuts   = new double[(sizeof(PUScenaraio)/sizeof(double)) * ntouse];


  for(unsigned int pS=0;pS<sizeof(PUScenaraio)/sizeof(double);pS++){
     TGraphErrors* graphS   = new TGraphErrors(ntouse);  graphS  ->SetLineColor(colors[pS]); graphS  ->SetMarkerColor(colors[pS]); graphS  ->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphDY  = new TGraphErrors(ntouse);  graphDY ->SetLineColor(colors[pS]); graphDY ->SetMarkerColor(colors[pS]); graphDY ->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphSDY = new TGraphErrors(ntouse);  graphSDY->SetLineColor(colors[pS]); graphSDY->SetMarkerColor(colors[pS]); graphSDY->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphB   = new TGraphErrors(ntouse);  graphB  ->SetLineColor(colors[pS]); graphB  ->SetMarkerColor(colors[pS]); graphB  ->SetMarkerStyle(markers[pS]);
     TGraphErrors* graphSB  = new TGraphErrors(ntouse);  graphSB ->SetLineColor(colors[pS]); graphSB ->SetMarkerColor(colors[pS]); graphSB ->SetMarkerStyle(markers[pS]);

     printf("####################### PU SCENARIO %i  #######################\n",pS);
     for(int i=0; i<ntouse; i++){
          stArray B  = makeArray(evcat, names[i], titles[i].Data(), signal, "ZZ|WW|WZ|Single top|t#bar{t}|W+jets|Z-#gamma^{*}+jets#rightarrow ll", f, PUScenaraio[pS], g);
          stArray DY = makeArray(evcat, names[i], titles[i].Data(), signal, background, f, PUScenaraio[pS], g);
        printf("$%35s$ & $\\geq$%iGeV & $%6.2E\\pm%6.2E$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ & $%6.2f\\pm%6.2f$ \\\\ \n",DY.Title.c_str(), (int)DY.Cut, DY.SEff, DY.SEffError, DY.S, DY.SError, DY.B, DY.BError,  DY.SB, DY.SBError, B.B, B.BError,  B.SB, B.SBError );
        printf("S = %f\n",DY.S);


        graphS  ->SetPoint(i, i, DY.S);     graphS  ->SetPointError(i, 0.0, 0.0); //DY.SError);
        graphDY ->SetPoint(i, i, DY.B);     graphDY ->SetPointError(i, 0.0, 0.0); //DY.BError);
        graphSDY->SetPoint(i, i, DY.SB);    graphSDY->SetPointError(i, 0.0, 0.0); //DY.SBError);
        graphB  ->SetPoint(i, i, B .B);     graphB  ->SetPointError(i, 0.0, 0.0); //B .BError);
        graphSB ->SetPoint(i, i, B .SB);    graphSB ->SetPointError(i, 0.0, 0.0); //B .SBError);
        DY.SHist->SetName((PUName[pS] + DY.SHist->GetName()).c_str() );
        DY.BHist->SetName((PUName[pS] + DY.BHist->GetName()).c_str() );
        SHists[pS*ntouse + i] = (TH1D*) DY.SHist;
        BHists[pS*ntouse + i] = (TH1D*) DY.BHist;
        Cuts[pS*ntouse + i] = DY.Cut;
     }

     TH1* tmpHisto = new TH1D("tmp","tmp",1,0,1);  tmpHisto->SetLineColor(colors[pS]); tmpHisto->SetMarkerColor(colors[pS]); tmpHisto->SetMarkerStyle(markers[pS]);
     leg->AddEntry(tmpHisto,PUName[pS].c_str(), "LP");

     mgraphS  ->Add(graphS  ,"CP");
     mgraphDY ->Add(graphDY ,"CP");
     mgraphSDY->Add(graphSDY,"CP");
     mgraphB  ->Add(graphB  ,"CP");
     mgraphSB ->Add(graphSB ,"CP");
  }

std::cout << "TESTA\n";

     TH1* tmphisto = new TH1D("tmphisto","tmphisto", ntouse,0,ntouse-1);
//     for(int i=0; i<ntouse; i++){tmphisto->GetXaxis()->SetBinLabel(i+1,titles[i]);}
//     tmphisto->SetMinimum(MinSB*0.9);   tmphisto->SetMaximum(MaxSB*1.1); 

  TVirtualPad* pad;
  TCanvas* c1 = new TCanvas("c1","c1", ntouse*100, 1000); 
  c1->Divide(1,3, 0, 0);
   double Min=1E100;
   double Max=-1E100;
  pad = c1->cd(1);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.00); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.15); mgraphS  ->Draw("A"); tmphisto = mgraphS  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle(signal.c_str());  tmphisto->SetTitleOffset(0.35,"Y");  Min = std::min(Min,tmphisto->GetMinimum());  Max=std::max(Max,tmphisto->GetMaximum());
  leg->Draw("same");
  pad = c1->cd(2);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.00); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.15); mgraphDY ->Draw("A"); tmphisto = mgraphDY ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("DY");    tmphisto->SetTitleOffset(0.35,"Y"); Min = std::min(Min,tmphisto->GetMinimum());  Max=std::max(Max,tmphisto->GetMaximum());
  pad = c1->cd(3);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.00); pad->SetBottomMargin(0.35);  pad->SetLeftMargin(0.15); mgraphSDY->Draw("A"); tmphisto = mgraphSDY->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/sqrt(S+DY)");  tmphisto->SetTitleOffset(0.35,"Y");
//  pad = c1->cd(4);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphB  ->Draw("A"); tmphisto = mgraphB  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("B");     tmphisto->SetTitleOffset(0.35,"Y");     
//  pad = c1->cd(5);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.13);  pad->SetLeftMargin(0.05); mgraphSB ->Draw("A"); tmphisto = mgraphSB ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/sqrt(S+B)");   tmphisto->SetTitleOffset(0.35,"Y");
       for(int i=0; i<ntouse; i++){tmphisto->GetXaxis()->SetBinLabel(tmphisto->GetXaxis()->FindBin(i),titles[i]);} tmphisto->LabelsOption("u"); tmphisto->SetLabelSize(0.08,"X");  pad->SetGridy(true);

std::cout << "TESTB\n";


  mgraphS ->GetHistogram()->SetMinimum(0.9*Min);   mgraphS ->GetHistogram()->SetMaximum(1.1*Max);
  mgraphDY->GetHistogram()->SetMinimum(0.9*Min);   mgraphDY->GetHistogram()->SetMaximum(1.1*Max);
  pad = c1->cd(0);
  


  c1->SaveAs((OutDir + "summary_DY.png").c_str());
  c1->SaveAs((OutDir + "summary_DY.C").c_str());
  delete c1;

std::cout << "TESTC\n";


   Min=1E100;
   Max=-1E100;
  c1 = new TCanvas("c1","c1", ntouse*100, 1000);
  c1->Divide(1,3, 0, 0);
  pad = c1->cd(1);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.00); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.15); mgraphS  ->Draw("A"); tmphisto = mgraphS  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle(signal.c_str());  tmphisto->SetTitleOffset(0.35,"Y"); Min = std::min(Min,tmphisto->GetMinimum());  Max=std::max(Max,tmphisto->GetMaximum());
  leg->Draw("same");
//  pad = c1->cd(2);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphDY ->Draw("A"); tmphisto = mgraphDY ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("DY");    tmphisto->SetTitleOffset(0.35,"Y");
//  pad = c1->cd(3);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.0); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.05); mgraphSDY->Draw("A"); tmphisto = mgraphSDY->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/sqrt(S+DY)");  tmphisto->SetTitleOffset(0.35,"Y");
  pad = c1->cd(2);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.00); pad->SetBottomMargin(0.00);  pad->SetLeftMargin(0.15); mgraphB  ->Draw("A"); tmphisto = mgraphB  ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("B");     tmphisto->SetTitleOffset(0.35,"Y"); Min = std::min(Min,tmphisto->GetMinimum());  Max=std::max(Max,tmphisto->GetMaximum());
  pad = c1->cd(3);  pad->SetTopMargin(0.0); pad->SetRightMargin(0.00); pad->SetBottomMargin(0.35);  pad->SetLeftMargin(0.15); mgraphSB ->Draw("A"); tmphisto = mgraphSB ->GetHistogram();  tmphisto->GetYaxis()->SetTitle("S/sqrt(S+B)");   tmphisto->SetTitleOffset(0.35,"Y");
       for(int i=0; i<ntouse; i++){tmphisto->GetXaxis()->SetBinLabel(tmphisto->GetXaxis()->FindBin(i),titles[i]);} tmphisto->LabelsOption("u"); tmphisto->SetLabelSize(0.08,"X"); pad->SetGridy(true);
  pad = c1->cd(0);
  mgraphS ->GetHistogram()->SetMinimum(0.9*Min);   mgraphS ->GetHistogram()->SetMaximum(1.1*Max);
  mgraphB ->GetHistogram()->SetMinimum(0.9*Min);   mgraphDY->GetHistogram()->SetMaximum(1.1*Max);
  c1->SaveAs((OutDir + "summary_ALL.png").c_str());
  c1->SaveAs((OutDir + "summary_ALL.C").c_str());
  delete c1;

std::cout << "TESTD\n";


  leg->SetX1NDC(0.55);
  leg->SetX2NDC(0.90);
  leg->SetY1NDC(0.70);
  leg->SetY2NDC(0.98);

  for(int i=0; i<ntouse; i++){
std::cout << "TESTE\n";
     TCanvas* c1 = new TCanvas("c1","c1", 800, 800);
     BHists[0*ntouse + i]->SetMaximum(10*BHists[0*ntouse + i]->GetMaximum());
     BHists[0*ntouse + i]->SetMinimum(1E-2);
     BHists[0*ntouse + i]->SetAxisRange(0, 150, "X");
     BHists[0*ntouse + i]->SetLineColor(colors[0]); BHists[0*ntouse + i]->SetLineWidth(2); BHists[0*ntouse + i]->SetMarkerColor(colors[0]); BHists[0*ntouse + i]->SetMarkerStyle(markers[0]);   BHists[0*ntouse + i]->Draw("HIST");     
     BHists[1*ntouse + i]->SetLineColor(colors[1]); BHists[1*ntouse + i]->SetLineWidth(2); BHists[1*ntouse + i]->SetMarkerColor(colors[1]); BHists[1*ntouse + i]->SetMarkerStyle(markers[1]);   BHists[1*ntouse + i]->Draw("HIST same");
//     BHists[2*ntouse + i]->SetLineColor(colors[2]); BHists[2*ntouse + i]->SetLineWidth(2); BHists[2*ntouse + i]->SetMarkerColor(colors[1]); BHists[2*ntouse + i]->SetMarkerStyle(markers[2]);   BHists[2*ntouse + i]->Draw("HIST same");
     leg->Draw("same");
     TLine* l1 = new TLine(Cuts[0*ntouse + i], 0, Cuts[0*ntouse + i], BHists[0*ntouse + i]->GetMaximum());
     l1->SetLineWidth(3);  l1->SetLineColor(1);
     l1->Draw("same");
     c1->SetLogy(true);
     c1->SetBottomMargin(0.15);
     c1->Update();
     c1->SaveAs((OutDir + "summary_1DB" + names[i] + ".png").c_str());
     delete l1;
     delete c1;

std::cout << "TESTF\n";

     TCanvas* c2 = new TCanvas("c1","c1", 800, 800);
     SHists[0*ntouse + i]->SetMaximum(10*SHists[0*ntouse + i]->GetMaximum());
     SHists[0*ntouse + i]->SetMinimum(1E-2);
     SHists[0*ntouse + i]->SetAxisRange(0, 250, "X");
     SHists[0*ntouse + i]->SetLineColor(colors[0]); SHists[0*ntouse + i]->SetLineWidth(2); SHists[0*ntouse + i]->SetMarkerColor(colors[0]); SHists[0*ntouse + i]->SetMarkerStyle(markers[0]);   SHists[0*ntouse + i]->Draw("HIST");     
     SHists[1*ntouse + i]->SetLineColor(colors[1]); SHists[1*ntouse + i]->SetLineWidth(2); SHists[1*ntouse + i]->SetMarkerColor(colors[1]); SHists[1*ntouse + i]->SetMarkerStyle(markers[1]);   SHists[1*ntouse + i]->Draw("HIST same");
//     SHists[2*ntouse + i]->SetLineColor(colors[2]); SHists[2*ntouse + i]->SetLineWidth(2); SHists[2*ntouse + i]->SetMarkerColor(colors[2]); SHists[2*ntouse + i]->SetMarkerStyle(markers[2]);   SHists[2*ntouse + i]->Draw("HIST same");
     leg->Draw("same");
     TLine* l2 = new TLine(Cuts[0*ntouse + i], 0, Cuts[0*ntouse + i], SHists[0*ntouse + i]->GetMaximum());
     l2->SetLineWidth(3);  l2->SetLineColor(1);
     l2->Draw("same");
     c2->SetLogy(true);
     c2->SetBottomMargin(0.15);
     c2->Update();
     c2->SaveAs((OutDir + "summary_1DS" + names[i] + ".png").c_str());
     delete l2;
     delete c2;
std::cout << "TESTG\n";
  }

std::cout << "TESTH\n";

  delete f;
}



void performancePU(string OutDir, string evcat, string signal, string background,  string fname, const std::vector<string>& names, const std::vector<TString>& titles)
{
   int colors[]  = {1,  kGreen-3, kGreen+3, kBlue, kBlue, kRed+3, kRed, kRed, kOrange, kOrange, kOrange};
   int styles[]  = {1,  2,        9,        9,     1,     1,      1,    1,    2,       1,     1};
   int markers[] = {20, 24,       21,       25,    22,    24,     20,   24,   25,      21,    20};

  int ntouse = names.size();
  TString meanFitFunc("pol3");
  TString rmsFitFunc("pol3");

  TFile *f=TFile::Open(fname.c_str());
  TFile *g=NULL;//TFile::Open("gammaTemplates_data.root");
  std::vector<TGraphAsymmErrors*> bckgMeanList, bckgRMSList;
  std::vector<TGraphAsymmErrors *> incEffGrList;
  std::vector< std::vector<TGraphAsymmErrors *> > effGrList; 
  std::vector<std::pair<int,int> > effBins;
  effBins.push_back(std::pair<int,int>(1,5));
  effBins.push_back(std::pair<int,int>(6,10));  
  effBins.push_back(std::pair<int,int>(11,15));
  effBins.push_back(std::pair<int,int>(15,30));

  evcat += "_met";
  FILE* pFile = fopen((OutDir + "Eff.txt").c_str(),"w");
  for(int i=0; i<ntouse; i++)
    {
      string name=names[i];
      TString title=titles[i];
      TString idxStr("");
      idxStr += i;

      std::cout <<  (string(signal)    +"#rightarrow VV" + "/"+evcat+"_"+name+"_vspu" ) << endl;

      TH2D *signalProc= (TH2D*) GetObjectFromPath(f, signal    +"#rightarrow VV" + "/"+evcat+"_"+name+"_vspu", true);    
      TH2D *bckgProc=NULL;
      char* BckgSamples = new char [background.size()+1];  strcpy (BckgSamples, background.c_str());
      char* BckgSubSample = strtok (BckgSamples,"|");
      while (BckgSubSample != NULL){
///         if(!bckgProc){bckgProc =    (TH2D*)GetObjectFromPath(g, "gammasum_"+name+"_vspu", true);
//         }else{        bckgProc->Add((TH2D*)GetObjectFromPath(g, "gammasum_"+name+"_vspu", true));}
         if(!bckgProc){bckgProc =    (TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name+"_vspu", true);
         }else{        bckgProc->Add((TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name+"_vspu", true));}
         BckgSubSample = strtok (NULL, "|");
      }


      TH2D *AllbckgProc=NULL;
      char* AllBckgSamples = new char [150];  strcpy (AllBckgSamples,"ZZ|WW|WZ|Single top|t#bar{t}|W+jets|Z-#gamma^{*}+jets#rightarrow ll");
      char* AllBckgSubSample = strtok (AllBckgSamples,"|");
      while (AllBckgSubSample != NULL){
         if(!AllbckgProc){AllbckgProc =    (TH2D*)GetObjectFromPath(f, string(AllBckgSubSample)+"/"+evcat+"_"+name+"_vspu", true);
         }else{           AllbckgProc->Add((TH2D*)GetObjectFromPath(f, string(AllBckgSubSample)+"/"+evcat+"_"+name+"_vspu", true));}
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
  bckgMeanList[0]->SetMaximum(50);
  bckgMeanList[0]->SetMinimum(0);
  TLegend *leg=p->BuildLegend(0.20, 0.60, 0.60, 0.90);
  formatForCmsPublic(p,leg,"CMS simulation, Z#rightarrow ll",ntouse);

  p=(TPad *)puC->cd(2);
  for(size_t ip=0; ip<bckgRMSList.size(); ip++)    bckgRMSList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgRMSList[0]->GetXaxis()->SetTitle("Pileup");
  bckgRMSList[0]->GetYaxis()->SetTitle("RMS");
  bckgRMSList[0]->SetMaximum(25);
  bckgRMSList[0]->SetMinimum(0);

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
  incEffGrList[0]->Draw("Apl");
  incEffGrList[0]->GetXaxis()->SetTitle("#varepsilon(signal)");
  incEffGrList[0]->GetYaxis()->SetTitle("#varepsilon(background)");
  incEffGrList[0]->SetMinimum(1E-5);
  incEffGrList[0]->SetMaximum(1.01);


  TGraph *effFrame= new TGraph();;
  effFrame->SetPoint(0,1e-5,1e-5);
  effFrame->SetPoint(1,1e-5,1.01);
  effFrame->SetPoint(2,1.01,1.01);
  effFrame->SetPoint(3,1.01,1e-5);
  effFrame->SetMarkerStyle(1);
//  effFrame->Draw("ap");
  effFrame->GetXaxis()->SetTitle("#varepsilon(signal)");
  effFrame->GetYaxis()->SetTitle("#varepsilon(background)");

  for(size_t ip=1; ip<incEffGrList.size(); ip++)   incEffGrList[ip]->Draw("pl");
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

  std::vector<string> names;   std::vector<TString> titles;
/*
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
*/

  names.clear();                             titles.clear();
  names.push_back("met"                 );   titles.push_back("E_{T}^{miss}");
  names.push_back("centralMet"          );   titles.push_back("central E_{T}^{miss}");
  names.push_back("clusteredMet"        );   titles.push_back("clustered-E_{T}^{miss}");
  names.push_back("assocChargedMet"     );   titles.push_back("assoc-E_{T}^{miss}(charged)");
  names.push_back("assocMet"            );   titles.push_back("assoc-E_{T}^{miss}");
  names.push_back("assocCMet"           );   titles.push_back("assoc-E_{T}^{miss} + #delta#beta");
  names.push_back("assocFwdMet"         );   titles.push_back("assocFwd-E_{T}^{miss}" );
  names.push_back("assocFwdCMet"        );   titles.push_back("assoFwdc-E_{T}^{miss} + #delta#beta");
  performancePU(OutDir+"Simple_", evcat, signal, background,  fname, names, titles);


  names.clear();                             titles.clear();
  names.push_back("met"                 );   titles.push_back("E_{T}^{miss}");
  names.push_back("minAssocChargedMet"  );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))");
  names.push_back("minAssocFwdMet"      );   titles.push_back("min(E_{T}^{miss},assocFwd-E_{T}^{miss})");
  names.push_back("minClusteredMet"     );   titles.push_back("min(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("min3Met"             );   titles.push_back("min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("redAssocFwdMet"      );   titles.push_back("red(E_{T}^{miss},assocFwd-E_{T}^{miss})");
  names.push_back("redClusteredMet"     );   titles.push_back("red(E_{T}^{miss},clustered-E_{T}^{miss})");
  names.push_back("red3Met"             );   titles.push_back("red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})");
  performancePU(OutDir+"Combined_", evcat, signal, background,  fname, names, titles);
  performanceSummary(OutDir, evcat, signal, background,  fname, names, titles);


  return;
}

