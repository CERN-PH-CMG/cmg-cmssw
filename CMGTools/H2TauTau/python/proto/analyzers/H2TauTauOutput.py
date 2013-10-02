import os 
from CMGTools.H2TauTau.proto.analyzers.H2TauTauHistogramList import TauMuHistogramList, TauEleHistogramList, MuEleHistogramList, TauTauHistogramList
from CMGTools.RootTools.fwlite.Output import Output

def inclusiveRegionName(name):
    '''Inclusive region name (wrt to jet categories)'''
    newName = name.split('_')[0:2]
    newName.append('Inclusive')
    return '_'.join( newName )


def wholeMTRegionName(name):
    newName = name.split('_')
    newName[0] = 'AllMT'
    return '_'.join( newName )
    

class H2TauTauOutput(Output):

    def __init__(self, name, regions, leg1Name, leg2Name):
        self.name = name
        super(H2TauTauOutput, self).__init__(name, 'w')

        # determine which type of histogram list we want to use,
        # depending on the specified di-tau channel
        # leg1Name = 'tau'
        # leg2Name = 'mu'
        classObject = None
        l1Name = leg1Name.split('_')[0]
        l2Name = leg2Name.split('_')[0]
        
        if l1Name == 'tau' and l2Name == 'mu':
            ClassObject = TauMuHistogramList
        elif l1Name == 'tau' and l2Name == 'ele':
            ClassObject = TauEleHistogramList
        elif l1Name == 'mu' and l2Name == 'ele':
            ClassObject = MuEleHistogramList
        elif l1Name == 'tau' and l2Name == 'tau':
            ClassObject = TauTauHistogramList
        
            
        inclusiveRegions = set()
        for regionName in regions.regionNames():
            self.histoLists[ regionName ] = ClassObject( '/'.join([self.name, regionName]), leg1Name, leg2Name ) 
            incRegName = inclusiveRegionName( regionName )
            inclusiveRegions.add( incRegName )
        for regionName in inclusiveRegions:
            self.histoLists[ regionName ] = ClassObject( '/'.join([self.name, regionName ]), leg1Name, leg2Name ) 
        wholeMTRegions = set()
        for regionName in self.histoLists.keys():
            wholeMTRegName = wholeMTRegionName( regionName )
            wholeMTRegions.add( wholeMTRegName )
        for regionName in wholeMTRegions:
            self.histoLists[ regionName ] = ClassObject( '/'.join([self.name, regionName ]), leg1Name, leg2Name ) 
       
            
    def Fill(self, event, regionName ):
        histoList = self.histoLists[regionName]
        histoList.Fill( event, event.eventWeight )
        # inclusive analysis
        incRegionName = inclusiveRegionName( regionName )
        histoList = self.histoLists[incRegionName]
        histoList.Fill( event, event.eventWeight )
        # whole MT
        wholeMTRegName = wholeMTRegionName( regionName )
        histoList = self.histoLists[wholeMTRegName]
        histoList.Fill( event, event.eventWeight )
        # whole MT, inclusive
        wholeMTRegNameInclusive = wholeMTRegionName( incRegionName )
        histoList = self.histoLists[wholeMTRegNameInclusive]
        histoList.Fill( event, event.eventWeight )

