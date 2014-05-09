#!/usr/local/bin/python

# change above line to point to local 
# python executable

import urllib, urlparse, string, time, os, shutil, sys

## ==============================================================
## STEERING PARAMETERS
## ==============================================================

useLHAPDF = False # DEFAULT IS False, before to switch to True follow https://twiki.cern.ch/twiki/bin/view/CMS/CmgWMass#LHAPDF
  
# foldername = "test_RecoilCorr_fLuc1";
# foldername = "test_RecoilCorr_scale1";
# foldername = "test_RecoilCorrNewFiles";
# foldername = "test_NoPUmet_recoil15";
foldername = "test_pfnopu_std";
# foldername = "test_PhiStarEta";

ntuple_folder = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/";
# ntuple_folder = "root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
# ntuple_folder = "root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/";
# ntuple_folder = "~/eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
lhapdf_folder="/afs/cern.ch/work/p/perrozzi/private/WMassMC/lhapdf/"

usePileupSF = 1; # 0=no, 1=yes
useEffSF = 2; # 0=no, 1=MuonPOG, 2=Heiner
usePtSF = 0; # 0=no, 1=yes
useMomentumCorr = 1; # 0=none, 1=Rochester, 2=MuscleFit
# RochCorrByNsigma = 1;
LocalSmearingRochCorrNToys = 0;
GlobalSmearingRochCorrNsigma = 0;
usePhiMETCorr = 0; # 0=none, 1=yes
useRecoilCorr = 1; # 0=none, 1=yes
RecoilCorrResolutionNSigma = "0"; # 0=none, 1=yes
RecoilCorrScaleNSigma = "0"; # 0=none, 1=yes
# LHAPDF_reweighting_sets="11200" # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
# LHAPDF_reweighting_members="51" # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
LHAPDF_reweighting_sets="10042" # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
LHAPDF_reweighting_members="1" # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41
## CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
IS_MC_CLOSURE_TEST= 0; 

indip_normalization_lumi_MC = 0; # independent normalization of MC in fb-1 (otherwise normalized to DATA)
intLumi_MC_fb = 81293448/31314/1e3;# data = 4.7499 fb-1 prescaled trigger, 5.1 fb-1 unprescaled; works only if indip_normalization_lumi_MC is TRUE
useAlsoGenPforSig= 1;

ZMass = "91.188"; # 91.1876
WMassCentral_MeV = "80419"; # 80385
WMassStep_MeV = "10"; # 15
WMassNSteps = "5"; # 60
# WMassNSteps = "0"; # 60
# WMassNSteps = "0"; # 60
etaMuonNSteps = "1"; # 5
etaMaxMuons = "1.6"; # 0.6, 0.8, 1.2, 1.6, 2.1
# etaMaxMuons = "2.1"; # 0.6, 0.8, 1.2, 1.6, 2.1

parallelize = 1;
resumbit_sample = "" # DATA , WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD

runPhiStarEta = 0;
runWanalysis = 0;
runZanalysis = 1;
controlplots = 0;

mergeSigEWKbkg = 0;

## PRODUCE R(X)=W/Z DISTRIBUTION TO REWEIGHT Z in DATA
runR_WdivZ= 0;
## PRODUCE TEMPLATES, i.e. Z(DATA)*R(X)
run_BuildSimpleTemplates= 0;

## PERFORM W MASS FIT
runPrepareDataCards = 0;
DataCards_systFromFolder="" # evaluate systematics wrt folder (or leave it empty)

## NEW FIT
print "if it doesn't work, try with this first: cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_5_3_3_patch3/src; SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -";
runClosureTestLikeLihoodRatioAnsMergeResults = 0;
mergeResults = 0;

## OLD FIT
ExtractNumbers = 0; # NOT REALLY USED
run_BuildEvByEvTemplates= 0; # NOT REALLY USED
runDataCardsParametrization = 0; # NOT REALLY USED
runClosureTest = 0;
run_MassFit         = 0;
fitType             = 0; # 0 = ROOT, 1 = CUSTOM

#######################
### PLOTTING ###
#######################
runWSigBkgFit = 0;
runZSigBkgFit = 0;

run_Z_MCandDATAcomparisons_stack = 0;
run_W_MCandDATAcomparisons_stack = 0;
run_PhiStarEta_MCandDATAcomparisons_stack = 0;
run_Z_MCandDATAcomparison = 0;
run_W_MCandDATAcomparison = 0;

run_ZvsWlike_comparisonMC   = 0;
run_ZvsWlike_comparisonDATA = 0;

runWandZcomparisonMC   = 0;
runWandZcomparisonDATA = 0;


## END STEERING PARAMETERS
## ============================================================== #
## ============================================================== #
## ============================================================== #

print "cd /afs/cern.ch/work/p/perrozzi/private/CMSSW_6_1_1/src; SCRAM_ARCH=slc5_amd64_gcc462;eval `scramv1 runtime -sh`; cd -;"

if(IS_MC_CLOSURE_TEST==1):
    foldername+="_MCclosureTest";
    useEffSF=0;
    usePtSF=0;
    usePileupSF=0;

if(LocalSmearingRochCorrNToys<1):
  LocalSmearingRochCorrNToys=1
if(useMomentumCorr==1): 
  foldername+="_RochCorr";
  if(GlobalSmearingRochCorrNsigma>0): 
    foldername+=str(GlobalSmearingRochCorrNsigma)+"s_smear";
else: 
    if(useMomentumCorr==2): foldername+="_MuscleFitCorr";
if(usePhiMETCorr==1): 
  foldername+="_phiMETcorr";
if(useRecoilCorr==1): 
  foldername+="_RecoilCorr";
  if("0" not in RecoilCorrResolutionNSigma):
    foldername+="_Resol"+str(RecoilCorrResolutionNSigma);
  if("0" not in RecoilCorrScaleNSigma):
    foldername+="_Scale"+str(RecoilCorrScaleNSigma);

if(useEffSF==1): foldername+="_EffSFCorr";
if(useEffSF==2): foldername+="_EffHeinerSFCorr";
if(usePtSF==1): foldername+="_PtSFCorr";
if(usePileupSF==1): foldername+="_PileupSFCorr";
nsamples=10;
DATA , WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD = range(10)
sample      = [ "DATA" , "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"];
isMCorDATA  = [      1 ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,   0      ];
CS_pb       = [     -1 ,    31314  ,      31314 ,   2895.6   ,      2895.6 ,   157.51 ,   0.119  ,   4.514  ,   0.596 , 84679.3  ];
                                                                                                                                            # # Run2011A  Run2011B  JSON  TRIGGER GOOD VTX
# Nevts[nsamples]  = { 81053834  ,   81053834 ,  36209629  ,    36209629 , 59136395 , 1103468  , 1197558  , 1221134 , 24958039 , (50367238+91137428)*0.8894*0.3991*0.9996   }; # 42X
Nevts       = [  53056567 , 81293448  ,   81293448 ,  36225556  ,    36225556 , 59314581 , 1103468  , 1197558  , 1221134 , 24958039   ]; # 44X
int_lumi_fb = [       5.1 ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ];
# int_lumi_fb = [       (81293448/31314/1e3) ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ];
Nevts_Per_fb= [        0  ,         0 ,        0   ,          0 ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ];
generated_PDF_set =     [       LHAPDF_reweighting_sets  ,    10042 ,    10042 ,     10042 ,   10042 ,     10042 ,     10042 ,     10042 ,     10042 ,     10042 ];
generated_PDF_member =  [       LHAPDF_reweighting_members  ,        0 ,        0 ,        0 ,        0 ,         0 ,         0 ,         0 ,         0 ,         0 ];
contains_PDF_reweight = [       0  ,        0 ,        0 ,        0 ,        0 ,         0 ,         0 ,         0 ,         0 ,         0 ];


fWana_str = [
  ntuple_folder+"DATA/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"WJets/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"WJets/WTreeProducer_tree_FakeRecoSkimmed.root",
  # ntuple_folder+"DYJets/WTreeProducer_tree_SignalRecoSkimmed.root",
  # ntuple_folder+"DYJets/WTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"DYJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"DYJetsLL/WTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"TTJets/WTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"VVJets/ZZ/WTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"VVJets/WW/WTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"VVJets/WZ/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/ZZJetsTo2L2Nu/WTreeProducer_tree.root",
  ntuple_folder+"VVJets/WWJetsTo2L2Nu/WTreeProducer_tree.root",
  ntuple_folder+"VVJets/WZJetsTo3LNu/WTreeProducer_tree.root",
  ntuple_folder+"QCD/WTreeProducer_tree.root"
];  
fZana_str = [
  ntuple_folder+"DATA/ZTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"DATA_RelVal53X/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"WJets/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"WJets/ZTreeProducer_tree_FakeRecoSkimmed.root",
  # ntuple_folder+"DYJets/ZTreeProducer_tree_SignalRecoSkimmed.root",
  # ntuple_folder+"DYJets/ZTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"DYJetsLL/ZTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"TTJets/ZTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"VVJets/ZZ/ZTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"VVJets/WW/ZTreeProducer_tree_RecoSkimmed.root",
  # ntuple_folder+"VVJets/WZ/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/ZZJetsTo2L2Nu/ZTreeProducer_tree.root",
  ntuple_folder+"VVJets/WWJetsTo2L2Nu/ZTreeProducer_tree.root",
  ntuple_folder+"VVJets/WZJetsTo3LNu/ZTreeProducer_tree.root",
  ntuple_folder+"QCD/ZTreeProducer_tree.root"
];

print os.getcwd()
print ".! cp launch_analysis.py JobOutputs/",foldername;
file_dest="JobOutputs/"+foldername+"/launch_analysis.py";
if not os.path.exists("JobOutputs/"+foldername):
    os.makedirs("JobOutputs/"+foldername)
shutil.copyfile("launch_analysis.py", file_dest)


if(runWanalysis or runZanalysis or run_BuildEvByEvTemplates or runPhiStarEta):
    
    if(useLHAPDF and os.environ.get('LHAPATH') == lhapdf_folder+"share/lhapdf/PDFsets"):
        print "ENVIRONMENT VARIABLES OK"
    else:
        print "REMEMBER TO SET ENVIRONMENT VARIABLES (MUST RUN ON LXPLUS):"
        print "/afs/cern.ch/project/eos/installation/0.2.30/bin/eos.select -b fuse mount ~/eos"
        print "source /afs/cern.ch/sw/lcg/external/gcc/4.3.2/x86_64-slc5/setup.sh"
        print "source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh"
        if(useLHAPDF):
          print ("export PATH="+lhapdf_folder+"bin/:$PATH")
          print ("export LD_LIBRARY_PATH="+lhapdf_folder+"lib/:$LD_LIBRARY_PATH")
          print ("export LHAPATH="+lhapdf_folder+"share/lhapdf/PDFsets")
          sys.exit()
  
    counter=0
    for i in range(0, nsamples):
        
        if (resumbit_sample!=""):
            # parallelize = 0
            if not sample[i] in resumbit_sample: 
                print "skipping ",sample[i]," which is not",resumbit_sample;
                continue;
          
        counter=counter+1
        # print "counter ",counter
        
        if(IS_MC_CLOSURE_TEST and (sample[i]!="WJetsSig" and sample[i]!="DYJetsSig")): continue; # TEMPORARY
        jobID= "test_numbers_"+sample[i];
        
        print ''    
        print "ANALYZING jobID= ", jobID ;
        
        WfileDATA= fWana_str[i];
        ZfileDATA= fZana_str[i];
        if( ("WJetsSig" in sample[i]) and useAlsoGenPforSig): WfileDATA.replace("_SignalRecoSkimmed","");
        else:
            if( ("DYJetsSig" in sample[i]) and useAlsoGenPforSig): ZfileDATA.replace("_SignalRecoSkimmed","");

        tot_N_evts = Nevts[i];

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
        print "lumi_SF= ",ZfileDATA_lumi_SF

        #########################################/
        ## END OF STEERING PARAMETERS
        #########################################/
        
        ## RUNNING CODE

        if(jobID==""): outputdir="dummytest";
        else: outputdir=jobID;
        filename_outputdir = "JobOutputs/"+foldername+'/'+outputdir+'/';
        
        # print working directory
        print os.getcwd()

        print "checking directory "+filename_outputdir
        ## check if the outputfolder is already existing
        # make the outputfolder even if is already existing (nothing happens)
        dir_check = 0
        if not os.path.exists(filename_outputdir):
            print "doesn't exists, making directory ",filename_outputdir
            os.makedirs(filename_outputdir)
            dir_check = 1
        
        
        # make the outputfolder trash were backup of the folder will be stored (overwrite protection!)
        # gSystem->MakeDirectory(Form("JobOutputs/%s/%s/trash",foldername.Data(),outputdir.Data()));
        # gROOT->ProcessLine(Form(".! cp JobOutputs/%s/%s/\* JobOutputs/%s/%s/trash/",foldername.Data(),outputdir.Data(),foldername.Data(),outputdir.Data()));        

        NPDF_sets = LHAPDF_reweighting_sets.count(',')+1
        if(LHAPDF_reweighting_sets!=""):
          PAR_PDF_SETS = LHAPDF_reweighting_sets
          PAR_PDF_MEMBERS = LHAPDF_reweighting_members
        else:
          PAR_PDF_SETS = "-1"
          PAR_PDF_MEMBERS = "1"
          
        if( dir_check ==0 ): # if the output folder existed use the same common.h
        # if( False ): # if the output folder existed use the same common.h
            print "using JobOutputs/"+foldername+"/"+outputdir+"/common.h"
            shutil.copyfile(("JobOutputs/"+foldername+"/"+outputdir+"/common.h"), "includes/common.h")
        else: # otherwise build it from this cfg
            print "creating JobOutputs/"+foldername+"/"+outputdir+"/common.h from includes/common.h.bkp"
            shutil.copyfile("includes/common.h.bkp", "includes/common.h");
            os.system("sh "+os.getcwd()+"/manipulate_parameters.sh "+ZMass+" "+WMassCentral_MeV+" "+WMassStep_MeV+" "+WMassNSteps+" "+etaMuonNSteps+" \""+etaMaxMuons+"\" "+str(NPDF_sets)+" "+str(PAR_PDF_SETS)+" "+str(PAR_PDF_MEMBERS)+" "+str(LocalSmearingRochCorrNToys))
            shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/"+outputdir+"/common.h");
        
        os.chdir("AnalysisCode/");
        # print os.system("rm *.d")
        # print os.system("rm *.so")
        print os.getcwd()


        if parallelize and (runWanalysis or runZanalysis or runPhiStarEta) and counter>1:
            print "waiting 10 sec to 20 sec (if W and Z are launched) before to proceed with the next sample to let the compilation finish"
            os.system("sleep 3");
        
        # if(i==0): os.system("touch *.*");
        
        if(runWanalysis):
            
            wstring="\""+WfileDATA+"\","+str(WfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(GlobalSmearingRochCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_PDF_reweight[i])+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(RecoilCorrResolutionNSigma)+","+str(RecoilCorrScaleNSigma)
            if(counter<2):
                if(useLHAPDF):
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\#define\ LHAPDF_ON/' Wanalysis.C")
                    print("c++ -o runWanalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Wanalysis.C rochcor_44X_v3.C common_stuff.C RecoilCorrector.cc runWanalysis.C")
                    os.system("rm runWanalysis.o; c++ -o runWanalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Wanalysis.C rochcor_44X_v3.C common_stuff.C RecoilCorrector.cc runWanalysis.C")
                else:
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\/\/\#define\ LHAPDF_ON/' Wanalysis.C")
                    print("c++ -o runWanalysis.o `root-config --glibs --libs --cflags` -lm Wanalysis.C rochcor_44X_v3.C common_stuff.C RecoilCorrector.cc runWanalysis.C")
                    os.system("rm runWanalysis.o; c++ -o runWanalysis.o `root-config --glibs --libs --cflags`  -lm Wanalysis.C common_stuff.C rochcor_44X_v3.C RecoilCorrector.cc runWanalysis.C")
            
            # print("root -l -b -q \'runWanalysis2.cpp("+wstring+")\'");
            
            # os.system("root -l -b -q \'runWanalysis2.cpp("+wstring+")\'");
            print " "
            line = os.popen("./runWanalysis.o -1,0,0,"+wstring).read()
            nEntries = [int(s) for s in line.split() if s.isdigit()][0]
            # print wstring
            if not parallelize:
                os.system("./runWanalysis.o 0,0,"+str(nEntries)+","+wstring)
                # os.system("root -l -b -q \'runWanalysis.C("+wstring+")\'");
            else:
                nevents = 2e5
                if ("DATA" in sample[i]):
                  nevents = 1e6  
                if (WMassNSteps=="0"):
                  nevents = 100e6  
                  
                nChuncks =  int(nEntries/nevents)+2
                if(nChuncks>2): print "nChuncks ",nChuncks-1
                if nChuncks>2  and (("WJetsSig" in sample[i]) or ("DATA" in sample[i])):
                  for x in xrange(1, nChuncks):
                    ev_ini=int(nevents*(x-1))
                    ev_fin= nEntries 
                    if (x<nChuncks-1):
                      ev_fin= int(nevents*(x)-1)
                    # ev_fin= int(nevents*(x)-1) if (x<nChuncks-1) else nEntries
                    print x,ev_ini,ev_fin
                    os.system("./runWanalysis.o "+str(x)+","+str(ev_ini)+","+str(ev_fin)+","+wstring+" > ../"+filename_outputdir+"Wlog_"+str(x)+".log 2>&1 &")
                    os.system("sleep 3");
                else:
                    os.system("./runWanalysis.o 0,0,"+str(nEntries)+","+wstring+" > ../"+filename_outputdir+"Wlog.log 2>&1 &")
                # print "root -l -b -q \'runWanalysis.C("+wstring+")\' ../> "+filename_outputdir+"Wlog.log 2>&1 &";
                # os.system("root -l -b -q \'runWanalysis.C("+wstring+")\' > ../"+filename_outputdir+"Wlog.log 2>&1 &");
                os.system("sleep 3");

        if(runZanalysis):

            zstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(GlobalSmearingRochCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(0)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_PDF_reweight[i])+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(RecoilCorrResolutionNSigma)+","+str(RecoilCorrScaleNSigma)
            
            if(counter<2):
            # os.system("touch *.*");
                if(useLHAPDF):
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\#define\ LHAPDF_ON/' Zanalysis.C")
                    print("c++ -o runZanalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Zanalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc runZanalysis.C")
                    os.system("rm runZanalysis.o; c++ -o runZanalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Zanalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc runZanalysis.C")                    
                else:
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\/\/\#define\ LHAPDF_ON/' Zanalysis.C")
                    print("c++ -o runZanalysis.o `root-config --glibs --libs --cflags`  -lm Zanalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc runZanalysis.C")
                    os.system("rm runZanalysis.o; c++ -o runZanalysis.o `root-config --glibs --libs --cflags`  -lm Zanalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc runZanalysis.C")

            print " "
            line = os.popen("./runZanalysis.o -1,0,0,"+zstring).read()
            nEntries = [int(s) for s in line.split() if s.isdigit()][0]
            # print zstring
            if not parallelize:
                # os.system("./runZanalysis.o 0,0,"+str(nEntries)+","+zstring)
                os.system("./runZanalysis.o "+zstring)
                # os.system("root -l -b -q \'runZanalysis.C("+zstring+")\'");
            else:
                nevents = 2e5
                if ("DATA" in sample[i]):
                  nevents = 1e6  
                if (WMassNSteps=="0"):
                  nevents = 100e6  
                  
                nChuncks =  int(nEntries/nevents)+2
                if(nChuncks>2): print "nChuncks ",nChuncks-1
                if nChuncks>2  and (("DYJetsSig" in sample[i]) or ("DATA" in sample[i])):
                  for x in xrange(1, nChuncks):
                    ev_ini=int(nevents*(x-1))
                    ev_fin= nEntries 
                    if (x<nChuncks-1):
                      ev_fin= int(nevents*(x)-1)
                    # ev_fin= int(nevents*(x)-1) if (x<nChuncks-1) else nEntries
                    print x,ev_ini,ev_fin
                    os.system("./runZanalysis.o "+str(x)+","+str(ev_ini)+","+str(ev_fin)+","+zstring+" > ../"+filename_outputdir+"Zlog_"+str(x)+".log 2>&1 &")
                    os.system("sleep 3");
                else:
                    os.system("./runZanalysis.o 0,0,"+str(nEntries)+","+zstring+" > ../"+filename_outputdir+"Zlog.log 2>&1 &")
                # print "root -l -b -q \'runWanalysis.C("+zstring+")\' ../> "+filename_outputdir+"Wlog.log 2>&1 &";
                # os.system("root -l -b -q \'runWanalysis.C("+zstring+")\' > ../"+filename_outputdir+"Wlog.log 2>&1 &");
                os.system("sleep 3");
                    
            # print zstring
            # if not parallelize:
                # # os.system("root -l -b -q \'runZanalysis.C("+zstring+")\'");
                # os.system("./runZanalysis.o "+zstring)

            # else:
                # if(runWanalysis): os.system("sleep 3");
                # os.system("./runZanalysis.o "+zstring+" > ../"+filename_outputdir+"Zlog.log 2>&1 &")
                # # print "root -l -b -q \'runZanalysis.C("+zstring+")\' > ../"+filename_outputdir+"Zlog.log 2>&1 &";
                # # os.system("root -l -b -q \'runZanalysis.C("+zstring+")\' > ../"+filename_outputdir+"/Zlog.log 2>&1 &");

                
                
                
                
        if(runPhiStarEta):

            zstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(GlobalSmearingRochCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(0)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_PDF_reweight[i])+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(RecoilCorrResolutionNSigma)+","+str(RecoilCorrScaleNSigma)
            
            if(counter<2):
            # os.system("touch *.*");
                if(useLHAPDF):
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\#define\ LHAPDF_ON/' runPhiStarEtaAnalysis.C")
                    print("c++ -o PhiStarEtaAnalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm runPhiStarEtaAnalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc HTransformToHelicityFrame.c PhiStarEtaAnalysis.C")
                    os.system("rm PhiStarEtaAnalysis.o; c++ -o PhiStarEtaAnalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm runPhiStarEtaAnalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc HTransformToHelicityFrame.c PhiStarEtaAnalysis.C")                    
                else:
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\/\/\#define\ LHAPDF_ON/' runPhiStarEtaAnalysis.C")
                    print("c++ -o PhiStarEtaAnalysis.o `root-config --glibs --libs --cflags`  -lm runPhiStarEtaAnalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc HTransformToHelicityFrame.c PhiStarEtaAnalysis.C")
                    os.system("rm PhiStarEtaAnalysis.o; c++ -o PhiStarEtaAnalysis.o `root-config --glibs --libs --cflags`  -lm runPhiStarEtaAnalysis.C rochcor_44X_v3.C common_stuff.C ../includes/common.h RecoilCorrector.cc HTransformToHelicityFrame.c PhiStarEtaAnalysis.C")

            print zstring
            if not parallelize:
                # os.system("root -l -b -q \'PhiStarEtaAnalysis.C("+zstring+")\'");
                os.system("./PhiStarEtaAnalysis.o "+zstring)

            else:
                if(runWanalysis): os.system("sleep 3");
                os.system("./PhiStarEtaAnalysis.o "+zstring+" > ../"+filename_outputdir+"Zlog.log 2>&1 &")
                # print "root -l -b -q \'PhiStarEtaAnalysis.C("+zstring+")\' > ../"+filename_outputdir+"Zlog.log 2>&1 &";
                # os.system("root -l -b -q \'PhiStarEtaAnalysis.C("+zstring+")\' > ../"+filename_outputdir+"/Zlog.log 2>&1 &");

        if(run_BuildEvByEvTemplates):
            zTemplstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(GlobalSmearingRochCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(run_BuildEvByEvTemplates)+","+str(usePhiMETCorr)+","+str(useRecoilCorr)+","+str(RecoilCorrResolutionNSigma)+","+str(RecoilCorrScaleNSigma)
            # print zTemplstring
            # exit()
            if not parallelize:
                os.system("root -l -b -q \'runZanalysis.C("+zTemplstring+")\'");
            else:
                if(runWanalysis): os.system("sleep 3");
                # print "root -l -b -q \'runZanalysis.C("+zTemplstring+")\' > ../"+filename_outputdir+"Zlog.log 2>&1 &";
                os.system("root -l -b -q \'runZanalysis.C("+zTemplstring+")\' > ../"+filename_outputdir+"/ZEvByEvTemplog.log 2>&1 &");
                # gROOT->ProcessLine(".! touch *.*");
                # gSystem->CompileMacro("rochcor_44X_v3.C");
                # # gSystem->CompileMacro("rochcor_42X.C");
                # # gSystem->CompileMacro("MuScleFitCorrector.cc");
                # gSystem->CompileMacro("Zanalysis.C");
                # # cout << "int_lumi_fb[IS_MC_CLOSURE_TEST? DYJetsSig : DATA]= "<<(IS_MC_CLOSURE_TEST? intLumi_MC_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1) << endl;
                # cout << Form("Zanalysis zTEMPLATESOnDATA(\"%s\",%.f)",fZana_str[IS_MC_CLOSURE_TEST? DYJetsSig : DATA].Data(),IS_MC_CLOSURE_TEST? intLumi_MC_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1) << endl;
                # gROOT->ProcessLine(Form("Zanalysis zTEMPLATESOnDATA(\"%s\",%.f)",fZana_str[IS_MC_CLOSURE_TEST? DYJetsSig : DATA].Data(),IS_MC_CLOSURE_TEST? intLumi_MC_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1));
                # # gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d,1,%s,1)",IS_MC_CLOSURE_TEST,filename_outputdir.Data()));
                # cout << "processing line " << Form("zTEMPLATESOnDATA.Loop(%d,1,\"../JobOutputs/%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,foldername.Data(),useMomentumCorr,GlobalSmearingRochCorrNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
                # gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d,1,\"../JobOutputs/%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,foldername.Data(),useMomentumCorr,GlobalSmearingRochCorrNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
                                                        # # # ".Loop(%d,%d,\"../%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,GlobalSmearingRochCorrNsigma,useEffSF,usePileupSF,sample[i].Data()));
                # # # gROOT->ProcessLine(Form(".! mv Zanalysis.root ../JobOutputs/%s/ZanalysisOnDATA.root",outputdir.Data()));
                # gROOT->ProcessLine(Form(".! mv ../JobOutputs/%s/Zanalysis.root ../JobOutputs/%s/Templates_from_ZanalysisOnDATA.root",foldername.Data(),foldername.Data()));
                # gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../JobOutputs/%s/",foldername.Data()));
                
        os.chdir("../");
        
if (ExtractNumbers or run_BuildSimpleTemplates):
    for dir in os.listdir("JobOutputs/"+foldername):
        # print "JobOutputs/"+foldername+"/"+dir
        if os.path.isdir("JobOutputs/"+foldername+"/"+dir) and ("test_numbers_" in dir) :
            dataset_item = dir.replace("test_numbers_","");
            # print dir,' is a set'

            # # for filename_outputdir in "JobOutputs/"+foldername+'/'+outputdir+'/':
            if(ExtractNumbers):
                os.chdir("AnalysisCode/");
                # # os.system("touch *.*");
                # os.system("root -l -b -q \'R_WdivZ.C(\"../JobOutputs/"+foldername+"/"+dir+"\",1,"+str(tot_N_evts)+")\' > ../JobOutputs/"+foldername+"/"+outputdir+"/numbers.txt");
                isample = -1
                if (dataset_item in sample): isample = sample.index(dataset_item)
                if isample > -1: tot_N_evts = Nevts[isample];
                else: tot_N_evts =1
                
                print "root -l -b -q \'R_WdivZ.C(\"../JobOutputs/"+foldername+"/"+dir+"\",1,"+str(tot_N_evts)+")\' > ../JobOutputs/"+foldername+"/"+dir+"/numbers.txt";
                os.system("root -l -b -q \'R_WdivZ.C(\"../JobOutputs/"+foldername+"/"+dir+"\",1,"+str(tot_N_evts)+")\' > ../JobOutputs/"+foldername+"/"+dir+"/numbers.txt");
                shutil.copyfile("R_WdivZ.C","../JobOutputs/"+foldername+"/"+dir+"/R_WdivZ.C");
                os.chdir("../");
                
            if(run_BuildSimpleTemplates):
                os.chdir("AnalysisCode/");
                os.system("root -l -b -q \' BuildSimpleTemplates.C( \"../JobOutputs/"+foldername+"/"+dir+"\" ) \' ");
                # cout << "running " << Form(".! root -l -b -q \'BuildSimpleTemplates.C(\"../JobOutputs/%s/%s\")\'",foldername.Data(),outputdir.Data()) << endl;
                # gROOT->ProcessLine(Form(".! root -l -b -q \'BuildSimpleTemplates.C(\"../JobOutputs/%s/%s\")\'",foldername.Data(),outputdir.Data()));
                os.chdir("../");
            
            
# gSystem->ChangeDirectory("AnalysisCode/");

if(mergeSigEWKbkg):
# gROOT->ProcessLine(".! touch *.*");
# gSystem->ChangeDirectory("../utils/");
# # cout << Form("../JobOutputs/%s/test_numbers_EWK/",foldername.Data()) << endl;
# gSystem->MakeDirectory(Form("../JobOutputs/%s/test_numbers_EWK/",foldername.Data()));
# cout << gSystem->WorkingDirectory() << endl;
    os.chdir("utils/");
    os.system("sh merge_MC.sh \"../JobOutputs/"+foldername+"/\" ");
    os.chdir("../");


if(runR_WdivZ):
    os.chdir("AnalysisCode/");
    # gROOT->ProcessLine(".! touch *.*");
    os.system("eval `scramv1 runtime -sh`")
    os.system("root -l -b -q \'R_WdivZ.C(\"../JobOutputs/"+foldername+"\",0,1,"+str(LHAPDF_reweighting_sets)+")\'");
    os.system("mv *.root ../JobOutputs/"+foldername+"/");
    os.system("cp R_WdivZ.C ../JobOutputs/"+foldername+"/");
    os.chdir("../");

if(runPrepareDataCards):
    os.chdir("AnalysisCode/");
    # gROOT->ProcessLine(".! touch *.*");
    # cout << gSystem->WorkingDirectory() << endl;
    # gSystem->ChangeDirectory("../AnalysisCode/");
    # gSystem->MakeDirectory(Form("../JobOutputs/%s/DataCards",foldername.Data()));
    if not os.path.exists("../JobOutputs/"+foldername+"/DataCards"): os.makedirs("../JobOutputs/"+foldername+"/DataCards")
    # TString samples_datacards;
    # for(int i=0;i<nsamples;i++){
      # samples_datacards+=sample[i];
      # if(i<nsamples-1) samples_datacards+="-";
    # }
    print "running .x prepareDatacards.C(\"../JobOutputs/"+foldername+"\",\"\")"
    os.system("root -l -b -q \'prepareDatacards.C+(\"../JobOutputs/"+foldername+"\",\"../JobOutputs/"+DataCards_systFromFolder+"\",\"\")\'")
    # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    # gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    # gSystem->ChangeDirectory("../");  
    os.chdir("../");

if(runDataCardsParametrization):
    os.chdir("AnalysisCode/");
    print "running .x DataCardsParametrization.C(\"../JobOutputs/"+foldername+"\",\"\")"
    os.system("root -l -b -q \'DataCardsParametrization.C(\"../JobOutputs/"+foldername+"\",\"\")\'")
    os.chdir("../");

if(runClosureTest):
    # cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_5_3_3_patch3/src; SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -;
    shutil.copyfile("AnalysisCode/ClosureTest_fits.C","JobOutputs/"+foldername+"/DataCards/ClosureTest_fits.C");
    # shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/DataCards/common.h")
    os.chdir("JobOutputs/"+foldername+"/DataCards");
    print os.getcwd()
    os.system("cd /afs/cern.ch/work/p/perrozzi/private/CMSSW_6_1_1/src; SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -; root -l -b -q \'ClosureTest_fits.C()\'")
    os.chdir("../");

if(runClosureTestLikeLihoodRatioAnsMergeResults):
    # cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_5_3_3_patch3/src; SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -;
    shutil.copyfile("AnalysisCode/ClosureTest_fits_likelihoodratio.C","JobOutputs/"+foldername+"/DataCards/ClosureTest_fits.C");
    # shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/DataCards/common.h")
    os.chdir("JobOutputs/"+foldername+"/DataCards");
    print os.getcwd()
    os.system("cd /afs/cern.ch/work/p/perrozzi/private/CMSSW_6_1_1/src; SCRAM_ARCH=slc5_amd64_gcc462;eval `scramv1 runtime -sh`; cd -; root -l -b -q \'ClosureTest_fits.C()\'")
    os.chdir("../../../");

if(runClosureTestLikeLihoodRatioAnsMergeResults or mergeResults):
    print "ciao"
    # shutil.copyfile("AnalysisCode/merge_results.C",'JobOutputs/'+foldername+'/DataCards/merge_results.C');
    os.system("cp AnalysisCode/merge_results.C JobOutputs/"+foldername+"/DataCards/merge_results.C");
    os.chdir("JobOutputs/"+foldername+"/DataCards");
    print os.getcwd();
    os.system("root -l -b -q \'merge_results.C+()\'");
    os.chdir("../");


if(run_Z_MCandDATAcomparisons_stack):
    # gROOT->ProcessLine(".! touch *.*");
    # gSystem->ChangeDirectory("DATAcode/");
    os.chdir("PlottingCode/");  
    if not os.path.exists("../JobOutputs/"+foldername+"/ZcomparisonPlots_MCvsDATA"): os.makedirs("../JobOutputs/"+foldername+"/ZcomparisonPlots_MCvsDATA")
    jobIDMCsig= "../JobOutputs/"+foldername+"/test_numbers_"+sample[DYJetsSig]
    jobIDMCEWK= "../JobOutputs/"+foldername+"/test_numbers_EWK"
    jobIDMCTT= "../JobOutputs/"+foldername+"/test_numbers_"+sample[TTJets]
    jobIDMCQCD= "../JobOutputs/"+foldername+"/test_numbers_"+sample[QCD]
    jobIDDATA= "../JobOutputs/"+foldername+"/test_numbers_"+sample[DATA]
    os.system("root -l -b -q \'PlotZdistributionsMCvsDATA_stack.C(\""+jobIDMCsig+"/\",\""+jobIDMCEWK+"/\",\""+jobIDMCTT+"/\",\""+jobIDMCQCD+"/\",\""+jobIDDATA+"/\")\' ");
    os.system("mv *.png ../JobOutputs/"+foldername+"/ZcomparisonPlots_MCvsDATA");
    os.system("cp PlotZdistributionsMCvsDATA_stack.C ../JobOutputs/"+foldername+"/ZcomparisonPlots_MCvsDATA");
    # gSystem->ChangeDirectory("../");

if(run_W_MCandDATAcomparisons_stack):
    # gROOT->ProcessLine(".! touch *.*");
    # gSystem->ChangeDirectory("DATAcode/");
    os.chdir("PlottingCode/");  
    if not os.path.exists("../JobOutputs/"+foldername+"/WcomparisonPlots_MCvsDATA"): os.makedirs("../JobOutputs/"+foldername+"/WcomparisonPlots_MCvsDATA")
    jobIDMCsig= "../JobOutputs/"+foldername+"/test_numbers_"+sample[WJetsSig]
    jobIDMCEWK= "../JobOutputs/"+foldername+"/test_numbers_EWK"
    jobIDMCTT= "../JobOutputs/"+foldername+"/test_numbers_"+sample[TTJets]
    jobIDMCQCD= "../JobOutputs/"+foldername+"/test_numbers_"+sample[QCD]
    jobIDDATA= "../JobOutputs/"+foldername+"/test_numbers_"+sample[DATA]
    os.system("root -l -b -q \'PlotWdistributionsMCvsDATA_stack.C(\""+jobIDMCsig+"/\",\""+jobIDMCEWK+"/\",\""+jobIDMCTT+"/\",\""+jobIDMCQCD+"/\",\""+jobIDDATA+"/\")\' ");
    os.system("mv *.png ../JobOutputs/"+foldername+"/WcomparisonPlots_MCvsDATA");
    os.system("cp PlotWdistributionsMCvsDATA_stack.C ../JobOutputs/"+foldername+"/WcomparisonPlots_MCvsDATA");
    # gSystem->ChangeDirectory("../");

if(run_PhiStarEta_MCandDATAcomparisons_stack):
    # gROOT->ProcessLine(".! touch *.*");
    # gSystem->ChangeDirectory("DATAcode/");
    os.chdir("PlottingCode/");  
    if not os.path.exists("../JobOutputs/"+foldername+"/PhiStarEtacomparisonPlots_MCvsDATA"): os.makedirs("../JobOutputs/"+foldername+"/PhiStarEtacomparisonPlots_MCvsDATA")
    jobIDMCsig= "../JobOutputs/"+foldername+"/test_numbers_"+sample[DYJetsSig]
    jobIDMCEWK= "../JobOutputs/"+foldername+"/test_numbers_EWK"
    jobIDMCTT= "../JobOutputs/"+foldername+"/test_numbers_"+sample[TTJets]
    jobIDMCQCD= "../JobOutputs/"+foldername+"/test_numbers_"+sample[QCD]
    jobIDDATA= "../JobOutputs/"+foldername+"/test_numbers_"+sample[DATA]
    os.system("root -l -b -q \'PlotPhiStarEtadistributionsMCvsDATA_stack.C(\""+jobIDMCsig+"/\",\""+jobIDMCEWK+"/\",\""+jobIDMCTT+"/\",\""+jobIDMCQCD+"/\",\""+jobIDDATA+"/\")\' ");
    os.system("mv *.png ../JobOutputs/"+foldername+"/PhiStarEtacomparisonPlots_MCvsDATA");
    os.system("cp PlotPhiStarEtadistributionsMCvsDATA_stack.C ../JobOutputs/"+foldername+"/PhiStarEtacomparisonPlots_MCvsDATA");
    # gSystem->ChangeDirectory("../");

if(runWSigBkgFit):
      # gROOT->ProcessLine(".! touch *.*");
      os.chdir("SignalExtraction/");  
      os.system("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitWm.C+\(\\\""+foldername+"/\\\"\)");
      os.chdir("../");  

if(runZSigBkgFit):
    # gROOT->ProcessLine(".! touch *.*");
    os.chdir("SignalExtraction/");  
    # print ("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C  fitWm.C+\(\\\""+foldername+"/\\\"\)");
    print ("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitZmm.C+\(\\\""+foldername+"/\\\"\)");
    os.system("source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitZmm.C+\(\\\""+foldername+"/\\\"\)");
    os.chdir("../");  




print ''