from ROOT import TH1

class Style:
    def __init__(self,
                 markerStyle = 8,
                 markerColor = 1,
                 lineStyle = 1,
                 lineColor = 1,
                 lineWidth = 2,
                 fillColor = None,
                 fillStyle = 3001 ):
        self.markerStyle_ = markerStyle
        self.markerColor_ = markerColor
        self.lineStyle_ = lineStyle
        self.lineColor_ = lineColor
        self.lineWidth_ = lineWidth
        if fillColor is None:
            self.fillColor_ = lineColor
        else:
            self.fillColor_ = fillColor
        self.fillStyle_ = fillStyle
        
    def formatHisto( self, hist, title=None):
        hist.SetMarkerStyle( self.markerStyle_ )
        hist.SetMarkerColor( self.markerColor_ )
        hist.SetLineStyle( self.lineStyle_ )
        hist.SetLineColor( self.lineColor_ )
        hist.SetLineWidth( self.lineWidth_ )
        hist.SetFillColor( self.fillColor_ )
        hist.SetFillStyle( self.fillStyle_ )
        hist.GetXaxis().SetTitleSize(0.05)
        hist.GetYaxis().SetTitleSize(0.05)
        hist.GetYaxis().SetTitleOffset(1.5)
        if title!=None:
            hist.SetTitle( title )
        return hist

def formatPad( pad ):
    pad.SetLeftMargin(0.15)
    pad.SetBottomMargin(0.12)


# the following standard files are defined and ready to be used.
# more standard styles can be added on demand.
# user defined styles can be created in the same way in any python module

sBlack = Style()
sBlue = Style(lineColor=4, markerColor=4)
sGreen = Style(lineColor=8, markerColor=8)
sRed = Style(lineColor=2, markerColor=2)
sYellow = Style(lineColor=1, markerColor=5, fillColor=5)


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
