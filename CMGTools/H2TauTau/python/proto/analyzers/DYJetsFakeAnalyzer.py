from CMGTools.RootTools.analyzers.GenParticleAnalyzer import *
from CMGTools.RootTools.utils.DeltaR import matchObjectCollection
from CMGTools.RootTools.physicsobjects.genutils import *

class DYJetsFakeAnalyzer( GenParticleAnalyzer ):
    '''Checks which kind of DYJet event this is.
    isFake gets written to the event.
    - Z->tau tau : isFake = 0
    - Z->l l matched : isFake = 1
    - other : isFake =2 

    set the lepton type as leptonType in the configuration
    '''

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
            # print '-'*50
            # print map(str, taus)
            # print isZtautau
            if isZtautau:
                event.isFake = 0
            else:
                genLegs, dummy = bosonToX( event.genParticles, 23,
                                           self.cfg_ana.leptonType )
                # import pdb; pdb.set_trace()
                pairs = matchObjectCollection( [event.diLepton.leg1(),
                                                event.diLepton.leg2()],
                                               genLegs, 0.3**2 )
                if pairs[event.diLepton.leg1()] and pairs[event.diLepton.leg2()]:
                    # import pdb; pdb.set_trace()
                    # print 'fake1',event.diLepton.leg1().pdgId(), event.diLepton.leg2().pdgId()
                    event.isFake = 1
                else:
                    event.isFake = 2
        if self.cfg_ana.verbose:
            # import pdb; pdb.set_trace()
            print event.diLepton
            print 'isFake = ', event.isFake
        return True
    
            # now check if the two di-tau legs are matched to the gen leptons
