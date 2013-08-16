# Secondary vertex analysis for top mass measurement

# INSTALLATION
#
cvs co -d UserCode/TopMass  UserCode/CMG/UserArea/TopMass
cd UserCode/TopMass
scram b 


# For V5_13_0 cmgtools framework
https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsReleasesExperimental#Colin_537_patch4_V5_13_0


# For kinematic fit
cvs co -r V06-08-00 AnalysisDataFormats/TopObjects
cvs co -r V00-00-05 DataFormats/WrappedStdDictionaries
cvs co -r V06-08-00 TopQuarkAnalysis/TopEventProducers
cvs co -r V06-08-00 TopQuarkAnalysis/TopJetCombination
cvs co TopQuarkAnalysis/TopHitFit

cp /afs/cern.ch/user/p/palencia/public/hitFit/CMGMuonHitFitTranslator.cc               TopQuarkAnalysis/TopHitFit/src/
cp /afs/cern.ch/user/p/palencia/public/hitFit/CMGElectronHitFitTranslator.cc           TopQuarkAnalysis/TopHitFit/src/
cp /afs/cern.ch/user/p/palencia/public/hitFit/CMGMETHitFitTranslator.cc                TopQuarkAnalysis/TopHitFit/src/
cp /afs/cern.ch/user/p/palencia/public/hitFit/CMGJetHitFitTranslator.cc                TopQuarkAnalysis/TopHitFit/src/
cp /afs/cern.ch/user/p/palencia/public/hitFit/TtSemiLepHitFitProducerCMG.h             TopQuarkAnalysis/TopHitFit/plugins/
cp /afs/cern.ch/user/p/palencia/public/hitFit/SealModuleCMG.cc                         TopQuarkAnalysis/TopHitFit/plugins/
cp /afs/cern.ch/user/p/palencia/public/hitFit/BuildFile.xml                            TopQuarkAnalysis/TopHitFit/plugins/
cp /afs/cern.ch/user/p/palencia/public/hitFit/TtSemiLepHitFitProducer_Muons_cfi.py     TopQuarkAnalysis/TopHitFit/python/
cp /afs/cern.ch/user/p/palencia/public/hitFit/TtSemiLepHitFitProducer_Electrons_cfi.py TopQuarkAnalysis/TopHitFit/python/

scram b 

#CALIBRATION
python scripts/bsubCalib.py -o ~/www/TOP-12-030/ee/ -n 10000 -c ee
python scripts/massfitter2.py calib -o ~/www/TOP-12-030/ee -i FitSecVtxDistributionsWS.root
python scripts/massfitter2.py fit -o ~/www/TOP-12-030/ee -i FitSecVtxDistributionsWS.root


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

#
# SKIMMING cfg FILES
#
The skimming config files are in /test. The latest version is: 
skimLeptonJets.py

It has some command line functionalities which can be changed; i.e.
- sampleLocation: the CMG style sample location, default is
'/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'

- the sampleName: which is the variable name characteizing the sample;
  default is ttbar

- the maxEvents variable; default is '-1'(!)

In order to run it do the following:
cmsRun skimLeptonJets.py sampleLocation=<SAMPLELOCATION>
sampleName=<SAMPLENAME> maxEvents=1234

