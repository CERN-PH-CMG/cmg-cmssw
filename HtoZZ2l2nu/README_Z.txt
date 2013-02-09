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
root -b -q "bin/G/FitQtSpectrum.C+(\"~/work/vbfz/plotter_vbfz_2012.root\",\"~/work/vbfz/plotter_vbfz_gamma_raw_2012.root\",VBFZ)"
mv gammawgts.root data/gammaqtweights_vbfz_2012.root
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/vbfz/results/2012_gamma/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_2012.root'" -s 8nh -t Data
runPlotter --json data/photon-samples_2012.json --inDir ~/work/vbfz/results/2012_gamma/qt/ --outDir ~/work/vbfz/results/2012_gamma/qt/plots/ --iLumi 19577 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_gamma_qt_2012.root

### closure
#use empty pileup distribution in test/runAnalysis_cfg.py.templ
runLocalAnalysisOverSamples.py -e runVBFZAnalysis -j data/samples_vbfz_2012.json -o ~/work/vbfz/results/2012_closure -d /store/cmst3/user/psilva/Moriond2013_ntuples -c test/runAnalysis_cfg.py.templ -p "@runSystematics=False @is2011=False" -s 8nh -t DY
runPlotter --json data/samples_vbfz_2012.json --inDir ~/work/vbfz/results/2012_closure/ --outDir ~/work/vbfz/results/2012_closure/plots/ --iLumi 16698 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_closure_2012.root --noPlot
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_vbfz_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/vbfz/results/2012_gamma_closure/raw  -c test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh -t MC
runPlotter --json data/photon-samples_vbfz_2012.json --inDir ~/work/vbfz/results/2012_gamma_closure/raw/ --outDir ~/work/vbfz/results/2012_gamma_closure/raw/plots/ --iLumi 16698 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_gamma_closure_raw_2012.root --noPlot
root -b -q "bin/G/FitQtSpectrum.C+(\"~/work/vbfz/plotter_vbfz_closure_2012.root\",\"~/work/vbfz/plotter_vbfz_gamma_closure_raw_2012.root\",VBFZ)"
mv gammawgts.root data/gammaqtweights_vbfz_2012.root
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_vbfz_2012.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ~/work/vbfz/results/2012_gamma_closure/qt  -c test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='data/gammaqtweights_vbfz_closure_2012.root'" -s 8nh -t MC
runPlotter --json data/photon-samples_vbfz_2012.json --inDir ~/work/vbfz/results/2012_gamma_closure/qt/ --outDir ~/work/vbfz/results/2012_gamma_closure/qt/plots/ --iLumi 16698 --iEcm 8 --outFile ~/work/vbfz/plotter_vbfz_gamma_closure_2012.root --noPlot

#CHECK ME THIS POINT FWD

#optimize
python test/computeLimits/optimize_VBFZ.py -p 1 -m 0 -i ~/work/vbfz/results/2012/plotter_vbfz_2012.root -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_vbfz_2012.json -e 8 -t ~/work/vbfz/results/2012/gamma_vbfz_2012.root -x 0.215

#correlation plot
combineCards.py ee_7TeV=hzz2l2v__7TeV_ee.dat ee_8TeV=hzz2l2v__8TeV_ee.dat mumu_7TeV=hzz2l2v__7TeV_mumu.dat mumu_8TeV=hzz2l2v__8TeV_mumu.dat > hzz2l2v__7p8TeV_combined.dat
text2workspace.py hzz2l2v__7p8TeV_combined.dat -m 0 -P CMGTools.HtoZZ2l2nu.XsecModel:xsecCorrelation -v2
combine -M  MultiDimFit --algo=singles --cl=0.68 hzz2l2v__7p8TeV_combined.dat.root 
combine -M MultiDimFit --algo grid --points==10000 hzz2l2v__7p8TeV_combined.dat.root
root -l higgsCombineTest.MultiDimFit.mH120.root
limit->Draw("mu_8TeV:mu_7TeV>>(100,0,2,100,0,2)","deltaNLL*(deltaNLL<6)","colz")
