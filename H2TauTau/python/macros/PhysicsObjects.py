class DiTau:
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

class Lepton:
    '''Extends the cmg::Lepton functionalities.'''
    
    def __init__(self, lepton):
        self.lepton = lepton
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.lepton, name)

    def __str__(self):
        return 'lepton: {pdgId}, pt = {pt:5.1f}, eta = {eta:3.2f}'.format( pdgId = self.pdgId(),
                                                                           pt = self.pt(),
                                                                           eta = self.eta() )

Jet = Lepton


def charge0( particles ):
    '''filter a list of particles for the ones with charge 0.'''
    oppCharge = [particle for particle in particles if particle.charge()==0]



def bestDiTau( diTaus ):
    '''returns the diTau with the max sumPt'''
    best = max( diTaus, key=DiTau.sumPt) 
    return best 


