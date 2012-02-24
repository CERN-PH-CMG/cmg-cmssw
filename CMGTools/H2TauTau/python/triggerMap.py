pathsAndFilters = {
    "HLT_IsoMu12_LooseIsoPFTau10_v4": ("hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12"),
    "HLT_IsoMu15_LooseIsoPFTau15_v2": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v4": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v5": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v6": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v8": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_LooseIsoPFTau15_v9": ("hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15"),
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1"),
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1"),
    "HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6": ("hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1"),
    }

    

if __name__ == '__main__':

    for path in sorted(pathsAndFilters):   
        print 'path:', path
        filters = pathsAndFilters[path]
        print '\tleg1 filter:', filters[0]
        print '\tleg2 filter:', filters[1]
        
    
