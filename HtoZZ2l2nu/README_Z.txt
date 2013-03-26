### SPECIFIC TO SM VBF Z CROSS SECTION

# 53x
export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_3_patch2
cd CMSSW_5_3_3_patch2/src/
cvs co -p UserCode/CMG/CMGTools/HtoZZ2l2nu/TAGS.txt | sh

#run selection
runLocalAnalysisOverSamples.py -e runVBFZAnalysis -j data/samples_vbfz_2012.json -o ~/work/vbfz/results/2012 -d /store/cmst3/user/psilva/Moriond2013_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=False @is2011=False" -s 8nh 
runPlotter --json data/samples_vbfz_2012.json --inDir ~/work/vbfz/results/2012/ --outDir ~/work/vbfz/results/2012/plots/ --iLumi 19577 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_2012.root

#photon control sample
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/vbfz/results/2012_gamma/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t Data
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/results/2012_gamma/raw/ --outDir ~/work/vbfz/results/2012_gamma/raw/plots/ --iLumi 19577 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_gamma_raw_2012.root
root -b -q "bin/G/FitQtSpectrum.C+(\"~/work/vbfz/plotter_vbfz_2012.root\",\"~/work/vbfz/plotter_vbfz_gamma_raw_2012.root\",NOFITSMOOTH,PT,ALL)"
mv gammawgts.root data/gammaqtweights_vbfz_2012.root
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/vbfz/results/2012_gamma/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_2012.root'" -s 8nh -t Data
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/results/2012_gamma/qt/ --outDir ~/work/vbfz/results/2012_gamma/qt/plots/ --iLumi 19577 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_gamma_qt_2012.root



#optimize
python test/computeLimits/optimize_VBFZ.py -p 1

#correlation plot
#combineCards.py ee_7TeV=hzz2l2v__7TeV_ee.dat ee_8TeV=hzz2l2v__8TeV_ee.dat mumu_7TeV=hzz2l2v__7TeV_mumu.dat mumu_8TeV=hzz2l2v__8TeV_mumu.dat > hzz2l2v__7p8TeV_combined.dat
#text2workspace.py hzz2l2v__7p8TeV_combined.dat -m 0 -P CMGTools.HtoZZ2l2nu.XsecModel:xsecCorrelation -v2
#combine -M  MultiDimFit --algo=singles --cl=0.68 hzz2l2v__7p8TeV_combined.dat.root 
#combine -M MultiDimFit --algo grid --points==10000 hzz2l2v__7p8TeV_combined.dat.root
#root -l higgsCombineTest.MultiDimFit.mH120.root
#limit->Draw("mu_8TeV:mu_7TeV>>(100,0,2,100,0,2)","deltaNLL*(deltaNLL<6)","colz")
