
void CMSPrelim()
{
  
  
  TPaveText* cmsprel  = new TPaveText(0.19, 0.895 , 0.19 + 0.30, 0.895 + 0.10, "NDC");
  cmsprel->SetBorderSize(   0 );
  cmsprel->SetFillStyle(    0 );
  cmsprel->SetTextAlign(   12 );
  cmsprel->SetTextColor( 1 );
  cmsprel->SetTextFont ( 62 );

  //cmsprel->SetTextSize ( 0.035 );
  //cmsprel->AddText("CMS Preliminary,  #sqrt{s}=7-8 TeV,  L=24.3 fb^{-1},  H#rightarrow#tau#tau");

  //cmsprel->SetTextSize ( 0.027 );
  //cmsprel->AddText("CMS Preliminary, L=4.9 fb^{-1}@#sqrt{s}=7 TeV, L=19.3 fb^{-1}@#sqrt{s}=8 TeV, H#rightarrow#tau#tau");
  //cmsprel->AddText("CMS Preliminary,  #sqrt{s}=7 TeV, L=4.9 fb^{-1}; #sqrt{s}=8 TeV, L=19.3 fb^{-1}; H#rightarrow#tau#tau");

  cmsprel->SetTextSize ( 0.030 );
  cmsprel->AddText("CMS Preliminary,  H#rightarrow#tau#tau,  4.9 fb^{-1} at 7 TeV, 19.4 fb^{-1} at 8 TeV");

  cmsprel->Draw();

}


void plotNLLMassToys(long injmass){


  TString filenamesb=TString("nllsb_")+injmass;
  TString filenameb=TString("nllb_")+injmass;

  TH1F * H[8];
  TH1F * HDiff[8];
  TH1F * HDiffFine[8];
  TH1F * HSigma[8];
  TF1* GScanToy[500];
  for(Int_t m=0;m<8;m++){
    H[m]=new TH1F(TString("H")+(long)(110+5*m),TString("mass=")+(long)(110+5*m),50,-200,-25);    
    HDiff[m]=new TH1F(TString("HDiff")+(long)(110+5*m),TString("mass=")+(long)(110+5*m),20,0.0,8);    
    HDiffFine[m]=new TH1F(TString("HDiffFine")+(long)(110+5*m),TString("mass=")+(long)(110+5*m),1000,0.0,8);    
    HSigma[m]=new TH1F(TString("HSigma")+(long)(110+5*m),TString("mass=")+(long)(110+5*m),50,10,10);     
  }


  TH1F HMass("HMass","",8,110-2.5,150-2.5);
  TH1F HPull("HPull","",11,-5.5,5.5);
  TH1F HSigmaLow("HSigmaLow","",8,0,30);
  HSigmaLow.GetXaxis()->SetTitle("#sigma_{low}");
  TH1F HSigmaHigh("HSigmaHigh","",8,0,30);
  HSigmaHigh.GetXaxis()->SetTitle("#sigma_{high}");
  TH2F HSigma2D("HSigma2D","",8,0,20,8,0,20); 
  HSigma2D.GetYaxis()->SetTitle("#sigma_{high}");
  HSigma2D.GetXaxis()->SetTitle("#sigma_{low}");
  TH2F HBestFitvsSigmaHigh("HBestFitvsSigmaHigh","",8,110-2.5,145+2.5,8,110-2.5,145+2.5);
  HBestFitvsSigmaHigh.GetYaxis()->SetTitle("#sigma_{high}");
  HBestFitvsSigmaHigh.GetXaxis()->SetTitle("m_{bestfit}");
  TH1F HSigmaFull("HSigmaFull","",20,0,50);
  HSigmaFull.GetXaxis()->SetTitle("#sigma_{low}+#sigma_{high}");

  ifstream file;
  file.open((filenamesb+".txt").Data());  

  ifstream fileb;
  fileb.open((filenameb+".txt").Data());  

  bool GoodToy[500];
  int n;
  float nll[8];
  float nllb[8];
  int ngood=0;
  while(!file.eof()){

    file>>n>>nll[0]>>nll[1]>>nll[2]>>nll[3]>>nll[4]>>nll[5]>>nll[6]>>nll[7];
    fileb>>n>>nllb[0]>>nllb[1]>>nllb[2]>>nllb[3]>>nllb[4]>>nllb[5]>>nllb[6]>>nllb[7];
    //cout<<n<<nll[0]<<nll[1]<<nll[2]<<nll[3]<<nll[4]<<nll[5]<<nll[6]<<nll[7]<<endl;

    GoodToy[n]=1;
    for(Int_t m=0;m<8;m++)
      if(nll[m]==0.) GoodToy[n]=0;
    if(!GoodToy[n])continue;

    float minnll=100.;
    int minm=0;
    float minmass=0.;
    for(Int_t m=0;m<8;m++)
      if(minnll>nll[m]){
	minnll = nll[m];
	minm   = m;
      }
    minmass=110+minm*5;

    for(Int_t m=0;m<8;m++){
      H[m]->Fill(nll[m]);
      HDiff[m]->Fill(nll[m]-minnll);
      HDiffFine[m]->Fill(nll[m]-minnll);
    }
    
    //////scan of each toy
    GScanToy[n]=new TF1(TString("GScanToy")+(long)(110+5*m),"(110<=x&&x<115)*([0]+([1]-[0])*(x-110)/5) + (115<=x&&x<120)*([1]+([2]-[1])*(x-115)/5)  + (120<=x&&x<125)*([2]+([3]-[2])*(x-120)/5) + (125<=x&&x<130)*([3]+([4]-[3])*(x-125)/5) + (130<=x&&x<135)*([4]+([5]-[4])*(x-130)/5) + (135<=x&&x<140)*([5]+([6]-[5])*(x-135)/5) + (140<=x&&x<145)*([6]+([7]-[6])*(x-140)/5)",110,145);
    for(Int_t m=0;m<8;m++){
      GScanToy[n]->SetParameter(m,(nll[m]-minnll));
    }

    //for this toy find the uncertainty 
    float x1sigmalow=0;
    float x1sigmahigh=0;
    float y1sigmalow=1000;
    float y1sigmahigh=1000;
    for(Int_t m=0;m<1000;m++){
      float xlow=110+m*(145-110)/1000.;
      float xhigh=145+m*(110-145)/1000.;
      float ylow=GScanToy[n]->Eval(xlow);
      float yhigh=GScanToy[n]->Eval(xhigh);
      if(ylow<y1sigmalow && xlow<110+minm*5 && ylow>0.5){
	y1sigmalow=ylow;
	x1sigmalow=xlow;
      }
      if(yhigh<y1sigmahigh && xhigh>110+minm*5 && yhigh>0.5){
	y1sigmahigh=yhigh;
	x1sigmahigh=xhigh;
      }
    }
    //cout<<110+minm*5<<" "<<x1sigmalow<<" "<<x1sigmahigh<<endl;
    float sigmamass=0.;
    if( x1sigmalow !=0  && x1sigmahigh ==0)sigmamass = minmass - x1sigmalow;
    if( x1sigmahigh!=0  && x1sigmalow  ==0)sigmamass = x1sigmahigh - minmass;
    if( x1sigmalow !=0  && x1sigmahigh !=0)sigmamass = ((minmass - x1sigmalow) + (x1sigmahigh - minmass))/2.;
    if(sigmamass!=0.)HPull.Fill((minmass-injmass)/sigmamass);

    if( x1sigmalow !=0 ) {
      HSigmaLow.Fill(minmass-x1sigmalow);
    }
    if( x1sigmahigh !=0 ) {
      HSigmaHigh.Fill(x1sigmahigh-minmass);
      HBestFitvsSigmaHigh.Fill(minmass,x1sigmahigh);
    }
    if( x1sigmalow !=0  && x1sigmahigh !=0 ) {
      HSigma2D.Fill(minmass-x1sigmalow,x1sigmahigh-minmass);
      HSigmaFull.Fill(x1sigmahigh-x1sigmalow);
    }


    /////Find best fit mass 
    HMass.Fill(110 + minm*5);

    ngood++;
  }
  file.close();
  fileb.close();
  cout<<"Number of good toys = "<<ngood<<endl;


  float MinNLL=1000;
  for(Int_t m=0;m<8;m++)
    if(MinNLL>H[m]->GetMean())
      MinNLL=H[m]->GetMean();

  file.open((filenamesb+".txt").Data());  
  fileb.open((filenameb+".txt").Data());  
  while(!file.eof()){
    file>>n>>nll[0]>>nll[1]>>nll[2]>>nll[3]>>nll[4]>>nll[5]>>nll[6]>>nll[7];
    if(!GoodToy[n])continue;
    for(Int_t m=0;m<8;m++)
      HSigma[m]->Fill(nll[m]-MinNLL);
    
  }
  file.close();
  fileb.close();

  TCanvas C("C","",600,600);
  C.Print(filenamesb+".ps[");

  //fill what is effectively the asimov dataset 
  TGraph GScan;
  float minnll=100.;
  for(Int_t m=0;m<8;m++)
    if(minnll>H[m]->GetMean())
      minnll=H[m]->GetMean();

  const double quantiles[5]={0.05,0.16,0.5,0.84,0.95};
  const double xq[8][5];

  TLine QLine;
  QLine.SetLineColor(2);
  
  ///////////////////
  for(Int_t m=0;m<8;m++){
    HDiffFine[m]->GetQuantiles(5,xq[m],quantiles);

    C.Clear();
    //HSigma[m]->Draw("pe");
    //H[m]->Draw("pe");
    //HDiff[m]->Draw("hist");

    HDiffFine[m]->Draw("hist");
    for(Int_t q=0;q<5;q++){
      QLine.DrawLine(xq[m][q],0,xq[m][q],HDiffFine[m]->GetMaximum());     
    }

    C.Print(filenamesb+".ps");
    if(m==7)C.Print(filenamesb+"_DNLLDistribution.png");

    GScan.SetPoint(m,110+m*5,H[m]->GetMean()-minnll);
  }

  TGraph* HBands[5];
  for(Int_t q=0;q<5;q++){
    HBands[q]=new TGraph();
    HBands[q]->SetPoint(0,110,0);
    for(Int_t m=0;m<8;m++){
      HBands[q]->SetPoint(m+1,110+m*5,xq[m][q]);
    }
    HBands[q]->SetPoint(9,145,0);
  }
  HBands[0]->SetFillColor(kYellow);      HBands[0]->SetLineColor(kYellow); HBands[0]->SetLineWidth(0);
  HBands[1]->SetFillColor(kYellow);      HBands[1]->SetLineColor(kYellow); HBands[1]->SetLineWidth(0);
  HBands[2]->SetFillColor(0);            HBands[2]->SetLineColor(kBlue); 
  HBands[3]->SetFillColor(kGreen);       HBands[3]->SetLineColor(kGreen);
  HBands[4]->SetFillColor(kYellow);      HBands[4]->SetLineColor(kYellow);

      
  /////////////////////Likelihood scan
  TH1F HScanFrame("HScanFrame","",1,110,145);
  HScanFrame.GetYaxis()->SetRangeUser(0,4);
  //HScanFrame.GetYaxis()->SetTitle("#scale[1]{#Delta} -ln(#font[132]{L})");
  HScanFrame.GetYaxis()->SetTitle("#scale[1]{#Delta}( -ln L )");
  HScanFrame.GetXaxis()->SetTitle("#bf{m_{H} [GeV]}");
  TLine line;
  line.SetLineColor(2);
  C.Clear();
  HScanFrame.Draw();
  for(Int_t q=4;q>=0;q--){
    if(q!=2) HBands[q]->Draw("Fsame");
  }
 
  GScan.SetLineColor(kBlue);
  GScan.Draw("lsame");


  TFile FData("likelihood-mass-scan.root","READ");
  TGraph*DataInput=(TGraph*)FData.Get("b+_All_final_1nd/mass_scan");
  
  float mindata=100.;
  float minmassdata=0.;
  for(Int_t m=0;m<8;m++){
    double x; double y;
    DataInput->GetPoint(m*5,x,y);
    if(mindata>y){
      mindata=y;
      minmassdata=110+m*5;
    }
  }

  TGraph GData;
  for(Int_t m=0;m<8;m++){
    double x; double y;
    DataInput->GetPoint(m*5,x,y);
    GData.SetPoint(m,x,y-mindata);
  }

  GData.Draw("plsame");
  //DataInput->Draw("plsame");
  //GScanToy[0]->Draw("lsame");

  C.RedrawAxis();
  line.DrawLine(110,0.5,145,0.5);
  line.DrawLine(110,1.92,145,1.92);

  float datasigmahigh=8.55;
  float datasigmalow=5.85;
  TLine verticalline;
  verticalline.SetLineStyle(2);
  verticalline.DrawLine(minmassdata-datasigmalow,0,minmassdata-datasigmalow,0.5);
  verticalline.DrawLine(minmassdata+datasigmahigh,0,minmassdata+datasigmahigh,0.5);


  TLegend legend;
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  legend.AddEntry(&GData,"observed","LP");  
  legend.AddEntry(&GScan,"H(125 GeV) expected","L");
  legend.AddEntry(HBands[3],"#pm 1#sigma expected","F");
  legend.AddEntry(HBands[4],"#pm 2#sigma expected","F");
  legend.SetX1NDC(0.37);
  legend.SetX2NDC(0.85);
  legend.SetY1NDC(0.67);
  legend.SetY2NDC(0.88);
  legend.SetTextSize(.04);
  legend.SetTextAlign(12);
  legend.Draw();

  TLatex text;
  text.SetTextColor(2);
  text.SetTextSize(0.04);
  text.DrawLatex(142,0.5+0.05,"1#sigma");
  text.DrawLatex(142,1.92+0.05,"2#sigma");
  
  //CMSPrelim("CMS Preliminary,  H#rightarrow#tau#tau,  L = 24.3 fb^{-1}"); //#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}
  //CMSPrelim("CMS Prelim.,  H#rightarrow#tau#tau,  #sqrt{s} = 7 - 8 TeV,  L = 24.3 fb^{-1}"); 
  //CMSPrelim(""); 
  //CMSPrelim("CMS Preliminary, #sqrt{s}=7 TeV, L=4.9 fb^{-1}, #sqrt{s}=8 TeV, L=19.3 fb^{-1}, H#rightarrow#tau#tau"); 
  CMSPrelim(); 
 
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_DNLL.png");
  C.Print(filenamesb+"_DNLL.pdf");


  //////////////////////Pull distribution
  TH1F HMassFrame("HMassFrame","",1,100,155);
  HMassFrame.GetYaxis()->SetRangeUser(0,HMass.GetMaximum()*1.3);
  HMassFrame.GetYaxis()->SetTitle("# of toys");
  HMassFrame.GetXaxis()->SetTitle("m_{H}   [GeV]");


  cout<<" Fit with 3 gaussians "<<endl;
  TF1 Gaus("Gaus","[0]*exp(-0.5*(x-[1])**2/([2]*[2])) + [3]*exp(-0.5*(x-[5])**2/([2]*[2])) + [4]*exp(-0.5*(x-[6])**2/([2]*[2]))",110,145);
  Gaus.SetParLimits(0,0,10000);
  Gaus.SetParLimits(1,110,145);
  Gaus.SetParLimits(2,1,20);
  Gaus.SetParLimits(3,0,10000);
  Gaus.SetParLimits(4,0,10000);
  Gaus.FixParameter(5,110-(injmass-110));
  Gaus.FixParameter(6,145+(145-injmass));
  C.Clear();
  HMass.Fit(&Gaus);
  char title[100];
  sprintf(title,"Mean = %.1f +- %.1f  Sigma = %.1f +- %.1f",Gaus.GetParameter(1),Gaus.GetParError(1),Gaus.GetParameter(2),Gaus.GetParError(2));
  HMassFrame.SetTitle(title);
  HMassFrame.Draw("hist");
  HMass.Draw("histpesame");
  Gaus.SetLineColor(4);
  Gaus.Draw("lsame");
  TF1 GausSig("GausSig","[0]*exp(-0.5*(x-[1])**2/([2]*[2]))",90,160);
  GausSig.SetParameter(0,Gaus.GetParameter(0));
  GausSig.SetParameter(1,Gaus.GetParameter(1));
  GausSig.SetParameter(2,Gaus.GetParameter(2));
  GausSig.SetLineColor(2);
  GausSig.Draw("lsame");
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_MassUnc.png");
  C.Print(filenamesb+"_MassUnc.pdf");

  cout<<" Fit with 1 gaussian "<<endl;
  C.Clear();
  TF1 GausCore("GausCore","[0]*exp(-0.5*(x-[1])**2/([2]*[2]))",110,145);
  GausCore.SetParLimits(0,0.1,10000);
  GausCore.SetParLimits(1,injmass-5.0,injmass+5.0);
  GausCore.SetParLimits(2,1,20);
  //HMass.Fit(&GausCore,"","",injmass-5.5,injmass+5.5);
  HMass.Fit(&GausCore,"","LL",injmass-10.0,injmass+10.0);
  sprintf(title,"Mean = %.1f +- %.1f  Sigma = %.1f +- %.1f",GausCore.GetParameter(1),GausCore.GetParError(1),GausCore.GetParameter(2),GausCore.GetParError(2));
  HMassFrame.SetTitle(title);
  HMassFrame.Draw("hist");
  HMass.Draw("histpesame");
  GausCore.SetLineColor(2);
  GausCore.Draw("lsame");
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_MassUncCore.png");
  C.Print(filenamesb+"_MassUncCore.pdf");

  C.Clear();
  cout<<"# of Pull Toys = "<<HPull.Integral()<<endl;
  TF1 GausPull("GausPull","[0]*exp(-0.5*(x-[1])**2/([2]*[2]))",-5,5);
  GausPull.SetParLimits(0,0.,1000);
  GausPull.SetParLimits(1,-1,1.1);
  GausPull.SetParLimits(2,0.2,3);
  GausPull.SetLineColor(4);
  HPull.Fit(&GausPull,"","I",-2,2);
  sprintf(title,"Mean = %.2f +- %.2f  Sigma = %.2f +- %.2f",GausPull.GetParameter(1),GausPull.GetParError(1),GausPull.GetParameter(2),GausPull.GetParError(2));
  HPull.SetTitle(title);
  HPull.GetXaxis()->SetTitle("(  m_{best-fit}  -  m_{injected}  )/#sigma_{m}");
  HPull.GetYaxis()->SetTitle("# of toys");
  HPull.Draw("pe");
  GausPull.Draw("lsame");
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_MassPull.png");
  C.Print(filenamesb+"_MassPull.pdf");

  C.Clear();
  TLine sigmaline;
  cout<<" # of toys for 2D uncertainty plot "<<HSigma2D.Integral()<<endl; 
  TF1 SigmaPol("SigmaPol","[0]+[1]*x",0,20);
  //HSigma2D.Fit(&SigmaPol);
  HSigma2D.Draw("colz");
  //SigmaPol.Draw("lsame");
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_sigma2D.png");
  C.Clear();
  cout<<" # of toys for sigmalow "<<HSigmaLow.Integral()<<endl; 
  HSigmaLow.Draw();
  sigmaline.DrawLine(datasigmalow,0,datasigmalow,HSigmaLow.GetMaximum());
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_sigmaLow.png");
  C.Clear();
  cout<<" # of toys for sigmahigh "<<HSigmaHigh.Integral()<<endl; 
  HSigmaHigh.Draw();
  sigmaline.DrawLine(datasigmahigh,0,datasigmahigh,HSigmaHigh.GetMaximum());
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_sigmaHigh.png");
  C.Clear();
  cout<<" # of toys for sigmafull "<<HSigmaFull.Integral()<<endl; 
  HSigmaFull.Draw();
  sigmaline.DrawLine(datasigmalow+datasigmahigh,0,datasigmalow+datasigmahigh,HSigmaFull.GetMaximum());
  C.Print(filenamesb+".ps");
  C.Print(filenamesb+"_sigmafull.png");
  

  C.Clear();
  TF1 SigmaBFitvsSigma("SigmaBFitvsSigma","[0]+[1]*x",0,20);
  //HBestFitvsSigmaHigh.Fit(&SigmaBFitvsSigma);
  HBestFitvsSigmaHigh.Draw("colz");
  //SigmaBFitvsSigma.Draw("lsame");
  C.Print(filenamesb+".ps");


  C.Print(filenamesb+".ps]");
  gROOT->ProcessLine(".q");
}
  
