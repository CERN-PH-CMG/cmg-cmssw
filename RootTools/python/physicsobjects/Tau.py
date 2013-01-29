from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.TauDecayModes import tauDecayModes


class Tau( Lepton ):
    
    def __init__(self, tau):
        self.tau = tau
        super(Tau, self).__init__(tau)
        self.eOverP = None

    def calcEOverP(self):
        if self.eOverP is not None:
            return self.eOverP
        self.leadChargedEnergy = self.tau.leadChargedHadrEcalEnergy() \
                                 + self.tau.leadChargedHadrHcalEnergy()
        # self.leadChargedMomentum = self.tau.leadChargedHadrPt() / math.sin(self.tau.theta())
        self.leadChargedMomentum = self.tau.leadPFChargedHadrCand().energy()
        self.eOverP = self.leadChargedEnergy / self.leadChargedMomentum
        return self.eOverP         

    def relIso(self, dummy1, dummy2):
        '''Just making the tau behave as a lepton.'''
        return -1

    def mvaId(self):
        '''For a transparent treatment of electrons, muons and taus. Returns -99'''
        return -99

    def dxy(self, vertex=None):
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrVertex();   
        p4 = self.p4();
        return ( - (vtx.x()-vertex.position().x()) *  p4.y()
                 + (vtx.y()-vertex.position().y()) *  p4.x() ) /  p4.pt();    

    def dz(self, vertex=None):
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrVertex();   
        p4 = self.p4();        
        return  (vtx.z()-vertex.position().z()) - ((vtx.x()-vertex.position().x())*p4.x()+(vtx.y()-vertex.position().y())*p4.y())/ p4.pt() *  p4.z()/ p4.pt();
    
    def __str__(self):
        lep = super(Tau, self).__str__()
        spec = '\tTau: decay = {decMode:<15}, eOverP = {eOverP:4.2f}'.format(
            decMode = tauDecayModes.intToName( self.decayMode() ),
            eOverP = self.calcEOverP()
            )
        return '\n'.join([lep, spec])


def isTau(leg):
    '''Duck-typing a tau'''
    try:
        leg.leadChargedHadrPt()
    except AttributeError:
        return False
    return True

