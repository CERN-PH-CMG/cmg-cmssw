{
  TString mpointName("H(200)");
  
  TString cat("mumu");
  TString subCats[]={"eq0jets","eq1jets","geq2jets"};
  TString subCatTitles[]={"=0 jets","=1 jets", "#geq 2 jets"};
  TString cutTypes[]={"cutOptMedium_summt_vs_redMetL",
		      "cutOptMedium_dphill"};
 //TString cutType("cutOptTight_summt_vs_redMetL");
  //TString cutType("cutOptTight_dphill");
  
  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  setStyle();
  
  //get histos from file
  TFile *f=TFile::Open("plotter.root");    
  
  //build signal and background distributions 
  int nSubCats=sizeof(subCats)/sizeof(TString);
  int nCuts=sizeof(cutTypes)/sizeof(TString);
  TH1 *signal[nCuts][nSubCats], *bckg[nCuts][nSubCats];
  for(int j=0; j<nCuts; j++)
    for(int i=0; i<nSubCats; i++) 
      { 
	signal[j][i]=0; 
	bckg[j][i]=0; 
      }

  TIter next(f->GetListOfKeys());
  TObject *obj=0;
  int idir(0);
  while(1)
    {
      idir++;
      obj=next();
      if(obj==0) break;
      TString dirname=obj->GetName();
      for(int j=0; j<nCuts; j++)
	{
	  for(int i=0; i<nSubCats; i++)
	    {

	      TString hname=dirname +"/" + cat + subCats[i] + "_"+cutTypes[j];
	      TH1 *h = f->Get(hname);

	      TString tit=h->GetTitle();
	      if(tit.Contains("H(")) 
		{
		  if(!tit.Contains(mpointName)) continue;
		  if(signal[j][i]==0) 
		    { 
		      signal[j][i] = (TH1 *) h->Clone("signal"+cutTypes[j]+subCats[i]);
		      signal[j][i]->SetDirectory(0);
		      if(j==0) signal[j][i]->GetYaxis()->SetTitle("red-E_{T}^{miss} #parallel");
		    }
		  else
		    signal[j][i]->Add(h);
		}
	      else if( !tit.Contains("data") )
		{
		  if(bckg[j][i]==0) 
		    {
		      bckg[j][i] = (TH2D *) h->Clone("bckg"+cutTypes[j]+subCats[i]);
		      bckg[j][i]->SetDirectory(0);
		      if(j==0) bckg[j][i]->GetYaxis()->SetTitle("red-E_{T}^{miss} #parallel");
		    }
		}
	      else 
		bckg[j][i]->Add(h);
	    }
	}
    }

  //FoM canvas
  TCanvas *fomC = getNewCanvas("fom","fom",false);
  fomC->SetWindowSize(1500,500);
  fomC->Divide(3,1);
  for(int j=0; j<nCuts; j++)
    {
      bool is2D(false);
      if(signal[j][0]->IsA()->InheritsFrom("TH2")) is2D=true;
      
      //show the results
      TCanvas *varC = getNewCanvas(cutTypes[j],cutTypes[j],false);
      varC->SetWindowSize(500*nSubCats,500);
      varC->Divide(nSubCats,1);
      for(int i=0; i<nSubCats; i++)
	{
	  TPad *p=(TPad *)varC->cd(i+1);
	  p->Divide(1,2);
	  TPad *sp=(TPad *)p->cd(1);
	  signal[j][i]->Draw((is2D ? "contz" : "hist") );
	  TLegend *leg = sp->BuildLegend();  
	  TString legTit="CMS simulation\\"+subCatTitles[i]+"\\"+mpointName;
	  legTit.ReplaceAll("H(","m_{H^{0}}=");
	  legTit.ReplaceAll(")"," GeV/c^{2}");
	  formatForCmsPublic(sp,leg,legTit,1);      
	  
	  sp=(TPad *)p->cd(2);
	  bckg[j][i]->Draw((is2D ? "contz" : "hist") );
	  bckg[j][i]->SetTitle("background");
	  leg=sp->BuildLegend();
	  formatForCmsPublic(sp,leg,"",1);   
	}

      //compute the FoM
      for(int i=0; i<nSubCats; i++)
	{
	  TString idxStr(""); idxStr +=(j+1); idxStr+=(i+1);       
	  
	  //define the denomimator
	  TH1 *fomDenH = (TH1 *) signal[j][i]->Clone("fomden"+idxStr);
	  fomDenH->SetDirectory(0);
	  fomDenH->Add(bckg[j][i]);
	  
	  for(int xbin=1; xbin<= fomDenH->GetXaxis()->GetNbins(); xbin++)
	    {
	      if(is2D)
		for(int ybin=1; ybin<= fomDenH->GetYaxis()->GetNbins(); ybin++)
		  {
		    Double_t val=fomDenH->GetBinContent(xbin,ybin); 
		    Double_t valerr=fomDenH->GetBinError(xbin,ybin); 
		    fomDenH->SetBinContent(xbin,ybin,sqrt(val) );
		    fomDenH->SetBinError(xbin,ybin, (val>0 ? valerr/(2*sqrt(val)):0.) );
		  }
	      else
		{
		  Double_t val=fomDenH->GetBinContent(xbin); 
		  Double_t valerr=fomDenH->GetBinError(xbin); 
		  fomDenH->SetBinContent(xbin,sqrt(val) );
		  fomDenH->SetBinError(xbin, (val>0 ? valerr/(2*sqrt(val)):0.) );
		}
	    }
      
	  //define the fom
	  TH1 *fomH = (TH1 *) signal[j][i]->Clone("fom"+idxStr);
	  fomH->SetDirectory(0);
	  fomH->Divide(fomDenH);

	  p=(TPad *)fomC->cd(j*2+1);
	  if(is2D)
	    {
	      TH1 *proj= ((TH2 *)fomH)->ProjectionX("px_"+idxStr);
	      proj->SetLineColor(i+1);
	      proj->SetFillStyle(3001+i);
	      proj->SetFillColor(i+1);
	      proj->SetLineWidth(2);
	      proj->SetTitle(subCatTitles[i]);
	      proj->GetYaxis()->SetTitle("S/#sqrt{S+B}");
	      proj->Draw( (i==0 ? "hist" : "histsame") );
	    }
	  else
	    {
	      fomH->SetLineColor(i+1);
	      fomH->SetFillStyle(3001+i);
	      fomH->SetFillColor(i+1);
	      fomH->SetLineWidth(2);
	      fomH->SetTitle(subCatTitles[i]);
	      fomH->GetYaxis()->SetTitle("S/#sqrt{S+B}");
	      fomH->Draw((i==0 ? "hist" : "histsame") );
	      continue;
	    }
	  
	  sp=(TPad *)fomC->cd(j*2+2);
	  proj= ((TH2 *)fomH)->ProjectionY("py_"+idxStr);
	  proj->SetLineColor(i+1);
	  proj->SetFillStyle(3001+i);
	  proj->SetFillColor(i+1);
	  proj->SetLineWidth(2);
	  proj->SetTitle(subCatTitles[i]);
	  proj->GetYaxis()->SetTitle("S/#sqrt{S+B}");
	  proj->GetXaxis()->SetTitle("red-E_{T}^{miss} #parallel");
	  proj->Draw( (i==0 ? "hist" : "histsame") );
	}
    }
  
  p=(TPad *)fomC->cd(1);
  TLegend *leg = p->BuildLegend();
  TString legTit="CMS simulation\\"+mpointName;
  legTit.ReplaceAll("H(","m_{H^{0}}=");
  legTit.ReplaceAll(")"," GeV/c^{2}");
  formatForCmsPublic(p,leg,legTit,3);
}
