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
        

    def formatHisto( self, hist ):
        hist.SetMarkerStyle( self.markerStyle_ )
        hist.SetMarkerColor( self.markerColor_ )
        hist.SetLineStyle( self.lineStyle_ )
        hist.SetLineColor( self.lineColor_ )
        hist.SetLineWidth( self.lineWidth_ )
        return hist

# the following are the styles to be used by everybody.
# more standard styles can be added on demand.

sBlack = Style()
sRedPoints = Style(markerColor = 2)
sBlueSquares = Style(markerStyle = 21, markerColor=4)
