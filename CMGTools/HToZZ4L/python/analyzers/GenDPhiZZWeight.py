from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaPhi


       
class GenDPhiZZWeight( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(GenDPhiZZWeight,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(GenDPhiZZWeight, self).declareHandles()

    def beginLoop(self, setup):
        super(GenDPhiZZWeight,self).beginLoop(setup)
        self.mchandles['genParticles'] = AutoHandle( 'prunedGenParticles', 'std::vector<reco::GenParticle>' )

    def process(self, event):
        event.genDPhiZZ = -999
        event.genDPhiZZWeight = 1
        event.genDPhiZZSameFlavour = -1

        if not self.cfg_comp.isMC: return True
        self.readCollections( event.input )

        zs = [ gp for gp in self.mchandles['genParticles'].product() if abs(gp.pdgId()) == 23 and 20 <= gp.status() and gp.status() <= 30 ]
        if len(zs) == 2:
            event.genDPhiZZ = abs(deltaPhi(zs[0].phi(), zs[1].phi()))
            dau = zs[:]
            for i in 0,1:
                while abs(dau[i].pdgId()) == 23:
                    found = False
                    for idau in xrange(dau[i].numberOfDaughters()):
                        ddau = dau[i].daughter(idau)
                        if abs(ddau.pdgId()) in (11,13,15):
                            dau[i] = ddau
                            found = True
                            break
                    if not found:
                        for idau in xrange(dau[i].numberOfDaughters()):
                            ddau = dau[i].daughter(idau)
                            if abs(ddau.pdgId()) == 23:
                                dau[i] = ddau
                                found = True
                                break
                    if not found: break
            if abs(dau[0].pdgId()) in (11,13,15) and abs(dau[1].pdgId()) in (11,13,15):
                event.genDPhiZZSameFlavour = (1 if abs(dau[0].pdgId()) == abs(dau[1].pdgId()) else 0)
        return True
        
