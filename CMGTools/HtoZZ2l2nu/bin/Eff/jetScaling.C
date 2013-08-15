
{
  TString fname("plotter.root");
  TString titToMatch("Z/#gamma");
  
  int ntouse(2);
  TString evcat("mumu");
  TString names[]={"njetsvspu","njetsincvspu"};
  TString titles[]={"|#eta|<2.4","|#eta|<5"};

  int colors[]={1,kGreen-3,kRed+3,kGreen+3,kRed-3};
  int styles[]={1,2,2,3,9};
  int markers[]={20,24,20,24,21};

  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  setStyle();
  gStyle->SetOptFit(0);

  TCanvas *effC = getNewCanvas("effC","effC",false);
  effC->SetWindowSize(1000,500);
  effC->Divide(2,1);

  TFile *f=TFile::Open(fname);
  TGraphAsymmErrors *procmean[ntouse]; 
  TObjArray *revol[ntouse];
  
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
	  
          TH1 *h = (TH1 *) f->Get(hname);
	  int maxjets=h->GetYaxis()->GetNbins();
	  
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
	  revol[i] = new TObjArray;

	  for(int ijetbin=2; ijetbin<=maxjets; ijetbin++)
	    {
	      TGraphAsymmErrors *gr= new TGraphAsymmErrors;
	      TString grname(names[i]+"_r_");
	      grname += (ijetbin-1);
	      gr->SetName(grname);
	      gr->SetTitle(titles[i]);
	      gr->SetLineColor(colors[i]);
	      gr->SetLineStyle(styles[i]);
	      gr->SetFillStyle(0);
	      gr->SetMarkerColor(colors[i]);
	      gr->SetMarkerStyle(markers[i]);
	      gr->SetLineWidth(2);
	      revol[i]->Add(gr);
	    }
	  
	  for(int ibin=1; ibin<=14; ibin++) 
	    { 
	      TH1*proj=((TH2 *)h)->ProjectionY("_py",ibin,ibin); 
	      procmean[i]->SetPoint(ibin-1,ibin-1,proj->GetMean()); 
	      procmean[i]->SetPointError(ibin-1,0,0,proj->GetMeanError(),proj->GetMeanError());

	      for(int ijetbin=2; ijetbin<=maxjets; ijetbin++)
		{
		  Double_t nk= proj->GetBinContent(ijetbin-1);
		  Double_t nkerr= proj->GetBinError(ijetbin-1);
		  Double_t nkp1= proj->GetBinContent(ijetbin);
		  Double_t nkp1err= proj->GetBinError(ijetbin);
		  Double_t r=nkp1/nk;
		  Double_t rerr=sqrt(pow(nkp1*nkerr,2)+pow(nk*nkp1err,2))/(nk*nk);
		  ((TGraphAsymmErrors *)(revol[i]->At(ijetbin-2)))->SetPoint(ibin-1,ibin-1,r);
		  ((TGraphAsymmErrors *)(revol[i]->At(ijetbin-2)))->SetPointError(ibin-1,0,0,rerr,rerr);
		}

	    }
	  
	  effC->cd(1);
	  procmean[i]->Draw(i==0 ? "ap" : "p");
	  procmean[i]->GetXaxis()->SetTitle("Pileup");
	  procmean[i]->GetYaxis()->SetTitle("<Jets/event>");
	  
	  TPad *p=(TPad *)effC->cd(2);
	  if(i==0)
	    {
	      p->Divide(1,revol[i]->GetEntriesFast());
	    }
	  for(int ijetbin=2; ijetbin<=maxjets; ijetbin++)
	    {
	      TString ratioTitle("R_{"); ratioTitle += (ijetbin-1); ratioTitle += "} = N_{"; 
	      ratioTitle += ijetbin-1;     ratioTitle += "}/N_{";     ratioTitle += (ijetbin-2); ratioTitle += "}";
	      p->cd(ijetbin-1);
	      ((TGraphAsymmErrors *)revol[i]->At(ijetbin-2))->Draw(i==0 ? "ap" : "p");
	      ((TGraphAsymmErrors *)revol[i]->At(ijetbin-2))->GetXaxis()->SetTitle("Pileup");
	      ((TGraphAsymmErrors *)revol[i]->At(ijetbin-2))->GetYaxis()->SetTitle(ratioTitle);
	      ((TGraphAsymmErrors *)revol[i]->At(ijetbin-2))->GetYaxis()->SetRangeUser(0,1.0);
	    }
	}
    }

  TPad *p=(TPad *)effC->cd(1);
  TLegend *leg=p->BuildLegend();
  formatForCmsPublic(p,leg,"CMS simulation",sizeof(names)/sizeof(TString));
}
