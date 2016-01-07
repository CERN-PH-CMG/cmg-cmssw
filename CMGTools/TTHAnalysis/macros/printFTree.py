import ROOT
import sys

for myfile in sys.argv[1:]:
    f = ROOT.TFile.Open(myfile)
    t = f.Get("sf/t")
    print myfile+': '+str(t.GetEntries())
    f.Close()
