from ROOT import TLegend

import re

class Legend( TLegend ):
    def __init__(self, legendFile, objects):
        self.buildLegend(legendFile, objects)
        
    def buildLegend(self, legendFile, objects ):
        legendDesc = open( legendFile )
        FLOAT = '-*\d+[.\d*]*'
        pattmp = '\s*(FLOAT)\s+(FLOAT)\s+(FLOAT)\s+(FLOAT)\s*'
        pattern = re.compile( pattmp.replace('FLOAT', FLOAT) )
        legendEntries = []
        for line in legendDesc.readlines():
            line = line.rstrip()
            match = pattern.match( line )
            if match!=None:
                x1 = float(match.group(1))
                x2 = float(match.group(2))
                y1 = float(match.group(3))
                y2 = float(match.group(4))
            else:
                legendEntries.append( line )
        # print x1, x2, y1, y2, legendEntries
        if len(legendEntries)!= len(objects):
            raise ValueError('if you specify a legend file, it should have the correct number of lines.')
        
        TLegend.__init__( self, x1, x2, y1, y2)
        for entry,hist in zip(legendEntries,objects):
            self.AddEntry( hist, entry)
