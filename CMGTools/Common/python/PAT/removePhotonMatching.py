import FWCore.ParameterSet.Config as cms

def removePhotonMatching( process, postfix ):
    getattr(process,'patDefaultSequence'+postfix).remove( getattr(process,'photonMatch'+postfix) ) 
