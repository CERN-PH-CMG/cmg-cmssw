import FWCore.ParameterSet.Config as cms

from CMGTools.DiJetHighMass.myBaseJetSequence_cff import myBaseJetSequence
from CMGTools.DiJetHighMass.myPFJetSequence_cff import myPFJetSequence
from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET

def myJetSequence( process ): 

    postfixAK5PF = "AK5PF"
    postfixAK7PF = "AK7PF"
    postfixAK7Calo = "AK7Calo"

    myBaseJetSequence( process, postfixAK7Calo )
    myPFJetSequence( process, postfixAK5PF )
    myPFJetSequence( process, postfixAK7PF )

    process.cmgPFMET = cmgBaseMET.clone()
    process.cmgPFMET.cfg.inputCollection = "patMETs"

    process.jetSequence = cms.Sequence(
        process.cmgPFMET +
        getattr(process, "baseJetSequence" + postfixAK7Calo) +
        getattr(process, "pfJetSequence" + postfixAK5PF) +
        getattr(process, "pfJetSequence" + postfixAK7PF)
        )
