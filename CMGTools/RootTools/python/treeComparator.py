from CMGTools.RootTools.PyRoot import * 
from CMGTools.RootTools.Style import * 

num = 0
def hname():
    global num
    num+=1
    return 'h_{num}'.format(num=num)

legend = None

def draw(var, cut, t1, t2, w1='1', w2='1',
         name1=None, name2=None,
         normalize=True, nbins=20, graphics=True):
    global legend
    h1 = TH1F(hname(), '', nbins, 0, 200)
    h1.Sumw2()
    t1.Project(h1.GetName(), var,'({cut})*({w1})'.format(cut=cut,w1=w1),'')
    h2 = h1.Clone(hname())
    h2.Sumw2()
    t2.Project(h2.GetName(), var,'({cut})*({w2})'.format(cut=cut,w2=w2),'')
    if normalize:
        h1.Scale(1./h1.Integral())
        h2.Scale(1./h2.Integral())
    else:
        h2.Scale(h1.Integral()/h2.Integral())
    sBlue.formatHisto(h1)
    sBlack.formatHisto(h2)
    h1.SetMarkerSize(0.8)
    h2.SetMarkerSize(0.8)
    h1.SetStats(0)
    h2.SetStats(0)
    if name1 is None: name1 = t1.GetTitle()
    if name2 is None: name2 = t2.GetTitle()
    legend = TLegend(0.55,0.7,0.88,0.88)
    legend.SetFillColor(0)
    legend.AddEntry(h1, name1, 'lp')
    legend.AddEntry(h2, name2, 'lp')
    if graphics:
        h2.Draw()
        h1.Draw('same')
        legend.Draw('same')
        gPad.Update()
    return h1, h2, legend

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
