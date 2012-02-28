
import sys
import copy
from CMGTools.RootTools.RootInit import *
from CMGTools.RootTools.Style import sBlack, sBlue

from ROOT import TGraphAsymmErrors

args = sys.argv[1:]
# fileName = args[0]


def printHist(hist):
    print hist.GetNbinsX(), hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax()

class Efficiency(object):
    def __init__(self, region, file, legend='', rebin=None):
        self.dir_num = file.Get( '_'.join([region, 'Num']) ) 
        self.dir_denom = file.Get( '_'.join([region, 'Denom']) ) 
        # self.eff = self.num.Clone('eff')
        # self.eff.Divide(self.denom)
        sname = file.GetName()
        sname = sname.replace('EfficiencyAnalyzer/EfficiencyAnalyzer.root', '')
        self.desc = ', '.join([sname, region, legend])
        rebin = int(rebin)
        def load( dir, hists):
            for key in dir.GetListOfKeys():
                keyname = key.GetName()
                histname = keyname.split('_')[-1]
                hist = dir.Get( keyname )
                if rebin is not None:
                    hist.Rebin( rebin )
                hist.Sumw2()
                hists[histname] = hist

        self.hists_num = {}
        load(self.dir_num, self.hists_num)
        self.hists_denom = {}
        load(self.dir_denom, self.hists_denom)
        self.hists_eff = {}
        for histName, num in self.hists_num.iteritems():
            denom = self.hists_denom[histName]
            # eff = TGraphAsymmErrors( num.GetNbinsX() )
            eff = num.Clone( '_'.join([histName,'eff']) )
            #printHist(num)
            #printHist(denom)
            eff.Divide(num, denom)
            self.hists_eff[histName] = eff
        self.support = {}

    def formatHistos(self, style):
        map( style.formatHisto, self.hists_eff.values() )
        map( style.formatHisto, self.hists_num.values() )
        map( style.formatHisto, self.hists_denom.values() )

    def draw(self,name, ymin=0, ymax=1.1, same=False):
        if not same:
            h = self.hists_num[name]
            sup = TH2F( name, '',
                        h.GetNbinsX(),
                        h.GetXaxis().GetXmin(),
                        h.GetXaxis().GetXmax(),
                        10, ymin, ymax)
            self.support[name] = sup
            sup.SetStats(0)
            sup.SetTitle( h.GetTitle() )
            sup.SetXTitle( h.GetXaxis().GetTitle() )
            sup.SetYTitle( h.GetYaxis().GetTitle() )
            sup.Draw()
        self.hists_eff[name].Draw('Psame')


import sys
from optparse import OptionParser

parser = OptionParser()
parser.usage = '%prog <region> <var> <dir1> [dir2 ..]'

parser.add_option("-r", "--rebin",
                  dest="rebin",
                  default=None,help='rebin factor for your histograms')

options, args = parser.parse_args()
if len(args)<3:
    print 'provide at least 3 arguments'
    sys.exit(1)

region = args[0]
var = args[1]
files = args[2:]

styles = [ sBlue, sBlack ]
sBlack.markerStyle = 25

keeper = []

def setup( fileName, index ):
    print 'setup', fileName
    ffileName = '/'.join( [fileName, 'EfficiencyAnalyzer/EfficiencyAnalyzer.root'] )
    file = TFile( ffileName)
    eff = Efficiency( region, file, '', options.rebin)
    eff.formatHistos( styles[index] )
    keeper.extend( [file] )
    return eff

effs = []
for index, file in enumerate(files):
    effs.append( setup( file, index ) )

    
def draw(name, ymin=0, ymax=1.1):
    same = False
    legend = TLegend(0.6, 0.15, 0.89, 0.35)
    keeper.append( legend )
    for eff in effs:
        eff.draw( name, ymin, ymax, same)
        if not same:
            same = True
        legend.AddEntry(eff.hists_eff[name], eff.desc, 'lp' )
    legend.Draw('same')

def drawNum(name, norm=True ):
    same = ''
    legend = TLegend(0.6, 0.15, 0.89, 0.35)
    keeper.append( legend )
    for eff in effs:
        if norm:
            eff.hists_num[name].DrawNormalized( same )
        else:
            eff.hists_num[name].Draw( same )
        if same=='':
            same = 'same'
        legend.AddEntry(eff.hists_eff[name], eff.desc, 'lp' )
    legend.Draw('same')
        
    

draw(var)
