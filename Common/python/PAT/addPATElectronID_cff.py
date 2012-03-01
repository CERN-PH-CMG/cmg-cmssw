import FWCore.ParameterSet.Config as cms

def addPATElectronID( process, sequenceName, postfix ):
    """
    Adds electron ID working points to PAT electrons
    """
    getattr(process, "patElectrons" + postfix).addElectronID = cms.bool(True)

    from CMGTools.Common.PAT.electronIDs_cfi import electronIDs
    getattr(process, "patElectrons" + postfix).electronIDSources  = electronIDs.clone()

    process.load('CMGTools.Common.PAT.patElectronID_cff')

##     getattr(process, "makePatElectrons" + postfix).replace(
##             getattr(process, "patElectrons" + postfix),
##             process.patElectronIDSequence  + getattr(process, "patElectrons" + postfix) 
##             )
    
    getattr(process, sequenceName + postfix).replace(
            getattr(process, "patElectrons" + postfix),
            process.patElectronIDSequence  + getattr(process, "patElectrons" + postfix) 
            )
    

##     if runOnMC :
##         getattr(process, "makePatElectrons" + postfix).replace(
##             getattr(process, "electronMatch" + postfix),
##             process.patElectronIDSequence  + getattr(process, "electronMatch" + postfix) 
##             )

##     else :
##         getattr(process, "patPF2PATSequence" + postfix ).replace(
##             getattr(process, "patDefaultSequence" + postfix),
##             process.patElectronIDSequence +  getattr(process, "patDefaultSequence" + postfix)
##             )

