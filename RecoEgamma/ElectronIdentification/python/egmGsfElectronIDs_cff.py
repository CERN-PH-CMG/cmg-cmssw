# Misc loads for VID framework
from RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi import *
from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry

# Load the producer module to build full 5x5 cluster shapes and whatever 
# else is needed for IDs
from RecoEgamma.ElectronIdentification.ElectronIDValueMapProducer_cfi import *

from RecoEgamma.ElectronIdentification.Identification.cutBasedElectronTrigID_CSA14_V0_cff \
import cutBasedElectronTrigIDCSA14V0
csa142012likeTrigeringSelectionv0 = central_id_registry.getMD5FromName(cutBasedElectronTrigIDCSA14V0.idName)
egmGsfElectronIDs.physicsObjectIDs.append(
                                     cms.PSet( idDefinition = cutBasedElectronTrigIDCSA14V0,
                                              idMD5 = cms.string(csa142012likeTrigeringSelectionv0) )
                                     )


egmGsfElectronIDSequence = cms.Sequence(electronIDValueMapProducer * egmGsfElectronIDs)
