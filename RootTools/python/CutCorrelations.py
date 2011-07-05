from ROOT import TH2F

import time

from CMGTools.RootTools.TreeFunctions import * 

class CutCorrelations:
    def __init__(self, name):
        self.cuts = {}
        self.name = name
    def reset(self):
        self.cuts = {}
        self.histo = None
    def addCut( self, cut ):
        # cut is a dictionary
        # adding to the cuts dictionary
        self.cuts = dict( self.cuts.items() + cut.items() )
    def printCuts(self):
        for k, v in self.cuts.iteritems():
            print k, '-->', v
    def bookCorrelationHisto( self ):
        nbins = len(self.cuts)
        hname = 'correlations_'+self.name
        self.histo = TH2F( hname, hname, nbins, 0, nbins, nbins, 0, nbins)
        self.histo.SetStats(0)
        i=0
        for key, v in self.cuts.iteritems():
            i = i+1
            self.histo.GetYaxis().SetBinLabel(i, key)
            self.histo.GetXaxis().SetBinLabel(i, key)
            
        # self.histo.Draw()
        # time.sleep(4)
    def fillCorrelationHisto( self, tree, addCut='1'):
        if addCut != '1':
            setEventList( tree, addCut )
        self.bookCorrelationHisto()
        i1 = 0
        for key1, v1 in self.cuts.iteritems():
            i1 = i1+1
            i2 = 0
            for key2, v2 in self.cuts.iteritems():
                i2 = i2+1
                cut = v1 + ' && ' + v2
                print i1, i2
                n = tree.Draw('1', cut , 'goff')
                self.histo.SetBinContent(i1, i2, n)
        if addCut != '1':
            tree.SetEventList(0)
    def draw( self ):
        if self.histo == None:
            self.histo.Draw('boxtext')
        else:
            print 'call bookCorrelationHisto or fillCorrelationHisto first'
