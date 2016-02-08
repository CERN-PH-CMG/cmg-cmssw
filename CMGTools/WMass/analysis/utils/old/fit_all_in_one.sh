#!/bin/bash

source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh

# use_PForNoPUorTKmet=( 2 1 0 ) # 0:PF, 1:NOPU, 2:TK 
use_PForNoPUorTKmet=( 2 ) # 0:PF, 1:NOPU, 2:TK 

# MOMENTUM CORRECTION VARIATIONS
momcorr_scale_variations=( 0 )

# RECOIL CORRECTION VARIATIONS
useRecoilCorr=( 2 ) # 0: NO, 1: OLD, 2: PDFw3GAUS
# Recoil_U1resol_variations=( 0 )
# Recoil_U1scale_variations=( 1 -1 )

Recoil_U1scale_variations=( 0 )
Recoil_U1resol_variations=( 0 )
Recoil_U2resol_variations=( 0 )

zpt_scale_variations=( \
       -1
      )
       # 30 
       # 29 31 32 33 34 35 100 120 \
       # 1030 
       # 1029 1031 1032 1033 1034 1035 1036 1037\
       # 1329 1331 1332 1333 1334 1335 1336 1337\
       # 1529 1531 1532 1533 1534 1535 1536 1537\

# Recoil_U1resol_variations=( 0 1 -1 0  0 )
# Recoil_U1scale_variations=( 0 0  0 1 -1 )
# useRecoilCorr=( 1 ) # 1: YES, 0: NO

# infile_run='launch_analysis.py'
# infile_run_input='launch_analysis_testSplit_bkp_bash.py'
infile_run_input='launch_analysis_bash.py'

# echo ${#Recoil_U1resol_variations[@]}
usebatch=1 #  use batch submission for W/Z ANALYSIS
run_all_or_just_fit=( -1 ) #  1 = RUN ALL, -1 = W/Z ANALYSIS, -2 = MERGE, -3 = BACKGROUND FIT, -4 = DATACARDS, -5 = LIKELIHOOD FIT
resubmit=0
run_W_or_Z=1 #  0 = W,  1 = Z,  2 = both (W and Z)
fit_W_or_Z="Z" # "W" or "Z" or "W,Z"

for ((n=0; n < ${#run_all_or_just_fit[@]} ; n++));
    do
  for ((z=0; z < ${#zpt_scale_variations[@]} ; z++));
      do
    for ((j=0; j < ${#use_PForNoPUorTKmet[@]} ; j++));
        do
      for ((k=0; k < ${#useRecoilCorr[@]} ; k++));
          do
        for ((iu2=0; iu2 < ${#Recoil_U2resol_variations[@]} ; iu2++));
            do
          # for ((i=0; i < ${#Recoil_U1resol_variations[@]} ; i++));
              # do
            # for ((m=0; m < ${#Recoil_U1scale_variations[@]} ; m++));
                # do
              for ((h=0; h < ${#momcorr_scale_variations[@]} ; h++));
                  do
                  
                  if [ ${useRecoilCorr[${k}]} -eq 0 ] ; then
                    if [ ${Recoil_U2resol_variations[${iu2}]} -ne 0 ] || [ ${Recoil_U1resol_variations[${iu2}]} -ne 0 ] || [ ${Recoil_U1scale_variations[${iu2}]} -ne 0 ]; then
                      continue
                    fi
                  fi
                  
                  abs_run_all_or_just_fit=$(echo ${run_all_or_just_fit[${n}]} | tr -d -)
                  echo "zpt_scale_variations " ${zpt_scale_variations[${z}]} " (z="$z")"
                  echo "use_PForNoPUorTKmet " ${use_PForNoPUorTKmet[${j}]} " (j="$j")"
                  echo "useRecoilCorr " ${useRecoilCorr[${k}]} " (k="$k")"
                  echo "Recoil_U2resol_variations " ${Recoil_U2resol_variations[${iu2}]} " (iu2="$iu2")"
                  echo "Recoil_U1resol_variations " ${Recoil_U1resol_variations[${iu2}]} " (i="$i")"
                  echo "Recoil_U1scale_variations " ${Recoil_U1scale_variations[${iu2}]} " (m="$m")"
                  echo "momcorr_scale_variations " ${momcorr_scale_variations[${h}]} " (h="$h")"

                  infile_run='launch_analysis_testSplit_bash_use_met'$use_PForNoPUorTKmet'_U1scale_'${Recoil_U1scale_variations[${iu2}]}'_U1resol_'${Recoil_U1resol_variations[${iu2}]}'_U2resol_'${Recoil_U2resol_variations[${iu2}]}'_zpt_'${zpt_scale_variations[${z}]}'.py'

                  # echo ${Recoil_U1resol_variations[${iu2}]} ${Recoil_U1scale_variations[${iu2}]}; 
                  # for (resol,scale) in Recoil_U1scale_variations:
                  # echo "i="$i " Recoil_U1resol_variations " ${Recoil_U1resol_variations[${iu2}]} " Recoil_U1scale_variations "${Recoil_U1scale_variations[${iu2}]}
                  # echo "h="$h " momcorr_scale_variations " ${momcorr_scale_variations[${h}]}
                  cp $infile_run_input $infile_run

                  sed -i "s/.*fit_W_or_Z = \"/fit_W_or_Z = \"$fit_W_or_Z\" \# \"W\" or \"Z\"/g" $infile_run
                  sed -i "s/.*usePtSF = .*/usePtSF =  ${zpt_scale_variations[${z}]}\; \#\ Boson\ pT\ reweighting /g" $infile_run
                  sed -i "s/.*use_PForNoPUorTKmet = .*/use_PForNoPUorTKmet = ${use_PForNoPUorTKmet[${j}]}\ \#\ 0\:PF\,\ 1\:NOPU\,\ 2\:TK\;/g" $infile_run
                  sed -i "s/.*useRecoilCorr = .*/useRecoilCorr = ${useRecoilCorr[${k}]}\ \#\ 1\:YES\,\ 0\:NO/g" $infile_run
                  
                  sed -i "s/.*resubmit = .*/resubmit = ${resubmit}\; /g" $infile_run
                  
                  grep usePtSF\ = $infile_run
                  echo " "
                  
                  # continue
                  
                  ###################################
                  # echo '## RUN W AND Z ANALYSES ##'
                  # ###################################

                  sed -i "s/.*RecoilCorrResolutionNSigmaU1 =.*/RecoilCorrResolutionNSigmaU1 = \"${Recoil_U1resol_variations[${iu2}]}\";/" $infile_run
                  sed -i "s/.*RecoilCorrResolutionNSigmaU2 =.*/RecoilCorrResolutionNSigmaU2 = \"${Recoil_U2resol_variations[${iu2}]}\";/" $infile_run
                  sed -i "s/.*RecoilCorrScaleNSigmaU1 =.*/RecoilCorrScaleNSigmaU1 = \"${Recoil_U1scale_variations[${iu2}]}\";/" $infile_run
                  sed -i "s/.*GlobalSmearingRochCorrNsigma =.*/GlobalSmearingRochCorrNsigma = ${momcorr_scale_variations[${h}]};/" $infile_run
                  sed -i "s/.*useBatch =.*/useBatch = ${usebatch};/" $infile_run
                  
                  grep Recoil_U2resol_variations\ = $infile_run
                  grep RecoilCorrResolutionNSigmaU1\ = $infile_run
                  grep RecoilCorrScaleNSigmaU1\ = $infile_run
                  grep GlobalSmearingRochCorrNsigma\ = $infile_run
                  
                  # continue
                  
                  if [ $run_W_or_Z -eq 0 ]; then # run W
                    sed -i 's/runWanalysis = 0;/runWanalysis = 1;/g' $infile_run
                    sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  elif [ $run_W_or_Z -eq 1 ]; then # run Z
                    sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                    sed -i 's/runZanalysis = 0;/runZanalysis = 1;/g' $infile_run
                  else
                    sed -i 's/runWanalysis = 0;/runWanalysis = 1;/g' $infile_run
                    sed -i 's/runZanalysis = 0;/runZanalysis = 1;/g' $infile_run
                  fi
                  sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' $infile_run
                  sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 1;/runPrepareDataCardsFast = 0;/g' $infile_run
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 1;/runWSigBkgFit = 0;/g' $infile_run
                  if [ $abs_run_all_or_just_fit -eq 3 ]; then
                    sed -i 's/controlplots = 0;/controlplots = 1;/g' $infile_run
                  fi
                  
                  # if [ $abs_run_all_or_just_fit -le 1 ] || [ ${run_all_or_just_fit[${n}]} -eq -1 ]; then
                  if ([ $abs_run_all_or_just_fit -le 1 ] && [ ${run_all_or_just_fit[${n}]} -gt 0 ]) || [ ${run_all_or_just_fit[${n}]} -eq -1 ]; then
                    ###################################
                    echo '## 1) RUN W AND Z ANALYSES ##'
                    ###################################
                    python $infile_run
                  fi
                  
                  # TEMPORARY
                  # continue
                  
                  if [ ${#zpt_scale_variations[@]} -eq 1 ]; then
                    ##############################################################
                    echo '## CHECK IF RUN W AND Z ANALYSES ARE STILL RUNNING ##'
                    ##############################################################
                    still_running=1
                    while [ $still_running -eq 1 ]; do
                      if [[ $useBatch == 0 ]]; then
                        line=$(ps aux | grep $USER | grep analysis |grep root)
                        # echo $line
                        if [[ ! ${line} =~ "runWanalysis" ]]; then
                          if [[ ! ${line} =~ "runZanalysis" ]]; then
                            still_running=0
                        # if still_running==1: print 'runWanalysis or runZanalysis still running'
                        # else: print 'runWanalysis and runZanalysis not running anymore'
                          fi
                        else
                        # else: 
                          sleep 10
                          rm AnalysisCode/core.*
                          # print 'runWanalysis or runZanalysis still running',line
                        fi
                      else
                        line=$(bjobs | grep analysis |wc -l)
                        line2=$(bjobs | grep analysis |grep RUN|wc -l)
                        if [[ ${line} -eq 0 ]]; then
                          still_running=0
                        else
                          echo ${line}' jobs still pending, '${line2}' in RUN state'
                          sleep 20
                          rm AnalysisCode/core.*
                        fi
                      fi
                    done
                  fi

                  sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                  sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  sed -i 's/mergeSigEWKbkg = 0;/mergeSigEWKbkg = 1;/g' $infile_run
                  sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 1;/runPrepareDataCardsFast = 0;/g' $infile_run
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 1;/runWSigBkgFit = 0;/g' $infile_run
                  if ([ $abs_run_all_or_just_fit -le 2  ] && [  ${run_all_or_just_fit[${n}]} -gt 0 ]) || [ ${run_all_or_just_fit[${n}]} -eq -2 ]; then
                    ###################################
                    echo '## 2) MERGE EWK AND TT     ##'
                    ###################################
                    # echo "launching MERGING"
                    python $infile_run
                  fi

                    
                  sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                  sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' $infile_run
                  sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 1;/runPrepareDataCardsFast = 0;/g' $infile_run
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 0;/runWSigBkgFit = 1;/g' $infile_run
                  sed -i 's/controlplots = 0;/controlplots = 1;/g' $infile_run
                  # if [ $abs_run_all_or_just_fit -eq 3 ]; then
                  if [ ${abs_run_all_or_just_fit[${n}]} -eq 3 ]; then
                    ###################################
                    echo '## 3) FIT QCD BACKGROUND   ##'
                    ###################################
                    # echo "launching FIT QCD background"
                    python $infile_run
                  fi
                    
                  sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                  sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' $infile_run
                  sed -i 's/runR_WdivZ= 0;/runR_WdivZ= 1;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 1;/runPrepareDataCardsFast = 0;/g' $infile_run
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 1;/runWSigBkgFit = 0;/g' $infile_run
                  # ###################################
                  # echo '## RUN R = W div Z      ##'
                  # ###################################
                  # python $infile_run
                    
                  sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                  sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' $infile_run
                  sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 0;/run_BuildSimpleTemplates= 1;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 1;/runPrepareDataCardsFast = 0;/g' $infile_run
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 1;/runWSigBkgFit = 0;/g' $infile_run
                  # #########################################
                  # echo '## RUN BUILD SIMPLE TEMPLATES ##'
                  # #########################################
                  # python $infile_run

                  sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                  sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' $infile_run
                  sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 0;/runPrepareDataCardsFast = 1;/g' $infile_run
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 1;/runWSigBkgFit = 0;/g' $infile_run
                  # if [ $abs_run_all_or_just_fit -le 4 ] || [ ${run_all_or_just_fit[${n}]} -eq -4 ]; then
                  if ([ $abs_run_all_or_just_fit -le 4  ] && [  ${run_all_or_just_fit[${n}]} -gt 0 ]) || [ ${run_all_or_just_fit[${n}]} -eq -4 ]; then
                    ####################################
                    echo '## 4) RUN PREPARE DATACARDS ##'
                    ####################################
                    python $infile_run
                  fi
  # continue
                  sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' $infile_run
                  sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' $infile_run
                  sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' $infile_run
                  sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' $infile_run
                  sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' $infile_run
                  sed -i 's/runPrepareDataCardsFast = 1;/runPrepareDataCardsFast = 0;/g' $infile_run
                  
                  sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/g' $infile_run
                  sed -i 's/mergeResults = 1;/mergeResults = 0;/g' $infile_run
                  sed -i 's/runWSigBkgFit = 1;/runWSigBkgFit = 0;/g' $infile_run
                  # sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' $infile_run
                  # sed -i 's/mergeResults = 0;/mergeResults = 1;/g' $infile_run
                  
                  # if [ $abs_run_all_or_just_fit -le 5 ] || [ ${run_all_or_just_fit[${n}]} -eq -5 ]; then
                  if ([ $abs_run_all_or_just_fit -le 5  ] && [  ${run_all_or_just_fit[${n}]} -gt 0 ]) || [ ${run_all_or_just_fit[${n}]} -eq -5 ]; then
                    ###################################################
                    echo '## 5) RUN LIKELIHOOD FIT AND MERGE RESULTS ##'
                    ###################################################
                    python $infile_run
                  fi

                  echo " "
              done
            # done
          # done
        done
      done
    done
  done
done
