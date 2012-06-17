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
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2011.json -n 5 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/work/split" -t Data
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -n 5 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/work/split" -t 2012B

#
# merge the ntuples
#
haddOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -d /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/split -o /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/ -t Data


##############
#    2012    #
##############
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -n 1 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/store/cmst3/user/psilva/12_05_11_HZZ2l2v_ntuples/split" -t Data_

runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples_2012.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=False" -s 8nh 

#photon+jets
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-data_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh
runPlotter --json data/photon-samples-data_2012.json --inDir ~/work/gamma/2012/raw/ --outDir ~/work/gamma/2012/raw/plots/ --outFile ~/work/gamma/2012/raw/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-data_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_2012.root'" -s 8nh
runPlotter --json data/photon-samples-data_2012.json --inDir ~/work/gamma/2012/qt/ --outDir ~/work/gamma/2012/qt/plots/ --outFile ~/work/gamma/2012/qt/plotter.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-data_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_2012.root','data/gammanvtxweights_2012.root'" -s 8nh
runPlotter --json data/photon-samples-data_2012.json --inDir ~/work/gamma/2012/nvtx/ --outDir ~/work/gamma/2012/nvtx/plots/ --outFile ~/work/gamma/2012/nvtx/plotter.root 

#photon+jets (closure)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_qt.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_mc8tev.root','data/gammanvtxweights_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples-mc_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_nvtx.root 

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples_2011.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=True" -s 8nh 

#photon+jets
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-data_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh
runPlotter --json data/photon-samples-data_2011.json --inDir ~/work/gamma/2011/raw/ --outDir ~/work/gamma/2011/raw/plots/ --outFile ~/work/gamma/2011/raw/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-data_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_2011.root'" -s 8nh
runPlotter --json data/photon-samples-data_2011.json --inDir ~/work/gamma/2011/qt/ --outDir ~/work/gamma/2011/qt/plots/ --outFile ~/work/gamma/2011/qt/plotter.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-data_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_2011.root','data/gammanvtxweights_2011.root'" -s 8nh
runPlotter --json data/photon-samples-data_2011.json --inDir ~/work/gamma/2011/nvtx/ --outDir ~/work/gamma/2011/nvtx/plots/ --outFile ~/work/gamma/2011/nvtx/plotter.root 

#photon+jets (closure)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh -t MC
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/mc/ --outDir ~/work/gamma/2011/mc/plots/ --outFile ~/work/gamma/2011/mc/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_mc7tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/mc/ --outDir ~/work/gamma/2011/mc/plots/ --outFile ~/work/gamma/2011/mc/plotter_qt.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_mc7tev.root','data/gammanvtxweights_mc7tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples-mc_2011.json --inDir ~/work/gamma/2011/mc/ --outDir ~/work/gamma/2011/mc/plots/ --outFile ~/work/gamma/2011/mc/plotter_nvtx.root 

#
# use bin/G/getGammaTemplates.C to generate the weights for the samples in both cases (data/MC)
#
runPlotter --iLumi 4616 --inDir ~/work/gamma/ --outDir ~/work/gamma/plots --json data/photon-samples.json


##
## GRAVITON STUDIES
##
haddOverSamples.py -j data/samples-graviton.json -d /store/cmst3/user/psilva/12_05_06_HZZ2l2v_44x_ntuples/split/ -o /store/cmst3/user/psilva/12_05_06_HZZ2l2v_44x_ntuples/ -m 0 -t Graviton
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-graviton.json -d /store/cmst3/user/psilva/12_05_06_HZZ2l2v_44x_ntuples/ -o ~/work/gzz -c test/runAnalysis_cfg.py.templ -s 8nh -t Graviton
runPlotter --iLumi 4616 --inDir ~/work/gzz/ --outDir ~/work/gzz/plots --json data/samples-graviton.json --outFile ~/work/gzz/plotter.root

