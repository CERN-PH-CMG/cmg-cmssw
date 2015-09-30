#! /usr/bin/env python

import os
from array import array
from ROOT import TFile, TTree, TH1

fnames = ['ZJetsToNuNu', 'DYJetsToLL']
prod_version = 'Prod_v03/'
heppy_output_dir = '/lustre/cmsdata/DM/ntuples/'+prod_version
tnames = ['SR', 'ZCR', 'WCR', 'TCR', 'GCR']

for fname in fnames:
    oldfile = TFile((heppy_output_dir+fname+".root"), "READ")
    print "In file", fname, ":"
    oldtrees = []
    for tname in tnames:
        oldtrees.append(oldfile.Get(tname))
        print " - Tree", tname, "has", oldtrees[-1].GetEntriesFast(), "events"
    newfile = []
    for f in range(0, 2+1):
        newfile.append( TFile((heppy_output_dir+fname+"_%db.root" % f), "RECREATE") )
        for oldtree in oldtrees:
            newtree = oldtree.CopyTree("nBJets"+("=" if f<2 else ">")+"=%d" % f);
            print " - - New tree", oldtree.GetName(), f, "b has", newtree.GetEntriesFast(), "events"
            newtree.AutoSave()
        newfile[-1].Close()


