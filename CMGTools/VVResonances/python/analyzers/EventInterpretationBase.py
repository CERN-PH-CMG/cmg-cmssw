import random
import math
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsutils.JetReCalibrator import JetReCalibrator
from PhysicsTools.HeppyCore.utils.deltar import *
import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.VVResonances.tools.PyJetToolbox import *




class EventInterpretationBase( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EventInterpretationBase,self).__init__(cfg_ana, cfg_comp, looperName)
        self.selectFat = self.cfg_ana.selectFat
        self.selectPairLNu = self.cfg_ana.selectPairLNu
        self.selectPairLL = self.cfg_ana.selectPairLL
        self.selectPairJJ = self.cfg_ana.selectPairJJ
        self.isMC =cfg_comp.isMC
        if hasattr(cfg_ana,'matchDR'):
            self.matchDR = cfg_ana.matchDR
        else:
            self.matchDR = 0.2

        mcGT   = cfg_ana.mcGT   if hasattr(cfg_ana,'mcGT')   else "PHYS14_25_V2"
        dataGT = cfg_ana.dataGT if hasattr(cfg_ana,'dataGT') else "GR_70_V2_AN1"
        self.shiftJEC = self.cfg_ana.shiftJEC if hasattr(self.cfg_ana, 'shiftJEC') else 0
        self.recalibrateJets = self.cfg_ana.recalibrateJets
        if   self.recalibrateJets == "MC"  : self.recalibrateJets =     self.cfg_comp.isMC
        elif self.recalibrateJets == "Data": self.recalibrateJets = not self.cfg_comp.isMC
        elif self.recalibrateJets not in [True,False]: raise RuntimeError, "recalibrateJets must be any of { True, False, 'MC', 'Data' }, while it is %r " % self.recalibrateJets
        self.doJEC = self.recalibrateJets or (self.shiftJEC != 0)
        if self.doJEC:
          if self.cfg_comp.isMC:
              if hasattr(self.cfg_comp,'globalTag'):
                  self.jetReCalibrator = JetReCalibrator(self.cfg_comp.globalTag,self.cfg_ana.recalibrationType, False,cfg_ana.jecPath)
              else:
                  self.jetReCalibrator = JetReCalibrator(mcGT,self.cfg_ana.recalibrationType, False,cfg_ana.jecPath)
          else:
              if hasattr(self.cfg_comp,'globalTag'):
                  self.jetReCalibrator = JetReCalibrator(self.cfg_comp.globalTag,self.cfg_ana.recalibrationType, True,cfg_ana.jecPath)
              else:    
                  self.jetReCalibrator = JetReCalibrator(dataGT,self.cfg_ana.recalibrationType, True,cfg_ana.jecPath)

        self.attachBTag = cfg_ana.attachBTag    
        if self.attachBTag:
            self.btagDiscriminator = cfg_ana.btagDiscriminator
            
        if hasattr(cfg_ana,'doSkim'):
            self.doSkim = cfg_ana.doSkim
        else:
            self.doSkim=False
            
            
    def declareHandles(self):
        super(EventInterpretationBase, self).declareHandles()
        self.handles['packed'] = AutoHandle( 'packedPFCandidates', 'std::vector<pat::PackedCandidate>' )
        self.handles['rho'] = AutoHandle( self.cfg_ana.rho, 'double' )
        if self.attachBTag or self.doSkim:
            self.handles['jets'] = AutoHandle( self.cfg_ana.standardJets, 'std::vector<pat::Jet>' )
            self.handles['fatjets'] = AutoHandle( self.cfg_ana.fatJets, 'std::vector<pat::Jet>' )
            self.handles['subjets'] = AutoHandle( (self.cfg_ana.subJets,'SubJets'), 'std::vector<pat::Jet>' )


            

    def removeLeptonFootPrint(self,leptons,cands):
        toRemove=[]
        for lepton in leptons:
            for p in range(0,lepton.numberOfSourceCandidatePtrs()):
                index = lepton.sourceCandidatePtr(p).key()
                toRemove.append(cands[index])
        return list(set(cands)-set(toRemove))


    def matchSubJets(self,jets,genquarks):
        for j in jets:
            for s in j.subjets:
                s.mcquark = None
                s.matched=0
                for g in genquarks:
                    if deltaR(s.eta(),s.phi(),g.eta(),g.phi())<self.matchDR:
                        s.matched=1
                        s.mcquark = g
                        break;

    def skim(self,leptons):
        cleanedJets = []
        for jet in self.handles['fatjets'].product():
            overlap=False
            for lepton in leptons:
                if deltaR(jet.eta(),jet.phi(),lepton.eta(),lepton.phi())<0.8:
                    overlap=True
                    break;
            if not overlap:    
                cleanedJets.append(jet)
        nJets = len(cleanedJets)        
        nLeptons = len(leptons)
        if (nLeptons>0 and nJets>0) or nJets>1:
            return True
        return False
                
                

                    
    def makeFatJets(self,cands):
        toolboxFat  = PyJetToolbox(cands)
        toolboxFat.setInterface(True,self.cfg_ana.ktPowerFat,self.cfg_ana.rFat)
        toolboxFat.setMassDrop(self.cfg_ana.massdrop)
        toolboxFat.setSubjets(True,'inc',self.cfg_ana.subjets)
        toolboxFat.setPruning(self.cfg_ana.prunning)
        toolboxFat.setNtau(True)
        toolboxFat.setSoftDrop(self.cfg_ana.softdrop)
        # Lets cluster !! Fat jets first
        fatJets=toolboxFat.inclusiveJets(100.0,True)
        filtered = filter(self.selectFat,fatJets)

        standardFatJets = self.handles['fatjets'].product()

        if self.attachBTag:
            for fat in filtered:
                for standardFat in standardFatJets:
                    fat.btag = standardFat.bDiscriminator(self.btagDiscriminator)
                for j in fat.subjets:
                    for standard in self.handles['subjets'].product():
                        if deltaR(j.eta(),j.phi(),standard.eta(),standard.phi())<0.1:
                            j.btag = standard.bDiscriminator(self.btagDiscriminator)
                            break

        return filtered

    def makeSatelliteJets(self,cands):
        toolbox  = PyJetToolbox(cands)
        toolbox.setInterface(True,self.cfg_ana.ktPower,self.cfg_ana.r)
        toolbox.setMassDrop(False)
        toolbox.setSubjets(False,'inc',2)
        toolbox.setPruning(False)
        toolbox.setNtau(False)
        toolbox.setSoftDrop(False)
        
        unfiltered =  toolbox.inclusiveJets(30.0,False)
        if self.attachBTag:
            for j in unfiltered:
                    for standard in self.handles['jets'].product():
                        if deltaR(j.eta(),j.phi(),standard.eta(),standard.phi())<0.1:
                            j.btag = standard.bDiscriminator(self.btagDiscriminator)
                            break

        if self.jetReCalibrator is not None:
            self.jetReCalibrator.correctAll(unfiltered, self.rho, delta=self.shiftJEC)
            return filter(lambda x: x.pt()>30, unfiltered)
        else:
            return unfiltered

    def removeJetFootPrint(self,jets,cands):
        toRemove=[]
        for j in jets:
            toRemove.extend(j.constituents)
        return list(set(cands)-set(toRemove))


    def topology(self,obj):
        if len(obj['satelliteJets'])<2:
            obj['vbfDEta'] = -1.0
            obj['vbfMJJ'] = -1.0
        else:            
            j1 = obj['satelliteJets'][0]
            j2 = obj['satelliteJets'][1]
            obj['vbfDEta'] = abs(j1.eta()-j2.eta())
            obj['vbfMJJ'] = (j1.p4()+j2.p4()).M()

        NL = 0     
        NM = 0     
        NT = 0     
       
        for s in obj['satelliteJets']:
           btag = s.bTag()
           if btag>0.423:
               NL=NL+1
           if btag>0.814:
               NM=NM+1
           if btag>0.941:
               NT=NT+1
        obj['nLooseBTags'] = NL        
        obj['nMediumBTags'] = NM        
        obj['nTightBTags'] = NT        
    
    def beginLoop(self, setup):
        super(EventInterpretationBase,self).beginLoop(setup)

    def process(self, event):
        self.readCollections( event.input )
        rho  = float(self.handles['rho'].product()[0])
        self.rho = rho

            

        


                
                
