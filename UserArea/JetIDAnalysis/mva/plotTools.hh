std::string *fString  = 0;
std::string *fWeights = 0;
int *fColor           = 0;
int  fId              = 0;
int  fYId             = 0;

TTree * load(std::string iName) { 
  TFile *lFile = new TFile(iName.c_str());
  TTree *lTree  = (TTree*) lFile->FindObjectAny("Events");
  if(lTree == 0) lTree = (TTree*) lFile->FindObjectAny("Flat"); 
  if(lTree == 0) lTree = (TTree*) lFile->FindObjectAny("TestTree"); 
  return lTree;
}
void scale(TH1F** iH,double iInt,const int iN) {
  double lTotInt = iH[0]->Integral();
  cout <<  " Total " << lTotInt << " -- " << iInt/lTotInt << " - " << iN << endl;
  double lMax = 0;
  for(int i0 = 0; i0 < iN; i0++) {
    if(i0 != iN-1) iH[i0]->Scale(double(iInt/lTotInt));
    for(int i1 = 0; i1 < iH[i0]->GetNbinsX()+1; i1++) {
      double pMax = iH[i0]->GetBinContent(i1);
      if(lMax < pMax) lMax = pMax;
    }
  }
  for(int i0 = 0; i0 < iN; i0++) iH[i0]->GetYaxis()->SetRangeUser(0.001,lMax*1.7);
}
void drawDifference(TH1* iH0,TH1 *iH1,TH1 *iHH=0,TH1 *iHL=0) {
  std::string lName = std::string(iH0->GetName());
  TH1F *lHDiff  = new TH1F((lName+"Diff").c_str(),(lName+"Diff").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax());
  TH1F *lHDiffH = new TH1F((lName+"DiffH").c_str(),(lName+"DiffH").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax()); lHDiffH->SetLineWidth(1); lHDiffH->SetLineColor(kRed);
  TH1F *lHDiffL = new TH1F((lName+"DiffL").c_str(),(lName+"DiffL").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax()); lHDiffL->SetLineWidth(1); lHDiffL->SetLineColor(kBlue);
  lHDiff->SetFillColor(kViolet); lHDiff->SetFillStyle(1001); lHDiff->SetLineWidth(1);
  TH1F *lXHDiff1 = new TH1F((lName+"XDiff1").c_str(),(lName+"XDiff1").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax());
  TH1F *lXHDiff2 = new TH1F((lName+"XDiff2").c_str(),(lName+"XDiff2").c_str(),iH0->GetNbinsX(),iH0->GetXaxis()->GetXmin(),iH0->GetXaxis()->GetXmax());
  int i1 = 0;
  lXHDiff1->SetLineWidth(2); lXHDiff1->SetLineColor(kRed);
  lXHDiff2->SetLineWidth(2); lXHDiff2->SetLineColor(kRed);

  lHDiff->GetYaxis()->SetTitle("Ratio");
  lHDiff->GetYaxis()->SetRangeUser(0.8,1.2);
  lHDiff->GetYaxis()->SetTitleOffset(0.4);
  lHDiff->GetYaxis()->SetTitleSize(0.2);
  lHDiff->GetYaxis()->SetLabelSize(0.11);
  for(int i0 = 0; i0 < lHDiff->GetNbinsX()+1; i0++) {
    double lXCenter = lHDiff->GetBinCenter(i0);
    double lXVal     = iH0   ->GetBinContent(i0);
    //double lXValH    = iHH   ->GetBinContent(i0);
    //double lXValL    = iHL   ->GetBinContent(i0);
    lXHDiff1->SetBinContent(i0, 1.0);
    lXHDiff2->SetBinContent(i0, 1.0);
    while(iH1->GetBinCenter(i1) < lXCenter) {i1++;}
    if(iH1->GetBinContent(i0) > 0) lHDiff->SetBinContent(i0,lXVal      /(iH1->GetBinContent(i0)));
    if(iH1->GetBinContent(i0) > 0) lHDiff->SetBinError  (i0,sqrt(lXVal)/(iH1->GetBinContent(i0)));
    //if(iH1->GetBinContent(i0) > 0) lHDiffL->SetBinContent(i0,lXValL/(iH1->GetBinContent(i0)));
    //if(iH1->GetBinContent(i0) > 0) lHDiffH->SetBinContent(i0,lXValH/(iH1->GetBinContent(i0)));
    //if(iH1->GetBinContent(i0) > 0)  cout << "unc" << lXVal << " -- " << sqrt(lXVal)/(iH1->GetBinContent(i0)) << endl;
  }
  lHDiff->SetMarkerStyle(kFullCircle);
  lHDiff->Draw("EP");
  lXHDiff1->Draw("hist sames");
  lXHDiff2->Draw("hist sames");
  //lHDiffH ->Draw("hist sames");
  //lHDiffL ->Draw("hist sames");
}

void draw(TH1F** iH,const int iN,std::string iName,std::string iFName,int iHiggs=-1) { 
  TCanvas *lC0 = new TCanvas(("XX"+iName).c_str(),("XX"+iName).c_str(),fId,fYId,800,600); if(fId + 500 > 1100) fYId = (fYId + 400) % 1000; fId = (fId + 500) % 1100; 
  TLegend *lL = new TLegend(0.6,0.65,0.9,0.9); lL->SetFillColor(0); lL->SetBorderSize(0);
  for(int i0 = 0; i0 < iN; i0++) { 
    iH[i0]->SetFillColor(fColor[i0]);
    std::stringstream pSS; pSS << i0;
    cout << "===> " << iH[i0] << " -- " << iH[i0]->Integral() << endl;
    for(int i1 = i0+1; i1 < iN-1; i1++)  {if(i1 != iHiggs) iH[i0]->Add(iH[i1]);} 
    if(i0 != iN-1 && iH[i0]->Integral() > 0.0001) lL->AddEntry(iH[i0],fString[i0].c_str(),"f");
    iH[i0]->SetLineWidth(1); iH[i0]->SetLineColor(kBlack);
  }
  if(iHiggs != -1) iH[iHiggs]->Add(iH[0]);
  scale(iH,iH[iN-1]->Integral(),iN);
  iH[iN-1]->SetMarkerStyle(kFullCircle); //iH[iN-1]->GetYaxis()->SetRangeUser(0.1,1500);
  lC0->Divide(1,2); lC0->cd();  lC0->cd(1)->SetPad(0,0.2,1.0,1.0); gPad->SetLeftMargin(0.2) ;
  iH[iN-1]->Draw("EP");
  if(iHiggs != -1) iH[iHiggs]->Draw("hist sames");
  for(int i0 = 0; i0 < iN-1; i0++) {
    if(i0 == iHiggs) continue;
    iH[i0]->Draw("hist sames");
    lL->Draw();
  }
  iH[iN-1]->Draw("EP sames");
  lC0->cd(2)->SetPad(0,0,1.0,0.2); gPad->SetLeftMargin(0.2) ;
  drawDifference(iH[iN-1],iH[0]);//,iH[6],iH[7]);
  //lC0->SaveAs((iFName+".png").c_str());
}
TH1F* draw(TTree *iTree,int iId,int iType,TH1F *iH,std::string iVar,std::string iCut,std::string iSId) { 
  std::stringstream lName;   
  if(iH == 0 ) { 
    lName   << iSId <<  "Met"  << iId ; 
    if(iType > -1) lName << "SS"; 
    if(iType == -2) lName << "mt"; 
    if(iType == -3) lName << "ssmt";
    if(iType == -4) lName << "ssw";
  } else { 
    lName << iH->GetName();
  }
  std::stringstream lVar;    std::stringstream lSSVar; 
  std::string lProjMet = iVar;
  lVar   << lProjMet << ">>" << lName.str();
  lSSVar << lProjMet << ">>+" << lName.str();
  //std::string lCut     = "((abs(eta_2) < 1.47 || abs(eta_2) > 1.566) && pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.5 && abs(eta_2) < 2.3  && mt_1 < 4000 && pzeta < 200  && m_2 > -0.105)*"+fWeights[iId]+iCut;
  //std::string lSSCut   = "((abs(eta_2) < 1.47 || abs(eta_2) > 1.566) && pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.5 && abs(eta_2) < 2.3  && mt_1 < 4000 && pzeta < 200  && m_2 > -0.105)*"+fWeights[iId]+iCut;
  //std::string lMTCut   = "((abs(eta_2) < 1.47 || abs(eta_2) > 1.566) && pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.5 && abs(eta_2) < 2.3  && mt_1 > 70 &&                  m_2 > -0.105)*"+fWeights[iId]+iCut;
  std::string lMtCut = "400"; if(iVar != "mt_1")  lMtCut = "40";
  std::string lPZCut = "200"; if(iVar != "pzeta") lPZCut = "2500";//00";//25
  std::string lCut     = "(pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.1 && abs(eta_2) < 2.3  && mt_1 < "+lMtCut+" && pzeta < "+lPZCut+"  )*"+fWeights[iId]+iCut;
  std::string lSSCut   = "(pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.1 && abs(eta_2) < 2.3  && mt_1 < "+lMtCut+" && pzeta < "+lPZCut+"  )*"+fWeights[iId]+iCut;
  std::string lMTCut   = "(pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.1 && abs(eta_2) < 2.3  && mt_1 > 70                   )*"+fWeights[iId]+iCut;
  TH1F *lMet = iH;
  if(iH == 0) { 
    lMet = new TH1F(lName.str().c_str(),lName.str().c_str(),getNBins(lProjMet),getXMin(lProjMet),getXMax(lProjMet)); 
    if(iId != 6 && iId != 7) lMet->SetFillColor(fColor[iId]);
    if(iId == 6 || iId == 7) lMet->SetLineColor(fColor[iId]);
    lMet->SetLineWidth(1);
    lMet->GetXaxis()->SetTitle(getXAxis(lProjMet));//"m_{T} (GeV/c^{2})");//m_{sv}[GeV/c^{2}]");
    lMet->GetYaxis()->SetTitle(getYAxis(lProjMet));//"Events/5 GeV");
    lMet->Sumw2();
  }
  if(iType == -3) iTree->Draw(lSSVar.str()  .c_str()     ,(lMTCut  +"*(q_1*q_2 > 0)").c_str());
  if(iType == -2) iTree->Draw(lSSVar.str()  .c_str()     ,(lMTCut  +"*(q_1*q_2 < 0)").c_str());
  if(iType == -1) iTree->Draw(lVar.str()    .c_str()     ,(lCut    +"*(q_1*q_2 < 0)").c_str());
  if(iType >  -1) iTree->Draw(lSSVar.str()  .c_str()     ,(lSSCut  +"*(q_1*q_2 > 0)").c_str());
  if(iType == -1) cout << " ===> " << iId << " --> " << lCut << endl;
  return lMet;
}
TLegend * legend(int iN,TH1** iH) {
  TLegend *lL = new TLegend(0.20,0.7,0.35,0.9); lL->SetFillColor(0); lL->SetBorderSize(0);
  for(int i0 = 1; i0 < iN; i0++) {
    if(iH[i0]->Integral() > 0.1 && i0 % 2  == 0) lL->AddEntry(iH[i0],fString[i0].c_str(),"p");
    if(iH[i0]->Integral() > 0.1 && i0 % 2  != 0) lL->AddEntry(iH[i0],fString[i0].c_str(),"l");
  }
  return lL;
}
TH1F* draw(std::string iVar,TTree *iTree,int iId,std::string iCut,TH1F *iH = 0,std::string iSId="B") {
  std::stringstream lName;   lName   <<  "Met"  << iId << iSId;// << iVar;
  std::stringstream lVar;
  lVar   << iVar << ">>+" << lName.str();
  //std::string lCut   = "(pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.5 && abs(eta_2) < 2.5 && q_1*q_2 < 0)*"+fWeights[iId]+iCut;
  std::string lCut   = fWeights[iId]+iCut;
  cout << "Cut : " << lCut << endl;
  TH1F *lMet = iH;
  if(iH == 0) lMet = new TH1F(lName.str().c_str(),lName.str().c_str(),getNBins(iVar),getXMin(iVar),getXMax(iVar));
  lMet->SetLineColor(fColor[iId]);
  lMet->SetLineWidth(1);
  lMet->GetXaxis()->SetTitle(getXAxis(iVar));//"m_{T} (GeV/c^{2})");//m_{sv}[GeV/c^{2}]");                                                                                                                         
  lMet->GetYaxis()->SetTitle(getYAxis(iVar));//"Events/5 GeV");                                                                                                                                                    
  iTree->Draw(lVar.str()  .c_str()     ,(lCut    ).c_str());
  lMet->SetMarkerStyle(kFullCircle);
  lMet->SetLineWidth(2);
  return lMet;
}

TH2F* draw2D(std::string iVar,std::string iVar1,TTree *iTree,int iId,std::string iCut,TH2F *iH = 0) {
  std::stringstream lName;   lName   <<  "Met"  << iId;// << iVar;
  std::stringstream lVar;
  lVar   << iVar1 << ":" << iVar << ">>+" << lName.str();
  //std::string lCut   = "(pt_1 > 10 && pt_2 > 10 && abs(eta_1) < 2.5 && abs(eta_2) < 2.5 && q_1*q_2 < 0)*"+fWeights[iId]+iCut;
  std::string lCut   = fWeights[iId]+iCut;
  cout << "Cut : " << lCut << endl;
  TH2F *lMet = iH;
  if(iH == 0) lMet = new TH2F(lName.str().c_str(),lName.str().c_str(),getNBins(iVar),getXMin(iVar),getXMax(iVar),getNBins(iVar1),getXMin(iVar1),getXMax(iVar1));
  lMet->SetLineColor(fColor[iId]);
  lMet->SetLineWidth(1);
  lMet->GetXaxis()->SetTitle(getXAxis(iVar));
  lMet->GetYaxis()->SetTitle(getYAxis(iVar1));//"Events/5 GeV");     
  iTree->Draw(lVar.str()  .c_str()     ,(lCut    ).c_str());
  lMet->SetMarkerStyle(kFullCircle);
  lMet->SetLineWidth(2);
  return lMet;
}
TH1F* drawRaw(std::string iVar,TTree *iTree,int iId,std::string iCut,TH1F *iH = 0) {
  std::stringstream lName;   lName   <<  "Met"  << iId;
  std::stringstream lVar;
  lVar   << iVar << ">>+" << lName.str();
  std::string lCut   = iCut+"*"+fWeights[iId]+iCut;
  TH1F *lMet = iH;
  if(iH == 0) lMet = new TH1F(lName.str().c_str(),lName.str().c_str(),getNBins(iVar),getXMin(iVar),getXMax(iVar));
  lMet->SetLineColor(fColor[iId]);
  lMet->SetLineWidth(1);
  lMet->GetXaxis()->SetTitle(getXAxis(iVar));//"m_{T} (GeV/c^{2})");//m_{sv}[GeV/c^{2}]");                                                                                                                         
  lMet->GetYaxis()->SetTitle(getYAxis(iVar));//"Events/5 GeV");                                                                                                                                                    
  iTree->Draw(lVar.str()  .c_str()     ,(lCut    ).c_str());
  return lMet;
}
void drawBasic(TH1F** iH,const int iN,std::string iName) { 
  TCanvas *lC0 = new TCanvas(("XX"+iName).c_str(),("XX"+iName).c_str(),fId,fYId,600,600); if(fId + 500 > 1100) fYId = (fYId + 400) % 1000; fId = (fId + 500) % 1100; 
  TLegend *lL = new TLegend(0.6,0.65,0.9,0.9); lL->SetFillColor(0); lL->SetBorderSize(0);
  for(int i0 = 0; i0 < iN; i0++) { 
    std::stringstream pSS; pSS << i0;
    cout << "===> " << iH[i0] << " -- " << iH[i0]->Integral() << endl;
    if(iH[i0]->Integral() > 0.1) lL->AddEntry(iH[i0],fString[i0].c_str(),"f");
  }
  scale(iH,iH[iN-1]->Integral(),iN);
  //iH[iN-1]->SetMarkerStyle(kFullCircle); //iH[iN-1]->GetYaxis()->SetRangeUser(0.1,1500);
  //lC0->Divide(1,2); lC0->cd();  lC0->cd(1)->SetPad(0,0.2,1.0,1.0); gPad->SetLeftMargin(0.2) ;
  iH[iN-1]->Draw("hist");
  for(int i0 = 0; i0 < iN-1; i0++) {
    iH[i0]->Draw("hist sames");
    lL->Draw();
  }
  //iH[iN-1]->Draw("hist sames");
  //lC0->cd(2)->SetPad(0,0,1.0,0.2); gPad->SetLeftMargin(0.2) ;
  //drawDifference(iH[iN-1],iH[0]);//,iH[6],iH[7]);
  //lC0->SaveAs((iFName+".png").c_str());
}
