from CMGTools.RootTools.treeComparator import *
from CMGTools.H2TauTau.proto.plotter.categories_TauMu import * 
from CMGTools.RootTools.ReWeighter import ReWeighter

if __name__ == '__main__':
    import sys
    import pprint
    import shelve
    
    p1 = sys.argv[1]
    p2 = sys.argv[2]
    a1 = p1.split('/')[0]
    a2 = p2.split('/')[0]
    patterns = [ (a1, p1), (a2, p2) ]
    trees = getTrees('H2TauTauTreeProducerTauMu', patterns)
    pprint.pprint(trees)
    trees[a1].SetTitle( a1 )
    trees[a2].SetTitle( a2 )

    cut = cat_Inc.replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.7')

    if 0:
        h1, h2 = draw('mt', cut, trees[a1], trees[a2])
        mtrw = ReWeighter('mt', h1, h2)
        weightFile = shelve.open('weights.txt')
        weightFile['weights'] = mtrw
        weightFile.close()
    else:
        weightFile = shelve.open('weights.txt')
        mtrw = weightFile['weights']
        wei = mtrw.weightStr('mt')
        h1, h2 = draw('mt', cut,  
                      trees[a1], trees[a2],
                      w1=wei, w2=wei, normalize=False)
        
    
    
    
    # h1, h2 = draw('mt',cat_Inc, trees['wj'], trees['dy'])
