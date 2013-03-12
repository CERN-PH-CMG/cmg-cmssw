#!/bin/bash

#
# closure test
#

step=$1
tag=("pythia" "mg")
outdir="~/work/ewkz"



for i in ${tag[@]}; do

    if [ "$step" == "1" ]; then
	runLocalAnalysisOverSamples.py -e runVBFZAnalysis -j data/dy-samples_${i}.json -o ${outdir}/dy/${i} -d /store/cmst3/user/psilva/Moriond2013_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p "@runSystematics=False @is2011=False" -s 8nh; 
	runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_${i}.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ${outdir}/g/raw_${i}  -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p "@is2011=False" -s 8nh;
    fi

    if [ "$step" == "2" ]; then
	runPlotter --json data/dy-samples_${i}.json --inDir ${outdir}/dy/${i}/ --noPlot --iLumi 19577 --iEcm 8 --outFile ${outdir}/plotter_ewkz_${i}.root;
	runPlotter --json data/photon-samples_${i}.json --inDir ${outdir}/g/raw_${i}/ --noPlot --iLumi 19577 --iEcm 8 --outFile ${outdir}/plotter_ewkz_g_raw_${i}.root;
	root -b -q "$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/bin/G/FitQtSpectrum.C+(\"${outdir}/plotter_ewkz_${i}.root\",\"${outdir}/plotter_ewkz_g_raw_${i}.root\",NOFITSMOOTH,PT,PUREG)";
	mv gammawgts.root ${outdir}/gammaqtweights_ewkz_${i}.root;
    fi

    if [ "$step" == "3" ]; then
	runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplatesForVBFZ -j data/photon-samples_${i}.json -d /store/cmst3/user/psilva/Moriond2013_ntuples -o ${outdir}/g/qt_${i} -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p "@is2011=False @weightsFile='${outdir}/gammaqtweights_ewkz_${i}.root'" -s 8nh;
    fi

    if [ "$step" == "4" ]; then
	runPlotter --json data/photon-samples_${i}.json --inDir ${outdir}/g/qt_${i}/ --noPlot --iLumi 19577 --iEcm 8 --outFile ${outdir}/plotter_ewkz_g_qt_${i}.root;
    fi
done

