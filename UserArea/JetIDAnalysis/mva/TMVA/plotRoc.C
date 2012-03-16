#include "../download/NYStyle/test/NYStyle.h"

TGraph *getRoc(std::string iName, std::string iVar = "BDTG") { 
  TFile *lFile = new TFile(iName.c_str());
  TTree *lTree = (TTree*) lFile->FindObjectAny("TestTree");
  const int lN = 100;
  double *lX = new double[lN];
  double *lY = new double[lN];
  double lBDTMax  = lTree->GetMaximum(iVar.c_str());
  double lBDTMin  = lTree->GetMinimum(iVar.c_str());
  double lBDTBin  = (lBDTMax-lBDTMin)/lN;
  double lSTot    = lTree->GetEntries("class == 0"); 
  double lBTot    = lTree->GetEntries("class == 1");
  for(int i0 = 0; i0 < lN; i0++) { 
    double pCut = lBDTMin + i0*lBDTBin;
    std::stringstream pSS; pSS << iVar << " > " << pCut;
    double lSig    = lTree->GetEntries((pSS.str()+" && class == 0").c_str());
    double lBack   = lTree->GetEntries((pSS.str()+" && class == 1").c_str());
    lY[i0] = lSig/lSTot; 
    lX[i0] = lBack/lBTot;
  }
  TGraph *lG0 = new TGraph(lN,lX,lY); lG0->SetLineWidth(2);
  lG0->GetXaxis()->SetTitle("Background Efficiency");
  lG0->GetYaxis()->SetTitle("Signal Efficiency");
  return lG0;
}
void plotRocAna(std::string iAna="ETau") { 
  TLegend *lL = new TLegend(0.55,0.2,0.9,0.7); lL->SetFillColor(0);    lL->SetBorderSize(0);
  TGraph *lRoc0 = getRoc(("TMVA0_"+iAna+".root").c_str());  lRoc0->SetLineColor(kRed);   lL->AddEntry(lRoc0,"svfit"     ,"l");
  TGraph *lRoc1 = getRoc(("TMVA1_"+iAna+".root").c_str());  lRoc1->SetLineColor(kBlue);  lL->AddEntry(lRoc1,"vars"      ,"l");
  TGraph *lRoc2 = getRoc(("TMVA2_"+iAna+".root").c_str());  lRoc2->SetLineColor(kBlack); lL->AddEntry(lRoc2,"vars+svfit","l");
  TGraph *lRoc3 = getRoc(("TMVA3_"+iAna+".root").c_str());  lRoc3->SetLineColor(kGreen); lL->AddEntry(lRoc3,"svfit'"    ,"l");
  double lX = 10; double lY = 10;
  if(iAna == "ETau") { lX = 410; lY = 10;}
  if(iAna == "EMu")  { lX = 820; lY = 10;}
  TCanvas *lC0 = new TCanvas(("c0"+iAna).c_str(),("c0"+iAna).c_str(),lX,lY,500,500);
  lRoc0->Draw("alp");
  lRoc1->Draw("lp");
  lRoc2->Draw("lp");
  lRoc3->Draw("lp");
  lL->Draw();
  lC0->SaveAs(("Roc"+iAna+".png").c_str());
}
void plotHist(std::string iAna ="ETau",      std::string iVar="met",std::string iXName="#slash{E_{T}}",
	      std::string iUnit="",double iXMin   =0.    ,double iXMax=120.,int iNBins=10 ) { 
  TFile *lFile = new TFile(("TMVA3_"+iAna+".root").c_str());
  TTree *lTree = (TTree*) lFile->FindObjectAny("TestTree");
  double lX = 10; double lY = 10;
  if(iAna == "ETau") { lX = 410; lY = 10;}
  if(iAna == "EMu")  { lX = 820; lY = 10;}
  TCanvas *lC0 = new TCanvas(("c0"+iAna+iVar).c_str(),("c0"+iAna+iVar).c_str(),lX,lY,500,500);
  std::stringstream pSS; pSS << iVar << ">>" << iAna << iVar << "Hist";
  TH1F *lH0 = new TH1F((iAna+iVar+"Hist0").c_str(),(iAna+iVar+"Hist0").c_str(),iNBins,iXMin,iXMax); lH0->SetMarkerStyle(kFullCircle); lH0->Sumw2(); lH0->SetLineWidth(1);
  TH1F *lH1 = new TH1F((iAna+iVar+"Hist1").c_str(),(iAna+iVar+"Hist1").c_str(),iNBins,iXMin,iXMax); lH1->SetFillColor(kOrange-3); lH1->SetLineWidth(1);
  lTree->Draw((pSS.str()+"0").c_str(),"class==0 && m_svfit > 10 && BDTG < -0.1");
  lTree->Draw((pSS.str()+"1").c_str(),"class==1 && m_svfit > 10 && BDTG < -0.1");
  std::stringstream pS1; pS1 << "Events/" << (iXMax-iXMin)/double(iNBins) << iUnit; 
  lH0->Scale(1./lH0->Integral()); lH0->GetXaxis()->SetTitle((iXName+"["+iUnit+"]").c_str()); lH0->GetYaxis()->SetTitle(pS1.str().c_str());
  lH1->Scale(1./lH1->Integral()); lH1->GetXaxis()->SetTitle((iXName+"["+iUnit+"]").c_str()); lH1->GetYaxis()->SetTitle(pS1.str().c_str());
  TLegend *lL = new TLegend(0.5,0.7,0.8,0.9); lL->SetFillColor(0);    lL->SetBorderSize(0);
  lL->AddEntry(lH0,"Z#rightarrow#tau#tau","lp");
  lL->AddEntry(lH1,"ggHiggs 120","lf");
  lH0->Draw();
  lH1->Draw("hist sames");
  lH0->Draw("sames");
  lL->Draw();
  lC0->SaveAs((iAna+iVar+".png").c_str());
}
void plotRoc() { 
  Prep();
  //plotRocAna("MuTau"); 
  //plotRocAna("ETau");
  //plotRocAna("EMu");
  //return;
  plotHist("MuTau");
  plotHist("ETau");
  plotHist("EMu");
}


void plotXRoc() { 
  Prep();
  TCanvas *lC0 = new TCanvas("c0","c0",800,600);
  int iNBins = 40; double iXMin = -0.1; double iXMax=0.1; std::string iXName = "(m_{SV}-m_{SA})/m_{SV}"; std::string lCmd = "m_sv:m_svfit";
  //int iNBins = 40; double iXMin = -0.1; double iXMax=0.1; std::string iXName = "(m_{SV}-m_{SA})/m_{SV}"; std::string lCmd = "(m_sv-m_svfit)/m_sv";
  std::stringstream lStr0; lStr0 << lCmd << ">>+Hist0";
  std::stringstream lStr1; lStr1 << lCmd << ">>+Hist1";

  TFile *l0File = new TFile("higgs_mc_2.root");
  TH1F *lH0 = new TH1F("Hist0","Hist0",iNBins,iXMin,iXMax); lH0->SetMarkerStyle(kFullCircle); lH0->Sumw2(); lH0->SetLineWidth(1);
  TTree *l0Tree0 = (TTree*) l0File->FindObjectAny("EMu");
  TTree *l1Tree0 = (TTree*) l0File->FindObjectAny("ETau");
  TTree *l2Tree0 = (TTree*) l0File->FindObjectAny("MuTau");
  l0Tree0->Draw(lStr0.str().c_str(),"m_svfit > 0");
  //l1Tree0->Draw(lStr0.str().c_str(),"m_svfit > 0");
  //l2Tree0->Draw(lStr0.str().c_str(),"m_svfit > 0");
  return;

  TFile *l1File = new TFile("zjets_mc_2.root");
  TH1F *lH1 = new TH1F("Hist1","Hist1",iNBins,iXMin,iXMax); lH1->SetFillColor(kOrange-3); lH1->SetLineWidth(1);
  TTree *l0Tree1 = (TTree*) l1File->FindObjectAny("EMu");
  TTree *l1Tree1 = (TTree*) l1File->FindObjectAny("ETau");
  TTree *l2Tree1 = (TTree*) l1File->FindObjectAny("MuTau");
  l0Tree1->Draw(lStr1.str().c_str());
  l1Tree1->Draw(lStr1.str().c_str());
  l2Tree1->Draw(lStr1.str().c_str());

  std::stringstream pS1; pS1 << "Events/" << (iXMax-iXMin)/double(iNBins); 
  lH0->Scale(1./lH0->Integral()); lH0->GetXaxis()->SetTitle((iXName).c_str()); lH0->GetYaxis()->SetTitle(pS1.str().c_str());
  lH1->Scale(1./lH1->Integral()); lH1->GetXaxis()->SetTitle((iXName).c_str()); lH1->GetYaxis()->SetTitle(pS1.str().c_str());
  TLegend *lL = new TLegend(0.2,0.7,0.5,0.9); lL->SetFillColor(0);    lL->SetBorderSize(0);
  lL->AddEntry(lH0,"Z#rightarrow#tau#tau","lp");
  lL->AddEntry(lH1,"ggHiggs 120","lf");
  lH1->Draw("hist");
  //lH0->Draw("ep sames");
  lL->Draw();
  lC0->SaveAs("AllrDiff.png");
}
