bsub -o logs/dRdRProfMultBetaFull.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:beta:betaStar dRdRProfMultBetaFull tmva.json

bsub -o logs/dRdRProfMult.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles dRdRProfMult

bsub -o logs/dRdRProfMultdRLS.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:dRLeadCent:dRLead2nd dRdRProfMultdRLS

bsub -o logs/dRdRProfMultFractions4.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:leadFrac:secondFrac:thirdFrac:fourthFrac dRdRProfMultFractions4

bsub -o logs/dRdRProfMultFractions2.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:leadFrac:secondFrac dRdRProfMultFractions2

bsub -o logs/dRdRProfMultBeta.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:beta:betaStar dRdRProfMultBeta

bsub -o logs/dRdRProfMultIpar.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05:nParticles:d0:dZ dRdRProfMultIpar

### bsub -o logs/dRleadSecond.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRLeadCent:dRLead2nd dRLeadSecond
### bsub -o logs/fractions2.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh leadFrac:secondFrac fractions2
### bsub -o logs/fractions4.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh leadFrac:secondFrac:thirdFrac:fourthFrac fractions4
### 
### bsub -o logs/dRLSecodRProf.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRLeadCent:dRLead2nd:frac01:frac02:frac03:frac04:frac05 dRLSdRProf


### bsub -o logs/majmindRProf.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh majW:minW:frac01:frac02:frac03:frac04:frac05 majmindRProf
### bsub -o logs/dRdRProf.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:frac01:frac02:frac03:frac04:frac05 dRdRProf
### bsub -o logs/dRMult.log -q 8nh /afs/cern.ch/user/m/musella/Analysis/CMGTools/CMSSW_4_4_3/src/CMG/JetIDAnalysis/mva/classifyWorkFlow.sh dRMean:nParticles dRMult
