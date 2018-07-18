from ROOT import TFile, heppy, Double

import os
import logging 

class TauMVAID(object): 

    def __init__(self, gbrforest_fname, mva, mva_opt,
                 working_points, verbose=True):
        '''constructor
        gbrforest_fname : path to the root file containing the gbr forest, 
        for evaluation of the mva ID
        working_points : dictionary of working points containing the desired 
        working point name as a key and the corresponding root file containing 
        a TGraph. This TGraph gives the threshold to be applied to the mva 
        score as a function of the raw tau pT
        '''
        logging.info('initializing tau id MVA')
        logging.info(', '.join([gbrforest_fname, mva, mva_opt]))
        self._mvaid = heppy.PATTauDiscriminationByMVAIsolationRun2FWlite(
            os.path.expandvars(gbrforest_fname),
            mva, mva_opt
            ) 
        self.mva = mva
        self.mva_opt = mva_opt
        self.wp_graphs = self._get_wp_graphs(working_points)

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
        return self.score(tau) > self.threshold(tau.pt(), working_point)

    def score(self, tau):
        '''returns the mva score for a heppy Tau'''
        return self._mvaid(tau.physObj)

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
    Eff40 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff40.root', #VVTight
    Eff50 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff50.root', #VTight
    Eff60 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff60.root', #Tight 
    Eff70 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff70.root', #Medium
    Eff80 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff80.root', #Loose
    Eff90 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff90.root', #VLoose
    Eff95 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff95.root'  #VVLoose
    )

for wp in working_points:
    working_points[wp] = '/'.join([basedir, working_points[wp]])

tau_mvaid_2017 = TauMVAID(
    basedir+'GBRForest_tauIdMVAIsoDBoldDMwLT2017v2.root', 
    'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2', 'DBoldDMwLTwGJ',
    working_points
    )

