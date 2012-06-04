pathsAndFilters = {
    "HLT_IsoMu12_LooseIsoPFTau10_v4": ("hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12"),
    "HLT_IsoMu15_LooseIsoPFTau15_v2": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v4": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v5": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v6": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v8": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v9": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
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
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v*': ( ('hltOverlapFilterIsoMu17LooseIsoPFTau20', [15,0]), ('hltOverlapFilterIsoMu17LooseIsoPFTau20',[13]))
    }
    

if __name__ == '__main__':

    for path in sorted(pathsAndFilters):   
        print 'path:', path
        filters = pathsAndFilters[path]
        print '\tleg1 filter:', filters[0]
        print '\tleg2 filter:', filters[1]
        
    
