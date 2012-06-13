pathsAndFilters = {
    "HLT_IsoMu12_LooseIsoPFTau10_v4": ("hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12"),
    "HLT_IsoMu15_LooseIsoPFTau15_v2": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v4": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v5": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v6": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v8": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v9": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    # 2011 TauEle Ele15_*_LooseIsoPFTau15
    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1": ("hltPFTau15TrackLooseIso", "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2": ("hltPFTau15TrackLooseIso", "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4": ("hltPFTau15TrackLooseIso", "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v6": ("hltPFTau15TrackLooseIso", "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    # 2011 TauEle Ele15_*_TightIsoPFTau20
    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v1": ("hltPFTauTighIso20TrackTightIso", "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2": ("hltPFTauTighIso20TrackTightIso", "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    # 2011 TauEle Ele18_*_MediumIsoPFTau20 
    "HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1": ("hltPFTauMediumIso20TrackMediumIso", "HLTEle18CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    # 2011 TauEle Ele20_*_MediumIsoPFTau20 
    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1": ("hltPFTauMediumIso20TrackMediumIso", "HLTEle20CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5": ("hltPFTauMediumIso20TrackMediumIso", "HLTEle20CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6": ("hltPFTauMediumIso20TrackMediumIso", "HLTEle20CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
    # the following is used for synchronization
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"), 
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1"),
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1"),
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1"),
    "HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1": ("hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
    #
    # 2012:
    # note: pdgId for taus incorrectly set to 0...
    'HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v*': ( ('hltOverlapFilterIsoMu18LooseIsoPFTau20', [15,0]), ('hltOverlapFilterIsoMu18LooseIsoPFTau20',[13])),
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v*': ( ('hltOverlapFilterIsoMu17LooseIsoPFTau20', [15,0]), ('hltOverlapFilterIsoMu17LooseIsoPFTau20',[13])),

    # hadronic tau triggers 2011
    'HLT_DoubleIsoPFTau20_Trk5_v1': ("hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau20_Trk5_v2': ("hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau20_Trk5_v3': ("hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau20_Trk5_v4': ("hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau25Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau25_Trk5_eta2p1_v1': ("hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau25_Trk5_eta2p1_v2': ("hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau25_Trk5_eta2p1_v3': ("hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau25_Trk5_eta2p1_v4': ("hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau30Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau35_Trk5_eta2p1_v1': ("hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau35_Trk5_eta2p1_v2': ("hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau35_Trk5_eta2p1_v3': ("hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched"),
    'HLT_DoubleIsoPFTau35_Trk5_eta2p1_v4': ("hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched","hltFilterDoubleIsoPFTau35Trk5LeadTrack5IsolationL1HLTMatched"),
    # hadronic tau triggers 2012
    'HLT_DoubleMediumIsoPFTau25_Trk5_eta2p1_Jet30_v1': ("hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02","hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02"),
    'HLT_DoubleMediumIsoPFTau25_Trk5_eta2p1_Jet30_v2': ("hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02","hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02"),
    'HLT_DoubleMediumIsoPFTau25_Trk5_eta2p1_Jet30_v3': ("hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02","hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02"),
    'HLT_DoubleMediumIsoPFTau25_Trk5_eta2p1_Jet30_v4': ("hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02","hltDoublePFTau25TrackPt5MediumIsolationProng4Dz02"),
    'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v1': ("hltDoublePFTau30TrackPt5MediumIsolationProng4Dz02","hltDoublePFTau30TrackPt5MediumIsolationProng4Dz02"),
    'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v2': ("hltDoublePFTau30TrackPt5MediumIsolationProng4Dz02","hltDoublePFTau30TrackPt5MediumIsolationProng4Dz02"),
    }
    

if __name__ == '__main__':

    for path in sorted(pathsAndFilters):   
        print 'path:', path
        filters = pathsAndFilters[path]
        print '\tleg1 filter:', filters[0]
        print '\tleg2 filter:', filters[1]
        
   
