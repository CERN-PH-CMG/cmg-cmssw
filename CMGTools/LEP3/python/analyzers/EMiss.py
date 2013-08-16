from ROOT import TLorentzVector
from math import pi, acos

class EMiss ( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, jets = [] ):
        lv = TLorentzVector( 0., 0., 0., 240. )
        super( EMiss, self).__init__( lv )
        self.jets_ = []
        for jet in jets :
            self.add(jet)
        
    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        # changing the first letter of the function name to upper case. 
        capName = ''.join( [name[0].capitalize(), name[1:]] ) 
        # return getattr( super(DiObject, self), capName )
        return getattr( self, capName )

    def add(self,jet):
        self.jets_.append(jet)
        self.SetPxPyPzE(self.px()-jet.px(), self.py()-jet.py(), self.pz()-jet.pz(), self.e()-jet.energy())

    def jets(self):
        return self.jets_
    
    def __str__(self):
        return ', '.join( ['pTMiss, EMiss, mMiss, thetaMiss:', str(self.Pt()), str(self.E()), str(self.M()), str(acos(self.Pz()/self.P())*180./pi) ] )


class EVis ( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, jets = [] ):
        lv = TLorentzVector( 0., 0., 0., 0. )
        super( EVis, self).__init__( lv )
        self.jets_ = []
        for jet in jets :
            self.add(jet)
        
    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        # changing the first letter of the function name to upper case. 
        capName = ''.join( [name[0].capitalize(), name[1:]] ) 
        # return getattr( super(DiObject, self), capName )
        return getattr( self, capName )

    def add(self,jet):
        self.jets_.append(jet)
        self.SetPxPyPzE(self.px()+jet.px(), self.py()+jet.py(), self.pz()+jet.pz(), self.e()+jet.energy())

    def jets(self):
        return self.jets_
    
    def __str__(self):
        return ', '.join( ['pTVis, EVis, mVis, thetaVis:', str(self.Pt()), str(self.E()), str(self.M()), str(acos(self.Pz()/self.P())*180./pi) ] )


