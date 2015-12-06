#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

import ROOT
if "/EcalEnergyCalibrator_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/MonoXAnalysis/python/tools/EcalEnergyCalibrator.cc+" % os.environ['CMSSW_BASE']);
from ROOT import EcalEnergyCalibrator

class residualCalibratedEcalEnergyFriend:
    def __init__(self):
        print ""
    def listBranches(self):
        fulllist = [ ("nCalibEle","I") ]
        for efloat in "pt energy eta phi mass".split():
            fulllist.append( ("CalibEle"+"_"+efloat,"F",10,"nCalibEle") )
        return fulllist
    def __call__(self,event):
        # prepare output
        ret = {}; eleret = {}
        leps = [l for l in Collection(event,"LepGood","nLepGood")]
        eles = [l for l in leps if abs(l.pdgId)==11]
        ret['nCalibEle'] = len(eles)

        #compute the calibrated energy
        for efloat in "pt energy eta phi mass".split():
            eleret[efloat] = []
        calibrator = EcalEnergyCalibrator( (event.run == 1), False)
        iel=0
        for el in eles:
            for efloat in "eta phi mass".split():
                eleret[efloat].append( getattr(el,efloat) )  
            seed = event.run * event.lumi + event.evt + iel
            calibenergy = calibrator.calibrate(el.correctedEcalEnergy, el.etaSc, el.r9, seed)
            eleret["energy"].append( calibenergy )
            eleret["pt"].append( calibenergy/cosh(el.eta) )
            iel+=1
            
        ### return
        fullret = {}
        for k,v in ret.iteritems():
            fullret[k] = v
        for k,v in eleret.iteritems():
            fullret["CalibEle_%s" % k] = v

        return fullret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVarsMonojet()
        def analyze(self,ev):
            if ev.metNoMu_pt < 200: return True
            print "\nrun %6d lumi %4d event %d: metNoMu %d" % (ev.run, ev.lumi, ev.evt, ev.metNoMu_pt)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)
