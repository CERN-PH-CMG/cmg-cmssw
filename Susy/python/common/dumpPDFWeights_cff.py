import FWCore.ParameterSet.Config as cms

dumpPdfWeightsCTEQ = cms.EDProducer("PdfWeightProducer",
                                # Fix POWHEG if buggy (this PDF set will also appear on output,
                                # so only two more PDF sets can be added in PdfSetNames if not "")
                                #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                                GenTag = cms.untracked.InputTag("genParticles"),
                                PdfInfoTag = cms.untracked.InputTag("generator"),
                                PdfSetNames = cms.untracked.vstring("cteq66.LHgrid")
)

dumpPdfWeightsMRST = cms.EDProducer("PdfWeightProducer",
                                # Fix POWHEG if buggy (this PDF set will also appear on output,
                                # so only two more PDF sets can be added in PdfSetNames if not "")
                                #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                                GenTag = cms.untracked.InputTag("genParticles"),
                                PdfInfoTag = cms.untracked.InputTag("generator"),
                                PdfSetNames = cms.untracked.vstring("MRST2006nnlo.LHgrid")
)

dumpPdfWeights = cms.Sequence(dumpPdfWeightsCTEQ+dumpPdfWeightsMRST)
