import FWCore.ParameterSet.Config as cms

# PU jet ID cuts

loosePUIDcuts = cms.VPSet(
    cms.PSet(
        minPt = cms.double(0.),
        maxPt = cms.double(20.),
        maxEtas = cms.vdouble(2.5, 2.75, 3.0, 5.0),
        minDiscs = cms.vdouble(-0.95 ,-0.96, -0.94, -0.95)
        ),
    cms.PSet(
        minPt = cms.double(20.),
        maxPt = cms.double(99999.), # WP 30-50 also valid above
        maxEtas = cms.vdouble(2.5, 2.75, 3.0, 5.0),
        minDiscs = cms.vdouble(-0.63, -0.60, -0.55, -0.45)
        )
)

# Can also take the WPs from the official file if it's up-to-date
from CMGTools.External.JetIdParams_cfi import full_53x_wp

mediumPUIDcuts = cms.VPSet(
    cms.PSet(
        minPt = cms.double(0.),
        maxPt = cms.double(20.),
        maxEtas = cms.vdouble(2.5, 2.75, 3.0, 5.0),
        minDiscs = full_53x_wp.Pt1020_Medium # Pt010 is identical
        ),
    cms.PSet(
        minPt = cms.double(20.),
        maxPt = cms.double(99999.), # WP 30-50 also valid above
        maxEtas = cms.vdouble(2.5, 2.75, 3.0, 5.0),
        minDiscs = full_53x_wp.Pt2030_Medium #Pt3050 is identical
        )
)



# PU jet ID selector

cmgPFJetPUIDSel = cms.EDFilter(
    'CMGJetPUIDSelector' ,
    src = cms.InputTag('cmgPFJetSel'),
    cut = cms.string(''), # dummy cut
    puIDName = cms.string('full53x'), # Name of the saved discriminator
    puJetIDParams = loosePUIDcuts
)
