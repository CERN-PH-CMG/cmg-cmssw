workFlow=/afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh

bsub -o logs/fullPlusRMS.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:nvtx:nNeutrals:beta:betaStar:dZ:nCharged fullPlusRMS tmva.json,tmva_twoptnovtxcat.json
bsub -o logs/full.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:nNeutrals:beta:betaStar:dZ:nCharged full tmva.json,tmva_twoptnovtxcat.json
bsub -o logs/simple.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:beta:betaStar simple tmva.json,tmva_twoptcat.json
bsub -o logs/simpleNoVtxCat.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:beta:betaStar simpleNoVtxCat tmva.json,tmva_twoptnovtxcat.json

### bsub -o logs/optimalNoVtxCatsTwoPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:nNeutrals:beta:betaStar:dZ:nCharged optimalNoVtxCatsTwoPtCats tmva.json,tmva_twoptnovtxcat.json
### bsub -o logs/optimalPtTwoPtCats.log -q 1nd $workFlow jetPt:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged optimalPtTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/optimalTwoPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged optimalTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/optimalPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged optimalPtCats tmva.json
### bsub -o logs/optimalNoPtCats.log -q 1nd $workFlow jetPt:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged optimalNoPtCats tmva.json,tmva_noptcat.json
### 
### bsub -o logs/safeTwoPtCats.log -q 1nd $workFlow dRMean:nNeutrals:beta:betaStar:dZ:nCharged safeTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/safePtTwoPtCats.log -q 1nd $workFlow jetPt:dRMean:nNeutrals:beta:betaStar:dZ:nCharged safePtTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/safeNoPtCats.log -q 8nh $workFlow dRMean:nNeutrals:beta:betaStar:dZ:nCharged safeNoPtCats tmva.json,tmva_noptcat.json
### bsub -o logs/safePtCats.log -q 1nd $workFlow dRMean:nNeutrals:beta:betaStar:dZ:nCharged safePtCats tmva.json
### bsub -o logs/safePtNoVtxCatsTwoPtCats.log -q 1nd $workFlow jetPt:dRMean:nNeutrals:beta:betaStar:dZ:nCharged safeNoVtxCatsPtTwoPtCats tmva.json,tmva_twoptnovtxcat.json
### 
### bsub -o logs/minimalPtTwoPtCats.log -q 1nd $workFlow jetPt:frac01:frac02:frac03:frac04:frac05:beta:betaStar minimalPtTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/minimalTwoPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:beta:betaStar minimalTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/minimalPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:beta:betaStar minimalPtCats tmva.json
### bsub -o logs/minimalNoPtCats.log -q 1nd $workFlow jetPt:frac01:frac02:frac03:frac04:frac05:beta:betaStar minimalNoPtCats tmva.json,tmva_noptcat.json
### bsub -o logs/minimaNoVtxCatslTwoPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:beta:betaStar minimalNoVtxCatsTwoPtCats tmva.json,tmva_twoptnovtxcat.json
### 
### bsub -o logs/fullNoPtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged fullNoPtCats tmva.json,tmva_noptcat.json
### bsub -o logs/fullTwoPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged fullTwoPtCats tmva.json,tmva_twoptcat.json
### bsub -o logs/fullPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged fullPtCats tmva.json
### 
### bsub -o logs/dRProf.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05 dRProf tmva.json
### bsub -o logs/tkImpact.log -q 8nh $workFlow beta:betaStar:dZ tkImpact tmva_central.json BDT_tkImpact,Fisher_tkImpact
### bsub -o logs/mult.log -q 1nd $workFlow nNeutrals:nCharged mult tmva.json 
 
