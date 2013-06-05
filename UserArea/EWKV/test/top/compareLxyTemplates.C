{
  /*
  TString dist("xbvslxy");
  TString files[]={"~/work/top_539/plotter_raw.root","~/work/top_539/plotter_syst.root"};
  TString proc[]={"t#bar{t}","t#bar{t}172.5"};
  TString title[]={"m_{b}=0.0 GeV","m_{b}=4.8 GeV"};
  int colors[]={1,kRed};
  TString xtitle="p_{T}(B)/p_{T}(b)";
  bool doReweight(false);  
  */

  TString dist("ptbvslxy");
  TString files[]={"~/work/top_539/plotter_raw.root","~/work/top_539/plotter_syst.root"};
  TString proc[]={"t#bar{t}","t#bar{t}systmcatnlo"};
  TString title[]={"Madgraph+Pythia","MC@NLO+Herwig"};
  int colors[]={1,kRed};
  TString xtitle="p_{T}(b) [GeV]";
  bool doReweight(true);

  //get distributions from file
  const size_t nprocs=sizeof(proc)/sizeof(TString);
  TH1 *kinH[nprocs], *lxyH[nprocs];
  for(size_t i=0; i<nprocs; i++)
    {
      TFile *fIn=TFile::Open(files[i]);
      TH2F *h=(TH2F *)fIn->Get(proc[i]+"/emu_"+dist);  
      h->Add((TH2F *)fIn->Get(proc[i]+"/mumu_"+dist));
      h->Add((TH2F *)fIn->Get(proc[i]+"/ee_"+dist));
      h->SetDirectory(0);

      TString pf(""); pf+=i;
      kinH[i]=h->ProjectionX("kin"+pf); 
      kinH[i]->SetTitle(title[i]);
      kinH[i]->SetDirectory(0);
      kinH[i]->SetFillStyle(0);
      kinH[i]->SetLineColor(colors[i]);
      kinH[i]->Rebin(2);
      kinH[i]->GetYaxis()->SetTitle("Events (a.u.)");
      kinH[i]->GetXaxis()->SetTitle(xtitle);
      if(i) kinH[i]->Scale(kinH[0]->Integral()/kinH[i]->Integral());

      lxyH[i]=h->ProjectionY("kin"+pf);
      if(i>0 && doReweight)
	{
	  lxyH[i]->Reset("ICE");
	  TH1 *kinRatio=(TH1 *)kinH[0]->Clone("kinratio");
	  kinRatio->Divide(kinH[i]);
	  TGraph *gr=new TGraph(kinRatio);
	  for(int xbin=1; xbin<=h->GetXaxis()->GetNbins(); xbin++)
	    {
	      //float wgt=kinRatio->GetBinContent(xbin);
	      float wgt=gr->Eval( kinH[i]->GetXaxis()->GetBinCenter(xbin) );
	      TH1D *tmp = (TH1D *)h->ProjectionY("tmp",xbin,xbin);
	      lxyH[i]->Add(tmp,wgt);
	    }
	  kinRatio->Delete();
	}
      lxyH[i]->GetYaxis()->SetTitle("Events (a.u.)");
      lxyH[i]->GetXaxis()->SetTitle("L_{xy} [cm]");
      lxyH[i]->SetTitle(title[i]);
      lxyH[i]->SetDirectory(0);
      lxyH[i]->SetFillStyle(0);
      lxyH[i]->SetLineColor(colors[i]);

      fIn->Close();
    }


  TCanvas *c=new TCanvas("c","c",800,400);
  c->Divide(2,1);
  c->cd(1);
  for(size_t i=0; i<nprocs; i++) kinH[i]->Draw( i==0 ? "hist" : "histsame" );

  TPaveText *pt = new TPaveText(0.1,0.95,0.9,0.99,"brNDC");
  pt->SetTextAlign(12);
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->AddText("CMS simulation,#sqrt{s}=8 TeV");
  pt->Draw();


  c->cd(2);

  pt= new TPaveText(0.6,0.6,0.9,0.8,"brNDC");
  pt->SetTextAlign(12);
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);

  TLegend *leg=new TLegend(0.6,0.8,0.9,0.9,"","brNDC");
  leg->SetTextAlign(12);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->AddEntry(lxyH[0],lxyH[0]->GetTitle(),"f");

  char buf[200];  
  Double_t xq[4],yq[4],yq2[4];
  xq[0]=0.25;
  xq[1]=0.5;
  xq[2]=0.75;
  xq[3]=1.0;
  lxyH[0]->GetQuantiles(4,yq,xq);
  for(size_t i=0; i<nprocs; i++){
    lxyH[i]->Draw( i==0 ? "hist" : "histsame" );
    if(i==0) continue;
    lxyH[i]->GetQuantiles(4,yq2,xq);
    leg->AddEntry(lxyH[i],lxyH[i]->GetTitle(),"f");

    double mean_err=sqrt(pow(lxyH[0]->GetMeanError(),2)+pow(lxyH[i]->GetMeanError(),2));
    double varMean=TMath::Abs(lxyH[0]->GetMean()-lxyH[i]->GetMean());
    double varMedian=TMath::Abs(yq[1]-yq2[1]);
    sprintf(buf,"#Delta <L_{xy}> = %3.1f #pm %3.1f #mum",1e4*varMean,1e4*mean_err);
    pt->AddText(buf);
    sprintf(buf,"#Delta L_{xy}^{0.5q} = %3.1f #pm %3.1f #mum",1e4*varMedian,1e4*mean_err*2./TMath::Pi());
    pt->AddText(buf);
  }
  leg->Draw();
  pt->Draw();

}
