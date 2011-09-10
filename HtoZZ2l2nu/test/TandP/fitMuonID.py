import FWCore.ParameterSet.Config as cms
### USAGE:
###    cmsRun fitMuonID.py <scenario> [ <id> [ <binning1> ... <binningN> ] ]
###
### scenarios:
###   - data_all (default)  
###   - signal_mc

import sys
args = sys.argv[1:]
if (sys.argv[0] == "cmsRun"): args =sys.argv[2:]
scenario = "data_all"
if len(args) > 0: scenario = args[0]
print "Will run scenario ", scenario 

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    Variables = cms.PSet(
        mass = cms.vstring("Tag-muon Mass", "70", "130", "GeV/c^{2}"),
        pt = cms.vstring("muon p_{T}", "0", "1000", "GeV/c"),
        eta    = cms.vstring("muon #eta", "-2.5", "2.5", ""),
        abseta = cms.vstring("muon |#eta|", "0", "2.5", ""),
        phi    = cms.vstring("muon #phi at vertex", "-3.1416", "3.1416", ""),
        charge = cms.vstring("muon charge", "-2.5", "2.5", ""),
        tag_pt = cms.vstring("Tag p_{T}", "0", "1000", "GeV/c"),
        tag_nVertices   = cms.vstring("Number of vertices", "0", "999", ""),
        tag_nVerticesDA = cms.vstring("Number of vertices", "0", "999", ""),
        isoTrk03Abs    = cms.vstring("Probe abs trk iso", "-2", "9999999", ""),
        tag_combRelIso = cms.vstring("Tag comb rel iso", "-2", "9999999", ""),
        l3pt = cms.vstring("L3 p_{T}", "-99999999", "999999999", ""),
        pair_nL1EG5 = cms.vstring("Number of L1 EG5", "-1", "9999", ""),
        avgInsDelLumi = cms.vstring("Avg. Inst. Lumi", "200", "2000", "Hz/ub"),
        pair_dz = cms.vstring("Pair DZ", "-25", "25", "cm"),
                              
    ),

    Categories = cms.PSet(
        Glb   = cms.vstring("Global", "dummy[pass=1,fail=0]"),
        VBTF     = cms.vstring("VBTFLike", "dummy[pass=1,fail=0]"),
        VBTFold  = cms.vstring("VBTFLike", "dummy[pass=1,fail=0]"),
        TMOSL = cms.vstring("TMOneStationLoose", "dummy[pass=1,fail=0]"),
        TMOST = cms.vstring("TMOneStationTight", "dummy[pass=1,fail=0]"),
        PF    = cms.vstring("PF Muon", "dummy[pass=1,fail=0]"),
        TM    = cms.vstring("Tracker Muon", "dummy[pass=1,fail=0]"),
        TMA   = cms.vstring("Arbitrated Tracker Muon", "dummy[pass=1,fail=0]"),
        Isol  = cms.vstring("Comb rel iso", "dummy[pass=1,fail=0]"),
        IsolTk3 = cms.vstring("Tk Abs Iso < 3", "dummy[pass=1,fail=0]"),
        Mu15  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu24  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        IsoMu15   = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        IsoMu17   = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu7 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        DoubleMu3 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu8_forEMu  = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        Mu17_forEMu = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_Mu15 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        tag_Mu24 = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        mcTrue = cms.vstring("MC true", "dummy[pass=1,fail=0]"),
        passingIso = cms.vstring("Pass ISO", "dummy[pass=1,fail=0]"),
        passingMuon = cms.vstring("Pass Muon", "dummy[pass=1,fail=0]"),
        passingTrk = cms.vstring("Pass Trk", "dummy[pass=1,fail=0]"),
        passingKin = cms.vstring("Pass Kin", "dummy[pass=1,fail=0]"),

    ),
    PDFs = cms.PSet(
        voigtPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[90,80,100], width[2.495], sigma[3,1,20])",
            "Exponential::backgroundPass(mass, lp[0,-5,5])",
            "Exponential::backgroundFail(mass, lf[0,-5,5])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpo = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpoMin70 = cms.vstring(
            "Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,3,10])",
            "SUM::signal(vFrac[0.8,0.5,1]*signal1, signal2)",
            "Exponential::backgroundPass(mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0.7,1]",
            "signalFractionInPassing[0.9]"
        )
    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(40),

    Efficiencies = cms.PSet(), # will be filled later
)

TRIGGER = cms.PSet(tag_Mu24 = cms.vstring("pass"))
if "mc" in scenario or "39X" in scenario or "38X" in scenario:
    TRIGGER = cms.PSet(tag_Mu15 = cms.vstring("pass"), tag_pt = cms.vdouble(24.,9999.))

PT_ETA_BINS = cms.PSet(
    pt     = cms.vdouble(  20, 30, 100 ),
    abseta = cms.vdouble(  0.0, 0.8, 1.2, 2.4),
    tag_combRelIso = cms.vdouble(  -2, 0.1)
    #,
    #pair_dz = cms.vdouble(  -2, 2),
)

ETA_BINS = cms.PSet(
    pt  = cms.vdouble(20,100),
    eta = cms.vdouble(-2.4, -2.1, -1.6, -1.1, -0.6, 0, 0.6, 1.1, 1.6, 2.1, 2.4),
)



ETA_PHI_BINS = ETA_BINS.clone(
   eta = cms.vdouble(-2.4, -1.6, -8.0, 0, 0.8, 1.6, 2.4),
   phi = cms.vdouble(*[3.1416*i/3.0 for i in range(-3,4)]), 
)
VTX_BINS  = cms.PSet(
    pt     = cms.vdouble(  20, 120 ),
    abseta = cms.vdouble(  0.0, 2.4),
    tag_nVertices = cms.vdouble(0.5,2.5,4.5,6.5,8.5,10.5,12.5,14.5,16.5)
)
VTXDA_BINS  = cms.PSet(
    pt     = cms.vdouble(  20, 120 ),
    abseta = cms.vdouble(  0.0, 2.4),
    tag_nVerticesDA = cms.vdouble(0.5,2.5,4.5,6.5,8.5,10.5,12.5,14.5,16.5,18.5,20.5),
    tag_combRelIso = cms.vdouble(  -2, 0.1)
    #,
    #pair_dz = cms.vdouble(  -2, 2)
)


ETA_BINS_FINE = cms.PSet(
    pt  = cms.vdouble(20,100),
    eta = cms.vdouble(*[x/10. for x in xrange(-24,25,1)]),
)
OVERALL = cms.PSet(
    pt  = cms.vdouble(20,100),
    abseta = cms.vdouble(0.0, 2.4),
)
OVERALL_ABSETA = cms.PSet(
    pt  = cms.vdouble(20,100),
    abseta = cms.vdouble(0.0, 1.2, 2.4),
)

OVERALL_ENDCAPS21 = cms.PSet(
    pt  = cms.vdouble(20,100),
    abseta = cms.vdouble(1.2, 2.1),
)


CHARGE = cms.PSet(
    pt     = cms.vdouble(20,100),
    abseta = cms.vdouble(0.0, 2.4),
    charge = cms.vdouble(-2,0,2),
)

LUMI_BINS = cms.PSet(
    pt     = cms.vdouble(20,100),
    avgInsDelLumi = cms.vdouble(200,400,800,1000,1200,1400,1600,1800,2000)
    )

#PREFIX="/data/gpetrucc/7TeV/tnp/2011.02.17/"
PREFIX="/data/Analysis/jobSubmission/muTnP_v0/"
process.TnP_MuonID = Template.clone(
    InputFileNames = cms.vstring(PREFIX+"tnpZ_2011A_v1_GOLDEN.root",    PREFIX+"tnpZ_2011A_v2_GOLDEN.root",
                                 PREFIX+"tnpZ_2011A_v1_NotGolden.root", PREFIX+"tnpZ_2011A_v2_NotGolden.root"),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("/data/Analysis/TnP/TnP_MuonID_%s.root" % scenario),
    Efficiencies = cms.PSet(),
)



if "data" in scenario:
    if   "v1" in scenario: process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_2011A_v1_GOLDEN.root" ]
    elif "v2" in scenario: process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_2011A_v2_GOLDEN.root" ]
    else:                  process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_2011A_v1_GOLDEN.root", PREFIX+"tnpZ_2011A_v2_GOLDEN.root" ]
if "JSON" not in scenario:
    if   "v1" in scenario: process.TnP_MuonID.InputFileNames += [ PREFIX+"tnpZ_2011A_v1_NotGolden.root" ]
    elif "v2" in scenario: process.TnP_MuonID.InputFileNames += [ PREFIX+"tnpZ_2011A_v2_NotGolden.root" ]
    else:                  process.TnP_MuonID.InputFileNames += [ PREFIX+"tnpZ_2011A_v1_NotGolden.root", PREFIX+"tnpZ_2011A_v2_NotGolden.root" ]

if "38X" in scenario and "data" in scenario:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_Data_Nov4B.root" ]
if "39X" in scenario and "data" in scenario:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_Data_Dec22B.root" ]
if scenario.find("signal_mc") != -1:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_DYToMMpowhegZ2_Spring11PU.root" ]
if scenario.find("some_mc") != -1:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_MCDYPoweg_38X_dzIso.334pb.root" ]
if scenario.find("39X_mc") != -1:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_MC_39XDY_1.root" ]
if scenario.find("some_mc2") != -1:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_DYToMMpowhegZ2_Fall10PU.root" ]
if scenario.find("39X_mc2") != -1:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_DYToMMpowhegZ2_Winter10PU.root" ]
if scenario.find("realistic_mc") != -1:
    process.TnP_MuonID.InputFileNames = [ PREFIX+"tnpZ_DYToMuMu_Spring11.347pb.root", 
                                          PREFIX+"tnpZ_DYToTauTau_Spring11.347pb.root", 
                                          PREFIX+"tnpZ_WJets_Spring11.347pb.root", 
                                          PREFIX+"tnpZ_QCDMu15_Spring11.347pb.root", 
                                          PREFIX+"tnpZ_TTbar_Spring11.347pb.root", ]
if "tag35" in scenario:
    process.TnP_MuonID.Variables.tag_pt[1]='35'

    
if scenario == "SingleMu-05Aug2011" or scenario == "SingleMu-May10ReReco" or scenario == "SingleMu-PromptReco-v4" or scenario == "SingleMu-PromptReco-v6":
    process.TnP_MuonID.InputFileNames = [ PREFIX+"hadd-"+scenario+".root" ]
elif scenario == "SingleMu-All-v0":
    process.TnP_MuonID.InputFileNames = [ PREFIX+"hadd-SingleMu-May10ReReco.root",
                                          PREFIX+"hadd-SingleMu-PromptReco-v4.root",
                                          PREFIX+"hadd-SingleMu-05Aug2011.root",
                                          PREFIX+"hadd-SingleMu-PromptReco-v6.root"
                                          ]



#IDS = [ "TMOST", "VBTF", "PF", "Glb", "passingIso", "passingKin", "passingMuon", "passingTrk" ]
IDS = ["VBTF", "passingIso", "passingTrk" ]

#IDS += [ "Glb" ]
ALLBINS = [("pt_abseta",PT_ETA_BINS)]
#ALLBINS += [("eta", ETA_BINS)]
ALLBINS += [ ("vtxDA",VTXDA_BINS)]
#ALLBINS += [("overall_abseta",OVERALL_ABSETA)]

#ALLBINS += [ ("vtx",VTX_BINS)]

#ALLBINS+=[("eta_fine",ETA_BINS_FINE)]
#ALLBINS += [("overall",OVERALL), ("charge",CHARGE), ("overall_abseta",OVERALL_ABSETA),("overall_endcaps21",OVERALL_ENDCAPS21)]
#ALLBINS+=[("eta_phi",ETA_PHI_BINS)]
#ALLBINS+=[("lumi_pt", LUMI_BINS)]

print "ALLBINS:"
print ALLBINS

if len(args) > 1 and args[1] not in IDS: IDS += [ args[1] ]
for ID in IDS:
    if len(args) > 1 and ID != args[1]: continue
    for X,B in ALLBINS:
        if len(args) > 2 and X not in args[2:]: continue
        module = process.TnP_MuonID.clone(OutputFileName = cms.string("TnP_MuonID_%s_%s_%s.root" % (scenario, ID, X)))
        shape = "vpvPlusExpo"
        #if "eta" in X and not "abseta" in X: shape = "voigtPlusExpo"
        #if "pt_abseta" in X: shape = "voigtPlusExpo"
        if X.find("pt_abseta") != -1: module.Variables.mass[1]="77";
        if X.find("overall") != -1: module.binsForFit = 120
        DEN = B.clone(); num = ID;
        if "24" in ID and hasattr(DEN,'pt') and "pt" not in X: DEN.pt[0] = 25
        if "_from_" in ID:
            parts = ID.split("_from_")
            num = parts[0]
            setattr(DEN, parts[1], cms.vstring("pass"))
        if scenario.find("tagiso") != -1:  
            DEN.tag_combRelIso = cms.vdouble(-1, 0.1)
        if scenario.find("loosetagiso") != -1:  
            DEN.tag_combRelIso = cms.vdouble(-1, 0.2)
        if scenario.find("probeiso") != -1:
            DEN.isoTrk03Abs = cms.vdouble(-1, 3)
        #if scenario.find("calo") != -1: DEN.caloCompatibility = cms.vdouble(0.9,1.1)  # same as above, I think.
        if "mc" in scenario:
            if num == "Mu24": num = "Mu15"
            if num == "IsoMu17": num = "IsoMu15"
            if num == "DoubleMu7": num = "DoubleMu3"
            if num == "Mu8_forEMu": num = "DoubleMu3"
            if num == "Mu17_forEMu": num = "DoubleMu3"
        if "EG5" in scenario: DEN.pair_nL1EG5 = cms.vdouble(0.5,999)
        setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(num,"pass"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = DEN,
            BinToPDFmap = cms.vstring(shape)
        ))
        if scenario.find("mc") != -1:
            setattr(module.Efficiencies, ID+"_"+X+"_mcTrue", cms.PSet(
                EfficiencyCategoryAndState = cms.vstring(num,"pass"),
                UnbinnedVariables = cms.vstring("mass"),
                BinnedVariables = DEN.clone(mcTrue = cms.vstring("true"))
            ))
        setattr(process, "TnP_MuonID_"+ID+"_"+X, module)        
        setattr(process, "run_"+ID+"_"+X, cms.Path(module))
        print "DEN:"
        print DEN


temp = process.dumpPython()
outputfile = file("fitMuonID_dump_" + scenario + ".py",'w')
outputfile.write(temp)
outputfile.close()
