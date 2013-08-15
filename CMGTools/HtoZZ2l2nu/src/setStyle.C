#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

using namespace std;

//
TStyle *setStyle(bool gridOn) {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetHatchesSpacing(1.0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(gridOn);
  tdrStyle->SetPadGridY(gridOn);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  
  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);
  
  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);
  
  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.1);

  // For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.05, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.8);
  tdrStyle->SetTitleYOffset(1.0);
  //  tdrStyle->SetTitleOffset(1.25, "Y"); // Another way to set the Offset

  // For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.04, "XYZ");

  // For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  tdrStyle->SetPalette(1);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();
  return tdrStyle;
}



//
TCanvas *getNewCanvas(TString name, TString title,bool setBatch)
{
  
  TCanvas *c1 = new TCanvas(name,title,1);
  if(setBatch)  c1->SetBatch();
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.05);
  c1->SetBottomMargin(0.1);
  c1->SetTopMargin(0.05);
  c1->SetGridx(false);
  c1->SetGridy(false);
  
  return c1;
}


//
TObject *formatPlot(TObject *h, 
		    int color, int line, int linew, int marker, int fill, 
		    bool detachFromCurrentDir, bool setSumw2, 
		    int linecolor, int fillcolor, int markercolor)
{
  if(h==0) return h;
  if( linecolor < 0 ) linecolor=color;  
  if( fillcolor < 0 ) fillcolor=color; 
  if( markercolor < 0 ) markercolor=color;
  if( h->IsA()->InheritsFrom(TAttLine::Class()) ) {
    TAttLine * l = dynamic_cast<TAttLine *>(h);
    l->SetLineStyle(line);   l->SetLineWidth(linew); l->SetLineColor(linecolor);
  }
  if( h->IsA()->InheritsFrom(TAttMarker::Class()) ) {
    TAttMarker * m =  dynamic_cast<TAttMarker *>(h); 
    m->SetMarkerColor(markercolor);  m->SetMarkerStyle(marker); 
  }
  if( h->IsA()->InheritsFrom(TAttFill::Class()) ) {
    TAttFill * f = dynamic_cast<TAttFill *>(h); 
    f->SetFillColor(fillcolor); f->SetFillStyle(fill);
  }
  if( h->IsA()->InheritsFrom(TH1::Class()) ) {
    TH1 * h1 = (TH1 *) h; 
    h1->SetLineStyle(line);          h1->SetLineWidth(linew); h1->SetLineColor(linecolor);
    h1->SetMarkerColor(markercolor); h1->SetMarkerStyle(marker); 
    h1->SetFillColor(fillcolor);     h1->SetFillStyle(fill);
    if( setSumw2 && h1->GetSumw2N()<=h1->GetNbinsX()) h1->Sumw2();
    if( detachFromCurrentDir ) h1->SetDirectory(0);
    ((TH1 *)h)->SetLabelFont(42, "XYZ");
    ((TH1 *)h)->SetTitleFont(62, "XYZ");
  }

  
  return h;
}

//
void fixExtremities(TH1 *h,bool addOverflow, bool addUnderflow)
{
  if(h==0) return;

  if(addUnderflow)
    {
      double fbin  = h->GetBinContent(0) + h->GetBinContent(1);
      double fbine = sqrt(h->GetBinError(0)*h->GetBinError(0)
			  + h->GetBinError(1)*h->GetBinError(1));
      h->SetBinContent(1,fbin);
      h->SetBinError(1,fbine);
      h->SetBinContent(0,0);
      h->SetBinError(0,0);
    }
  
  if(addOverflow)
    {  
      int nbins = h->GetNbinsX();
      double fbin  = h->GetBinContent(nbins) + h->GetBinContent(nbins+1);
      double fbine = sqrt(h->GetBinError(nbins)*h->GetBinError(nbins) 
			  + h->GetBinError(nbins+1)*h->GetBinError(nbins+1));
      h->SetBinContent(nbins,fbin);
      h->SetBinError(nbins,fbine);
      h->SetBinContent(nbins+1,0);
      h->SetBinError(nbins+1,0);
    }
}

//
void formatForCmsPublic(TPad * c, TLegend *leg, TString title, int nsamp, float legx, float legy, float legw, float legh, TString legopt)
{
  if(title.Length()!=0)
    {
      TPaveText *pave = new TPaveText(0.5,0.96,0.94,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);

      TObjArray * tokens = title.Tokenize("\\\\");
      int nt = tokens->GetEntries();
      for(int it=0; it<nt; ++it) 
	{
	  TObjString * t = (TObjString *)tokens->At(it);
	  pave->AddText(t->GetString());
	}

      pave->Draw("same");
    }

  float legx1=legx, legx2=legx+legw;
  float legy1 = legy, legy2 = legy-legh*(float)(nsamp);  
  if(leg==0) leg = ((TPad *)c)->BuildLegend(legx1,legy1,legx2,legy2);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetLineWidth(1);
  leg->SetTextFont(42);
  leg->SetEntryOption(legopt);
  leg->SetX1NDC(legx1);
  leg->SetY1NDC(legy1);//-nsamp*legh);
  leg->SetX2NDC(legx2);
  leg->SetY2NDC(legy2);
}


