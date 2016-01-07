from ROOT import kBlack, TPaveText

def officialStyle(style):
    style.SetCanvasColor     (0)
    style.SetCanvasBorderSize(10)
    style.SetCanvasBorderMode(0)
    style.SetCanvasDefH      (700)
    style.SetCanvasDefW      (700)
    style.SetCanvasDefX      (100)
    style.SetCanvasDefY      (100)

    # color palette for 2D temperature plots
    # style.SetPalette(1,0)

    # Pads
    style.SetPadColor       (0)
    style.SetPadBorderSize  (10)
    style.SetPadBorderMode  (0)
    style.SetPadBottomMargin(0.13)
    style.SetPadTopMargin   (0.08)
    style.SetPadLeftMargin  (0.15)
    style.SetPadRightMargin (0.05)
    style.SetPadGridX       (0)
    style.SetPadGridY       (0)
    style.SetPadTickX       (1)
    style.SetPadTickY       (1)

    # Frames
    style.SetLineWidth(3)
    style.SetFrameFillStyle ( 0)
    style.SetFrameFillColor ( 0)
    style.SetFrameLineColor ( 1)
    style.SetFrameLineStyle ( 0)
    style.SetFrameLineWidth ( 2)
    style.SetFrameBorderSize(10)
    style.SetFrameBorderMode( 0)

    # Histograms
    style.SetHistFillColor(2)
    style.SetHistFillStyle(0)
    style.SetHistLineColor(1)
    style.SetHistLineStyle(0)
    style.SetHistLineWidth(3)
    style.SetNdivisions(505)

    # Functions
    style.SetFuncColor(1)
    style.SetFuncStyle(0)
    style.SetFuncWidth(2)

    # Various
    style.SetMarkerStyle(20)
    style.SetMarkerColor(kBlack)
    style.SetMarkerSize (1.4)

    style.SetTitleBorderSize(0)
    style.SetTitleFillColor (0)
    style.SetTitleX         (0.2)

    style.SetTitleSize  (0.055,"X")
    style.SetTitleOffset(1.200,"X")
    style.SetLabelOffset(0.005,"X")
    style.SetLabelSize  (0.050,"X")
    style.SetLabelFont  (42   ,"X")

    style.SetStripDecimals(False)
    style.SetLineStyleString(11,"20 10")

    style.SetTitleSize  (0.055,"Y")
    style.SetTitleOffset(1.600,"Y")
    style.SetLabelOffset(0.010,"Y")
    style.SetLabelSize  (0.050,"Y")
    style.SetLabelFont  (42   ,"Y")

    style.SetTextSize   (0.055)
    style.SetTextFont   (42)

    style.SetStatFont   (42)
    style.SetTitleFont  (42)
    style.SetTitleFont  (42,"X")
    style.SetTitleFont  (42,"Y")

    style.SetOptStat    (0)


def CMSPrelim(dataset, channel, lowX, lowY):
    cmsprel  =  TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    cmsprel.SetBorderSize(   0 )
    cmsprel.SetFillStyle(    0 )
    cmsprel.SetTextAlign(   12 )
    cmsprel.SetTextSize ( 0.04 )
    cmsprel.SetTextColor(    1 )
    cmsprel.SetTextFont (   62 )
    cmsprel.AddText(dataset)
    
##     lumi     =  TPaveText(lowX+0.38, lowY+0.061, lowX+0.45, lowY+0.161, "NDC")
##     lumi.SetBorderSize(   0 )
##     lumi.SetFillStyle(    0 )
##     lumi.SetTextAlign(   12 )
##     lumi.SetTextSize ( 0.04 )
##     lumi.SetTextColor(    1 )
##     lumi.SetTextFont (   62 )
##     lumi.AddText(dataset)
    
    chan     =  TPaveText(lowX+0.68, lowY+0.061, lowX+0.80, lowY+0.161, "NDC")
    chan.SetBorderSize(   0 )
    chan.SetFillStyle(    0 )
    chan.SetTextAlign(   12 )
    chan.SetTextSize ( 0.05 )
    chan.SetTextColor(    1 )
    chan.SetTextFont (   62 )
    chan.AddText(channel)
    
    return cmsprel, chan

import ROOT as rt

def tdrGrid( gridOn):
  tdrStyle.SetPadGridX(gridOn)
  tdrStyle.SetPadGridY(gridOn)

#fixOverlay: Redraws the axis
def fixOverlay(): gPad.RedrawAxis()


def setTDRStyle():
  tdrStyle =  rt.TStyle("tdrStyle","Style for P-TDR")

   #for the canvas:
  tdrStyle.SetCanvasBorderMode(0)
  tdrStyle.SetCanvasColor(rt.kWhite)
  tdrStyle.SetCanvasDefH(600) #Height of canvas
  tdrStyle.SetCanvasDefW(600) #Width of canvas
  tdrStyle.SetCanvasDefX(0)   #POsition on screen
  tdrStyle.SetCanvasDefY(0)


  tdrStyle.SetPadBorderMode(0)
  #tdrStyle.SetPadBorderSize(Width_t size = 1)
  tdrStyle.SetPadColor(rt.kWhite)
  tdrStyle.SetPadGridX(False)
  tdrStyle.SetPadGridY(False)
  tdrStyle.SetGridColor(0)
  tdrStyle.SetGridStyle(3)
  tdrStyle.SetGridWidth(1)

#For the frame:
  tdrStyle.SetFrameBorderMode(0)
  tdrStyle.SetFrameBorderSize(1)
  tdrStyle.SetFrameFillColor(0)
  tdrStyle.SetFrameFillStyle(0)
  tdrStyle.SetFrameLineColor(1)
  tdrStyle.SetFrameLineStyle(1)
  tdrStyle.SetFrameLineWidth(1)
  
#For the histo:
  #tdrStyle.SetHistFillColor(1)
  #tdrStyle.SetHistFillStyle(0)
  tdrStyle.SetHistLineColor(1)
  tdrStyle.SetHistLineStyle(0)
  tdrStyle.SetHistLineWidth(1)
  #tdrStyle.SetLegoInnerR(Float_t rad = 0.5)
  #tdrStyle.SetNumberContours(Int_t number = 20)

  tdrStyle.SetEndErrorSize(2)
  #tdrStyle.SetErrorMarker(20)
  #tdrStyle.SetErrorX(0.)
  
  tdrStyle.SetMarkerStyle(20)
  
#For the fit/function:
  tdrStyle.SetOptFit(1)
  tdrStyle.SetFitFormat("5.4g")
  tdrStyle.SetFuncColor(2)
  tdrStyle.SetFuncStyle(1)
  tdrStyle.SetFuncWidth(1)

#For the date:
  tdrStyle.SetOptDate(0)
  # tdrStyle.SetDateX(Float_t x = 0.01)
  # tdrStyle.SetDateY(Float_t y = 0.01)

# For the statistics box:
  tdrStyle.SetOptFile(0)
  tdrStyle.SetOptStat(0) # To display the mean and RMS:   SetOptStat("mr")
  tdrStyle.SetStatColor(rt.kWhite)
  tdrStyle.SetStatFont(42)
  tdrStyle.SetStatFontSize(0.025)
  tdrStyle.SetStatTextColor(1)
  tdrStyle.SetStatFormat("6.4g")
  tdrStyle.SetStatBorderSize(1)
  tdrStyle.SetStatH(0.1)
  tdrStyle.SetStatW(0.15)
  # tdrStyle.SetStatStyle(Style_t style = 1001)
  # tdrStyle.SetStatX(Float_t x = 0)
  # tdrStyle.SetStatY(Float_t y = 0)

# Margins:
  tdrStyle.SetPadTopMargin(0.05)
  tdrStyle.SetPadBottomMargin(0.13)
  tdrStyle.SetPadLeftMargin(0.16)
  tdrStyle.SetPadRightMargin(0.02)

# For the Global title:

  tdrStyle.SetOptTitle(0)
  tdrStyle.SetTitleFont(42)
  tdrStyle.SetTitleColor(1)
  tdrStyle.SetTitleTextColor(1)
  tdrStyle.SetTitleFillColor(10)
  tdrStyle.SetTitleFontSize(0.05)
  # tdrStyle.SetTitleH(0) # Set the height of the title box
  # tdrStyle.SetTitleW(0) # Set the width of the title box
  # tdrStyle.SetTitleX(0) # Set the position of the title box
  # tdrStyle.SetTitleY(0.985) # Set the position of the title box
  # tdrStyle.SetTitleStyle(Style_t style = 1001)
  # tdrStyle.SetTitleBorderSize(2)

# For the axis titles:

  tdrStyle.SetTitleColor(1, "XYZ")
  tdrStyle.SetTitleFont(42, "XYZ")
  tdrStyle.SetTitleSize(0.06, "XYZ")
  # tdrStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
  # tdrStyle.SetTitleYSize(Float_t size = 0.02)
  tdrStyle.SetTitleXOffset(0.9)
  tdrStyle.SetTitleYOffset(1.25)
  # tdrStyle.SetTitleOffset(1.1, "Y") # Another way to set the Offset

# For the axis labels:

  tdrStyle.SetLabelColor(1, "XYZ")
  tdrStyle.SetLabelFont(42, "XYZ")
  tdrStyle.SetLabelOffset(0.007, "XYZ")
  tdrStyle.SetLabelSize(0.05, "XYZ")

# For the axis:

  tdrStyle.SetAxisColor(1, "XYZ")
  tdrStyle.SetStripDecimals(True)
  tdrStyle.SetTickLength(0.03, "XYZ")
  tdrStyle.SetNdivisions(510, "XYZ")
  tdrStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
  tdrStyle.SetPadTickY(1)

# Change for log plots:
  tdrStyle.SetOptLogx(0)
  tdrStyle.SetOptLogy(0)
  tdrStyle.SetOptLogz(0)

# Postscript options:
  tdrStyle.SetPaperSize(20.,20.)
  # tdrStyle.SetLineScalePS(Float_t scale = 3)
  # tdrStyle.SetLineStyleString(Int_t i, const char* text)
  # tdrStyle.SetHeaderPS(const char* header)
  # tdrStyle.SetTitlePS(const char* pstitle)

  # tdrStyle.SetBarOffset(Float_t baroff = 0.5)
  # tdrStyle.SetBarWidth(Float_t barwidth = 0.5)
  # tdrStyle.SetPaintTextFormat(const char* format = "g")
  # tdrStyle.SetPalette(Int_t ncolors = 0, Int_t* colors = 0)
  # tdrStyle.SetTimeOffset(Double_t toffset)
  # tdrStyle.SetHistMinimumZero(kTRUE)

  tdrStyle.SetHatchesLineWidth(5)
  tdrStyle.SetHatchesSpacing(0.05)

  tdrStyle.cd()
  