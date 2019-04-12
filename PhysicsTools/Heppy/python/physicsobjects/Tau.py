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
        self._mvaid_score = None         #where 2017 MVAID score should be stored to not recompute everytime it is needed
        self._mvaid_normscore = None     #where 2017 MVAID normscore should be stored to not recompute everytime WPs are needed
        
    def relIso(self, dBetaFactor=0, allCharged=0):
        '''Just making the tau behave as a lepton, with dummy parameters.'''
        return -1

    def mvaId(self):
        '''For a transparent treatment of electrons, muons and taus. Returns -99'''
        return -99

    def mva_score(self, norm=False):
        '''returns the score of the isolation mva'''
        if norm:
            if self._mvaid_normscore is None:
                self._mvaid_normscore = tau_mvaid.score_norm(self)
            return self._mvaid_normscore
        if self._mvaid_score is None:
            self._mvaid_score = tau_mvaid.score(self)
        return self._mvaid_score

    def mva_passes(self, working_point):
        '''returns True if the tau passes the given working point of the isolation mva'''
        if self._mvaid_normscore is None:
            self.mva_score(norm=True)
        passes = self._mvaid_normscore > tau_mvaid.threshold(self.pt(), working_point)
        return 1. if passes else 0.

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

    def countWP(self,name):
        '''Returns the number of Working Points 
        that are passed for given isolation name.
        
        For example if a tau only passes VLoose, Loose,
        and Medium Working Points this will return 3.

        Uses self.tauID() to evaluate the WPs.
        '''
        WPs = ['VVTight', 'VTight', 'Tight', 'Medium',
               'Loose', 'VLoose', 'VVLoose']
        testname = ''.join(['by{}',name[2:]])
        n_WP = 0
        for WP in WPs:
            if self.tauID(testname.format(WP), verbose=False):
                n_WP += 1
        return n_WP

    def tauID(self, name, verbose=True):
        '''Returns heppy redefined score if fitting one 
        of implemented names, else calls the wrapped tau 
        tauID(name) function.

        If isolation name doesn't include a WP or "raw",
        returns self.countWP(name) that gives a count of 
        the number of passed WPs for this discriminator.
        '''
        if 'IsolationMVArun2017v2DBoldDMwLT' in name:
            #if raw score
            if name == 'byIsolationMVArun2017v2DBoldDMwLTraw2017':
                return self.mva_score()
            #if WPs
            if name == 'byVVLooseIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("VVLoose")
            if name == 'byVLooseIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("VLoose")
            if name == 'byLooseIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("Loose")
            if name == 'byMediumIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("Medium")
            if name == 'byTightIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("Tight")
            if name == 'byVTightIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("VTight")
            if name == 'byVVTightIsolationMVArun2017v2DBoldDMwLT2017':
                return self.mva_passes("VVTight")
            #if numbering scheme
            if ('byIsolation' in name) and ('raw' not in name):
                return self.countWP(name)
        #using numbering scheme
        if ('byIsolation' in name) and ('raw' not in name):
            return self.countWP(name)
        # search in MINIAOD
        if self.physObj.isTauIDAvailable(name):
            return self.physObj.tauID(name)
        # name not available
        if verbose: 
            print 'name', name, 'not available'
        return False

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

