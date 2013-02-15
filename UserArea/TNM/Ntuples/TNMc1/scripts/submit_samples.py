import os

samples = [
#('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START50_V15-v1/AODSIM',True,False),
#('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/WWtoAnything_ptmin500_TuneZ2Star_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/QCD_Pt-15to3000_Tune4C_Flat_8TeV_pythia8/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM',True,False),
#('/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM',True,False),
#('/HT/Run2012A-PromptReco-v1/RECO',False,False),
#('/JetHT/Run2012B-PromptReco-v1/AOD',False,False),
#('/yxin_RSG_WW_1000_pythia6_01/yxin-yxin_RSG_WW_1000_pythia6_01-52e9c298e8547223f910bab8db11615e/USER',True,True),
#('/yxin_RSG_WW_2000_pythia6_01/yxin-yxin_RSG_WW_2000_pythia6_01-54dbdee3e49fbf0c9fb4ed9452c44bd3/USER',True,True),

#('/RadionToHHTo4B_1TeV',True,True),
#('/RadionToHHTo4B_1p5TeV',True,True),
#('/RadionToHHTo4B_2TeV',True,True),
#('/JetHT/Run2012C-PromptReco-v1/AOD',False,False),
#('/JetHT/Run2012C-PromptReco-v2/AOD',False,False),
#('/HT/Run2012A-13Jul2012-v1/AOD',False,False),
#('/JetHT/Run2012B-13Jul2012-v1/AOD',False,False),
('/QCD_Pt-15to3000_Tune4C_Flat_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
('/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
#('/RSGravitonToWW_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
#('/RSGravitonToWW_kMpl01_M-1500_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
#('/RSGravitonToZZ_kMpl01_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
#('/RSGravitonToZZ_kMpl01_M-1500_Tune23_8TeV-herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
#('/RSGravitonToWW_kMpl02_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
#('/RSGravitonToWW_kMpl04_M-1500_TuneZ2star_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',True,False),
]

for sample, mc, fastsim in samples:
    print sample, "mc", mc, "fastsim", fastsim
    shortsample="_".join(sample.strip("/").split("/"))+"_grid"
    crabcfg=open("crab.cfg","w")
    crabcfg.writelines("""
[CRAB]
#
#   This section contains the default values for general parameters.
#   They can be set also as a command-line option, i.e.
#
#   key1 = value1
#   [SECTION]
#   key2 = value2
#
#   in this section corresponds to
#
#   crab.py -key1=value1 -key2=SECTION.value2
#
#   in the command line.
#
jobtype = cmssw

### test cvs
### Specify the scheduler to be used.
### Supported schedulers are : [ glite, glidein, condor_g, caf, condor, arc, sge, pbs, lsf ]
scheduler = glite
#scheduler = lsf

### To configure CRAB as client. Let CRAB select one of the available servers for you
### CRAB will submit jobs to an available server, which will submit and manage for the users
use_server = 0

### Only to debug propose, configure CRAB  as client and set the name of the server
### (e.g. pi, lnl etc etc )
### CRAB will submit jobs to the server, which will submit and manage for the users
#server_name = cern_vocms20

[LSF]
queue=2nd

[CMSSW]

### The data you want to access (to be found on DBS)
### /primarydataset/processeddataset/datatier[/OptionalADS]
""")
    crabcfg.writelines("datasetpath = "+sample)
    crabcfg.writelines("""

### Within a dataset you can ask to run over the related parent files too.
### E.g., this will give you access to the RAW data while running over a RECO sample.
### Setting use_parent=True CRAB determines the parent files from DBS and will add
### secondaryFileNames = cms.untracked.vstring( <LIST of parent FIles> ) to the pool source section of your parameter set.
#use_parent = True

### To run CRAB for private events production set datasetPath= None
#datasetpath=none

### To select a single (list of) run within a single processed dataset define run number (list)
### selection can be a comma-separated list of run numbers and run number ranges: 1,2,3-4
#runselection=1,2,3-4


### Specifies a comma separated list of seeds to increment from job to job. The initial value is taken from the CMSSW config file.
### increment_seeds=sourceSeed,g4SimHits will set sourceSeed=11,12,13 and g4SimHits=21,22,23 on subsequent jobs if the values of
### the two seeds are 10 and 20 in the CMSSW config file.
### See also preserve_seeds. Seeds not listed in increment_seeds or preserve_seeds are randomly set for each job.
#increment_seeds=sourceSeed,g4SimHits

### Specifies a comma separated list of seeds to which CRAB will not change from their values in the user CMSSW config file.
### preserve_seeds=sourceSeed,g4SimHits will leave the Pythia and GEANT seeds the same for every job.
### See also increment_seeds. Seeds not listed in increment_seeds or preserve_seeds are randomly set for each job.
#preserve_seeds=sourceSeed,g4SimHits

### Relevant only for Monte Carlo production, defaults to 1. The first job will generate events with this run number, subsequent jobs will
### increment the run number. Failing to set this number means CMSSW will not be able to read multiple such files as they
### will all have the same run and event numbers. This check in CMSSW can be bypassed by setting
### process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck') in the input source, should you need to
### read files produced without setting first_run.

#first_run=1

### Name of the generator your MC job is using. Some generators require CRAB to skip events, others do not.
### Possible values are pythia (default), comphep, lhe, and madgraph. This will skip events in your generator input file.
#generator=pythia

### Force CRAB to skip the inclusion of file produced by TFileService to list of output files.
### Default is 0, namely the file is included.
# skip_TFileService_output = 1

### Force CRAB to add the EDM output file, as defined in PSET in PoolOutputModule (if any)
### to be added to the list of output files. Default is 0 (== no inclusion)
# get_edm_output = 1


### To use a local DBS istance specify the related URL here.
""")
    if fastsim:
        crabcfg.writelines("dbs_url = http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet")
    crabcfg.writelines("""

### The name of ParameterSet to be used
pset=cfg.py

### Parameters to be passed to the python config file, see
### https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#Passing_Command_Line_Arguments_T
#pycfg_params = option1=one option2=two

### A JSON file that describes which runs and lumis to process. CRAB will skip luminosity blocks not
### listed in the file. When using this setting, you must also use lumi-based splitting rather than
### event based splitting as shown below.
#lumi_mask = some_mask_file.json

### Splitting parameters:
### Total number of events to be accessed: -1 means all
### NOTE: "-1" is not usable if no input

### Number of events to be processed per job
""")
    if mc:
        crabcfg.writelines("""total_number_of_events=-1
events_per_job = 30000""")
    else:
        crabcfg.writelines("""total_number_of_lumis=-1
lumis_per_job = 30
lumi_mask = /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-202305_8TeV_PromptReco_Collisions12_JSON.txt
""")
    crabcfg.writelines("""

### Number of jobs to be created per task
#number_of_jobs = 5

### Analogous to events, lumis can be used to split up datasets
#total_number_of_lumis=1000
#lumis_per_job = 100


### The output files produced by your application (comma separated list)
output_file = ntuple.root

### Dataset of PU to import in the local DBS for data publication
#dataset_pu = /pileup/dataset/toimport

# This setting is for experts only. See crab -help
# split_by_event = 1

[USER]

### If CRAB.server_mode = 1
### To set Your e-mail address to be used by the server to notify you
#eMail = your_email_address

### If CRAB.server_mode = 1
### To specify the percentage of finished job in a task, corresponding
### to when the notification email will be sent to you by the server default is 100%
#thresholdLevel = 100

### To use a specific name of UI directory where CRAB will create job to submit (with full path).
### the default directory will be "crab_0_data_time"
""")
    crabcfg.writelines("ui_working_dir = crab_"+shortsample)
    crabcfg.writelines("""

### OUTPUT file management ###
### To have back the job executable output into UI set return_data= 1
return_data = 0

### To specify the UI directory where to store the CMS executable output
### FULL path is mandatory. Default is  <ui_working_dir>/res will be used.
#outputdir= /full/path/yourOutDir

### To specify the UI directory where to store the stderr, stdout and .BrokerInfo of submitted jobs
### FULL path is mandatory. Default is <ui_working_dir>/res will be used.
#logdir= /full/path/yourLogDir


### To copy the CMS executable output into a SE set copy_data = 1
copy_data = 1

### if you want to copy data in a "official CMS site" (PhEDEx node)
### you have to specify the name as written in
storage_element = srm-eoscms.cern.ch
### the user_remote_dir will be created under the SE mountpoint
### in the case of publication this directory is not considered
""")
    crabcfg.writelines("user_remote_dir = /store/cmst3/user/hinzmann/NTUPLE/"+shortsample)
    crabcfg.writelines("""

### if you want to copy your data at CAF
#storage_element = T2_CH_CAF
### the user_remote_dir will be created under the SE mountpoint
### in the case of publication this directory is not considered
#user_remote_dir = name_directory_you_want

### at caf, you can decide in which mountpoint to copy your output, selecting the first part of LFN
### default value is /store/caf/user
### to test eos area you can use 
#caf_lfn = /store/eos/user

### if you want to copy your data to your area in castor at cern
### or in a "not official CMS site" you have to specify the complete name of SE
#storage_element=srm-cms.cern.ch
### this directory is the mountpoin of SE
storage_path=/srm/v2/server?SFN=/eos/cms
### directory or tree of directory under the mounpoint
#user_remote_dir = name_directory_you_want

################ DEPRECATED WITH EOS ########################
### if you are using CAF scheduler, you can specify the pool
### where to write your output. The default is cmscafuser
#storage_pool = cmscafuser
############################################################

### This is the storage port number. Default is 8443
#storage_port = N

### To use the local stage out (local to the closeSE) in case of remote stage out failure.
### The publication of data is not supported with local_stage_out = 1. Work in progress
local_stage_out= 0

### To publish produced output in a local istance of DBS set publish_data = 1
publish_data=0

### Specify the dataset name. The full path will be <primarydataset>/<publish_data_name>/USER
#publish_data_name = yourDataName
### Specify the URL of DBS istance where CRAB has to publish the output files
#dbs_url_for_publication = http://cmssrv17.fnal.gov:8989/DBS108LOC1/servlet/DBSServlet


### To specify additional files to be put in InputSandBox
### write the full path  if the files are not in the current directory
### (wildcard * are allowed): comma separated list
#additional_input_files = file1, file2, /full/path/file3

### A user script that will be run on WN (instead of default cmsrun). It is up to the user to setup properly the script itself to
### run on WN enviroment. CRAB guarantees that the CMSSW environment is setup (e.g. scram is in the path) and that the modified
### pset.py will be placed in the working directory, with name CMSSW.py . The user must ensure that a job report named
### crab_fjr.xml will be written. This can be guaranteed by passing the arguments "-j crab_fjr.xml" to cmsRun in the script. The
### script itself will be added automatically to the input sandbox so user MUST NOT add it within the USER.additional_input_files.
#script_exe=file_exe

### To switch from status print on screen to DB serialization to a file specify here the destination files.
### CRAB will create it on CRAB_Working_Dir/share
#xml_report=name_you_want

### To use the automate namespace definition (perfomed by CRAB). Default is 0
### The same policy used for the stage out in case of data publication will be applied.
#usenamespace=1

### To enable the higer verbose level on wrapper specify debug_wrapper = 1.
### The Pset contents before and after the CRAB manipulation will be written
### together with other useful infos.
#debug_wrapper = 1

### Set it to 1 to skip the check of free space left on your working directory
### before attempting to get the output back. Default is 0 (=False)
#dontCheckSpaceLeft = 1

### This is the value of the activity reported in the dashboard (release: client 2_7_8 and server 1_1_4).
### Default value is analysis
#tasktype = what_you_want  

[GRID]

### Only with debug propose: to change the CMS-WMS to be used.
### By default CRAB takes the needed configuration files from the cmsdoc page
#rb = CERN

### To specify  VOMS role and/or group
#role = superman
#group = superheros

### To skip the CRAB check of your proxy
#dont_check_proxy = 1

### To add other requirements to jdl file, as example the Operating System
#requirements = (other.GlueHostOperatingSystemName == "RedHat")

### To add other parameters to jdl file: semicolon separated list;
#additional_jdl_parameters = AllowZippedISB = false

### To use a specific WMS end point put here the right name:
#wms_service=

### To specify a cpu time and wall_clock_time(=real time) in minutes
#max_cpu_time = 60
#max_wall_clock_time = 60

### To manage White/Black lists
### Use the dns domain (eg fnal, cern, ifae, fzk, cnaf, lnl,....) or the CMSSite name
### T1_US_FNAL. Only Tier2 centers have resources dedicated to user analysis.
### All the storage/computing elements (SE/CE) contained in the strings (comma separated list)
### will/will not be considered for submission.
### SE Black List:
se_black_list = T0,T1,T3,T2_BE_UCL
### SE White List
#se_white_list = cern

### CE Black List:
#ce_black_list = infn
### CE White List:
#ce_white_list = cern

## fields written into jdl
virtual_organization = cms

### Temporary useful parameter to allow the WMSAuthorisation handling. Specifying skipwmsauth = 1
### the pyopenssl problems will disappear. It is needed working on gLite UI outside of CERN.
#skipwmsauth=1


[CONDORG]

# Set this to condor to override the batchsystem defined in gridcat.
#batchsystem = condor

# Specify addition condor_g requirments
# use this requirment to run on a cms dedicated hardare
# globus_rsl = (condor_submit=(requirements 'ClusterName == \"CMS\" && (Arch == \"INTEL\" || Arch == \"X86_64\")'))
# use this requirement to run on the new hardware
#globus_rsl = (condor_submit=(requirements 'regexp(\"cms-*\",Machine)'))
""")
    crabcfg.close()
    
    
    cfg=open("cfg.py","w")
    cfg.writelines("""
import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X
# if not cmsswIs44X():
#    raise ValueError('Sorry, you are not working in 44X. use the correct cfg')

sep_line = '-'*67
print sep_line
print 'CMG PAT-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("PAT")


print 'querying database for source files'


""")
    cfg.writelines("runOnMC = "+str(mc))
    if "RadionToHHTo4B_1TeV" in sample:
        cfg.writelines("""
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_10_1_Mvy.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_11_1_voa.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_1_1_jar.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_12_1_HEX.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_13_1_yvO.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_14_1_tFW.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_15_1_3Ji.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_16_1_zCN.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_17_1_Vzw.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_18_1_YLy.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_19_1_iF5.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_20_1_yaa.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_2_1_7S9.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_3_1_oFF.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_4_1_LMz.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_5_1_1eu.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_6_1_6tt.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_7_1_d4t.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_8_1_bhk.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1TeV_9_1_HYK.root',
    )
)""")
    elif "RadionToHHTo4B_1p5TeV" in sample:
        cfg.writelines("""
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_10_1_oRN.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_1_1_wjd.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_2_1_Ki6.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_3_1_pK4.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_4_1_6JK.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_5_1_HfD.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_6_1_S3c.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_7_1_Z4v.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_8_1_izE.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_1500GeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_1500GeV_9_1_vav.root ',
    )
)""")
    elif "RadionToHHTo4B_2TeV" in sample:
        cfg.writelines("""
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_10_1_hbQ.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_11_1_y7o.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_1_1_K3X.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_12_1_Bik.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_13_1_YJe.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_14_1_ZjZ.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_15_1_OdQ.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_16_1_Kmo.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_17_1_ATD.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_18_1_g4b.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_19_1_s5w.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_20_1_qyp.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_2_1_qJI.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_3_1_f6F.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_4_1_Pok.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_5_1_S53.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_6_1_eyc.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_7_1_pUD.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_8_1_pJh.root',
'root://xrootd.unl.edu//store/user/sertac/2012/Radion/MR_2TeV/MadGraph_PYTHIA6_FastSim53X_RadionToHH_Hbb_2TeV_9_1_6kk.root',
    )
)""")
    else:
        cfg.writelines("""
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/data/Run2012C/JetHT/AOD/PromptReco-v1/000/198/044/9852196C-E6C5-E111-AA60-003048F01E88.root'
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_10_1_fkb.root',
    )

#from CMGTools.Production.datasetToSource import *
#process.source = datasetToSource(
#   'CMS',
#   '/JetHT/Run2012C-PromptReco-v1/AOD'
#   )
)""")

    cfg.writelines("""
print sep_line
print process.source.fileNames
print sep_line 

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

if runOnMC is False:
    # removing MC stuff
    print 'removing MC stuff, as we are running on Data'

    process.patElectrons.addGenMatch = False
    process.makePatElectrons.remove( process.electronMatch )
    
    process.patMuons.addGenMatch = False
    process.makePatMuons.remove( process.muonMatch )
    
    process.PATCMGSequence.remove( process.PATCMGGenSequence )
    process.PATCMGJetSequence.remove( process.jetMCSequence )
    process.PATCMGJetSequence.remove( process.patJetFlavourId )
    process.patJets.addGenJetMatch = False
    process.patJets.addGenPartonMatch = False

    if cmsswIs52X():
        process.PATCMGJetSequenceCHSpruned.remove( process.jetMCSequenceCHSpruned )
        process.patJetsCHSpruned.addGenJetMatch = False
        process.patJetsCHSpruned.addGenPartonMatch = False

    process.PATCMGTauSequence.remove( process.tauGenJets )
    process.PATCMGTauSequence.remove( process.tauGenJetsSelectorAllHadrons )
    process.PATCMGTauSequence.remove( process.tauGenJetMatch )
    process.PATCMGTauSequence.remove( process.tauMatch )
    process.patTaus.addGenJetMatch = False
    process.patTaus.addGenMatch = False

    process.patMETs.addGenMET = False 
    process.patMETsRaw.addGenMET = False 

    # setting up JSON file
    # json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt'
    # print 'using json file: ', json
    # from CMGTools.Common.Tools.applyJSON_cff import *
    # applyJSON(process, json )

    # adding L2L3Residual corrections
    process.patJetCorrFactors.levels.append('L2L3Residual')
    if cmsswIs52X():
        process.patJetCorrFactorsCHSpruned.levels.append('L2L3Residual')


print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.PATCMGJetCHSSequence = cloneProcessingSnippet(process, process.PATCMGJetSequence, 'CHS')
process.PATCMGJetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.PATCMGJetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.PATCMGJetCHSSequence, 'particleFlow', 'pfNoPileUp')
process.patJetCorrFactorsCHS.payload = 'AK5PFchs'

########################################################
## Path definition
########################################################

process.dump = cms.EDAnalyzer('EventContentAnalyzer')

process.load('CMGTools.Common.PAT.addFilterPaths_cff')
process.p = cms.Path(
    process.prePathCounter + 
    process.PATCMGSequence + 
    process.PATCMGJetCHSSequence 
    )

process.p += process.postPathCounter

# For testing, you can remove some of the objects:
# NOTE: there are a few dependencies between these sequences
# process.PATCMGSequence.remove(process.PATCMGPileUpSubtractionSequence)
# process.PATCMGSequence.remove(process.PATCMGRhoSequence)
# process.PATCMGSequence.remove(process.PATCMGMuonSequence)
# process.PATCMGSequence.remove(process.PATCMGElectronSequence)
# process.PATCMGSequence.remove(process.PATCMGGenSequence)
# process.PATCMGSequence.remove(process.PATCMGJetSequence)
# process.PATCMGSequence.remove(process.PATCMGTauSequence)
# process.PATCMGSequence.remove(process.PATCMGMetSequence)
# process.p.remove(process.PATCMGJetCHSSequence)


########################################################
## Setup electron energy corrections
########################################################

if cmsswIs44X():
    from CMGTools.Common.Tools.setupGsfElectronCalibration import setupGsfElectronCalibration
    setupGsfElectronCalibration( process, runOnMC )


########################################################
## PAT output definition
########################################################

## Output Module Configuration (expects a path 'p')
from CMGTools.Common.eventContent.patEventContentCMG_cff import patEventContentCMG
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = patEventContentCMG
                               )
# needed to override the CMG format, which drops the pat taus
process.out.outputCommands.append('keep patTaus_selectedPatTaus_*_*')

#FIXME now keeping the whole event content...
# process.out.outputCommands.append('keep *_*_*_*')

#process.outpath = cms.EndPath(process.out)

########################################################
## CMG output definition
########################################################

from CMGTools.Common.eventContent.patEventContentCMG_cff import everything
process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

#process.outpath += process.outcmg




########################################################
## Below, stuff that you probably don't want to modify
########################################################



## Geometry and Detector Conditions (needed for a few patTuple production steps)

from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
process.schedule = getSchedule(process, runOnMC, False)
#process.schedule.append( process.outpath )

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

print sep_line

print 'Fastjet instances (dominating our processing time...):'
from CMGTools.Common.Tools.visitorUtils import SeqVisitor
v = SeqVisitor('FastjetJetProducer')
process.p.visit(v)

print sep_line

print 'starting CMSSW'


#### TNMc1 part

process.MessageLogger.cerr.default.limit = 5

# This is required in order to configure HLTConfigProducer
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

# set up JSON ---------------------------------------------------------------

if runOnMC==False:
    from CMGTools.Common.Tools.applyJSON_cff import *
    #json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON.txt'
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-202305_8TeV_PromptReco_Collisions12_JSON.txt'
    print 'json:', json
    applyJSON(process, json )

process.load("Ntuples.TNMc1.ntuple_cfi")

from CMGTools.External.pujetidsequence_cff import *

process.selectedPatJetspuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.selectedPatJetsCHSpuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJetsCHS"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.ak5PFJetsCHSprunedSubJetspuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("ak5PFJetsCHSpruned:SubJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

#process.p = cms.Path(process.selectedPatJetspuJetId * process.selectedPatJetsCHSpuJetId * process.ak5PFJetsCHSprunedSubJetspuJetId * process.demo)

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

GT = None
if runOnMC:
    GT = 'START53_V10::All' # for Summer12 MC
else:
    GT = 'GR_P_V41_AN2::All' # for run2012C
process.GlobalTag.globaltag = GT

##### AK7 jets

process.load('CMGTools.Common.PAT.PATCMG_cff')
process.load("CMGTools.Common.PAT.jetSubstructure_cff")
if not runOnMC:
    process.PATCMGJetSequenceAK7CHSpruned.remove( process.jetMCSequenceAK7CHSpruned )
    process.patJetsAK7CHSpruned.addGenJetMatch = False
    process.patJetsAK7CHSpruned.addGenPartonMatch = False
    process.patJetCorrFactorsAK7CHSpruned.levels.append('L2L3Residual')

process.load("Ntuples.TNMc1.PAT_ak7jets_cff")
if not runOnMC:
    process.PATCMGJetSequenceAK7CHS.remove( process.jetMCSequenceAK7CHS )
    process.patJetsAK7CHS.addGenJetMatch = False
    process.patJetsAK7CHS.addGenPartonMatch = False
    process.patJetCorrFactorsAK7CHS.levels.append('L2L3Residual')

##### Razor stuff

process.load("CMGTools.Susy.RazorMultiJet.razorMultijet_cff")
process.load("CMGTools.Susy.common.susy_cff")

process.razorMJObjectSequence.remove(process.razorMJHemiSequence)
# This is the UCSB tau veto.  Need to remove for SMSs:
process.razorMJTauSequence.remove(process.razorMJTauVeto)

if not runOnMC:
    process.demo.buffers.remove('sint')
    process.demo.buffers.remove('recoLeafCandidate')

if runOnMC:
    process.demo.buffers.remove('hcalFilter')
    process.demo.buffers.remove('edmTriggerResultsHelper')
    process.demo.buffers.remove('edmTriggerResultsHelper1')

##### Sequence

print 'Global tag       : ', process.GlobalTag.globaltag

if runOnMC==True:
    #process.p += cms.Sequence(process.PATCMGJetSequenceAK7CHS+process.PATCMGJetSequenceAK7CHSpruned+process.demo)
    process.p += cms.Sequence(process.razorMJObjectSequence+process.susyGenSequence+process.PATCMGJetSequenceAK7CHS+process.PATCMGJetSequenceAK7CHSpruned+process.demo)
else:
    #process.p += cms.Sequence(process.PATCMGJetSequenceAK7CHS+process.PATCMGJetSequenceAK7CHSpruned+process.demo)
    process.p += cms.Sequence(process.razorMJObjectSequence+process.PATCMGJetSequenceAK7CHS+process.PATCMGJetSequenceAK7CHSpruned+process.demo)

##### HCAL laser filter for 2012

if not runOnMC:
    process.load("Ntuples.TNMc1.hcallasereventfilter2012_cfi")
    #inputfilelist=["data/AllBadHCALLaser.txt"]
    #for f in inputfilelist:
    #    mylist=open(f,'r').readlines()
    #     for j in mylist:
    #         process.hcallasereventfilter2012.EventList.append(j.strip())
    print "load laser event list"
    from Ntuples.TNMc1.AllBadHCALLaser import eventlist
    process.hcallasereventfilter2012.EventList=eventlist
    process.hcallasereventfilter2012Path=cms.Path(process.hcallasereventfilter2012)
    process.schedule = cms.Schedule(process.hcallasereventfilter2012Path,process.p)

### Tweaks to run PAT and ntuple in one go

process.demo.edmTriggerResultsHelper1[0]="edmTriggerResultsHelper          TriggerResults               1"

### Tweaks to run on the grid

process.p.remove(process.vertexWeightSequence)

""")
    if fastsim:
        cfg.writelines("""
### Tweaks to run on fastsim samples

process.schedule.remove(process.metNoiseCleaningPath)
process.schedule.remove(process.hcalLaserEventFilterPath)
process.schedule.remove(process.CSCTightHaloFilterPath)
process.schedule.remove(process.HBHENoiseFilterPath)
process.schedule.remove(process.trackingFailureFilterPath)

#process.demo.buffers.remove('edmTriggerResultsHelper1')
#process.p.remove(process.PATCMGTauSequence)
#process.demo.buffers.remove('patTau')
#process.p.remove(process.PATCMGMetSequence)
#process.p.remove(process.MetSignificanceSequence)
#process.demo.buffers.remove('patMET')
#process.demo.buffers.remove('patMET1')
""")
    cfg.close()
    #os.system("crab -create")
    #for i in range(0,10):
    #   os.system("crab -c crab_"+shortsample+" -submit "+str(i*300+1)+"-"+str((i+1)*300))
    #os.system("crab -c crab_"+shortsample+" -status")
    #os.system("crab -c crab_"+shortsample+" -get")
    #os.system("crab -c crab_"+shortsample+" -forceResubmit all")
    #os.system("crab -c crab_"+shortsample+" -resubmit all")
    #os.system("crab -c crab_"+shortsample+" -kill all")
