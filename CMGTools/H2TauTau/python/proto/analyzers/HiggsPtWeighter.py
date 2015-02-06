import re

from ROOT import TFile

# from CMGTools.RootTools.analyzers.GenParticleAnalyzer import GenParticleAnalyzer # RIC: no need to have our version 
from PhysicsTools.Heppy.analyzers.gen.GeneratorAnalyzer import GeneratorAnalyzer
from PhysicsTools.HeppyCore.utils.deltar import matchObjectCollection
# from PhysicsTools.Heppy.physicsutils.genutils import *
from PhysicsTools.HeppyCore.statistics.average import Average

# class HiggsPtWeighter( GenParticleAnalyzer ):
class HiggsPtWeighter( GeneratorAnalyzer ):
    '''Weight the event to get the NLO Higgs pT distribution for ggH events
    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):

        
        # RIC: parent class GeneratorAnalyzer sucks these parameters when being __init__
        # this is a sort of setting the defaults. Any nicer idea?
        if not hasattr( cfg_ana, 'stableBSMParticleIds'  ) : cfg_ana.stableBSMParticleIds  = []
        if not hasattr( cfg_ana, 'savePreFSRParticleIds' ) : cfg_ana.savePreFSRParticleIds = []
        if not hasattr( cfg_ana, 'makeAllGenParticles'   ) : cfg_ana.makeAllGenParticles   = True
        if not hasattr( cfg_ana, 'makeSplittedGenLists'  ) : cfg_ana.makeSplittedGenLists  = True # produces different collections for different particle types 
        if not hasattr( cfg_ana, 'allGenTaus'            ) : cfg_ana.allGenTaus            = True # if False, lep decaying taus are not included

        super(HiggsPtWeighter,self).__init__(cfg_ana, cfg_comp, looperName)
        
        self.rePatGGH = re.compile('HiggsGGH(\d+)')
        
        self.isGGH, self.higgsMass = self.isGGH(self.cfg_comp)
        if self.isGGH:
            self.higgsPtWeightFile = TFile("$CMSSW_BASE/src/CMGTools/H2TauTau/data/weight_ptH_{mass}_8TeV.root".format(mass=self.higgsMass))
            self.higgsPtWeightHistogram = self.higgsPtWeightFile.Get("Nominal")
            self.higgsPtWeightHistogramUp = self.higgsPtWeightFile.Get("Up")
            self.higgsPtWeightHistogramDown = self.higgsPtWeightFile.Get("Down")

        
    def isGGH(self, component):
        if self.cfg_comp.isMC:
            match = self.rePatGGH.match( self.cfg_comp.name )
            if not match:
                return False, -1
            else:
                mass = match.group(1)
                return True, mass
        else:
            return False, -1
        

    def beginLoop(self, setup):
        print self, self.__class__
        super(HiggsPtWeighter,self).beginLoop(setup)
        self.averages.add('weight', Average('weight') )

    def process(self, event):
        event.higgsPtWeight = 1 
        event.higgsPtWeightUp = 1 
        event.higgsPtWeightDown = 1 
        if not self.isGGH:
            return True
        
        result = super(HiggsPtWeighter, self).process(event)
        higgsBosons = [gen for gen in event.genParticles if gen.status()==3 and gen.pdgId()==25]
        if len(higgsBosons)!=1:
            strerr = '{nhiggs} Higgs bosons, this should not happen for a ggH component. Your component is:\n {comp}'.format(nhiggs=len(higgsBosons), comp=str(self.cfg_comp))
            raise ValueError(strerr)

        event.genHiggs = higgsBosons[0] 
        higgsPt = event.genHiggs.pt()
        event.higgsPtWeight = self.higgsPtWeightHistogram.GetBinContent(self.higgsPtWeightHistogram.FindBin(higgsPt))
        event.higgsPtWeightUp = self.higgsPtWeightHistogramUp.GetBinContent(self.higgsPtWeightHistogramUp.FindBin(higgsPt))
        event.higgsPtWeightDown = self.higgsPtWeightHistogramDown.GetBinContent(self.higgsPtWeightHistogramDown.FindBin(higgsPt))

        ### MULTIPLYING TO THE EVENT WEIGHT 
        event.eventWeight *= event.higgsPtWeight
        ### 
        
        self.averages['weight'].add( event.higgsPtWeight )
        if self.cfg_ana.verbose:
            print 'HiggsPtWeighter: pt_H={pt:6.2f}, weight={weight:6.2f}'.format(
                pt=higgsPt,
                weight=event.higgsPtWeight
            )
        return True

