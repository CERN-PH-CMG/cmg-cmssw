### SPECIFIC TO SM ZZ CROSS SECTION

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_ZZ.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=True" -s 8nh 

runPlotter --json data/samples_ZZ.json --inDir test/results/ --outDir test/results/plots/ --iLumi 5051 --iEcm 7 --outFile plotter_zz_2011.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/raw/ --outDir ~/work/gamma/2011/raw/plots/ --outFile ~/work/gamma/2011/raw/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_2011.root'" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/qt/ --outDir ~/work/gamma/2011/qt/plots/ --outFile ~/work/gamma/2011/qt/plotter.root 
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_2011.root','data/gammanvtxweights_2011.root'" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/nvtx/ --outDir ~/work/gamma/2011/nvtx/plots/ --outFile ~/work/gamma/2011/nvtx/plotter.root 

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_ZZ_2012.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=False" -s 8nh 

runPlotter --json data/samples_ZZ_2012.json --inDir test/results/ --outDir test/results/plots/ --iLumi 5045 --iEcm 8 --outFile plotter_zz_2012.root
