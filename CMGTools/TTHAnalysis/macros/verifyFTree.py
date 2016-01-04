import ROOT
import os, sys

# usage: python verifyFTree BIGTREE_DIR FTREE_DIR DATASET_NAME ...

dsets = sys.argv[3:]
if len(sys.argv)<4:
    dsets = [d.replace('evVarFriend_','').replace('.root','') for d in os.listdir(sys.argv[2]) if 'evVarFriend' in d]

def openRootOrUrl(myfile):
    _f_t = None
    if os.path.exists(myfile):
        _f_t = ROOT.TFile.Open(myfile)
    elif os.path.exists(myfile+'.url'):
        with open(myfile+'.url','r') as urlf:
            myfile = urlf.readline().replace('\n','')
            _f_t = ROOT.TXNetFile(myfile)
    return _f_t

for dset in dsets:
    f_t = openRootOrUrl(sys.argv[1]+'/'+dset+'/treeProducerSusyMultilepton/tree.root')
    t_t = f_t.Get("tree")
    n_t = t_t.GetEntries()
    f_t.Close()
    f_f = openRootOrUrl(sys.argv[2]+'/evVarFriend_'+dset+'.root')
    t_f = f_f.Get("sf/t")
    n_f = t_f.GetEntries()
    f_f.Close()
    print '%s: %d - %d : %s'%(dset,n_t,n_f,'OK' if n_t==n_f else 'ERROR '*15+' !!!')
