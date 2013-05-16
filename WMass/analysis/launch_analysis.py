#!/usr/local/bin/python

# change above line to point to local 
# python executable

import urllib, urlparse, string, time, os, shutil

## ==============================================================
## STEERING PARAMETERS
## ==============================================================
  
# foldername = "results_test44X_100massPoints3etabins";
# foldername = "results_test44X_100massPoints";
# foldername = "results_test44X_RescaleToWJetsLumi";
# foldername = "results_test44X_MCDATAcomparison";
# foldername = "results_test44X_correctPoles";
foldername = "test_fit";

usePileupSF = 1; # 0=no, 1=yes
useEffSF = 1; # 0=no, 1=yes
useMomentumCorr = 1; # 0=none, 1=Rochester, 2=MuscleFit
smearRochCorrByNsigma = 0;
## CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
IS_MC_CLOSURE_TEST= 0; 
normalize_lumi_MC_CLOSURE_TEST = 1;
intLumi_MC_CLOSURE_TEST_fb = 5.1;# data = 4.7499 prescaled trigger, 5.1 unprescaled; works only if normalize_lumi_MC_CLOSURE_TEST is TRUE
useAlsoGenPforSig= 0;

ZMass = "91.188"; # 91.1876
WMassCentral_MeV = "80419"; # 80385
WMassStep_MeV = "5"; # 15
WMassNSteps = "50"; # 60
etaMuonNSteps = "2"; # 5
etaMaxMuons = "0.6, 2.1"; # 0.6, 0.8, 1.2, 1.6, 2.1

parallelize = 0;
resumbit_sample = ""

runWanalysis = 1;
runZanalysis = 0;

mergeSigEWKbkg   = 0;

ExtractNumbers = 0;

## PRODUCE R(X)=W/Z DISTRIBUTION TO REWEIGHT Z in DATA
runR_WdivZ= 0;
## PRODUCE TEMPLATES, i.e. Z(DATA)*R(X)
run_BuildSimpleTemplates= 0;
run_BuildEvByEvTemplates= 0;

## PERFORM W MASS FIT
runPrepareDataCards = 0;
runDataCardsParametrization = 0;

## NEW FIT
print "if it doesn't work, try with this first: cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_5_3_3_patch3/src; setenv SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -";
runClosureTestLikeLihoodRatio = 0;
mergeResults = 0;

## OLD FIT
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

print "cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_6_1_1/src; setenv SCRAM_ARCH=slc5_amd64_gcc462;eval `scramv1 runtime -sh`; cd -;"

if(IS_MC_CLOSURE_TEST==1):
    foldername+="_MCclosureTest";
    useEffSF=0;
    usePileupSF=0;

if(useMomentumCorr==1): foldername+="_RochCorr";
else: 
    if(useMomentumCorr==2): foldername+="_MuscleFitCorr";

if(useEffSF==1): foldername+="_EffSFCorr";
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

common_folder = "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/";

fWana_str = [
  common_folder+"batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree.root",
  common_folder+"batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree_SignalRecoSkimmed.root",
  common_folder+"batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree_FakeRecoSkimmed.root",
  common_folder+"batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree_SignalRecoSkimmed.root",
  common_folder+"batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree_FakeRecoSkimmed.root",
  common_folder+"batch_TT_Trg/TTJets/WTreeProducer/WTreeProducer_tree.root",
  common_folder+"batch_VV_Trg/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
  common_folder+"batch_VV_Trg/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
  common_folder+"batch_VV_Trg/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree.root",
  common_folder+"batch_QCD_Trg/QCD15/WTreeProducer/WTreeProducer_tree.root"
];  
fZana_str = [
  common_folder+"batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree.root",
  common_folder+"batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree_SignalRecoSkimmed.root",
  common_folder+"batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree_FakeRecoSkimmed.root",
  common_folder+"batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree_SignalRecoSkimmed.root",
  common_folder+"batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree_FakeRecoSkimmed.root",
  common_folder+"batch_TT_Trg/TTJets/ZTreeProducer/ZTreeProducer_tree.root",
  common_folder+"batch_VV_Trg/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
  common_folder+"batch_VV_Trg/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
  common_folder+"batch_VV_Trg/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree.root",
  common_folder+"batch_QCD_Trg/QCD15/ZTreeProducer/ZTreeProducer_tree.root"
];

print os.getcwd()
print ".! cp launch_analysis.py JobOutputs/",foldername;
file_dest="JobOutputs/"+foldername+"/launch_analysis.py";
if not os.path.exists("JobOutputs/"+foldername):
    os.makedirs("JobOutputs/"+foldername)
shutil.copyfile("launch_analysis.py", file_dest)


if(runWanalysis or runZanalysis or run_BuildEvByEvTemplates):
    for i in range(0, nsamples):
    
        if (resumbit_sample!=""):
            # parallelize = 0
            if not resumbit_sample in sample[i]: 
                print "skipping ",sample[i]," which is not",resumbit_sample;
                continue;
          
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
        
        if IS_MC_CLOSURE_TEST:
            if normalize_lumi_MC_CLOSURE_TEST:
                WfileDATA_lumi_SF = intLumi_MC_CLOSURE_TEST_fb/int_lumi_fb[i]
            else: 
                WfileDATA_lumi_SF = 1
        else:
            WfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i]
        
        ZfileDATA_lumi_SF = WfileDATA_lumi_SF
        
        # WfileDATA_lumi_SF = ((intLumi_MC_CLOSURE_TEST_fb/int_lumi_fb[i]) if normalize_lumi_MC_CLOSURE_TEST==1 else 1) if IS_MC_CLOSURE_TEST==1 else int_lumi_fb[DATA]/int_lumi_fb[i];
        # ZfileDATA_lumi_SF = ((intLumi_MC_CLOSURE_TEST_fb/int_lumi_fb[i]) if normalize_lumi_MC_CLOSURE_TEST==1 else 1) if IS_MC_CLOSURE_TEST==1 else int_lumi_fb[DATA]/int_lumi_fb[i];
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
        
        if( dir_check ==0 ): # if the output folder existed use the same common.h
        # if( False ): # if the output folder existed use the same common.h
            print "using JobOutputs/"+foldername+"/"+outputdir+"/common.h"
            shutil.copyfile(("JobOutputs/"+foldername+"/"+outputdir+"/common.h"), "includes/common.h")
        else: # otherwise build it from this cfg
            print "creating JobOutputs/"+foldername+"/"+outputdir+"/common.h from includes/common.h.bkp"
            shutil.copyfile("includes/common.h.bkp", "includes/common.h");
            os.system("sh "+os.getcwd()+"/manipulate_parameters.sh "+ZMass+" "+WMassCentral_MeV+" "+WMassStep_MeV+" "+WMassNSteps+" "+etaMuonNSteps+" \""+etaMaxMuons+"\"")
            shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/"+outputdir+"/common.h");

        os.chdir("AnalysisCode/");
        # print os.system("rm *.d")
        # print os.system("rm *.so")
        print os.getcwd()


        if parallelize and (runWanalysis or runZanalysis) and i>0:
            print "waiting 30 sec to 60 sec (if W and Z are launched) before to proceed with the next sample to let the compilation finish"
            os.system("sleep 30");
        
        # if(i==0): os.system("touch *.*");
        
        if(runWanalysis):
            wstring="\""+WfileDATA+"\","+str(WfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(smearRochCorrByNsigma)+","+str(useEffSF)+","+str(usePileupSF)
            # print wstring
            if not parallelize:
                os.system("root -l -b -q \'runWanalysis.C("+wstring+")\'");
            else:
                # print "root -l -b -q \'runWanalysis.C("+wstring+")\' ../> "+filename_outputdir+"Wlog.log 2>&1 &";
                os.system("root -l -b -q \'runWanalysis.C("+wstring+")\' > ../"+filename_outputdir+"Wlog.log 2>&1 &");

        if(runZanalysis):
            # os.system("touch *.*");
            zstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(smearRochCorrByNsigma)+","+str(useEffSF)+","+str(usePileupSF)+","+str(0)
            # print zstring
            if not parallelize:
                os.system("root -l -b -q \'runZanalysis.C("+zstring+")\'");
            else:
                if(runWanalysis): os.system("sleep 30");
                # print "root -l -b -q \'runZanalysis.C("+zstring+")\' > ../"+filename_outputdir+"Zlog.log 2>&1 &";
                os.system("root -l -b -q \'runZanalysis.C("+zstring+")\' > ../"+filename_outputdir+"/Zlog.log 2>&1 &");

        if(run_BuildEvByEvTemplates):
            zTemplstring="\""+ZfileDATA+"\","+str(ZfileDATA_lumi_SF)+",\""+sample[i]+"\","+str(useAlsoGenPforSig)+","+str(IS_MC_CLOSURE_TEST)+","+str(isMCorDATA[i])+",\""+filename_outputdir+"\","+str(useMomentumCorr)+","+str(smearRochCorrByNsigma)+","+str(useEffSF)+","+str(usePileupSF)+","+str(run_BuildEvByEvTemplates)
            # print zTemplstring
            # exit()
            if not parallelize:
                os.system("root -l -b -q \'runZanalysis.C("+zTemplstring+")\'");
            else:
                if(runWanalysis): os.system("sleep 30");
                # print "root -l -b -q \'runZanalysis.C("+zTemplstring+")\' > ../"+filename_outputdir+"Zlog.log 2>&1 &";
                os.system("root -l -b -q \'runZanalysis.C("+zTemplstring+")\' > ../"+filename_outputdir+"/ZEvByEvTemplog.log 2>&1 &");
                # gROOT->ProcessLine(".! touch *.*");
                # gSystem->CompileMacro("rochcor_44X_v3.C");
                # # gSystem->CompileMacro("rochcor_42X.C");
                # # gSystem->CompileMacro("MuScleFitCorrector.cc");
                # gSystem->CompileMacro("Zanalysis.C");
                # # cout << "int_lumi_fb[IS_MC_CLOSURE_TEST? DYJetsSig : DATA]= "<<(IS_MC_CLOSURE_TEST? intLumi_MC_CLOSURE_TEST_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1) << endl;
                # cout << Form("Zanalysis zTEMPLATESOnDATA(\"%s\",%.f)",fZana_str[IS_MC_CLOSURE_TEST? DYJetsSig : DATA].Data(),IS_MC_CLOSURE_TEST? intLumi_MC_CLOSURE_TEST_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1) << endl;
                # gROOT->ProcessLine(Form("Zanalysis zTEMPLATESOnDATA(\"%s\",%.f)",fZana_str[IS_MC_CLOSURE_TEST? DYJetsSig : DATA].Data(),IS_MC_CLOSURE_TEST? intLumi_MC_CLOSURE_TEST_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1));
                # # gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d,1,%s,1)",IS_MC_CLOSURE_TEST,filename_outputdir.Data()));
                # cout << "processing line " << Form("zTEMPLATESOnDATA.Loop(%d,1,\"../JobOutputs/%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,foldername.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
                # gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d,1,\"../JobOutputs/%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,foldername.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
                                                        # # # ".Loop(%d,%d,\"../%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[i].Data()));
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
                # cout << "running " << Form(".! root -l -b -q \'BuildSimpleTemplates.C(\"../JobOutputs/%s/%s\")\'",foldername.Data(),outputdir.Data()) << endl;
                os.system("root -l -b -q \' BuildSimpleTemplates.C( \"../JobOutputs/"+foldername+"/"+dir+"\" ) \' ");
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
    os.system("root -l -b -q \'R_WdivZ.C(\"../JobOutputs/"+foldername+"\")\'");
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
    os.system("root -l -b -q \'prepareDatacards.C(\"../JobOutputs/"+foldername+"\",\"\")\'")
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
    # cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_5_3_3_patch3/src; setenv SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -;
    shutil.copyfile("AnalysisCode/ClosureTest_fits.C","JobOutputs/"+foldername+"/DataCards/ClosureTest_fits.C");
    # shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/DataCards/common.h")
    os.chdir("JobOutputs/"+foldername+"/DataCards");
    print os.getcwd()
    os.system("cd /afs/cern.ch/work/p/perrozzi/private/CMSSW_6_1_1/src; setenv SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -; root -l -b -q \'ClosureTest_fits.C()\'")
    os.chdir("../");

if(runClosureTestLikeLihoodRatio):
    # cd /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_5_3_3_patch3/src; setenv SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -;
    shutil.copyfile("AnalysisCode/ClosureTest_fits_likelihoodratio.C","JobOutputs/"+foldername+"/DataCards/ClosureTest_fits.C");
    # shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/DataCards/common.h")
    os.chdir("JobOutputs/"+foldername+"/DataCards");
    print os.getcwd()
    os.system("cd /afs/cern.ch/work/p/perrozzi/private/CMSSW_6_1_1/src; setenv SCRAM_ARCH slc5_amd64_gcc462;cmsenv; cd -; root -l -b -q \'ClosureTest_fits.C()\'")
    os.chdir("../");

if(mergeResults):
    shutil.copyfile("AnalysisCode/merge_results.C","JobOutputs/"+foldername+"/DataCards/merge_results.C");
    # shutil.copyfile("includes/common.h","JobOutputs/"+foldername+"/DataCards/common.h")
    os.chdir("JobOutputs/"+foldername+"/DataCards");
    print os.getcwd()
    os.system("root -l -b -q \'merge_results.C()\'")
    os.chdir("../");
    
    
    
    # if(run_MassFit){  
      # gROOT->ProcessLine(".! touch *.*");
      # gROOT->ProcessLine(Form(".x TemplateFit.C(\"../JobOutputs/%s/%s/WanalysisOnDATA.root\",\"../JobOutputs/%s/Templates_from_ZanalysisOnDATA.root\",\"../JobOutputs/%s/R_WdivZ_OnMC.root\",%d",
                          # foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()),foldername.Data(),foldername.Data(),fitType));
      # gROOT->ProcessLine(Form(".! mv FitResults.root ../JobOutputs/%s/FitResults%s.root",foldername.Data(),fitType==0?"_ROOTchi2Fit":"_CustomChi2Fit"));
      # gROOT->ProcessLine(Form(".! cp TemplateFit.C ../JobOutputs/%s/",foldername.Data()));
    # }

# gSystem->ChangeDirectory("../PlottingCode");

# if(run_ZvsWlike_comparisonMC){
  # gROOT->ProcessLine(".! touch *.*");
  # # gSystem->ChangeDirectory("DATAcode/");
  # gSystem->MakeDirectory(Form("../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
  # TString jobIDZ= Form("test_numbers_%s",sample[DYJetsSig].Data());
  # gROOT->ProcessLine(Form(".x PlotZvsWlikeDistributionsDATA.C(\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data()));
  # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
  # gROOT->ProcessLine(Form(".! cp PlotZvsWlikeDistributionsDATA.C ../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
  # # gSystem->ChangeDirectory("../");
# }
# if(run_ZvsWlike_comparisonDATA){
  # gROOT->ProcessLine(".! touch *.*");
  # # gSystem->ChangeDirectory("DATAcode/");
  # gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
  # TString jobIDZ= Form("test_numbers_%s",sample[DATA].Data());
  # gROOT->ProcessLine(Form(".x PlotZvsWlikeDistributionsDATA.C(\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data()));
  # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
  # gROOT->ProcessLine(Form(".! cp PlotZvsWlikeDistributionsDATA.C ../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
  # # gSystem->ChangeDirectory("../");
# }

    # if(runWandZcomparisonMC){
      # gROOT->ProcessLine(".! touch *.*");
      # # gSystem->ChangeDirectory("DATAcode/");
      # gSystem->MakeDirectory(Form("../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
      # TString jobIDZ= Form("test_numbers_%s",sample[DYJetsSig].Data());
      # TString jobIDW= Form("test_numbers_%s",sample[WJetsSig].Data());
      # gROOT->ProcessLine(Form(".x PlotWvsZdistributionsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data(),foldername.Data(),jobIDW.Data()));
      # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
      # gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
      # # gSystem->ChangeDirectory("../");
    # }

    # if(runWandZcomparisonDATA){
      # gROOT->ProcessLine(".! touch *.*");
      # # gSystem->ChangeDirectory("DATAcode/");
      # gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
      # gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
      # TString jobIDZ= Form("test_numbers_%s",sample[DATA].Data());
      # TString jobIDW= Form("test_numbers_%s",sample[DATA].Data());
      # gROOT->ProcessLine(Form(".x PlotWvsZdistributionsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data(),foldername.Data(),jobIDW.Data()));
      # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
      # gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
      # # gSystem->ChangeDirectory("../");
    # }

    # if(run_Z_MCandDATAcomparison){
      # gROOT->ProcessLine(".! touch *.*");
      # # gSystem->ChangeDirectory("DATAcode/");
      # gSystem->MakeDirectory(Form("../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
      # TString jobIDMC= Form("test_numbers_%s",sample[DYJetsSig].Data());
      # TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
      # gROOT->ProcessLine(Form(".x PlotZdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
      # gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
      # gROOT->ProcessLine(Form(".! cp PlotZdistributionsMCvsDATA.C ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
      # # gSystem->ChangeDirectory("../");
    # }
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