{
  TFile *fIn=TFile::Open("~/work/top_539/plotter_raw.root");
  TH2F *h=(TH2F *)fIn->Get("t#bar{t}/emu_xbvslxy");  h->SetDirectory(0);
  h->Add((TH2F *)fIn->Get("t#bar{t}/mumu_xbvslxy"));
  h->Add((TH2F *)fIn->Get("t#bar{t}/ee_xbvslxy"));
  h->GetXaxis()->SetTitle("p_{T}(B)/p_{T}(b)");
  fIn->Close();


  //xb
  TH1D *xbH=h->ProjectionX("xb");
  xbH->SetDirectory(0);

  fIn=TFile::Open("../../data/jec/xb_weights.root");
  TH1F *pythia_Z2_nominal  = (TH1F *) fIn->Get("pythia_Z2_nominal");     
  pythia_Z2_nominal->Rebin(5);
  pythia_Z2_nominal->Scale(xbH->Integral()/pythia_Z2_nominal->Integral());
  pythia_Z2_nominal->SetDirectory(0);

  TH1F *pythia_Z2_corcella = (TH1F *) fIn->Get("pythia_Z2_corcella");     
  pythia_Z2_corcella->Rebin(5);
  pythia_Z2_corcella->Scale(xbH->Integral()/pythia_Z2_corcella->Integral());   
  pythia_Z2_corcella->SetDirectory(0);

  TH1F *pythia_p11         = (TH1F *) fIn->Get("pythia_Z2_p11");                      
  pythia_p11->Rebin(5);
  pythia_p11->Scale(xbH->Integral()/pythia_p11->Integral());            
  pythia_p11->SetDirectory(0);

  TGraph *pythia_Z2_nominalGr  = new TGraph(pythia_Z2_nominal);
  TGraph *pythia_Z2_corcellaGr = new TGraph(pythia_Z2_corcella);
  TGraph *pythia_p11Gr         = new TGraph(pythia_p11);

  TGraphSmooth *gs = new TGraphSmooth(TString("z2smooth"));
  pythia_Z2_nominalGr = gs->SmoothSuper(pythia_Z2_nominalGr,"",3);
  pythia_Z2_nominalGr->SetFillStyle(0);
  pythia_Z2_nominalGr->SetLineColor(pythia_Z2_nominal->GetLineColor());
  gs = new TGraphSmooth(TString("corcsmooth"));
  pythia_Z2_corcellaGr = gs->SmoothSuper(pythia_Z2_corcellaGr,"",3);
  pythia_Z2_corcellaGr->SetFillStyle(0);
  pythia_Z2_corcellaGr->SetLineColor(pythia_Z2_corcella->GetLineColor());
  gs = new TGraphSmooth(TString("p11smooth"));
  pythia_p11Gr= gs->SmoothSuper(pythia_p11Gr,"",3);
  pythia_p11Gr->SetFillStyle(0);
  pythia_p11Gr->SetLineColor(pythia_p11->GetLineColor());
  fIn->Close();

  //weights
  TGraph *orig_to_nominal     = new TGraph;
  TGraph *nominal_to_corcella = new TGraph; nominal_to_corcella->SetLineColor(kRed);
  TGraph *nominal_to_p11      = new TGraph; nominal_to_p11->SetLineColor(kGreen);
  double wgt(1.0);
  for(int ibin=1; ibin<=xbH->GetXaxis()->GetNbins(); ibin++)
    {
      //weights to new xb shape
      double x=xbH->GetBinCenter(ibin);
      double y=xbH->GetBinContent(ibin);
      y=1;
      if(y==0) continue;
      
      wgt=pythia_Z2_nominalGr->Eval(x)/y;
      orig_to_nominal->SetPoint(ibin-1,x,wgt);
      
      wgt=pythia_Z2_corcellaGr->Eval(x)/y;
      nominal_to_corcella->SetPoint(ibin-1,x,wgt);

      wgt=pythia_p11Gr->Eval(x)/y;
      nominal_to_p11->SetPoint(ibin-1,x,wgt);
    }
  
  //generate displaced distributions
  TH1D *lxy               = h->ProjectionY("lxy"); 
  TH1D *lxy_nominal       = (TH1D *) lxy->Clone("lxynom");     lxy_nominal->Reset("ICE");
  TH1D *lxy_corcella      = (TH1D *) lxy->Clone("lxycor");     lxy_corcella->Reset("ICE");   lxy_corcella->SetLineColor(kRed);
  TH1D *lxy_p11           = (TH1D *) lxy->Clone("lxyp11");     lxy_p11->Reset("ICE");        lxy_p11->SetLineColor(kGreen);
  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      TH1D *tmp = (TH1D *)h->ProjectionY("tmp",ibin,ibin);
      double ixb = h->GetXaxis()->GetBinCenter(ibin);
      wgt = orig_to_nominal->Eval(ixb);
      lxy_nominal    ->Add(tmp,  wgt);
      lxy_corcella   ->Add(tmp , nominal_to_corcella->Eval(ixb) );
      lxy_p11        ->Add(tmp , nominal_to_p11->Eval(ixb) );
    }
  lxy_nominal->Scale( lxy->Integral()/lxy_nominal->Integral());
  lxy_corcella->Scale( lxy->Integral()/lxy_corcella->Integral());
  lxy_p11->Scale( lxy->Integral()/lxy_p11->Integral());


  //show the results
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(1);
  gStyle->SetPalette(53);

  TCanvas *c=new TCanvas("c","c",1200,400);
  c->Divide(3,1);
  c->cd(1); 
   pythia_Z2_nominal ->Draw("hist"); pythia_Z2_nominal->SetLineWidth(2);
  pythia_Z2_corcella->Draw("histsame");
  pythia_p11        ->Draw("histsame");

  //  pythia_Z2_nominalGr ->Draw("al"); pythia_Z2_nominalGr->SetLineWidth(2);
  //pythia_Z2_nominalGr->GetYaxis()->SetRangeUser(0,1000);
  //  pythia_Z2_corcellaGr->Draw("l");
  //pythia_p11Gr        ->Draw("l");
  //  xbH->Draw("same");
  
  TLegend *leg = new TLegend(0.5,0.5,0.9,0.9);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->AddEntry(pythia_Z2_nominalGr,"Z2","f");
  leg->AddEntry(pythia_Z2_corcellaGr,"Z2+EPJC65 (2010) 171-180","f");
  leg->AddEntry(pythia_p11Gr,"P11","f");
  leg->Draw();

  TPaveText *pt= new TPaveText(0.1,0.95,0.9,0.99,"brNDC");
  pt->SetTextAlign(12);
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->AddText("CMS simulation,#sqrt{s}=8 TeV");
  pt->Draw();
  TPad*p=  (TPad *)c->cd(2); h->Draw("colz");
  p->SetLogz();
  TPaveText *pt= new TPaveText(0.15,0.85,0.6,0.9,"brNDC");
  pt->SetTextAlign(12);
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);
  char buf[200];
  sprintf(buf,"#rho=%3.4f",h->GetCorrelationFactor());
  pt->AddText(buf);
  pt->Draw();
 

  c->cd(3); 
  lxy_nominal->Draw("hist");
  lxy_corcella->Draw("histsame");
  lxy_p11->Draw("histsame");

  Double_t xq[4],yqnom[4],yq_corcella[4],yq_p11[4];
  xq[0]=0.25;
  xq[1]=0.5;
  xq[2]=0.75;
  xq[3]=1.0;
  lxy_nominal->GetQuantiles(4,yqnom,xq);
  lxy_corcella->GetQuantiles(4,yq_corcella,xq);
  lxy_p11->GetQuantiles(4,yq_p11,xq);

  TPaveText *pt= new TPaveText(0.15,0.85,0.6,0.9,"brNDC");
  pt->SetTextAlign(12);
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);

  char buf[200];  
  
  double mean_err=h->GetMeanError(1);

  double var1=TMath::Abs(lxy_corcella->GetMean()-lxy_p11->GetMean());
  double var2=TMath::Abs(lxy_p11->GetMean()-lxy_nominal->GetMean());
  double var =TMath::Max(var1,var2);
  sprintf(buf,"#Delta <L_{xy}> = %3.1f #pm %3.1f #mum",1e4*var,1e4*mean_err*sqrt(2)); cout << buf << endl;
  pt->AddText(buf);


  var1=TMath::Abs(yq_corcella[1]-yq_p11[1]);
  var2=TMath::Abs(yq_p11[1]-yqnom[1]);
  var =TMath::Max(var1,var2);
  sprintf(buf,"#Delta L_{xy}^{0.5q} = %3.1f #pm %3.1f #mum",1e4*var,1e4*mean_err*sqrt(2)*2./TMath::Pi()); cout << buf << endl; 
  pt->AddText(buf);
  
  pt->Draw();


  TCanvas *c2=new TCanvas("c2","c2",600,600);
  c2->cd();
  orig_to_nominal->Draw("al");
  orig_to_nominal->GetXaxis()->SetTitle( xbH->GetXaxis()->GetTitle() );
   orig_to_nominal->GetYaxis()->SetTitle( "weight" );
  nominal_to_corcella->Draw("l");
  nominal_to_p11->Draw("l");
}
