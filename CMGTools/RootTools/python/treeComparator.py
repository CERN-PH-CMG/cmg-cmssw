from CMGTools.RootTools.PyRoot import * 

num = 0
def hname():
    global num
    num+=1
    return 'h_{num}'.format(num=num)

def draw(var, cut, t1, t2, w1='1', w2='1'):
    h1 = TH1F(hname(), '1', 10, 0, 200)
    h1.Sumw2()
    t1.Project(h1.GetName(), var,'({cut})*({w1})'.format(cut=cut,w1=w1),'')
    h1.Scale(1./h1.Integral())
    h2 = h1.Clone(hname())
    h2.Sumw2()
    t2.Project(h2.GetName(), var,'({cut})*({w2})'.format(cut=cut,w2=w2),'')
    h2.Scale(1./h2.Integral())
    h2.Draw()
    h1.SetLineColor(4)
    h1.Draw('same')
    gPad.Update()
    return h1, h2

# hw, hdy = draw('diTau.obj.mTLeg2()', 'diTau.obj.mTLeg2()<300')

def getTrees( treeName, patterns ):
    trees = dict()
    for alias, pattern in patterns:
        print 'loading', alias, treeName, pattern
        tree = Chain(treeName, pattern)
        trees[alias] = tree
        # tree.SetWeight(1./tree.GetEntries(), 'global')
    return trees

if __name__ == '__main__':
    import sys
    import pprint
    
    a1,p1 = sys.argv[1].split(':')
    a2,p2 = sys.argv[2].split(':')
    patterns = [ (a1, p1), (a2, p2) ]
    trees = getTrees('H2TauTauTreeProducerTauMu', patterns)
    pprint.pprint(trees)
