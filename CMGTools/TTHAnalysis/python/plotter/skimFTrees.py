import ROOT
import os, sys

# usage: python skimFTrees.py BIGTREE_DIR FTREE_DIR outdir DATASET_NAME ...

dsets = sys.argv[4:]
if len(sys.argv)<5:
    dsets = [d.replace('evVarFriend_','').replace('.root','') for d in os.listdir(sys.argv[2]) if 'evVarFriend' in d]
dsets = [d for d in dsets if d in os.listdir(sys.argv[1])]

fname = [x for x in sys.argv[2].split('/') if x!=''][-1]

for dset in dsets:
    print dset,
    fsel = ROOT.TFile.Open(sys.argv[1]+'/'+dset+'/selection_eventlist.root')
    elist = fsel.elist
    f_f = ROOT.TFile.Open(sys.argv[2]+'/evVarFriend_'+dset+'.root')
    t_f = f_f.Get("sf/t")
    t_f.SetEventList(elist)
    os.system('mkdir -p %s/%s'%(sys.argv[3],fname))
    f2 = ROOT.TFile('%s/%s/evVarFriend_%s.root'%(sys.argv[3],fname,dset),'recreate')
    f2.cd()
    f2.mkdir('sf')
    f2.cd('sf')
    t2 = t_f.CopyTree('1')
    f2.Write()
    print ': skimmed friend trees put in %s'%f2.GetName()
    f2.Close()
    f_f.Close()
    fsel.Close()
