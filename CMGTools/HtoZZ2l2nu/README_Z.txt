### SPECIFIC TO SM VBF Z CROSS SECTION

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_vbfz_2011.json -o ~/work/vbfz/results/2011 -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=True" -s 8nh 
runPlotter --json data/samples_vbfz_2011.json --inDir ~/work/vbfz/results/2011/ --outDir ~/work/vbfz/results/2011/plots/ --iLumi 5051 --iEcm 7 --outFile  ~/work/vbfz/results/2011/plotter_vbfz_2011.root

#the photon pain strikes back
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_vbf_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2011/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh
runPlotter --json data/photon-samples_vbf_2011.json --inDir ~/work/vbfz/gamma/2011/raw/ --outDir ~/work/vbfz/gamma/2011/raw/plots/ --outFile ~/work/vbfz/gamma/2011/raw/plotter.root --only qt --iLumi 5051 --iEcm 7
root -b -q "bin/G/getGammaWeights.C+(\"~/work/vbfz/gamma/2011/raw/plotter.root\",\"qt\")"
mv gammaqtweights.root data/gammaqtweights_vbfz_2011.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_vbf_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2011/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_vbfz_2011.root'" -s 8nh
runPlotter --json data/photon-samples_vbf_2011.json --inDir ~/work/vbfz/gamma/2011/qt/ --outDir ~/work/vbfz/gamma/2011/qt/plots/ --outFile ~/work/vbfz/gamma/2011/qt/plotter.root  --iLumi 5051 --iEcm 7
root -b -q "bin/G/getGammaWeights.C+(\"~/work/vbfz/gamma/2011/qt/plotter.root\",\"nvtx\")"
mv gammavtxweights.root data/gammanvtxweights_vbfz_2011.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_vbf_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2011/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_vbfz_2011.root','data/gammanvtxweights_vbfz_2011.root'" -s 8nh
runPlotter --json data/photon-samples_vbf_2011.json --inDir ~/work/vbfz/gamma/2011/nvtx/ --outDir ~/work/vbfz/gamma/2011/nvtx/plots/ --outFile ~/work/vbfz/gamma/2011/nvtx/plotter.root  --iLumi 5051 --iEcm 7

#optimize
python test/computeLimits/optimize_VBFZ.py -p 1 -m 0 -i ~/work/vbfz/results/2011/plotter_vbfz_2011.root -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_vbfz_2011.json -e 7 -t ~/work/vbfz/results/2011/gamma_vbfz_2011.root -x 0.165
  
##############
#    2012    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_vbfz_2012.json -o ~/work/vbfz/results/2012 -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=False" -s 8nh 
runPlotter --json data/samples_vbfz_2012.json --inDir ~/work/vbfz/results/2012/ --outDir ~/work/vbfz/results/2012/plots/ --iLumi 5041 --iEcm 8 --outFile  ~/work/vbfz/results/2012/plotter_vbfz_2012.root

#the photon pain strikes back
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2012/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/gamma/2012/raw/ --outDir ~/work/vbfz/gamma/2012/raw/plots/ --outFile ~/work/vbfz/gamma/2012/raw/plotter.root --only qt --iLumi 5041
root -b -q "bin/G/getGammaWeights.C+(\"~/work/vbfz/gamma/2012/raw/plotter.root\",\"qt\")"
mv gammaqtweights.root data/gammaqtweights_vbfz_2012.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2012/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_2012.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/gamma/2012/qt/ --outDir ~/work/vbfz/gamma/2012/qt/plots/ --outFile ~/work/vbfz/gamma/2012/qt/plotter.root  --iLumi 5041
root -b -q "bin/G/getGammaWeights.C+(\"~/work/vbfz/gamma/2012/qt/plotter.root\",\"nvtx\")"
mv gammavtxweights.root data/gammanvtxweights_vbfz_2012.root

runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2012/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_2012.root','data/gammanvtxweights_vbfz_2012.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/gamma/2012/nvtx/ --outDir ~/work/vbfz/gamma/2012/nvtx/plots/ --outFile ~/work/vbfz/gamma/2012/nvtx/plotter.root  --iLumi 5041

# hell closure (disable pu reweighting)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/gamma/2012/mc/ --outDir ~/work/vbfz/gamma/2012/mc/plots/ --outFile ~/work/vbfz/gamma/2012/mc/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/gamma/2012/mc/ --outDir ~/work/vbfz/gamma/2012/mc/plots/ --outFile ~/work/vbfz/gamma/2012/mc/plotter_qt.root --only ee
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/vbfz/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_mc8tev.root','data/gammanvtxweights_vbfz_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/gamma/2012/mc/ --outDir ~/work/vbfz/gamma/2012/mc/plots/ --outFile ~/work/vbfz/gamma/2012/mc/plotter_nvtx.root --only ee


#optimize
python test/computeLimits/optimize_VBFZ.py -p 1 -m 0 -i ~/work/vbfz/results/2012/plotter_vbfz_2012.root -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_vbfz_2012.json -e 8 -t ~/work/vbfz/results/2012/gamma_vbfz_2012.root -x 0.215



#correlation plot
combineCards.py ee_7TeV=hzz2l2v__7TeV_ee.dat ee_8TeV=hzz2l2v__8TeV_ee.dat mumu_7TeV=hzz2l2v__7TeV_mumu.dat mumu_8TeV=hzz2l2v__8TeV_mumu.dat > hzz2l2v__7p8TeV_combined.dat
text2workspace.py hzz2l2v__7p8TeV_combined.dat -m 0 -P CMGTools.HtoZZ2l2nu.XsecModel:xsecCorrelation -v2
combine -M MultiDimFit --algo grid --points==10000 hzz2l2v__7p8TeV_combined.dat.root
root -l higgsCombineTest.MultiDimFit.mH120.root
limit->Draw("mu_8TeV:mu_7TeV>>(100,0,2,100,0,2)","deltaNLL*(deltaNLL<6)","colz")
