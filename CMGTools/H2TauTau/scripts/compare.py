import sys
import numpy as np

import ROOT
from optparse import OptionParser

ROOT.gROOT.SetBatch(True)

colours = [1, 2, 3, 6, 8]
styles = [2, 1, 3, 4, 5]

def findTree(f):
    for key in f.GetListOfKeys():
        tree = f.Get(key.GetName())
        if isinstance(tree, ROOT.TTree):
            return tree
    print 'Failed to find a TTree in file', f
    return None

def applyHistStyle(h, i):
    h.SetLineColor(colours[i])
    h.SetLineStyle(styles[i])
    h.SetLineWidth(3)
    h.SetStats(False)


def comparisonPlots(u_names, trees, titles, pname='sync.pdf'):

    c = ROOT.TCanvas()
    c.Print(pname+'[')

    for branch in u_names:
        min_x = min(t.GetMinimum(branch) for t in trees)
        max_x = max(t.GetMaximum(branch) for t in trees)

        if min_x == max_x:
            continue

        if min_x < -9000 and max_x < 5000.:
            min_x = - max_x

        min_x = min(0., min_x)

        hists = []
        for i, t in enumerate(trees):
            h_name = branch+t.GetName()
            h = ROOT.TH1F(h_name, branch, 50, min_x, max_x + (max_x - min_x) * 0.01)
            applyHistStyle(h, i)
            t.Project(h_name, branch, '1') # Should introduce weight...
            hists.append(h)
        
        ymax = max(h.GetMaximum() for h in hists)

        leg = ROOT.TLegend(0.15,0.79,0.5,0.89)
        leg.SetFillColor(0)
        leg.SetFillStyle(0)
        leg.SetLineColor(0)

        for i, h in enumerate(hists):
            title = titles[i]
            h.GetYaxis().SetRangeUser(0., ymax * 1.2)
            leg.AddEntry(h, title + ': ' + str(h.Integral()))
            if i == 0:
                h.Draw('HIST E')
            else:
                h.Draw('SAME HIST E')

        leg.Draw()

        c.Print(pname)

    c.Print(pname+']')

def interSect(tree1, tree2, var='evt', common=False, save=False,  titles=[]):
    # run, lumi, evt
    tlist1 = ROOT.TEntryList()
    tlist2 = ROOT.TEntryList()
    
    tree1.Draw(var)
    r_evt1 = tree1.GetV1()
    evt1 = np.array([r_evt1[i] for i in xrange(tree1.GetEntries())], dtype=int)

    tree2.Draw(var)
    r_evt2 = tree2.GetV1()
    evt2 = np.array([r_evt2[i] for i in xrange(tree2.GetEntries())], dtype=int)

    if common:
        indices1 = np.nonzero(np.in1d(evt1, evt2))
        indices2 = np.nonzero(np.in1d(evt2, evt1))
    else:
        indices1 = np.nonzero(np.in1d(evt1, evt2) == 0)
        indices2 = np.nonzero(np.in1d(evt2, evt1) == 0)

    if save:
        if len(titles) < 2:
            titles = ['tree1', 'tree2']

        evt1[indices1].tofile(titles[0]+'.csv', sep=',', format='%d')
        evt2[indices2].tofile(titles[1]+'.csv', sep=',', format='%d')

    for ind1 in indices1[0]:
        tlist1.Enter(ind1)

    for ind2 in indices2[0]:
        tlist2.Enter(ind2)

    return tlist1, tlist2

if __name__ == '__main__':
        
    parser = OptionParser()

    parser.add_option('-t', '--titles', type='string', dest='titles', default='Imperial,KIT', help='Comma-separated list of titles for the N input files (e.g. Imperial,KIT)')
    parser.add_option('-i', '--no-intersection', dest='do_intersect', action='store_false', default=True, help='Do not produce plots for events not in common')
    parser.add_option('-c', '--no-common', dest='do_common', action='store_false', default=True, help='Do not produce plots for events in common')

    (options,args) = parser.parse_args()

    if len(args) < 2:
        print 'provide at least 2 input root files'
        sys.exit(1)

    titles = options.titles.split(',')
    
    if len(titles) < len(args):
        print 'Provide at least as many titles as input files'
        sys.exit(1)

    tfiles = [ROOT.TFile(arg) for arg in args]

    trees = [findTree(f) for f in tfiles]

    # find all branches that exist in all files

    b_names = [set([b.GetName() for b in t.GetListOfBranches()]) for t in trees]

    u_names = set.intersection(*b_names)

    print 'Making plots for all common branches', u_names

    comparisonPlots(u_names, trees, titles)

    if len(trees) == 2 and options.do_intersect:
        intersect = interSect(trees[0], trees[1], save=True, titles=titles)
        trees[0].SetEntryList(intersect[0])
        trees[1].SetEntryList(intersect[1])
        comparisonPlots(u_names, trees, titles, 'intersect.pdf')


    if len(trees) == 2:
        intersect = interSect(trees[0], trees[1], common=True)
        trees[0].SetEntryList(intersect[0])
        trees[1].SetEntryList(intersect[1])
        comparisonPlots(u_names, trees, titles, 'common.pdf')


