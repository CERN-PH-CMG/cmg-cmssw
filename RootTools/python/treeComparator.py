from CMGTools.RootTools.PyRoot import * 
from CMGTools.RootTools.Style import * 
from CMGTools.RootTools.HistComparator import * 

num = 0
def hname():
    global num
    num+=1
    return 'h_{num}'.format(num=num)

legend = None

def draw(var1, cut, t1, t2, w1='1', w2='1',
         name1=None, name2=None,
         normalize=True, nbins=20, xmin=0, xmax=200, var2=None):
    print var1
    print var2
    print w1
    print w2
    print cut
    if var2 is None:
        var2 = var1
    global legend
    h1 = TH1F(hname(), '', nbins, xmin, xmax)
    h1.Sumw2()
    t1.Project(h1.GetName(), var1,'({cut})*({w1})'.format(cut=cut,w1=w1),'')
    h2 = h1.Clone(hname())
    h2.Sumw2()
    t2.Project(h2.GetName(), var2,'({cut})*({w2})'.format(cut=cut,w2=w2),'')
    if normalize:
        h1.Scale(1./h1.Integral())
        h2.Scale(1./h2.Integral())
    else:
        pass
        # h2.Scale(h1.Integral()/h2.Integral())
    sBlue.markerStyle = 25
    sBlue.formatHisto(h2)
    sBlack.formatHisto(h1)
    h2.SetFillStyle(1001)
    h2.SetFillColor(5)
    h1.SetMarkerSize(0.8)
    h2.SetMarkerSize(0.8)
    h1.SetStats(0)
    h2.SetStats(0)
    if name1 is None: name1 = t1.GetTitle()
    if name2 is None: name2 = t2.GetTitle()
    h1.SetTitle(name1)
    h2.SetTitle(name2)
    legend = TLegend(0.55,0.7,0.88,0.88)
    legend.SetFillColor(0)
    legend.AddEntry(h1, name1, 'lp')
    legend.AddEntry(h2, name2, 'lpf')

    comparator = HistComparator(var1, h1, h2)
    return comparator


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
    trees = getTrees(None, patterns)
    pprint.pprint(trees)
    comp = draw('svfitMass', '1', trees[a1], trees[a2],
                name1 = a1, name2 = a2,
                graphics=False)
    comp.draw()
