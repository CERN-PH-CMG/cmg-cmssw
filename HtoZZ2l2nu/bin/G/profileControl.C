{

  TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2011/nvtx/plotter.root";
  TFile *_file0=TFile::Open(gammaFile);
  
  gStyle->SetOptStat(0);
  
  //   TString histo("metvsavginstlumi");
  //   TString histoY("<E_{T}^{miss}>");
  //   TString histoX("<Inst. Lumi>");
  
  TString histo("met20vsrho");
  TString histoY("<red-E_{T}^{miss}>");
  TString histoX("Energy density (#rho)");
  
  //   TString histo("nvtxvsavginstlumi");
  //  TString histoY("<N_{vtx}>");
  //  TString histoX("<Inst. Lumi>");
 
  // TString histo("metvsrun");
  //  TString histoY("<E_{T}^{miss}>");
  //  TString histoX("Run number");
  
  //    TString histo("nvtxvsrun");
  //     TString histoY("<N_{vtx}>");
  //     TString histoX("Run number");
  
  
  TString cats[]={"eq0jets"};//{"","eq0jets","eq1jets","eq2jets"};
  TString titles[]={""}; //=0 jetsinclusive","=0 jets","=1 jets","#geq 2jets"};
  const size_t ncats=sizeof(cats)/sizeof(TString);
  TCanvas *c = new TCanvas("c","c",600,600);
  //  c->Divide(2,2);
  
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
      grgamma->GetYaxis()->SetTitle("<red-E_{T}^{miss}>");
      grgamma->GetXaxis()->SetTitle("Energy density (#rho)");
      TGraphErrors *grgammaee=new TGraphErrors(gammaee);
      //grgammaee->Draw("e1same");
      mumu->Draw("same");
      ee->Draw("same");

      if(i) continue;
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

    }



}
