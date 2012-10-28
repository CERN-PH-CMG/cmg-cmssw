import os
import subprocess

samples = [
#('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START50_V15-v1/AODSIM',True,False, 'grid'),
#('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/WWtoAnything_ptmin500_TuneZ2Star_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/QCD_Pt-15to3000_Tune4C_Flat_8TeV_pythia8/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM',True,False, 'grid'),
#('/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False, 'grid'),
#('/HT/Run2012A-PromptReco-v1/RECO',False,False, 'grid'),
#('/JetHT/Run2012B-PromptReco-v1/AOD',False,False, 'grid'),
#('/HT/Run2012A-13Jul2012-v1/AOD',False,False, 'ProductionTask'),
#('/JetHT/Run2012B-13Jul2012-v1/AOD',False,False, 'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v1/AOD',False,False, 'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v2/AOD',False,False, 'ProductionTask'),
('/RadionToHHTo4B_1TeV',True,True, 'cmsBatch'),
('/RadionToHHTo4B_1p5TeV',True,True, 'cmsBatch'),
('/RadionToHHTo4B_2TeV',True,True, 'cmsBatch'),
#('/yxin_RSG_WW_1000_pythia6_01/yxin-yxin_RSG_WW_1000_pythia6_01-52e9c298e8547223f910bab8db11615e/USER',True,True, 'grid'),
#('/yxin_RSG_WW_2000_pythia6_01/yxin-yxin_RSG_WW_2000_pythia6_01-54dbdee3e49fbf0c9fb4ed9452c44bd3/USER',True,True, 'grid'),
#('/QCD_Pt-15to3000_Tune4C_Flat_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12_DR53X-PU_S8_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/RSGravitonToWW_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/RSGravitonToWW_kMpl01_M-1500_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/RSGravitonToZZ_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/RSGravitonToZZ_kMpl01_M-1500_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/RSGravitonToWW_kMpl02_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/RSGravitonToWW_kMpl04_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False,'grid'),
#('/HT/Run2012A-13Jul2012-v1/AOD',False,False,'ProductionTask'),
#('/JetHT/Run2012B-13Jul2012-v1/AOD',False,False,'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v1/AOD',False,False,'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v2/AOD',False,False,'ProductionTask'),
]

for sample, mc, fastsim, production in samples:
    print sample, "mc", mc, "fastsim", fastsim
    #if mc:
    #	shortsample=sample.strip("/").split("/")[0]+"_grid"
    #else:
    shortsample="_".join(sample.strip("/").split("/"))+"_grid"
    filelist=open("fileList_"+shortsample+".txt","w")
    if production=="ProductionTask":
      command="cmsLs -l /store/cmst3/user/hinzmann/CMG"+sample+"/PAT_CMG_V5_6_0_B/NTUPLE_Oct25"+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/CMG"+sample+"/PAT_CMG_V5_6_0_B/NTUPLE_Oct25"
    elif production=="cmsBatch":
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE"+sample+""+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE"+sample+""
    else:
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE/"+shortsample+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE/"+shortsample
    print command
    p=subprocess.Popen([command],shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    files=p.stdout
    filesl=[]
    for line in files:
      if "ntuple" in line:
        filename=line.split(" ")[-1].strip()
        if filename.endswith(".root"):
	  sample_id="_".join(filename.split("/")[-1].strip(".root").split("_")[0:2])
	  print 'edmFileUtil',"root://eoscms//eos/cms"+filename
	  fileOk=True
          stdout = subprocess.Popen(['edmFileUtil',"root://eoscms//eos/cms"+filename], stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()[0]
          for error in ["Fatal Root Error","Could not open file","Tree Events appears to be missing. Not a valid collection"]:
              if error in stdout:
	          print stdout
	          fileOk=False
	  if not fileOk:
	      continue
	  found=False
	  for f in filesl:
	    if sample_id+"." in f or sample_id+"_" in f:
	      filesl[filesl.index(f)]=filename
	      found=True
	  if not found:
	    filesl+=[filename]
    print "files:",len(filesl)
    for filename in filesl:
        filelist.write("root://eoscms//eos/cms"+filename+"\n")
    filelist.close()

    os.system("heavy_tagged_dijet_analysis fileList_"+shortsample+".txt "+shortsample+".root &> "+shortsample+".log &")

    continue
    # make local copy
    filelist=open("fileList_"+shortsample+"_local.txt","w")
    command="mkdir /tmp/hinzmann/"+shortsample
    print command
    os.system(command)
    for filename in filesl:
      command="cmsStage "+filename+" /tmp/hinzmann/"+shortsample
      print command
      os.system(command)
    for filename in filesl:
        filelist.write("/tmp/hinzmann/"+shortsample+"/"+filename.split("/")[-1]+"\n")
    filelist.close()
