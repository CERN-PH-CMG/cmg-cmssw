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
tree=pfjetanalyzer/tree
[[ -n $5 ]] && tree=$5

inputDir=/store/cmst3/user/musella/JETID_NTUPLES/V00-02-03_newJec_v3/DYJetsToLL-51
outputDir=/store/cmst3/user/musella/JETID_MVA_5x_v5
webDir=~/www/higgs/jetid_optimization_5x_v5

classify=$mydir/classify.py
ntuplePlotter=$mydir/../macro/ntuplePlotter.py
plotMVA=$mydir/../macro/plotMVA.py

export DISPLAY=""
commonOpts="-i $inputDir  -T common.json,${tmva_json}"
$classify $commonOpts -V $variables -l _$label  -t $tree
cmsMkdir $outputDir
cmsMkdir $outputDir/weights
cmsStage -f tmva_$label.root $outputDir
find weights -exec cmsStage -f \{\}  $outputDir/weights \;

$ntuplePlotter -i $outputDir -f tmva_$label.root -o histograms_$label.root -T -c -t TestTree -m $mvas
cmsStage -f histograms_$label.root $outputDir

mkdir $webDir
$plotMVA $(cmsPfn $outputDir/histograms_$label.root) $webDir/$label $label "" $mvas

