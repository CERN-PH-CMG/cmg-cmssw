import os 
from CMGTools.H2TauTau.macros.H2TauTauHistogramList import H2TauTauHistogramList
from CMGTools.H2TauTau.macros.Output import Output

def inclusiveRegionName(name):
    newName = name.split('_')[0:2]
    newName.append('Inclusive')
    return '_'.join( newName )


def wholeMTRegionName(name):
    newName = name.split('_')
    newName[0] = 'AllMT'
    return '_'.join( newName )
    

class H2TauTauOutput(Output):

    def __init__(self, name, regions ):
##         self.name = name
##         self._MakeOutputDir()
##         self.histoLists = {}
        super(H2TauTauOutput, self).__init__(name)

        inclusiveRegions = set()
        for regionName in regions.regionNames():
            self.histoLists[ regionName ] = H2TauTauHistogramList( '/'.join([self.name, regionName])) 
            incRegName = inclusiveRegionName( regionName )
            inclusiveRegions.add( incRegName )
        for regionName in inclusiveRegions:
            self.histoLists[ regionName ] = H2TauTauHistogramList( '/'.join([self.name, regionName ])) 
        wholeMTRegions = set()
        for regionName in self.histoLists.keys():
            wholeMTRegName = wholeMTRegionName( regionName )
            wholeMTRegions.add( wholeMTRegName )
        for regionName in wholeMTRegions:
            self.histoLists[ regionName ] = H2TauTauHistogramList( '/'.join([self.name, regionName ])) 
       
##     def _MakeOutputDir(self):
##         index = 0
##         name = self.name
##         while True:
##             try:
##                 # print 'mkdir', self.name
##                 os.mkdir( name )
##                 break
##             except OSError:
##                 index += 1
##                 name = '%s_%d' % (self.name, index)
            
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

##     def Write(self):
##         for histoList in self.histoLists.values():
##             histoList.Write()

