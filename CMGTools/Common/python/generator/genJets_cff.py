from CMGTools.Common.factories.genJetFactory_cfi import *
from CMGTools.Common.skims.cmgPOSel_cfi import cmgPOSel

from CMGTools.Common.jet_cff import ptCut 
genJetSel = cmgPOSel.clone( src='genJet', cut=ptCut )

genJetsSequence = cms.Sequence(
    genJet
    + genJetSel
    )
    
