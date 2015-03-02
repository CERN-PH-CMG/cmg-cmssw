triggers_mumu = ["HLT_Mu17_Mu8_v*","HLT_Mu17_TkMu8_v*"]
triggers_mu = ["HLT_IsoMu24_v*","HLT_IsoMu24_eta2p1_v*"]
triggers_jpsi = ["HLT_Dimuon10_Jpsi_Barrel_v*","HLT_Dimuon*_Jpsi_Displaced_v*"]
triggers_jpsi_nobarrelcuts = ["DoubleMu3_Jpsi_v*","DoubleMu3_Jpsi_v*","Dimuon6p5_Jpsi_v*"]
triggers_upsilon = ["HLT_Dimuon5_Upsilon_Barrel_v*","HLT_Dimuon7_Upsilon_Barrel_v*","HLT_Dimuon9_Upsilon_Barrel_v*"]
triggers_2upsilon = ["HLT_Dimuon0_Upsilon_Muon_v*","HLT_Dimuon0_Jpsi_Muon_v*"]

triggers_jpsi2012 = ["HLT_Dimuon*_Jpsi_v*"]
triggers_upsilon2012 = ["HLT_Dimuon*_Upsilon_v*"]

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
    
    # 2011 MuOnia Dimuon10_Jpsi_Barrel
    "HLT_Dimuon10_Jpsi_Barrel_v1": ("hltVertexmumuFilterJpsiBarrel"),
    "HLT_Dimuon10_Jpsi_Barrel_v2": ("hltVertexmumuFilterJpsiBarrel"),
    "HLT_Dimuon10_Jpsi_Barrel_v3": ("hltVertexmumuFilterJpsiBarrel"),
    "HLT_Dimuon10_Jpsi_Barrel_v5": ("hltVertexmumuFilterJpsiBarrel"),
    "HLT_Dimuon10_Jpsi_Barrel_v6": ("hltVertexmumuFilterDimuon10JpsiBarrel"),
    "HLT_Dimuon10_Jpsi_Barrel_v9": ("hltVertexmumuFilterDimuon10JpsiBarrel"),
    "HLT_Dimuon10_Jpsi_Barrel_v10": ("hltVertexmumuFilterDimuon10JpsiBarrel"),
  }
    

if __name__ == '__main__':

    for path in sorted(pathsAndFilters):   
        print 'path:', path
        filters = pathsAndFilters[path]
        print '\tleg1 filter:', filters[0]
        print '\tleg2 filter:', filters[1]
        
   
