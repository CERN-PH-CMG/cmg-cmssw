#!/bin/sh

echo "merging chunks (if they exist)"

samples=("DATA" "WJetsSig" "WJetsFake" "DYJetsSig" "DYJetsFake" "TTJets" "ZZJets" "WZJets" "WWJets" "QCD")
analyses=("Wanalysis" "Zanalysis" "PhiStarEtaAnalysis")

for (( id_sample=0; id_sample<${#samples[@]}; id_sample++ ))
  do
  for (( id_ana=0; id_ana<${#analyses[@]}; id_ana++ ))
    do
    
    nchunks=$(ls ${1}/test_numbers_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root |wc -l)
    if [ $nchunks -gt 0 ]
    then
      hadd -f ${1}/test_numbers_${samples[id_sample]}/${analyses[id_ana]}OnDATA.root ${1}/test_numbers_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root 
      rm ${1}/test_numbers_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root 
    fi
    
  done
done

# nchunks=$(ls ${1}/test_numbers_DATA/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_DATA/WanalysisOnDATA.root ${1}/test_numbers_DATA/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_DATA/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_WJetsSig/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_WJetsSig/WanalysisOnDATA.root ${1}/test_numbers_WJetsSig/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_WJetsSig/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_WJetsFake/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_WJetsFake/WanalysisOnDATA.root ${1}/test_numbers_WJetsFake/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_WJetsFake/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_DYJetsSig/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_DYJetsSig/WanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_DYJetsSig/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_DYJetsFake/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_DYJetsFake/WanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_DYJetsFake/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_TTJets/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_TTJets/WanalysisOnDATA.root ${1}/test_numbers_TTJets/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_TTJets/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_ZZJets/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_ZZJets/WanalysisOnDATA.root ${1}/test_numbers_ZZJets/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_ZZJets/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_WZJets/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_WZJets/WanalysisOnDATA.root ${1}/test_numbers_WZJets/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_WZJets/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_WWJets/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_WWJets/WanalysisOnDATA.root ${1}/test_numbers_WWJets/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_WWJets/Wanalysis_chunk*.root 
# fi
# nchunks=$(ls ${1}/test_numbers_QCD/Wanalysis_chunk*.root |wc -l)
# if [ $nchunks -gt 0 ]
# then
  # hadd -f ${1}/test_numbers_QCD/WanalysisOnDATA.root ${1}/test_numbers_QCD/Wanalysis_chunk*.root 
  # rm ${1}/test_numbers_QCD/Wanalysis_chunk*.root 
# fi

echo 'EWK ONLY (EWK)'
# EWK ONLY
mkdir ${1}/test_numbers_EWK
echo 'Z analysis (W+Jets sig+fake, DY+Jets fake, ZZ, WZ, WW)'
hadd -f ${1}/test_numbers_EWK/ZanalysisOnDATA.root ${1}/test_numbers_WJetsSig/ZanalysisOnDATA.root ${1}/test_numbers_WJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_ZZJets/ZanalysisOnDATA.root ${1}/test_numbers_WZJets/ZanalysisOnDATA.root ${1}/test_numbers_WWJets/ZanalysisOnDATA.root 
echo 'W analysis (W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW)'
hadd -f ${1}/test_numbers_EWK/WanalysisOnDATA.root ${1}/test_numbers_WJetsFake/WanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/WanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/WanalysisOnDATA.root ${1}/test_numbers_ZZJets/WanalysisOnDATA.root ${1}/test_numbers_WZJets/WanalysisOnDATA.root ${1}/test_numbers_WWJets/WanalysisOnDATA.root 
echo 'PhiStarEta analysis (W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW)'
hadd -f ${1}/test_numbers_EWK/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WJetsSig/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WJetsFake/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_DYJetsFake/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_ZZJets/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WZJets/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WWJets/PhiStarEtaAnalysisOnDATA.root 

echo 'EWK + TT (EWKTT)'
# EWK + TT
mkdir ${1}/test_numbers_EWKTT
echo 'Z analysis'
hadd -f ${1}/test_numbers_EWKTT/ZanalysisOnDATA.root ${1}/test_numbers_EWK/ZanalysisOnDATA.root ${1}/test_numbers_TTJets/ZanalysisOnDATA.root
echo 'W analysis'
hadd -f ${1}/test_numbers_EWKTT/WanalysisOnDATA.root ${1}/test_numbers_EWK/WanalysisOnDATA.root ${1}/test_numbers_TTJets/WanalysisOnDATA.root
echo 'PhiStarEta analysis'
hadd -f ${1}/test_numbers_EWKTT/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_EWK/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_TTJets/PhiStarEtaAnalysisOnDATA.root

echo 'EWK + TT + SIG (MCDATALIKE)'
# EWK + TT + SIG
mkdir ${1}/test_numbers_MCDATALIKE
echo 'Z analysis'
hadd -f ${1}/test_numbers_MCDATALIKE/ZanalysisOnDATA.root ${1}/test_numbers_EWKTT/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/ZanalysisOnDATA.root
echo 'W analysis'
hadd -f ${1}/test_numbers_MCDATALIKE/WanalysisOnDATA.root ${1}/test_numbers_EWKTT/WanalysisOnDATA.root ${1}/test_numbers_WJetsSig/WanalysisOnDATA.root
echo 'PhiStarEta analysis'
hadd -f ${1}/test_numbers_MCDATALIKE/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_EWKTT/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_DYJetsSig/PhiStarEtaAnalysisOnDATA.root

########################################################
echo 'EWK ONLY (EWK)'
# EWK ONLY
# mkdir ${1}/test_numbers_EWK
echo 'Z analysis (W+Jets sig+fake, DY+Jets fake, ZZ, WZ, WW)'
hadd -f ${1}/test_numbers_EWK/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_WJetsSig/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_WJetsFake/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_DYJetsFake/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_ZZJets/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_WZJets/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_WWJets/Zanalysis_controlplotsOnDATA.root 
echo 'W analysis (W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW)'
hadd -f ${1}/test_numbers_EWK/WanalysisOnDATA.root ${1}/test_numbers_WJetsFake/WanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/WanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/WanalysisOnDATA.root ${1}/test_numbers_ZZJets/WanalysisOnDATA.root ${1}/test_numbers_WZJets/WanalysisOnDATA.root ${1}/test_numbers_WWJets/WanalysisOnDATA.root 
# echo 'PhiStarEta analysis (W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW)'
# hadd -f ${1}/test_numbers_EWK/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WJetsFake/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_DYJetsSig/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_DYJetsFake/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_ZZJets/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WZJets/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WWJets/PhiStarEtaAnalysisOnDATA.root 

echo 'EWK + TT (EWKTT)'
# EWK + TT
# mkdir ${1}/test_numbers_EWKTT
echo 'Z analysis'
hadd -f ${1}/test_numbers_EWKTT/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_EWK/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_TTJets/Zanalysis_controlplotsOnDATA.root
echo 'W analysis'
hadd -f ${1}/test_numbers_EWKTT/WanalysisOnDATA.root ${1}/test_numbers_EWK/WanalysisOnDATA.root ${1}/test_numbers_TTJets/WanalysisOnDATA.root
# echo 'PhiStarEta analysis'
# hadd -f ${1}/test_numbers_EWKTT/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_EWK/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_TTJets/PhiStarEtaAnalysisOnDATA.root

echo 'EWK + TT + SIG (MCDATALIKE)'
# EWK + TT + SIG
# mkdir ${1}/test_numbers_MCDATALIKE
echo 'Z analysis'
hadd -f ${1}/test_numbers_MCDATALIKE/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_EWKTT/Zanalysis_controlplotsOnDATA.root ${1}/test_numbers_DYJetsSig/Zanalysis_controlplotsOnDATA.root
echo 'W analysis'
hadd -f ${1}/test_numbers_MCDATALIKE/WanalysisOnDATA.root ${1}/test_numbers_EWKTT/WanalysisOnDATA.root ${1}/test_numbers_WJetsSig/WanalysisOnDATA.root
# echo 'PhiStarEta analysis'
# hadd -f ${1}/test_numbers_MCDATALIKE/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_EWKTT/PhiStarEtaAnalysisOnDATA.root ${1}/test_numbers_WJetsSig/PhiStarEtaAnalysisOnDATA.root

# hadd -f ${1}/test_numbers_EWK/ZanalysisOnDATA.root ${1}/test_numbers_WJetsSig/ZanalysisOnDATA.root ${1}/test_numbers_WJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsSig/ZanalysisOnDATA.root ${1}/test_numbers_DYJetsFake/ZanalysisOnDATA.root ${1}/test_numbers_TTJets/ZanalysisOnDATA.root ${1}/test_numbers_ZZJets/ZanalysisOnDATA.root ${1}/test_numbers_WZJets/ZanalysisOnDATA.root ${1}/test_numbers_WWJets/ZanalysisOnDATA.root 