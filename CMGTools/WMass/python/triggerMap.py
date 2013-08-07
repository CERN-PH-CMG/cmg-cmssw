pathsAndFilters = {
    # 2011 Mu Iso24
    "HLT_IsoMu24_v1": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v2": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v3": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v4": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v5": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v6": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v7": ("hltSingleMuIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v8": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v9": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v10": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v11": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v12": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v13": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    "HLT_IsoMu24_v14": ("hltSingleMuL2QualIsoL3IsoFiltered24"),
    # 2011 Mu Iso24_eta2p1
    "HLT_IsoMu24_eta2p1_v1": ("hltL3fL1sMu14Eta2p1L1f0L2f14QL3Filtered24"),
    "HLT_IsoMu24_eta2p1_v2": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    "HLT_IsoMu24_eta2p1_v3": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    "HLT_IsoMu24_eta2p1_v4": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    "HLT_IsoMu24_eta2p1_v5": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    "HLT_IsoMu24_eta2p1_v6": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    "HLT_IsoMu24_eta2p1_v7": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    "HLT_IsoMu24_eta2p1_v8": ("hltL3IsoL1sMu14Eta2p1L1f0L2f14QL2IsoL3f24L3IsoFiltered"),
    
  }
    

if __name__ == '__main__':

    for path in sorted(pathsAndFilters):   
        print 'path:', path
        filters = pathsAndFilters[path]
        print '\tleg1 filter:', filters[0]
        print '\tleg2 filter:', filters[1]
        
   
