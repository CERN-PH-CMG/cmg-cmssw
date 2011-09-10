import FWCore.ParameterSet.Config as cms

### USAGE:
###    cmsRun fitTrigger_Z.py <scenario>
### scenarios:
###   - data_all:    will fit tnpJPsi_Data.root with bins suitable for the current data
###   - datalike_mc: will fit tnpJPsi_{JPsiMuMu,ppMuX}_Spring10_0.117pb.root MC but
###                  with same config as data

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

process.TnP_Trigger = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    InputFileNames = cms.vstring('tnpZ_Data.root'),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("/data/Analysis/TnP/TnP_Z_Trigger_%s.root" % scenario),

    Variables = cms.PSet(
        run = cms.vstring("Run #", "1", "300000", ""),
        mass = cms.vstring("Tag-Probe Mass", "70", "130", "GeV/c^{2}"),
        pt     = cms.vstring("Probe p_{T}", "0", "1000", "GeV/c"),
        eta    = cms.vstring("Probe |#eta|", "-2.5", "2.5", ""),
        abseta = cms.vstring("Probe |#eta|", "0", "2.5", ""),
        tag_pt = cms.vstring("Tag p_{T}", "2.6", "1000", "GeV/c"),
        avgInsDelLumi = cms.vstring("Avg. Inst. Lumi", "200", "2000", "Hz/ub"),
	tag_nVerticesDA = cms.vstring("Number of vertices", "0", "999", ""),
        tag_combRelIso = cms.vstring("Tag comb rel iso", "-2", "9999999", ""),
					     
    ),

    Categories = cms.PSet(
        Calo = cms.vstring("POG_Glb",  "dummy[pass=1,fail=0]"),
        Glb  = cms.vstring("POG_Glb",  "dummy[pass=1,fail=0]"),
        VBTF = cms.vstring("VBTFLike", "dummy[pass=1,fail=0]"),
        Isol = cms.vstring("MC true",  "dummy[pass=1,fail=0]"),
        Mu9  = cms.vstring("MC true",  "dummy[pass=1,fail=0]"),
        passingIso = cms.vstring("Pass ISO", "dummy[pass=1,fail=0]"),
        passingMuon = cms.vstring("Pass Muon", "dummy[pass=1,fail=0]"),
        passingTrk = cms.vstring("Pass Trk", "dummy[pass=1,fail=0]"),
        passingKin = cms.vstring("Pass Kin", "dummy[pass=1,fail=0]"),
        IsoMu17 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),                         
        IsoMu24 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),                         
        DoubleMu7 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),                                
        tag_IsoMu17 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),
        tag_Mu24 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),
	DoubleMu13Mu8_Mu13 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),
	DoubleMu13Mu8_Mu8 = cms.vstring('MC true', 'dummy[pass=1,fail=0]'),			     
    ),

    PDFs = cms.PSet(
        gaussPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[90,80,100], width[2.495], sigma[3,1,20])",
            "Exponential::backgroundPass(mass, lp[0,-5,5])",
            "Exponential::backgroundFail(mass, lf[0,-5,5])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        )
    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(40),

    Efficiencies = cms.PSet(), # will be filled later
)
ONE_BIN = cms.PSet(
    pt     = cms.vdouble(15, 100),
    abseta = cms.vdouble( 0, 2.1)
)
PT_BINS  = ONE_BIN.clone(pt = cms.vdouble(15, 35, 100))
ETA_BINS = ONE_BIN.clone(abseta = cms.vdouble(0, 1.2, 2.1))

PT_ETA_BINS = cms.PSet(
    pt     = cms.vdouble(  20, 30, 100 ),
    abseta = cms.vdouble(  0.0, 0.8, 1.2, 2.4),
#    tag_Mu24 = cms.vstring("pass")
)
LUMI_BINS = cms.PSet(
    pt     = cms.vdouble(20,100),
    avgInsDelLumi = cms.vdouble(200,400,800,1000,1200,1400,1600,1800,2000)
    )

VTXDA_BINS  = cms.PSet(
    pt     = cms.vdouble(  20, 120 ),
    abseta = cms.vdouble(  0.0, 2.4),
    tag_nVerticesDA = cms.vdouble(0.5,2.5,4.5,6.5,8.5,10.5,12.5,14.5,20.5),
    tag_combRelIso = cms.vdouble(  -2, 0.1)
    #,
    #pair_dz = cms.vdouble(  -2, 2)
)



if scenario == "data_all":
    process.TnP_Trigger.binsForMassPlots = cms.uint32(20)

if scenario == "datalike_mc":
    process.TnP_Trigger.InputFileNames = [ "tnpZ_MC.root", ]

PREFIX="file:/data/Analysis/jobSubmission/muTnP_v1/"

if scenario == "SingleMu-05Aug2011" or scenario == "SingleMu-May10ReReco" or scenario == "SingleMu-PromptReco-v4" or scenario == "SingleMu-PromptReco-v6":
    process.TnP_Trigger.InputFileNames = [ PREFIX+"hadd-"+scenario+".root" ]
elif scenario == "SingleMu-All-v0":
    process.TnP_Trigger.InputFileNames = [#PREFIX+"hadd-SingleMu-May10ReReco.root",
                                          PREFIX+"hadd-SingleMu-PromptReco-v4.root",
                                          PREFIX+"hadd-SingleMu-05Aug2011.root",
                                          PREFIX+"hadd-SingleMu-PromptReco-v6.root"
                                          ]



ALLBINS=[]
#ALLBINS=[("all",ONE_BIN),("pt",PT_BINS),("eta",ETA_BINS)]
#ALLBINS += [("pt_abseta",PT_ETA_BINS)]
#ALLBINS+=[("lumi_pt", LUMI_BINS)]
ALLBINS += [("nVtxDA",VTXDA_BINS)]

ALLCOMBINATIONS = []
#ALLCOMBINATIONS += [ ("Mu9","Track")]
#ALLCOMBINATIONS += [("IsoMu17","Track")]

#ALLCOMBINATIONS += [("IsoMu17","passingIso")]
#ALLCOMBINATIONS += [("IsoMu17","Track")]

ALLCOMBINATIONS += [("DoubleMu7","passingIso")]
#ALLCOMBINATIONS += [("DoubleMu7","Track")]


#ALLCOMBINATIONS += [("DoubleMu13Mu8_Mu8","Track")]
#ALLCOMBINATIONS += [("DoubleMu13Mu8_Mu8","passingIso")]

# this defines the intervals for which a give triger is unprescaled
RUNBINS = {}
RUNBINS["IsoMu17"] = cms.vdouble(163262, 167043)
RUNBINS["DoubleMu7"] = cms.vdouble(1, 164237)
RUNBINS["DoubleMu13Mu8_Mu13"] = cms.vdouble(165088,300000)
RUNBINS["DoubleMu13Mu8_Mu8"] = cms.vdouble(165088,300000)
RUNBINS["IsoMu24"] = cms.vdouble(160431,300000)



for (T,M) in ALLCOMBINATIONS:
        for BN,BV in ALLBINS:
            BINNEDVARS = BV.clone()
            if M == "VBTF_Isol":
                setattr(BINNEDVARS, "VBTF", cms.vstring("pass"))
                setattr(BINNEDVARS, "Isol", cms.vstring("pass"))
            elif M != "Track": 
                setattr(BINNEDVARS, M, cms.vstring("pass"))
            # NOTE: trigger requirement on the tag.
            #setattr(BINNEDVARS, "tag_IsoMu17", cms.vstring("pass"))
            #setattr(BINNEDVARS, "tag_Mu24", cms.vstring("pass"))
	    # add a bin to restrict to the runs where the trigger was unprescaled
	    setattr(BINNEDVARS, "run", RUNBINS[T]) 
	    if "Double" in T:
		    setattr(BINNEDVARS, "tag_Mu24", cms.vstring("pass"))
            setattr(process.TnP_Trigger.Efficiencies, M+"_To_"+T+"_"+BN, cms.PSet(
                EfficiencyCategoryAndState = cms.vstring(T,"pass"),
                UnbinnedVariables = cms.vstring("mass"),
                BinnedVariables = BINNEDVARS,
                BinToPDFmap = cms.vstring("gaussPlusExpo")
            ))
# for  X,B in ALLBINS:
#     setattr(process.TnP_Trigger.Efficiencies, "Track_To_VBTF_Mu9_"+X, cms.PSet(
#         EfficiencyCategoryAndState = cms.vstring("VBTF","pass","Mu9","pass"),
#         UnbinnedVariables = cms.vstring("mass"),
#         BinnedVariables = B,
#         BinToPDFmap = cms.vstring("gaussPlusExpo")
#     ))
#     setattr(process.TnP_Trigger.Efficiencies, "Track_To_VBTF_Isol_Mu9_"+X, cms.PSet(
#         EfficiencyCategoryAndState = cms.vstring("VBTF","pass","Isol","pass","Mu9","pass"),
#         UnbinnedVariables = cms.vstring("mass"),
#         BinnedVariables = B,
#         BinToPDFmap = cms.vstring("gaussPlusExpo")
#     ))


process.p = cms.Path(
    process.TnP_Trigger
)


temp = process.dumpPython()
outputfile = file("fitTrigger_dump_" + scenario + ".py",'w')
outputfile.write(temp)
outputfile.close()
