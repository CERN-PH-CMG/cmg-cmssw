from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer  import * 

leptonTypeHZZ = NTupleObjectType("leptonHZZ", baseObjectTypes = [ leptonTypeExtra ], variables = [
    NTupleVariable("mvaIdPhys14",   lambda lepton : lepton.mvaRun2("NonTrigPhys14") if abs(lepton.pdgId()) == 11 else 1, help="EGamma POG MVA ID for non-triggering electrons, Phys14 re-training; 1 for muons"),
    # Extra isolation variables
    NTupleVariable("chargedHadIso04",   lambda x : x.chargedHadronIsoR(0.4)/x.pt(),   help="PF Iso, R=0.4, charged hadrons only"),
    NTupleVariable("neutralHadIso04",   lambda x : x.neutralHadronIsoR(0.4)/x.pt(),   help="PF Iso, R=0.4, neutral hadrons only"),
    NTupleVariable("photonIso04",       lambda x : x.photonIsoR(0.4)/x.pt(),          help="PF Iso, R=0.4, photons only"),
    NTupleVariable("puChargedHadIso04", lambda x : x.puChargedHadronIsoR(0.4)/x.pt(), help="PF Iso, R=0.4, pileup charged hadrons only"),
    NTupleVariable("rho",               lambda x : x.rho,                             help="rho for isolation"),
    NTupleVariable("EffectiveArea04",   lambda x : x.EffectiveArea04,                 help="EA for isolation"),
])

ZZType = NTupleObjectType("ZZType", baseObjectTypes=[fourVectorType], variables = [
    NTupleVariable("hasFSR",   lambda x : x.hasFSR(), int),
    NTupleVariable("z1_hasFSR",   lambda x : x.leg1.hasFSR(), int),
    NTupleVariable("z2_hasFSR",   lambda x : x.leg2.hasFSR(), int),
    NTupleSubObject("z1",  lambda x : x.leg1,fourVectorType),
    NTupleSubObject("z2",  lambda x : x.leg2,fourVectorType),
    NTupleSubObject("z1_l1",  lambda x : x.leg1.leg1,leptonTypeHZZ),
    NTupleSubObject("z1_l2",  lambda x : x.leg1.leg2,leptonTypeHZZ),
    NTupleSubObject("z2_l1",  lambda x : x.leg2.leg1,leptonTypeHZZ),
    NTupleSubObject("z2_l2",  lambda x : x.leg2.leg2,leptonTypeHZZ),
    # -------
    NTupleVariable("KD",   lambda x : getattr(x, 'KD', -1.0), help="MELA KD"),
    NTupleVariable("MELAcosthetastar", lambda x : x.melaAngles.costhetastar if hasattr(x,'melaAngles') else -99.0, help="MELA angle costhetastar"),
    NTupleVariable("MELAcostheta1", lambda x : x.melaAngles.costheta1 if hasattr(x,'melaAngles') else -99.0, help="MELA angle costheta1"),
    NTupleVariable("MELAcostheta2", lambda x : x.melaAngles.costheta2 if hasattr(x,'melaAngles') else -99.0, help="MELA angle costheta2"),
    NTupleVariable("MELAphi", lambda x : x.melaAngles.phi if hasattr(x,'melaAngles') else -99.0, help="MELA angle phi"),
    NTupleVariable("MELAphistar1", lambda x : x.melaAngles.phistar1 if hasattr(x,'melaAngles') else -99.0, help="MELA angle phistar1"),

])


