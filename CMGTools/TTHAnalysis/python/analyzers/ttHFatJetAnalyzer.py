import random
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import * 
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.JetReCalibrator import JetReCalibrator


class ttHFatJetAnalyzer( Analyzer ):
    """Taken from RootTools.JetAnalyzer, simplified, modified, added corrections    """
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ttHFatJetAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)
        # -- this part needs some updates for 7.0.X (but AK7 is correct)
        #mcGT   = cfg_ana.mcGT   if hasattr(cfg_ana,'mcGT')   else "PLS170_V6AN2"
        #dataGT = cfg_ana.dataGT if hasattr(cfg_ana,'dataGT') else "GR_70_V2_AN1"
        #if self.cfg_comp.isMC:
            #self.jetReCalibrator    = JetReCalibrator(mcGT,"AK7PF",    False)
            #self.jetReCalibratorCHS = JetReCalibrator(mcGT,"AK7PFchs", False)
        #else:
            #self.jetReCalibrator    = JetReCalibrator(dataGT,"AK7PF",    True)
            #self.jetReCalibratorCHS = JetReCalibrator(dataGT,"AK7PFchs", True)
        #self.shiftJEC = self.cfg_ana.shiftJEC if hasattr(self.cfg_ana, 'shiftJEC') else 0
        #self.doJEC = self.cfg_ana.recalibrateJets or (self.shiftJEC != 0)
        self.jetLepDR = self.cfg_ana.jetLepDR  if hasattr(self.cfg_ana, 'jetLepDR') else 0.5
        self.lepPtMin = self.cfg_ana.minLepPt  if hasattr(self.cfg_ana, 'minLepPt') else -1


    def declareHandles(self):
        super(ttHFatJetAnalyzer, self).declareHandles()
        self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol, 'std::vector<pat::Jet>' )
        #self.handles['rho']  = AutoHandle( ('fixedGridRhoFastjetAll','',''), 'double' )
    
    def beginLoop(self):
        super(ttHFatJetAnalyzer,self).beginLoop()
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        #rho  = float(self.handles['rho'].product()[0])

        ## Read jets, if necessary recalibrate and shift MET
        allJets = map(Jet, self.handles['jets'].product()) 

        ## Apply jet selection
        event.fatJets     = []
        event.fatJetsNoID = []
        for jet in allJets:
            if self.testJetNoID( jet ): 
                event.fatJetsNoID.append(jet) 
                if self.testJetID (jet ):
                    event.fatJets.append(jet)


    def testJetID(self, jet):
        #jet.puJetIdPassed = jet.puJetId() 
        jet.pfJetIdPassed = jet.jetID('POG_PFID_Loose') 
        if self.cfg_ana.relaxJetId:
            return True
        else:
            return jet.pfJetIdPassed
            #return jet.pfJetIdPassed and (jet.puJetIdPassed or not(self.doPuId)) 
        
    def testJetNoID( self, jet ):
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta;
 
