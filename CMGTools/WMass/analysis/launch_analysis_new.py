#!/usr/bin/env python

import string, os, shutil, sys, subprocess

def file_exists_and_is_not_empty(fpath):
  return (os.path.isfile(fpath) and (os.path.getsize(fpath) > 0))

## ==============================================================
## STEERING PARAMETERS
## ==============================================================

useLHAPDF = False

outfolder_prefix="PREFIX";

ntuple_basepath = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/";
ntuple_basepath_8TeV_ABC = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_8TeV/";
# ntuple_basepath = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/";
# ntuple_basepath = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_10_15/";
# ntuple_basepath = "root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
# ntuple_basepath = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/";
# ntuple_basepath = "~/eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
lhapdf_path="/afs/cern.ch/work/p/perrozzi/private/WMassMC/lhapdf/"

use_PForNoPUorTKmet = 2; # 0:PF, 1:NOPU, 2:TK
use_LHE_weights = 0; # 0=no, 1=yes
usePileupSF = 1; # 0=no, 1=yes
useEffSF = 2; # 0=no, 1=MuonPOG, 2=Heiner
usePtSF = 0; # Boson pT reweighting: -1=none, 0=data, 1...=other options

### MUON
useMomentumCorr = 4; # 0=none, 1=Rochester, 2=MuscleFit, 3=KalmanCorrector, 4=KalmanCorrectorParam
MuonCorrGlobalScaleNsigma = 0; # vary global muon scale    (0=no)
MuonCorrKalmanNvarsNsigma = 0; # vary each muon fit eigenv (0=no)

WlikeCharge = 1; # Charge of the Wlike (+1,-1)

### RECOIL
useRecoilCorr = 2; # 0=none, 1=yes, 2=PDFw3gaus
RecoilCorrVarDiagoParU1orU2fromDATAorMC = 0; # SYST VARIATIONS: 0=NONE, 1= U1 DATA p1, 2= U1 DATA p2, 3= U2 DATA, 4= U1 MC p1, 5= U1 MC p1, 6= U1 MC p1
RecoilCorrVarDiagoParSigmas = 0; # Number of sigmas for recoil syst
correctToMadgraph = 0; # 0: uses DATA as target - 1: uses Madgraph as target

usePhiMETCorr = 0; # 0=none, 1=yes

### EWK CORR
syst_ewk_Alcaraz = -1; # -1=none, 0=fixed POWHEG QCD+EWK NLO

# LHAPDF_reweighting_sets="11200"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="51"  # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
# LHAPDF_reweighting_sets="10042"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="1"   # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
# LHAPDF_reweighting_sets="232000" # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="1"   # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
LHAPDF_reweighting_sets="229800" # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
LHAPDF_reweighting_members="1"   # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
# LHAPDF_reweighting_sets="11000"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="53"  # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
## CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
IS_MC_CLOSURE_TEST= 0;

indip_normalization_lumi_MC = 0; # independent normalization of MC in fb-1 (otherwise normalized to DATA)
intLumi_MC_fb = 81293448/31314/1e3;# data = 4.7499 fb-1 prescaled trigger, 5.1 fb-1 unprescaled; works only if indip_normalization_lumi_MC is TRUE
useAlsoGenPforSig= 1;

ZMassCentral_MeV = "91188"; # 91.1876
WMassCentral_MeV = "80398"; # 80.385
WMassSkipNSteps = "5"; # 15
etaMuonNSteps = "1"; # 5
etaMaxMuons = "0.9"; # 0.6, 0.8, 1.2, 1.6, 2.1

# DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadSig,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW
resubmit_sample = "DATA, WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW"
# resubmit_sample = "DYJetsPow" # DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadSig,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW
# resubmit_sample = "DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW"

useBatch = 0;
batchQueue = "1nh";
WMassNSteps = "5"; # 60 -- N of mass steps above and below the central
# WMassNSteps = "0"; # 60

runWanalysis = 0;
runZanalysis = 1;
controlplots = 0;
noLSFJobOutput = 0; # 1: Puts all the batch logs in a single file
recreateSubPrograms = 0; # 1: Recompiles run?analysis.o and remakes run?analysis.sh

mergeSigEWKbkg = 0;
removeChunks = 0; # 0: Don't remove chunks after merge - 1: Remove them

## PERFORM W or Z MASS FIT
fit_W_or_Z = "Z" # "W,Z" or "W" or "Z"

usePowOrMadForSig = "POWHEG"; # use "POWHEG" or use "MADGRAPH"
runPrepareDataCardsFast = 0; # ALTERNATIVE FAST WAY: TEMPLATES ARE IN THE TEMPLATE FOLDER, PSEUDO-DATA IN THE LOCAL FOLDER
DataCards_templateFromFolder="" # evaluate systematics wrt folder (or leave it empty) -- full template folder

## NEW FIT
runClosureTestLikeLihoodRatio = 0; # 1: also executes merge if not using batch jobs
mergeResults = 0;

#######################
### PLOTTING ###
#######################
runWSigBkgFit = 0;
runZSigBkgFit = 0;

run_Z_MCandDATAcomparisons_stack = 0;
run_W_MCandDATAcomparisons_stack = 0;

#######################
## OBSOLETE STUFF
#######################

## OLD FIT
runPrepareDataCards = 0;     # TEMPLATES ARE IN THE LOCAL FOLDER, PSEUDO-DATA IN THE SYsT FOLDER
if  (use_PForNoPUorTKmet==0): # 0:PF, 1:NOPU, 2:TK
  sysfoldmet="_pfmet";
elif(use_PForNoPUorTKmet==1): # 0:PF, 1:NOPU, 2:TK
  sysfoldmet="_pfnopu";
elif(use_PForNoPUorTKmet==2): # 0:PF, 1:NOPU, 2:TK
  sysfoldmet="_tkmet";

DataCards_systFromFolder=outfolder_prefix+sysfoldmet+"_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr" # evaluate systematics wrt folder (or leave it empty)
# Old code sections use DataCards_systFromFolder (like runPrepareDataCards),
# in which you put the full systematics folder.
# New sections use DataCards_templateFromFolder in which you put the full template folder

runDataCardsParametrization = 0; # NOT REALLY USED

## END STEERING PARAMETERS
## ============================================================== #
## ============================================================== #
## ============================================================== #

# Check if it's running from the correct dir
base_path = os.getcwd()
if os.path.dirname(os.path.realpath(__file__)) == base_path:
  print "Working in:"
  print base_path
else:
  print "You need to execute this script from the 'analysis' directory"
  sys.exit(1)

# if usePhiMETCorr != 0 \
# or syst_ewk_Alcaraz != 0
if RecoilCorrVarDiagoParU1orU2fromDATAorMC != 0 \
or LHAPDF_reweighting_members !="1" \
or MuonCorrGlobalScaleNsigma != 0 \
or MuonCorrKalmanNvarsNsigma != 0 :
  print "Computing a systematic: number of mass steps is set to 0\n"
  WMassNSteps = "0"

if RecoilCorrVarDiagoParU1orU2fromDATAorMC != 0 \
and RecoilCorrVarDiagoParSigmas == 0 :
  print "ERROR: Selected recoil correction set " + str(RecoilCorrVarDiagoParU1orU2fromDATAorMC) + " with 0 sigmas"
  print "Check the 'RecoilCorrVarDiagoParSigmas' variable"
  sys.exit(1)

if (WlikeCharge != 1) and (WlikeCharge != -1) :
  print "ERROR: Asked for a Wlike of charge", WlikeCharge
  print "Check the 'WlikeCharge' variable"
  sys.exit(1)

# Muon internal (Zanalisys wants them this way)
MuonCorrNsigma = 0
MuonCorrKalmanNparameters = 1; # number of muon fit params (1: no eigen var - 45: KalmanCorrectorParam)
if(MuonCorrGlobalScaleNsigma!=0 and MuonCorrKalmanNvarsNsigma!=0):
  print "Muon global scale and fit eigenvalues cannot be varied simultaneously (with the current Zanalysis.C)"
  sys.exit(1)
if(MuonCorrGlobalScaleNsigma!=0):
  MuonCorrKalmanNparameters = 1
  MuonCorrNsigma = MuonCorrGlobalScaleNsigma
if(MuonCorrKalmanNvarsNsigma!=0):
  MuonCorrKalmanNparameters=45
  MuonCorrNsigma = MuonCorrKalmanNvarsNsigma

# Build outfolder name

outfolder_name = outfolder_prefix

if (WlikeCharge == 1):
  outfolder_name+="_muPos"
else:
  outfolder_name+="_muNeg"

if(use_PForNoPUorTKmet==0): # 0:PF, 1:NOPU, 2:TK
  outfolder_name+="_pfmet";
elif(use_PForNoPUorTKmet==1): # 0:PF, 1:NOPU, 2:TK
  outfolder_name+="_pfnopu";
elif(use_PForNoPUorTKmet==2): # 0:PF, 1:NOPU, 2:TK
  outfolder_name+="_tkmet";

if(use_LHE_weights==1):
  outfolder_name+="_LHEweights";

if(IS_MC_CLOSURE_TEST==1):
  outfolder_name+="_MCclosureTest";
  # useEffSF=0;
  # usePtSF=0;
  # usePileupSF=0;

if(syst_ewk_Alcaraz>-1):
  outfolder_name+="_ewk"+str(syst_ewk_Alcaraz);

if  (useMomentumCorr==1):
  outfolder_name+="_RochCorr";
elif(useMomentumCorr==2):
  outfolder_name+="_MuscleFitCorr";
elif(useMomentumCorr==3):
  outfolder_name+="_KalmanCorr";
elif(useMomentumCorr==4):
  outfolder_name+="_KalmanCorrParam";

if(useMomentumCorr!=0):
  if(MuonCorrGlobalScaleNsigma != 0):
    outfolder_name+="_globalScaleSigma"+str(MuonCorrGlobalScaleNsigma)
  if(MuonCorrKalmanNvarsNsigma != 0):
    outfolder_name+="_KalmanVarsNSigma"+str(MuonCorrKalmanNvarsNsigma)

if(usePhiMETCorr==1):
  outfolder_name+="_phiMETcorr";

if(useRecoilCorr>0):
  outfolder_name+="_RecoilCorr"+str(useRecoilCorr);
  if(correctToMadgraph):
    outfolder_name+="_toMad";
  if  (RecoilCorrVarDiagoParU1orU2fromDATAorMC==1):
    outfolder_name+="_U1Datap1";
  elif(RecoilCorrVarDiagoParU1orU2fromDATAorMC==2):
    outfolder_name+="_U1Datap2";
  elif(RecoilCorrVarDiagoParU1orU2fromDATAorMC==3):
    outfolder_name+="_U2Data";
  elif(RecoilCorrVarDiagoParU1orU2fromDATAorMC==4):
    outfolder_name+="_U1MCp1";
  elif(RecoilCorrVarDiagoParU1orU2fromDATAorMC==5):
    outfolder_name+="_U1MCp2";
  elif(RecoilCorrVarDiagoParU1orU2fromDATAorMC==6):
    outfolder_name+="_U2MC";
  if  (RecoilCorrVarDiagoParSigmas!=0):
    outfolder_name+="_RecCorrNSigma_"+str(RecoilCorrVarDiagoParSigmas)

if(useEffSF==1): outfolder_name+="_EffSFCorr";
if(useEffSF==2): outfolder_name+="_EffHeinerSFCorr";
if(int(usePtSF)!=-1): outfolder_name+="_PtSFCorr"+str(usePtSF);
if(usePileupSF==1): outfolder_name+="_PileupSFCorr";

## END INITIAL CHECKS AND FOLDERNAME BUILDING
## ============================================================== #
## ============================================================== #
## ============================================================== #

DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadSig,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW = range(19)

sample      = [ "DATA" , "WJetsPowPlus", "WJetsPowNeg", "WJetsMadSig",    "WJetsMadFake", "DYJetsPow", "DYJetsMadSig", "DYJetsMadFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets",   "QCD", "T_s", "T_t", "T_tW", "Tbar_s", "Tbar_t", "Tbar_tW"];
isMCorDATA  = [      1 ,            0  ,          0   ,            0 ,                0 ,         0  ,            0  ,              0 ,         0,         0,         0,        0,       0,     0,     0,      0,        0,        0,     0    ];
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections or MCM
#                               POWHEG            POWHEG        MADGRAPH     MADGRAPH      POWHEG         POWHEG      twiki       twiki     twiki     twiki     twiki  twiki  twiki  twiki  twiki  twiki  twiki
CS_pb       = [     -1 ,         5913  ,        4126  ,        31314 ,            31314 ,       950  ,        2895.6 ,         2895.6 ,      165 ,       5.9,      43  ,    18.2 , 84679.3,  2.72,  42.6,    5.3,     1.49,       22,       5.3];
Nevts       = [141327580,     131310100,    109963100 ,      77571005,          77571005,    60235900,      34705858 ,       34705858 ,  54618956,  3899263 ,  3935865 ,  3860146,24939986,229786,3249552,744859,   139258,  1943163,    801626]; # 53X
int_lumi_fb = [     5.1 ,           0  ,            0 ,           0  ,                0 ,          0 ,              0,              0 ,       0  ,       0  ,        0 ,        0,       0,     0,      0,     0,        0,        0,         0];
# int_lumi_fb = [       (81293448/31314/1e3) ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ];
Nevts_Per_fb= [      0  ,            0 ,          0   ,            0 ,                0 ,          0 ,            0  ,             0  ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,         0];
generated_PDF_set =     [LHAPDF_reweighting_sets,229800,229800, 10042,            10042 ,      229800,         10042 ,          10042 ,    10042 ,     10042,     10042,    10042,   10042, 10042,  10042, 10042,    10042,    10042,     10042];
generated_PDF_member =  [LHAPDF_reweighting_members,0 ,       0 ,   0,                0 ,          0 ,             0 ,              0 ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,        0 ];
gen_mass_value_MeV  =   [  0  ,  80398 ,        80398 ,         80419,            91188 ,       91188,         91188 ,              0 ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,        0 ];
contains_LHE_weights =  [  0  ,      1 ,            1 ,             0,                0 ,           1,             0 ,              0 ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,        0 ];
nsamples=len(sample);

if(use_LHE_weights==0):
  for i in range(len(contains_LHE_weights)):
    contains_LHE_weights[i] = 0

### FROM THE POWHEG CONFIGURATION FILE ###
# ZMassCentral_MeV = "91.188"; # 91.1876
# WMassCentral_MeV = "80419"; # 80385

dummy_deltaM_MeV = []
dummy_deltaM_MeV_central_Index = 0
counter=0

for x in xrange(-302,-102,4):
  dummy_mass_step = float(x)
  dummy_deltaM_MeV.insert(counter,"%.0f" % dummy_mass_step)
  # print counter, x, dummy_mass_step, dummy_deltaM_MeV[counter]
  counter=counter+1

for x in xrange(-102,102,2):
  dummy_mass_step = float(x)
  if(dummy_mass_step==0): dummy_deltaM_MeV_central_Index = counter
  dummy_deltaM_MeV.insert(counter,"%.0f" % dummy_mass_step)
  # print counter, x, dummy_mass_step, dummy_deltaM_MeV[counter]
  counter=counter+1

for x in xrange(102,302,4):
  dummy_mass_step = float(x)
  dummy_deltaM_MeV.insert(counter,"%.0f" % dummy_mass_step)
  # print counter, x, dummy_mass_step, dummy_deltaM_MeV[counter]
  counter=counter+1

# print dummy_deltaM_MeV
# print dummy_deltaM_MeV_central_Index

Wmass_values_array = ""
Zmass_values_array = ""
if(WMassSkipNSteps=="0"): WMassSkipNSteps = "1"
counter=0

for i in xrange(-int(WMassNSteps),int(WMassNSteps)+1):
  WmassJoin_string = str(gen_mass_value_MeV[WJetsPowPlus]+int(round(float(dummy_deltaM_MeV[dummy_deltaM_MeV_central_Index+(int(WMassSkipNSteps))*i]))))
  if(counter>0): Wmass_values_array = ''.join((Wmass_values_array, ","))
  Wmass_values_array = ''.join((Wmass_values_array, WmassJoin_string))
  ZmassJoin_string = str(gen_mass_value_MeV[DYJetsPow]+int(round(float(dummy_deltaM_MeV[dummy_deltaM_MeV_central_Index+(int(WMassSkipNSteps))*i]))))
  if(counter>0): Zmass_values_array = ''.join((Zmass_values_array, ","))
  Zmass_values_array = ''.join((Zmass_values_array, ZmassJoin_string))
  counter = counter+1

# print Wmass_values_array
# print Zmass_values_array

##################################################
### TEMP: WAITING TO RECOVER THE FULL DATASET ####
##################################################
Nevts[DYJetsPow] /= 2
##################################################

fWana_str = [
  ntuple_basepath+"DATA/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_basepath+"WPlusPOWHEG/WTreeProducer_tree.root",
  ntuple_basepath+"WMinusPOWHEG/WTreeProducer_tree.root",
  ntuple_basepath+"WJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"WJetsLL/WTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_basepath+"DYJetsMM/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"DYJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"DYJetsLL/WTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_basepath+"TTJets/WTreeProducer_tree.root",
  ntuple_basepath+"VVJets/ZZ/WTreeProducer_tree.root",
  ntuple_basepath+"VVJets/WW/WTreeProducer_tree.root",
  ntuple_basepath+"VVJets/WZ/WTreeProducer_tree.root",
  ntuple_basepath+"QCD/QCD20Mu15/WTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/T_s/WTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/T_t/WTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/T_tW/WTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/Tbar_s/WTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/Tbar_t/WTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/Tbar_tW/WTreeProducer_tree.root"
];

fZana_str = [
#  ntuple_basepath_8TeV_ABC+"DATA_Run2012ABCD/ZTreeProducer_tree.root", # this is the 8TeV data contain also the tkmetABC
  ntuple_basepath+"DATA/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_basepath+"WJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"WJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"WJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"WJetsLL/ZTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_basepath+"DYJetsMM/ZTreeProducer_tree_SignalRecoSkimmed.root",
  # ntuple_basepath+"DYJetsMM/InclWeights/ZTreeProducer_tree.root",
  # ntuple_basepath+"DYJetsMM/allEvts/ZTreeProducer_tree.root",
#  ntuple_basepath_8TeV_ABC+"DYJetsLL/ZTreeProducer_tree_tkmetABC.root",  # this is the 8TeV DYJetsLL contains also the tkmetABC
  ntuple_basepath+"DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_basepath+"DYJetsLL/ZTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_basepath+"TTJets/ZTreeProducer_tree.root",
  ntuple_basepath+"VVJets/ZZ/ZTreeProducer_tree.root",
  ntuple_basepath+"VVJets/WW/ZTreeProducer_tree.root",
  ntuple_basepath+"VVJets/WZ/ZTreeProducer_tree.root",
  ntuple_basepath+"QCD/QCD20Mu15/ZTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/T_s/ZTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/T_t/ZTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/T_tW/ZTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/Tbar_s/ZTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/Tbar_t/ZTreeProducer_tree.root",
  ntuple_basepath+"SingleTop/Tbar_tW/ZTreeProducer_tree.root"
];

if not os.path.exists("JobOutputs/"+outfolder_name):
  print "Out folder doesn't exists, making directory"
  print "JobOutputs/"+outfolder_name
  os.makedirs("JobOutputs/"+outfolder_name)

print "Copying script over:"
print "cp "+os.path.basename(__file__)+" JobOutputs/"+outfolder_name;
print
path_dest="JobOutputs/"+outfolder_name+"/"+os.path.basename(__file__)
shutil.copyfile(os.path.basename(__file__), path_dest)

if(runWanalysis or runZanalysis):

  if(useLHAPDF and os.environ.get('LHAPATH') == lhapdf_path+"share/lhapdf/PDFsets"):
    print "ENVIRONMENT VARIABLES OK"
  else:
    print "REMEMBER TO SET ENVIRONMENT VARIABLES (MUST RUN ON LXPLUS):"
    # "cmsenv" and "source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh"
    if(useLHAPDF):
      print ("export PATH="+lhapdf_path+"bin/:$PATH")
      print ("export LD_LIBRARY_PATH="+lhapdf_path+"lib/:$LD_LIBRARY_PATH")
      print ("export LHAPATH="+lhapdf_path+"share/lhapdf/PDFsets")
      sys.exit()

  NPDF_sets = LHAPDF_reweighting_sets.count(',')+1
  if(LHAPDF_reweighting_sets!=""):
    PAR_PDF_SETS = LHAPDF_reweighting_sets
    PAR_PDF_MEMBERS = LHAPDF_reweighting_members
  else:
    PAR_PDF_SETS = "-1"
    PAR_PDF_MEMBERS = "1"

  ## check if common.h is already existing
  if not recreateSubPrograms and file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"/common.h"):
    print "using JobOutputs/"+outfolder_name+"/common.h"
  else:
    # build common.h from cfg
    print "Creating JobOutputs/"+outfolder_name+"/common.h from includes/common.h"
    shutil.copyfile("includes/common.h", "JobOutputs/"+outfolder_name+"/common.h");
    # Edit template
    os.system("sh "+base_path+"/utils/manipulate_parameters.sh "+ZMassCentral_MeV+" "+WMassCentral_MeV+" "+WMassSkipNSteps+" "+WMassNSteps+" "+etaMuonNSteps+" \""+etaMaxMuons+"\" "+str(NPDF_sets)+" "+str(PAR_PDF_SETS)+" "+str(PAR_PDF_MEMBERS)+" "+str(WlikeCharge)+" "+Wmass_values_array+" "+Zmass_values_array+" "+str(dummy_deltaM_MeV_central_Index)+" "+str(usePtSF)+" "+str(MuonCorrKalmanNparameters)+" "+"JobOutputs/"+outfolder_name+"/common.h")

  print ""

  os.chdir(base_path+"/JobOutputs/"+outfolder_name);
  code_dir = base_path + "/AnalysisCode/"

  ### Compiling run?analysis.o (if needed)

  if runWanalysis and (recreateSubPrograms or not file_exists_and_is_not_empty("runWanalysis.o")):
    # Copy Wanalysis.C in the dest folder (it has some parameters)
    shutil.copyfile(code_dir+"/Wanalysis.C", "Wanalysis.C")
    if(useLHAPDF):
      print("c++ -O2 -o runWanalysis.o -DLHAPDF_ON `root-config --glibs --libs --cflags` -I "+lhapdf_path+"/include -L "+lhapdf_path+"/lib -lLHAPDF  -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Wanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runWanalysis.C")
      os.system("rm -f runWanalysis.o; c++ -O2 -o runWanalysis.o -DLHAPDF_ON `root-config --glibs --libs --cflags` -I "+lhapdf_path+"/include -L "+lhapdf_path+"/lib -lLHAPDF  -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Wanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runWanalysis.C")
    else:
      print("c++ -O2 -o runWanalysis.o `root-config --glibs --libs --cflags` -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Wanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runWanalysis.C")
      os.system("rm -f runWanalysis.o; c++ -O2 -o runWanalysis.o `root-config --glibs --libs --cflags` -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Wanalysis.C "+code_dir+"common_stuff.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runWanalysis.C")

  if runZanalysis and (recreateSubPrograms or not file_exists_and_is_not_empty("runZanalysis.o")):
    # Copy Zanalysis.C in the dest folder (it has some parameters)
    shutil.copyfile(code_dir+"/Zanalysis.C", "Zanalysis.C")
    if(useLHAPDF):
      print("c++ -O2 -o runZanalysis.o -DLHAPDF_ON `root-config --glibs --libs --cflags`  -I "+lhapdf_path+"/include -L "+lhapdf_path+"/lib -lLHAPDF -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
      os.system("rm -f runZanalysis.o; c++ -O2 -o runZanalysis.o -DLHAPDF_ON `root-config --glibs --libs --cflags` -I "+lhapdf_path+"/include -L "+lhapdf_path+"/lib -lLHAPDF -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
    else:
      print("c++ -O2 -o runZanalysis.o `root-config --glibs --libs --cflags` -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
      os.system("rm -f runZanalysis.o; c++ -O2 -o runZanalysis.o `root-config --glibs --libs --cflags` -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"rochcor_44X_v3.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"KalmanCalibrator.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
  
  for i in range(0, nsamples):

    if (resubmit_sample!=""):
      if not sample[i] in resubmit_sample:
        print "skipping",sample[i],"\twhich is not in {",resubmit_sample,"}"
        continue

    if(IS_MC_CLOSURE_TEST and (sample[i]!="WJetsMadSig" and sample[i]!="DYJetsPow" and sample[i]!="DYJetsMadSig")):
      continue; # TEMPORARY

    sampleID= "output_"+sample[i];

    print ''
    print "ANALYZING sampleID =", sampleID ;

    WfileDATA= fWana_str[i];
    ZfileDATA= fZana_str[i];
    if( ("WJetsMadSig" in sample[i] or "WJetsPow" in sample[i]) and useAlsoGenPforSig): WfileDATA.replace("_SignalRecoSkimmed","");
    else:
      if( ("DYJetsPow" in sample[i]  or "DYJetsMadSig" in sample[i]) and useAlsoGenPforSig): ZfileDATA.replace("_SignalRecoSkimmed","");

    if(CS_pb[i] >0): int_lumi_fb[i] = Nevts[i]/CS_pb[i]/1e3;

    # if IS_MC_CLOSURE_TEST:
      # if indip_normalization_lumi_MC:
        # WfileDATA_lumi_SF = intLumi_MC_fb/int_lumi_fb[i]
      # else:
        # WfileDATA_lumi_SF = 1
    # else:
      # WfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i]
    # if IS_MC_CLOSURE_TEST:

    if indip_normalization_lumi_MC:
      WfileDATA_lumi_SF = intLumi_MC_fb/int_lumi_fb[i]
    else:
      WfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i]

    ZfileDATA_lumi_SF = WfileDATA_lumi_SF

    # WfileDATA_lumi_SF = ((intLumi_MC_fb/int_lumi_fb[i]) if indip_normalization_lumi_MC==1 else 1) if IS_MC_CLOSURE_TEST==1 else int_lumi_fb[DATA]/int_lumi_fb[i];
    # ZfileDATA_lumi_SF = ((intLumi_MC_fb/int_lumi_fb[i]) if indip_normalization_lumi_MC==1 else 1) if IS_MC_CLOSURE_TEST==1 else int_lumi_fb[DATA]/int_lumi_fb[i];
    print "lumi_SF =",ZfileDATA_lumi_SF

    #########################################/
    ## END OF STEERING PARAMETERS
    #########################################/

    ## RUNNING CODE

    if(sampleID==""): sampleIDfolder="dummytest";
    else: sampleIDfolder=sampleID;

    outputSamplePath = base_path+"/JobOutputs/"+outfolder_name+'/'+sampleIDfolder+'/'
    if not os.path.exists(outputSamplePath):
      os.makedirs(outputSamplePath)

    if(runWanalysis):

      wstring="\""+WfileDATA+"\","+str(WfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+outputSamplePath+"\","+str(useMomentumCorr)+","+str(MuonCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_LHE_weights[i])+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(0)+","+str(RecoilCorrVarDiagoParSigmas)+","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+","+str(use_PForNoPUorTKmet)+","+str(syst_ewk_Alcaraz)+","+str(gen_mass_value_MeV[i])+","+str(contains_LHE_weights[i])

      line = os.popen(base_path+"/JobOutputs/"+outfolder_name+"/runWanalysis.o -1,0,0,"+wstring).read()
      nEntries = [int(s) for s in line.split() if s.isdigit()][0]

      if not useBatch:
        os.chdir(code_dir)
        os.system(base_path+"/JobOutputs/"+outfolder_name+"/runWanalysis.o 0,0,"+str(nEntries)+","+wstring)
      else:
        nevents = 2e5
        # if (("DYJetsMadSig" in sample[i]  or "DYJetsPow" in sample[i]) and float(LHAPDF_reweighting_members)>1):
        # if ("WJetsMadSig" in sample[i]  or "WJetsPow" in sample[i]):
          # nevents = 3e4
        if ("DATA" in sample[i]):
          nevents = 5e5
        # if (WMassNSteps=="0"):
          # nevents = 1e6

        os.chdir(outputSamplePath)

        nChuncks =  int(nEntries/nevents)+2
        text_file = open("Wanalysis_nChuncks.log", "w")
        text_file.write(str(nChuncks-1))
        text_file.close()

        # Create script if needed
        if recreateSubPrograms>0 or not file_exists_and_is_not_empty("runWanalysis.sh"):
          text_file = open("runWanalysis.sh", "w")
          text_file.write("# x=$1; ev_ini=$2; ev_fin=$3\n")
          text_file.write("cd "+code_dir+"\n")
          text_file.write("eval `scramv1 runtime -sh`\n")
          text_file.write("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh \n")
          text_file.write(base_path+"/JobOutputs/"+outfolder_name+"/runWanalysis.o ${1},${2},${3},"+wstring)
          text_file.close()
          os.system("chmod 755 runWanalysis.sh")

        print "nChuncks ",nChuncks-1
        for x in xrange(1, nChuncks):
          ev_ini= int(nevents*(x-1))
          ev_fin= int(nevents*(x)-1)
          if (x==nChuncks-1):
            ev_fin= nEntries
          print x,ev_ini,ev_fin
          if not file_exists_and_is_not_empty("Wanalysis_chunk"+str(x)+".root"):
            LSFJobOutput = ''
            if noLSFJobOutput>0:
              #LSFJobOutput = '-o /dev/null'
              LSFJobOutput = "-o "+outputSamplePath+"/batch_logs_W.txt"
            jobname = "Wanalysis_"+outfolder_name+"_"+sample[i]+"_"+str(x)
            print "Submitting job: "+jobname
            os.system("bsub -C 0 "+LSFJobOutput+" -u pippo123 -q "+batchQueue+" -J "+jobname+" runWanalysis.sh "+str(x)+" "+str(ev_ini)+" "+str(ev_fin))
            os.system("usleep 10000");

    if(runZanalysis):

      zstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+outputSamplePath+"\","+str(useMomentumCorr)+","+str(MuonCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(0)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_LHE_weights[i])+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(correctToMadgraph)+","+str(RecoilCorrVarDiagoParSigmas)+","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+","+str(use_PForNoPUorTKmet)+","+str(syst_ewk_Alcaraz)+","+str(gen_mass_value_MeV[i])+","+str(contains_LHE_weights[i])

      line = os.popen(base_path+"/JobOutputs/"+outfolder_name+"/runZanalysis.o -1,0,0,"+zstring).read();
      nEntries = [int(s) for s in line.split() if s.isdigit()][0]

      if not useBatch:
        os.chdir(code_dir)
        os.system(base_path+"/JobOutputs/"+outfolder_name+"/runZanalysis.o 0,0,"+str(nEntries)+","+zstring)
      else:
        nevents = 2e5
        if ("DYJetsMadSig" in sample[i]  or "DYJetsPow" in sample[i]):
          nevents = 3e4
          if useRecoilCorr>0 and RecoilCorrVarDiagoParSigmas!=0:
            nevents = nevents/3
          if (MuonCorrKalmanNvarsNsigma!=0):
            nevents = nevents/3

        os.chdir(outputSamplePath)

        nChuncks =  int(nEntries/nevents)+2
        text_file = open("Zanalysis_nChuncks.log", "w")
        text_file.write(str(nChuncks-1))
        text_file.close()

        # Create script if needed
        if recreateSubPrograms>0 or not file_exists_and_is_not_empty("runZanalysis.sh"):
          text_file = open("runZanalysis.sh", "w")
          text_file.write("# x=$1; ev_ini=$2; ev_fin=$3\n")
          text_file.write("cd "+code_dir+"\n")
          text_file.write("eval `scramv1 runtime -sh`\n")
          text_file.write("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh \n")
          text_file.write(base_path+"/JobOutputs/"+outfolder_name+"/runZanalysis.o ${1},${2},${3},"+zstring)
          text_file.close()
          os.system("chmod 755 runZanalysis.sh")

        print "nChuncks ",nChuncks-1
        for x in xrange(1, nChuncks):
          ev_ini= int(nevents*(x-1))
          ev_fin= int(nevents*(x)-1)
          if (x==nChuncks-1):
            ev_fin= nEntries
          print x,ev_ini,ev_fin
          if not file_exists_and_is_not_empty("Zanalysis_chunk"+str(x)+".root"):
            LSFJobOutput = ''
            if noLSFJobOutput>0:
              #LSFJobOutput = '-o /dev/null'
              LSFJobOutput = "-o "+outputSamplePath+"/batch_logs_Z.txt"
            jobname = "Zanalysis_"+outfolder_name+"_"+sample[i]+"_"+str(x)
            print "Submitting job: "+jobname
            os.system("bsub -C 0 "+LSFJobOutput+" -u pippo123 -q "+batchQueue+" -J "+jobname+" runZanalysis.sh "+str(x)+" "+str(ev_ini)+" "+str(ev_fin))
            os.system("usleep 10000");

    os.chdir(base_path);

if(mergeSigEWKbkg):
  os.chdir("utils/");
  os.system("./merge_MC.sh \"../JobOutputs/"+outfolder_name+"/\"")
  os.chdir(base_path);
  os.system("find JobOutputs/"+outfolder_name+"/output_* -type d -name LSFJOB_* -exec rm -rf {} +")
  os.system("find JobOutputs/"+outfolder_name+"/output_* -type f -name batch_logs_* -delete")

if(removeChunks):
  if file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"output_MCDATALIKEMAD/WanalysisOnDATA.root") \
  or file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"output_MCDATALIKEMAD/ZanalysisOnDATA.root") \
  or file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"output_MCDATALIKEPOW/WanalysisOnDATA.root") \
  or file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"output_MCDATALIKEPOW/ZanalysisOnDATA.root") :
    print "Removing chunks from JobOutputs/"+outfolder_name
    os.system("find JobOutputs/"+outfolder_name+"/output_* -type f -name [WZ]analysis_chunk*.root -delete")
  else:
    print "Cannot find any merged histogram in "+outfolder_name+", not deleting chunks"
  os.chdir(base_path);

if(runPrepareDataCards):
  os.chdir("AnalysisCode/");
  if not os.path.exists("../JobOutputs/"+outfolder_name+"/DataCards"): os.makedirs("../JobOutputs/"+outfolder_name+"/DataCards")
  print "running .x prepareDatacards.C++(\"../JobOutputs/"+outfolder_name+"\",\"../JobOutputs/"+DataCards_systFromFolder+"\",\"\",1,1,\""+str(fit_W_or_Z)+"\")\'"
  os.system("root -l -b -q \'prepareDatacards.C++(\"../JobOutputs/"+outfolder_name+"\",\"../JobOutputs/"+DataCards_systFromFolder+"\",\"\",1,1,\""+str(fit_W_or_Z)+"\")\'")
  os.chdir(base_path);

if(runPrepareDataCardsFast):
  # common.h  is already in place, as is the one for the systematic
  # common2.h is got from the template folder, specified by the variable (same folder if empty)
  if (DataCards_templateFromFolder != ""):
    shutil.copyfile("JobOutputs/"+DataCards_templateFromFolder+"/common.h", "JobOutputs/"+outfolder_name+"/common2.h");
  else:
    shutil.copyfile("JobOutputs/"+outfolder_name+"/common.h", "JobOutputs/"+outfolder_name+"/common2.h");
  os.system("sed -i 's/.*namespace WMass{.*/namespace WMass2{/' JobOutputs/"+outfolder_name+"/common2.h")

  if not os.path.exists("JobOutputs/"+outfolder_name+"/DataCards"): os.makedirs("JobOutputs/"+outfolder_name+"/DataCards")
  shutil.copyfile(base_path+"/AnalysisCode/prepareDatacardsFast.C", base_path+"/JobOutputs/"+outfolder_name+"/prepareDatacardsFast.C");
  os.chdir("JobOutputs/"+outfolder_name);
  print "running .x prepareDatacardsFast.C++(\"./\",\"../"+DataCards_templateFromFolder+"\",\""+usePowOrMadForSig+"\",1,1,\""+str(fit_W_or_Z)+"\","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'"
  os.system("root -l -b -q \'prepareDatacardsFast.C++(\"./\",\"../"+DataCards_templateFromFolder+"\",\""+usePowOrMadForSig+"\",1,1,\""+str(fit_W_or_Z)+"\","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'")
  os.chdir(base_path);


if(runDataCardsParametrization):
  os.chdir("AnalysisCode/");
  print "running .x DataCardsParametrization.C(\"../JobOutputs/"+outfolder_name+"\",\"\")"
  os.system("root -l -b -q \'DataCardsParametrization.C(\"../JobOutputs/"+outfolder_name+"\",\"\")\'")
  os.chdir(base_path);


if(runClosureTestLikeLihoodRatio):
  shutil.copyfile("AnalysisCode/ClosureTest_fits_likelihoodratio.C","JobOutputs/"+outfolder_name+"/DataCards/ClosureTest_fits_likelihoodratio.C");
  os.chdir("JobOutputs/"+outfolder_name+"/DataCards");
  print "We are working in:\n" + os.getcwd() + "\n"
  os.system("rm -f "+os.getcwd()+"/ClosureTest_fits_C.*")
  os.system("user=$(whoami);"
        "cd /afs/cern.ch/work/${user:0:1}/${user}/private/CMSSW_6_1_1/src; SCRAM_ARCH=slc5_amd64_gcc462;"
        "eval `scramv1 runtime -sh`;"
        "cd -;"
        "source /afs/cern.ch/sw/lcg/contrib/gcc/4.6/x86_64-slc6-gcc46-opt/setup.sh;"
        "root -l -b -q \'ClosureTest_fits_likelihoodratio.C++(1,0,\""+str(fit_W_or_Z)+"\","+str(useBatch)+",\""+os.getcwd()+"\","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'")
  # proc=subprocess.Popen("ls "+os.getcwd()+"/submit_datacard_*", shell=True, stdout=subprocess.PIPE, )
  # a = proc.communicate()[0].rstrip().split('\n')
  # print a
  # os.system("user=$(whoami); cd /afs/cern.ch/work/${user:0:1}/${user}/private/CMSSW_6_1_1/src; SCRAM_ARCH=slc5_amd64_gcc462;eval `scramv1 runtime -sh`; cd -; source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh; root -l -b -q \'ClosureTest_fits.C++(1,0,\""+str(fit_W_or_Z)+"\")\'")
  os.chdir(base_path);

if(mergeResults or (runClosureTestLikeLihoodRatio and useBatch==0)):
  os.system("cp AnalysisCode/merge_results.C JobOutputs/"+outfolder_name+"/DataCards/merge_results.C");
  os.chdir("JobOutputs/"+outfolder_name+"/DataCards");
  print os.getcwd();
  os.system("rm -rf LSF*; rm -f output_W*.root");
  os.system("root -l -b -q \'merge_results.C++(1,0,\""+str(fit_W_or_Z)+"\","+str(useBatch)+","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'");
  os.chdir(base_path);


if(run_Z_MCandDATAcomparisons_stack):
  os.chdir("PlottingCode/");
  if not os.path.exists("../JobOutputs/"+outfolder_name+"/ZcomparisonPlots_MCvsDATA"): os.makedirs("../JobOutputs/"+outfolder_name+"/ZcomparisonPlots_MCvsDATA")
  sampleIDMCsig= "../JobOutputs/"+outfolder_name+"/output_"+sample[DYJetsPow]
  sampleIDMCEWK= "../JobOutputs/"+outfolder_name+"/output_EWK"
  sampleIDMCTT= "../JobOutputs/"+outfolder_name+"/output_"+sample[TTJets]
  sampleIDMCQCD= "../JobOutputs/"+outfolder_name+"/output_"+sample[QCD]
  sampleIDDATA= "../JobOutputs/"+outfolder_name+"/output_"+sample[DATA]
  os.system("root -l -b -q \'PlotZdistributionsMCvsDATA_stack.C(\""+sampleIDMCsig+"/\",\""+sampleIDMCEWK+"/\",\""+sampleIDMCTT+"/\",\""+sampleIDMCQCD+"/\",\""+sampleIDDATA+"/\")\' ");
  os.system("mv *.png ../JobOutputs/"+outfolder_name+"/ZcomparisonPlots_MCvsDATA");
  os.system("cp PlotZdistributionsMCvsDATA_stack.C ../JobOutputs/"+outfolder_name+"/ZcomparisonPlots_MCvsDATA");

if(run_W_MCandDATAcomparisons_stack):
  os.chdir("PlottingCode/");
  if not os.path.exists("../JobOutputs/"+outfolder_name+"/WcomparisonPlots_MCvsDATA"): os.makedirs("../JobOutputs/"+outfolder_name+"/WcomparisonPlots_MCvsDATA")
  sampleIDMCsig= "../JobOutputs/"+outfolder_name+"/output_"+sample[WJetsMadSig]
  sampleIDMCEWK= "../JobOutputs/"+outfolder_name+"/output_EWK"
  sampleIDMCTT= "../JobOutputs/"+outfolder_name+"/output_"+sample[TTJets]
  sampleIDMCQCD= "../JobOutputs/"+outfolder_name+"/output_"+sample[QCD]
  sampleIDDATA= "../JobOutputs/"+outfolder_name+"/output_"+sample[DATA]
  os.system("root -l -b -q \'PlotWdistributionsMCvsDATA_stack.C(\""+sampleIDMCsig+"/\",\""+sampleIDMCEWK+"/\",\""+sampleIDMCTT+"/\",\""+sampleIDMCQCD+"/\",\""+sampleIDDATA+"/\")\' ");
  os.system("mv *.png ../JobOutputs/"+outfolder_name+"/WcomparisonPlots_MCvsDATA");
  os.system("cp PlotWdistributionsMCvsDATA_stack.C ../JobOutputs/"+outfolder_name+"/WcomparisonPlots_MCvsDATA");

if(runWSigBkgFit):
  os.chdir("SignalExtraction/");
  os.system("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitWm.C+\(\\\""+outfolder_name+"/\\\"\)");
  os.chdir(base_path);

if(runZSigBkgFit):
  os.chdir("SignalExtraction/");
  print ("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitZmm.C+\(\\\""+outfolder_name+"/\\\"\)");
  os.system("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitZmm.C+\(\\\""+outfolder_name+"/\\\"\)");
  os.chdir(base_path);


print ''
