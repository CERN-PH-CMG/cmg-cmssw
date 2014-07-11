from CMGTools.RootTools.physicsobjects.Lepton import Lepton
import math

class IsoTrack( Lepton ):
    
    def __init__(self, isoTrack):
        self.isoTrack = isoTrack
        super(IsoTrack, self).__init__(isoTrack)

    def relIso(self, dummy1, dummy2):
        '''Just making the tau behave as a lepton.'''
        return -1

    def relIso(self, dummy1, dummy2):
        '''Just making the tau behave as a lepton.'''
        return -1

    def mvaId(self):
        '''For a transparent treatment of electrons, muons and taus. Returns -99'''
        return -99

    def dxy(self, vertex=None):
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.vertex();  # FIXME 
        p4 = self.p4();
        return ( - (vtx.x()-vertex.position().x()) *  p4.y()
                 + (vtx.y()-vertex.position().y()) *  p4.x() ) /  p4.pt();    

    def dz(self, vertex=None):
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.vertex();  # FIXME 
        p4 = self.p4();        
        return  (vtx.z()-vertex.position().z()) - ((vtx.x()-vertex.position().x())*p4.x()+(vtx.y()-vertex.position().y())*p4.y())/ p4.pt() *  p4.z()/ p4.pt();

    def __str__(self):
        lep = super(IsoTrack, self).__str__()
        return lep
        #spec = '\t\tTau: decay = {decMode:<15}, eOverP = {eOverP:4.2f}, isoMVALoose = {isoMVALoose}'.format(
        #    decMode = tauDecayModes.intToName( self.decayMode() ),
        #    eOverP = self.calcEOverP(),
        #    isoMVALoose = self.tauID('byLooseIsoMVA')
        #    )
        #return '\n'.join([lep, spec])


