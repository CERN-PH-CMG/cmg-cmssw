import sys
import ROOT

for myf in sys.argv[2:]:
    f = ROOT.TFile(myf)
    t = f.Get("tree")
    if not t: t = f.Get("sf/t")
    if not t: raise RuntimeError, "Tree not found"
    n = t.GetEntries(sys.argv[1])
    if n>0: print n,'entries in',myf
    f.Close()
