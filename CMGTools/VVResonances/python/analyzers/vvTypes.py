from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer  import * 


leptonTypeUltra = NTupleObjectType("leptonTypeUltra", baseObjectTypes=[leptonTypeExtra], variables = [
    NTupleVariable("cleanedChargedIso",   lambda x : x.cleanedChargedIso, float),       
    NTupleVariable("cleanedNeutralIsoDB",   lambda x : x.cleanedNeutralIsoDB, float),       
    NTupleVariable("cleanedNeutralIsoRho",   lambda x : x.cleanedNeutralIsoRho, float),       
    NTupleVariable("cleanedRelIsoDB",   lambda x : (x.cleanedChargedIso+x.cleanedNeutralIsoDB)/x.pt(), float),       
    NTupleVariable("cleanedRelIsoRho",   lambda x : (x.cleanedChargedIso+x.cleanedNeutralIsoRho)/x.pt(), float)       

])



LNuType = NTupleObjectType("LNuType", baseObjectTypes=[fourVectorType], variables = [
    NTupleVariable("mt",   lambda x : x.mt(), float),       
    NTupleVariable("deltaPhi",   lambda x : x.deltaPhi(), float),       
])



LLType = NTupleObjectType("LLType", baseObjectTypes=[fourVectorType], variables = [
    NTupleVariable("mt",   lambda x : x.mt(), float),       
    NTupleVariable("deltaPhi",   lambda x : x.deltaPhi(), float),       
    NTupleVariable("deltaR",   lambda x : x.deltaR(), float),       
])


JTopologyType = NTupleObjectType("JTopologyType", baseObjectTypes=[], variables = [
    NTupleVariable("DEtaJJ",   lambda x : x['vbfDEta'], float),
    NTupleVariable("MJJ",   lambda x : x['vbfMJJ'], float),
    NTupleVariable("nLooseBTags",   lambda x : x['nLooseBTags'], int),
    NTupleVariable("nMediumBTags",   lambda x : x['nMediumBTags'], int),
    NTupleVariable("nTightBTags",   lambda x : x['nTightBTags'], int),
    NTupleVariable("nJets",   lambda x : len(x['satelliteJets']), int),
    NTupleVariable("topMass",   lambda x : x['topMass'], float),
    NTupleVariable("otherLeptons",   lambda x : len(x['otherLeptons']), int)
])



JetType = NTupleObjectType("SubJetType", baseObjectTypes=[fourVectorType], variables = [
    NTupleVariable("area",   lambda x : x.jetArea(), float),       
    NTupleVariable("rawFactor",   lambda x : x.rawFactor(), float),       
    NTupleVariable("btag",   lambda x : x.bTag(), float),       
    NTupleVariable("nConstituents",   lambda x : len(x.constituents), int),           
    NTupleVariable("looseID",   lambda x : x.looseID, int),           
    NTupleVariable("tightID",   lambda x : x.tightID, int),           
    NTupleVariable("chargedHadronEnergyFraction",   lambda x : x.chargedHadronEnergyFraction(), float),       
    NTupleVariable("neutralHadronEnergyFraction",   lambda x : x.neutralHadronEnergyFraction(), float),       
    NTupleVariable("photonEnergyFraction",   lambda x : x.photonEnergyFraction(), float),       
    NTupleVariable("HFHadronEnergyFraction",   lambda x : x.HFHadronEnergyFraction(), float),       
    NTupleVariable("HFEMEnergyFraction",   lambda x : x.HFEMEnergyFraction(), float),       
    NTupleVariable("muonEnergyFraction",   lambda x : x.muonEnergyFraction(), float),       
    NTupleVariable("electronEnergyFraction",   lambda x : x.electronEnergyFraction(), float),       
    NTupleVariable("leptonEnergyFraction",   lambda x : x.leptonEnergyFraction(), float),       

])


FatJetType = NTupleObjectType("FatJetType", baseObjectTypes=[JetType], variables = [
    NTupleVariable("tau1",   lambda x : x.Ntau[0], float),       
    NTupleVariable("tau2",   lambda x : x.Ntau[1], float),       
    NTupleVariable("tau3",   lambda x : x.Ntau[2], float),       
    NTupleVariable("tau4",   lambda x : x.Ntau[3], float),       
    NTupleVariable("massDropMu",   lambda x : x.massDrop[0], float),       
    NTupleVariable("massDropY",   lambda x : x.massDrop[1], float),       
])


VVType = NTupleObjectType("VVType", baseObjectTypes=[], variables = [
  NTupleSubObject("LV",  lambda x : x['pair'],fourVectorType),
  NTupleVariable("deltaPhi",   lambda x : x['pair'].deltaPhi(), float),       
  NTupleVariable("deltaR",   lambda x : x['pair'].deltaR(), float),       
  NTupleVariable("mt",   lambda x : x['pair'].mt(), float),       
  NTupleSubObject("event",  lambda x : x,JTopologyType),
])


LNuJJType = NTupleObjectType("LNuJJType", baseObjectTypes=[VVType], variables = [
    NTupleSubObject("altLV",  lambda x : x['pair'].leg1.alternateLV+x['pair'].leg2.p4(),fourVectorType),
    NTupleSubObject("rawLV",  lambda x : x['pair'].leg1.rawP4()+x['pair'].leg2.p4(),fourVectorType),
    NTupleSubObject("l1",  lambda x : x['pair'].leg1,LNuType),
    NTupleSubObject("altl1",  lambda x : x['pair'].leg1.alternateLV,fourVectorType),
    NTupleSubObject("l1_l",  lambda x : x['pair'].leg1.leg1,leptonTypeUltra),
    NTupleSubObject("l1_met",  lambda x : x['pair'].leg1.leg2,metType),
    NTupleSubObject("l2",  lambda x : x['pair'].leg2,FatJetType),
    NTupleSubObject("l2_softDrop",  lambda x : x['pair'].leg2.softDropJet,JetType),
    NTupleSubObject("l2_pruned",  lambda x : x['pair'].leg2.prunedJet,JetType),
    NTupleSubObject("l2_pruned_s1",  lambda x : x['pair'].leg2.subjets[0],JetType),
    NTupleVariable("l2_pruned_s1_matched",   lambda x : x['pair'].leg2.subjets[0].matched, int),       
    NTupleSubObject("l2_pruned_s2",  lambda x : x['pair'].leg2.subjets[1],JetType),          
    NTupleVariable("l2_pruned_s2_matched",   lambda x : x['pair'].leg2.subjets[1].matched, int),       
])


LLJJType = NTupleObjectType("LLJJType", baseObjectTypes=[VVType], variables = [
    NTupleSubObject("l1",  lambda x : x['pair'].leg1,LLType),
    NTupleSubObject("l1_l1",  lambda x : x['pair'].leg1.leg1,leptonTypeUltra),
    NTupleSubObject("l1_l2",  lambda x : x['pair'].leg1.leg2,leptonTypeUltra),
    NTupleSubObject("l2",  lambda x : x['pair'].leg2,FatJetType),
    NTupleSubObject("l2_softDrop",  lambda x : x['pair'].leg2.softDropJet,JetType),
    NTupleSubObject("l2_pruned",  lambda x : x['pair'].leg2.prunedJet,JetType),
    NTupleSubObject("l2_pruned_s1",  lambda x : x['pair'].leg2.subjets[0],JetType),
    NTupleVariable("l2_pruned_s1_matched",   lambda x : x['pair'].leg2.subjets[0].matched, int),       
    NTupleSubObject("l2_pruned_s2",  lambda x : x['pair'].leg2.subjets[1],JetType),          
    NTupleVariable("l2_pruned_s2_matched",   lambda x : x['pair'].leg2.subjets[1].matched, int),       
])



JJType = NTupleObjectType("JJType", baseObjectTypes=[VVType], variables = [
    NTupleSubObject("l2",  lambda x : x['pair'].leg2,FatJetType),
    NTupleSubObject("l2_softDrop",  lambda x : x['pair'].leg2.softDropJet,JetType),
    NTupleSubObject("l2_pruned",  lambda x : x['pair'].leg2.prunedJet,JetType),
    NTupleSubObject("l2_pruned_s1",  lambda x : x['pair'].leg2.subjets[0],JetType),
    NTupleVariable("l2_pruned_s1_matched",   lambda x : x['pair'].leg2.subjets[0].matched, int),       
    NTupleSubObject("l2_pruned_s2",  lambda x : x['pair'].leg2.subjets[1],JetType),          
    NTupleVariable("l2_pruned_s2_matched",   lambda x : x['pair'].leg2.subjets[1].matched, int),       
    NTupleSubObject("l1",  lambda x : x['pair'].leg1,FatJetType),
    NTupleSubObject("l1_softDrop",  lambda x : x['pair'].leg1.softDropJet,JetType),
    NTupleSubObject("l1_pruned",  lambda x : x['pair'].leg1.prunedJet,JetType),
    NTupleSubObject("l1_pruned_s1",  lambda x : x['pair'].leg1.subjets[0],JetType),
    NTupleVariable("l1_pruned_s1_matched",   lambda x : x['pair'].leg1.subjets[0].matched, int),       
    NTupleSubObject("l1_pruned_s2",  lambda x : x['pair'].leg1.subjets[1],JetType),          
    NTupleVariable("l1_pruned_s2_matched",   lambda x : x['pair'].leg1.subjets[1].matched, int),       
])



NuNuJJType = NTupleObjectType("NuNuJJType", baseObjectTypes=[VVType], variables = [
    NTupleSubObject("met",  lambda x : x['pair'].leg1,metType),
    NTupleSubObject("jet",  lambda x : x['pair'].leg2,FatJetType),
    NTupleSubObject("jet_softDrop",  lambda x : x['pair'].leg2.softDropJet,JetType),
    NTupleSubObject("jet_pruned",  lambda x : x['pair'].leg2.prunedJet,JetType),
    NTupleSubObject("jet_pruned_s1",  lambda x : x['pair'].leg2.subjets[0],JetType),
    NTupleVariable("jet_pruned_s1_matched",   lambda x : x['pair'].leg2.subjets[0].matched, int),       
    NTupleSubObject("jet_pruned_s2",  lambda x : x['pair'].leg2.subjets[1],JetType),
    NTupleVariable("jet_pruned_s2_matched",   lambda x : x['pair'].leg2.subjets[1].matched, int)       
])


