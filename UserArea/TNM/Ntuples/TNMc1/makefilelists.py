import os

dataset_user = 'hinzmann' 
dataset_files = 'ntuple.*root'
dataset_names = [

#'/RSGravitonToJJ_M-500_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-700_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-1200_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-2000_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-3500_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-4500_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-500_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-700_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-1200_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-2000_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-3500_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/RSGravitonToJJ_M-4500_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-500_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-700_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2', 
#'/QstarToJJ_M-1200_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-2000_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-3500_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-4500_TuneD6T_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-500_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-700_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-1200_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-2000_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-3500_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
#'/QstarToJJ_M-4500_TuneZ2star_8TeV-pythia6/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',

#'/HT/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/NTUPLE3',
#'/JetHT/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/NTUPLE3',
#'/JetHT/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/NTUPLE3',
#'/JetHT/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195017-196531/NTUPLE3',
'/JetHT/Run2012C-PromptReco-v1/NTUPLE3',
'/JetHT/Run2012C-PromptReco-v2/NTUPLE3_runrange_start-200245',
#'/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE3',
#'/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START50_V15-v1/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE3',
#'/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE3',
#'/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE3',

]

from CMGTools.Production.datasetToSource import *

for dataset_name in dataset_names:
    print dataset_name
    source = datasetToSource(
        dataset_user,
        dataset_name,
        dataset_files,
    )
    if len(source.fileNames)==0:
        os.system("edmIntegrityCheck.py "+dataset_name+" -w '*.qwe'")
        source = datasetToSource(
            dataset_user,
            dataset_name,
            dataset_files,
        )
    print source.fileNames
    filelist=open("fileList"+dataset_name.replace("/","_")+".txt","w")
    for filename in source.fileNames:
        filelist.write("root://eoscms//eos/cms"+filename+"\n")
    filelist.close()
