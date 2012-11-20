#!/bin/sh

mkdir ${1}/test_numbers_EWK
mkdir ${1}/test_numbers_EWKTT

# hadd -f ${1}/test_numbers_EWK/ZanalysisOnDATA.root ${1}/test_numbers_WJetsSig/ZanalysisOnDATA.root ${1}/test_numbers_WJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_TTJets/ZanalysisOnDATA.root ${1}/test_numbers_ZZJets/ZanalysisOnDATA.root ${1}/test_numbers_WZJets/ZanalysisOnDATA.root ${1}/test_numbers_WWJets/ZanalysisOnDATA.root 

# EWK ONLY FOR Z ANALYSIS
hadd -f ${1}/test_numbers_EWK/ZanalysisOnDATA.root ${1}/test_numbers_WJetsSig/ZanalysisOnDATA.root ${1}/test_numbers_WJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_ZZJets/ZanalysisOnDATA.root ${1}/test_numbers_WZJets/ZanalysisOnDATA.root ${1}/test_numbers_WWJets/ZanalysisOnDATA.root 
# EWK + TT FOR Z ANALYSIS
hadd -f ${1}/test_numbers_EWKTT/ZanalysisOnDATA.root ${1}/test_numbers_EWK/ZanalysisOnDATA.root ${1}/test_numbers_TTJets/ZanalysisOnDATA.root
# EWK ONLY FOR W ANALYSIS
hadd -f ${1}/test_numbers_EWK/WanalysisOnDATA.root ${1}/test_numbers_WJetsFake/WanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/WanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/WanalysisOnDATA.root ${1}/test_numbers_ZZJets/WanalysisOnDATA.root ${1}/test_numbers_WZJets/WanalysisOnDATA.root ${1}/test_numbers_WWJets/WanalysisOnDATA.root 
# EWK + TT FOR W ANALYSIS
hadd -f ${1}/test_numbers_EWKTT/WanalysisOnDATA.root ${1}/test_numbers_EWK/WanalysisOnDATA.root ${1}/test_numbers_TTJets/WanalysisOnDATA.root
