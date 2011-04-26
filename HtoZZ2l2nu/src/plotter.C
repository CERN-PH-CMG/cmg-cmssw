#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

using namespace std;

//
TLegend *showPlots(TPad *c, TList &stack, TList &spimpose, TList &data, bool buildLegend, TString legopt)
{
  if(c==0) return 0;
  if( stack.First()==0 && spimpose.First()==0 && data.First()==0 ) return 0;

  //prepare the pad
  c->cd();
  //c->SetGridx();
  //c->SetGridy();
  c->Clear();
  TString name=c->GetName();
  TString title=c->GetTitle();
  
  bool th2dfound(false), graphfound(false);

  //start with the legend with the data
  std::vector<std::pair<TObject *,TString> > allKeys;
  TObject *key = 0; 
  TIterator *dataIt = data.MakeIterator();
  while ( (key = dataIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      th2dfound |= ((TClass*)key->IsA())->InheritsFrom("TH2");
      graphfound |= ((TClass*)key->IsA())->InheritsFrom("TGraph");
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
    }
  
  //add the plots to superimpose
  TIterator *spimposeIt = spimpose.MakeIterator();
  while ( (key = spimposeIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      th2dfound |= ((TClass*)key->IsA())->InheritsFrom("TH2");
      graphfound |= ((TClass*)key->IsA())->InheritsFrom("TGraph");
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
    }

  //loop over the stack in reversed sense
  THStack *hstack = new THStack(name+"_stack",title);  
  TIterator *reverseStackIt = stack.MakeIterator(kIterBackward);
  while ( (key = reverseStackIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      th2dfound |= ((TClass*)key->IsA())->InheritsFrom("TH2");
      graphfound |= ((TClass*)key->IsA())->InheritsFrom("TGraph");
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
    }

  
  //build the legend
  TLegend *leg = new TLegend(0.7,0.96-0.05*allKeys.size(),0.93,0.96,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetFillColor(0);
  leg->SetTextFont(42);
  for(std::vector<std::pair<TObject *,TString> >::iterator kIt = allKeys.begin();
      kIt != allKeys.end();
      kIt++)
    leg->AddEntry( kIt->first, kIt->second, legopt );      

  TH1 *refFrame=0;  
  if(th2dfound || graphfound)
    {
      int nplots=allKeys.size();
      int nx = nplots/2;
      int ny = ( nplots%2 ? nx : nx+1);
      c->Divide(nx,ny);
      int ipad(0);
      reverseStackIt = stack.MakeIterator(kIterBackward);
      while ( (key = reverseStackIt->Next()) ) 
	{
	  ipad++;
	  c->cd(ipad);
	  key->Draw( th2dfound ? "box" : "ap");
	}
	  
      spimposeIt = spimpose.MakeIterator();
      while ( (key = spimposeIt->Next()) ) 
	{
	  ipad++;
	  c->cd(ipad);
	  key->Draw( th2dfound ? "box" : "ap");
	}

      //draw the data
      dataIt = data.MakeIterator();
      while ( (key = dataIt->Next()) ) 
	{
	  ipad++;
	  c->cd(ipad);
	  key->Draw( th2dfound ? "box" : "ap");
	}
      
      //draw the legend
      c->cd(1);
      leg->Draw("same");
    }
  else
    {
      //prepare the sum and build the stack
      bool canvasFilled(false);
      TH1 *p=(TH1 *) stack.First();
      if(p)
	{
	  refFrame=(TH1 *) p->Clone(name+"_frame");
	  formatPlot(refFrame,1,1,1,0,0,true,true,1,1,1);
	  refFrame->Reset("ICE");  
	  refFrame->SetTitle(title+"_stackcontour");
	  TIterator *stackIt = stack.MakeIterator();
	  while ( (key = stackIt->Next()) ) 
	    {
	      TH1 *p = (TH1 *) key;
	      refFrame->Add(p);
	      hstack->Add( p );
	    }
	  
	  //draw the frame and the stack
	  if(refFrame->Integral()>0)
	    {
	      refFrame->Draw("hist");  
	      hstack->Draw("histsame"); 
	      canvasFilled=true;
	    }
	}
      
      //draw the super impose distributions
      spimposeIt = spimpose.MakeIterator();
      while ( (key = spimposeIt->Next()) ) 
	{
	  TH1 *p = (TH1 *) key;
	  if(canvasFilled) p->Draw("histsame");
	  else {
	    p->Draw("hist"); 
	    refFrame=p;
	    canvasFilled=true; 
	  }
	}
      
      //draw the data
      dataIt = data.MakeIterator();
      while ( (key = dataIt->Next()) ) 
	{
	  TH1 *p = (TH1 *) key;
	  p->SetMarkerSize(1.3);
	  if(canvasFilled) p->Draw("e2psame");
	  else {
	    p->Draw("e2p"); 
	    refFrame=p;
	    canvasFilled=true; 
	  }
	}

      //draw the legend
      if(canvasFilled)  leg->Draw("same");
    }
  
  if(refFrame) c->SetSelected(refFrame);
  c->Modified();
  c->Update();

  return leg;
}

//
TLegend *showStackPlot(TPad *c, TList &stack, bool buildLegend, TString legopt)
{
  TList dummyData;
  TList dummySPImpose;
  return showPlots(c,stack,dummySPImpose,dummyData,buildLegend,legopt);
}

//
TLegend *showSimplePlot(TPad *c, TList &data, bool buildLegend, TString legopt)
{
  TList dummyStack;
  TList dummySPImpose;
  return showPlots(c,dummyStack,dummySPImpose,data,buildLegend,legopt);
}


//
void showMCtoDataComparison(TPad *c, TList &stack, TList &data, bool doChi2,float yscale)
{
  if(c==0) return;
  if( stack.First()==0 || data.First()==0 ) return;

  //prepare the pad
  c->cd();
  //c->SetGridx();
  //c->SetGridy();
  c->Clear();
  TString name=c->GetName();
  TString title=c->GetTitle();
  
  //build the stack sum
  TH1 *sumH=0;
  TObject *key = 0; 
  TIterator *stackIt = stack.MakeIterator();
  while ( (key = stackIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      if( ((TClass*)key->IsA())->InheritsFrom("TH2") ) continue;
      if( ((TClass*)key->IsA())->InheritsFrom("TGraph") ) continue;
      if(sumH==0) { 
	sumH = (TH1 *) p->Clone(TString(p->GetName())+"sum"); 
	sumH->Reset("ICE");
      }
      sumH->Add(p);
    }

  //compare different data
  bool canvasFilled(false);
  Int_t idata(1);
  TIterator *dataIt = data.MakeIterator();
  while ( (key = dataIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      if( ((TClass*)key->IsA())->InheritsFrom("TH2") ) continue;
      if( ((TClass*)key->IsA())->InheritsFrom("TGraph") ) continue;

      TH1 *dataToMCH = (TH1 *) p->Clone(TString(p->GetName())+"_tomc");
      dataToMCH->Reset("ICE");
      if(doChi2)
	{
	  dataToMCH->GetYaxis()->SetTitle("#chi^{2}=((MC-Data)/(#sigma_{MC}^{2}+#sigma_{Data}^2))^2");
	  for(Int_t ibin=1; ibin<=dataToMCH->GetXaxis()->GetNbins(); ibin++)
	    {
	      Float_t diff = sumH->GetBinContent(ibin)-p->GetBinContent(ibin);
	      Float_t err = pow(sumH->GetBinError(ibin),2)+pow(dataToMCH->GetBinError(ibin),2);
	      if(err==0) continue;
	      Float_t chi2 = (diff<0 ? -1 : 1) * pow(diff,2)/err;
	      dataToMCH->SetBinContent(ibin,chi2);
	      dataToMCH->SetBinError(ibin,0,0);
	    }
	}
      else
	{
	  dataToMCH->Add(p);
	  dataToMCH->GetYaxis()->SetTitle("Data / MC");
	  dataToMCH->Divide(sumH);
	}

      c->cd();
      TString opt("e2p");
      if(canvasFilled) opt +="same";
      dataToMCH->Draw(opt);
      dataToMCH->GetXaxis()->SetTitleOffset(0.85);
      dataToMCH->GetXaxis()->SetLabelSize(0.04 * yscale);
      dataToMCH->GetXaxis()->SetTitleSize(0.05 * yscale);
      dataToMCH->GetXaxis()->SetTickLength( 0.03 * yscale );
      dataToMCH->GetYaxis()->SetTitleOffset(0.5);
      dataToMCH->GetYaxis()->SetLabelSize(0.04 * yscale);
      dataToMCH->GetYaxis()->SetTitleSize(0.04 * yscale);
      canvasFilled=true;
    }

  c->Modified();
  c->Update();
}

//
TString getPlotAsTable(TList *stack, TList *spimpose, TList *data)
{
  TString tabtex("");

  bool createHeader(true);
  TString colfmt="l";
  TString colnames="";
  TList *alllists[] = {stack, spimpose,data};
  for(size_t i=0; i<3; i++)
    {
      TList *ll = alllists[i];
      if(ll==0) continue;
      {
	TObject *key=0;
	TIterator *pIt = ll->MakeIterator();
	while ( (key = pIt->Next()) ) 
	  {
	    TH1 *p = (TH1 *) key;
	    Int_t nbins=p->GetXaxis()->GetNbins();
	    if(createHeader)
	      {
		for(Int_t iibin=1; iibin<=nbins; iibin++)
		  {
		    colfmt += "c";
		    colnames += " & ";
		    colnames += p->GetXaxis()->GetBinLabel(iibin);
		  }
		tabtex +=  "\\begin{table}[htp]\n";
		tabtex += "\\begin{center}\n";
		tabtex += "\\caption{}\n";
		tabtex += "\\label{tab:table}\n";
		tabtex += "\\begin{tabular}{"+colfmt+"} \\hline\n";
		tabtex += "Process " + colnames + "\\\\ \\hline\\hline\n";
		createHeader=false;
	      }


	    tabtex += p->GetTitle() ;
	    for(Int_t iibin=1; iibin<=nbins; iibin++)
	      {
		float val = p->GetBinContent(iibin);
                float valerr = p->GetBinError(iibin);
		tabtex += " & ";
		tabtex += val;
		tabtex += " $\\pm$ ";
		tabtex += valerr;
	      }
            tabtex += "\\\\\n";
	  }
        tabtex += "\\hline\n";
      }
    }
  tabtex += "\\end{tabular}\n";
  tabtex += "\\end{center}\n";
  tabtex += "\\end{table}\n";
  return tabtex;
}
