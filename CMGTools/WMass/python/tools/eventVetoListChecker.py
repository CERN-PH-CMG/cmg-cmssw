#!/usr/bin/env python
import array
import ROOT
import os.path

class EventVetoListChecker:
    _store={}
    def __init__(self,path,vetoes):
        self.pathtofiles = path
        self.vetoes = vetoes
        self.name = ''
    def initDataset(self,dataset):
        self.dataset = dataset.strip().split('_')[0] if "Run20" in dataset else dataset
        for veto in self.vetoes: 
            fname = self.pathtofiles+'/'+self.dataset+'_'+veto+'.txt'
            if os.path.isfile(fname): 
                self.loadFile(veto,fname)
                print 'Initialized CheckEventVetoList from %s'%fname
            self.name = veto
    def loadFile(self,veto,fname):
        with open(fname, 'r') as f:
            for line in f:
                if len(line.strip()) == 0 or line.strip()[0] == '#': continue
                run,lumi,evt = line.strip().split(':')
                self.addEvent(veto,int(run),int(lumi),long(evt))
    def addEvent(self,veto,run,lumi,evt):
        if (veto,run,lumi) not in self._store:
            self._store[(veto,run,lumi)]=array.array('L')
        self._store[(veto,run,lumi)].append(evt)
    def filter(self,veto,run,lumi,evt):
        mylist=self._store.get((veto,run,lumi),None)
        return ((not mylist) or (long(evt) not in mylist))
    def listBranches(self):
        self.branches = []
        for veto in self.vetoes:
            self.branches.append( (veto, 'I') )
        return self.branches[:]
    def __call__(self,event):
        run = int(event.run)
        lumi = int(event.lumi)
        evt = long(event.evt)
        ret = {}
        for veto in self.vetoes:
            ret[veto] = 1 if self.filter(veto,run,lumi,evt) else 0
        return ret
