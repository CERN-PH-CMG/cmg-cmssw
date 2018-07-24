from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from PhysicsTools.Heppy.physicsutils.TauDecayModes import tauDecayModes
from PhysicsTools.Heppy.physicsutils.TauMVAID import tau_mvaid_2017 as tau_mvaid

import math

# Find all tau IDs here: 
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePFTauID#Tau_ID_2014_preparation_for_AN1
# and the names by which they are accessible with tau.tauId(...) here
# http://cmslxr.fnal.gov/lxr/source/PhysicsTools/PatAlgos/python/producersLayer1/tauProducer_cfi.py

class Tau(Lepton):
    
    def __init__(self, tau):
        self.tau = tau
        super(Tau, self).__init__(tau)
        self._mvaid_score = None         #where 2017 MVAID score should be stored to not recompute everytime it is needed
        self._mvaid_normscore = None     #where 2017 MVAID normscore should be stored to not recompute everytime WPs are needed
        
    def relIso(self, dBetaFactor=0, allCharged=0):
        '''Just making the tau behave as a lepton, with dummy parameters.'''
        return -1

    def relIsoR(self, R=0.3, dBetaFactor=0, allCharged=0):
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
        return self._mvaid_normscore > tau_mvaid.threshold(self.pt(), working_point)

    def dxy_approx(self, vertex=None):
        '''Returns standard dxy for an arbitrary passed vertex'''
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrCand().vertex()
        p4 = self.p4()
        return ( - (vtx.x()-vertex.position().x()) *  p4.y()
                 + (vtx.y()-vertex.position().y()) *  p4.x() ) /  p4.pt()

    def countWP(self,name, allWP=False):
        '''Returns the number of Working Points 
        that are passed for given isolation name. 
        '''
        WPs = ['VVTight', 'VTight', 'Tight', 'Medium',
               'Loose', 'VLoose', 'VVLoose', ]
        testname = 'by{}'+name[2:]
        n_WP = 0
        for WP in WPs:
            
            # if from heppy-implemented isolation (known list of WPs)
            if allWP:
                if not self.tauID(testname.format(WP)):
                    n_WP += 1 # in this case n_WP is the number of not passed WP
                else:
                    return len(WPs) - n_WP #stops counting when reached a passed WP

            # else all WPs will be tested from MINIAOD
            elif self.physObj.isTauIDAvailable(testname.format(WP)):
                if self.tauID(testname.format(WP)):
                    n_WP += 1 # in this case n_WP is the number of passed WP

        return n_WP

    def tauID(self, name):
        '''Returns heppy redefined score if fitting one 
        of implemented names, else calls the wrapped tau 
        tauID(name) function.
        '''
        if 'IsolationMVArun2017v2DBoldDMwLT' in name:
            #if raw score
            if name == 'byIsolationMVArun2017v2DBoldDMwLTraw2017':
                if self._mvaid_score is None:
                    self._mvaid_score = self.mva_score()
                return self._mvaid_score

            #if WPs
            if self._mvaid_normscore is None:
                self._mvaid_normscore = self.mva_score(norm=True)

            if name == 'byVVLooseIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("VVLoose") else 0.
            elif name == 'byVLooseIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("VLoose") else 0.
            elif name == 'byLooseIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("Loose") else 0.
            elif name == 'byMediumIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("Medium") else 0.
            elif name == 'byTightIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("Tight") else 0.
            elif name == 'byVTightIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("VTight") else 0.
            elif name == 'byVVTightIsolationMVArun2017v2DBoldDMwLT2017':
                return 1. if self.mva_passes("VVTight") else 0.

            #if numbering scheme
            if ('byIsolation' in name) and ('raw' not in name):
                return self.countWP(name, allWP=True)

        if ('byIsolation' in name) and ('raw' not in name):
            return self.countWP(name)

        return self.physObj.tauID(name)

    def dxy(self, vertex=None):
        '''More precise dxy calculation as pre-calculated in the tau object
        for the primary vertex it was constructed with.
        Returns standard dxy calculation if the passed vertex differs from the
        one in the tau object.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        # x/y/z are directly saved in the tau object instead of a reference to 
        # the PV
        if abs(vertex.z() - self.vertex().z()) < 0.0001:
            return self.physObj.dxy()
        else:
            return self.dxy_approx(vertex)

    def dz(self, vertex=None):
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

