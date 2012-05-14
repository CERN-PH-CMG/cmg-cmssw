from ROOT import TLorentzVector
from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
class DiObjectPair( TLorentzVector ):
    '''Class used for A->VV'''
    def __init__(self, leg1, leg2,leg3,leg4):

        a=DiObject(leg1,leg2)
        b=DiObject(leg3,leg4)
        lv=a+b
        super( DiObjectPair, self).__init__(lv)
        self.leg1=a
        self.leg2=b

    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        # changing the first letter of the function name to upper case. 
        capName = ''.join( [name[0].capitalize(), name[1:]] ) 
        return getattr( self, capName )


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

    def PdgId(self):
        '''Dummy, needed to fill the tree'''
        return 24



    def sortedMassPairs(self,onlyOS = False):
        pairs=[self.leg1,self.leg2, \
               DiObject(self.leg1.leg1,self.leg2.leg1),
               DiObject(self.leg1.leg1,self.leg2.leg2),
               DiObject(self.leg1.leg2,self.leg2.leg1),
               DiObject(self.leg1.leg2,self.leg2.leg2)]
        if onlyOS:
            pairs=filter(lambda x: x.charge==0,pairs)

        pairs=sorted(pairs,key=lambda x: x.mass())
        return pairs

    def minPairMass(self):
        sortedPairs=self.sortedMassPairs()
        return sortedPairs[0].mass()

    def minOSPairMass(self):
        sortedPairs=self.sortedMassPairs(True)
        if len(sortedPairs)>0:
            return sortedPairs[0].mass()
        else:
            return 999.

    def __str__(self):
        return ', '.join( ['DiObjectPair:', str(self.leg1), str(self.leg2)] )

        ###MELA#########################################


        ###MELA#########################################

        
