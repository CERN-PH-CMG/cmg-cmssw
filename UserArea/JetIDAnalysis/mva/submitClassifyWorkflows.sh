workFlow=/afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh

bsub -o logs/baseNoPtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged baseNoPtCats tmva.json,tmva_noptcat.json
bsub -o logs/minimalNoPtCats.log -q 8nh $workFlow dRMean:nNeutrals:beta:betaStar:dZ:nCharged minimalNoPtCats tmva.json,tmva_noptcat.json

bsub -o logs/minimalPtCats.log -q 1nd $workFlow dRMean:nNeutrals:beta:betaStar:dZ:nCharged minimalPtCats tmva.json

## bsub -o logs/dRProf.log -q 8nh $workFlow frac01:frac02:frac03:frac04:frac05 dRProf tmva.json
## bsub -o logs/tkImpact.log -q 8nh $workFlow beta:betaStar:dZ tkImpact tmva_central.json BDT_tkImpact,Fisher_tkImpact
## bsub -o logs/mult.log -q 8nh $workFlow nNeutrals:nCharged mult tmva.json 

### bsub -o logs/baseLDPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged baseLDPtCats tmva_ld.json Fisher_basePtCats,Likelihood_basePtCats
### bsub -o logs/baseLDDPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged baseLDDPtCats tmva_ldd.json FisherD_basePtCats,LikelihoodD_basePtCats
### bsub -o logs/basePtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged basePtCats tmva.json



### bsub -o logs/baseNodRMeanPtCats.log -q 1nd $workFlow frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged baseNodRMeanPtCats tmva.json
### bsub -o logs/baseNodRProfPtCats.log -q 1nd $workFlow dRMean:nNeutrals:beta:betaStar:dZ:nCharged baseNodRProfPtCats tmva.json
### bsub -o logs/baseNoMultPtCats.log   -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:beta:betaStar:dZ baseNoMultPtCats tmva.json
### bsub -o logs/baseNoVtxCompPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:nCharged baseNoVtxCompPtCats tmva.json
### 
### bsub -o logs/basePlusPtPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:jetPt:beta:betaStar:dZ:nCharged basePlusPtPtCats tmva.json
### 
### bsub -o logs/basePtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:dZ:nCharged basePtCats tmva.json
### bsub -o logs/baseNoVtxCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:nvtx:beta:betaStar:dZ:nCharged baseNoVtxCat tmva_novtxcat.json
### bsub -o logs/fullNoVtxCat.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:dRLeadCent:dRLead2nd:leadFrac:secondFrac:ptD:minW:majW:nvtx:beta:betaStar:nCharged fullNoVtxCat tmva_novtxcat.json

### ## No pt categorization
### bsub -o logs/baseNoPtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:nNeutrals:beta:betaStar:nCharged baseNoPtCats tmva_noptcat.json
### 
### bsub -o logs/baseNoPtCatsFlatPt.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:nNeutrals:beta:betaStar:nCharged baseNoPtCatsFlatPt  tmva_noptcat.json,flatpt.json
### 
### bsub -o logs/baseNoPtCatsNowei.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:nNeutrals:beta:betaStar:nCharged baseNoPtCatsNowei tmva_noptcat.json,nowei.json
### 
### ## pt categorization (all categories) 
### bsub -o logs/basePtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:nCharged basePtCats tmva.json
### 
### bsub -o logs/basePtCatsFlatPt.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:nCharged basePtCatsFlatPt  tmva.json,flatpt.json
### 
### bsub -o logs/basePtCatsNowei.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nNeutrals:beta:betaStar:nCharged basePtCatsNowei  tmva.json,nowei.json
### 
### ## pt categorization + jetPt
### bsub -o logs/basePlusPtPtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:nNeutrals:beta:betaStar:nCharged basePlusPtPtCats tmva.json
### 
### bsub -o logs/basePlusPtPtCatsFlatPt.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:nNeutrals:beta:betaStar:nCharged basePlusPtPtCatsFlatPt  tmva.json,flatpt.json
### 
### bsub -o logs/basePlusPtPtCatsNowei.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:nNeutrals:beta:betaStar:nCharged basePlusPtPtCatsNowei  tmva.json,nowei.json
### 
### ## pt categorization + jetPtEta
## bsub -o logs/basePlusEtaPtCats.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:beta:betaStar:nCharged basePlusEtaPtCats tmva.json
### 
### bsub -o logs/basePlusEtaPtCatsFlatPt.log -q 1nd $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:beta:betaStar:nCharged basePlusEtaPtCatsFlatPt  tmva.json,flatpt.json
### 
### bsub -o logs/basePlusEtaPtCatsNowei.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:beta:betaStar:nCharged basePlusEtaPtCatsNowei  tmva.json,nowei.json
### 
### ## No pt categorization + jetEta
### bsub -o logs/basePlusEtaNoPtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:beta:betaStar:nCharged basePlusEtaNoPtCats tmva_noptcat.json
### 
### bsub -o logs/basePlusEtaNoPtCatsFlatPt.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:beta:betaStar:nCharged basePlusEtaNoPtCatsFlatPt  tmva_noptcat.json,flatpt.json
### 
### bsub -o logs/basePlusEtaNoPtCatsNowei.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:beta:betaStar:nCharged basePlusEtaNoPtCatsNowei tmva_noptcat.json,nowei.json
### 
### ## No pt categorization all variables
### bsub -o logs/fullPlusEtaNoPtCats.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:dRLeadCent:dRLead2nd:leadFrac:secondFrac:ptD:minW:majW:beta:betaStar:nCharged fullPlusEtaNoPtCats tmva_noptcat.json
### 
### bsub -o logs/fullPlusEtaNoPtCatsFlatPt.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:dRLeadCent:dRLead2nd:leadFrac:secondFrac:ptD:minW:majW:beta:betaStar:nCharged fullPlusEtaNoPtCatsFlatPt  tmva_noptcat.json,flatpt.json
### 
### bsub -o logs/fullPlusEtaNoPtCatsNowei.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:jetPt:jetEta:nNeutrals:dRLeadCent:dRLead2nd:leadFrac:secondFrac:ptD:minW:majW:beta:betaStar:nCharged fullPlusEtaNoPtCatsNowei tmva_noptcat.json,nowei.json

### Phase space scan

## bsub -o logs/dRdRProfMultBetaPtFlat.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:jetPt:beta:betaStar dRdRProfMultBetaPtFlat tmva_noptcat.json
## 
## bsub -o logs/dRdRProfMultBetaPtFlatFull.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:jetPt:beta:betaStar dRdRProfMultBetaPtFlatFull tmva.json
## 
## bsub -o logs/dRdRProfMultBetaPtFlatQuick.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:jetPt:beta:betaStar dRdRProfMultBetaPtFlatQuick tmva_quick.json
## 
## bsub -o logs/dRdRProfMultBetaPt.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:jetPt:beta:betaStar dRdRProfMultBetaPt tmva_noptcat.json
## 
## bsub -o logs/dRdRProfMultBetaFull.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:beta:betaStar dRdRProfMultBetaFull tmva_noptcat.json
## 
## bsub -o logs/dRdRProfMult.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles dRdRProfMult
## 
## bsub -o logs/dRdRProfMultdRLS.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:dRLeadCent:dRLead2nd dRdRProfMultdRLS
## 
## bsub -o logs/dRdRProfMultFractions4.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:leadFrac:secondFrac:thirdFrac:fourthFrac dRdRProfMultFractions4
## 
## bsub -o logs/dRdRProfMultFractions2.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:leadFrac:secondFrac dRdRProfMultFractions2
## 
## bsub -o logs/dRdRProfMultBeta.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:beta:betaStar dRdRProfMultBeta
## 
## bsub -o logs/dRdRProfMultIpar.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:d0:dZ dRdRProfMultIpar
## 
## bsub -o logs/dRleadSecond.log -q 8nh $workFlow dRLeadCent:dRLead2nd dRLeadSecond
## bsub -o logs/fractions2.log -q 8nh $workFlow leadFrac:secondFrac fractions2
## bsub -o logs/fractions4.log -q 8nh $workFlow leadFrac:secondFrac:thirdFrac:fourthFrac fractions4
## 
## bsub -o logs/dRLSecodRProf.log -q 8nh $workFlow dRLeadCent:dRLead2nd:frac01:frac02:frac03:frac04:frac05 dRLSdRProf
## 
## 
## bsub -o logs/majmindRProf.log -q 8nh $workFlow majW:minW:frac01:frac02:frac03:frac04:frac05 majmindRProf
## bsub -o logs/dRdRProf.log -q 8nh $workFlow dRMean:frac01:frac02:frac03:frac04:frac05 dRdRProf
## bsub -o logs/dRMult.log -q 8nh $workFlow dRMean:nParticles dRMult
