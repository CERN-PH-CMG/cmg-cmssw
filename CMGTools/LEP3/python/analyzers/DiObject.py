from ROOT import TLorentzVector
from math import pi, acos
from ROOT import reco

class DiObject ( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, leg1, leg2):
        if leg2.energy() > leg1.energy():
            leg2, leg1 = leg1, leg2
        lv1 = TLorentzVector( leg1.px(), leg1.py(), leg1.pz(), leg1.energy() )
        lv2 = TLorentzVector( leg2.px(), leg2.py(), leg2.pz(), leg2.energy() )
        lv1 += lv2 
        super( DiObject, self).__init__( lv1 )
        self.p4_ = lv1
        
        c1 = ( leg1.px()*leg2.px() + \
               leg1.py()*leg2.py() + \
               leg1.pz()*leg2.pz() ) / ( leg1.p() * leg2.p() )
        #print c1
        if c1>=1.0: c1 = 1.-1E-12
        if c1<=-1.0: c1 = -1.+1E-12
        #print leg1, leg2, c1
        
        self.angle_ = acos(c1)*180./pi
        
        self.leg1 = leg1
        self.leg2 = leg2
        self.btag_ = []
        self.component_ = []
        # protect in case non standard object (as taus...) checking with hasattr
        for i in range(8):
            if hasattr(leg1,"btag"):
                self.btag_.append ( max(self.leg1.btag(i),self.leg2.btag(i)) )
            else:
                self.btag_.append ( -999.)
            if hasattr(leg1,"component"):    
                self.component_.append ( Component(self,i) )
            else:
                self.component_.append ( -999. )
                
        self.pdgId_ = 0
        if self.leg1.pdgId() != 0 : self.pdgId_ = self.leg1.pdgId()
        if self.leg2.pdgId() != 0 : self.pdgId_ = self.leg2.pdgId()
            
        if hasattr(leg1,"nConstituents"):
            self.nConstituents_ = self.leg1.nConstituents() + self.leg2.nConstituents()
        else:
            self.nConstituents_ = -999.

    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        # print 'calling getattr', name
        if name.lower() == 'mass':
            name = 'M'
        # changing the first letter of the function name to upper case. 
        capName = ''.join( [name[0].capitalize(), name[1:]] ) 
        return getattr( super(DiObject, self), capName )
        # return getattr( self, capName )

    def PdgId(self):
        '''Dummy, needed to fill the tree'''
        return 0

    def nConstituents(self):
        return self.nConstituents_

    def btag(self, i):
        return self.btag_[i]

    def component(self, i):
        return self.component_[i]

    def setP4(self,p4):
        self.SetPxPyPzE(p4.X(),p4.Y(),p4.Z(),p4.T())

    def p4(self):
        return reco.Candidate.LorentzVector(self.px(), self.py(),
                                            self.pz(), self.e())
        # return self
        #return TLorentzVector(self.px(), self.py(), self.pz(), self.e())

    def angle(self):
        return self.angle_

    def pdgId(self):
        return self.pdgId_

    def __str__(self):
        theStr = []
        theStr.append( '{className} : {pdgId:>3}, E = {energy:5.1f}, eta = {eta:5.2f}, theta = {theta:5.2f},  phi = {phi:5.2f}'.format(
            className = self.__class__.__name__,
            pdgId = self.pdgId(),
            energy = self.energy(),
            eta = self.eta(),
            theta = self.theta(),
            phi = self.phi() ))
        leg1lines = str(self.leg1).split('\n')
        for line in leg1lines:
            theStr.append('\t'+line)
        leg2lines = str(self.leg2).split('\n')
        for line in leg2lines:
            theStr.append('\t'+line)
        return '\n'.join( theStr )


class Component(object):

    def __init__(self, diobject, i):
        self.energy_ = diobject.leg1.component(i).energy() + diobject.leg2.component(i).energy()
        self.number_ = diobject.leg1.component(i).number() + diobject.leg2.component(i).number()
        self.fraction_ = self.energy_ / diobject.energy()
        self.pt_ = diobject.leg1.component(i).pt() + diobject.leg2.component(i).pt()

    def energy(self):
        return self.energy_

    def fraction(self):
        return self.fraction_

    def number(self):
        return self.number_

    def pt(self):
        return self.pt_
