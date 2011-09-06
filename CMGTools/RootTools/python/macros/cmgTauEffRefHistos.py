import FWCore.ParameterSet.Config as cms

###Define the sample here and just add new lines for later productions
sampleName = "DYToTauTau_M_20_TuneZ2_7TeV_pythia6_tauola.Summer11_PU_S3_START42_V11_v2.AODSIM.V2.PAT_CMG_V2_0_1"
#############


processName = "CmgTauEffHistos"
process = cms.Process(processName)
#process.setName_(processName)
process.load("CMGTools.Common.sources."+sampleName+".source_cff")
process.TFileService = cms.Service("TFileService",fileName = cms.string("file:"+processName+"_"+sampleName+".root"))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


#############create generated taus and define histograms
process.genTau = cms.EDFilter("GenParticleSelector",
                         src = cms.InputTag( "genParticlesStatus3" ),
                         cut = cms.string(" abs( pdgId() ) == 15 "),
                         )

process.genTauHistograms = cms.EDAnalyzer(
    "GenParticleHistograms",
    inputCollection = cms.InputTag("genTau"),
    histograms = cms.untracked.PSet(

      pt = cms.untracked.VPSet(
        cms.untracked.PSet(
         var = cms.untracked.string('pt()'),
         low = cms.untracked.double(0),
         high = cms.untracked.double(200),
         nbins = cms.untracked.int32(200)
        )
      ),

      eta = cms.untracked.VPSet(
        cms.untracked.PSet(
         var = cms.untracked.string('eta()'),
         low = cms.untracked.double(-5),
         high = cms.untracked.double(5),
         nbins = cms.untracked.int32(100)
        )
      ),

      phi = cms.untracked.VPSet(
        cms.untracked.PSet(
         var = cms.untracked.string('phi()'),
         low = cms.untracked.double(-3.2),
         high = cms.untracked.double(3.2),
         nbins = cms.untracked.int32(64)
        )
      ),


    )
)

#############truth-match the cmgTaus and define histograms
process.cmgTauSelTrue = cms.EDFilter("CmgTauSelector",
                                     src = cms.InputTag( "cmgTauSel" ),
                                     cut = cms.string( " genJetp4().Pt() > 0.0" ))


process.cmgTauSelTrueHistogramsGen = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection =  cms.InputTag("cmgTauSelTrue"),
    histograms = cms.untracked.PSet(

      pt = cms.untracked.VPSet(
        cms.untracked.PSet(
         var = cms.untracked.string('genTaup4().pt()'),
         low = cms.untracked.double(0),
         high = cms.untracked.double(200),
         nbins = cms.untracked.int32(200)
        )
      ),

      eta = cms.untracked.VPSet(
        cms.untracked.PSet(
         var = cms.untracked.string('genTaup4().eta()'),
         low = cms.untracked.double(-5),
         high = cms.untracked.double(5),
         nbins = cms.untracked.int32(100)
        )
      ),

      phi = cms.untracked.VPSet(
        cms.untracked.PSet(
         var = cms.untracked.string('genTaup4().phi()'),
         low = cms.untracked.double(-3.2),
         high = cms.untracked.double(3.2),
         nbins = cms.untracked.int32(64)
        )
      ),


    )
)



#############generated taus by charge and define histograms
process.genTauPlus = process.genTau.clone()
process.genTauPlus.src = cms.InputTag( "genTau" )
process.genTauPlus.cut = cms.string(" abs(pdgId()) == 15 && charge()==1 ")
process.genTauPlusHistograms = process.genTauHistograms.clone()
process.genTauPlusHistograms.inputCollection = cms.InputTag("genTauPlus")

process.genTauMinus = process.genTau.clone()
process.genTauMinus.src = cms.InputTag( "genTau" )
process.genTauMinus.cut = cms.string(" abs(pdgId()) == 15 && charge()==-1 ")
process.genTauMinusHistograms = process.genTauHistograms.clone()
process.genTauMinusHistograms.inputCollection = cms.InputTag("genTauMinus")

#############truth-match cmgTaus by charge and define histograms
process.cmgTauSelPlusTrue = process.cmgTauSelTrue.clone()
process.cmgTauSelPlusTrue.src = cms.InputTag( "cmgTauSelTrue" )
process.cmgTauSelPlusTrue.cut = cms.string( " genJetp4().Pt() > 0.0  && charge() == 1" )
process.cmgTauSelPlusTrueHistogramsGen = process.cmgTauSelTrueHistogramsGen.clone()
process.cmgTauSelPlusTrueHistogramsGen.inputCollection =  cms.InputTag("cmgTauSelPlusTrue")

process.cmgTauSelMinusTrue = process.cmgTauSelTrue.clone()
process.cmgTauSelMinusTrue.src = cms.InputTag( "cmgTauSelTrue" )
process.cmgTauSelMinusTrue.cut = cms.string( " genJetp4().Pt() > 0.0  && charge() == -1" )
process.cmgTauSelMinusTrueHistogramsGen = process.cmgTauSelTrueHistogramsGen.clone()
process.cmgTauSelMinusTrueHistogramsGen.inputCollection =  cms.InputTag("cmgTauSelMinusTrue")



######
process.analysisSequence = cms.Sequence(
    process.genTau
    +process.genTauHistograms    
    +process.genTauPlus
    +process.genTauPlusHistograms
    +process.genTauMinus
    +process.genTauMinusHistograms
    
    +process.cmgTauSelTrue
    +process.cmgTauSelTrueHistogramsGen
    +process.cmgTauSelPlusTrue
    +process.cmgTauSelPlusTrueHistogramsGen
    +process.cmgTauSelMinusTrue
    +process.cmgTauSelMinusTrueHistogramsGen 
)
process.p = cms.Path(process.analysisSequence)






