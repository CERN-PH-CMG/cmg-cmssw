from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from PhysicsTools.Heppy.physicsutils.TauDecayModes import tauDecayModes
import math

# Find all tau IDs here: 
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePFTauID#Tau_ID_2014_preparation_for_AN1
# and the names by which they are accessible with tau.tauId(...) here
# http://cmslxr.fnal.gov/lxr/source/PhysicsTools/PatAlgos/python/producersLayer1/tauProducer_cfi.py

class Tau(Lepton):
    
    def __init__(self, tau):
        self.tau = tau
        super(Tau, self).__init__(tau)
        
    def relIso(self, dBetaFactor=0, allCharged=0):
        '''Just making the tau behave as a lepton, with dummy parameters.'''
        return -1

    def mvaId(self):
        '''For a transparent treatment of electrons, muons and taus. Returns -99'''
        return -99

    def mva_score(self, norm=False):
        '''returns the score of the isolation mva'''
        if norm:
            return tau_mvaid.score_norm(self)
        return tau_mvaid.score(self)

    def mva_passes(self, working_point):
        '''returns True if the tau passes the given working point of the isolation mva'''
        return tau_mvaid.passes(self, working_point)

    def dxy_approx(self, vertex=None):
        # TEMPORARY FIX: always use dxy/dz from leadChargedHadrCand as in NanoAOD, ignoring vertex
        return self.leadChargedHadrCand().dxy()
        '''Returns standard dxy for an arbitrary passed vertex'''
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrCand().vertex()
        p4 = self.p4()
        return ( - (vtx.x()-vertex.position().x()) *  p4.y()
                 + (vtx.y()-vertex.position().y()) *  p4.x() ) /  p4.pt()

    def tauID(self, name):
        '''Returns heppy redefined score if fitting one 
        of implemented names, else calls the wrapped tau 
        tauID(name) function.
        '''
        if name == 'byIsolationMVArun2017v2DBoldDMwLTraw2017':
            return self.mva_score()
        elif name == 'byVVLooseIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff95")
        elif name == 'byVLooseIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff90")
        elif name == 'byLooseIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff80")
        elif name == 'byMediumIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff70")
        elif name == 'byTightIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff60")
        elif name == 'byVTightIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff50")
        elif name == 'byVVTightIsolationMVArun2017v2DBoldDMwLT2017':
            return self.mva_passes("Eff40")
        return self.physObj.tauID(name)

    def dxy(self, vertex=None):
        '''More precise dxy calculation as pre-calculated in the tau object
        for the primary vertex it was constructed with.
        Returns standard dxy calculation if the passed vertex differs from the
        one in the tau object.
        '''
        # TEMPORARY FIX: always use dxy/dz from leadChargedHadrCand as in NanoAOD, ignoring vertex
        return self.leadChargedHadrCand().dxy()
        if vertex is None:
            vertex = self.associatedVertex
        # x/y/z are directly saved in the tau object instead of a reference to 
        # the PV
        if abs(vertex.z() - self.vertex().z()) < 0.0001:
            return self.physObj.dxy()
        else:
            return self.dxy_approx(vertex)

    def dz(self, vertex=None):
        # TEMPORARY FIX: always use dxy/dz from leadChargedHadrCand as in NanoAOD, ignoring vertex
        return self.leadChargedHadrCand().dz()
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrCand().vertex()
        p4 = self.p4()
        return  (vtx.z()-vertex.position().z()) - ((vtx.x()-vertex.position().x())*p4.x()+(vtx.y()-vertex.position().y())*p4.y())/ p4.pt() *  p4.z()/ p4.pt()
    
    def zImpact(self, vertex=None):
        '''z impact at ECAL surface'''
        if vertex is None:
            vertex = self.associatedVertex
        return vertex.z() + 130./math.tan(self.theta())

    def __str__(self):
        lep = super(Tau, self).__str__()
        spec = '\t\tTau: decay = {decMode:<15}'.format(
            decMode = tauDecayModes.intToName(self.decayMode())
        )
        return '\n'.join([lep, spec])


def isTau(leg):
    '''Duck-typing a tau'''
    try:
        # Method independently implemented in pat tau and PF tau
        leg.leadPFChargedHadrCandsignedSipt()
    except AttributeError:
        return False
    return True

