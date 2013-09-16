import os
from ROOT import TFile, TF1

class TurnOnCurve( object ):
    
    def __init__(self, rootfilename=None, tf1name=None):
        if rootfilename is None:
            rootfilename = '/'.join([os.environ['CMSSW_BASE'],
                                     'src/CMGTools/H2TauTau/data/llrCorrections.root'])
        if tf1name is None:
            tf1name = 'turnOnTauMuTauAll'
        self.rootfile = TFile( rootfilename )
        self.tf1 = self.rootfile.Get( tf1name )
        
    def weight(self, pt):
        return self.tf1.Eval( pt )


if __name__ == '__main__':
    import time
    toc = TurnOnCurve()
    print 17, toc.weight( 17 )
    print 18, toc.weight( 18 )
    print 19, toc.weight( 19 )
    print 20, toc.weight( 20 )
    print 100, toc.weight( 100 )
    toc.tf1.Draw()
    time.sleep(5)
