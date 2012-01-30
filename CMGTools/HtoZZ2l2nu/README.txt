#
# Tags for 2011 run CMSSW_4_2_4
#
--- Tag ---    -------- Package --------                        
V2011          CMGTools/HtoZZ2l2nu                              
v11_09_28      CommonTools/ClusteredPFMetProducer               
HEAD           CommonTools/ParticleFlow                         
NoTag          Configuration/Skimming                           
V06-04-18      DataFormats/PatCandidates                        
pm20111022a    HiggsAnalysis/HiggsToGammaGamma                  
V00-4_2_X_patch2 HiggsAnalysis/HiggsToWW2Leptons                  
V06-4_2_X      HiggsAnalysis/HiggsToWW2e                        
V26Apr2011     JetMETAnalysis/simpleDRfilter                    
V00-4_1_X      MyAnalysis/IsolationTools                        
V08-06-38      PhysicsTools/PatAlgos                            
V00-05-21      PhysicsTools/PatExamples                         
V00-03-17      PhysicsTools/SelectorUtils                       
V08-03-14      PhysicsTools/Utilities                           
V01-03-11      RecoBTag/PerformanceDB                           
V02-04-17      RecoJets/Configuration                           
NoTag          RecoLuminosity/LumiDB                            
V00-00-07      RecoMET/METAnalyzers                             
Colin_June23   RecoParticleFlow/PostProcessing                  
V01-02-02      RecoTauTag/Configuration                         
V01-02-06      RecoTauTag/RecoTau                               
V01-02-00      RecoTauTag/TauTagTools                           
b4_2_x_2011Mar30 TauAnalysis/MCEmbeddingTools                     
edm-20May2011  WWAnalysis/DataFormats                           
WW_414_SKIM_V05 WWAnalysis/Filters                               
R42X_S1_V00_S2_V00_S3_V00 WWAnalysis/Tools                                 



#
# create ntuples
#
runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t W

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t TT

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t Z

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t DY

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t SingleT

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t May10

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t v4

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t Aug05

runOverSamples.py -j data/beauty-samples.json -p "-cfg=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/test/zzllvvCleanEventAndAnalysis_cfg.py -castor=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_2_4/src/CMGTools/HtoZZ2l2nu/data" -n 5 -s True -d patdir -t v6

#
# run the analysis
#
runLocalAnalysisOverSamples.py -e runOnEventSummary -j data/beauty-samples.json -o ~/scratch0/hzz/ -d /castor/cern.ch/cms/store/cmst3/user/querten/11_11_07_HtoZZ2l2nNTuples -c test/runAnalysis_cfg.py.templ -s 8nh 



#
# z+jets replacement
#

## generate unweigthed distributions
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples.json -d /castor/cern.ch/cms//store/cmst3/user/psilva/HZZ_11.11.24 -o ~/scratch0/gamma/  -c test/runAnalysis_cfg.py.templ -s 8nh

## generate plotter.root
runPlotter --iLumi 4616 --inDir ~/scratch0/gamma/ --outDir /tmp/psilva/ --json data/photon-samples.json

## use bin/G/getGammaTemplates.C to generate the weights for the sampe

## run again with weights
runLocalAnalysisOverSamples.py -e runGammaPlusJetsMETtemplates -j data/photon-samples.json -d /castor/cern.ch/cms//store/cmst3/user/psilva/HZZ_11.11.24 -o ~/scratch0/gamma/  -c test/runAnalysis_cfg.py.templ -s 8nh -c test/runAnalysis_cfg.py.templ -p "@weightsFile=/afs/cern.ch/user/p/psilva/public/GammaWeights/mc_gammaqtvsnvtxweight.root"


## run the replacement
runLocalAnalysisOverSamples.py -e runOnEventSummary -j data/samples-with-dy-replacement.json -o ~/scratch0/gamma-data/ -p "@weightsFile=/afs/cern.ch/user/p/psilva/public/GammaWeights/data_gammaptvsetaweight.root" -d /castor/cern.ch/cms/store/cmst3/user/querten/11_10_19_HtoZZ2l2nNTuples/ -c test/runAnalysis_cfg.py.templ -s True

## run the final plots
runPlotter --iLumi 4616 --inDir ~/scratch0/gamma-data/ --outDir /tmp/psilva/ --json data/samples-with-dy-replacement.json --only zmass
