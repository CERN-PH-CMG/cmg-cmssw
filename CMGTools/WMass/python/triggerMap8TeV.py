pathsAndFilters = {
    "HLT_IsoMu24_eta2p1_v9": ("hltL3fL1sMu16Eta2p1L1f0L2f16QL3Filtered24Q"),
 #   "HLT_IsoMu24_eta2p1_v10": (""),
    "HLT_IsoMu24_eta2p1_v11": ("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"),
    "HLT_IsoMu24_eta2p1_v12": ("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"),
    "HLT_IsoMu24_eta2p1_v13": ("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"),
    "HLT_IsoMu24_eta2p1_v14": ("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"),
    "HLT_IsoMu24_eta2p1_v15": ("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"),
    "HLT_IsoMu24_eta2p1_v16": ("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"),
  
  }
    

if __name__ == '__main__':

    for path in sorted(pathsAndFilters):   
        print 'path:', path
        filters = pathsAndFilters[path]
        print '\tleg1 filter:', filters[0]
        print '\tleg2 filter:', filters[1]
        
   
