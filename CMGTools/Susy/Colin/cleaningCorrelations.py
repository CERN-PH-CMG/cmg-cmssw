from cutStrings import *

cutCor = CutCorrelations('a')

cutCor.addCut( ('PBNR Failed', idNH90G95Failed) )
cutCor.addCut( ('PBNR Had Failed', idNH90Failed) )
cutCor.addCut( ('PBNR Phot Failed', idG95Failed) )
#cutCor.addCut( ('Beam Halo', beamHaloCSCTightFailed) )
#cutCor.addCut( ('Tracking failure', 'trackingFailure.obj==0') )
#cutCor.addCut( ('Bad EE RH recovery', 'badEERecov.obj==0') )
cutCor.addCut( ('delta phi', dPhi) )
cutCor.addCut( ('delta phi 0', dPhi0) )
cutCor.addCut( ('delta phi 12', dPhi12) )
# cutCor.draw()

# cutCor.addCut( ('TP Failed', ecalDeadTPFailed) )

# cutCor.addCut( {hbheNoise2011NonIsoFailed:hbheNoise2011NonIsoFailed} )
# cutCor.addCut( {hbheNoise2011IsoFailed:hbheNoise2011IsoFailed} )
# cutCor.addCut( {hbheNoise2010Failed:hbheNoise2010Failed} )
# cutCor.addCut( {muVetoFailed:muVetoFailed} )
# cutCor.addCut( {eleVetoFailed:eleVetoFailed} )

cutCor.printCuts()
# cutCor.fillCorrelationHisto( events, 'ht.obj[0].sumEt()>350 && mht.obj[0].et()>500')
# cutCor.draw()

mhtCuts = [200,400,600,800]

# mhtCuts = [300, 500, 700, 900]

for cut in mhtCuts:
    print 'mht cut ', cut
    cutCor.fillCorrelationHisto( events, 'ht.obj[0].sumEt()>350 && mht.obj[0].et()>%d' % cut)
    cutCor.draw()

