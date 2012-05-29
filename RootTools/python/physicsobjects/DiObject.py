import math

from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Tau, Electron
from CMGTools.RootTools.utils.DeltaR import deltaR2

class DiObject( object ):
    
    def __init__(self, diobject):
        self.diobject = diobject
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
        header = 'DiObject: mvis=%3.2f, mT=%3.2f, pZeta=%3.2f, sumpT=%3.2f' \
                 % (self.diobject.mass(),
                    self.diobject.mTLeg2(),
                    self.diobject.pZeta(),
                    self.sumPt() )
        return '\n'.join( [header] )



class DiMuon( DiObject ):

    def __init__(self, diobject):
        super(DiMuon, self).__init__(diobject)
        self.mu1 = Muon( diobject.leg1() )
        self.mu2 = Muon( diobject.leg2() )

    def __str__(self):
        header = 'DiMuon: mvis=%3.2f, sumpT=%3.2f' \
                 % (self.diobject.mass(),
                    self.sumPt() )
        return '\n'.join( [header] )

class DiElectron( DiObject ):

    def __init__(self, diobject):
        super(DiElectron, self).__init__(diobject)
        self.mu1 = Electron( diobject.leg1() )
        self.mu2 = Electron( diobject.leg2() )

    def __str__(self):
        header = 'DiElectron: mvis=%3.2f, sumpT=%3.2f' \
                 % (self.diobject.mass(),
                    self.sumPt() )
        return '\n'.join( [header] )


class DiTau( DiObject ):
    def __init__(self, diobject):
        super(DiTau, self).__init__(diobject)
        
    def match(self, genParticles):
        #TODO review matching algorithm
        #TODO move matching stuff even higher?
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


class TauMuon( DiTau ):
    '''Holds a CMG TauMuon, and the 2 legs as a python Tau and Muon'''
    def __init__(self, diobject):
        super(TauMuon, self).__init__(diobject)
        self.tau = Tau( diobject.leg1() )
        self.mu = Muon( diobject.leg2() )

    def leg1(self):
        return self.tau

    def leg2(self):
        return self.mu



class TauElectron( DiTau ):
    def __init__(self, diobject):
        super(TauElectron, self).__init__(diobject)
        self.tau = Tau( diobject.leg1() )
        self.ele = Electron( diobject.leg2() )

    def leg1(self):
        return self.tau

    def leg2(self):
        return self.ele


class MuonElectron( DiTau ):
    def __init__(self, diobject):
        super(MuonElectron, self).__init__(diobject)
        self.mu = Muon( diobject.leg1() )
        self.ele = Electron( diobject.leg2() )

    def leg1(self):
        return self.mu

    def leg2(self):
        return self.ele

class TauTau( DiTau ):
    def __init__(self, diobject):
        super(TauTau, self).__init__(diobject)
        self.tau = Tau( diobject.leg1() )
        self.tau2 = Tau( diobject.leg2() )

    def leg1(self):
        return self.tau

    def leg2(self):
        return self.tau2
    

