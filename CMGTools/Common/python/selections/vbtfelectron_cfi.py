import FWCore.ParameterSet.Config as cms

#electron identification
def addVBTFIDSelection(var):
    vbtfid = cms.PSet(
        selection = cms.string('test_bit(sourcePtr().electronID(\"%s\"),0) ' % var)
    )
    return vbtfid
  
vbtfelectron60ID = addVBTFIDSelection('simpleEleId60relIso')
vbtfelectron70ID = addVBTFIDSelection('simpleEleId70relIso')
vbtfelectron80ID = addVBTFIDSelection('simpleEleId80relIso')
vbtfelectron90ID = addVBTFIDSelection('simpleEleId90relIso')
vbtfelectron95ID = addVBTFIDSelection('simpleEleId95relIso')

#conversion rejection
def addVBTFCRSelection(var):
    vbtfcr = cms.PSet(
        selection = cms.string('test_bit(sourcePtr().electronID(\"%s\"),2)' % var)
    )
    return vbtfcr
  
vbtfelectron60CR = addVBTFCRSelection('simpleEleId60relIso')
vbtfelectron70CR = addVBTFCRSelection('simpleEleId70relIso')
vbtfelectron80CR = addVBTFCRSelection('simpleEleId80relIso')
vbtfelectron90CR = addVBTFCRSelection('simpleEleId90relIso')
vbtfelectron95CR = addVBTFCRSelection('simpleEleId95relIso')

