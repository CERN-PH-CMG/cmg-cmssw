import math

from ROOT import TLorentzVector

class DiTau(object):
    '''Extends the cmg::DiTau functionalities.'''
    def __init__(self, ditau):
        self.ditau = ditau
        #p4 = LorentzVector( 1,0,0,1)
        # self.ditau.setP4(p4)

    def sumPt( self ):
        '''pt_leg1 + pt_leg2. used for finding the best DiTau.'''
        return self.leg1().pt() + self.leg2().pt()

    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.ditau, name)

    def __str__(self):
        return 'ditau: mvis=%3.2f, mT=%3.2f, pZeta=%3.2f, sumpT=%3.2f' % (self.ditau.mass(),
                                                                          self.ditau.mTLeg2(),
                                                                          self.ditau.pZeta(),
                                                                          self.sumPt() ) 
        

class Lepton(object):
    '''Extends the cmg::Lepton functionalities.'''
    
    def __init__(self, lepton):
        self.lepton = lepton
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.lepton, name)

    def __str__(self):
        tmp = 'lepton: {pdgId}, pt = {pt:5.1f}, eta = {eta:3.2f}'
        return tmp.format( pdgId = self.pdgId(),
                           pt = self.pt(),
                           eta = self.eta() )
    
class Tau( Lepton ):
    
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


#COLIN: Hehe ^^  just playing around will need a real Jet class at some point
Jet = Lepton

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


