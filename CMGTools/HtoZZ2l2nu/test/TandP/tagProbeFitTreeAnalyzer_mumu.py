import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    

#fit models
twoVoigtiansModel = cms.vstring( "Voigtian::signalPass(mass, meanP[90,80,100], width[2.495], sigmaP[3,1,20])", ## allow different means and sigmas for
                                 "Voigtian::signalFail(mass, meanF[90,80,100], width[2.495], sigmaF[3,1,20])",    ## passing and failing probes
                                 "Exponential::backgroundPass(mass, lp[0,-5,5])",
                                 "Exponential::backgroundFail(mass, lf[0,-5,5])",
                                 "efficiency[0.9,0,1]",
                                 "signalFractionInPassing[0.9]"
                                 )
simpleGaussianModel = cms.vstring( "Gaussian::signal(mass, mean[91.2, 89.0, 93.0], sigma[2.3, 0.5, 10.0])",
                                   "RooExponential::backgroundPass(mass, cPass[-0.02,-5,0])",
                                   "RooExponential::backgroundFail(mass, cFail[-0.02,-5,0])",
                                   "efficiency[0.9,0,1]",
                                   "signalFractionInPassing[0.9]"    
                                   )


process.TagProbeFitTreeAnalyzer = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                                 InputFileNames = cms.vstring("TagAndProbe_ZMuMu.root"),
                                                 InputDirectoryName = cms.string("muonEffs"),
                                                 InputTreeName = cms.string("fitter_tree"),
                                                 OutputFileName = cms.string("TagAndProbe_ZMuMu_result.root"),
                                                 NumCPU = cms.uint32(1),
                                                 SaveWorkspace = cms.bool(True),
                                                 Variables = cms.PSet( mass = cms.vstring("Tag-Probe Mass", "76", "106", "GeV/c^{2}"),
                                                                       pt = cms.vstring("Probe p_{T}", "0", "200", "GeV/c"),
                                                                       ),
                                                 Categories = cms.PSet( passingMuon = cms.vstring("passingMuon", "dummy[true=1,false=0]"),
                                                                        passingKin = cms.vstring("passingKin", "dummy[true=1,false=0]"),
                                                                        passingTrk = cms.vstring("passingTrk", "dummy[true=1,false=0]"),
                                                                        passingIso = cms.vstring("passingIso", "dummy[true=1,false=0]")
                                                                        ),
                                                 PDFs = cms.PSet(  twoVoigtians = twoVoigtiansModel ),
                                                 Efficiencies = cms.PSet( passingMuon_pt = cms.PSet(  EfficiencyCategoryAndState = cms.vstring("passingMuon","true"),
                                                                                                      UnbinnedVariables = cms.vstring("mass"),
                                                                                                      BinnedVariables = cms.PSet( pt = cms.vdouble(20., 30., 50., 80., 120., 200.) ),
                                                                                                      BinToPDFmap = cms.vstring("twoVoigtians")
                                                                                                      ),
                                                                          passingKin_pt = cms.PSet(  EfficiencyCategoryAndState = cms.vstring("passingKin","true"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( pt = cms.vdouble(20.0, 30.0, 50., 80., 120., 200.0) ),
                                                                                                     BinToPDFmap = cms.vstring("twoVoigtians")
                                                                                                     ),
                                                                          passingTrk_pt = cms.PSet(  EfficiencyCategoryAndState = cms.vstring("passingTrk","true"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( pt = cms.vdouble(20.0, 30.0, 50., 80., 120., 200.0) ),
                                                                                                     BinToPDFmap = cms.vstring("twoVoigtians")
                                                                                                     ),
                                                                          passingIso_pt = cms.PSet(  EfficiencyCategoryAndState = cms.vstring("passingIso","true"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( pt = cms.vdouble(20.0, 30.0, 50., 80., 120., 200.0) ),
                                                                                                     BinToPDFmap = cms.vstring("twoVoigtians")
                                                                                                     )
                                                                          )
                                                 )
process.fitness = cms.Path( process.TagProbeFitTreeAnalyzer )


#  LocalWords:  twoVoigtiansModel
