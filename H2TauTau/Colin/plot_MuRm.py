from CMGTools.RootTools.treeComparator import *
from CMGTools.H2TauTau.proto.plotter.categories_TauMu import * 
from CMGTools.RootTools.ReWeighter import ReWeighter

if __name__ == '__main__':
    import sys
    import pprint
    import shelve
    
    a1,p1 = sys.argv[1].split(':')
    a2,p2 = sys.argv[2].split(':')
    patterns = [ (a1, p1), (a2, p2) ]
    trees = getTrees('H2TauTauTreeProducerTauMu', patterns)
    pprint.pprint(trees)

    # cut = cat_Inc_RlxTauIso + '&& nJets20>0'
    cut = cat_Inc_RlxTauIso

    if 1:
        h1, h2 = draw('mt',cat_Inc, trees['t1'], trees['t2'])
        mtrw = ReWeighter('mt', h1, h2)
        weightFile = shelve.open('weights.txt')
        weightFile['weights'] = mtrw
        weightFile.close()
    else:
        weightFile = shelve.open('weights.txt')
        mtrw = weightFile['weights']
        h1, h2 = draw('mt', '({cut})*({weight})'.format(cut=cut, weight=mtrw.weightStr('mt')),
                      trees['t1'], trees['t2'])
        
    
    
    
    # h1, h2 = draw('mt',cat_Inc, trees['wj'], trees['dy'])
