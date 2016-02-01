import sys
import ROOT

# this script can be used to count how many entries in a list of tree or friend tree files satisfy a certain requirement
# (useful to look for event numbers)

for myf in sys.argv[2:]:
    f = ROOT.TFile(myf)
    t = f.Get("tree")
    if not t: t = f.Get("sf/t")
    if not t: raise RuntimeError, "Tree not found"
    n = t.GetEntries(sys.argv[1])
    if n>0: print n,'entries in',myf
    f.Close()
