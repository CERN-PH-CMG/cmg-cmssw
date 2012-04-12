from ROOT import TLorentzVector

class DiObject( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, leg1, leg2):
        if leg2.pt() > leg1.pt():
            leg2, leg1 = leg1, leg2
        lv1 = TLorentzVector( leg1.px(), leg1.py(), leg1.pz(), leg1.energy() )
        lv2 = TLorentzVector( leg2.px(), leg2.py(), leg2.pz(), leg2.energy() )
        lv1 += lv2 
        super( DiObject, self).__init__( lv1 )
        self.leg1 = leg1
        self.leg2 = leg2

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
        return 23

    def Sip3D(self):
        '''Dummy, needed to fill the tree'''
        return -1

    def RelIso(self, dBetaCor):
        '''Sum of the relative isolation (dbeta corrected) of the 2 legs'''
        return self.leg1.relIso( dBetaCor ) + self.leg2.relIso(dBetaCor )
        
    def charge(self):
        return self.leg1.charge() + self.leg2.charge()
        
    def __str__(self):
        return ', '.join( ['DiObject:', str(self.leg1), str(self.leg2)] )
