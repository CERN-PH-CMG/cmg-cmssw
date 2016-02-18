#!/usr/bin/env python

import os, shutil, sys, subprocess, ROOT

### Import config

sys.path.append("configdir")
try:
  conffile = sys.argv[1]
except IndexError:
  conffile = "config_nominal"
config = __import__(conffile)

## ==============================================================
## STEERING PARAMETERS
## ==============================================================

useLHAPDF = False

outfolder_prefix="profileFit"

ntuple_basepath = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/"
ntuple_basepathFIX = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2015_05_24_53X_sumEtFIX/"
ntuple_basepath_8TeV_ABC = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_8TeV/"
lhapdf_path="/afs/cern.ch/work/p/perrozzi/private/WMassMC/lhapdf/"

use_PForNoPUorTKmet = 2  # 0:PF, 1:NOPU, 2:TK
use_LHE_weights = 0  # 0=no, 1=yes
usePileupSF = 1  # 0=no, 1=yes
useEffSF = 2  # 0=no, 1=MuonPOG, 2=Heiner all, 3=Heiner no tight, 4=Heiner no iso, 5=Heiner no tight subleading mu, 6=Heiner no hlt
                                            # 13=Heiner tight 1%, 14=Heiner iso 1%, 15=Heiner tight subleading mu 1%, 16=Heiner hlt 1%
usePtSF = 0  # Boson pT reweighting: -1=none, 0=data, 1...=other options

### EWK CORR
syst_ewk_Alcaraz = 0  # -1=none, 0=POWHEG QCD+EWK NLO (bug-fixed), 1= 0 +syst photos vs pythia (31 = 3 times), 2= 0 +syst no nloewk vs nloewk (32 = 3 times)
### REWEIGHT POLARIZATION
reweight_polarization = 1  # 0 = none, 1 = reweight POW to DATA

# LHAPDF_reweighting_sets="11200"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="51"  # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
# LHAPDF_reweighting_sets="10042"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="1"   # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
# LHAPDF_reweighting_sets="232000" # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="1"   # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
LHAPDF_reweighting_sets="229800"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
LHAPDF_reweighting_members="1"   # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
# LHAPDF_reweighting_sets="11000"  # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="53"  # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
## CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
IS_MC_CLOSURE_TEST= 0

indip_normalization_lumi_MC = 0  # independent normalization of MC in fb-1 (otherwise normalized to DATA)
intLumi_MC_fb = 81293448/31314/1e3  # data = 4.7499 fb-1 prescaled trigger, 5.1 fb-1 unprescaled; works only if indip_normalization_lumi_MC is TRUE
useAlsoGenPforSig= 1
normalize_MC_to_half_of_the_data = 1  # useful for W-like because we use half of it to calibrate the recoil

### MUON
useMomentumCorr = 4  # 0=none, 1=Rochester, 2=MuscleFit, 3=KalmanCorrector, 4=KalmanCorrectorParam
MuonKalmanVariation = 0     # vary a muon fit eigenv 0=No, [1...45]=Yes
MuonScaleVariation = False  # vary global muon scale (True/False)
MuonVariationSigmas = 0     # n of sigmas for muon options

# Wlike properties
NMassValues = 11
Zmass_values = [91138, 91148, 91158, 91168, 91178, 91188, 91198, 91208, 91218, 91228, 91238]  # Masses of the Wlike
WlikeCharge = 1  # Charge of the Wlike (+1,-1)

etaMaxMuons = 0.9  # Leading muon cut
etaMaxMuons2 = 2.1  # Subleading muon cut

### RECOIL
useRecoilCorr = 2           # 0=none, 1=yes, 2=PDFw3gaus, 3=RooKeys
RecoilStatVariation = 0     # SYST VARIATIONS: 1...144
RecoilVariationSigmas = 0   # Number of sigmas for recoil syst
correctToMadgraph = 0       # 0: uses DATA as target -- 1: uses Madgraph as target (also needed to write recoil closure plots)

usePhiMETCorr = 0  # 0=none, 1=yes

# {DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadSig,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW}
resubmit_sample = "WJetsMadFake, DYJetsPow, DYJetsMadFake, TTJets, ZZJets, WWJets, WZJets, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW"
# resubmit_sample = "DATA"

useBatch = 1
batchQueue = "1nh"

runZanalysis = 1
controlplots = 0
noLSFJobOutput = 1  # 1: Puts all the batch logs in a single file
recreateSubPrograms = 0  # 1: Recompiles run?analysis and remakes run?analysis.sh

### MERGE ###
mergeSigEWKbkg = 0
mergeWhichAnalysis = "Zanalysis"  # "Zanalysis Wanalysis" -- no comma!
removeChunks = 0  # 0: Don't remove chunks after merge --- 1: Remove them

#######################
### FIT ###
#######################

## PERFORM W or Z MASS FIT
fit_W_or_Z = "Z" # "W,Z" or "W" or "Z"

usePowOrMadForSig = "POWHEG"  # use "POWHEG" or use "MADGRAPH"
runPrepareDataCardsFast = 0  # ALTERNATIVE FAST WAY: TEMPLATES ARE IN THE TEMPLATE FOLDER, PSEUDO-DATA IN THE LOCAL FOLDER
DataCards_templateFromFolder="" # evaluate systematics wrt folder (or leave it empty) -- full template folder

## NEW FIT
runClosureTestLikeLihoodRatio = 0  # 1: also executes merge if not using batch jobs
mergeResults = 0

## ============================================================== #
## END STEERING PARAMETERS
## ============================================================== #

## ============================================================== #
## START FUNCTION DEFINITIONS
## ============================================================== #

def batch_job_is_running(jobname,chunk):
  job_running = subprocess.Popen("bjobs -w", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.read()
  jobname = jobname+"["+chunk+"]"
  if jobname in job_running:
    return True
  else:
    return False

def file_exists_and_is_not_empty(fpath):
  if not (os.path.isfile(fpath) and (os.path.getsize(fpath) > 0)): return False
  if fpath.endswith('.root'):
    f = ROOT.TFile(fpath)
    if f.TestBit(ROOT.TFile.kRecovered) or f.IsZombie():
      return False
  return True

## ============================================================== #
## END FUNCTION DEFINITIONS
## ============================================================== #

# Check if it's running on lxplus
import socket
if "lxplus" not in socket.gethostname():
  print "YOU NEED TO RUN THIS SCRIPT ON LXPLUS"
  sys.exit(1)

# Check if it's running from the correct dir
base_path = os.getcwd()
code_dir = base_path + "/ProfileCode/"
if os.path.dirname(os.path.realpath(__file__)) == base_path:
  print "Working in:"
  print base_path
else:
  print "You need to execute this script from the 'analysis' directory"
  sys.exit(1)

if int(RecoilStatVariation) != 0 \
and int(RecoilVariationSigmas) == 0 :
  print "ERROR: Selected recoil correction var " + str(RecoilStatVariation) + " with 0 sigmas"
  print "Check the 'RecoilVariationSigmas' variable"
  sys.exit(1)

if (int(WlikeCharge) != 1) and (int(WlikeCharge) != -1) :
  print "ERROR: Asked for a Wlike of charge", WlikeCharge
  print "Check the 'WlikeCharge' variable"
  sys.exit(1)

if(int(MuonVariationSigmas)!=0 and int(MuonKalmanVariation)==0 and MuonScaleVariation==False):
  print "You asked to variate the muon by "+str(MuonVariationSigmas)+" sigmas but gave no syst to vary"
  sys.exit(1)

# Build  outfolder name

outfolder_name = outfolder_prefix

if (int(WlikeCharge) == 1):
  outfolder_name+="_muPos"
else:
  outfolder_name+="_muNeg"

outfolder_name += "_eta"+str(etaMaxMuons).replace('.','p')

systid =""

if(int(use_PForNoPUorTKmet)==0): # 0:PF, 1:NOPU, 2:TK
  systid+="_pfmet"
elif(int(use_PForNoPUorTKmet)==1): # 0:PF, 1:NOPU, 2:TK
  systid+="_pfnopu"
elif(int(use_PForNoPUorTKmet)==2): # 0:PF, 1:NOPU, 2:TK
  systid+=""  # "_tkmet" is implicit

if(int(use_LHE_weights)==1):
  systid+="_LHEweights"

if(int(IS_MC_CLOSURE_TEST)==1):
  systid+="_MCclosureTest"

if(int(syst_ewk_Alcaraz)>-1):
  systid+=""  # "_ewk"+str(syst_ewk_Alcaraz) is implicit
if(int(reweight_polarization)>0):
  systid+=""  # "_polariz"+str(reweight_polarization) is implicit

if  (int(useMomentumCorr)==1):
  systid+="_RochCorr"
elif(int(useMomentumCorr)==2):
  systid+="_MuscleFitCorr"
elif(int(useMomentumCorr)==3):
  systid+="_KalmanCorr"
elif(int(useMomentumCorr)==4):
  systid+=""  # "_KalmanCorrParam" is implicit

if(int(MuonVariationSigmas)!=0):
  syststring=["Down", "Up"][MuonVariationSigmas>0]
  if(int(MuonKalmanVariation) != 0):
    systid+="_KalmanVar"+str(MuonKalmanVariation)+syststring
  if(int(MuonScaleVariation) == True):
    systid+="_MuonScale"+str(abs(int(MuonVariationSigmas)))+syststring

# if(int(usePhiMETCorr)==1):
#   systid+="_phiMETcorr" is implicit

if(int(useRecoilCorr)>0):
  systid+=""  # "_RecoilCorr"+str(useRecoilCorr) is implicit
  if(int(correctToMadgraph)):
    systid+="_toMad"
  if(int(RecoilStatVariation)!=0):
    syststring=["Down", "Up"][RecoilVariationSigmas>0]
    systid+="_RecoilEigen"+str(RecoilStatVariation)+syststring

if(int(useEffSF)==1): systid+="_EffSFCorr"
if(int(useEffSF)>=2): systid+=""  # "_EffHeinerSFCorr" implicit
if(int(useEffSF)==3): systid+="_noTight"
if(int(useEffSF)==13): systid+="_Tight1perc"
if(int(useEffSF)==4): systid+="_noIso"
if(int(useEffSF)==14): systid+="_Iso1perc"
if(int(useEffSF)==5): systid+="_noTightSub"
if(int(useEffSF)==15): systid+="_TightSub1perc"
if(int(useEffSF)==6): systid+="_noHLT"
if(int(useEffSF)==16): systid+="_HLT1perc"

# Both implicit
# if(int(usePtSF)!=-1): outfolder_name+="_PtSFCorr"+str(usePtSF)
# if(int(usePileupSF)==1): outfolder_name+="_PileupSFCorr"

if (systid==""):
  systid = "_"

outfolder_name += systid

## END INITIAL CHECKS AND FOLDERNAME BUILDING
## ============================================================== #
## ============================================================== #
## ============================================================== #

DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadSig,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW = range(19)

sample      = [ "DATA" , "WJetsPowPlus", "WJetsPowNeg", "WJetsMadSig",    "WJetsMadFake", "DYJetsPow", "DYJetsMadSig", "DYJetsMadFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets",   "QCD", "T_s", "T_t", "T_tW", "Tbar_s", "Tbar_t", "Tbar_tW"]
isMCorDATA  = [      1 ,            0  ,          0   ,            0 ,                0 ,         0  ,            0  ,              0 ,         0,         0,         0,        0,       0,     0,     0,      0,        0,        0,         0]
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections or MCM
#                               POWHEG            POWHEG        MADGRAPH     MADGRAPH      POWHEG         POWHEG      twiki       twiki     twiki     twiki     twiki  twiki  twiki  twiki  twiki  twiki  twiki
CS_pb       = [     -1 ,         5913  ,        4126  ,        31314 ,            31314 ,       950  ,        2895.6 ,         2895.6 ,      165 ,       5.9,      43  ,    18.2 , 84679.3,  2.72,  42.6,    5.3,     1.49,       22,       5.3]
Nevts       = [141327580,     131310100,    109963100 ,      77571005,          77571005,    60235900,      34705858 ,       34705858 ,  54618956,  3899263 ,  3935865 ,  3860146,24939986,229786,3249552,744859,   139258,  1943163,    801626]  # 53X
int_lumi_fb = [     5.1 ,           0  ,            0 ,           0  ,                0 ,          0 ,              0,              0 ,       0  ,       0  ,        0 ,        0,       0,     0,      0,     0,        0,        0,         0]
# int_lumi_fb = [       (81293448/31314/1e3) ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ]
Nevts_Per_fb= [      0  ,            0 ,          0   ,            0 ,                0 ,          0 ,            0  ,             0  ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,         0]
generated_PDF_set =     [LHAPDF_reweighting_sets,229800,229800, 10042,            10042 ,      229800,         10042 ,          10042 ,    10042 ,     10042,     10042,    10042,   10042, 10042,  10042, 10042,    10042,    10042,     10042]
generated_PDF_member =  [LHAPDF_reweighting_members,0 ,       0 ,   0,                0 ,          0 ,             0 ,              0 ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,         0]
gen_mass_value_MeV  =   [  0  ,  80398 ,        80398 ,         80419,            91188 ,       91188,         91188 ,              0 ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,         0]
contains_LHE_weights =  [  0  ,      1 ,            1 ,             0,                0 ,           1,             0 ,              0 ,        0 ,         0,         0,        0,       0,     0,      0,     0,        0,        0,         0]
nsamples=len(sample)

if(int(use_LHE_weights)==0):
  for i in range(len(contains_LHE_weights)):
    contains_LHE_weights[i] = 0

### FROM THE POWHEG CONFIGURATION FILE ###
# ZMassCentral_MeV = "91.188"  # 91.1876
# WMassCentral_MeV = "80419"  # 80385

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
WMassNSteps = 1
WMassSkipNSteps = "1"
counter = 0

for i in xrange(-int(WMassNSteps),int(WMassNSteps)+1):
  WmassJoin_string = str(gen_mass_value_MeV[WJetsPowPlus]+int(round(float(dummy_deltaM_MeV[dummy_deltaM_MeV_central_Index+(int(WMassSkipNSteps))*i]))))
  if(counter>0): Wmass_values_array = ''.join((Wmass_values_array, ","))
  Wmass_values_array = ''.join((Wmass_values_array, WmassJoin_string))
  ZmassJoin_string = str(gen_mass_value_MeV[DYJetsPow]+int(round(float(dummy_deltaM_MeV[dummy_deltaM_MeV_central_Index+(int(WMassSkipNSteps))*i]))))
  if(counter>0): Zmass_values_array = ''.join((Zmass_values_array, ","))
  Zmass_values_array = ''.join((Zmass_values_array, ZmassJoin_string))
  counter = counter+1

fWana_str = [
  [ntuple_basepath+"DATA/WTreeProducer_tree_RecoSkimmed.root",-1],
  [ntuple_basepath+"WPlusPOWHEG/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"WMinusPOWHEG/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"WJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepath+"WJetsLL/WTreeProducer_tree_FakeRecoSkimmed.root",-1],
  [ntuple_basepath+"DYJetsMM/WTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepath+"DYJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepath+"DYJetsLL/WTreeProducer_tree_FakeRecoSkimmed.root",-1],
  [ntuple_basepath+"TTJets/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"VVJets/ZZ/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"VVJets/WW/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"VVJets/WZ/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"QCD/QCD20Mu15/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/T_s/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/T_t/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/T_tW/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/Tbar_s/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/Tbar_t/WTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/Tbar_tW/WTreeProducer_tree.root",-1]
];

fZana_str = [
#  ntuple_basepath_8TeV_ABC+"DATA_Run2012ABCD/ZTreeProducer_tree.root", # this is the 8TeV data contain also the tkmetABC
  [ntuple_basepathFIX+"DATA/ZTreeProducer_tree_RecoSkimmed.root",-1],
  [ntuple_basepath+"WJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepath+"WJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepath+"WJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepath+"WJetsLL/ZTreeProducer_tree_FakeRecoSkimmed.root",133],
  [ntuple_basepathFIX+"DYMM/ZTreeProducer_tree_SignalRecoSkimmed.root",20921509],
  # ntuple_basepath+"DYJetsMM/InclWeights/ZTreeProducer_tree.root",
  # ntuple_basepath+"DYJetsMM/allEvts/ZTreeProducer_tree.root",
#  ntuple_basepath_8TeV_ABC+"DYJetsLL/ZTreeProducer_tree_tkmetABC.root",  # this is the 8TeV DYJetsLL contains also the tkmetABC
  [ntuple_basepathFIX+"DYLL/ZTreeProducer_tree_SignalRecoSkimmed.root",-1],
  [ntuple_basepathFIX+"DYLL/ZTreeProducer_tree_FakeRecoSkimmed.root",8481],
  [ntuple_basepath+"TTJets/ZTreeProducer_tree.root",143880],
  [ntuple_basepath+"VVJets/ZZ/ZTreeProducer_tree.root",127139],
  [ntuple_basepath+"VVJets/WW/ZTreeProducer_tree.root",7990],
  [ntuple_basepath+"VVJets/WZ/ZTreeProducer_tree.root",63875],
  [ntuple_basepath+"QCD/QCD20Mu15/ZTreeProducer_tree.root",-1],
  [ntuple_basepath+"SingleTop/T_s/ZTreeProducer_tree.root",11],
  [ntuple_basepath+"SingleTop/T_t/ZTreeProducer_tree.root",280],
  [ntuple_basepath+"SingleTop/T_tW/ZTreeProducer_tree.root",1902],
  [ntuple_basepath+"SingleTop/Tbar_s/ZTreeProducer_tree.root",12],
  [ntuple_basepath+"SingleTop/Tbar_t/ZTreeProducer_tree.root",192],
  [ntuple_basepath+"SingleTop/Tbar_tW/ZTreeProducer_tree.root",2085]
];

if not os.path.exists("JobOutputs/"+outfolder_name):
  print "Out folder doesn't exists, making directory"
  print "JobOutputs/"+outfolder_name
  os.makedirs("JobOutputs/"+outfolder_name)

print "Copying script over:"
print "cp "+os.path.basename(__file__)+" JobOutputs/"+outfolder_name
print "cp configdir/"+conffile+".py JobOutputs/"+outfolder_name
print
path_dest="JobOutputs/"+outfolder_name+"/"+os.path.basename(__file__)
shutil.copyfile(os.path.basename(__file__), path_dest)
path_dest="JobOutputs/"+outfolder_name+"/"+conffile+".py"
shutil.copyfile(base_path+"/configdir/"+conffile+".py", path_dest)

with open("JobOutputs/"+outfolder_name+"/log.txt", "a") as logfile:
  commit_SHA = subprocess.Popen("git rev-parse HEAD", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.read()
  import datetime
  logstring = str(datetime.datetime.now()) + " - " + commit_SHA
  print "Writing on log file:", logstring
  logfile.write(logstring)
print ""

if(runZanalysis and not os.path.isfile("JobOutputs/"+outfolder_name+"/MERGED")):

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
    # build common.hpp from cfg
    print "Creating JobOutputs/"+outfolder_name+"/common.hpp from includes/common_template.hpp"
    shutil.copyfile("includes/common_template.hpp", "JobOutputs/"+outfolder_name+"/common.hpp")
    # Edit template
    os.system("sh "+base_path+"/utils/build_common.sh "+str(NMassValues)+" "+str(PAR_PDF_SETS)+" "+str(PAR_PDF_MEMBERS)+" "+str(etaMaxMuons)+" "+str(etaMaxMuons2)+" "+str(WlikeCharge)+" "+str(MuonKalmanVariation)+" "+str(MuonScaleVariation).lower()+" "+str(MuonVariationSigmas)+" "+str(RecoilStatVariation)+" "+str(RecoilVariationSigmas)+" "+str(usePtSF)+" "+str(dummy_deltaM_MeV_central_Index)+" "+"JobOutputs/"+outfolder_name+"/common.hpp"+" \""+(", ".join(map(str,Zmass_values)))+"\"")

  print ""
  os.chdir(base_path+"/JobOutputs/"+outfolder_name)

  ### Compiling run?analysis (if needed)

  if (recreateSubPrograms or not file_exists_and_is_not_empty("runZanalysis")):
    # Copy Zanalysis.C in the dest folder (it has some parameters)
    shutil.copyfile(code_dir+"/Zanalysis.C", "Zanalysis.C")
    cpp_command = "c++ -O2"
    if(useLHAPDF):
      print(cpp_command+" -o runZanalysis -DLHAPDF_ON `root-config --glibs --libs --cflags`  -I "+lhapdf_path+"/include -L "+lhapdf_path+"/lib -lLHAPDF -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
      os.system(cpp_command+" -o runZanalysis -DLHAPDF_ON `root-config --glibs --libs --cflags` -I "+lhapdf_path+"/include -L "+lhapdf_path+"/lib -lLHAPDF -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
    else:
      print(cpp_command+" -o runZanalysis `root-config --cflags --glibs` -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
      os.system(cpp_command+" -o runZanalysis `root-config --glibs --cflags` -L $ROOFITSYS/lib -lRooFit -lRooStats -lRooFit -lRooFitCore -lFoam -lMathMore -I$ROOFITSYS/include -lm -I . -I "+code_dir+"  Zanalysis.C "+code_dir+"common_stuff.C "+code_dir+"RecoilCorrector.cc "+code_dir+"KalmanCalibratorParam.cc "+code_dir+"PdfDiagonalizer.cc "+code_dir+"runZanalysis.C")
  
  for i in range(0, nsamples):

    if (resubmit_sample!=""):
      if not sample[i] in resubmit_sample:
        print "skipping",sample[i],"\twhich is not in {",resubmit_sample,"}"
        continue

    if(int(IS_MC_CLOSURE_TEST) and (sample[i]!="WJetsMadSig" and sample[i]!="DYJetsPow" and sample[i]!="DYJetsMadSig")):
      continue  # TEMPORARY

    sampleID= "output_"+sample[i]

    print ''
    print "ANALYZING sampleID =", sampleID

    WfileDATA= fWana_str[i][0]
    ZfileDATA= fZana_str[i][0]
    if( ("WJetsMadSig" in sample[i] or "WJetsPow" in sample[i]) and int(useAlsoGenPforSig)): WfileDATA.replace("_SignalRecoSkimmed","")
    else:
      if( ("DYJetsPow" in sample[i] or "DYJetsMadSig" in sample[i]) and int(useAlsoGenPforSig)): ZfileDATA.replace("_SignalRecoSkimmed","")

    if(CS_pb[i] >0): int_lumi_fb[i] = Nevts[i]/CS_pb[i]/1e3

    if int(indip_normalization_lumi_MC):
      WfileDATA_lumi_SF = intLumi_MC_fb/int_lumi_fb[i]
    elif int(normalize_MC_to_half_of_the_data)>0 and sample[i]!="DATA":
      WfileDATA_lumi_SF = (int_lumi_fb[DATA]/2)/int_lumi_fb[i]
    else:
      WfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i]

    ZfileDATA_lumi_SF = WfileDATA_lumi_SF

    # WfileDATA_lumi_SF = ((intLumi_MC_fb/int_lumi_fb[i]) if indip_normalization_lumi_MC==1 else 1) if IS_MC_CLOSURE_TEST==1 else int_lumi_fb[DATA]/int_lumi_fb[i]
    # ZfileDATA_lumi_SF = ((intLumi_MC_fb/int_lumi_fb[i]) if indip_normalization_lumi_MC==1 else 1) if IS_MC_CLOSURE_TEST==1 else int_lumi_fb[DATA]/int_lumi_fb[i]
    print "lumi_SF =",ZfileDATA_lumi_SF

    #########################################/
    ## END OF STEERING PARAMETERS
    #########################################/

    ## RUNNING CODE

    if(sampleID==""): sampleIDfolder="dummytest"
    else: sampleIDfolder=sampleID

    outputSamplePath = base_path+"/JobOutputs/"+outfolder_name+'/'+sampleIDfolder+'/'
    if not os.path.exists(outputSamplePath):
      os.makedirs(outputSamplePath)

    zstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+outputSamplePath+"\","+str(useMomentumCorr)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_LHE_weights[i])+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(correctToMadgraph)+","+str(use_PForNoPUorTKmet)+","+str(syst_ewk_Alcaraz)+","+str(gen_mass_value_MeV[i])+","+str(contains_LHE_weights[i])+","+str(reweight_polarization)
    
    if fWana_str[i][1] == -1:
      line = os.popen(base_path+"/JobOutputs/"+outfolder_name+"/runWanalysis.o -1,0,0,"+wstring).read()
      nEntries = [int(s) for s in line.split() if s.isdigit()][0]
    else:
      nEntries = fWana_str[i][1]

    if not useBatch:
      os.chdir(code_dir)
      os.system(base_path+"/JobOutputs/"+outfolder_name+"/runZanalysis 0,0,"+str(nEntries)+","+zstring)
    else:
      jobfirst = 1
      nevents = 1e5
      if (sample[i] == "DYJetsMadSig" or sample[i] == "DYJetsPow" or sample[i] == "TTJets"):
        nevents = 5e4

      os.chdir(outputSamplePath)

      nChuncks =  int(nEntries/nevents)+2
      text_file = open("Zanalysis_nChuncks.log", "w")
      text_file.write(str(nChuncks-1))
      text_file.close()

      print "nChuncks ",nChuncks-1
      for chunk in xrange(1, nChuncks):
        ev_ini= int(nevents*(chunk-1))
        ev_fin= int(nevents*(chunk)-1)
        if (chunk==nChuncks-1):
          ev_fin= nEntries
        print chunk,ev_ini,ev_fin
        if not file_exists_and_is_not_empty("Zanalysis_chunk"+str(chunk)+".root") and not batch_job_is_running("Zanalysis_"+outfolder_name+"_"+sample[i],str(chunk)):
          # Create scripts if needed
          if recreateSubPrograms>0 or not file_exists_and_is_not_empty("runZanalysis_"+str(chunk)+".sh"):
            text_file = open("runZanalysis_"+str(chunk)+".sh", "w")
            text_file.write("cd "+code_dir+"\n")
            text_file.write("eval `scramv1 runtime -sh`\n")
            text_file.write("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh \n")
            text_file.write(base_path+"/JobOutputs/"+outfolder_name+"/runZanalysis "+str(chunk)+","+str(ev_ini)+","+str(ev_fin)+","+zstring)
            text_file.close()
            os.system("chmod 755 runZanalysis_"+str(chunk)+".sh")
          # Send array if we reached maximum capacity (1000) or last chunk
          if chunk-jobfirst == 999 or chunk == nChuncks-1:
            joblist = str(jobfirst)+"-"+str(chunk)
            jobname = "Zanalysis_"+outfolder_name+"_"+sample[i]+"["+joblist+"]"
            LSFJobOutput = ''
            if noLSFJobOutput>0:
              LSFJobOutput = "-o "+outputSamplePath+"/batch_logs_Z.txt "
            print "Submitting job: "+jobname
            os.system("bsub -C 0 "+LSFJobOutput+"-u noEmail -q "+batchQueue+" -J "+jobname+" runZanalysis_\${LSB_JOBINDEX}.sh")
            jobfirst = chunk +1
        else:
          # Send array if there is a block waiting
          if chunk - jobfirst > 0:
            joblist = str(jobfirst)+"-"+str(chunk-1)
            jobname = "Zanalysis_"+outfolder_name+"_"+sample[i]+"["+joblist+"]"
            LSFJobOutput = ''
            if noLSFJobOutput>0:
              LSFJobOutput = "-o "+outputSamplePath+"/batch_logs_Z.txt "
            print "Submitting job: "+jobname
            os.system("bsub -C 0 "+LSFJobOutput+"-u noEmail -q "+batchQueue+" -J "+jobname+" runZanalysis_\${LSB_JOBINDEX}.sh")
          jobfirst = chunk +1

    os.chdir(base_path)

if(mergeSigEWKbkg and not os.path.isfile("JobOutputs/"+outfolder_name+"/MERGED")):
  rcode = os.system(code_dir+"/merge_MC.sh \"JobOutputs/"+outfolder_name+"/\" \""+mergeWhichAnalysis+"\"")
  if rcode != 0:
    print "Some merges failed, exiting"
    sys.exit(rcode)
  os.system("find JobOutputs/"+outfolder_name+"/output_* -type d -name LSFJOB_* -exec rm -rf {} +")
  os.system("find JobOutputs/"+outfolder_name+"/output_* -type f -name batch_logs_* -delete")

if  file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"/output_EWKTT/Zanalysis.root") \
and file_exists_and_is_not_empty("JobOutputs/"+outfolder_name+"/output_DYJetsPow/Zanalysis.root") :
  open("JobOutputs/"+outfolder_name+"/MERGED", "a").close()
else:
  print "Cannot find merged histogram in "+outfolder_name+", exiting"
  sys.exit(1)

if(removeChunks):
  print "Removing chunks from JobOutputs/"+outfolder_name
  os.system("find JobOutputs/"+outfolder_name+"/output_* -type f -name [WZ]analysis_chunk*.root -delete")

# Legacy
RecoilCorrVarDiagoParU1orU2fromDATAorMC = 0

if(runPrepareDataCardsFast):
  # common.h  is already in place, as is the one for the systematic
  # common2.h is got from the template folder, specified by the variable (same folder if empty)
  if (DataCards_templateFromFolder != ""):
    shutil.copyfile("JobOutputs/"+DataCards_templateFromFolder+"/common.h", "JobOutputs/"+outfolder_name+"/common2.h")
  else:
    shutil.copyfile("JobOutputs/"+outfolder_name+"/common.h", "JobOutputs/"+outfolder_name+"/common2.h")
  os.system("sed -i 's/.*namespace WMass{.*/namespace WMass2{/' JobOutputs/"+outfolder_name+"/common2.h")

  if not os.path.exists("JobOutputs/"+outfolder_name+"/DataCards"): os.makedirs("JobOutputs/"+outfolder_name+"/DataCards")
  shutil.copyfile(base_path+"/AnalysisCode/prepareDatacardsFast.C", base_path+"/JobOutputs/"+outfolder_name+"/prepareDatacardsFast.C")
  os.chdir("JobOutputs/"+outfolder_name)
  print "running .x prepareDatacardsFast.C++(\"./\",\"../"+DataCards_templateFromFolder+"\",\""+usePowOrMadForSig+"\",1,1,\""+str(fit_W_or_Z)+"\","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'"
  os.system("root -l -b -q \'prepareDatacardsFast.C++(\"./\",\"../"+DataCards_templateFromFolder+"\",\""+usePowOrMadForSig+"\",1,1,\""+str(fit_W_or_Z)+"\","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'")
  os.chdir(base_path)

if(runClosureTestLikeLihoodRatio):
  shutil.copyfile("AnalysisCode/ClosureTest_fits_likelihoodratio.C","JobOutputs/"+outfolder_name+"/DataCards/ClosureTest_fits_likelihoodratio.C")
  os.chdir("JobOutputs/"+outfolder_name+"/DataCards")
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
  os.chdir(base_path)

if(mergeResults or (runClosureTestLikeLihoodRatio and useBatch==0)):
  os.system("cp AnalysisCode/merge_results.C JobOutputs/"+outfolder_name+"/DataCards/merge_results.C")
  os.chdir("JobOutputs/"+outfolder_name+"/DataCards")
  print os.getcwd()
  os.system("rm -rf LSF*; rm -f output_W*.root")
  os.system("root -l -b -q \'merge_results.C++(1,0,\""+str(fit_W_or_Z)+"\","+str(useBatch)+","+str(RecoilCorrVarDiagoParU1orU2fromDATAorMC)+")\'")
  os.chdir(base_path)
