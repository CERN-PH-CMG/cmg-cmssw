import imp, os

# datasets to run as defined from run_susyMT2.cfg
# right now configured for maximal job splitting
# in principle one only needs to modify the following two lines:
production_label = "fullProd_test3"
cmg_version = 'MT2_CMGTools-from-CMSSW_7_2_3'

handle = open("heppy_config.py", 'r')
cfo = imp.load_source("heppy_config", "heppy_config.py", handle)
conf = cfo.config
handle.close()

os.system("scramv1 runtime -sh")
os.system("source /cvmfs/cms.cern.ch/crab3/crab.sh")

os.environ["PROD_LABEL"] = production_label
os.environ["CMG_VERSION"] = cmg_version

for comp in conf.components:
    #set maximal splitting
    NJOBS = len(comp.files)
    os.environ["NJOBS"] = str(NJOBS)
    os.environ["DATASET"] = str(comp.name)
    os.system("crab submit -c heppy_crab_config_env.py")

os.system("rm -f python.tar.gz")
os.system("rm -f cmgdataset.tar.gz")
os.system("rm -f cafpython.tar.gz")
