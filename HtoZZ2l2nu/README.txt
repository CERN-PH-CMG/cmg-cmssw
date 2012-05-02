#
# installation
# Note: LandS has to be compiled on it's own cf. https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideLandS
#
scramv1 project CMSSW CMSSW_4_4_4
cd CMSSW_4_4_4/src/
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
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 5 -s 2nd -d aoddir -t MC_WJets    -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_DY       -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_WW       -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_WZ       -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_ZZ       -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_TTJets   -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_SingleT  -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_GG       -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t MC_VBF      -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_mc_cfg.py"
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -R "tmp>5000" -n 1 -s 2nd -d aoddir -t Data        -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_data_cfg.py"

#
# create ntuples
#
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_WW
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_WZ
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_ZZ
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n 25 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_WJets
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  5 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_TTJets
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  5 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_SingleT
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n 10 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_DY
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_GG
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t MC_VBF
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split" -t Data

runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/photon-samples.json -n  1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/psilva/12_03_15_HZZ2l2v_ntuples/split"

#
# merge the ntuples
#
haddOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -d /store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/split/ -o /store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples/
haddOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/photon-samples.json -d /store/cmst3/user/psilva/12_03_15_HZZ2l2v_ntuples/split -o /store/cmst3/user/psilva/12_03_15_HZZ2l2v_ntuples

#
# run the analysis
#
runLocalAnalysisOverSamples.py -e runOnEventSummary -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results -d /store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -s 8nh 

runLocalAnalysisOverSamples.py -e run2011Analysis -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results-syst -d /store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p "@runSystematics=True" -s 8nh 

#std leptons
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples.json -o test/results -d /store/cmst3/user/psilva/12_03_31_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True" -s 8nh 

runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samplesNoHWW.json -d /store/cmst3/user/psilva/12_03_31_HZZ2l2v_ntuples -o test/results -c test/runAnalysis_cfg.py.templ

#
# z+jets replacement
#

#add the files for the ntuples
haddOverSamples.py -j data/photon-samples.json -d /store/cmst3/user/psilva/12_03_27_HZZ2l2v_ntuples -o /store/cmst3/user/psilva/grid_12_03_27_HZZ2l2v_ntuples -m 1 -t G_


## generate unweigthed distributions

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples.json -d /store/cmst3/user/querten/12_04_14_HZZ2l2v_ntuples -o ~/work/gamma/  -c test/runAnalysis_cfg.py.templ -s 8nh

runPlotter --iLumi 4616 --inDir ~/work/gamma/ --outDir ~/work/gamma/plots --json data/photon-samples.json

## use bin/G/getGammaTemplates.C to generate the weights for the sampe

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples.json -d /store/cmst3/user/querten/12_04_14_HZZ2l2v_ntuples -o ~/work/gamma-wgt/  -c test/runAnalysis_cfg.py.templ -s 8nh -p "@weightsFile=data/gammaqtweights.root"

runPlotter --iLumi 4616 --inDir ~/work/gamma-wgt/ --outDir ~/work/gamma-wgt/plots --json data/photon-samples-onlygamma.json --outFile ~/work/gamma-wgt/plotter_wgt.root

## run the replacement
#runLocalAnalysisOverSamples.py -e runOnEventSummary -j data/samples-with-dy-replacement.json -o ~/scratch0/gamma-data/ -p "@weightsFile=/afs/cern.ch/user/p/psilva/public/GammaWeights/data_gammaptvsetaweight.root" -d /castor/cern.ch/cms/store/cmst3/user/querten/11_10_19_HtoZZ2l2nNTuples/ -c test/runAnalysis_cfg.py.templ -s True

## run the final plots
#runPlotter --iLumi 5035 --inDir ~/scratch0/gamma-data/ --outDir /tmp/psilva/ --json data/samples-with-dy-replacement.json --only zmass
#runPlotter --iLumi 5035 --inDir results/ --outDir results/plots/ --outFile plotter.root  --json ../data/samples.json --only zmass


##
## GRAVITON STUDIES
##
haddOverSamples.py -j data/samples-graviton.json -d /store/cmst3/user/psilva/12_05_01_HZZ2l2v_ntuples44x/split -o /store/cmst3/user/psilva/12_05_01_HZZ2l2v_ntuples44x/ -m 0 -t Graviton
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-graviton.json -d /store/cmst3/user/psilva/12_05_01_HZZ2l2v_ntuples44x/ -o ~/work/gzz -c test/runAnalysis_cfg.py.templ -s 8nh -t Graviton
runPlotter --iLumi 4616 --inDir ~/work/gzz/ --outDir ~/work/gzz/plots --json data/samples-graviton.json --outFile ~/work/gzz/plotter.root

