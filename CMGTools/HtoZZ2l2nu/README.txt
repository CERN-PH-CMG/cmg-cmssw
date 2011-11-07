#
# create ntuples
#
runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t W

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t TT

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t Z

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t DY

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t SingleT

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t May10

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t v4

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t Aug05

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t v6


#
# z+jets replacement
#
runLocalAnalysisOverSamples.py -e runOnEventSummary -j data/samples-with-dy-replacement.json -o ~/scratch0/gamma-data/ -p "@weightsFile=/afs/cern.ch/user/p/psilva/public/GammaWeights/data_gammaptvsetaweight.root" -d /castor/cern.ch/cms/store/cmst3/user/querten/11_10_19_HtoZZ2l2nNTuples/ -c test/runAnalysis_cfg.py.templ -s True

runPlotter --iLumi 2200 --inDir ~/scratch0/gamma-data/ --outDir /tmp/psilva/ --json data/samples-with-dy-replacement.json --only zmass
