import random
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import * 
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.JetReCalibrator import JetReCalibrator
from CMGTools.TTHAnalysis.tools.QGLikelihoodCalculator import QGLikelihoodCalculator

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
        self.jetGammaDR = self.cfg_ana.jetGammaDR  if hasattr(self.cfg_ana, 'jetGammaDR') else 0.4
        self.gammaPtMin = self.cfg_ana.minGammaPt  if hasattr(self.cfg_ana, 'minGammaPt') else -1
        self.gammaEtaCentral = self.cfg_ana.gammaEtaCentral  if hasattr(self.cfg_ana, 'gammaEtaCentral') else 0
        self.qglcalc = QGLikelihoodCalculator("/afs/cern.ch/user/t/tomc/public/QG_pdfs_13TeV_2014-10-12/pdfQG_AK4chs_antib_NoQC_13TeV.root")


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

        
        ## Apply jet selection
        event.jets = []
        event.jetsFailId = []
        event.jetsAllNoID = []
        event.jetsIdOnly = []
        for jet in allJets:
            if self.testJetNoID( jet ): 
                event.jetsAllNoID.append(jet) 
                if self.testJetID (jet ):
                    
                    if(self.cfg_ana.doQG):
                        self.computeQGvars(jet)
                        jet.qgl = self.qglcalc.computeQGLikelihood(jet, rho)

                    ##manually match to genparticle
                    #deltaRmin = 999.
                    #foundPartonId = 0
                    #foundPartonMomId = 0
                    #for ipart in event.genParticles:
                    #  if ipart.status() != 23 and ipart.status() != 3: continue
                    #  if ipart.pt() < 1.: continue
                    #  if abs(ipart.eta()) > 10: continue
                    #  if abs(ipart.pdgId())>5 and ipart.pdgId()!=21: continue
                    #  thisDeltaR = deltaR( ipart.eta(), ipart.phi(), jet.eta(), jet.phi() )
                    #  if thisDeltaR < deltaRmin:
                    #    deltaRmin=thisDeltaR
                    #    foundPartonId=ipart.pdgId()
                    #    if( ipart.numberOfMothers()>0 ):
                    #      foundPartonMomId=ipart.mother(0).pdgId()
                    #jet.partonId = 0
                    #jet.partonMotherId = 0
                    #if deltaRmin<0.4:
                    #  jet.partonId=foundPartonId
                    #  jet.partonMotherId=foundPartonMomId

                    event.jets.append(jet)
                    event.jetsIdOnly.append(jet)
                        
                else:
                    event.jetsFailId.append(jet)
            elif self.testJetID (jet ):
                event.jetsIdOnly.append(jet)

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

        ## Clean Jets from photons
        photons = [ g for g in event.selectedPhotonsCentral ]
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
 


    def computeQGvars(self, jet):

       jet.mult = 0
       sum_weight = 0.
       sum_pt = 0.
       sum_deta = 0.
       sum_dphi = 0.
       sum_deta2 = 0.
       sum_detadphi = 0.
       sum_dphi2 = 0.



       for ii in range(0, jet.numberOfDaughters()) :

         part = jet.daughter(ii)

         usePart = True

         if part.charge() == 0 : # neutral particles 

           if part.pt() > 1.: jet.mult += 1

         else : # charged particles

           jet.mult += 1

           if part.trackHighPurity()==False: usePart=False
           if part.fromPV()<=1: usePart=False



         if usePart:
           deta = part.eta() - jet.eta()
           dphi = deltaPhi(part.phi(), jet.phi())
           partPt = part.pt()
           weight = partPt*partPt
           sum_weight += weight
           sum_pt += partPt
           sum_deta += deta*weight
           sum_dphi += dphi*weight
           sum_deta2 += deta*deta*weight
           sum_detadphi += deta*dphi*weight
           sum_dphi2 += dphi*dphi*weight




       a = 0.
       b = 0.
       c = 0.

       if sum_weight > 0 :
         jet.ptd = math.sqrt(sum_weight)/sum_pt
         ave_deta = sum_deta/sum_weight
         ave_dphi = sum_dphi/sum_weight
         ave_deta2 = sum_deta2/sum_weight
         ave_dphi2 = sum_dphi2/sum_weight
         a = ave_deta2 - ave_deta*ave_deta
         b = ave_dphi2 - ave_dphi*ave_dphi
         c = -(sum_detadphi/sum_weight - ave_deta*ave_dphi)
       else: jet.ptd = 0.

       delta = math.sqrt(math.fabs((a-b)*(a-b)+4.*c*c))

       if a+b-delta > 0: jet.axis2 = -math.log(math.sqrt(0.5*(a+b-delta)))
       else: jet.axis2 = -1.


