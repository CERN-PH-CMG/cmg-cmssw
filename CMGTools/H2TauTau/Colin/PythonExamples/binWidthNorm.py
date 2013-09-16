from ROOT import kGray, kPink, TH1F, TPaveText, TPad, TCanvas
from numpy import array



def NormalizeToBinWidth(h):
    '''Divides each bin by the bin size'''
    for i in range (1,h.GetNbinsX()+1) :
       h.SetBinContent(i, h.GetBinContent(i) / h.GetBinWidth(i))
       h.SetBinError  (i, h.GetBinError(i)   / h.GetBinWidth(i))

if __name__ == '__main__':
    h = TH1F ('h','h',5,0,10)
    h.SetBinContent (4,10)
    h.SetBinError (4,3)
    h2 = h.Clone ('h2')
    NormalizeToBinWidth (h2)
    c1 = TCanvas ()
    h.Draw()
    h2.Draw('same')

    binning_svfitMass = array([0., 20., 40., 60., 80., 100., 120., 140., 160., 180.,200., 250., 300., 350.])
    h3 = TH1F ('h3','h3',len (binning_svfitMass) - 1, binning_svfitMass)
    h3.SetBinContent (3,50)
    h3.SetBinError (3,10)
    h3.SetBinContent (11,50)
    h3.SetBinError (11,10)
    h4 = h3.Clone ('h4')
    NormalizeToBinWidth (h4)
    h3.Draw ()
    h4.Draw ('same')
    
