{

  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  setStyle();
  TCanvas *cnv = getNewCanvas("cnv","cnv",false);
  cnv->SetWindowSize(600,600);
  
  TFile *fin=TFile::Open("/tmp/psilva/GGtoHtoZZto2L2Nu.root");
  TString hptvars[]={"","std","renup","rendown","facup","facdown"};
  TString tit[]={"Powheg","Powheg reweighted","Renormalization up","Renormalization down", "Factorization up","Factorization down"};
  Int_t ls[]={1,1,2,2,6,6};
  Int_t lc[]={1,kBlue,kGray+3,kGray+3,kGray+1,kGray+1};
  TH1D * histos[sizeof(hptvars)/sizeof(TString)];
  for(size_t ivar=0; ivar<sizeof(hptvars)/sizeof(TString); ivar++)
    {
      TH1D *h=(TH1D *) fin->Get("localAnalysis/all/hpt"+hptvars[ivar]);
      h->SetDirectory(0);
      h->SetTitle(tit[ivar]);
      h->SetLineStyle(ls[ivar]);
      h->SetLineColor(lc[ivar]);
      h->SetLineWidth(2);
      h->SetMarkerColor(lc[ivar]);
      h->SetFillColor(lc[ivar]);
      histos[ivar]=h;
    }
  fin->Close();

  cnv->cd();
  histos[0]->Draw("hist");
  for(size_t ih=1; ih<sizeof(hptvars)/sizeof(TString); ih++)
    histos[ih]->Draw("histsame");
  TLegend *leg=cnv->BuildLegend();
  formatForCmsPublic(cnv,leg,"CMS simulation, m_{H^{0}}=200 GeV/c^{2}",5);
}
