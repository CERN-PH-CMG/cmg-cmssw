import random
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Jet
from PhysicsTools.HeppyCore.utils.deltar import * 
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.physicsutils.JetReCalibrator import JetReCalibrator
import PhysicsTools.HeppyCore.framework.config as cfg

import operator 
import itertools
import copy
from ROOT import TLorentzVector, TVectorD
import ROOT
import math

from copy import deepcopy

class METNoHFAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(METNoHFAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(METNoHFAnalyzer, self).declareHandles()
        self.handles['cmgCand'] = AutoHandle( self.cfg_ana.candidates, self.cfg_ana.candidatesTypes )
        #self.handles['vertices'] =  AutoHandle( "offlineSlimmedPrimaryVertices", 'std::vector<reco::Vertex>', fallbackLabel="offlinePrimaryVertices" )

    def beginLoop(self, setup):
        super(METNoHFAnalyzer,self).beginLoop(setup)

    

    def makeMETNoHF(self, event):

        pfcands = self.handles['cmgCand'].product()
        px = py = 0
        for i, x in enumerate(pfcands):
            if x.fromPV()>0 and abs(x.eta()) < 3.0:
                px += x.px()
                py += x.py()
        #px += - event.met.shiftedPx(0)
        #py += - event.met.shiftedPy(0)
        
#        px = - sum([x.px() for x in pfcands if x.fromPV()>0 and abs(x.eta()) < 3.0]) - event.met.shiftedPx(0)
#        py = - sum([x.py() for x in pfcands if x.fromPV()>0 and abs(x.eta()) < 3.0]) - event.met.shiftedPy(0)
        
        if self.cfg_ana.recalibrate and hasattr(event, 'deltaMetFromJetSmearing'+self.cfg_ana.jetAnalyzerCalibrationPostFix):
            deltaMetSmear = getattr(event, 'deltaMetFromJetSmearing'+self.cfg_ana.jetAnalyzerCalibrationPostFix)
            px += deltaMetSmear[0]
            py += deltaMetSmear[1]

        if self.cfg_ana.recalibrate and hasattr(event, 'deltaMetFromJEC'+self.cfg_ana.jetAnalyzerCalibrationPostFix):
            deltaMetJEC = getattr(event, 'deltaMetFromJEC'+self.cfg_ana.jetAnalyzerCalibrationPostFix)
            px += deltaMetJEC[0]
            py += deltaMetJEC[1]

        event.metNoHF = ROOT.reco.Particle.LorentzVector(px, py, 0, math.hypot(px, py)) 
    
    
    
    def process(self, event):
        self.readCollections( event.input )

        self.makeMETNoHF(event)

        return True


setattr(METNoHFAnalyzer,"defaultConfig", cfg.Analyzer(
    class_object = METNoHFAnalyzer,
    recalibrate = True,
    jetAnalyzerCalibrationPostFix = "",
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    collectionPostFix = "",
    )
)
