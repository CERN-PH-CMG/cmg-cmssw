import copy
from ROOT import Math

#instanciating template
PtEtaPhiE4DLV = Math.PtEtaPhiE4D(float)
PtEtaPhiM4DLV = Math.PtEtaPhiM4D(float)

class PhysicsObject(object):
    '''Extends the cmg::PhysicsObject functionalities.'''

    def __init__(self, physObj):
        self.physObj = physObj

    def scaleEnergy( self, scale ):
        p4 = self.physObj.p4()
        p4 *= scale 
        self.physObj.setP4( p4 )  
##         p4 = self.physObj.polarP4()
##         sp4 = PtEtaPhiE4DLV(
##             p4.Pt()*scale,
##             p4.Eta(),
##             p4.Phi(),
##             p4.E()
##             )
##         sp4.SetE( sp4.E()*scale )
##         ptepmsp4 = PtEtaPhiM4DLV(
##             sp4.Pt(),
##             sp4.Eta(),
##             sp4.Phi(),
##             sp4.M()
##             )
##         self.physObj.setP4( p4.__class__(
##             sp4.Pt(),
##             sp4.Eta(),
##             sp4.Phi(),
##             sp4.M()
##             ) )
        
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.physObj, name)

    def __str__(self):
        tmp = '{className} : {pdgId:>3}, pt = {pt:5.1f}, eta = {eta:5.2f}, phi = {phi:5.2f}'
        return tmp.format( className = self.__class__.__name__,
                           pdgId = self.pdgId(),
                           pt = self.pt(),
                           eta = self.eta(),
                           phi = self.phi() )
