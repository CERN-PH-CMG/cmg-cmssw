from CMGTools.RootTools.analyzers.GenParticleAnalyzer import *
from CMGTools.RootTools.utils.DeltaR import matchObjectCollection
from CMGTools.RootTools.physicsobjects.genutils import *
from math import sqrt

class DYJetsFakeAnalyzer( GenParticleAnalyzer ):
    '''Checks which kind of DYJet event this is.
    isFake gets written to the event.
    - Z->tau tau : isFake = 0
    - Z->tau tau with matched rec hadr. tau->l: isFake = 3
    - Z->l l matched : isFake = 1
    - other : isFake = 2 

    set the lepton type as leptonType in the configuration
    '''

    def isHadronicTau(self, tau):
        if tau.numberOfDaughters() > 0:
            # Catch status 2 tau
            if abs(tau.daughter(0).pdgId()) == 15:
                return self.isHadronicTau(tau.daughter(0))
            else:
                sumPx = 0.
                sumPy = 0.
                isHadronic = False
                for i in range(0, tau.numberOfDaughters()):
                    tdId = abs(tau.daughter(i).pdgId())
                    if tdId not in (11, 12, 13, 14, 15, 16):
                        if tdId == 24:
                            return self.isHadronicTau(tau.daughter(i))
                        isHadronic = True
                        sumPx += tau.daughter(i).px()
                        sumPy += tau.daughter(i).py()
                return isHadronic and sqrt(sumPx*sumPx + sumPy*sumPy) > 18.
        else:
            print 'WARNING, generated tau has no daughter'
        return False

    def process(self, iEvent, event):
        event.isFake = 0
        if not self.cfg_comp.isMC:
            return True
        result = super(DYJetsFakeAnalyzer, self).process(iEvent, event)
        
        # check if it is a Z->tau tau event.
        tauDaus, bosonPresent = bosonToX( event.genParticles, 23, 15)
        if not bosonPresent:
            event.isFake = 0
        else:
            # isZtautau = len( tauDaus )==2
            taus = filter(lambda x: x.status()==3 and abs(x.pdgId())==15,
                          event.genParticles)
            isZtautau = len(taus)==2

            hadronicTaus = [tau for tau in taus if self.isHadronicTau(tau)]

            # print '-'*50
            # print map(str, taus)
            # print isZtautau

            # Updated recipe Aug 2013: match both electrons and muons regardless of status 
            # with pt > 8
            genLeptons = filter(lambda x: x.pt() > 8. and (abs(x.pdgId()) == 11 or abs(x.pdgId()) == 13), event.genParticles)
            # genLegs, dummy = bosonToX( event.genParticles, 23,
                                       # self.cfg_ana.leptonType )
            # import pdb; pdb.set_trace()
            lepPairs = matchObjectCollection( [event.diLepton.leg1(),
                                            event.diLepton.leg2()],
                                           genLeptons, 0.5**2 )
            tauPairs = matchObjectCollection( [event.diLepton.leg1(),
                                            event.diLepton.leg2()],
                                           hadronicTaus, 0.5**2 )

            # if lepPairs[event.diLepton.leg1()] and lepPairs[event.diLepton.leg2()]:

            # Updated recipe: Only match tau leg, not lepton leg
            # This assumes that leg 1 is the hadronic tau!
            if isZtautau and not lepPairs[event.diLepton.leg1()] and tauPairs[event.diLepton.leg1()]:
                event.isFake = 0
            elif isZtautau and lepPairs[event.diLepton.leg1()]:
                event.isFake = 3
            elif not isZtautau and lepPairs[event.diLepton.leg1()]:
                # import pdb; pdb.set_trace()
                # print 'fake1',event.diLepton.leg1().pdgId(), event.diLepton.leg2().pdgId()
                event.isFake = 1
            # Keep this explicitly, but would be covered by global else below
            elif isZtautau and not tauPairs[event.diLepton.leg1()]:
                event.isFake = 2
            elif not isZtautau and not lepPairs[event.diLepton.leg1()]:
                event.isFake = 2
            else:
                print 'WARNING: Logic issue, no assignment in DY jets fake analyzer'
        if self.cfg_ana.verbose:
            # import pdb; pdb.set_trace()
            print event.diLepton
            print 'isFake = ', event.isFake
        return True
    
            # now check if the two di-tau legs are matched to the gen leptons
