workFlow=$CMSSW_BASE/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh


###### bsub -o logs/chsFullPlusRMSPlusChNe.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:dRMeanNeut:dRMeanCh:nvtx:nNeutrals:beta:betaStar:dZ:nCharged chsFullPlusRMSPlusChNe tmva.json,tmva_twoptnovtxcat.json BDT_chsFullPlusRMSPlusChNe chspfjetanalyzer/tree
###### bsub -o logs/chsFullPlusRMS.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:nvtx:nNeutrals:beta:betaStar:dZ:nCharged chsFullPlusRMS tmva.json,tmva_twoptnovtxcat.json BDT_chsFullPlusRMS chspfjetanalyzer/tree
###### bsub -o logs/chsSimpleNoVtxCat.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:beta:betaStar chsSimpleNoVtxCat tmva.json,tmva_twoptnovtxcat.json BDT_chsSimpleNoVtxCat chspfjetanalyzer/tree
###### bsub -o logs/chsSimpleNoVtxCatChNe.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dRMeanNeut:dRMeanCh:nvtx:beta:betaStar chsSimpleNoVtxCatChNe tmva.json,tmva_twoptnovtxcat.json BDT_chsSimpleNoVtxCatChNe chspfjetanalyzer/tree


bsub -o logs/chsFullNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:nvtx:nNeutrals:beta:betaStar:dZ:nCharged chsFull tmva.json,tmva_twoptnovtxcat.json,nowei.json BDT_chsFullNoWei chspfjetanalyzer/tree
bsub -o logs/fullNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:nvtx:nNeutrals:beta:betaStar:dZ:nCharged fullNowei tmva.json,tmva_twoptnovtxcat.json,nowei.json

bsub -o logs/chsSimpleNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:beta:betaStar chsSimple tmva.json,tmva_twoptnovtxcat.json,nowei.json BDT_chsSimpleNoWei chspfjetanalyzer/tree
bsub -o logs/simpleNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:beta:betaStar simpleNowei tmva.json,tmva_twoptnovtxcat.json,nowei.json

bsub -o logs/chsShapeNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx chsShape tmva.json,tmva_twoptnovtxcat.json,nowei.json BDT_chsShapeNoWei chspfjetanalyzer/tree
bsub -o logs/shapeNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx shapeNowei tmva.json,tmva_twoptnovtxcat.json,nowei.json

bsub -o logs/chsShapePlusRMSNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx chsShapePlusRMSNowei tmva.json,tmva_twoptnovtxcat.json,nowei.json BDT_chsShapeNoWei chspfjetanalyzer/tree
bsub -o logs/shapePlusRMSNowei.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:nvtx shapePlusRMSNowei tmva.json,tmva_twoptnovtxcat.json,nowei.json

### bsub -o logs/fullPlusRMS.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:dR2Mean:nvtx:nNeutrals:beta:betaStar:dZ:nCharged fullPlusRMS tmva.json,tmva_twoptnovtxcat.json
### bsub -o logs/full.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:nNeutrals:beta:betaStar:dZ:nCharged full tmva.json,tmva_twoptnovtxcat.json
### bsub -o logs/simple.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:beta:betaStar simple tmva.json,tmva_twoptcat.json
### bsub -o logs/simpleNoVtxCat.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nvtx:beta:betaStar simpleNoVtxCat tmva.json,tmva_twoptnovtxcat.json

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
 
