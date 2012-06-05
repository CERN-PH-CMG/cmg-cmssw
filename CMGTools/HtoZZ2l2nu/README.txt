#
# installation
# Note: LandS has to be compiled on it's own cf. https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideLandS
#

# 44x
export SCRAM_ARCH=slc5_amd64_gcc434
scramv1 project CMSSW CMSSW_4_4_4
cd CMSSW_4_4_4/src/
cvs co -r from_44x_and_52x -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS_44x.txt | sh

# 55x
export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_2_3_patch2
cd CMSSW_5_2_3_patch2/src/
cvs co -r from_44x_and_52x -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS.txt | sh

# edit the BuildFile
nano CMGTools/HtoZZ2l2nu/BuildFile.xml
#set IS44X to 0 or 1 according to the release you're using
scramv1 b


#
# make sure there are no duplicates into the AODDIR
#
removeDuplicates.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -d aoddir &> DUPLICATEDFILESFOUND
sh DUPLICATEDFILESFOUND

#
# create pat-tuples
#
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -R "tmp>5000" -n 1 -d aoddir -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_data_cfg.py" -t Data -s 8nh

#
# create ntuples
#
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -n 5 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/split" -t Data

#
# merge the ntuples
#
haddOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -d /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/split -o /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/ -t Data


#
# run the analysis
#
runLocalAnalysisOverSamples.py -e runOnEventSummary -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results -d /store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -s 8nh 

runLocalAnalysisOverSamples.py -e run2011Analysis -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results-syst -d /store/cmst3/user/querten/12_03_13_HZZ2l2v_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p "@runSystematics=True" -s 8nh 

#std leptons
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples.json -o test/results -d /store/cmst3/user/psilva/12_03_31_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True" -s 8nh 

runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samplesNoHWW.json -d /store/cmst3/user/psilva/12_03_31_HZZ2l2v_ntuples -o test/results -c test/runAnalysis_cfg.py.templ

#2012
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -n 1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/psilva/12_05_11_HZZ2l2v_ntuples/split" -t Data_

runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samplesNoHWW.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True" -s 8nh 
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples_2012.json -o test/results -d /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True" -s 8nh 

#
# z+jets replacement
#

#add the files for the ntuples
haddOverSamples.py -j data/photon-samples.json -d /store/cmst3/user/psilva/12_03_27_HZZ2l2v_ntuples -o /store/cmst3/user/psilva/grid_12_03_27_HZZ2l2v_ntuples -m 1 -t G_


## generate unweigthed distributions
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma_2012/  -c test/runAnalysis_cfg.py.templ -s 8nh


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
haddOverSamples.py -j data/samples-graviton.json -d /store/cmst3/user/psilva/12_05_06_HZZ2l2v_44x_ntuples/split/ -o /store/cmst3/user/psilva/12_05_06_HZZ2l2v_44x_ntuples/ -m 0 -t Graviton
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-graviton.json -d /store/cmst3/user/psilva/12_05_06_HZZ2l2v_44x_ntuples/ -o ~/work/gzz -c test/runAnalysis_cfg.py.templ -s 8nh -t Graviton
runPlotter --iLumi 4616 --inDir ~/work/gzz/ --outDir ~/work/gzz/plots --json data/samples-graviton.json --outFile ~/work/gzz/plotter.root

