
{

  TString mpoint("H(200)");

  TString fitfunc("pol2");
  TString fitfunc2("pol2");
  TString titToMatch("#gamma");
  TString fname("plotter.root");
  int ntouse(4);
  TString evcat("mumuzsideband");
  TString names[]={"metvspu","minMetvspu","minProjMetvspu", "redMetvspu"};
  TString titles[]={"E_{T}^{miss}","min(E_{T}^{miss},trk-E_{T}^{miss})","min-proj-E_{T}^{miss}","red-E_{T}^{miss}"};

//   TString fitfunc("pol1");
//   TString fitfunc2("pol2");
//   TString titToMatch("H(200)");
//   TString fname("higgs200_plotter.root");
//   int ntouse(5);
//   TString evcat("matchedmuon");
//   TString names[]={"rhovsnpu","ecalisovsnpu","hcalisovsnpu","trackisovsnpu","absisovsnpu"};
//   TString titles[]={"#rho","I_{photon}","I_{neut. hadron}","I_{ch. hadron}", "I_{total}"};

  int colors[]={1,kGreen-3,kRed+3,kGreen+3,kRed-3};
  int styles[]={1,2,2,3,9};
  int markers[]={20,20,20,24,21};

  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  setStyle();
  gStyle->SetOptFit(0);

  TCanvas *effC = getNewCanvas("effC","effC",false);
  effC->SetWindowSize(1000,500);
  effC->Divide(2,1);

  TFile *f=TFile::Open(fname);
  TGraphAsymmErrors *procmean[ntouse], *procrms[ntouse];
  for(int i=0; i<ntouse; i++)
    {
      TString name=names[i];
      TString title=titles[i];
      int color=colors[i];
      int style=styles[i];
      TString idxStr("");
      idxStr += i;

      TH1 *proc=0;
      TIter next(f->GetListOfKeys());
      TObject *obj=0;
      int idir(0);
      while(1)
	{
          idir++;
          obj=next();
          if(obj==0) break;
          TString dirname=obj->GetName();
          TString hname=dirname +"/"+evcat+"_" + name;
	  cout << hname << endl;
          TH1 *h = f->Get(hname);
          TString tit=h->GetTitle();
          if(!tit.Contains(titToMatch)) continue;
	  
	  procmean[i] = new TGraphAsymmErrors;
	  procmean[i]->SetName(names[i]+"_mean");
	  procmean[i]->SetTitle(titles[i]);
	  procmean[i]->SetLineColor(colors[i]);
	  procmean[i]->SetLineStyle(styles[i]);
	  procmean[i]->SetFillStyle(0);
	  procmean[i]->SetMarkerColor(colors[i]);
	  procmean[i]->SetMarkerStyle(markers[i]);
	  procmean[i]->SetLineWidth(2);
	  
	  procrms[i] = new TGraphAsymmErrors;
	  procrms[i]->SetName(names[i]+"_rms");
	  procrms[i]->SetTitle(titles[i]);
	  procrms[i]->SetLineColor(colors[i]);
	  procrms[i]->SetLineStyle(styles[i]);
	  procrms[i]->SetFillStyle(0);
	  procrms[i]->SetMarkerColor(colors[i]);
	  procrms[i]->SetMarkerStyle(1);
	  procrms[i]->SetMarkerStyle(markers[i]);
	  procrms[i]->SetLineWidth(2);

	  for(int ibin=1; ibin<=14; ibin++) { 
	    TH1*proj=((TH2 *)h)->ProjectionY("_py",ibin,ibin); 
	    procmean[i]->SetPoint(ibin-1,ibin-1,proj->GetMean()); 
	    procmean[i]->SetPointError(ibin-1,0,0,proj->GetMeanError(),proj->GetMeanError());
	    procrms[i]->SetPoint(ibin-1,ibin-1,proj->GetRMS()); 
	    procrms[i]->SetPointError(ibin-1,0,0,proj->GetRMSError(),proj->GetRMSError()); 
	  }
	  
	  effC->cd(1);
	  procmean[i]->Draw(i==0 ? "ap" : "p");
	  procmean[i]->Fit(fitfunc,"Q");
	  procmean[i]->GetXaxis()->SetTitle("Pileup");
	  procmean[i]->GetYaxis()->SetTitle("Average");
	  
	  effC->cd(2);
	  procrms[i]->Draw(i==0 ? "ap" : "p");
	  procrms[i]->Fit(fitfunc2,"Q");
	  procrms[i]->GetXaxis()->SetTitle("Pileup");
	  procrms[i]->GetYaxis()->SetTitle("RMS");
	}
    }

  TPad *p=(TPad *)effC->cd(1);
  TLegend *leg=p->BuildLegend();
  formatForCmsPublic(p,leg,"CMS simulation",sizeof(names)/sizeof(TString));
}
