import ROOT
import sys

# this script returns the number of entries in a tree file

for myfile in sys.argv[1:]:
    f = ROOT.TFile.Open(myfile)
    t = f.Get("tree")
    print myfile+': '+str(t.GetEntries())
    f.Close()
