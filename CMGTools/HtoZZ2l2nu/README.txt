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

## generate unweigthed distributions
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples.json -d /castor/cern.ch/cms//store/cmst3/user/psilva/HZZ_11.11.24 -o ~/scratch0/gamma/  -c test/runAnalysis_cfg.py.templ -s 8nh

## generate plotter.root
runPlotter --iLumi 4616 --inDir ~/scratch0/gamma/ --outDir /tmp/psilva/ --json data/photon-samples.json

## use bin/G/getGammaTemplates.C to generate the weights for the sampe

## run again with weights
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples.json -d /castor/cern.ch/cms//store/cmst3/user/psilva/HZZ_11.11.24 -o ~/scratch0/gamma/  -c test/runAnalysis_cfg.py.templ -s 8nh -c test/runAnalysis_cfg.py.templ -p "@weightsFile=/afs/cern.ch/user/p/psilva/public/GammaWeights/mc_gammaqtvsnvtxweight.root"


## run the replacement
runLocalAnalysisOverSamples.py -e runOnEventSummary -j data/samples-with-dy-replacement.json -o ~/scratch0/gamma-data/ -p "@weightsFile=/afs/cern.ch/user/p/psilva/public/GammaWeights/data_gammaptvsetaweight.root" -d /castor/cern.ch/cms/store/cmst3/user/querten/11_10_19_HtoZZ2l2nNTuples/ -c test/runAnalysis_cfg.py.templ -s True

## run the final plots
runPlotter --iLumi 4616 --inDir ~/scratch0/gamma-data/ --outDir /tmp/psilva/ --json data/samples-with-dy-replacement.json --only zmass
