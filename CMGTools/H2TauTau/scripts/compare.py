import sys

import ROOT
from optparse import OptionParser

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
    # h.SetTextSize(0.055)
    # h.SetTextFont(42)
    h.SetLineColor(colours[i])
    h.SetLineStyle(styles[i])
    h.SetLineWidth(3)

parser = OptionParser()
(options,args) = parser.parse_args()

if len(args) < 2:
    print 'provide at least input root file'
    sys.exit(1)

tfiles = [ROOT.TFile(arg) for arg in args]

trees = [findTree(f) for f in tfiles]

# find all branches that exist in all files

b_names = [set([b.GetName() for b in t.GetListOfBranches()]) for t in trees]

u_names = set.intersection(*b_names)

print 'Making plots for all branches', u_names

c = ROOT.TCanvas()
c.Print('sync.pdf[')

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

    for i, h in enumerate(hists):
        h.GetYaxis().SetRangeUser(0., ymax * 1.1)
        if i == 0:
            h.Draw()
        else:
            h.Draw('SAME')

    c.Print('sync.pdf')


c.Print('sync.pdf]')



