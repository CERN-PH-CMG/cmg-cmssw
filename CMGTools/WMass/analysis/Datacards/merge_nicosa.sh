#!/bin/bash

set -e

basefolder="/afs/cern.ch/work/a/astacchi/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/JobOutputs/"
destfolder="${basefolder}/Datacards/"
basename="profileFit_muPos"

mkdir ${destfolder}

### SIGNAL

# Template
cp "${basefolder}/${basename}/output_DYJetsPow/ZanalysisOnDATA.root" "${destfolder}/DYJetsPow.root"

# MuonScale
hadd -a "${destfolder}/DYJetsPow.root" "${basefolder}/${basename}_MuonScaleUp/output_DYJetsPow/ZanalysisOnDATA.root"
hadd -a "${destfolder}/DYJetsPow.root" "${basefolder}/${basename}_MuonScaleDown/output_DYJetsPow/ZanalysisOnDATA.root"

# KalmanVars
for ((k=1; k<=1; k++)) # 45
do
    hadd -a "${destfolder}/DYJetsPow.root" "${basefolder}/${basename}_KalmanVar${k}Up/output_DYJetsPow/ZanalysisOnDATA.root"
    hadd -a "${destfolder}/DYJetsPow.root" "${basefolder}/${basename}_KalmanVar${k}Down/output_DYJetsPow/ZanalysisOnDATA.root"
done


### BACKGROUND

# Template
cp "${basefolder}/${basename}/output_EWKTT/ZanalysisOnDATA.root" "${destfolder}/EWKTTbkg.root"

# MuonScale
hadd -a "${destfolder}/EWKTTbkg.root" "${basefolder}/${basename}_MuonScaleUp/output_EWKTT/ZanalysisOnDATA.root"
hadd -a "${destfolder}/EWKTTbkg.root" "${basefolder}/${basename}_MuonScaleDown/output_EWKTT/ZanalysisOnDATA.root"

# KalmanVars
for ((k=1; k<=1; k++)) # 45
do
    hadd -a "${destfolder}/EWKTTbkg.root" "${basefolder}/${basename}_KalmanVar${k}Up/output_EWKTT/ZanalysisOnDATA.root"
    hadd -a "${destfolder}/EWKTTbkg.root" "${basefolder}/${basename}_KalmanVar${k}Down/output_EWKTT/ZanalysisOnDATA.root"
done
