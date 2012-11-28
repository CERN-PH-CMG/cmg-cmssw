### SPECIFIC TO SM ZZ CROSS SECTION

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e run2011Analysis -j data/samples_sm_2012.json -o  ~/work/zz/ll/2012 -d /store/cmst3/user/psilva/Moriond2013_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=False @is2011=False" -s 8nh 
runPlotter --json data/samples_sm_2012.json --inDir ~/work/zz/ll/2012/ --outDir ~/work/zz/ll/2012/plots --outFile ~/work/zz/ll/2012/plotter.root --iEcm 8 --iLumi 12196

#the photon pain
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/raw/ --outDir ~/work/gamma/2011/raw/plots/ --outFile ~/work/gamma/2011/raw/plotter.root --only qt --iLumi 5051 --iEcm 7
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_ZZ_2011.root'" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/qt/ --outDir ~/work/gamma/2011/qt/plots/ --outFile ~/work/gamma/2011/qt/plotter.root  --iLumi 5051 --iEcm 7
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_ZZ_2011.root','data/gammanvtxweights_ZZ_2011.root'" -s 8nh
runPlotter --json data/photon-samples_2011.json --inDir ~/work/gamma/2011/nvtx/ --outDir ~/work/gamma/2011/nvtx/plots/ --outFile ~/work/gamma/2011/nvtx/plotter.root  --iLumi 5051 --iEcm 7

##############
#    2012    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_ZZ_2012.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=False" -s 8nh 
runPlotter --json data/samples_ZZ_2012.json --inDir test/results/ --outDir test/results/plots/ --iLumi 5045 --iEcm 8 --outFile plotter_zz_2012.root

#the photon pain strikes back
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/raw/ --outDir ~/work/gamma/2012/raw/plots/ --outFile ~/work/gamma/2012/raw/plotter.root --only qt --iLumi 5041
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_ZZ_2012.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/qt/ --outDir ~/work/gamma/2012/qt/plots/ --outFile ~/work/gamma/2012/qt/plotter.root  --iLumi 5041
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_ZZ_2012.root','data/gammanvtxweights_ZZ_2012.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/nvtx/ --outDir ~/work/gamma/2012/nvtx/plots/ --outFile ~/work/gamma/2012/nvtx/plotter.root  --iLumi 5041

# hell closure (disable pu reweighting)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_ZZ_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_qt.root --only ee
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_ZZ_mc8tev.root','data/gammanvtxweights_ZZ_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_nvtx.root --only ee
