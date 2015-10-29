from ROOT import TLorentzVector
from math import pi,acos,asin
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi

class DiObject( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, leg1, leg2,doSort = True):
        if (leg2.pt() > leg1.pt()) and doSort:
            leg2, leg1 = leg1, leg2
        lv = leg1.p4WithFSR() + leg2.p4WithFSR()
        super( DiObject, self).__init__( lv.Px(), lv.Py(), lv.Pz(), lv.E() )
        self.leg1 = leg1
        self.leg2 = leg2

    def px(self):
         return self.Px()
    def py(self):
         return self.Py()
    def pz(self):
         return self.Pz()
    def energy(self):
         return self.Energy()

    def eta(self):
         return self.Eta()
    def phi(self):
         return self.Phi()

    def pt(self):
         return self.Pt()

    def mass(self):
         return self.M()

    def mll(self):
         return (self.leg1.p4() + self.leg2.p4()).M()

    def charge(self):
        return self.leg1.charge() + self.leg2.charge()
        
    def __str__(self):
        return ', '.join( ['DiObject:', str(self.leg1), str(self.leg2)] )

    def daughterLeptons(self):
        return [self.leg1,self.leg2]

    def daughterPhotons(self):
        return self.leg1.ownFsrPhotons + self.leg2.ownFsrPhotons

    def hasFSR(self):
        return self.daughterPhotons() != [] 

   
