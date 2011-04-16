import FWCore.ParameterSet.Config as cms

from CMGTools.DiJetHighMass.myBaseJetSequence_cff import myBaseJetSequence
from CMGTools.DiJetHighMass.myPFJetSequence_cff import myPFJetSequence

def myJetSequence( process ): 

    postfixAK5PF = "AK5PF"
    postfixAK7PF = "AK7PF"
    postfixAK5Calo = "AK5Calo"
    postfixAK7Calo = "AK7Calo"

    myBaseJetSequence( process, postfixAK5PF )
    myBaseJetSequence( process, postfixAK7PF )
    myBaseJetSequence( process, postfixAK5Calo )
    myBaseJetSequence( process, postfixAK7Calo )
    myPFJetSequence( process, postfixAK5PF )
    myPFJetSequence( process, postfixAK7PF )


    process.jetSequence = cms.Sequence(
        getattr(process, "baseJetSequence" + postfixAK5Calo) +
        getattr(process, "baseJetSequence" + postfixAK7Calo) +
        getattr(process, "baseJetSequence" + postfixAK5PF) +
        getattr(process, "baseJetSequence" + postfixAK7PF) +
        getattr(process, "pfJetSequence" + postfixAK5PF) +
        getattr(process, "pfJetSequence" + postfixAK7PF)
        )
