
from ROOT import TH1, TDirectory, TCanvas

from CMGTools.RootTools.Style import *

import sys, re, math

class RootDir:
    
    def __init__( self, tDir, style=None):
        self.tDir_ = tDir
        self.histograms_ = {}
        self.subDirs_ = {}
        self.style_ = style
        self.walk()
        
    def loadHistograms( self, tDir ):
        listOfKeys = tDir.GetListOfKeys()
        for key in listOfKeys:
            hist = tDir.Get( key.GetName() )
                
            if hist.InheritsFrom('TH1'):
                # print hist.GetName()
                
                if self.style_ != None:
                    hist = self.style_.formatHisto( hist )
                self.histograms_[key.GetName()] = hist 

    def setStyle( self, style ):
        self.style_ = style
        for key in self.histograms_.iterkeys():
            self.histograms_[key] = style.formatHisto( self.histograms_[key] )
        for key in self.subDirs_.iterkeys():
            self.subDirs_[key].setStyle( style )
            
    def walk( self ):        

        file = self.tDir_
        # print 'file : ', file.GetName()
    
        self.loadHistograms( self.tDir_ )
        # pattern = re.compile(regexp)
    
        listOfKeys = file.GetListOfKeys()
        for key in listOfKeys:
            # print key
            keyname = key.GetName()
            # match = pattern.search( keyname )
            # if match != None:
            subdir = file.GetDirectory( keyname )
            if subdir != None:
                rootDir = RootDir( subdir, self.style_ )
                rootDir.walk()
                self.subDirs_[subdir.GetName()] = rootDir

    def Draw( self ):
        nPlots = len(self.histograms_)

        if nPlots:
            self.canvas_ = TCanvas(self.tDir_.GetName(),self.tDir_.GetName(), 1000, 1000)

            ny = int(math.sqrt (nPlots) )
            nx = int(math.ceil( nPlots / float(ny) ))

            print nPlots, ny, nx
        
            self.canvas_.Divide(nx, ny)
            i = 1
            for key in self.histograms_.iterkeys():
                self.canvas_.cd(i)
                self.histograms_[key].Draw()
                i = i+1

            self.canvas_.Modified()
            self.canvas_.Update()
        
        for key in self.subDirs_.iterkeys():
            self.subDirs_[key].Draw()


if __name__ == '__main__':
    import os, sys
    from ROOT import gROOT, TFile,gPad
    gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )
    file = TFile( sys.argv[1] )
    rootDir = RootDir( file, sRedPoints )
    rootDir.subDirs_['cmgMuonHistograms'].Draw()
