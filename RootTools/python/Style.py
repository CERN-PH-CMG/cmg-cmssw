from ROOT import TH1

class Style:
    def __init__(self,
                 markerStyle = 8,
                 markerColor = 1,
                 lineStyle = 1,
                 lineColor = 1,
                 lineWidth = 2 ):
        self.markerStyle_ = markerStyle
        self.markerColor_ = markerColor
        self.lineStyle_ = lineStyle
        self.lineColor_ = lineColor
        self.lineWidth_ = lineWidth
        
        
    def formatHisto( self, hist, title=None):
        hist.SetMarkerStyle( self.markerStyle_ )
        hist.SetMarkerColor( self.markerColor_ )
        hist.SetLineStyle( self.lineStyle_ )
        hist.SetLineColor( self.lineColor_ )
        hist.SetLineWidth( self.lineWidth_ )
        hist.GetXaxis().SetTitleSize(0.05)
        hist.GetYaxis().SetTitleSize(0.05)
        hist.GetYaxis().SetTitleOffset(1.5)
        if title!=None:
            hist.SetTitle( title )
        return hist

def formatPad( pad ):
    pad.SetLeftMargin(0.15)
    pad.SetBottomMargin(0.12)

# the following are the styles to be used by everybody.
# more standard styles can be added on demand.

sBlack = Style()
sBlue = Style(lineColor=4, markerColor=4)
sGreen = Style(lineColor=8, markerColor=8)
sRed = Style(lineColor=2, markerColor=2)


sBlackSquares = Style(markerStyle = 21)
sBlueSquares = Style(lineColor=4, markerStyle = 21, markerColor=4)
sGreenSquares = Style( lineColor=8, markerStyle = 21, markerColor=8 )
sRedSquares = Style(lineColor=2, markerStyle = 21, markerColor=2)


styleSet = [sBlack, sBlue, sGreen, sRed, sBlackSquares, sBlueSquares, sGreenSquares, sRedSquares]
iStyle = 0

def nextStyle():
    global iStyle
    style = styleSet[iStyle]
    iStyle = iStyle+1
    if iStyle>=len(styleSet):
        iStyle = 0
    return style
