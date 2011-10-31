
from ROOT import TH1, TDirectory, TCanvas

from CMGTools.RootTools.Style import *

import sys, re, math, pprint, string

class RootDir:
    """Manages a TDirectory in PyROOT, and allows easy access to the histograms and plotting"""
    
    def __init__( self, tDir, style=None):
        self.tDir_ = tDir
        self.histograms_ = {}
        self.subDirs_ = {}
        self.style_ = style
        self.walk()
        
    def loadHistograms( self, tDir ):
        """Looks for all histograms in the directory, and stores them in a dictionary indexed by their key"""
        listOfKeys = tDir.GetListOfKeys()
        for key in listOfKeys:
            hist = tDir.Get( key.GetName() )
                
            if hist.InheritsFrom('TH1'):
                # print hist.GetName()
                
                if self.style_ != None:
                    hist = self.style_.formatHisto( hist )
                self.histograms_[key.GetName()] = hist 

    def setStyle( self, style ):
        """Set style for all histograms, in this directory and its sub-directories. See Style module for more information"""
        self.style_ = style
        for key in self.histograms_.iterkeys():
            self.histograms_[key] = style.formatHisto( self.histograms_[key] )
        for key in self.subDirs_.iterkeys():
            self.subDirs_[key].setStyle( style )
            
    def walk( self ):        
        """loads histograms, create RootDirs for each subdirectory. RootDirs are stored in a dictionary, just like the histograms.""" 

        file = self.tDir_
        # print 'file : ', file.GetName()
    
        self.loadHistograms( self.tDir_ )
        # pattern = re.compile(regexp)
    
        listOfKeys = file.GetListOfKeys()
        for key in listOfKeys:
            keyname = key.GetName()
            subdir = file.GetDirectory( keyname )
            if subdir != None:
                rootDir = RootDir( subdir, self.style_ )
                rootDir.walk()
                self.subDirs_[subdir.GetName()] = rootDir

    def DrawAll( self, xsize=800, ysize=800):
        """Draw all histograms in the RootDir canvas. Note that histograms in a given sub-directory can be drawn by doing: this.subDirs_['theSubDir'].Draw()"""
        
        nPlots = len(self.histograms_)

        if nPlots:
            self.canvas_ = TCanvas(self.tDir_.GetName(),self.tDir_.GetName(), xsize, ysize)

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
            self.subDirs_[key].DrawAll()

    def h( self, histName ):
        """Returns an histogram in this RootDir or in its subdirectories. 
        as histName, give the absolute path, which can be obtained from the
        Print function
        """
        pathList = histName.split('/')
        if len(pathList) == 1:
            histKey = pathList[0]
            hist = self.histograms_[ histKey ]
            if hist!=None:
                return hist
            else:
                return None
        else:
            subDirKey = pathList.pop(0)
            print subDirKey
            subDir = self.subDirs_[subDirKey]
            if subDir!=None:
                return subDir.h( string.join(pathList, '/') )
            else:
                return None

    def Draw( self, histName):
        """Draws an histogram. Use the absolute path."""
        hist = self.h(histName).Draw()

    def Print( self, path=None):
        """Print the contents of this RootDir, including its sub-directories."""
        for key in self.histograms_.iterkeys():
            if path!=None:
                print path+'/'+key
            
        for key in self.subDirs_.iterkeys():
            print key

            subPath = key
            if path!=None:
                subPath = path + '/' + key
            self.subDirs_[key].Print( subPath )
        
        

if __name__ == '__main__':

    import os, sys

    filename = os.environ.get('PYTHONSTARTUP')
    if filename and os.path.isfile(filename):
        exec(open(filename).read())

    from ROOT import gROOT, TFile,gPad
    gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )


    from optparse import OptionParser
    import sys
    
    parser = OptionParser()
    
    parser.usage = '''
    RootDir.py <root file name>
    '''
    parser.add_option("-l", "--list", 
                      dest="list", 
                      help="listing mode",
                      action='store_true',
                      default=False)

    (options,args) = parser.parse_args()

    if len(args) != 1:
        print 'ERROR: please provide an input root file'
        print args
        sys.exit(1)

    fileName = args[0]

    file = TFile( fileName )
    # dir = sys.argv[2]
    rootDir = RootDir( file, sRed )

    if options.list:
        rootDir.Print()
        
    
    # rootDir.subDirs_[dir].DrawAll()
