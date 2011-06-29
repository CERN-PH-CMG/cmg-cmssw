import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    

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
                                                 Categories = cms.PSet( passingGlb = cms.vstring("passingGlb", "dummy[true=1,false=0]"),
                                                                        passingIso = cms.vstring("passingIso", "dummy[true=1,false=0]"),
                                                                        ),
                                                 PDFs = cms.PSet(  gaussPlusExponential = cms.vstring( "Gaussian::signal(mass, mean[91.2, 89.0, 93.0], sigma[2.3, 0.5, 10.0])",
                                                                                                       "RooExponential::backgroundPass(mass, cPass[-0.02,-5,0])",
                                                                                                       "RooExponential::backgroundFail(mass, cFail[-0.02,-5,0])",
                                                                                                       "efficiency[0.9,0,1]",
                                                                                                       "signalFractionInPassing[0.9]"
                                                                                                       )
                                                                   ),
                                                 Efficiencies = cms.PSet( passingGlb_pt = cms.PSet(  EfficiencyCategoryAndState = cms.vstring("passingGlb","true"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( pt = cms.vdouble(20.0, 100.0,200.0) ),
                                                                                                     BinToPDFmap = cms.vstring("gaussPlusExponential")
                                                                                                     ),
                                                                          passingIso_pt = cms.PSet(  EfficiencyCategoryAndState = cms.vstring("passingIso","true"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( pt = cms.vdouble(20.0, 100.0,200.0) ),
                                                                                                     BinToPDFmap = cms.vstring("gaussPlusExponential")
                                                                                                     )
                                                                          )
                                                 )
process.fitness = cms.Path( process.TagProbeFitTreeAnalyzer )

