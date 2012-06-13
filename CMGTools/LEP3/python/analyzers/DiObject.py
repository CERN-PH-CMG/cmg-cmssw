from ROOT import TLorentzVector
from math import pi, acos

class DiObject ( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, leg1, leg2):
        if leg2.energy() > leg1.energy():
            leg2, leg1 = leg1, leg2
        lv1 = TLorentzVector( leg1.px(), leg1.py(), leg1.pz(), leg1.energy() )
        lv2 = TLorentzVector( leg2.px(), leg2.py(), leg2.pz(), leg2.energy() )
        lv1 += lv2 
        super( DiObject, self).__init__( lv1 )
        
        c1 = ( leg1.px()*leg2.px() + \
               leg1.py()*leg2.py() + \
               leg1.pz()*leg2.pz() ) / ( leg1.p() * leg2.p() )
        self.angle_ = acos(c1)*180./pi
        
        self.leg1 = leg1
        self.leg2 = leg2
        self.btag_ = []
        self.component_ = []
        for i in range(8):
            self.btag_.append ( max(self.leg1.btag(i),self.leg2.btag(i)) )
            self.component_.append ( Component(self,i) )
            
        self.nConstituents_ = self.leg1.nConstituents() + self.leg2.nConstituents()

    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        # changing the first letter of the function name to upper case. 
        capName = ''.join( [name[0].capitalize(), name[1:]] ) 
        # return getattr( super(DiObject, self), capName )
        return getattr( self, capName )

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
        self.SetPxPyPzE(p4.x(),p4.y(),p4.z(),p4.t())

    def p4(self):
        return self

    def angle(self):
        return self.angle_

    def __str__(self):
        return ', '.join( ['DiObject:', str(self.leg1), str(self.leg2)] )


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
