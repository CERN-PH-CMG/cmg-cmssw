import time
import os
from CMGTools.RootTools.PyRoot import *

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST:
            pass
        else: raise


class Comparator(object):

    def __init__(self, info1, info2):
        self.info1 = info1
        self.info2 = info2
        self.legend = None

    def browse(self):
        for h1name, h1 in sorted(self.info1.hists.iteritems()):
            h2 = self.info2.hists.get(h1name, None)
            if h2 is None:
                pass
            # print h1name, 'not in', d2dir.GetName()
            else:
                h1.SetTitle(h1name)
                h2.SetTitle(h1name)
                plotdir = '/'.join(['Plots_Comparator',os.path.dirname(h1name)])
                try:
                    mkdir_p( plotdir )
                except:
                    pass
                self.drawHists(h1, h2)
                pngname = '/'.join(['Plots_Comparator',h1name+'.png'])
                print pngname
                gPad.SaveAs(pngname)
                res = raw_input('')
                

    def drawHists(self, h1, h2):
        print 'Draw', h1.GetName()
        h1.SetMarkerColor(4)
        h2.SetMarkerColor(1)                    
        h1.SetLineColor(4)
        h1.SetFillColor(5)
        h1.SetFillStyle(1001)
        h2.SetLineColor(1)
        maximum = max(h1.GetBinContent(h1.GetMaximumBin()),
                      h2.GetBinContent(h2.GetMaximumBin()))
        print maximum
        h1.Draw('E2')
        h2.Draw('same')
        if self.legend is None:
            self.legend = TLegend(0.5, 0.65, 0.89, 0.8)
            self.legend.AddEntry(h1, self.info1.tfile.GetName(), 'pf')
            self.legend.AddEntry(h2, self.info2.tfile.GetName(), 'p')
        h1.GetYaxis().SetRangeUser(0.001, maximum*1.2)
        self.legend.Draw('same')
        gPad.Update()

        
class FlatFile(object):
    def __init__(self, tfile):
        self.tfile = tfile
        self.hists = {}
#         self.flatten( self.tfile, self.tfile.GetName(), self.hists )
        self.flatten( self.tfile, '.', self.hists )
        
    def flatten(self, dir, mothername, hists):
        for key in dir.GetListOfKeys():
            name = key.GetName()
            absname = '/'.join([mothername, name])
            obj = dir.Get(name)
            if type(obj) in dirTypes:
                self.flatten(obj, absname, self.hists)
            elif type(obj) in histTypes:
                self.hists[absname] = obj


if __name__ == '__main__':
    import sys

    file1 = FlatFile( TFile(sys.argv[1]) )
    file2 = FlatFile( TFile(sys.argv[2]) )
    comparator = Comparator(file1, file2)
    comparator.browse()

