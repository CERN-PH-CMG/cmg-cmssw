# Secondary vertex analysis for top mass measurement

# INSTALLATION
#
cvs co -d UserCode/TopMass  UserCode/CMG/UserArea/TopMass
cd UserCode/TopMass
scram b 

# PLEASE UPDATE FROM THIS POINT FOWARD
# THESE INSTRUCTIONS WERE VALID FOR THE SUMMER STUDIES

#
# SETUP THE ENVIRONMENT (each time you login again)
#
cd ~/work/CMSSW_5_2_6/src
export SCRAM_ARCH=slc5_amd64_gcc462
cmsenv

#
# CREATE THE PAT-TUPLE
# use the cfg under test if you need to create a pat-Tuple for this analysis

  
#
# RUN THE ANALYSIS 
# for a given sample: TTJets, WJets, ZJets, QCD, etc. give the directory where the event files are stored as input
# the script will create a ROOT output file with histograms

SecVtxAnalysis inputDir=/store/cmst3/user/psilva/SecVtx/TTJets


#
# RUN PSEUDO-EXPERIMENTS
#


FitKinTemplates templ=data/histoPt.root npe=10 onlySignal=0



#
# SCRIPTS
#

There are scrips in /scripts, some using information stored in /python/topinfo.py.

*scripts/submitJobs.py
Submits  multiple jobs to the batch system based on the list of
samples defined in the dictionary in /python/topinfo.py.
It uses the cmsBatch.py command and the /test/skimLeptonJets.py config
file. 
e.g. from /test do:
./../scripts/submitJobs.py -n 25 -o ntuplesOut -r
/store/cmst3/user/USERNAME/ntuples
where "-n" defines the number of files per job, "-o" the local
outputdirectory and "-r" the remote output directory (see also
cmsBatch.py -h for details, they are the same.
Or do submitJobs.py -h
