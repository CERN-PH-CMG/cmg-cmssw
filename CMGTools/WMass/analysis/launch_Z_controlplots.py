#!/usr/local/bin/python

# change above line to point to local 
# python executable

import urllib, urlparse, string, time, os, shutil, sys

## ==============================================================
## STEERING PARAMETERS
## ==============================================================

useLHAPDF = False # DEFAULT IS False, before to switch to True follow https://twiki.cern.ch/twiki/bin/view/CMS/CmgWMass#LHAPDF
  
foldername = "test_cout";

ntuple_folder = "root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
# ntuple_folder = "~/eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
lhapdf_folder="/afs/cern.ch/work/p/perrozzi/private/WMassMC/lhapdf/"

usePileupSF = 1; # 0=no, 1=yes
useEffSF = 1; # 0=no, 1=yes
usePtSF = 1; # 0=no, 1=yes
useMomentumCorr = 1; # 0=none, 1=Rochester, 2=MuscleFit
LocalSmearingRochCorrNToys = 0;
GlobalSmearingRochCorrNsigma = 0;
LHAPDF_reweighting_sets="10042" # cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
LHAPDF_reweighting_members="1" # cteq6ll.LHpdf=1 CT10nnlo.LHgrid=51, NNPDF23_nnlo_as_0118.LHgrid=100, MSTW2008nnlo68cl.LHgrid=41

indip_normalization_lumi_MC = 0; # independent normalization of MC in fb-1 (otherwise normalized to DATA)
intLumi_MC_fb = 81293448/31314/1e3;# data = 4.7499 fb-1 prescaled trigger, 5.1 fb-1 unprescaled; works only if indip_normalization_lumi_MC is TRUE
useAlsoGenPforSig= 1;

etaMuonNSteps = "1"; # "5"
etaMaxMuons = "0.6"; # "0.6, 0.8, 1.2, 1.6, 2.1"
# etaMaxMuons = "2.1"; # "0.6, 0.8, 1.2, 1.6, 2.1"

parallelize = 0; # loop over the samples launching the program in batch
resumbit_sample = "WZJets" # "DATA , WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD"

runWanalysis = 0; # perform the analysis on W control plots
Zanalysis_controlplots = 1; # perform the analysis on Z control plots

mergeSigEWKbkg = 0; # after running on all the samples, create merged EWK, EWK+TT, DATA-LIKE (sum all)

#######################
### PLOTTING ###
#######################
runWSigBkgFit = 0;
runZSigBkgFit = 0;

run_Z_MCandDATAcomparisons_stack = 0;
run_W_MCandDATAcomparisons_stack = 0;
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

ZMass = "91.188"; # 91.1876
WMassCentral_MeV = "80419"; # 80385
WMassStep_MeV = "10"; # 15
WMassNSteps = "0"; # 60
# runWanalysis = 1; # perform the analysis on W
# Zanalysis_controlplots = 1; # perform the analysis on Z control plots
# controlplots = 1;
IS_MC_CLOSURE_TEST=0;
controlplots=0;

print "cd /afs/cern.ch/work/p/perrozzi/private/CMSSW_6_1_1/src; SCRAM_ARCH=slc5_amd64_gcc462;eval `scramv1 runtime -sh`; cd -;"

if(LocalSmearingRochCorrNToys<1):
  LocalSmearingRochCorrNToys=1
if(useMomentumCorr==1): 
  foldername+="_RochCorr";
  if(GlobalSmearingRochCorrNsigma>0): 
    foldername+=str(GlobalSmearingRochCorrNsigma)+"s_smear";
else: 
    if(useMomentumCorr==2): foldername+="_MuscleFitCorr";

if(useEffSF==1): foldername+="_EffSFCorr";
if(usePtSF==1): foldername+="_PtSFCorr";
if(usePileupSF==1): foldername+="_PileupSFCorr";
nsamples=10;
DATA , WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD = range(10)
sample      = [ "DATA" , "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"];
isMCorDATA  = [      1 ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,   0      ];
CS_pb       = [     -1 ,    31314  ,      31314 ,   2895.6   ,      2895.6 ,   157.51 ,   0.119  ,   4.514  ,   0.596 , 84679.3  ];
                                                                                                                                            # # Run2011A  Run2011B  JSON  TRIGGER GOOD VTX
Nevts       = [  53056567 , 81293448  ,   81293448 ,  36225556  ,    36225556 , 59314581 , 1103468  , 1197558  , 1221134 , 24958039   ]; # 44X
int_lumi_fb = [       5.1 ,        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ];
Nevts_Per_fb= [        0  ,         0 ,        0   ,          0 ,           0 ,        0 ,       0  ,       0  ,       0 ,        0   ];
generated_PDF_set =     [       LHAPDF_reweighting_sets  ,    10042 ,    10042 ,     10042 ,   10042 ,     10042 ,     10042 ,     10042 ,     10042 ,     10042 ];
generated_PDF_member =  [       LHAPDF_reweighting_members  ,        0 ,        0 ,        0 ,        0 ,         0 ,         0 ,         0 ,         0 ,         0 ];
contains_PDF_reweight = [       0  ,        0 ,        0 ,        0 ,        0 ,         0 ,         0 ,         0 ,         0 ,         0 ];


fWana_str = [
  ntuple_folder+"DATA/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"WJets/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"WJets/WTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"DYJets/WTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"DYJets/WTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"TTJets/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/ZZ/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/WW/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/WZ/WTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"QCD/WTreeProducer_tree.root"
];  
fZana_str = [
  ntuple_folder+"DATA/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"WJets/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"WJets/ZTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"DYJets/ZTreeProducer_tree_SignalRecoSkimmed.root",
  ntuple_folder+"DYJets/ZTreeProducer_tree_FakeRecoSkimmed.root",
  ntuple_folder+"TTJets/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/ZZ/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/WW/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"VVJets/WZ/ZTreeProducer_tree_RecoSkimmed.root",
  ntuple_folder+"QCD/ZTreeProducer_tree.root"
];

print os.getcwd()
print ".! cp launch_analysis.py JobOutputs/",foldername;
file_dest="JobOutputs/"+foldername+"/launch_analysis.py";
if not os.path.exists("JobOutputs/"+foldername):
    os.makedirs("JobOutputs/"+foldername)
shutil.copyfile("launch_analysis.py", file_dest)


if(runWanalysis or Zanalysis_controlplots ):
    
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
        
        if indip_normalization_lumi_MC:
            WfileDATA_lumi_SF = intLumi_MC_fb/int_lumi_fb[i]
        else:
            WfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i]
        
        ZfileDATA_lumi_SF = WfileDATA_lumi_SF
        
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


        if parallelize and (runWanalysis or Zanalysis_controlplots) and counter>1:
            print "waiting 10 sec to 20 sec (if W and Z are launched) before to proceed with the next sample to let the compilation finish"
            os.system("sleep 3");
        
        # if(i==0): os.system("touch *.*");
        
        if(runWanalysis):
            
            wstring="\""+WfileDATA+"\","+str(WfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(GlobalSmearingRochCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_PDF_reweight[i])
            if(counter<2):
                if(useLHAPDF):
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\#define\ LHAPDF_ON/' Wanalysis.C")
                    print("c++ -o runWanalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Wanalysis.C rochcor_44X_v3.C runWanalysis.C")
                    os.system("rm runWanalysis.o; c++ -o runWanalysis.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Wanalysis.C rochcor_44X_v3.C runWanalysis.C")
                else:
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\/\/\#define\ LHAPDF_ON/' Wanalysis.C")
                    print("c++ -o runWanalysis.o `root-config --glibs --libs --cflags` -lm Wanalysis.C rochcor_44X_v3.C runWanalysis.C")
                    os.system("rm runWanalysis.o; c++ -o runWanalysis.o `root-config --glibs --libs --cflags`  -lm Wanalysis.C rochcor_44X_v3.C runWanalysis.C")
            
            # print("root -l -b -q \'runWanalysis2.cpp("+wstring+")\'");
            
            # os.system("root -l -b -q \'runWanalysis2.cpp("+wstring+")\'");
            print " "
            # print wstring
            if not parallelize:
                os.system("./runWanalysis.o "+wstring)
                # os.system("root -l -b -q \'runWanalysis.C("+wstring+")\'");
            else:
                os.system("./runWanalysis.o "+wstring+" > ../"+filename_outputdir+"Wlog.log 2>&1 &")
                # print "root -l -b -q \'runWanalysis.C("+wstring+")\' ../> "+filename_outputdir+"Wlog.log 2>&1 &";
                # os.system("root -l -b -q \'runWanalysis.C("+wstring+")\' > ../"+filename_outputdir+"Wlog.log 2>&1 &");

        if(Zanalysis_controlplots):

            zstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(GlobalSmearingRochCorrNsigma)+","+str(useEffSF)+","+str(usePtSF)+","+str(usePileupSF)+","+str(0)+","+str(controlplots)+","+str(generated_PDF_set[i])+""+","+str(generated_PDF_member[i])+","+str(contains_PDF_reweight[i])
            
            if(counter<2):
            # os.system("touch *.*");
                if(useLHAPDF):
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\#define\ LHAPDF_ON/' Zanalysis_controlplots.C")
                    print("c++ -o Zanalysis_controlplots.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Zanalysis_controlplots.C rochcor_44X_v3.C runZanalysis_controlplots.C ../includes/common.h RecoilCorrector.cc")
                    os.system("rm Zanalysis_controlplots.o; c++ -o Zanalysis_controlplots.o `root-config --glibs --libs --cflags`  -I "+lhapdf_folder+"/include -L "+lhapdf_folder+"/lib -lLHAPDF  -lm Zanalysis_controlplots.C rochcor_44X_v3.C runZanalysis_controlplots.C ../includes/common.h RecoilCorrector.cc")                    
                else:
                    os.system("sed -i 's/.*\#define\ LHAPDF_ON.*/\/\/\#define\ LHAPDF_ON/' Zanalysis_controlplots.C")
                    print("c++ -o Zanalysis_controlplots.o `root-config --glibs --libs --cflags`  -lm Zanalysis_controlplots.C rochcor_44X_v3.C runZanalysis_controlplots.C ../includes/common.h RecoilCorrector.cc")
                    os.system("rm Zanalysis_controlplots.o; c++ -o Zanalysis_controlplots.o `root-config --glibs --libs --cflags`  -lm Zanalysis_controlplots.C rochcor_44X_v3.C runZanalysis_controlplots.C ../includes/common.h RecoilCorrector.cc")

            print zstring
            if not parallelize:
                # os.system("root -l -b -q \'runZanalysis_controlplots.C("+zstring+")\'");
                os.system("./Zanalysis_controlplots.o "+zstring)

            else:
                if(runWanalysis): os.system("sleep 3");
                os.system("./Zanalysis_controlplots.o "+zstring+" > ../"+filename_outputdir+"Zlog.log 2>&1 &")
                # print "root -l -b -q \'runZanalysis_controlplots.C("+zstring+")\' > ../"+filename_outputdir+"Zlog.log 2>&1 &";
                # os.system("root -l -b -q \'runZanalysis_controlplots.C("+zstring+")\' > ../"+filename_outputdir+"/Zlog.log 2>&1 &");

                
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

# if(run_W_MCandDATAcomparison){
  # gROOT->ProcessLine(".! touch *.*");
  # # gSystem->ChangeDirectory("DATAcode/");
  # gSystem->MakeDirectory(Form("../JobOutputs/%s/WcomparisonPlots_MCvsDATA",foldername.Data()));
  # TString jobIDMC= Form("test_numbers_%s",sample[WJetsSig].Data());
  # TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
  # gROOT->ProcessLine(Form(".x PlotWdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
  # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/WcomparisonPlots_MCvsDATA",foldername.Data()));
  # gROOT->ProcessLine(Form(".! cp PlotWdistributionsMCvsDATA.C ../JobOutputs/%s/WcomparisonPlots_MCvsDATA",foldername.Data()));
  # # gSystem->ChangeDirectory("../");
# }


# # # gSystem->ChangeDirectory("../DATAcode/");
# # gSystem->MakeDirectory(Form("../JobOutputs/%s/ZcomparisonPlots_EWKvsDATA",foldername.Data()));
# # TString jobIDMC= Form("test_numbers_EWK");
# # TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
# # gROOT->ProcessLine(Form(".x PlotZdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
# # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/ZcomparisonPlots_EWKvsDATA",foldername.Data()));
# # gROOT->ProcessLine(Form(".! cp PlotZdistributionsMCvsDATA.C ../JobOutputs/%s/ZcomparisonPlots_EWKvsDATA",foldername.Data()));
# # gSystem->MakeDirectory(Form("../JobOutputs/%s/WcomparisonPlots_EWKvsDATA",foldername.Data()));
# # TString jobIDMC= Form("test_numbers_EWK");
# # TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
# # gROOT->ProcessLine(Form(".x PlotWdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
# # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/WcomparisonPlots_EWKvsDATA",foldername.Data()));
# # gROOT->ProcessLine(Form(".! cp PlotWdistributionsMCvsDATA.C ../JobOutputs/%s/WcomparisonPlots_EWKvsDATA",foldername.Data()));
# # # gSystem->ChangeDirectory("../");

# }

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
