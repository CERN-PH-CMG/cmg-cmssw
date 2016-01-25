import ROOT
import os, sys

# this script returns the fraction of events that pass a cut in a tree + friend tree
#
# usage: python fractionCutTree.py BIGTREE_DIR FTREE_DIR CUT_STRING DATASET_NAME ...

dsets = sys.argv[4:]
if len(sys.argv)<5:
    dsets = [d.replace('evVarFriend_','').replace('.root','') for d in os.listdir(sys.argv[2]) if 'evVarFriend' in d]

for dset in dsets:
    f_t = ROOT.TFile.Open(sys.argv[1]+'/'+dset+'/treeProducerSusyMultilepton/tree.root')
    t_t = f_t.Get("tree")
    f_f = ROOT.TFile.Open(sys.argv[2]+'/evVarFriend_'+dset+'.root')
    t_f = f_f.Get("sf/t")
    t_t.AddFriend(t_f)
    n_pass = t_t.GetEntries(sys.argv[3])
    n_tot = t_t.GetEntries()
    f_t.Close()
    f_f.Close()
    if n_pass!=0:    print '%s: %d out of %d : %.2f%%'%(dset,n_pass,n_tot,float(n_pass)/n_tot*100 if n_tot!=0 else 0)
