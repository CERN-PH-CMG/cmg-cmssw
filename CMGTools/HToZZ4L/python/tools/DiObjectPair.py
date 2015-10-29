from ROOT import TLorentzVector
from CMGTools.HToZZ4L.tools.DiObject import DiObject
class DiObjectPair( TLorentzVector ):
    '''Class used for A->VV'''
    def __init__(self, leg1, leg2,leg3,leg4):

        a=DiObject(leg1,leg2, doSort=False)
        b=DiObject(leg3,leg4, doSort=False)
        lv=a+b
        super( DiObjectPair, self).__init__(lv)
        self.leg1=a
        self.leg2=b


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
        leptons=sorted([self.leg1.leg1, \
                        self.leg1.leg2, \
                        self.leg2.leg1, \
                        self.leg2.leg2], \
                       key=lambda x: x.pt(), \
                       reverse=True)
        return leptons[N]
        
    def charge(self):
        return self.leg1.charge() + self.leg2.charge()

    def sortedMassPairs(self, onlyOS = False):
        pairs=[DiObject(self.leg1.leg1,self.leg1.leg2),
               DiObject(self.leg2.leg1,self.leg2.leg2),
               DiObject(self.leg1.leg1,self.leg2.leg1),
               DiObject(self.leg1.leg1,self.leg2.leg2),
               DiObject(self.leg1.leg2,self.leg2.leg1),
               DiObject(self.leg1.leg2,self.leg2.leg2)]
        if onlyOS:
            pairs=filter(lambda x: x.charge()==0,pairs)
        pairs=sorted(pairs,key=lambda x: x.mass())
        return pairs

    def minPairMass(self):
        return self.sortedMassPairs().mass()

    def minOSPairMass(self):
        sortedPairs=self.sortedMassPairs(True)
        if len(sortedPairs)>0:
            return sortedPairs[0].mass()
        else:
            return 999.

    def minPairMll(self, onlyOS=False):
        pairs=[(self.leg1.leg1,self.leg1.leg2),
               (self.leg2.leg1,self.leg2.leg2),
               (self.leg1.leg1,self.leg2.leg1),
               (self.leg1.leg1,self.leg2.leg2),
               (self.leg1.leg2,self.leg2.leg1),
               (self.leg1.leg2,self.leg2.leg2)]
        if onlyOS:
            pairs = filter(lambda (x,y): x.charge()!=y.charge(), pairs)
            if not pairs: return 999.
        return min(map(lambda (x,y) : (x.p4() + y.p4()).M(), pairs))

    def __str__(self):
        return ', '.join( ['DiObjectPair:', str(self.leg1), str(self.leg2)] )

    def hasFSR(self):
        return self.leg1.hasFSR() or self.leg2.hasFSR()

    def daughterLeptons(self):
        return [self.leg1.leg1,self.leg1.leg2,self.leg2.leg1,self.leg2.leg2]

    def daughterPhotons(self):
        return self.leg1.daughterPhotons()+self.leg2.daughterPhotons()
        
