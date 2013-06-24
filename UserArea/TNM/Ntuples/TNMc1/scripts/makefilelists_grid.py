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
#('/HT/Run2012A-recover-06Aug2012-v1/AOD',False,False, 'ProductionTask'),
#('/HT/Run2012A-13Jul2012-v1/AOD',False,False, 'ProductionTask'),
#('/JetHT/Run2012B-13Jul2012-v1/AOD',False,False, 'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v1/AOD',False,False, 'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v2/AOD',False,False, 'ProductionTask'),
#('/RadionToHHTo4B_1TeV',True,True, 'cmsBatch'),
#('/RadionToHHTo4B_1p5TeV',True,True, 'cmsBatch'),
#('/RadionToHHTo4B_2TeV',True,True, 'cmsBatch'),
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

#('/RSGravitonToWW_kMpl01_M-1000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-1000_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-1000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-1000_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-1500_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-1500_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-2000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-2000_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-2000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-2000_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-3000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToWW_kMpl01_M-3000_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-3000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RSGravitonToZZ_kMpl01_M-3000_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/WprimeToWZ_M-1000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/WprimeToWZ_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/WprimeToWZ_M-2000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/WprimeToWZ_M-3000_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RadionToHHTo4B_M-1000_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RadionToHHTo4B_M-1500_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RadionToHHTo4B_M-2000_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RadionToHHTo4B_M-2500_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/RadionToHHTo4B_M-3000_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/QCD_HT-100To250_TuneZ2star_8TeV-madgraph-pythia/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0/NTUPLE_Feb15',False,False,'ProductionTask'),
('/HT/Run2012A-22Jan2013-v1/AOD/CMGPF_V5_16_0/NTUPLE_Feb15',False,False,'ProductionTask'),
('/JetHT/Run2012B-22Jan2013-v1/AOD/CMGPF_V5_16_0/NTUPLE_Feb15',False,False,'ProductionTask'),
('/JetHT/Run2012C-22Jan2013-v1/AOD/CMGPF_V5_16_0/NTUPLE_Feb15',False,False,'ProductionTask'),
('/JetHT/Run2012D-22Jan2013-v1/AOD/CMGPF_V5_16_0/NTUPLE_Feb15',False,False,'ProductionTask'),

#('/QCD_Pt-15to3000_Tune4C_Flat_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/HT/Run2012A-recover-06Aug2012-v1/AOD/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/HT/Run2012A-13Jul2012-v1/AOD/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/JetHT/Run2012B-13Jul2012-v1/AOD/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/JetHT/Run2012C-24Aug2012-v2/AOD/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/JetHT/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/JetHT/Run2012D-PromptReco-v1/AOD/PAT_CMG_V5_13_0/NTUPLE_Feb15',False,False,'ProductionTask'),
#('/summer12_WJetsPt100',True,True, 'cmsBatch'),
#('/summer12_WJetsPt70To100',True,True, 'cmsBatch'),
#('/summer12_WJetsPt50To70',True,True, 'cmsBatch'),
#('/summer12_WW',True,True, 'cmsBatch'),
#('/summer12_TTBAR',True,True, 'cmsBatch'),
#('/summer12_RSG_WW_lvjj_c0p05_M1000',True,True, 'cmsBatch'),
#('/summer12_BulkG_WW_lvjj_c0p2_M1000',True,True, 'cmsBatch'),
#('/summer12_BulkG_WW_lvjj_c0p2_M1500',True,True, 'cmsBatch'),
#('/summer12_BulkG_WW_lvjj_c0p2_M2000',True,True, 'cmsBatch'),
#('/summer12_BulkG_WW_lvjj_c0p2_M2500',True,True, 'cmsBatch'),
#('/pythia8_qcd_m1500___Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_qcd_m2000___Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_qcd_m2500___Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_4000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_4000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_4000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_6000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_6000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_6000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_8000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_8000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_8000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_10000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_10000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_10000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_12000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_12000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_12000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_14000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_14000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_14000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_16000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_16000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_16000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_18000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_18000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_18000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m1500_20000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2000_20000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/pythia8_ci_m2500_20000_1_0_0_Nov15',True,True, 'cmsBatchFastsim'),
#('/RSGravitonToWW_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B/NTUPLE_Nov20',True,False, 'ProductionTask'),
#('/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B/NTUPLE_Nov20',True,False, 'ProductionTask'),
]

for sample, mc, fastsim, production in samples:
    print sample, "mc", mc, "fastsim", fastsim
    #if mc:
    #	shortsample=sample.strip("/").split("/")[0]+"_grid"
    #else:
    shortsample="_".join(sample.strip("/").split("/"))+"_grid"
    filelist=open("fileList_"+shortsample+".txt","w")
    if production=="ProductionTask":
      command="cmsLs -l /store/cmst3/user/hinzmann/CMG"+sample+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/CMG"+sample
    elif production=="cmsBatch":
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE"+sample+""+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE"+sample+""
    elif production=="cmsBatchFastsim":
      command="cmsLs -l /store/cmst3/user/hinzmann/fastsim"+sample+""+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/fastsim"+sample+""
    else:
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE/"+shortsample+"""/ | awk '{size+=$2}END{print size/1024/1024/1024" GB"}'"""
      os.system(command)
      command="cmsLs -l /store/cmst3/user/hinzmann/NTUPLE/"+shortsample
    print command
    p=subprocess.Popen([command],shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    files=p.stdout
    filesl=[]
    for line in files:
      if ".root" in line:
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
	  if not "Fastsim" in production:
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

    #os.system("heavy_tagged_dijet_analysis fileList_"+shortsample+".txt "+shortsample+".root &> "+shortsample+".log &")

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
