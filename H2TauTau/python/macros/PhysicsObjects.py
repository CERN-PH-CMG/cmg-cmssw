import math

from ROOT import TLorentzVector

from CMGTools.H2TauTau.macros.DeltaR import deltaR2

class DiTau(object):
    '''Extends the cmg::DiTau functionalities.'''
    def __init__(self, ditau):
        self.ditau = ditau
        #p4 = LorentzVector( 1,0,0,1)
        # self.ditau.setP4(p4)
        self.leg1Gen = None
        self.leg2Gen = None
        self.leg1DeltaR = -1
        self.leg2DeltaR = -1
         
    def sumPt(self):
        '''pt_leg1 + pt_leg2. used for finding the best DiTau.'''
        return self.leg1().pt() + self.leg2().pt()

    def match(self, genParticles):
        # print self
        genTaus = []
        ZorPhoton = [22, 23]
        for gen in genParticles:
            # print '\t', gen
            if abs(gen.pdgId())==15 and gen.mother().pdgId() in ZorPhoton:
                genTaus.append( gen )
        # print 'Gen taus: '
        # print '\n'.join( map( str, genTaus ) )
        if len(genTaus)!=2:
            #COLIN what about WW, ZZ? 
            return (-1, -1)
        else:
            dR2leg1Min, self.leg1Gen = ( float('inf'), None)
            dR2leg2Min, self.leg2Gen = ( float('inf'), None) 
            for genTau in genTaus:
                dR2leg1 = deltaR2(self.leg1().eta(), self.leg1().phi(),
                                  genTau.eta(), genTau.phi() )
                dR2leg2 = deltaR2(self.leg2().eta(), self.leg2().phi(),
                                  genTau.eta(), genTau.phi() )
                if dR2leg1 <  dR2leg1Min:
                    dR2leg1Min, self.leg1Gen = (dR2leg1, genTau)
                if dR2leg2 <  dR2leg2Min:
                    dR2leg2Min, self.leg2Gen = (dR2leg2, genTau)
            # print dR2leg1Min, dR2leg2Min
            # print self.leg1Gen
            # print self.leg2Gen
            self.leg1DeltaR = math.sqrt( dR2leg1Min )
            self.leg2DeltaR = math.sqrt( dR2leg2Min )
            return (self.leg1DeltaR, self.leg2DeltaR)
        
        
        
    def __getattr__(self, name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.ditau, name)

    def __str__(self):
        return 'ditau: mvis=%3.2f, mT=%3.2f, pZeta=%3.2f, sumpT=%3.2f' % (self.ditau.mass(),
                                                                          self.ditau.mTLeg2(),
                                                                          self.ditau.pZeta(),
                                                                          self.sumPt() ) 
        
class PhysicsObject(object):
    '''Extends the cmg::PhysicsObject functionalities.'''

    def __init__(self, physObj):
        self.physObj = physObj
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.physObj, name)

    def __str__(self):
        tmp = '{className}: {pdgId:>8}, pt = {pt:5.1f}, eta = {eta:5.2f}, phi = {phi:5.2f}'
        return tmp.format( className = self.__class__.__name__,
                           pdgId = self.pdgId(),
                           pt = self.pt(),
                           eta = self.eta(),
                           phi = self.phi() )
    
class Tau( PhysicsObject ):
    
    def __init__(self, tau):
        self.tau = tau
        super(Tau, self).__init__(tau)
        self.eOverP = None
        
    def calcEOverP(self):
        if self.eOverP is not None:
            return self.eOverP
        self.leadChargedEnergy = self.tau.leadChargedHadrECalEnergy() \
                                 + self.tau.leadChargedHadrHCalEnergy()
        self.leadChargedMomentum = self.tau.leadChargedHadrPt() / math.sin(self.tau.theta())
        self.eOverP = self.leadChargedEnergy / self.leadChargedMomentum
        return self.eOverP
        
    def __str__(self):
        lep = super(Tau, self).__str__()
        spec = '\ttau: eOverP = {eOverP:4.2f}'.format(eOverP = self.calcEOverP() )
        return '\n'.join([lep, spec])


Jet = PhysicsObject
Jet.__name__ = 'Jet'

Lepton = PhysicsObject
Lepton.__name__ = 'Lepton'

GenParticle = PhysicsObject
GenParticle.__name__ = 'GenParticle'


def isTau(leg):
    '''Duck-typing a tau'''
    try:
        leg.leadChargedHadrPt()
    except AttributeError:
        return False
    return True



def bestDiTau( diTaus ):
    '''returns the diTau with the max sumPt'''
    best = max( diTaus, key=DiTau.sumPt) 
    return best 


