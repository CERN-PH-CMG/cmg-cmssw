from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer  import * 

met_globalVariables = [
    NTupleVariable("rho",  lambda ev: ev.rho, float, help="kt6PFJets rho"),
    NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
##    NTupleVariable("nPU",  lambda ev: ev.nPU, long, help="getPU_NumInteractions"),

##    NTupleVariable("ntracksPV",  lambda ev: ev.goodVertices[0].tracksSize() , int, help="Number of tracks (with weight > 0.5)"),
##    NTupleVariable("ndofPV",  lambda ev: ev.goodVertices[0].ndof() , int, help="Degrees of freedom of the fit"),

   # ----------------------- lepton info -------------------------------------------------------------------- #     

    NTupleVariable("nLeptons",   lambda x : len(x.leptons) if  hasattr(x,'leptons') else  0 , float, mcOnly=False,help="Number of associated leptons"),

    NTupleVariable("zll_pt", lambda ev : ev.zll_p4.Pt() if  hasattr(ev,'zll_p4') else  0 , help="Pt of di-lepton system"),
    NTupleVariable("zll_eta", lambda ev : ev.zll_p4.Eta() if  hasattr(ev,'zll_p4') else  0, help="Eta of di-lepton system"),
    NTupleVariable("zll_phi", lambda ev : ev.zll_p4.Phi() if  hasattr(ev,'zll_p4') else  0, help="Phi of di-lepton system"),
    NTupleVariable("zll_mass", lambda ev : ev.zll_p4.M() if  hasattr(ev,'zll_p4') else  0, help="Invariant mass of di-lepton system"),

   # ----------------------- MET filter information (temporary)  -------------------------------------------------------------------- #

    NTupleVariable("Flag_HBHENoiseFilter", lambda ev: ev.hbheFilterNew, help="HBEHE temporary filter decision"),

   # ----------------------- dedicated met info -------------------------------------------------------------------- #

    NTupleVariable("met_uPara_zll", lambda ev : ev.met.upara_zll if  hasattr(ev,'zll_p4') else -999 , help="recoil MET"),
    NTupleVariable("met_uPerp_zll", lambda ev : ev.met.uperp_zll if  hasattr(ev,'zll_p4') else -999 , help="recoil MET"),

    NTupleVariable("metNoHF_uPara_zll", lambda ev : ev.metNoHF.upara_zll if hasattr(ev,'metNoHF') and hasattr(ev,'zll_p4') else -999 , help="recoil MET"),
    NTupleVariable("metNoHF_uPerp_zll", lambda ev : ev.metNoHF.uperp_zll if hasattr(ev,'metNoHF') and hasattr(ev,'zll_p4') else -999 , help="recoil MET"),

    NTupleVariable("met_rawPt", lambda ev : ev.met.uncorPt(), help="raw met p_{T}"),
    NTupleVariable("met_rawPhi", lambda ev : ev.met.uncorPhi(), help="raw met phi"),
    NTupleVariable("met_rawSumEt", lambda ev : ev.met.uncorSumEt(), help="raw met sumEt"),

    NTupleVariable("metNoHF_rawPt", lambda ev : ev.metNoHF.uncorPt() if hasattr(ev,'metNoHF') else -999, help="raw met p_{T}"),
    NTupleVariable("metNoHF_rawPhi", lambda ev : ev.metNoHF.uncorPhi() if hasattr(ev,'metNoHF') else -999, help="raw met phi"),
    NTupleVariable("metNoHF_rawSumEt", lambda ev : ev.metNoHF.uncorSumEt() if hasattr(ev,'metNoHF') else -999, help="raw met sumEt"),

    NTupleVariable("met_raw_uPara_zll", lambda ev : ev.met_raw.upara_zll if  hasattr(ev,'zll_p4') else -999 , help="recoil MET"),
    NTupleVariable("met_raw_uPerp_zll", lambda ev : ev.met_raw.uperp_zll if  hasattr(ev,'zll_p4') else -999 , help="recoil MET"),

    NTupleVariable("met_caloPt", lambda ev : ev.met.caloMETPt(), help="calo met p_{T}"),
    NTupleVariable("met_caloPhi", lambda ev : ev.met.caloMETPhi(), help="calo met phi"),
    NTupleVariable("met_caloSumEt", lambda ev : ev.met.caloMETSumEt(), help="calo met sumEt"),

   # ----------------------- type1met studies info -------------------------------------------------------------------- #     

    NTupleVariable("met_JetEnUp_Pt", lambda ev : ev.met.shiftedPt(ev.met.JetEnUp, ev.met.Raw), help="type1, JetEnUp, pt"),
    NTupleVariable("met_JetEnUp_Phi", lambda ev : ev.met.shiftedPhi(ev.met.JetEnUp, ev.met.Raw), help="type1, JetEnUp, phi"),

    NTupleVariable("met_JetEnDown_Pt", lambda ev : ev.met.shiftedPt(ev.met.JetEnDown, ev.met.Raw), help="type1, JetEnDown, pt"),
    NTupleVariable("met_JetEnDown_Phi", lambda ev : ev.met.shiftedPhi(ev.met.JetEnDown, ev.met.Raw), help="type1, JetEnDown, phi"),

    NTupleVariable("metNoHF_JetEnUp_Pt", lambda ev : ev.metNoHF.shiftedPt(ev.met.JetEnUp, ev.met.Raw) if hasattr(ev,'metNoHF') else -999, help="type1 noHF , JetEnUp, pt"),
    NTupleVariable("metNoHF_JetEnUp_Phi", lambda ev : ev.metNoHF.shiftedPhi(ev.met.JetEnUp, ev.met.Raw) if hasattr(ev,'metNoHF') else -999, help="type1 noHF , JetEnUp, phi"),

    NTupleVariable("metNoHF_JetEnDown_Pt", lambda ev : ev.metNoHF.shiftedPt(ev.met.JetEnDown, ev.met.Raw) if hasattr(ev,'metNoHF') else -999, help="type1 noHF , JetEnDown, pt"),
    NTupleVariable("metNoHF_JetEnDown_Phi", lambda ev : ev.metNoHF.shiftedPhi(ev.met.JetEnDown, ev.met.Raw) if hasattr(ev,'metNoHF') else -999, help="type1 noHF , JetEnDown, phi"),

   # --------------------------------------------------------

    NTupleVariable("ak4MET_Pt", lambda ev : ev.ak4MET.pt() if  hasattr(ev,'ak4MET') else  0 , help="type1, V4, pt"),
    NTupleVariable("ak4MET_Phi", lambda ev : ev.ak4MET.phi() if  hasattr(ev,'ak4MET') else  0 , help="type1, V4, phi"),

    NTupleVariable("ak4chsMET_Pt", lambda ev : ev.ak4chsMET.pt() if  hasattr(ev,'ak4chsMET') else  0 , help="type1, V4, pt"),
    NTupleVariable("ak4chsMET_Phi", lambda ev : ev.ak4chsMET.phi() if  hasattr(ev,'ak4chsMET') else  0 , help="type1, V4, phi"),

    NTupleVariable("ak420MET_Pt", lambda ev : ev.ak4pt20MET.pt() if  hasattr(ev,'ak4pt20MET') else  0 , help="type1, V4, pt20, pt"),
    NTupleVariable("ak420MET_Phi", lambda ev : ev.ak4pt20MET.phi() if  hasattr(ev,'ak4pt20MET') else  0 , help="type1, V4, pt20, phi"),

    NTupleVariable("ak4chs20MET_Pt", lambda ev : ev.ak4chspt20MET.pt() if  hasattr(ev,'ak4chspt20MET') else  0 , help="type1, V4, pt20, pt"),
    NTupleVariable("ak4chs20MET_Phi", lambda ev : ev.ak4chspt20MET.phi() if  hasattr(ev,'ak4chspt20MET') else  0 , help="type1, V4, pt>20, phi"),

    NTupleVariable("ak4Mix_Pt", lambda ev : ev.ak4Mix.pt() if  hasattr(ev,'ak4Mix') else  0 , help="type1, V4, pt20, Mix, pt"),
    NTupleVariable("ak4Mix_Phi", lambda ev : ev.ak4Mix.phi() if  hasattr(ev,'ak4Mix') else  0 , help="type1, V4, pt>20, Mix, phi"),

   # ----------------------- tkMet info -------------------------------------------------------------------- #     

    NTupleVariable("tkmet_genPt", lambda ev : ev.tkGenMet.pt() if  hasattr(ev,'tkGenMet') else  0 , help="TK E_{T}^{miss} dz<0.1 pt"),
    NTupleVariable("tkmet_genPhi", lambda ev : ev.tkGenMet.phi() if  hasattr(ev,'tkGenMet') else  0 , help="TK E_{T}^{miss} dz<0.1 phi"),

    ##
    NTupleVariable("tkmet_pt", lambda ev : ev.tkMet.pt() if  hasattr(ev,'tkMet') else  0, help="TK E_{T}^{miss} dz<0.1 pt"),
    NTupleVariable("tkmet_phi", lambda ev : ev.tkMet.phi() if  hasattr(ev,'tkMet') else  0 , help="TK E_{T}^{miss} dz<0.1 phi"),
    NTupleVariable("tkmet_sumEt", lambda ev : ev.tkMet.sumEt if  hasattr(ev,'tkMet') else  0 , help="TK sumEt charged dz<0.1 pt"),

    NTupleVariable("tkmet_uPara_zll", lambda ev : ev.tkMet.upara_zll if  hasattr(ev,'tkMet') and hasattr(ev,'zll_p4') else -999 , help="TK sumEt charged dz<0.1 pt"),
    NTupleVariable("tkmet_uPerp_zll", lambda ev : ev.tkMet.uperp_zll if  hasattr(ev,'tkMet') and hasattr(ev,'zll_p4') else -999 , help="TK sumEt charged dz<0.1 pt"),

    ##
    NTupleVariable("tkmetchs_pt", lambda ev : ev.tkMetPVchs.pt() if  hasattr(ev,'tkMetPVchs') else  0, help="TK E_{T}^{miss} fromPV>0 pt"),
    NTupleVariable("tkmetchs_phi", lambda ev : ev.tkMetPVchs.phi() if  hasattr(ev,'tkMetPVchs') else  0, help="TK E_{T}^{miss} fromPV>0 phi"),
    NTupleVariable("tkmetchs_sumEt", lambda ev : ev.tkMetPVchs.sumEt if  hasattr(ev,'tkMetPVchs') else  0, help="TK sumEt charged fromPV>0"),

    NTupleVariable("tkmetchs_uPara_zll", lambda ev : ev.tkMetPVchs.upara_zll if  hasattr(ev,'tkMetPVchs') and hasattr(ev,'zll_p4') else -999 , help="TK sumEt charged fromPV>0 pt"),
    NTupleVariable("tkmetchs_uPerp_zll", lambda ev : ev.tkMetPVchs.uperp_zll if  hasattr(ev,'tkMetPVchs') and hasattr(ev,'zll_p4') else -999 , help="TK sumEt charged fromPV>0 pt"),

#    NTupleVariable("tkmetPVLoose_pt", lambda ev : ev.tkMetPVLoose.pt(), help="TK E_{T}^{miss} fromPV>1 pt"),
#    NTupleVariable("tkmetPVLoose_phi", lambda ev : ev.tkMetPVLoose.phi(), help="TK E_{T}^{miss} fromPV>1 phi"),
#    NTupleVariable("tkmetPVLoose_sumEt", lambda ev : ev.tkMetPVLoose.sumEt, help="TK sumEt charged fromPV>1"),

#    NTupleVariable("tkmetPVTight_pt", lambda ev : ev.tkMetPVTight.pt(), help="TK E_{T}^{miss} fromPV>2 pt"),
#    NTupleVariable("tkmetPVTight_phi", lambda ev : ev.tkMetPVTight.phi(), help="TK E_{T}^{miss} fromPV>2 phi"),
#    #    NTupleVariable("tkmetPVTight_sumEt", lambda ev : ev.tkPVTight.sumEt, help="TK sumEt charged fromPV>2"),

    ]

met_globalObjects = {
    "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
    "metNoHF" : NTupleObject("metNoHF", metType, help="PF E_{T}^{miss}, after type 1 corrections (NoHF)"),
#    "metraw" : NTupleObject("metraw", metType, help="PF E_{T}^{miss}"),
#    "metType1chs" : NTupleObject("metType1chs", metType, help="PF E_{T}^{miss}, after type 1 CHS jets"),
    #"tkMet" : NTupleObject("tkmet", metType, help="TK PF E_{T}^{miss}"),
    #"metNoPU" : NTupleObject("metNoPU", fourVectorType, help="PF noPU E_{T}^{miss}"),
    }

met_collections = {
    "genleps"         : NTupleCollection("genLep",     genParticleWithLinksType, 10, help="Generated leptons (e/mu) from W/Z decays"),
    "gentauleps"      : NTupleCollection("genLepFromTau", genParticleWithLinksType, 10, help="Generated leptons (e/mu) from decays of taus from W/Z/h decays"),
    "gentaus"         : NTupleCollection("genTau",     genParticleWithLinksType, 10, help="Generated leptons (tau) from W/Z decays"),                            
    "generatorSummary" : NTupleCollection("GenPart", genParticleWithLinksType, 100 , help="Hard scattering particles, with ancestry and links"),
#    "selectedLeptons" : NTupleCollection("lep", leptonType, 50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
    "selectedPhotons"    : NTupleCollection("gamma", photonType, 50, help="photons with pt>20 and loose cut based ID"),
    }
