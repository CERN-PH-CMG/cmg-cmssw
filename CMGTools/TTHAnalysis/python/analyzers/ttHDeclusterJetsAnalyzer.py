from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.Jet import Jet
from CMGTools.TTHAnalysis.analyzers.ntupleTypes import ptRelv1
from PhysicsTools.HeppyCore.utils.deltar import deltaR
from copy import copy
import ROOT

class ttHDeclusterJetsAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHDeclusterJetsAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 
        # this we copy, since we modify
        self.verbose = self.cfg_ana.verbose

    def declareHandles(self):
        super(ttHDeclusterJetsAnalyzer, self).declareHandles()

    def beginLoop(self, setup):
        super(ttHDeclusterJetsAnalyzer,self).beginLoop(setup)

    def process(self, event):
        event.recoveredJets = []
        event.recoveredSplitJets = []
        for l in event.selectedLeptons:
            l.jetDecDR      = deltaR(l.eta(),l.phi(),l.jet.eta(),l.jet.phi())
            l.jetDecPtRatio = l.pt()/l.jet.pt()
            l.jetDecPtRel   = ptRelv1(l.p4(),l.jet.p4())
            l.jetDecPrunedPtRatio = l.pt()/l.jet.pt()
            l.jetDecPrunedMass    = l.jet.mass()
            if not self.cfg_ana.lepCut(l,l.jetDecPtRel): continue
            if self.verbose and type(self.verbose) == int: self.verbose -= 1
            j = l.jet 
            if self.verbose: print "lepton pt %6.1f [mc %d], jet pt %6.1f, mass %6.2f,  relIso %5.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (l.pt(), getattr(l,'mcMatchId',0), j.pt(), j.mass(), l.relIso03, l.jetDecDR, l.jetDecPtRel, l.jetDecPtRatio)
            objects  = ROOT.std.vector(ROOT.reco.Particle.LorentzVector)()
            for idau in xrange(j.numberOfDaughters()):
               dau = j.daughter(idau)
               objects.push_back(dau.p4())
            if objects.size() <= 1: continue
            reclusterJets = ROOT.ReclusterJets(objects, 1.,1.2)
            if self.cfg_ana.prune:
                pruned = reclusterJets.getPruned(self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
                l.jetDecPrunedPtRatio = l.pt()/pruned.pt()
                if self.verbose:
                    print "    ... pruned jet mass: %6.2f, ptRelv1 %5.2f, ptFraction %5.2f " % (pruned.mass(), ptRelv1(l.p4(),pruned), l.pt()/pruned.pt())
            for nsub in xrange(2,self.cfg_ana.maxSubjets+1):
               if nsub > objects.size(): break
               exclusiveJets = reclusterJets.getGroupingExclusive(nsub)
               drbest = 1; ibest = -1
               for isub in xrange(len(exclusiveJets)):
                   sj = exclusiveJets[isub]
                   dr  = deltaR(l.eta(),l.phi(),sj.eta(),sj.phi())
                   ptR = ptRelv1(l.p4(),sj)
                   ptF = l.pt()/sj.pt()
                   if dr < drbest or ibest == -1:
                       drbest = dr; ibest = isub
                   if self.cfg_ana.prune:
                       pp4 = reclusterJets.getPrunedSubjetExclusive(isub,self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
                       ppF = l.pt()/pp4.pt()
                       ppR = ptRelv1(l.p4(),pp4)
                       pM  = pp4.mass()
                   if self.verbose:
                      if self.cfg_ana.prune:
                         print "    subjet %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (isub,nsub, sj.pt(), sj.mass(), pM, dr, ptR, ppR, ptF, ppF)
                      else:
                         print "    subjet %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (isub,nsub, sj.pt(), sj.mass(), dr, ptR, ptF)
               sj = exclusiveJets[ibest]
               dr  = deltaR(l.eta(),l.phi(),sj.eta(),sj.phi())
               ptR = ptRelv1(l.p4(),sj)
               ptF = l.pt()/sj.pt()
               l.jetDecDR      = dr 
               l.jetDecPtRatio = ptF
               l.jetDecPtRel   = ptR
               if self.cfg_ana.prune:
                  pp4 = reclusterJets.getPrunedSubjetExclusive(ibest,self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
                  ppF = l.pt()/pp4.pt()
                  ppR = ptRelv1(l.p4(),pp4)
                  pM  = pp4.mass()
                  l.jetDecPrunedPtRatio = ppF #l.pt()/pp4.pt()
                  l.jetDecPrunedMass    = pM  #l.jet.mass()
               if self.verbose:
                  if self.cfg_ana.prune:
                     print "    best   %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (ibest,nsub, sj.pt(), sj.mass(), pM, dr, ptR, ppR, ptF, ppF)
                  else:
                     print "    best   %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (ibest,nsub, sj.pt(), sj.mass(), dr, ptR, ptF)
               if dr < self.cfg_ana.drMin and ptF < self.cfg_ana.ptRatioMax and (abs(ptF-1) < self.cfg_ana.ptRatioDiff or dr < self.cfg_ana.drMatch or ptR < self.cfg_ana.ptRelMin):
                    if self.verbose: print "       ---> take this as best subject, stop reclustering, consider it successful"
                    restp4 = None
                    for (i2,s2) in enumerate(exclusiveJets):
                        if i2 == ibest: continue
                        restp4 = s2 if restp4 == None else (restp4 + s2)
                        prunedJet = copy(l.jet)
                        prunedJet.physObj = ROOT.pat.Jet(l.jet.physObj)
                        prunedJet.setP4(s2)
                        event.recoveredSplitJets.append(prunedJet)
                    prunedJet = copy(l.jet)
                    prunedJet.physObj = ROOT.pat.Jet(l.jet.physObj)
                    prunedJet.setP4(restp4)
                    event.recoveredJets.append(prunedJet)
                    break
               if self.verbose: print ""
            if self.verbose: print ""
            event.recoveredJets.sort(key = lambda j : j.pt(), reverse=True)
            event.recoveredSplitJets.sort(key = lambda j : j.pt(), reverse=True)
        return True
