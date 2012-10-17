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