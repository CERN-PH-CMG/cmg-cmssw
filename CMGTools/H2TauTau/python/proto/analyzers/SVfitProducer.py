from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from TauAnalysis.SVfitStandalone.SVfitStandaloneAlgorithm import SVfitAlgo
from TauAnalysis.SVfitStandalone.MeasuredTauLepton import measuredTauLepton

from ROOT import TMatrixD, svFitStandalone, std

class SVfitProducer(Analyzer):
    '''Computes SVfit di-tau mass at the ntuple level.'''

    def __init__(self, *args):
        super(SVfitProducer, self).__init__(*args)
        self.legType = {'undef':0, 'tau':1, 'ele':2, 'muon':3, 'prompt':4}

    def process(self, event):

        decayMode1 = -1
        decayMode2 = -1

        if self.cfg_ana.l1type == 'tau':
            decayMode1 = event.leg1.decayMode()
        if self.cfg_ana.l2type == 'tau':
            decayMode2 = event.leg2.decayMode()

        leg1 = measuredTauLepton(self.legType[self.cfg_ana.l1type], event.leg1.pt(),
                                 event.leg1.eta(), event.leg1.phi(), event.leg1.mass(), decayMode1)
        leg2 = measuredTauLepton(self.legType[self.cfg_ana.l2type], event.leg2.pt(),
                                 event.leg2.eta(), event.leg2.phi(), event.leg2.mass(), decayMode2)

        measuredLeptons = std.vector('svFitStandalone::MeasuredTauLepton')()
        measuredLeptons.push_back(leg1) # RIC: order matters!
        measuredLeptons.push_back(leg2) # RIC: order matters!

        metcov = TMatrixD(2, 2)

        metcov[0][0] = event.diLepton.mvaMetSig(0, 0)
        metcov[0][1] = event.diLepton.mvaMetSig(0, 1)
        metcov[1][0] = event.diLepton.mvaMetSig(1, 0)
        metcov[1][1] = event.diLepton.mvaMetSig(1, 1)

        mex = event.diLepton.met().px()
        mey = event.diLepton.met().py()

        svfit = SVfitAlgo(measuredLeptons, mex, mey, metcov, self.cfg_ana.verbose)

        if self.cfg_ana.integration == 'VEGAS':
            svfit.integrateVEGAS()
        elif self.cfg_ana.integration == 'MarkovChain':
            svfit.integrateMarkovChain()
        else:
            print 'The integration method must be defined in the cfg as "integration".'
            print 'Options: [VEGAS, MarkovChain]'
            raise

        # debug
        if hasattr(self.cfg_ana, 'debug'):
            if self.cfg_ana.debug:
                if abs(event.diLepton.svfitMass()-svfit.getMass()) > 0.01:
                    print 'WARNING: run {RUN}, lumi {LUMI}, event {EVT}'.format(RUN=str(event.run),
                                                                                LUMI=str(event.lumi),
                                                                                EVT=str(event.eventId))
                    print 'precomputed svfit mass   ', event.diLepton.svfitMass()
                    print 'svfit mass computed here ', svfit.getMass()

        # method override
        event.diLepton.svfitMass = svfit.getMass
    