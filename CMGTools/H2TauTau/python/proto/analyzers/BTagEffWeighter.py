import re

from ROOT import TFile

from CMGTools.RootTools.analyzers.GenParticleAnalyzer import *
from PhysicsTools.HeppyCore.utils.deltar import matchObjectCollection
from CMGTools.RootTools.physicsobjects.genutils import *
from PhysicsTools.HeppyCore.statistics.average import Average

from CMGTools.H2TauTau.proto.BTagEfficiency import BTagEfficiency, BTagWeight

class BTagEffWeighter( GenParticleAnalyzer ):
    '''Weight the event to get the NLO Higgs pT distribution for ggH events

       btagEffAna = cfg.Analyzer(
          'BTagEffWeighter',
          workingPoint = 0.679, 
          verbose = True
          )
    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(BTagEffWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

    self.btagWeight_ = BTagWeight()
    self.btagEff_ = BTagEfficiency()


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


    def declareHandles(self):
        super(VBFAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol,
                                           'std::vector<cmg::PFJet>' )


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


    def beginLoop(self, setup):
        print self, self.__class__
        super(BTagEffWeighter,self).beginLoop(setup)
        self.averages.add('btagEffWeight', Average('btagEffWeight') )


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def process(self, event):
        super(BTagEffWeighter, self).process(event)
        
        btagEffWeight_ = 1 # this is a float
 
        # loop over jets
        self.btagWeight_.BTagWeightPy.cleanVector()
        cmgJets = self.handles['jets'].product()
        for jet in cmgJets :
            if abs (jet.eta()) > 2.4:
                continue

            jetpt = jet.pt()
            jeteta = jet.eta()
            jetflavor = abs (jet.partonFlavour())
            discr = self.workingPoint
            eff = 1.
            sf = 1.
            if jetflavor == 5 :
                eff = self.btagEff_.BTagEfficiencyPy.btagEFF(discr,1)
                sf =  self.btagEff_.BTagEfficiencyPy.btagSF(jetpt,1)
            elif jetflavor == 4 :
                eff = self.btagEff_.BTagEfficiencyPy.btagEFF(discr,0)
                sf =  self.btagEff_.BTagEfficiencyPy.btagSF(jetpt,0)
            else :
                eff = self.btagEff_.BTagEfficiencyPy.mistagEFF(jetpt,jeteta)
                sf =  self.btagEff_.BTagEfficiencyPy.mistagSF(jetpt,jeteta)

            self.btagWeight_.BTagWeightPy.addJet(eff,sf)
           
        btagEffWeight_ = self.btagWeight_.BTagWeightPy.weight()
        event.btagEffWeight = btagEffWeight_
       
        self.averages['btagEffWeight'].add( event.btagEffWeight )
        if self.cfg_ana.verbose:
            print 'BTagEffWeighter:',event.btagEffWeight
        return True

