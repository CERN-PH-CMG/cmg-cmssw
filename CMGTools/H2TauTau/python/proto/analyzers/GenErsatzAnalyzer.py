from PhysicsTools.Heppy.analyzers.core.Analyzer    import Analyzer
from PhysicsTools.Heppy.physicsobjects.GenParticle import GenParticle

class Boson(GenParticle):
    def __init__(self, boson):
        super(Boson, self).__init__(boson)
        self.leg1 = GenParticle(boson.daughter(0))
        self.leg2 = GenParticle(boson.daughter(1))

    def __str__(self):
        header = super(Boson, self).__str__()
        return '\n'.join( [header,
                           '\t'+str(self.leg1),
                           '\t'+str(self.leg2)] )

class ZBoson(Boson):
    pass

class WBoson(Boson):
    def __init__(self, boson):
        super(WBoson, self).__init__(boson)
        self.lep = self.leg1
        self.nu = self.leg2
        # in case of W, neutrino supposed to be on leg2
        assert( abs(self.leg1.pdgId()) not in [12,14,16]) 
        

class GenErsatzAnalyzer( Analyzer ):

    def process(self, event):
        # event.W = None
        # event.Z = None
        if not self.cfg_comp.isMC:
            return True
        result = super(GenErsatzAnalyzer, self).process(event)
        
        event.genZs = [ZBoson(part) for part in event.genVBosons if     part.pdgId() ==23]
        event.genWs = [WBoson(part) for part in event.genVBosons if abs(part.pdgId())==24]

        if self.cfg_ana.verbose:
            print 'W Bosons:'
            for b in wbosons:
                print b
            print 'Z Bosons:'
            for b in zbosons:
                print b
                
##         filter(lambda x: x.status()==3 and x.pdgId()==23, event.genParticles)
##         wbosons = filter(lambda x: x.status()==3 and abs(x.pdgId())==24, event.genParticles)
##         print map(str, wbosons)
