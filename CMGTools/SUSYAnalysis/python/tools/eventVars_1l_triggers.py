from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2, std
import ROOT
import time
import itertools
import PhysicsTools.Heppy.loadlibs
import array
import operator

class EventVars1L_triggers:
    def __init__(self):
        self.branches = [
            'HLT_HT350', 'HLT_HT600', 'HLT_HT900', 'HLT_MET170','HLT_HTMET', 'HLT_Had',
            'HLT_SingleMu', 'HLT_Mu45NoIso', 'HLT_Mu50NoIso', 'HLT_MuHad',
            'HLT_MuHT600', 'HLT_MuHT400MET70','HLT_MuMET120', 'HLT_MuHT400B',
            'HLT_SingleEl', 'HLT_ElNoIso', 'HLT_ElHad',
            'HLT_EleHT600','HLT_EleHT400MET70','HLT_EleHT200', 'HLT_ElHT400B'
            ]

    def listBranches(self):
        return self.branches[:]

    def __call__(self,event,base):

        # output dict:
        ret = {}

        '''
        ret['HLT_HT350'] = event.HLT_HT350
        ret['HLT_HT600'] = event.HLT_HT600
        ret['HLT_HT900'] = event.HLT_HT900
        ret['HLT_MET170'] = event.HLT_MET170
        ret['HLT_HTMET'] = event.HLT_HTMET
        ret['HLT_Had'] = event.HLT_Had

        ret['HLT_SingleMu'] = event.HLT_SingleMu
        ret['HLT_MuNoIso'] = event.HLT_MuNoIso
        ret['HLT_MuHad'] = event.HLT_MuHad

        ret['HLT_MuHT600'] = event.HLT_MuHT600
        ret['HLT_MuMET120'] = event.HLT_MuMET120
        ret['HLT_MuHT400MET70'] = event.HLT_MuHT400MET70
        ret['HLT_MuHT400B'] = event.HLT_MuHT400B

        ret['HLT_SingleEl'] = event.HLT_SingleEl
        ret['HLT_ElNoIso'] = event.HLT_ElNoIso
        ret['HLT_ElHad'] = event.HLT_ElHad

        ret['HLT_EleHT200'] = event.HLT_EleHT200
        ret['HLT_EleHT400MET70'] = event.HLT_EleHT400MET70
        ret['HLT_EleHT600'] = event.HLT_EleHT600
        ret['HLT_ElHT400B'] = event.HLT_ElHT400B
        '''

        ## replaces ^^^ list above (and saver as well)
        ## check that any HLT branch exists in tree
        if not (hasattr(event,'HLT_SingleMu') or  hasattr(event,'HLT_SingleEl')):
            #print 'Has no trigger info!'
            return ret

        ## loop over all HLT names and set them in tree
        for var in self.branches:
            #print var, getattr(event,var)

            if 'HLT_' in var:
                ret[var] = getattr(event,var)

        # return branches
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars1L()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)
