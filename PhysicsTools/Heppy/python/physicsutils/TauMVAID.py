from ROOT import TFile, heppy, Double

import os
import logging 
import pprint 

class TauMVAID(object): 

    def __init__(self, gbrforest_fname, norm_fname, 
                 mva, mva_opt,
                 working_points, verbose=True):
        '''constructor
        gbrforest_fname : path to the root file containing the gbr forest, 
                          for evaluation of the mva ID
        norm_fname : path to the root file containing the TFormula for the normalization
                     for the mva output
        working_points : dictionary of working points containing the desired 
                         working point name as a key and the corresponding root 
                         file containing a TGraph. 
                         This TGraph gives the threshold to be applied to the mva 
                         score as a function of the raw tau pT
        '''
        logging.info('initializing tau id MVA')
        logging.info(pprint.pformat([gbrforest_fname, 
                                     norm_fname, 
                                     mva, mva_opt]))
        self._mvaid = heppy.PATTauDiscriminationByMVAIsolationRun2FWlite(
            os.path.expandvars(gbrforest_fname),
            mva, mva_opt
            ) 
        self.mva = mva
        self.mva_opt = mva_opt
        self.norm_formula = self._get_norm_formula(norm_fname)
        self.wp_graphs = self._get_wp_graphs(working_points)

    def _get_norm_formula(self, norm_fname):
        rootfile = TFile(norm_fname)
        key = os.path.splitext( os.path.basename(norm_fname) )[0]
        return rootfile.Get(key)

    def _get_wp_graphs(self, working_points):
        wp_graphs = dict()
        self.rootfiles = [] # prevent garbage collector to close files
        for wpname, fname in working_points.iteritems(): 
            logging.info( 'working point {}: {}'.format(wpname, fname))
            rootfile = TFile(fname)
            self.rootfiles.append(rootfile)
            the_key = rootfile.GetListOfKeys().At(0)
            tgraph = rootfile.Get(the_key.GetName())
            wp_graphs[wpname] = tgraph
        return wp_graphs

    def passes(self, tau, working_point):
        return self.score_norm(tau) > self.threshold(tau.pt(), working_point)

    def score(self, tau):
        '''returns the mva score for a heppy Tau'''
        return self._mvaid(tau.physObj)

    def score_norm(self, tau):
        '''returns normalized mva score for a heppy tau'''
        return self.norm_formula.Eval(self.score(tau))

    def threshold(self, pt, working_point):
        '''returns the threshold to be applied to the mva score
        for a given working point, knowing the tau pT.'''
        graph = self.wp_graphs[working_point]
        xmin, xmax, dummy = Double(), Double(), Double()
        graph.GetPoint(0,xmin,dummy)
        graph.GetPoint(graph.GetN()-1, xmax, dummy)
        # pt set to be in the range of the TGraph, with a margin
        epsilon = 1e-3
        pt = max(pt, xmin+epsilon)
        pt = min(pt, xmax-epsilon)
        return graph.Eval(pt)


# 2017v2 MVA: 

basedir = '$CMSSW_BASE/src/PhysicsTools/Heppy/data/'
working_points = dict(
    VVTight = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff40.root',
    VTight = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff50.root',
    Tight = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff60.root', 
    Medium = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff70.root',
    Loose = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff80.root',
    VLoose = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff90.root',
    VVLoose = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff95.root'
    )
for wp in working_points:
    working_points[wp] = '/'.join([basedir, working_points[wp]])

tau_mvaid_2017 = TauMVAID(
    basedir+'GBRForest_tauIdMVAIsoDBoldDMwLT2017v2.root',
    basedir+'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2_mvaOutput_normalization.root',
    'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2', 'DBoldDMwLTwGJ',
    working_points
    )

