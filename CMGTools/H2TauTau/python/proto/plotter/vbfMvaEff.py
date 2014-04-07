from ROOT import TH1F
from CMGTools.RootTools.Style import sBlue, sRed

keeper = []

def distribution(tree, name, nbins=10, min=-0.7, max=1):
    
    h = TH1F(name, ';VBF mva', nbins, min, max)
    h.Sumw2()
    tree.Project(name, 'VBF_mva', '(VBF_mva>-0.7)*weight')
    h.Scale(1/h.Integral())
    if name is 'WJets':
        sBlue.formatHisto(h)
    elif name is 'zdata':
        sRed.formatHisto(h)   
    return h 

