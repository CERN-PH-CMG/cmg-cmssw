{

  TString gammaFile="~/work/htozz/53x_rereco/gamma/2012/qt/plotter.root";
  TFile *_file0=TFile::Open(gammaFile);
  
  gStyle->SetOptStat(0);
  
  TString histo("metvsavginstlumi");
  TString histoY("<E_{T}^{miss}>");
  TString histoX("Avg. inst luminosity");
    
//   TString histo("nvtxvsavginstlumi");
//   TString histoY("<N_{vtx}>");
//   TString histoX("Avg. inst luminosity");
 
//   TString histo("metvsrun");
//   TString histoY("<E_{T}^{miss}>");
//   TString histoX("Run number");
  
// TString histo("nvtxvsrun");
  // TString histoY("<N_{vtx}>");
  //  TString histoX("Run number");
  
  
  TString cats[]={"","eq0jets","eq1jets","eq2jets"};
  TString titles[]={"=0 jetsinclusive","=0 jets","=1 jets","#geq 2jets"};
  const size_t ncats=sizeof(cats)/sizeof(TString);
  TCanvas *c = new TCanvas("c","c",600,600);
  c->Divide(2,2);

  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  c2->Divide(2,2);
  
  for(int i=0; i<ncats; i++)
    {
      TString cat=cats[i];
      c->cd(i+1);
      TH1 *mumu=(TH1 *)_file0->Get("data (ll)/mumu"+cat+"_"+histo);      mumu->SetDirectory(0);
      TH1 *ee=(TH1 *)_file0->Get("data (ll)/ee"+cat+"_"+histo);          ee->SetDirectory(0);
      TH1 *gamma=(TH1 *)_file0->Get("data (#gamma)/mumu"+cat+"_"+histo); gamma->SetDirectory(0);
      TH1 *gammaee=(TH1 *)_file0->Get("data (#gamma)/ee"+cat+"_"+histo); gammaee->SetDirectory(0);

      //      gamma->SetMarkerStyle(1);  gamma->SetLineColor(kGray);       gamma->SetFillStyle(3315); gamma->SetFillColor(kGray);
      gamma->SetMarkerStyle(20);  gamma->SetLineColor(kRed);  gamma->SetMarkerColor(kRed); 
      mumu->SetTitle(titles[i]); mumu->GetYaxis()->SetTitle(histoY);mumu->GetXaxis()->SetTitle(histoX);

      gammaee->SetMarkerStyle(1);   gammaee->SetLineColor(kGreen-3); gammaee->SetFillStyle(3351); gammaee->SetFillColor(kGreen-3);
      ee->SetMarkerColor(kGreen+3); ee->SetMarkerColor(kGreen+3);  ee->SetMarkerColor(20);

      ee->SetTitle("");
      gamma->SetTitle("");

      TGraphErrors *grgamma=new TGraphErrors(gamma);
      grgamma->Draw("ap");
      grgamma->GetYaxis()->SetTitle(histoY);
      //grgamma->GetXaxis()->SetTitle("Energy density (#rho)");
      grgamma->GetXaxis()->SetTitle(histoX);
      TGraphErrors *grgammaee=new TGraphErrors(gammaee);
      //grgammaee->Draw("e1same");
      mumu->Draw("same");
      ee->Draw("same");

      TPaveText *pt = new TPaveText(0.7,0.8,0.9,0.9,"NDC");
      pt->SetBorderSize(0);
      pt->SetFillStyle(0);
      if(i==0) pt->AddText("=0 jets");
      if(i==1) pt->AddText("=1 jets");
      if(i==2) pt->AddText("=2 jets");
      if(i==3) pt->AddText("#geq3 jets");
      pt->SetTextFont(42);
      pt->Draw();


      TH1 *gammaDiffmm=(TH1F *) mumu->Clone(); gammaDiffmm->SetDirectory(0);
      TH1 *gammaDiffee=(TH1F *) ee->Clone();   gammaDiffee->SetDirectory(0);
      gammaDiffmm->Add(gamma,-1);
      gammaDiffee->Add(gammaee,-1);

      c2->cd(i+1);
      gammaDiffmm->Draw("e1");
      gammaDiffee->Draw("e1 same");
      pt->Clone()->Draw();
      gammaDiffmm->GetYaxis()->SetRangeUser(-10,10);
      gammaDiffmm->GetXaxis()->SetRangeUser(0,1500);
      gammaDiffmm->GetYaxis()->SetTitle("Rel. difference %");

      if(i) continue;
      c->cd(i+1);
      TLegend *leg= new TLegend(0.6,0.6,0.9,0.9);
      //      leg->SetNColumns(2);
      leg->AddEntry(mumu,"#mu#mu","lp");
      leg->AddEntry(ee,"ee","lp");     
      leg->AddEntry(grgamma,"#gamma+jets","lp");
      //leg->AddEntry(grgamma,"#gamma+jets (#mu#mu)","f");
      //leg->AddEntry(grgammaee,"#gamma+jets (ee)","f");
      leg->Draw();
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);
      leg->SetTextFont(42);
      c2->cd(i+1);

    }



}
