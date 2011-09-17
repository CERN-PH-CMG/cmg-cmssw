
{
  int ntouse(5);
  double wp(0.001);

  TString evcat("mumugeq10vtx");
  TString mpoint("H(400)");
  
  int ntouse(5);
  // TString names[]={"met",           "minMet",                             "minClusteredMet",                          "unclusteredMet",                                   "superMinMet"};
  //TString titles[]={"E_{T}^{miss}", "min(E_{T}^{miss},trk-E_{T}^{miss})", "min(E_{T}^{miss},clustered-E_{T}^{miss})", "min(clean E_{T}^{miss},unclustered-E_{T}^{miss})", "min(clustered E_{T}^{miss},unclustered E_{T}^{miss})"};

  //  TString names[]={"met","minMet","minProjMet", "redMet"};
  // TString titles[]={"E_{T}^{miss}","min(E_{T}^{miss},trk-E_{T}^{miss})","min-proj-E_{T}^{miss}","red-E_{T}^{miss}"};
  
//   TString names[]={"met","centralMet", "minMet","cleanMet", "unclusteredMet"};
//  TString titles[]={"E_{T}^{miss}", "central E_{T}^{miss}", "min(E_{T}^{miss},trk-E_{T}^{miss})","clean E_{T}^{miss}","unclustered E_{T}^{miss}"};
  
  //  TString names[]={"met","cleanMet","centralMet","unclusteredMet", "clusteredMet", "redMet","cmsRedMet"};
  // TString titles[]={"E_{T}^{miss}","clean E_{T}^{miss}","central E_{T}^{miss}","unclustered E_{T}^{miss}","clustered E_{T}^{miss}","red-E_{T}^{miss}","cms-red-E_{T}^{miss}"};

  int colors[]={1,kGreen-3,kGreen+3,kBlue,kBlue,kRed+3,kRed};
  int styles[]={1,2,9,9,1,1,1};

  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  setStyle();

  TCanvas *effC = getNewCanvas("effC","effC",false);
  effC->SetWindowSize(500,500);
  TGraph *frame=new TGraph;
  frame->SetPoint(0,1e-6,1e-6);
  frame->SetPoint(1,1e-6,1.01);
  frame->SetPoint(2,1.01,1.01);
  frame->SetPoint(3,1.01,1e-6);
  frame->SetMarkerStyle(1);
  effC->cd();
  frame->Draw("ap");
  frame->GetXaxis()->SetTitle("#varepsilon(signal)");
  frame->GetYaxis()->SetTitle("#varepsilon(instr. background)");

  TCanvas *fitC = getNewCanvas("fitC","fitC",false);
  fitC->SetWindowSize(600,ntouse*200);
  fitC->Divide(3,ntouse);
  TF1 *ffunc=new TF1("fitfunc", "[0]*pow(log(x),2)+[2]*log(x)+[1]",0,10);


  TFile *f=TFile::Open("plotter.root");
    
  for(size_t i=0; i<ntouse; i++)
    {
      TString name=names[i];
      TString title=titles[i];
      int color=colors[i];
      int style=styles[i];
      TString idxStr("");
      idxStr += i;      
      
      TH1 *signal=0, *dy=0;
      TIter next(f->GetListOfKeys());
      TObject *obj=0;
      int idir(0);
      while(1)
	{
	  idir++;
	  obj=next();
	  if(obj==0) break;
	  TString dirname=obj->GetName();
	  TString hname=dirname +"/"+evcat+"_" + name;// +"_";  hname += idir;
	  TH1 *h = f->Get(hname);
	  TString tit=h->GetTitle();
	  if(tit.Contains("#gamma^{*}"))
	    {
	      if(dy==0) 
		{ 
		  dy = (TH1 *) h->Clone("bckg_"+idxStr);
		  dy->SetDirectory(0);
		}
	      else
		dy->Add(h);
	      cout << " -bckg: " << tit << endl;  
	    }
	  else if(tit.Contains(mpoint))
	    {
	      if(signal==0) 
		{
		  signal = (TH1 *) h->Clone("signal_"+idxStr);
		  signal->SetDirectory(0);
		}
	      else 
		signal->Add(h);
	      cout << " -signal: " << tit << endl;  
	    }
	}
       
      //prepare efficiency graphs
      TGraphAsymmErrors *effGr =new TGraphAsymmErrors;
      effGr->SetName(name);
      effGr->SetTitle(title);
      effGr->SetLineColor(color);
      effGr->SetLineStyle(style);
      effGr->SetFillStyle(0);
      effGr->SetMarkerColor(color);
      effGr->SetMarkerStyle(1);
      effGr->SetLineWidth(2);

      //integrate
      bool reverseIntegration=false;
      double dyTotal=dy->Integral("width");
      double signalTotal=signal->Integral("width");
      const int nbins=dy->GetXaxis()->GetNbins();
      TGraphAsymmErrors *cutNeighboorhoodZoomGr = new TGraphAsymmErrors;
      TGraphAsymmErrors *wpEffGr = new TGraphAsymmErrors;
      int imp(0);
      for(int xbin=1; xbin<=nbins; xbin++)
	{
	  float signalCut(0),signalError(0), dyCut(0), dyError(0);
	  if(reverseIntegration)
	    {
	      signalCut=signal->IntegralAndError(1,xbin,signalError,"width");
	      dyCut=dy->IntegralAndError(1,xbin,dyError,"width");
	    }
	  else
	    {
	      signalCut=signal->IntegralAndError(xbin,nbins,signalError,"width");
	      dyCut=dy->IntegralAndError(xbin,nbins,dyError,"width"); 
	    }

	  //efficiency curve
	  double effb=float(dyCut/dyTotal);
	  double effbError=float(dyError/dyTotal);
	  double effsig=float(signalCut/signalTotal);
	  double effsigError=float(signalError/signalTotal);
	  
	  effGr->SetPoint(xbin-1,effsig,effb);
	  effGr->SetPointError(xbin-1,effsigError,effsigError,effbError,effbError);

	  //store neighborhood of the working point
	  double maxeffb(evcat.Contains("vbf") ? wp*100 : wp*5);
	  double mineffb(evcat.Contains("vbf") ? wp*0.01 : wp*0.5);
	  if(effb<maxeffb && effb>mineffb)
	    {
	      cutNeighboorhoodZoomGr->SetPoint(imp, effb, dy->GetXaxis()->GetBinCenter(xbin) );
	      cutNeighboorhoodZoomGr->SetPointError(imp, effbError,effbError,0,0);

	      wpEffGr->SetPoint(imp, effb, effsig);
	      wpEffGr->SetPointError(imp, effbError,effbError, effsigError,effsigError);

	      imp++;
	    }
	}
      
      //fit the working point
      fitC->cd(2+3*i);  
      Double_t xmin,xmax,ytemp;
      cutNeighboorhoodZoomGr->GetPoint(0,xmin,ytemp);
      cutNeighboorhoodZoomGr->GetPoint(imp-1,xmax,ytemp);
      ffunc->SetRange(xmin,xmax);
      cutNeighboorhoodZoomGr->Draw("ap");
      cutNeighboorhoodZoomGr->Fit(ffunc,"Q");
      double theCut = ffunc->Eval(wp);
      fitC->cd(3+3*i);
      wpEffGr->GetPoint(0,xmin,ytemp);
      wpEffGr->GetPoint(imp-1,xmax,ytemp);
      ffunc->SetRange(xmin,xmax);
      wpEffGr->Draw("ap");
      wpEffGr->Fit(ffunc,"Q");
      double effs = ffunc->Eval(wp);

      fitC->cd(1+3*i);
      dy->DrawNormalized("hist");
      signal->DrawNormalized("histsame");

      effC->cd();
      effGr->Draw("pl");

      printf("[Working point] \n%3.4lf => cut at %s>%3.4lf effSignal=%3.4lf\n",wp,title.Data(),theCut,effs);
    }
  f->Close();

  TPad *p=(TPad *)effC->cd();
  TLegend *leg=p->BuildLegend();
  TString legTit="CMS simulation\\"+mpoint;
  legTit.ReplaceAll("H(","m_{H^{0}}=");
  legTit.ReplaceAll(")"," GeV/c^{2}");
  formatForCmsPublic(p,leg,legTit,sizeof(names)/sizeof(TString));
  p->SetLogy();
}
