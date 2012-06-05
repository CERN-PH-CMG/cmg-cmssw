import os

dataset_user = 'hinzmann' 
dataset_files = 'ntuple.*root'
dataset_names = [
#'/Jet/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/NTUPLE2',
#'/HT/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/NTUPLE2',
#'/JetHT/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/NTUPLE2',
'/JetHT/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/NTUPLE2',
#'/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM/V5/PAT_CMG_V5_4_0/NTUPLE2',
]

from CMGTools.Production.datasetToSource import *

for dataset_name in dataset_names:
    print dataset_name
    os.system("edmIntegrityCheck.py "+dataset_name+" -w '*.qwe'")
    source = datasetToSource(
        dataset_user,
        dataset_name,
        dataset_files,
    )
    directory="/tmp/hinzmann/"+dataset_name.replace("/","_")
    os.system("mkdir "+directory)
    filelist=open("fileList"+dataset_name.replace("/","_")+".txt","w")
    for filename in source.fileNames:
        filelist.write(directory+"/"+filename.split("/")[-1]+"\n")
        print "cmsStage "+filename+" "+directory
        os.system("cmsStage "+filename+" "+directory)
    filelist.close()
    