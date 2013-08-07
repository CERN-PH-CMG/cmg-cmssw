import FWCore.ParameterSet.Config as cms

#electron identification
def addCiCIDSelection(var):
    cicid = cms.PSet(
        selection = cms.string('test_bit(sourcePtr().electronID(\"%s\"),0)' % var)
    )
    return cicid
  
cicVeryLooseID  = addCiCIDSelection('eidVeryLoose')
cicLooseID      = addCiCIDSelection('eidLoose')
cicMediumID     = addCiCIDSelection('eidMedium')
cicTightID      = addCiCIDSelection('eidTight')
cicSuperTightID = addCiCIDSelection('eidSuperTight')

#conversion rejection
def addCiCCRSelection(var):
    ciccr = cms.PSet(
        selection = cms.string('test_bit(sourcePtr().electronID(\"%s\"),2)' % var)
    )
    return ciccr
  
cicVeryLooseCR  = addCiCCRSelection('eidVeryLoose')
cicLooseCR      = addCiCCRSelection('eidLoose')
cicMediumCR     = addCiCCRSelection('eidMedium')
cicTightCR      = addCiCCRSelection('eidTight')
cicSuperTightCR = addCiCCRSelection('eidSuperTight')

#ip cut
def addCiCIPSelection(var):
    cicip = cms.PSet(
        selection = cms.string('test_bit(sourcePtr().electronID(\"%s\"),3)' % var)
    )
    return cicip
  
cicVeryLooseIP  = addCiCIPSelection('eidVeryLoose')
cicLooseIP      = addCiCIPSelection('eidLoose')
cicMediumIP     = addCiCIPSelection('eidMedium')
cicTightIP      = addCiCIPSelection('eidTight')
cicSuperTightIP = addCiCIPSelection('eidSuperTight')

