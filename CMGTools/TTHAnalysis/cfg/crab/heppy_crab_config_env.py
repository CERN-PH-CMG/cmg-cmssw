# here we set all crab options that are not fixed
# values we'll be taken from environment variables set in launchall.py
# fixed options will be taken from heppy_crab_config.py

import imp, os
file = open( "heppy_crab_config.py", 'r' )
cfg = imp.load_source( 'cfg', "heppy_crab_config.py", file)
config = cfg.config

print "Will send dataset", os.environ["DATASET"], "with", os.environ["NJOBS"], "jobs"

config.General.requestName = os.environ["DATASET"] + "_" + os.environ["CMG_VERSION"] # task name
config.General.workArea = 'crab_' + os.environ["DATASET"] # crab dir name

# this will divide task in *exactly* NJOBS jobs (for this we need JobType.pluginName = 'PrivateMC' and Data.splitting = 'EventBased')
config.Data.unitsPerJob = 10
config.Data.totalUnits = config.Data.unitsPerJob * int(os.environ["NJOBS"])

config.JobType.inputFiles.append(os.environ["CFG_FILE"])
# arguments to pass to scriptExe. They have to be like "arg=value". 
config.JobType.scriptArgs = ["dataset="+os.environ["DATASET"], "total="+os.environ["NJOBS"], "useAAA="+os.environ["USEAAA"], "cfgfile="+os.environ["CFG_FILE"].split('/')[-1]]
try: config.JobType.inputFiles.extend(os.environ["FILESTOSHIP"].split(','))
except KeyError: pass
try:
    config.JobType.outputFiles.extend([x.replace("/","_") for x in os.environ["FILESTOUNPACK"].split(',')])
    config.JobType.scriptArgs.append("filestounpack="+os.environ["FILESTOUNPACK"])
except KeyError: pass
if os.environ["ONLYUNPACKED"]!="True": config.JobType.outputFiles.append("heppyOutput.tgz")

#final output: /store/user/$USER/output_dir/cmg_version/production_label/dataset/$date_$time/0000/foo.bar
config.Data.outLFNDirBase += '/' + os.environ["STAGEOUTREMDIR"] + '/' + os.environ["CMG_VERSION"]
config.Data.primaryDataset =  os.environ["PROD_LABEL"]
config.Data.publishDataName = os.environ["DATASET"]
config.Data.ignoreLocality = (os.environ["useAAA"]!="local") # "full" or "eos"

config.Site.storageSite = os.environ["OUTSITE"]

try: config.Site.whitelist = os.environ["WHITESITES"].split(',')
except KeyError: pass

try: config.JobType.scriptArgs += ["nevents="+os.environ["MAXNUMEVENTS"]]
except KeyError: pass
