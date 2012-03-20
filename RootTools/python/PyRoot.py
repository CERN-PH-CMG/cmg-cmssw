import os, sys

import CMGTools.RootTools.StartUp 

from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F, TH1F, TH1D , TProfile, TLegend
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

# adding current directory in PYTHONPATH
sys.path.append('.')


from CMGTools.RootTools.Style import *

if __name__ == '__main__':

    import sys
    from optparse import OptionParser
    
    parser = OptionParser()
    parser.usage = '''
    PyRoot.py [root_file]
    '''
    
    (options,args) = parser.parse_args()

    file = None
    if len(args)>1:
        parser.print_help()
        print
        print 'Maximum one argument (a root file)'
        sys.exit(1)
    elif len(args)==1:
        file = TFile( args[0] )
        

    
