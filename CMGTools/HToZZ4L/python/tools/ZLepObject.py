from ROOT import TLorentzVector
from CMGTools.HToZZ4L.tools.DiObject import DiObject

class ZLepObject( TLorentzVector ):
    '''Class used for Z+1l'''
    def __init__(self, z, l):
        super(ZLepObject, self).__init__(z + TLorentzVector(l.px(), l.py(), l.pz(), l.energy()))
        self.z    = z
        self.lep3 = l

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

    def sortedPtLeg(self,N):
        ''' Gives the Nth highest pt lepton. 0 is the highest'''
        leptons=sorted([self.z.leg1, \
                        self.z.leg2, \
                        self.lep3], \
                       key=lambda x: x.pt(), \
                       reverse=True)
        return leptons[N]
        
    def charge(self):
        return self.z.charge() + self.lep3.charge()

    def minPairMll(self, onlyOS=False):
        pairs=[(self.z.leg1,self.z.leg2),
               (self.z.leg1,self.lep3),
               (self.z.leg2,self.lep3)]
        if onlyOS:
            pairs = filter(lambda (x,y): x.charge()!=y.charge(), pairs)
            if not pairs: return 999.
        return min(map(lambda (x,y) : (x.p4() + y.p4()).M(), pairs))

    def __str__(self):
        return ', '.join( ['ZLepObject:', str(self.z), str(self.lep3)] )

    def hasFSR(self):
        return self.z.hasFSR() or len(self.lep3.ownFsrPhotons)

    def daughterLeptons(self):
        return [self.z.leg1,self.z.leg2,self.lep3]

    def daughterPhotons(self):
        return self.z.daughterPhotons()+self.lep3.ownFsrPhotons
        
