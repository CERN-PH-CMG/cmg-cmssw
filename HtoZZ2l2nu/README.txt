#
# installation
#
scramv1 project CMSSW CMSSW_4_4_3
cd CMSSW_4_4_3/src/
cvs co -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS.txt | sh
scramv1 b

#
# make sure there are no duplicates into the AODDIR
#
removeDuplicates.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -d aoddir &> DUPLICATEDFILESFOUND
sh DUPLICATEDFILESFOUND

#
# create pat-tuples
#
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 5 -s 2nd -d aoddir -t MC_            -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 5 -s 2nd -d aoddir -t Data_DoubleEle -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_data_ee_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 5 -s 2nd -d aoddir -t Data_DoubleMu  -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_data_mumu_cfg.py"

#
# create ntuples
#
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n 10 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_02_20_HZZ2l2v_ntuples/split"

#
# merge the ntuples
#
haddOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -d /store/cmst3/user/querten/12_02_20_HZZ2l2v_ntuples/split/ -o /store/cmst3/user/querten/12_02_20_HZZ2l2v_ntuples/

#
# run the analysis
#
runLocalAnalysisOverSamples.py -e runOnEventSummary -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results -d /store/cmst3/user/querten/12_02_20_HZZ2l2v_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -s 8nh 



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
runPlotter --iLumi 5035 --inDir ~/scratch0/gamma-data/ --outDir /tmp/psilva/ --json data/samples-with-dy-replacement.json --only zmass
runPlotter --iLumi 5035 --inDir results/ --outDir results/plots/ --outFile plotter.root  --json ../data/samples.json --only zmass
