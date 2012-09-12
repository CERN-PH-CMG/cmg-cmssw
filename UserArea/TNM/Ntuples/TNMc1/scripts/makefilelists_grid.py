import os
import subprocess

samples = [
#('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START50_V15-v1/AODSIM',True,False),
#('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/WWtoAnything_ptmin500_TuneZ2Star_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
('/QCD_Pt-15to3000_Tune4C_Flat_8TeV_pythia8/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM',True,False),
('/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
('/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
('/HT/Run2012A-PromptReco-v1/RECO',False,False),
('/JetHT/Run2012B-PromptReco-v1/AOD',False,False),
#('/yxin_RSG_WW_1000_pythia6_01/yxin-yxin_RSG_WW_1000_pythia6_01-52e9c298e8547223f910bab8db11615e/USER',True,True),
#('/yxin_RSG_WW_2000_pythia6_01/yxin-yxin_RSG_WW_2000_pythia6_01-54dbdee3e49fbf0c9fb4ed9452c44bd3/USER',True,True),
]

for sample, mc, fastsim in samples:
    print sample, "mc", mc, "fastsim", fastsim
    if mc:
        shortsample=sample.strip("/").split("/")[0]+"_grid"
    else:
        shortsample="_".join(sample.strip("/").split("/"))+"_grid"
    filelist=open("fileList_"+shortsample+".txt","w")
    files=os.listdir("crab_"+shortsample+"/res")
    command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE/"+shortsample+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
    os.system(command)
    command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE/"+shortsample
    p=subprocess.Popen([command],shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    files=p.stdout
    filesl=[]
    for line in files:
        if "ntuple_" in line:
              filename=line.split(" ")[-1].strip()
	if filename.endswith(".root"):
	  sample_id="_".join(filename.split("/")[-1].split("_")[0:2])
	  found=False
	  for f in filesl:
	    if sample_id in f:
	        filesl[filesl.index(f)]=filename
		found=True
          if not found:
	    filesl+=[filename]
    print "files:",len(filesl)
    for filename in filesl:
        filelist.write("root://eoscms//eos/cms"+filename+"\n")
    filelist.close()

    os.system("heavy_tagged_dijet_analysis fileList_"+shortsample+".txt "+shortsample+".root &> "+shortsample+".log &")
    