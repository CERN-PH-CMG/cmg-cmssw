import operator 
import itertools
import copy
from math import *

from ROOT import std 
from ROOT import TLorentzVector, TVectorD

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

import ROOT
import os
import re

class ttHhistoCounterAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHhistoCounterAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 
        self.doLHE = getattr(cfg_ana, 'doLHE', True)
        self.cfg_ana = cfg_ana
        self.isInitSMS = False

    def declareHandles(self):
        super(ttHhistoCounterAnalyzer, self).declareHandles()
        self.mchandles['GenInfo'] = AutoHandle( ('generator','',''), 'GenEventInfoProduct' )

    def beginLoop(self, setup):
        super(ttHhistoCounterAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')
        if "outputfile" in setup.services :
            setup.services["outputfile"].file.cd()
            self.inputCounter = ROOT.TH1D("Count","Count",1,0,2)
            if self.cfg_comp.isMC:
                if self.doLHE:
                    self.inputLHE = ROOT.TH1D("CountLHE","CountLHE",20001,-0.5,20000.5)
                self.inputGenWeights = ROOT.TH1D("SumGenWeights","SumGenWeights",1,0,2)


    def initSMS(self,event):
        if self.isInitSMS or (not self.cfg_comp.isMC): raise RuntimeError, 'Trying to initSMS twice or to call it on data!'
        self.maxSMSmass = getattr(self.cfg_ana, 'SMS_max_mass', 3000)
        self.inputCounterSMS = ROOT.TH3D("CountSMS","CountSMS",int(self.maxSMSmass+1),-0.5,self.maxSMSmass+0.5,int(self.maxSMSmass+1),-0.5,self.maxSMSmass+0.5,1,0,2)
#        if self.doLHE: self.inputLHESMS = ROOT.TH3D("CountLHESMS","CountLHESMS",int(self.maxSMSmass+1),-0.5,self.maxSMSmass+0.5,int(self.maxSMSmass+1),-0.5,self.maxSMSmass+0.5,10001,-0.5,10000.5) ### too big!
        self.inputGenWeightsSMS = ROOT.TH3D("SumGenWeightsSMS","SumGenWeightsSMS",int(self.maxSMSmass+1),-0.5,self.maxSMSmass+0.5,int(self.maxSMSmass+1),-0.5,self.maxSMSmass+0.5,1,0,2)        
        self.massfill1 = getattr(self.cfg_ana, 'SMS_mass_1', 'genSusyMScan1')
        self.massfill2 = getattr(self.cfg_ana, 'SMS_mass_2', 'genSusyMScan2')
        self.masses_to_track = [self.massfill1,self.massfill2]
        for mass in getattr(self.cfg_ana, 'SMS_varying_masses', []):
            if mass not in self.masses_to_track: self.masses_to_track.append(mass)
        self.allmasses={}
        self.genMregexp = getattr(self.cfg_ana, 'SMS_regexp_evtGenMass', 'genSusyM.+')
        for name in dir(event):
            if not re.match(self.genMregexp,name): continue
            self.allmasses[name]=-1
        self.susyModel = event.susyModel
        for trkM in self.masses_to_track:
            if trkM not in self.allmasses: raise RuntimeError, 'Trying to track a SUSY SMS gen mass that does not exist in the event: %s'%trkM
        for mass in self.allmasses:
            self.allmasses[mass] = getattr(event,mass)
        self.bypass_trackMass_check = getattr(self.cfg_ana, 'bypass_trackMass_check', False)
        self.isInitSMS=True
        print 'SUSY SMS: the first event looks like this:'
        for mass,val in self.allmasses.iteritems():
            tag = '???'
            if mass == self.massfill1: tag = 'used as first tracked mass'
            elif mass == self.massfill2: tag = 'used as second tracked mass'
            elif mass in self.masses_to_track: tag = 'allowed to change between different events'
            else: tag = 'not allowed to change between different events unless bypass_trackMass_check is set to True'
            print '%s = %.1f, %s'%(mass,val,tag)

    def process(self, event):
        self.readCollections( event.input )
        self.inputCounter.Fill(1)

        isSMS = self.cfg_comp.isMC and getattr(event,'susyModel',False)

        if isSMS:

            if not self.isInitSMS: self.initSMS(event)

            if event.susyModel!=self.susyModel: raise RuntimeError, 'The SMS model changed in the middle of the run, from %s to %s!'%(self.susyModel,event.susyModel)
            if not self.bypass_trackMass_check:
                for mass,val in self.allmasses.iteritems():
                    if mass in self.masses_to_track: continue
                    if val!=getattr(event,mass): raise RuntimeError, 'An untracked SMS mass (%s) changed in the middle of the run! If expected, add it to the SMS_varying_masses list.'%mass

            m1 = getattr(event,self.massfill1)
            m2 = getattr(event,self.massfill2)
            if max(m1,m2)>self.maxSMSmass: raise RuntimeError, 'SMS mass found too large to be contained in the histogram: %f. Adapt the SMS_regexp_evtGenMass parameter.'%max(m1,m2)
            self.inputCounterSMS.Fill(m1,m2,1)

        if self.cfg_comp.isMC:
            if self.doLHE:
              for w in event.LHE_weights:
                    id_ = float(w.id)
                    wgt_ = float(w.wgt)
                    self.inputLHE.Fill(id_, wgt_)
#                    if isSMS: self.inputLHESMS.Fill(m1,m2,id_, wgt_)

        if self.cfg_comp.isMC:
            genWeight_ = float(self.mchandles['GenInfo'].product().weight())
            self.inputGenWeights.Fill(1, genWeight_);
            if isSMS: self.inputGenWeightsSMS.Fill(m1,m2,1, genWeight_);

        return True
