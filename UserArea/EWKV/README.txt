#
# installation
#

export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_9
cd CMSSW_5_3_9/src/
cvs co -p UserCode/CMG/UserArea/EWKV/TAGS.txt | sh


runLocalAnalysisOverSamples.py -e runTopAnalysis -j samples.json -d /store/cmst3/user/psilva/539_ntuples -o ~/work/top_539/ -c test/runAnalysis_cfg.py.templ -s 8nh
runLocalAnalysisOverSamples.py -e runTopAnalysis -j syst_samples.json -d /store/cmst3/user/psilva/539_ntuples -o ~/work/top_539/ -c test/runAnalysis_cfg.py.templ -s 8nh
runPlotter --iLumi 19683 --inDir ~/work/top_539/ --json samples.json --outFile ~/work/top_539/plotter_raw.root --noPlot
runPlotter --iLumi 19683 --inDir ~/work/top_539/ --json syst_samples.json --outFile ~/work/top_539/plotter_syst.root --noPlot


runPlotter --iLumi 19683 --inDir ~/work/top_539/ --outDir ~/work/top_539/plots --json samples.json --outFile ~/work/top_539/plotter.root --noLog  --showUnc  --plotExt .pdf

