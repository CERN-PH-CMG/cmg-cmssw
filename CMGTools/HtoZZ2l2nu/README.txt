#
# installation
# Note: LandS has to be compiled on it's own cf. https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideLandS
#

# 44x
export SCRAM_ARCH=slc5_amd64_gcc434
scramv1 project CMSSW CMSSW_4_4_4
cd CMSSW_4_4_4/src/
cvs co -r from_44x_and_52x -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS_44x.txt | sh

# 52x
export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_2_5
cd CMSSW_5_2_5/src/
cvs co -r V12-08-03 -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS.txt | sh

# 53x
export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_3_patch2
cd CMSSW_5_3_3_patch2/src/
cvs co -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS.txt | sh

# edit the BuildFile
nano CMGTools/HtoZZ2l2nu/BuildFile.xml
#set IS44X to 0 or 1 according to the release you're using
scramv1 b


#
# make sure there are no duplicates into the AODDIR
#
removeDuplicates.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -d aoddir &> DUPLICATEDFILESFOUND
sh DUPLICATEDFILESFOUND

# create pat-tuples
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -R "tmp>5000" -n 1 -d aoddir -p "-castor=patdir -cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvPattuple_data_cfg.py" -t Data -s 8nh

# create ntuples
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2011.json -n 5 -s 8nh -d patdir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/work/split" -t Data

# run full analysis
runOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_atgc.json -n 5 -s 8nh -d aoddir -p "-cfg=$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/zzllvvFullAnalysis_mc_cfg.py -castor=/afs/cern.ch/user/p/psilva/work/split" 


#
# merge the ntuples
#
haddOverSamples.py -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json -d /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/split -o /store/cmst3/user/psilva/2012_May_12_HZZ2l2v_ntuples/ -t Data


##############
#    2012    #
##############

runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples_2012.json -o  ~/work/htozz/53x_rereco/ll/2012 -d /store/cmst3/user/psilva/Moriond2013_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=False" -s 8nh 
runPlotter --json data/samples_2012.json --inDir ~/work/htozz/53x_rereco/ll/2012/ --noPlot --outFile ~/work/htozz/53x_rereco/ll/2012/plotter.root --iEcm 8 --iLumi 19577

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/htozz/53x_rereco/gamma/2012/raw -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/htozz/53x_rereco/gamma/2012/raw/ --noPlot --outFile ~/work/htozz/53x_rereco/gamma/2012/raw/plotter.root --iEcm 8 --iLumi 19577
root -b -q "bin/G/FitQtSpectrum.C+(\"~/work/htozz/53x_rereco/ll/2012/plotter.root\",\"~/work/htozz/53x_rereco/gamma/2012/raw/plotter.root\",NOFITSMOOTH,PT)"
mv gammawgts.root data/gammaqtweights_hzz_2012.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/htozz/53x_rereco/gamma/2012/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_hzz_2012.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/htozz/53x_rereco/gamma/2012/qt/ --noPlot --outFile ~/work/htozz/53x_rereco/gamma/2012/qt/plotter.root --iEcm 8 --iLumi 19577

#vary phase -p 1 - 4
python optimize.py -p 1 -m 0 -s mt_shapes -i /afs/cern.ch/user/p/psilva/public/HtoZZ/Moriond/plotter2012.root -o /afs/cern.ch/user/p/psilva/work/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/test/computeLimits/JOBS/ -g /afs/cern.ch/user/p/psilva/public/HtoZZ/Moriond/gamma_out.root -j /afs/cern.ch/work/p/psilva/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/data/samples_2012.json


#photon+jets (closure)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter.root --isSim --iLumi 10198
root -b -q "bin/G/getGammaWeights.C+(\"~/work/gamma/2012/mc/plotter.root\",\"qt\")"

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_mc_2012.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_qt.root --isSim --iLumi 10198

runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_qt.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_mc8tev.root','data/gammanvtxweights_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_nvtx.root 

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples_2011.json -o ~/work/htozz/53x/ll/2011 -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=True" -s 8nh 
runPlotter --json data/samples_2011.json --inDir ~/work/htozz/53x/ll/2011/ --outDir ~/work/htozz/53x/ll/2011/plots/ --outFile ~/work/htozz/53x/ll/2011/plotter.root --iEcm 7 --iLumi 5051

#photon+jets
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/htozz/53x/gamma/2011/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh -t Data
runPlotter --json data/photon-samples_2011.json --inDir ~/work/htozz/53x/gamma/2011/raw/ --outDir ~/work/htozz/53x/gamma/2011/raw/plots/ --outFile ~/work/htozz/53x/gamma/2011/raw/plotter.root --iEcm 7 --iLumi 5051
root -b -q "bin/G/FitQtSpectrum.C+(\"~/work/htozz/53x/ll/2011/plotter.root\",\"~/work/htozz/53x/gamma/2011/raw/plotter.root\",NOFITSMOOTH,PT)"
mv gammawgts.root data/gammaqtweights_hzz_2011.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/htozz/53x/gamma/2011/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_hzz_2011.root'" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/htozz/53x/gamma/2011/qt/ --noPlot --outFile ~/work/htozz/53x/gamma/2011/qt/plotter.root --iEcm 7 --iLumi 5051

#photon+jets (closure)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh -t MC
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/mc/ --outDir ~/work/gamma/2011/mc/plots/ --outFile ~/work/gamma/2011/mc/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_mc7tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/mc/ --outDir ~/work/gamma/2011/mc/plots/ --outFile ~/work/gamma/2011/mc/plotter_qt.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples-mc_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_mc7tev.root','data/gammanvtxweights_mc7tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples-mc_2011.json --inDir ~/work/gamma/2011/mc/ --outDir ~/work/gamma/2011/mc/plots/ --outFile ~/work/gamma/2011/mc/plotter_nvtx.root 


################################## 
# 2011 vs 2012 signal comparison #
##################################
#use the same pu distribution in test/runAnalysis_cfg.py.templ
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-qqsignal_2011vs2012.json -o test/comp-results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh -t 7TeV
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-qqsignal_2011vs2012.json -o test/comp-results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t 8TeV
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-ggsignal_2011vs2012.json -o test/comp-results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh -t 7TeV
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples-ggsignal_2011vs2012.json -o test/comp-results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t 8TeV

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


##
## OPTIMIZATION repeat for -p 1-4 to get the final plot (add -b if you want to blind)
##
python optimize.py -p 1 -m 0 -s mt_shapes -i /afs/cern.ch/user/p/psilva/public/HtoZZ/Council_chsnodphisoftjet/plotter.root -o /afs/cern.ch/user/p/psilva/work/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/test/computeLimits/JOBS_chsnodphisoftjmet/ -g /afs/cern.ch/user/p/psilva/public/HtoZZ/Council_chsnodphisoftjet/gamma_half_2012_12fbinv.root -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json