#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

using namespace std;

//
TH1 *getIntegralFrom(TH1 *h,TString newname)
{
  TH1 *intH=0;
  if(h==0) return intH;

  intH = (TH1 *) h->Clone(newname);
  intH->Reset("ICE");
  
  Int_t nbins=h->GetXaxis()->GetNbins();
  for(Int_t ibin=1; ibin<=nbins; ibin++)
    {
      Double_t val(0),val_err(0);
      val = h->IntegralAndError(ibin,nbins,val_err,"width");
      intH->SetBinContent(ibin,val);
      intH->SetBinError(ibin,val_err);
    }
  
  return intH;
}

//
std::pair<TH1D *,TH1D *> getProjections(TH2D *histo)
{
  TH1D *px=histo->ProjectionX(histo->GetName()+TString("_px"));
  formatPlot(px,
	     histo->GetLineColor(),histo->GetLineStyle(),histo->GetLineWidth(),
	     histo->GetMarkerStyle(),histo->GetFillStyle(),
	     false,false,
	     histo->GetLineColor(),histo->GetFillColor(),histo->GetMarkerColor());
  TH1D *py=histo->ProjectionY(histo->GetName()+TString("_py"));
  formatPlot(py,
	     histo->GetLineColor(),histo->GetLineStyle(),histo->GetLineWidth(),
	     histo->GetMarkerStyle(),histo->GetFillStyle(),
	     false,false,
	     histo->GetLineColor(),histo->GetFillColor(),histo->GetMarkerColor());
  return std::pair<TH1D *,TH1D *>(px,py);
}


//
TLegend *showPlotsAndMCtoDataComparison(TPad *p, TList &stack, TList &spimpose, TList &data, bool setLogy,bool forceNormalization,bool doIntegral)
{
  p->Divide(1,2);
  TPad *subp=(TPad *)p->cd(1);
  subp->SetPad(0,0.3,1.0,1.0);
  TLegend *leg=showPlots(subp,stack,spimpose,data,true,"lpf",forceNormalization,doIntegral);
  formatForCmsPublic(subp,leg,"",5);
  if(setLogy) subp->SetLogy();
  
  subp=(TPad *)p->cd(2);
  subp->SetPad(0,0.0,1.0,0.28);
  subp->SetTopMargin(0);
  subp->SetBottomMargin(0.5);
  float yscale = (1.0-0.3)/(0.28-0);
  showMCtoDataComparison(subp,stack,data,false,yscale,forceNormalization,doIntegral);
  
  return leg;
}


//
TLegend *showPlots(TPad *c, TList &origstack, TList &origspimpose, TList &origdata, bool buildLegend, TString legopt, bool forceNormalization, bool doIntegral)
{
  if(c==0) return 0;
  if( origstack.First()==0 && origspimpose.First()==0 && origdata.First()==0 ) return 0;

  //prepare the pad
  c->cd();
  //c->SetGridx();
  //c->SetGridy();
  c->Clear();
  TString name=c->GetName();
  TString title=c->GetTitle();
  
  bool th2dfound(false), graphfound(false), proffound(false);

  //start with the legend with the data
  std::vector<std::pair<TObject *,TString> > allKeys;
  TObject *key = 0; 
  TList data;
  TIterator *dataIt = origdata.MakeIterator();
  double totalData(0);
  while ( (key = dataIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      th2dfound |= ((TClass*)key->IsA())->InheritsFrom("TH2");
      proffound |= ((TClass*)key->IsA())->InheritsFrom("TProfile");
      graphfound |= ((TClass*)key->IsA())->InheritsFrom("TGraph");
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
      TString newname("data"); newname += allKeys.size();
      if(!doIntegral) data.AddFirst ( p->Clone(newname) );
      else            data.AddFirst ( getIntegralFrom(p,newname) );
      totalData += p->Integral(0,p->GetXaxis()->GetNbins()+1,"width");
    }
  
  //add the plots to superimpose
  TList spimpose;
  TIterator *spimposeIt = origspimpose.MakeIterator();
   while ( (key = spimposeIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      th2dfound |= ((TClass*)key->IsA())->InheritsFrom("TH2");
      proffound |= ((TClass*)key->IsA())->InheritsFrom("TProfile");
      graphfound |= ((TClass*)key->IsA())->InheritsFrom("TGraph");
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
      TString newname("gr"); newname += allKeys.size();
      if(!doIntegral) spimpose.AddFirst ( p->Clone(newname) );
      else            spimpose.AddFirst ( getIntegralFrom(p,newname) );
    }

  //loop over the stack in reversed sense
  THStack *hstack = new THStack("stack",title);  
  TList stack;
  TIterator *reverseStackIt = origstack.MakeIterator(kIterBackward);
 double totalMC(0); 
 while ( (key = reverseStackIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      th2dfound |= ((TClass*)key->IsA())->InheritsFrom("TH2");
      proffound |= ((TClass*)key->IsA())->InheritsFrom("TProfile");
      graphfound |= ((TClass*)key->IsA())->InheritsFrom("TGraph");
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
      TString newname("h"); newname += allKeys.size();
      if(!doIntegral) stack.AddFirst ( p->Clone(newname) );
      else            stack.AddFirst ( getIntegralFrom(p,newname) );  
      totalMC += p->Integral(0,p->GetXaxis()->GetNbins()+1,"width");
    }


  double sf=1.0;
  if(forceNormalization && totalMC>0){
    sf=totalData/totalMC;
    TIterator *spimposeIt = spimpose.MakeIterator();
    while ( (key = spimposeIt->Next()) )
      {
	TH1 *p = (TH1 *) key;
	double totalp=p->Integral(0,p->GetXaxis()->GetNbins()+1,"width");
	if(totalp>0)
	  p->Scale(totalData/totalp);
      }
  }
  cout << "Force normalization by: " << sf << "(" << forceNormalization << ")" << endl;


  //build the legend
  TLegend *leg = new TLegend(0.68,0.96-0.05*allKeys.size(),0.9,0.96,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetFillColor(0);
  leg->SetTextFont(42);
  for(std::vector<std::pair<TObject *,TString> >::iterator kIt = allKeys.begin();
      kIt != allKeys.end();
      kIt++)
    leg->AddEntry( kIt->first, kIt->second, legopt );      

  TH1 *refFrame=0;  
  if(th2dfound || graphfound || proffound)
    {
      int nplots=allKeys.size();
      int nx = sqrt(nplots*1.0)+1;
      int ny =  sqrt(nplots*1.0);

      TList stackx,stacky;
      TList spimposex,spimposey;
      TList datax,datay;

      c->Divide(nx,ny);
      int ipad(0);
      reverseStackIt = stack.MakeIterator(kIterBackward);
      while ( (key = reverseStackIt->Next()) ) 
	{
	  ipad++;
	  TPad *p = (TPad *)c->cd(ipad);
	  if(key==0 || p==0) continue;
	  if(th2dfound && ((TH2D *)key)->Integral()==0) continue;
	  //key->Draw( th2dfound ? "box" : "ap");
	  key->Draw( th2dfound ? "colz" : (proffound ? "e1" : "ap") );
	  TLegend *leg=p->BuildLegend();
	  if(leg)
	    {
	      leg->SetBorderSize(1);
	      leg->SetFillColor(0);
	      leg->SetTextFont(42);
	      formatForCmsPublic(p,leg,"",1);
	    }
	  
	  if(!th2dfound) continue;
	  //  std::pair<TH1D *,TH1D *> pxpy = getProjections((TH2D *)key);
	  //if(pxpy.first) stackx.AddFirst(pxpy.first);
	  //if(pxpy.second) stacky.AddFirst(pxpy.second); 
	}
      
      spimposeIt = spimpose.MakeIterator();
      while ( (key = spimposeIt->Next()) ) 
	{
	  ipad++;
	  TPad *p=(TPad *)c->cd(ipad);
	  if(key==0 || p==0) continue;
	  if(th2dfound && ((TH2D *)key)->Integral()==0) continue;
	  key->Draw( th2dfound ? "colz" : (proffound ? "e1" : "ap") );
	  TLegend *leg=p->BuildLegend();
	  if(leg)
	    {
	      leg->SetBorderSize(1);
	      leg->SetFillColor(0);
	      leg->SetTextFont(42);
	      formatForCmsPublic(p,leg,"",1);
	    }
	  
	  if(!th2dfound) continue;
	  //std::pair<TH1D *,TH1D *> pxpy = getProjections((TH2D *)key);
	  //if(pxpy.first)	  spimposex.Add(pxpy.first);
	  //if(pxpy.second)         spimposey.Add(pxpy.second); 
	}
      
      //draw the data
      bool hasData(false);
      dataIt = data.MakeIterator();
      while ( (key = dataIt->Next()) ) 
	{
	  hasData=true;
	  ipad++;
	  TPad *p=(TPad *)c->cd(ipad);
	  if(p==0) continue;
	  if(key==0 || p==0) continue;
	  if(th2dfound && ((TH2D *)key)->Integral()==0) continue;
	  key->Draw( th2dfound ? "colz" : (proffound ? "e1" : "ap") );
	  TLegend *leg=p->BuildLegend();
	  if(leg)
	    {
	      leg->SetBorderSize(1);
	      leg->SetFillColor(0);
	      leg->SetTextFont(42);
	      formatForCmsPublic(p,leg,"",1);
	    }

	  if(!th2dfound) continue;
	  //std::pair<TH1D *,TH1D *> pxpy = getProjections((TH2D *)key);
	  //if(pxpy.first)  datax.Add(pxpy.first);
	  //if(pxpy.second) datay.Add(pxpy.second); 
	}

      //project it
      /*      ipad++;
      TPad *p=(TPad *)c->cd(ipad);
      if(hasData) showPlotsAndMCtoDataComparison(p,stackx,spimposex,datax);
      else        showPlots(p,stackx,spimposex,datax);

      ipad++;
      p=(TPad *)c->cd(ipad);
      if(hasData) showPlotsAndMCtoDataComparison(p,stacky,spimposey,datay);
      else        showPlots(p,stacky,spimposey,datay);
      */
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
	      p->Scale(sf);
	      refFrame->Add(p);
	      hstack->Add( p );
	    }
	  
	  //draw the frame and the stack
	  if(refFrame->Integral()>0)
	    {
	      refFrame->Draw("hist");
	      //refFrame->GetYaxis()->SetRangeUser(1e-3,1.0);
	      //refFrame->GetYaxis()->SetRangeUser(1e-3,1e6);
	      refFrame->GetYaxis()->SetRangeUser(1e-3,refFrame->GetMaximum()*1.25);
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
	  if(canvasFilled) p->Draw("e1same");
	  else {
	    p->Draw("e1"); 
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
void showMCtoDataComparison(TPad *c, TList &stack, TList &data, bool doDiff,float yscale,bool forceNormalization, bool doIntegral)
{
  if(c==0) return;
  if( stack.First()==0 || data.First()==0 ) return;

  //prepare the pad
  c->cd();
  //c->SetGridx();
  c->SetGridy();
  c->Clear();
  TString name=c->GetName();
  TString title=c->GetTitle();
  
  //build the stack sum
  TH1 *sumH=0;
  TObject *key = 0; 
  TIterator *stackIt = stack.MakeIterator();
  double totalPredicted(0);
  while ( (key = stackIt->Next()) ) 
    {
      //      if( ((TClass*)key->IsA())->InheritsFrom("TH2") ) continue;
      TH1 *p = (TH1 *) key;
      if( ((TClass*)key->IsA())->InheritsFrom("TGraph") ) continue;
      if(sumH==0) { 
	sumH = (TH1 *) p->Clone(TString(p->GetName())+"sum"); 
	sumH->Reset("ICE");
      }
      totalPredicted += p->Integral(0,sumH->GetXaxis()->GetNbins()+1,"width");
      if(!doIntegral) sumH->Add(p);
      else            { TH1 *intP=getIntegralFrom(p,"tmp"); sumH->Add(intP); delete intP; }
    }

  //compare different data
  bool canvasFilled(false);
  TIterator *dataIt = data.MakeIterator();
  while ( (key = dataIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      double totalData=p->Integral(0,p->GetXaxis()->GetNbins()+1,"width");
      //      if( ((TClass*)key->IsA())->InheritsFrom("TH2") ) continue;
      if( ((TClass*)key->IsA())->InheritsFrom("TGraph") ) continue;

      TH1 *dataToMCH = !doIntegral ? 
	(TH1 *) p->Clone(TString(p->GetName())+"_tomc") :
	getIntegralFrom(p,TString(p->GetName())+"_tomc") ;
      
      if(doDiff)
	{
	  dataToMCH->GetYaxis()->SetTitle("Obs-Ref");
	  dataToMCH->Add(sumH,forceNormalization ? -totalData/totalPredicted :-1.0);
	}
      else
	{
	  dataToMCH->GetYaxis()->SetTitle("Obs/Ref");
	  if(!forceNormalization) dataToMCH->Divide(sumH);
	  else
	    {
	      dataToMCH->Reset("ICE");
	      dataToMCH->Divide(p,sumH,1.0,totalData/totalPredicted);
	    }
	}

      c->cd();
      TString opt("e1");
      if( ((TClass*)dataToMCH->IsA())->InheritsFrom("TH2") ) opt="colz";
      if(canvasFilled) opt +="same";
      dataToMCH->Draw(opt);
      dataToMCH->GetYaxis()->SetRangeUser(0,2.2);
      dataToMCH->GetXaxis()->SetTitleOffset(0.85);
      dataToMCH->GetXaxis()->SetLabelSize(0.04 * yscale);
      dataToMCH->GetXaxis()->SetTitleSize(0.05 * yscale);
      dataToMCH->GetXaxis()->SetTickLength( 0.03 * yscale );
      dataToMCH->GetYaxis()->SetTitleOffset(0.5);
      dataToMCH->GetYaxis()->SetLabelSize(0.04 * yscale);
      dataToMCH->GetYaxis()->SetTitleSize(0.04 * yscale);
      dataToMCH->GetYaxis()->SetNdivisions(5);
      canvasFilled=true;
    }

  c->Modified();
  c->Update();
}

//
TString getPlotAsTable(TList *stack, TList *spimpose, TList *data)
{
  TString tabtex("");
  TH1*stacksum=0;
  bool createHeader(true);
  TString colfmt="l";
  TString colnames="";
  TList totalStack;
  TList *alllists[] = {stack, &totalStack, spimpose,data};
  for(size_t i=0; i<4; i++)
    {
      TList *ll = alllists[i];
      if(ll==0) continue;
      {
	TObject *key=0;
	TIterator *pIt = ll->MakeIterator();
	while ( (key = pIt->Next()) ) 
	  {
	    TH1 *p = (TH1 *) key;
            if( i==0){ 
             if(stacksum==0 )
              {
                 stacksum=(TH1 *)p->Clone("stacksum");
                 totalStack.Add(stacksum);
                 stacksum->SetTitle("Total");
              }
             else stacksum->Add(p);
           }

	    Int_t nbins=p->GetXaxis()->GetNbins();
	    if(createHeader)
	      {
		for(Int_t iibin=1; iibin<=nbins; iibin++)
		  {
		    colfmt += "c";
		    colnames += " & ";
		    colnames += p->GetXaxis()->GetBinLabel(iibin);
		  }

		tabtex +=  "\\documentclass[]{report}\n";
		tabtex +=  "\\usepackage{graphics}\n";
		tabtex +=  "\\usepackage{pdflscape}\n";
		tabtex +=  "\\begin{document}\n";
		tabtex += "\\begin{landscape}\n";
		tabtex +=  "\\begin{table}[htp]\n";
		tabtex += "\\caption{}\n";
		//tabtex += "\\begin{center}\n";
		tabtex += "\\vspace{-20 cm}\n";
		tabtex += "\\scalebox{0.78}{\n";
		tabtex += "\\label{tab:table}\n";
		tabtex += "\\begin{tabular}{"+colfmt+"} \\hline\n";
		tabtex += "Process " + colnames + "\\\\ \\hline\\hline\n";
		createHeader=false;
	      }


	    tabtex += p->GetTitle() ;
	    for(Int_t iibin=1; iibin<=nbins; iibin++)
	      {
		char buf[100];
		if(i<2) sprintf(buf,"& %.3f $\\pm$ %.3f",p->GetBinContent(iibin),p->GetBinError(iibin));
		else        sprintf(buf,"& %f",p->GetBinContent(iibin));
		tabtex += buf;
	      }
            tabtex += "\\\\\n";
	  }
        tabtex += "\\hline\n";
      }
    }
  tabtex += "\\end{tabular}\n";
  tabtex += "}\n";
  //tabtex += "\\end{center}\n";
  tabtex += "\\end{table}\n";
  tabtex += "\\end{landscape}\n";
  tabtex += "\\end{document}";
  return tabtex;
}
