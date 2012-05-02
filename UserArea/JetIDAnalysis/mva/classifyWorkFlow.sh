#!/bin/bash

set -x

cd $(dirname $(which $0))
mydir=$PWD
eval $(scram ru -sh)
cd - 
cp -pv $mydir/*.json . 

variables=$1
label=$2
tmva_json=tmva_quick.json
[[ -n $3 ]] && tmva_json=$3
mvas=BDT_$label
[[ -n $4 ]] && mvas=$4

## inputDir=/store/cmst3/user/musella/JETID_NTUPLES/27_03_2012/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/
## inputDir=/store/cmst3/user/malberti/JETID_NTUPLES/29_03_2012/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/
## inputDir=/store/cmst3/user/musella/JETID_NTUPLES/18_04_2012/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/
inputDir=/store/cmst3/user/musella/JETID_NTUPLES/V00-00-09/DYJetsToLL-51/
outputDir=/store/cmst3/user/musella/JETID_MVA_5x_v3
webDir=~/www/higgs/jetid_optimization_5x_v3

classify=$mydir/classify.py
ntuplePlotter=$mydir/../macro/ntuplePlotter.py
plotMVA=$mydir/../macro/plotMVA.py

export DISPLAY=""
commonOpts="-i $inputDir  -T common.json,${tmva_json}"
$classify $commonOpts -V $variables -l _$label
cmsMkdir $outputDir
cmsMkdir $outputDir/weights
cmsStage -f tmva_$label.root $outputDir
find weights -exec cmsStage -f \{\}  $outputDir/weights \;

## $ntuplePlotter -i $outputDir -f tmva_$label.root -o histograms_$label.root -T -c -t TestTree -m Fisher_$label,BDT_$label
### $ntuplePlotter -i $outputDir -f tmva_$label.root -o histograms_$label.root -T -c -t TestTree -m BDT_$label
## $ntuplePlotter -i $outputDir -f tmva_$label.root -o histograms_$label.root -T -c -t TestTree -m Fisher_$label,Likelihood_$label
$ntuplePlotter -i $outputDir -f tmva_$label.root -o histograms_$label.root -T -c -t TestTree -m $mvas
cmsStage -f histograms_$label.root $outputDir

mkdir $webDir
$plotMVA $(cmsPfn $outputDir/histograms_$label.root) $webDir/$label $label "" $mvas

