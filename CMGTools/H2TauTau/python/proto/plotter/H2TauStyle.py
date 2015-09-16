from ROOT import TColor, kViolet, kBlue

class Style:

    def __init__(self,
                 markerStyle=8,
                 markerColor=1,
                 markerSize=1,
                 lineStyle=1,
                 lineColor=1,
                 lineWidth=2,
                 fillColor=None,
                 fillStyle=1001):
        self.markerStyle = markerStyle
        self.markerColor = markerColor
        self.markerSize = markerSize
        self.lineStyle = lineStyle
        self.lineColor = lineColor
        self.lineWidth = lineWidth
        if fillColor is None:
            self.fillColor = lineColor
        else:
            self.fillColor = fillColor
        self.fillStyle = fillStyle

    def formatHisto(self, hist, title=None):
        hist.SetMarkerStyle(self.markerStyle)
        hist.SetMarkerColor(self.markerColor)
        hist.SetMarkerSize(self.markerSize)
        hist.SetLineStyle(self.lineStyle)
        hist.SetLineColor(self.lineColor)
        hist.SetLineWidth(self.lineWidth)
        hist.SetFillColor(self.fillColor)
        hist.SetFillStyle(self.fillStyle)
        if title != None:
            hist.SetTitle(title)
        return hist

# the following standard files are defined and ready to be used.
# more standard styles can be added on demand.
# user defined styles can be created in the same way in any python module

sBlack = Style()
sData = Style(fillStyle=0, markerSize=1.3)
sBlue = Style(markerColor=4, fillColor=4)
sGreen = Style(markerColor=8, fillColor=8)
sRed = Style(markerColor=2, fillColor=2)
sYellow = Style(lineColor=1, markerColor=5, fillColor=5)
sViolet = Style(lineColor=1, markerColor=kViolet, fillColor=kViolet)

qcdcol = TColor.GetColor(250,202,255)
sHTT_QCD = Style(lineColor=1, markerColor=qcdcol, fillColor=qcdcol)
dycol =  TColor.GetColor(248,206,104)
sHTT_DYJets = Style(lineColor=1, markerColor=dycol, fillColor=dycol)
wcol = TColor.GetColor(222,90,106)
sHTT_WJets = Style(lineColor=1, markerColor=wcol, fillColor=wcol)
ttcol = TColor.GetColor(155,152,204)
sHTT_TTJets = Style(lineColor=1, markerColor=ttcol, fillColor=ttcol)
sHTT_Higgs = Style(lineColor=kBlue, markerColor=2, lineStyle=2, fillColor=0)
zlcol = TColor.GetColor(100,182,232)
sHTT_ZL = Style(lineColor=1, markerColor=zlcol, fillColor=zlcol)
dibosoncol = TColor.GetColor(222,90,106)
sHTT_VV = Style(lineColor=1, markerColor=dibosoncol, fillColor=dibosoncol)

sBlackSquares = Style(markerStyle=21)
sBlueSquares = Style(lineColor=4, markerStyle=21, markerColor=4)
sGreenSquares = Style(lineColor=8, markerStyle=21, markerColor=8)
sRedSquares = Style(lineColor=2, markerStyle=21, markerColor=2)


styleSet = [sBlue, sGreen, sRed, sYellow, sViolet, sBlackSquares, sBlueSquares, sGreenSquares, sRedSquares]
iStyle = 0


def nextStyle():
    global iStyle
    style = styleSet[iStyle]
    iStyle = iStyle+1
    if iStyle >= len(styleSet):
        iStyle = 0
    return style

histPref = {}
histPref['Data'] = {'style':sData, 'layer':2999, 'legend':'Observed'}
histPref['data_*'] = {'style':sBlack, 'layer':2002, 'legend':None}
histPref['Ztt'] = {'style':sHTT_DYJets, 'layer':4, 'legend':'Z#rightarrow#tau#tau'}
histPref['embed_*'] = {'style':sViolet, 'layer':4.1, 'legend':None}
histPref['TTJets*'] = {'style':sHTT_TTJets, 'layer':1, 'legend':'t#bar{t}'} 
histPref['T*tW*'] = {'style':sHTT_TTJets, 'layer':1, 'legend':'Single t'} 
histPref['TTo*'] = {'style':sHTT_TTJets, 'layer':1, 'legend':'Single t'} 
histPref['Single t'] = {'style':sHTT_TTJets, 'layer':1, 'legend':'Single t'} 
histPref['WW*'] = {'style':sHTT_VV, 'layer':0.9, 'legend':'Diboson'} 
histPref['WZ*'] = {'style':sHTT_VV, 'layer':0.8, 'legend':'Diboson'} 
histPref['ZZ*'] = {'style':sHTT_VV, 'layer':0.7, 'legend':'Diboson'} 
histPref['Diboson'] = {'style':sHTT_VV, 'layer':0.7, 'legend':'Diboson'} 
histPref['QCD*'] = {'style':sHTT_QCD, 'layer':2, 'legend':'QCD multijet'}
histPref['WJets*'] = {'style':sHTT_WJets, 'layer':3, 'legend':'W+jets'}  
histPref['Ztt_ZJ'] = {'style':sHTT_ZL, 'layer':3.1, 'legend':'Z#rightarrow#tau#tau/Z#rightarrow ll, j#rightarrow#tau'}
histPref['Ztt_ZL'] = {'style':sHTT_ZL, 'layer':3.2, 'legend':'Z#rightarrow ll'}
histPref['ZLL'] = {'style':sHTT_ZL, 'layer':3.2, 'legend':'Z#rightarrow ll'}
histPref['Ztt_TL'] = {'style':sViolet, 'layer':4.1, 'legend':'Z#rightarrow#tau#tau/Z#rightarrow ll, j#rightarrow#tau'}
histPref['Higgs*'] = {'style':sHTT_Higgs, 'layer':1001, 'legend':None}


