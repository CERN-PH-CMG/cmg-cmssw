import random
import math
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
import PhysicsTools.HeppyCore.framework.config as cfg





class PackedCandidateLoader( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(PackedCandidateLoader,self).__init__(cfg_ana, cfg_comp, looperName)
        if hasattr(cfg_ana,'cut'):
            self.select = cfg_ana.cut
        else:
            self.select = None
            
    def declareHandles(self):
        super(PackedCandidateLoader, self).declareHandles()
        self.handles['packed'] = AutoHandle( 'packedPFCandidates', 'std::vector<pat::PackedCandidate>' )

    def process(self, event):
        self.readCollections( event.input )
        cands = self.handles['packed'].product()
        if self.select==None:
            event.packedCandidatesForJets = cands
        else:    
            event.packedCandidatesForJets = filter(self.select,cands)

        


                
                
