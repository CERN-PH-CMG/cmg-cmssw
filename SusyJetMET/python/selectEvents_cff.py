
import FWCore.ParameterSet.Config as cms
import sys

def selectEvents( label, source ) :

    if label == 'Greedy':
        source.eventsToProcess = cms.untracked.VEventRange(
            '1:1319947',
            '1:415027', 
            '1:460640' 
            )

    elif label == 'METMinimizing':
        source.eventsToProcess = cms.untracked.VEventRange(
            '1:2012908',
            '1:1847671',
            '1:794179',
            '1:1645718',
            '1:404322',
            '1:1060074',
            '1:1647711',
            '1:1255239',
            '1:1655912',
            '1:2054772',
            '1:2114350',
            '1:2046360',
            '1:442111',
            '1:2122673',
            '1:530666',
            '1:656473',
            '1:1507963',
            '1:1370520',
            '1:477372',
            '1:667910',
            '1:1063607',
            '1:254794',
            '1:1090284'
            )

    elif label == 'Inconsistent':
        source.eventsToProcess = cms.untracked.VEventRange(
            '1:2012908', 
            '1:118978', 
            '1:1847671', 
            '1:794179', 
            '1:1645718', 
            '1:404322', 
            '1:1060074', 
            '1:1647711',
            '1:1255239', 
            '1:1655912', 
            '1:2054772', 
            '1:2114350', 
            '1:2046360', 
            '1:442111', 
            '1:2122673', 
            '1:530666', 
            '1:656473', 
            '1:1507963', 
            '1:1370520', 
            '1:477372', 
            '1:137134', 
            '1:1063607', 
            '1:254794', 
            '1:1090284' 
            )

    elif label == 'DeltaPt':
        source.eventsToProcess = cms.untracked.VEventRange(
            '1:2012908',
            '1:794179',
            '1:1645718',
            '1:404322',
            '1:1647711',
            '1:1655912',
            '1:2114350',
            '1:442111',
            '1:530666',
            '1:1507963',
            '1:254794',
            '1:1090284'
            )
    elif label == 'METCor1_200':
        source.eventsToProcess = cms.untracked.VEventRange(
            '1:133486',
            '1:510379',
            '1:697017',
            '1:835190',
            '1:872929',
            '1:327621',
            '1:791424',
            '1:829412',
            '1:538915',
            '1:367155',
            '1:308509',
            '1:484335',
            '1:155655',
            '1:91984',
            '1:1573468',
            '1:967690',
            '1:1529362',
            '1:1542308',

            )
        
    else:
        print label, "does not correspond to any list of events to select"
        sys.exit(1)

    print 'selecting events : '
    print source.eventsToProcess
