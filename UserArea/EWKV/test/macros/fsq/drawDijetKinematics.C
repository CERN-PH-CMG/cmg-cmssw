{
  TString url[] ={"0.00_0.00_0.00.root",
		  "0.05_0.00_0.00.root",
		  "0.00_0.05_0.00.root",
		  "0.00_0.05_0.05.root"
  };
  
  TString tit[] ={"#lambda^{Z}=0     #Delta#kappa^{Z}=0     #Delta g_{1}^{Z}=0", 
		  "#lambda^{Z}=0.05  #Delta#kappa^{Z}=0     #Delta g_{1}^{Z}=0",      
		  "#lambda^{Z}=0     #Delta#kappa^{Z}=0.05  #Delta g_{1}^{Z}=0",
		  "#lambda^{Z}=0     #Delta#kappa^{Z}=0     #Delta g_{1}^{Z}=0.05"
  }; 
  Int_t lw[]    ={2, 1, 1,1};
  Int_t line[]  ={1,  9,                  1, 1};
  Int_t color[] ={1,  41,                 29,15,29,30};
  Int_t fill[]  ={0,  3004,               3001, 3002};

   TString plot="spt";  
  //TString plot="zpt";  
  //TString plot="mjj";
  //TString plot="detajj";
  //TString plot="leadetaj";
  //TString plot="traileretaj";

  gROOT->SetStyle("Pub");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TCanvas *c=new TCanvas("c","c",600,600);

  TLegend *leg=new TLegend(0.5,0.6,0.85,0.85,"","brNDC");
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);

  for(size_t i=0; i<sizeof(url)/sizeof(TString); i++)
    {
      TFile *fIn=TFile::Open(url[i]);
      if(fIn==0) continue;
      if(fIn->IsZombie()) continue;

      TH1F *h=(TH1F *) fIn->Get(plot);
      h->SetDirectory(0);
      fIn->Close();
      
      //h->Rebin(4);
      h->SetLineWidth(lw[i]);
      h->SetLineStyle(line[i]);
      h->SetTitle(tit[i]);
      h->SetFillStyle(fill[i]);
      h->SetFillColor(color[i]);
      h->GetYaxis()->SetTitle("Events (normalized)");
      h->GetXaxis()->SetTitleOffset(1.6);
      h->GetYaxis()->SetTitleOffset(1.8);
      h->DrawNormalized( i==0 ? "hist" : "histsame");
      leg->AddEntry(h,h->GetTitle(),"f");

    }
  leg->Draw();

  TPaveText *pt=new TPaveText(0.15,0.9,0.5,0.95,"brNDC");
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->AddText("CMS simulation,#sqrt{s}=8 TeV");
  pt->SetTextAlign(21);
  pt->Draw();


  c->Modified();
  c->Update();
}
