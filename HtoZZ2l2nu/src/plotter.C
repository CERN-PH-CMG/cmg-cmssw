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
  
  //start with the legend with the data
  std::vector<std::pair<TObject *,TString> > allKeys;
  TObject *key = 0; 
  TIterator *dataIt = data.MakeIterator();
  while ( (key = dataIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
    }
  
  //add the plots to superimpose
  TIterator *spimposeIt = spimpose.MakeIterator();
  while ( (key = spimposeIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      allKeys.push_back(std::pair<TObject *,TString>(p,p->GetTitle()));
    }

  //loop over the stack in reversed sense
  THStack *hstack = new THStack(name+"_stack",title);  
  TIterator *reverseStackIt = stack.MakeIterator(kIterBackward);
  while ( (key = reverseStackIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
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


  //prepare the sum and build the stack
  TH1 *p=(TH1 *) stack.First();
  TH1 *refFrame=(TH1 *) p->Clone(name+"_frame");
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
  bool canvasFilled(false);
  if(refFrame->Integral()>0)
    {
      refFrame->Draw("hist");  
      hstack->Draw("histsame"); 
      canvasFilled=true;
    }

  //draw the super impose distributions
  spimposeIt = spimpose.MakeIterator();
  while ( (key = spimposeIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      if(canvasFilled) p->Draw("histsame");
      else { p->Draw("hist"); canvasFilled=true; }
    }

  //draw the data
  dataIt = data.MakeIterator();
  while ( (key = dataIt->Next()) ) 
    {
      TH1 *p = (TH1 *) key;
      if(canvasFilled) p->Draw("e1same");
      else { p->Draw("e1"); canvasFilled=true; }
    }

  //draw the legend
  if(canvasFilled) leg->Draw("same");

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
