#
# installation
#
scramv1 project CMSSW CMSSW_4_4_3
cd CMSSW_4_4_3/src/
cvs co -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS.txt | sh
scramv1 b

#
# create pat-tuples
#
runOverSamples.py -j data/samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_4_3/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py -castor=@patdir" -n 5 -s True -d aoddir

#
# create ntuples
#
runOverSamples.py -j data/samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_4_3/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_4_3/src/CMGTools/HtoZZ2l2nu/data" -d patdir -s 8nh -t G

#
# run the analysis
#
runLocalAnalysisOverSamples.py -e runOnEventSummary -j data/beauty-samples.json -o ~/scratch0/hzz/ -d /castor/cern.ch/cms/store/cmst3/user/querten/11_11_07_HtoZZ2l2nNTuples -c test/runAnalysis_cfg.py.templ -s 8nh 



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
