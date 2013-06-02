#
# installation
#

export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_9
cd CMSSW_5_3_9/src/
cvs co -p UserCode/CMG/UserArea/EWKV/TAGS.txt | sh

#create a local pat-tuple
runOverSamples.py -j data/vbfz_samples.json -R "tmp>5000" -n 1 -d aoddir -p "-castor=/afs/cern.ch/user/p/psilva/work/ntuples -cfg=$CMSSW_BASE/src/UserCode/EWKV/test/runDataAnalyzer_mc_cfg.py" -t lljj -s 8nh

runLocalAnalysisOverSamples.py -e runTopAnalysis -j data/top_samples.json -d /store/cmst3/user/psilva/539_ntuples -o ~/work/top_539/ -c test/runAnalysis_cfg.py.templ -s 8nh
runLocalAnalysisOverSamples.py -e runTopAnalysis -j data/top_syst_samples.json -d /store/cmst3/user/psilva/539_ntuples -o ~/work/top_539/ -c test/runAnalysis_cfg.py.templ -s 8nh
runPlotter --iLumi 19683 --inDir ~/work/top_539/ --json data/top_samples.json --outFile ~/work/top_539/plotter_raw.root --noPlot
runPlotter --iLumi 19683 --inDir ~/work/top_539/ --json data/top_syst_samples.json --outFile ~/work/top_539/plotter_syst.root --noPlot


runPlotter --iLumi 19683 --inDir ~/work/top_539/ --outDir ~/work/top_539/plots --json data/top_samples.json --outFile ~/work/top_539/plotter.root --noLog  --showUnc  --plotExt .pdf



####
EWK Z+2jets
####
runLocalAnalysisOverSamples.py -e runVBFZAnalysis -j data/vbfz_samples.json -d /store/cmst3/user/psilva/539_ntuples -o ~/work/ewkzp2j_539 -c test/runAnalysis_cfg.py.templ -p "@runSystematics=False @useMVA=False" -s 8nh



