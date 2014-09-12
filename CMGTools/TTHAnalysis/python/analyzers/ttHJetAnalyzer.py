import random
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import * 
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.JetReCalibrator import JetReCalibrator

def cleanNearestJetOnly(jets,leptons,deltaR):
    dr2 = deltaR**2
    good = [ True for j in jets ]
    for l in leptons:
        ibest, d2m = -1, dr2
        for i,j in enumerate(jets):
            d2i = deltaR2(l.eta(),l.phi(), j.eta(),j.phi())
            if d2i < d2m:
                ibest, d2m = i, d2i
        if ibest != -1: good[ibest] = False
    return [ j for (i,j) in enumerate(jets) if good[i] == True ] 

class ttHJetAnalyzer( Analyzer ):
    """Taken from RootTools.JetAnalyzer, simplified, modified, added corrections    """
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ttHJetAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)
        mcGT   = cfg_ana.mcGT   if hasattr(cfg_ana,'mcGT') else "START53_V27"
        dataGT = cfg_ana.dataGT if hasattr(cfg_ana,'dataGT') else "FT_53_V21_AN5"
        if self.cfg_comp.isMC:
            self.jetReCalibrator    = JetReCalibrator(mcGT,"AK5PF",    False)
            self.jetReCalibratorCHS = JetReCalibrator(mcGT,"AK5PFchs", False)
        else:
            self.jetReCalibrator    = JetReCalibrator(dataGT,"AK5PF",    True)
            self.jetReCalibratorCHS = JetReCalibrator(dataGT,"AK5PFchs", True)
        self.doPuId = self.cfg_ana.doPuId if hasattr(self.cfg_ana, 'doPuId') else True
        self.shiftJEC = self.cfg_ana.shiftJEC if hasattr(self.cfg_ana, 'shiftJEC') else 0
        self.doJEC = self.cfg_ana.recalibrateJets or (self.shiftJEC != 0)
        self.jetLepDR = self.cfg_ana.jetLepDR  if hasattr(self.cfg_ana, 'jetLepDR') else 0.5
        self.lepPtMin = self.cfg_ana.minLepPt  if hasattr(self.cfg_ana, 'minLepPt') else -1
        ### MM
        self.jetGammaDR = self.cfg_ana.jetGammaDR  if hasattr(self.cfg_ana, 'jetGammaDR') else 0.4
        self.gammaPtMin = self.cfg_ana.minGammaPt  if hasattr(self.cfg_ana, 'minGammaPt') else -1
        self.gammaEtaCentral = self.cfg_ana.gammaEtaCentral  if hasattr(self.cfg_ana, 'gammaEtaCentral') else 0
        ###
    def declareHandles(self):
        super(ttHJetAnalyzer, self).declareHandles()
        self.handles['jets']     = AutoHandle( self.cfg_ana.jetCol, 'std::vector<pat::Jet>' )
        self.handles['jets4MVA'] = AutoHandle( self.cfg_ana.jetCol4MVA, 'std::vector<pat::Jet>' )
        self.handles['rho'] = AutoHandle( ('fixedGridRhoFastjetAll','',''), 'double' )
    
    def beginLoop(self):
        super(ttHJetAnalyzer,self).beginLoop()
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        rho  = float(self.handles['rho'].product()[0])
        event.rho=rho

        ## Read jets, if necessary recalibrate and shift MET
        allJets = map(Jet, self.handles['jets'].product()) 
        event.deltaMetFromJEC = [0.,0.]
        if self.doJEC:
            #print "\nCalibrating jets %s for lumi %d, event %d" % (self.cfg_ana.jetCol, event.lumi, event.eventId)
            corr = self.jetReCalibratorCHS if 'CHS' in self.cfg_ana.jetCol else self.jetReCalibrator
            corr.correctAll(allJets, rho, delta=self.shiftJEC, metShift=event.deltaMetFromJEC)
        event.allJetsUsedForMET = allJets
       
        ## If using a different collection for MVA, set it up 
        allJets4MVA = []
        if self.cfg_ana.jetCol4MVA != self.cfg_ana.jetCol:
            allJets4MVA = map(Jet, self.handles['jets4MVA'].product())
            if self.doJEC:
                #print "\nCalibrating jets %s for lumi %d, event %d" % (self.cfg_ana.jetCol4MVA, event.lumi, event.eventId)
                corr = self.jetReCalibratorCHS if 'CHS' in self.cfg_ana.jetCol4MVA else self.jetReCalibrator
                corr.correctAll(allJets4MVA, rho, delta=self.shiftJEC)
        else:
            allJets4MVA = allJets[:]

        ## QG Likelihood
        #QGAlgo = "LD_CHS_CMGVARS" if ("CHS" in self.cfg_ana.jetCol) else "LD_CMGVARS";
        #QGCorr = "Pythia" if self.cfg_comp.isMC else "Data" # FIXME: what about herwig??
        #for jet in allJets:
        #    jet.QG = jet.quarkGluonID(rho, QGAlgo, QGCorr) 
        
        ## Apply jet selection
        event.jets = []
        event.jetsFailId = []
        event.jetsAllNoID = []
        for jet in allJets:
            if self.testJetNoID( jet ): 
                event.jetsAllNoID.append(jet) 
                if self.testJetID (jet ):
                    event.jets.append(jet)
                else:
                    event.jetsFailId.append(jet)

        ## Clean Jets from leptons
        leptons = [ l for l in event.selectedLeptons if l.pt() > self.lepPtMin ]
        if self.cfg_ana.cleanJetsFromTaus:
            leptons = leptons[:] + event.selectedTaus
        #event.cleanJets, dummy = cleanObjectCollection( event.jets,
        #                                                masks = leptons,
        #                                                deltaRMin = self.jetLepDR )
        event.cleanJetsAll = cleanNearestJetOnly(event.jets, leptons, self.jetLepDR)
        event.cleanJets    = [j for j in event.cleanJetsAll if abs(j.eta()) <  self.cfg_ana.jetEtaCentral ]
        event.cleanJetsFwd = [j for j in event.cleanJetsAll if abs(j.eta()) >= self.cfg_ana.jetEtaCentral ]

        ### MM
        ## Clean Jets from photons
        photons = [ g for g in event.loosePhotonsCentral ]
        event.gamma_cleanJetsAll = cleanNearestJetOnly(event.cleanJetsAll, photons, self.jetGammaDR)
        event.gamma_cleanJets    = [j for j in event.gamma_cleanJetsAll if abs(j.eta()) <  self.cfg_ana.jetEtaCentral ]
        event.gamma_cleanJetsFwd = [j for j in event.gamma_cleanJetsAll if abs(j.eta()) >= self.cfg_ana.jetEtaCentral ]
        ###

        ## Associate jets to leptons
        leptons = event.inclusiveLeptons if hasattr(event, 'inclusiveLeptons') else event.selectedLeptons
        #jlpairs = matchObjectCollection( event.inclusiveLeptons, allJets4MVA, self.jetLepDR**2)
        jlpairs = matchObjectCollection( leptons, allJets4MVA, self.jetLepDR**2)
        for lep in leptons:
            jet = jlpairs[lep]
            if jet is None:
                lep.jet = lep
            else:
                lep.jet = jet

        return True
        

    def testJetID(self, jet):
        jet.puJetIdPassed = jet.puJetId() 
        jet.pfJetIdPassed = jet.jetID('POG_PFID_Loose') 
        if self.cfg_ana.relaxJetId:
            return True
        else:
            return jet.pfJetIdPassed and (jet.puJetIdPassed or not(self.doPuId)) 
        
    def testJetNoID( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta;
 

