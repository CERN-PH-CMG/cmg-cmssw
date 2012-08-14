### SPECIFIC TO SM VBF Z CROSS SECTION

##############
#    2011    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_vbfz_2011.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=True" -s 8nh 
runPlotter --json data/samples_vbfz_2011.json --inDir test/results/ --outDir test/results/plots/ --iLumi 5051 --iEcm 7 --outFile plotter_vbfz_2011.root

#the photon pain strikes back
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_vbf_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=True" -s 8nh
runPlotter --json data/photon-samples_vbf_2011.json --inDir ~/work/gamma/2011/raw/ --outDir ~/work/gamma/2011/raw/plots/ --outFile ~/work/gamma/2011/raw/plotter.root --only qt --iLumi 5051 --iEcm 7
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_vbf_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_vbfz_2011.root'" -s 8nh
runPlotter --json data/photon-samples_vbf_2011.json --inDir ~/work/gamma/2011/qt/ --outDir ~/work/gamma/2011/qt/plots/ --outFile ~/work/gamma/2011/qt/plotter.root  --iLumi 5051 --iEcm 7
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_vbf_2011.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2011/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=True @weightsFile='data/gammaqtweights_vbfz_2011.root','data/gammanvtxweights_vbfz_2011.root'" -s 8nh
runPlotter --json data/photon-samples_vbf_2011.json --inDir ~/work/gamma/2011/nvtx/ --outDir ~/work/gamma/2011/nvtx/plots/ --outFile ~/work/gamma/2011/nvtx/plotter.root  --iLumi 5051 --iEcm 7

#optimize
python optimize_VBFZ.py -p 1 -m 0 -i $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/plotter_vbfz_2011.root -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_vbfz_2011.json -e 7 -t /afs/cern.ch/user/p/psilva/public/VBFZ/gamma_vbfz_2011.root -x 0.4486
  
##############
#    2012    #
##############
runLocalAnalysisOverSamples.py -e runZZAnalysis -j data/samples_vbfz_2012.json -o test/results -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=True @is2011=False" -s 8nh 
runPlotter --json data/samples_vbfz_2012.json --inDir test/results/ --outDir test/results/plots/ --iLumi 5041 --iEcm 8 --outFile plotter_vbfz_2012.root

#the photon pain strikes back
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/raw/ --outDir ~/work/gamma/2012/raw/plots/ --outFile ~/work/gamma/2012/raw/plotter.root --only qt --iLumi 5041
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/qt/ --outDir ~/work/gamma/2012/qt/plots/ --outFile ~/work/gamma/2012/qt/plotter.root  --iLumi 5041
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/nvtx  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz.root','data/gammanvtxweights_vbfz.root'" -s 8nh
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/nvtx/ --outDir ~/work/gamma/2012/nvtx/plots/ --outFile ~/work/gamma/2012/nvtx/plotter.root  --iLumi 5041

# hell closure (disable pu reweighting)
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter.root --only qt
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_qt.root --only ee
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForZZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples -o ~/work/gamma/2012/mc -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_mc8tev.root','data/gammanvtxweights_vbfz_mc8tev.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_2012.json --inDir ~/work/gamma/2012/mc/ --outDir ~/work/gamma/2012/mc/plots/ --outFile ~/work/gamma/2012/mc/plotter_nvtx.root --only ee


#optimize
python optimize_VBFZ.py -p 1 -m 0 -i $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/plotter_vbfz_2012.root -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_vbfz_2012.json -e 8 -t /afs/cern.ch/user/p/psilva/public/VBFZ/gamma_vbfz_2012.root -x 0.5484


#correlation plot
combineCards.py ee_7TeV=hzz2l2v__7TeV_ee.dat ee_8TeV=hzz2l2v__8TeV_ee.dat mumu_7TeV=hzz2l2v__7TeV_mumu.dat mumu_8TeV=hzz2l2v__8TeV_mumu.dat > hzz2l2v__7p8TeV_combined.dat
text2workspace.py hzz2l2v__7p8TeV_combined.dat -m 0 -P CMGTools.HtoZZ2l2nu.XsecModel:xsecCorrelation -v2
combine -M MultiDimFit --algo grid --points==10000 hzz2l2v__7p8TeV_combined.dat.root
root -l higgsCombineTest.MultiDimFit.mH120.root
limit->Draw("mu_8TeV:mu_7TeV>>(100,0,2,100,0,2)","deltaNLL*(deltaNLL<6)","colz")
