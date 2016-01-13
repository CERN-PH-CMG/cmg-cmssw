import ROOT
import sys

# this script returns the number of entries in a friend tree file

for myfile in sys.argv[1:]:
    f = ROOT.TFile.Open(myfile)
    t = f.Get("sf/t")
    print(myfile+': '),
    print t.GetEntries()
    f.Close()
