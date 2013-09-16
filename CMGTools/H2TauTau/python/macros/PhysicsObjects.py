import math
import operator

from ROOT import TLorentzVector

from CMGTools.H2TauTau.macros.DeltaR import deltaR2
from CMGTools.H2TauTau.macros.TauDecayModes import tauDecayModes

class DiLepton(object):
    def __init__(self, diobject):
        self.diobject = diobject
        self.lepton1 = Lepton( diobject.leg1() )
        self.lepton = Lepton( diobject.leg2() )
        #p4 = LorentzVector( 1,0,0,1)
        # self.diobject.setP4(p4)
        self.leg1Gen = None
        self.leg2Gen = None
        self.leg1DeltaR = -1
        self.leg2DeltaR = -1
        
    def sumPt(self):
        '''pt_leg1 + pt_leg2. used for finding the best DiTau.'''
        return self.leg1().pt() + self.leg2().pt()
    
    def __getattr__(self, name):
        '''all accessors  from cmg::DiObject are transferred to this class.'''
        return getattr(self.diobject, name)

    def __str__(self):
        header = 'Diobject: mvis=%3.2f, mT=%3.2f, pZeta=%3.2f, sumpT=%3.2f' \
                 % (self.diobject.mass(),
                    self.diobject.mTLeg2(),
                    self.diobject.pZeta(),
                    self.sumPt() )
        lepton1 = str( self.lepton1 )
        lepton = str( self.lepton ) 
        return '\n'.join( [header, lepton1, lepton] )
   

class DiTau(object):
    '''Extends the cmg::DiTau functionalities.'''
    def __init__(self, ditau):
        self.ditau = ditau
        self.tau = Tau( ditau.leg1() )
        self.lepton = Lepton( ditau.leg2() )
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
        header = 'DiTau: mvis=%3.2f, mT=%3.2f, pZeta=%3.2f, sumpT=%3.2f' \
                 % (self.ditau.mass(),
                    self.ditau.mTLeg2(),
                    self.ditau.pZeta(),
                    self.sumPt() )
        tau = str( self.tau )
        lepton = str( self.lepton ) 
        return '\n'.join( [header, tau, lepton] )

        
class PhysicsObject(object):
    '''Extends the cmg::PhysicsObject functionalities.'''

    def __init__(self, physObj):
        self.physObj = physObj

    def scaleEnergy( self, scale ):
        p4 = self.physObj.p4()
        p4 *= scale 
        self.physObj.setP4( p4 )  
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.physObj, name)

    def __str__(self):
        tmp = '{className:<10}: {pdgId:>8}, pt = {pt:5.1f}, eta = {eta:5.2f}, phi = {phi:5.2f}'
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
        spec = '\tTau: decay = {decMode:<15}, eOverP = {eOverP:4.2f}'.format(
            decMode = tauDecayModes.intToName( self.decayMode() ),
            eOverP = self.calcEOverP()
            )
        return '\n'.join([lep, spec])



class Jet( PhysicsObject):
    pass

class Lepton( PhysicsObject):
    pass

class GenParticle( PhysicsObject):
    pass



def isTau(leg):
    '''Duck-typing a tau'''
    try:
        leg.leadChargedHadrPt()
    except AttributeError:
        return False
    return True


def bestDiTau( diTaus ):
    '''returns the diTau with the max sumPt'''
    best = max( diTaus, key=operator.methodcaller( 'sumPt' ) ) 
    return best 

