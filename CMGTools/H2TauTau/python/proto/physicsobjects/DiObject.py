from CMGTools.H2TauTau.proto.physicsobjects.PhysicsObjects import Muon, Tau

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



class TauMuon( DiObject ):
    def __init__(self, diobject):
        super(TauMuon, self).__init__(diobject)
        self.tau = Tau( diobject.leg1() )
        self.mu = Muon( diobject.leg2() )

    def leg1(self):
        return self.tau

    def leg2(self):
        return self.mu
