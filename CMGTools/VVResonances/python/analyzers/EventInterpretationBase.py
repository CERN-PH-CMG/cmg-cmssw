import random
import math
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsutils.JetReCalibrator import JetReCalibrator
from PhysicsTools.HeppyCore.utils.deltar import *
import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.VVResonances.tools.PyJetToolbox import *
from copy import copy
from copy import deepcopy



class EventInterpretationBase( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EventInterpretationBase,self).__init__(cfg_ana, cfg_comp, looperName)
        self.selectFat = self.cfg_ana.selectFat
        self.selectPairLNu = self.cfg_ana.selectPairLNu
        self.selectPairLL = self.cfg_ana.selectPairLL
        self.selectPairJJ = self.cfg_ana.selectPairJJ
        self.selectPairJJNuNu = self.cfg_ana.selectPairJJNuNu
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
                  self.jetReCalibratorFAT = JetReCalibrator(self.cfg_comp.globalTag,self.cfg_ana.recalibrationTypeFAT, False,cfg_ana.jecPath)

              else:
                  self.jetReCalibrator = JetReCalibrator(mcGT,self.cfg_ana.recalibrationType, False,cfg_ana.jecPath)
                  self.jetReCalibratorFAT = JetReCalibrator(mcGT,self.cfg_ana.recalibrationTypeFAT, False,cfg_ana.jecPath,3,True)
          else:
              if hasattr(self.cfg_comp,'globalTag'):
                  self.jetReCalibrator = JetReCalibrator(self.cfg_comp.globalTag,self.cfg_ana.recalibrationType, True,cfg_ana.jecPath)
                  self.jetReCalibratorFAT = JetReCalibrator(self.cfg_comp.globalTag,self.cfg_ana.recalibrationTypeFAT, True,cfg_ana.jecPath)
              else:    
                  self.jetReCalibrator = JetReCalibrator(dataGT,self.cfg_ana.recalibrationType, True,cfg_ana.jecPath)
                  self.jetReCalibratorFAT = JetReCalibrator(dataGT,self.cfg_ana.recalibrationTypeFAT, True,cfg_ana.jecPath)

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
        cList=list(cands)
        for lepton in leptons:

            for p in range(0,lepton.numberOfSourceCandidatePtrs()):
                index = lepton.sourceCandidatePtr(p).key()
                if not cands[index] in toRemove: 
                    toRemove.append(cands[index])

        for r in toRemove:
            cList.remove(r)
        return cList


    def matchSubJets(self,jets,genquarks):
        for j in jets:
            for s in j.subjets_SD+j.subjets:
                s.mcquark = None
                s.matched=0
                for g in genquarks:
                    if deltaR(s.eta(),s.phi(),g.eta(),g.phi())<self.matchDR:
                        s.matched=1
                        s.mcquark = g
                        break;

    def skim(self,leptons,met):
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
        if (nLeptons>0 and nJets>0) or nJets>1 or (nJets>0 and met.pt()>300):
            return True
        return False
                

    def puppiWeight(self,cands):
        skimmedCollection=[]
        for p in cands:
            p.setP4(p.p4()*p.puppiWeight())


            if p.pt()>0:
                skimmedCollection.append(p)
        return skimmedCollection        

                    
    def makeFatJets(self,cands):
        toolboxFat  = PyJetToolbox(cands)
        toolboxFat.setInterface(True,self.cfg_ana.ktPowerFat,self.cfg_ana.rFat)
        toolboxFat.setMassDrop(self.cfg_ana.massdrop)
        toolboxFat.setSubjets(True,'inc',self.cfg_ana.subjets)
        toolboxFat.setPruning(self.cfg_ana.prunning)
        toolboxFat.setNtau(True)
        if hasattr(self.cfg_ana,'softdrop_beta'):
            toolboxFat.setSoftDrop(self.cfg_ana.softdrop,self.cfg_ana.softdrop_beta,self.cfg_ana.softdrop_zeta)
        else:
            toolboxFat.setSoftDrop(self.cfg_ana.softdrop)
        # Lets cluster !! Fat jets first
        fatJets=toolboxFat.inclusiveJets(200.0,True)
        filtered = filter(self.selectFat,fatJets)

        ##Apply JECS in original,SoftDrop and pruned:
        if self.jetReCalibratorFAT is not None:

            prunedJets=[]
            for j in fatJets:
                prunedJets.append(j)
                prunedJets.append(j.softDropJet)
                prunedJets.append(j.prunedJet)
                
            #do not apply L1 corrections:rho=0.0
            self.jetReCalibratorFAT.correctAll(prunedJets, 0.0, self.shiftJEC,True,False,[0.,0.],[0.,0.,0.])
#            self.jetReCalibratorFAT.correctAll(originalJets, self.rho, self.shiftJEC,True,False,[0.,0.],[0.,0.,0.])
            
            for p in prunedJets:
                p.setRawFactor(1.0/p.corr)
                


        standardFatJets = self.handles['fatjets'].product()

        if self.attachBTag:
            for fat in filtered:
                for standardFat in standardFatJets:
                    fat.btag = standardFat.bDiscriminator(self.btagDiscriminator)
                for j in fat.subjets_SD+fat.subjets:
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
            self.jetReCalibrator.correctAll(unfiltered, self.rho, self.shiftJEC,True,False,[0.,0.],[0.,0.,0.])
            for p in unfiltered:
                p.setRawFactor(1.0/p.corr)


            filtered=filter(lambda x: x.pt()>30, unfiltered)    
            return filtered
        else:
            return unfiltered

    def removeJetFootPrint(self,jets,cands):


        toRemove=[]
        for j in jets:
            toRemove.extend(j.constituents)
        out=list(set(cands)-set(toRemove))
        return out


    def topology(self,obj):

        #first clean nicely the jets from leptons and subjets
        objToClean=[]
        if abs(obj['pair'].leg1.pdgId())==23:
            objToClean.extend([obj['pair'].leg1.leg1,obj['pair'].leg1.leg2])
#            print 'Z->ll', obj['pair'].leg1.leg1.pt(),obj['pair'].leg1.leg1.eta(),obj['pair'].leg1.leg1.phi(),obj['pair'].leg1.leg2.pt(),obj['pair'].leg1.leg2.eta(),obj['pair'].leg1.leg2.phi()
        if abs(obj['pair'].leg1.pdgId())==24:
            objToClean.extend([obj['pair'].leg1.leg1])
#            print 'W->lnu', obj['pair'].leg1.leg1.pt(),obj['pair'].leg1.leg1.eta(),obj['pair'].leg1.leg1.phi()

        if abs(obj['pair'].leg1.pdgId())==1:
            objToClean.extend(obj['pair'].leg1.subjets)

        if abs(obj['pair'].leg2.pdgId())==23:
            objToClean.extend([obj['pair'].leg2.leg1,obj['pair'].leg2.leg2])
        if abs(obj['pair'].leg2.pdgId())==24:
            objToClean.extend([obj['pair'].leg2.leg1])
        if abs(obj['pair'].leg2.pdgId())==1:
            objToClean.extend(obj['pair'].leg2.subjets)
#            print 'jet', obj['pair'].leg2.pt(),obj['pair'].leg2.eta(),obj['pair'].leg2.phi()

        newJets=[]    
        for j in  obj['satelliteJets']:
            keep=True
            for c in objToClean:
                dr=deltaR(j.eta(),j.phi(),c.eta(),c.phi())
#                print 'dr=',dr
                if dr<0.4:
                    keep=False
                    break
            if keep:
#                print 'filtered satellite jet',j.pt(),j.eta(),j.phi() 
                newJets.append(j)


        obj['satelliteJets']=newJets            

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


        bestBTag=None
        minDR=1000.0
        for s in obj['satelliteJets']:
           btag = s.bTag()
           if btag>0.423:
               NL=NL+1
               DR=deltaR(obj['pair'].eta(),obj['pair'].phi(),s.eta(),s.phi())
               if DR<minDR:
                   bestBTag=s
                   minDR=DR
           if btag>0.814:
               NM=NM+1
           if btag>0.941:
               NT=NT+1
        obj['nLooseBTags'] = NL        
        obj['nMediumBTags'] = NM        
        obj['nTightBTags'] = NT    

        
        if bestBTag==None:
            obj['topMass'] = -1
        else:    
            obj['topMass'] = (bestBTag.p4()+s.p4()).mass()

    def beginLoop(self, setup):
        super(EventInterpretationBase,self).beginLoop(setup)

    def process(self, event):
        self.readCollections( event.input )
        rho  = float(self.handles['rho'].product()[0])
        self.rho = rho

            

        


                
                
