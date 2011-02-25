#include "CMGTools/RootTools/interface/Styles.h"

#include <TROOT.h>
#include <TClass.h>

#include <TStyle.h>
#include <string>
#include <TList.h>
#include <vector>

#include <iostream>

using namespace std; 


Styles::Styles() {

  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetHistMinimumZero(kTRUE);

  s1 = new Style(); 

  s1->SetLineWidth(2);   
  s1->SetLineColor(1);   

  s2 = new Style(); 

  s2->SetLineWidth(2);   
  s2->SetLineColor(4);   

  sg1 = new Style();

  sg1->SetMarkerColor(4);
  sg1->SetLineColor(4);
  sg1->SetLineWidth(2);  
  sg1->SetMarkerStyle(21);

  spyellow =  new Style();
  spyellow->SetMarkerColor(kYellow-3); 
  spyellow->SetMarkerStyle(8); 
  spyellow->SetLineColor(kYellow-3); 
  spyellow->SetLineWidth(2);  
  spyellow->SetFillStyle(3002); 
  spyellow->SetFillColor(kYellow); 
 
  spred = new Style();
  spred->SetMarkerColor(kRed); 
  spred->SetMarkerStyle(8); 
  spred->SetLineColor(kRed); 
  spred->SetLineWidth(2);  
  spred->SetFillStyle(3002); 
  spred->SetFillColor(kRed-8); 

  spblue = new Style();
  spblue->SetMarkerColor(kBlue); 
  spblue->SetMarkerStyle(8); 
  spblue->SetLineColor(4); 
  spblue->SetLineWidth(2);
  spblue->SetFillStyle(3002); 
  spblue->SetFillColor(kBlue-8); 

  spgreen = new Style();
  spgreen->SetMarkerColor(kGreen-6); 
  spgreen->SetMarkerStyle(8); 
  spgreen->SetLineColor(kGreen); 
  spgreen->SetLineWidth(2);
  spgreen->SetFillStyle(3002); 
  spgreen->SetFillColor(kGreen-6); 

  sppink1 = new Style();
  sppink1->SetMarkerColor(kPink-4); 
  sppink1->SetMarkerStyle(8); 
  sppink1->SetLineColor(kPink); 
  sppink1->SetLineWidth(2);
  sppink1->SetFillStyle(3002); 
  sppink1->SetFillColor(kPink-4); 

  sppink2 = new Style();
  sppink2->SetMarkerColor(kPink+4); 
  sppink2->SetMarkerStyle(8); 
  sppink2->SetLineColor(kRed); 
  sppink2->SetLineWidth(2);
  sppink2->SetFillStyle(3002); 
  sppink2->SetFillColor(kPink+4); 

  sgr1 = new Style();
  sgr1->SetLineWidth(1);
  sgr1->SetLineColor(1);

  sgr2 = new Style(); 
  sgr2->SetLineWidth(1);
  sgr2->SetLineColor(1);

  sdata1 = new Style();
  sdata1->SetMarkerStyle(8);
  sdata1->SetMarkerColor(1);
  sdata1->SetLineColor(1);
  sdata1->SetLineWidth(2);

  sdata2 = new Style();
  sdata2->SetMarkerStyle(24);
  sdata2->SetMarkerColor(4);
  sdata2->SetLineColor(4);
  sdata2->SetLineWidth(2);
  
  spgray = new Style();
  spgray->SetMarkerColor(kGray); 
  spgray->SetMarkerStyle(8); 
  spgray->SetLineColor(kBlack); 
  spgray->SetLineWidth(2);
  spgray->SetFillStyle(3002); 
  spgray->SetFillColor(kGray); 

}

void Styles::FormatHisto( TH1* h, const Style* s, bool solidFill) {

  h->SetStats(0);

  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleOffset(1.35);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetLabelSize(0.045);
  h->GetXaxis()->SetLabelSize(0.045);
  h->GetXaxis()->SetNdivisions(5);
  h->GetYaxis()->SetNdivisions(5); 

//   h->SetTitle( title );

  if(s) {
    h->SetLineWidth( s->GetLineWidth() );
    h->SetLineColor( s->GetLineColor() );
    if(!solidFill)
      h->SetFillStyle( s->GetFillStyle() );
    else 
      h->SetFillStyle( 1001 );

    h->SetFillColor( s->GetFillColor() );
    h->SetMarkerColor( s->GetMarkerColor() );
    h->SetMarkerStyle( s->GetMarkerStyle() );
  }
}

void Styles::FormatPad( TPad* pad, bool grid, bool logy, bool logx) {
  
  pad->SetGridx(grid);
  pad->SetGridy(grid);

  pad->SetLogx( logx );
  pad->SetLogy( logy );

  pad->SetBottomMargin(0.14);
  pad->SetLeftMargin(0.15);
  pad->SetRightMargin(0.1);
  pad->Modified();
  pad->Update();
}


void Styles::SavePlot(const char* name, const char* dir) {
  string eps = dir;
  eps += "/";
  eps += name;
  eps += ".eps";
  gPad->SaveAs( eps.c_str() );

  string png = dir;
  png += "/";
  png += name;
  png += ".png";
  gPad->SaveAs( png.c_str() );

}

void Styles::SetMax( TVirtualPad* pad, float safetyFactor) {
  
  
  TList* prim = pad->GetListOfPrimitives();

  // iterate on the list
  // keep track of all histograms 
  // and find hist with max height
  
  TIter next( prim );
  TObject *obj = 0;

  float maxmax = -1;

  vector< TH1* > histos;
  while ( (bool) (obj = next()) ) {

    if (! obj->IsA()->InheritsFrom( "TH1" ) ) 
      continue;
    
    TH1* histo = (TH1*) obj;
    histos.push_back( histo );

    float max = histo->GetMaximum();
    
    if( max > maxmax) 
      maxmax = max;
    
  }

  maxmax *= safetyFactor;

  for( unsigned i=0; i<histos.size(); ++i) {
    histos[i]->SetMaximum( maxmax );
    histos[i]->SetMinimum(0.000001);
  }

  pad->Modified();
  pad->Update();
  
  
}



void Styles::ScaleToOne( TPad* pad, float safetyFactor) {
  
  TList* prim = pad->GetListOfPrimitives();

  TIter next( prim );
  TObject *obj = 0;

  vector< TH1* > histos;
  while ( (bool) (obj = next()) ) {

    if (! obj->IsA()->InheritsFrom( "TH1" ) ) 
      continue;
    
    TH1* histo = (TH1*) obj;
 
    histo->Scale( 1/histo->GetEntries() );
  }

  SetMax( pad, safetyFactor );

  pad->Modified();
  pad->Update();
  
}
