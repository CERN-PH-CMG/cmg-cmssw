import FWCore.ParameterSet.Config as cms

def addVBTFSelection(var):
    vbtf = cms.PSet(
                    #isEcalDriven = cms.string('sourcePtr().ecalDrivenSeed()'),  #requires gsf core to be saved
                    numberOfInnerTrackerHitsLost = cms.string('sourcePtr().gsfTrack().isNonnull() && sourcePtr().gsfTrack().trackerExpectedHitsInner().numberOfLostHits()<2'),
                    conversionVeto = cms.string('sourcePtr().electronID(\"%s\")>4' % var),
                    eid = cms.string('sourcePtr().electronID(\"%s\")==1 || sourcePtr().electronID(\"%s\")==3 || sourcePtr().electronID(\"%s\")==5' % (var,var,var))
    )
    return vbtf
    
vbtfelectron60 = addVBTFSelection('simpleEleId60relIso')
vbtfelectron70 = addVBTFSelection('simpleEleId70relIso')
vbtfelectron80 = addVBTFSelection('simpleEleId80relIso')
vbtfelectron90 = addVBTFSelection('simpleEleId90relIso')
vbtfelectron95 = addVBTFSelection('simpleEleId95relIso')
