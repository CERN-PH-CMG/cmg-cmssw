{
  TString mpointName("H(300)");
  
  TString cat("mumu");
  TString cutType("cutOptTight_summt_vs_redMetL");

  // TString cat("mumu");
  // TString cutType("VBFdEtaiMassNM1C");

  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  setStyle();

  //get histos from file
  TFile *f=TFile::Open("plotter.root");    

  //build signal and background distributions 
  TH2D *signal=0, *bckg=0;
  TIter next(f->GetListOfKeys());
  TObject *obj=0;
  int idir(0);
  while(1)
    {
      idir++;
      obj=next();
      if(obj==0) break;
      TString dirname=obj->GetName();
      TString hname=dirname +"/" + cat + "_"+cutType;
      TH2 *h = f->Get(hname);

      TString tit=h->GetTitle();
      if(tit.Contains("H(")) 
	{
	  if(!tit.Contains(mpointName)) continue;
	  cout << " - signal: " << tit << endl;
	  if(signal==0) 
	    { 
	      signal = (TH2D *) h->Clone("signal");
	      signal->SetDirectory(0);
	      cout << signal->Integral() << endl;
	    }
	  else
	    signal->Add(h);
	}
      else if( !tit.Contains("data") )
	{
	  cout << " - bckg: " << tit << endl;
	  if(bckg==0) 
	    {
	      bckg = (TH2D *) h->Clone("bckg");
	      bckg->SetDirectory(0);
	    }
	  else 
	    bckg->Add(h);
	}
    }
  
  //show the results
  TCanvas *varC = getNewCanvas(cutType,cutType,false);
  varC->SetWindowSize(1000,500);
  varC->Divide(4,1);
  TPad *p=(TPad *)varC->cd(1);
  p->Divide(1,2);
  TPad *sp=(TPad *)p->cd(1);
  signal->Draw("contz");
  sp->BuildLegend()->SetBorderSize(0);
      
  sp=(TPad *)p->cd(2);
  bckg->Draw("contz");
  bckg->SetTitle("background");
  sp->BuildLegend()->SetBorderSize(0);
  
  //different figure of merit
  for(Int_t imode=1; imode<=3; imode++)
    {

      p=(TPad *)varC->cd(1+imode);
      p->Divide(1,2);
      sp=(TPad *)p->cd(1);

      TString idxStr(""); idxStr+=imode;

      //define the denomimator
      TH2D *fomDenH = (TH2D *) signal->Clone("fomden"+idxStr);
      if(imode==2 || imode==3) fomDenH->Reset("ICE");
      fomDenH->SetDirectory(0);
      fomDenH->Add(bckg);

      if(imode==1 || imode==2)
	{
	  for(int xbin=1; xbin<= fomDenH->GetXaxis()->GetNbins(); xbin++)
	    for(int ybin=1; ybin<= fomDenH->GetYaxis()->GetNbins(); ybin++)
	      {
		Double_t val=fomDenH->GetBinContent(xbin,ybin); 
		Double_t valerr=fomDenH->GetBinError(xbin,ybin); 
		fomDenH->SetBinContent(xbin,ybin,sqrt(val) );
		fomDenH->SetBinError(xbin,ybin, (val>0 ? valerr/(2*sqrt(val)):0.) );
	      }
	}
  
      TH2D *fomH = (TH2D *) signal->Clone("fom"+idxStr);
      fomH->SetDirectory(0);
      fomH->Divide(fomDenH);
      fomH->Draw("contz");
      if(imode==1)  fomH->SetTitle("S/#sqrt{S+B}");
      if(imode==2)  fomH->SetTitle("S/#sqrt{B}");
      if(imode==3)  fomH->SetTitle("S/B");
      sp->BuildLegend()->SetBorderSize(0);
  
      p=(TPad *)p->cd(2);
      p->Divide(2,1);
      sp =(TPad *)p->cd(1);
      TH1 *proj= fomH->ProjectionX("px_"+idxStr);
      proj->SetLineColor(1);
      proj->SetFillStyle(0);
      proj->Draw("hist");
      
      sp=(TPad *)p->cd(2);
      proj= fomH->ProjectionY("py_"+idxStr);
      proj->SetLineColor(1);
      proj->SetFillStyle(0);
      proj->Draw("hist");
    }
}
