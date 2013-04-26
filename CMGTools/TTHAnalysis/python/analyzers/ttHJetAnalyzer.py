import random
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
# from CMGTools.RootTools.physicsobjects.VBF import VBF
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.utils.DeltaR import deltaR2
from CMGTools.Common.Tools.cmsswRelease import isNewerThan
from CMGTools.TTHAnalysis.jetReCalibrator import JetReCalibrator

class ttHJetAnalyzer( Analyzer ):
    """Taken from RootTools.JetAnalyzer, simplified, modified, added corrections    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ttHJetAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

        if self.cfg_ana.recalibrateJets:
            if self.cfg_comp.isMC:
                self.jetReCalibrator    = JetReCalibrator("START53_V20","AK5PF",    False)
                self.jetReCalibratorCHS = JetReCalibrator("START53_V20","AK5PFchs", False)
            else:
                self.jetReCalibrator    = JetReCalibrator("GR_P_V42_AN4","AK5PF",    True)
                self.jetReCalibratorCHS = JetReCalibrator("GR_P_V42_AN4","AK5PFchs", True)
        self.doPuId = self.cfg_ana.doPuId if hasattr(self.cfg_ana, 'doPuId') else True

    def declareHandles(self):
        super(ttHJetAnalyzer, self).declareHandles()
        self.handles['jets']     = AutoHandle( self.cfg_ana.jetCol, 'std::vector<cmg::PFJet>' )
        self.handles['jets4MVA'] = AutoHandle( self.cfg_ana.jetCol4MVA, 'std::vector<cmg::PFJet>' )
        self.handles['rho'] = AutoHandle( ('kt6PFJets','rho',''), 'double' )
    
    def beginLoop(self):
        super(ttHJetAnalyzer,self).beginLoop()
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        ## Read jets, if necessary recalibrate
        allJets = map(Jet, self.handles['jets'].product()) 
        if self.cfg_ana.recalibrateJets:
            #print "\nCalibrating jets %s for lumi %d, event %d" % (self.cfg_ana.jetCol, event.lumi, event.eventId)
            rho  = float(self.handles['rho'].product()[0])
            corr = self.jetReCalibratorCHS if 'CHS' in self.cfg_ana.jetCol else self.jetReCalibrator
            for j in allJets: corr.correct(j, rho)
       
        ## If using a different collection for MVA, set it up 
        allJets4MVA = []
        if self.cfg_ana.jetCol4MVA != self.cfg_ana.jetCol:
            allJets4MVA = map(Jet, self.handles['jets4MVA'].product())
            if self.cfg_ana.recalibrateJets:
                #print "\nCalibrating jets %s for lumi %d, event %d" % (self.cfg_ana.jetCol4MVA, event.lumi, event.eventId)
                rho  = float(self.handles['rho'].product()[0])
                corr = self.jetReCalibratorCHS if 'CHS' in self.cfg_ana.jetCol4MVA else self.jetReCalibrator
                for j in allJets4MVA: corr.correct(j, rho)
        else:
            allJets4MVA = allJets[:]

        ## Apply jet selection
        event.jets = []
        event.jetsFailId = []
        for jet in allJets:
            if self.testJetNoID( jet ): 
                if self.testJetID (jet ):
                    event.jets.append(jet)
                else:
                    event.jetsFailId.append(jet)
        
        ## Clean Jets from leptons
        leptons = event.selectedLeptons
        event.cleanJets, dummy = cleanObjectCollection( event.jets,
                                                        masks = leptons,
                                                        deltaRMin = 0.5 )
        ## Associate jets to leptons
        jlpairs = matchObjectCollection( leptons, allJets4MVA, 0.5*0.5)
        for lep in leptons:
            jet = jlpairs[lep]
            if jet is None:
                lep.jet = lep
            else:
                lep.jet = jet

        return True
        

    def testJetID(self, jet):
        jet.puJetIdPassed = jet.puJetId()
        jet.pfJetIdPassed = jet.getSelection('cuts_looseJetId')
        if self.cfg_ana.relaxJetId:
            return True
        else:
            return jet.pfJetIdPassed and (jet.puJetIdPassed or not(self.doPuId)) 
        
    def testJetNoID( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta;
 

