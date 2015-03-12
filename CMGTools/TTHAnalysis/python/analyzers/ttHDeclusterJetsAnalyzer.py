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
            # lepton-jet variable, jet is obtained from reclustering of associated akt04 jet daughters, with kt exclusive
            l.jetDecDR      = deltaR(l.eta(),l.phi(),l.jet.eta(),l.jet.phi())
            l.jetDecPtRatio = l.pt()/l.jet.pt()
            l.jetDecPtRel   = ptRelv1(l.p4(),l.jet.p4())
            l.jetDecPrunedPtRatio = l.pt()/l.jet.pt()
            l.jetDecPrunedMass    = l.jet.mass()
            
            # lepton-jet variable, jet is obtained from reclustering of associated akt04 jet daughters, with kt inclusive R=0.2
            l.jetDec02DR      = deltaR(l.eta(),l.phi(),l.jet.eta(),l.jet.phi())
            l.jetDec02PtRatio = l.pt()/l.jet.pt()
            l.jetDec02PtRel   = ptRelv1(l.p4(),l.jet.p4())
            l.jetDec02PrunedPtRatio = l.pt()/l.jet.pt()
            l.jetDec02PrunedMass    = l.jet.mass()
            
            # # lepton-jet variable, jet is obtained from reclustering of associated ak08 jet daughters, with kt inclusive R=0.2
            # l.fjetDec02DR      = deltaR(l.eta(),l.phi(),l.fatjet.eta(),l.fatjet.phi())
            # l.fjetDec02PtRatio = l.pt()/l.fatjet.pt()
            # l.fjetDec02PtRel   = ptRelv1(l.p4(),l.fatjet.p4())
            # l.fjetDec02PrunedPtRatio = l.pt()/l.fatjet.pt()
            # l.fjetDec02PrunedMass    = l.fatjet.mass()
            
            if not self.cfg_ana.lepCut(l,l.jetDecPtRel): continue
            if self.verbose and type(self.verbose) == int: self.verbose -= 1
            j = l.jet  
            fj = l.fatjet
            if self.verbose: print "lepton pt %6.1f [mc %d], jet pt %6.1f, mass %6.2f,  relIso %5.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (l.pt(), getattr(l,'mcMatchId',0), j.pt(), j.mass(), l.relIso03, l.jetDecDR, l.jetDecPtRel, l.jetDecPtRatio)
            objects  = ROOT.std.vector(ROOT.reco.Particle.LorentzVector)()
            fobjects  = ROOT.std.vector(ROOT.reco.Particle.LorentzVector)()
            for idau in xrange(j.numberOfDaughters()):
               dau = j.daughter(idau)
               objects.push_back(dau.p4())
            for fidau in xrange(fj.numberOfDaughters()):
                fdau = fj.daughter(fidau)
                fobjects.push_back(fdau.p4()) 
               
            
            ##### considering only candidates from the ak04 jet   
            if objects.size() <= 1: continue           
            # kt exclusive
            reclusterJets = ROOT.heppy.ReclusterJets(objects, 1.,10)
            # kt inclusive R=0.2
            reclusterJets02 = ROOT.heppy.ReclusterJets(objects, 1.,0.2)          
            if self.cfg_ana.prune:
                pruned = reclusterJets.getPruned(self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
                l.jetDecPrunedPtRatio = l.pt()/pruned.pt()
                if self.verbose:
                    print "    ... pruned jet mass: %6.2f, ptRelv1 %5.2f, ptFraction %5.2f " % (pruned.mass(), ptRelv1(l.p4(),pruned), l.pt()/pruned.pt())
                    
            # compute lepton-jet variable, jet is obtained from reclustering of associated akt04 jet daughters, with kt exclusive        
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
                         print "    exclusive subjet %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (isub,nsub, sj.pt(), sj.mass(), pM, dr, ptR, ppR, ptF, ppF)
                      else:
                         print "    exlcusive subjet %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (isub,nsub, sj.pt(), sj.mass(), dr, ptR, ptF)
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
                     print "    best exclusive subject %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (ibest,nsub, sj.pt(), sj.mass(), pM, dr, ptR, ppR, ptF, ppF)
                  else:
                     print "    best exclusive subject %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (ibest,nsub, sj.pt(), sj.mass(), dr, ptR, ptF)
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

            # lepton-jet variable, jet is obtained from reclustering of associated akt04 jet daughters, with kt inclusive R=0.2
            inclusiveJets02 = reclusterJets02.getGrouping(self.cfg_ana.ptMinSubjets)
            drbest = 1; ibest = -1
            for isub in xrange(len(inclusiveJets02)):
                ij = inclusiveJets02[isub]
                dr  = deltaR(l.eta(),l.phi(),ij.eta(),ij.phi())
                ptR = ptRelv1(l.p4(),ij)
                ptF = l.pt()/ij.pt()
                if dr < drbest or ibest == -1:
                    drbest = dr; ibest = isub
                if self.cfg_ana.prune:
                    pp4 = reclusterJets02.getPrunedSubjetInclusive(isub,self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
                    ppF = l.pt()/pp4.pt()
                    ppR = ptRelv1(l.p4(),pp4)
                    pM  = pp4.mass()
                if self.verbose:
                   if self.cfg_ana.prune:
                      print "    inclusive subjet %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (isub,nsub, ij.pt(), ij.mass(), pM, dr, ptR, ppR, ptF, ppF)
                   else:
                      print "    inclusive subjet %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (isub,nsub, ij.pt(), ij.mass(), dr, ptR, ptF)
            ij = inclusiveJets02[ibest]
            dr  = deltaR(l.eta(),l.phi(),ij.eta(),ij.phi())
            ptR = ptRelv1(l.p4(),ij)
            ptF = l.pt()/ij.pt()
            l.jetDec02DR      = dr 
            l.jetDec02PtRatio = ptF
            l.jetDec02PtRel   = ptR
            if self.cfg_ana.prune:
               pp4 = reclusterJets02.getPrunedSubjetInclusive(ibest,self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
               ppF = l.pt()/pp4.pt()
               ppR = ptRelv1(l.p4(),pp4)
               pM  = pp4.mass()
               l.jetDec02PrunedPtRatio = ppF #l.pt()/pp4.pt()
               l.jetDec02PrunedMass    = pM  #l.jet.mass()
            if self.verbose:
               if self.cfg_ana.prune:
                  print "    best inclusive subject %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (ibest,nsub, ij.pt(), ij.mass(), pM, dr, ptR, ppR, ptF, ppF)
               else:
                  print "    best inclusive subject %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (ibest,nsub, ij.pt(), ij.mass(), dr, ptR, ptF)

            # ##### considering candidates from the ak08 jet   
            # if fobjects.size() <= 1: continue           
            # # kt inclusive R=0.2
            # reclusterFJets02 = ROOT.heppy.ReclusterJets(fobjects, 1.,0.2)          
            
            # # lepton-jet variable, jet is obtained from reclustering of associated akt08 jet daughters, with kt inclusive R=0.2
            # inclusiveFJets02 = reclusterFJets02.getGrouping(self.cfg_ana.ptMinSubjets)
            # drbest = 1; ibest = -1
            # for isub in xrange(len(inclusiveFJets02)):
            #     ij = inclusiveFJets02[isub]
            #     dr  = deltaR(l.eta(),l.phi(),ij.eta(),ij.phi())
            #     ptR = ptRelv1(l.p4(),ij)
            #     ptF = l.pt()/ij.pt()
            #     if dr < drbest or ibest == -1:
            #         drbest = dr; ibest = isub
            #     if self.cfg_ana.prune:
            #         pp4 = reclusterFJets02.getPrunedSubjetInclusive(isub,self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
            #         ppF = l.pt()/pp4.pt()
            #         ppR = ptRelv1(l.p4(),pp4)
            #         pM  = pp4.mass()
            #     if self.verbose:
            #         if self.cfg_ana.prune:
            #             print "    inclusive subjet from fj %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (isub,nsub, ij.pt(), ij.mass(), pM, dr, ptR, ppR, ptF, ppF)
            #         else:
            #             print "    inclusive subjet from fj %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (isub,nsub, ij.pt(), ij.mass(), dr, ptR, ptF)
            # ij = inclusiveFJets02[ibest]
            # dr  = deltaR(l.eta(),l.phi(),ij.eta(),ij.phi())
            # ptR = ptRelv1(l.p4(),ij)
            # ptF = l.pt()/ij.pt()
            # l.fjetDec02DR      = dr 
            # l.fjetDec02PtRatio = ptF
            # l.fjetDec02PtRel   = ptR
            # if self.cfg_ana.prune:
            #     pp4 = reclusterFJets02.getPrunedSubjetInclusive(ibest,self.cfg_ana.pruneZCut,self.cfg_ana.pruneRCutFactor)
            #     ppF = l.pt()/pp4.pt()
            #     ppR = ptRelv1(l.p4(),pp4)
            #     pM  = pp4.mass()
            #     l.fjetDec02PrunedPtRatio = ppF #l.pt()/pp4.pt()
            #     l.fjetDec02PrunedMass    = pM  #l.jet.mass()
            # if self.verbose:
            #     if self.cfg_ana.prune:
            #         print "    best inclusive subjet from fj  %d/%d: pt %6.1f, mass %6.2f (pruned %6.2f), dr(lep) = %.3f, ptRel v1 = %5.1f (pruned %5.1f), ptF = %5.2f (pruned %5.2f)" % (ibest,nsub, ij.pt(), ij.mass(), pM, dr, ptR, ppR, ptF, ppF)
            #     else:
            #         print "    best inclusive subject from fj  %d/%d: pt %6.1f, mass %6.2f, dr(lep) = %.3f, ptRel v1 = %5.1f, ptF = %5.2f" % (ibest,nsub, ij.pt(), ij.mass(), dr, ptR, ptF)     
                    
                     
        return True
