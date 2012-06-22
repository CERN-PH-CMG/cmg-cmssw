import json
import os 

class YRParser(object):
    def __init__(self,jsonFile):
        f=open(jsonFile)
        self.dict=json.load(f)

    def get(self,mass):
        return  (item for item in self.dict if item["mH"] == mass).next()


yrparser7TeV = YRParser( '/'.join( [os.environ['CMSSW_BASE'],
                                    'src/CMGTools/RootTools/python/yellowreport/YR_7TeV.json']))
yrparser8TeV = YRParser( '/'.join( [os.environ['CMSSW_BASE'],
                                    'src/CMGTools/RootTools/python/yellowreport/YR_8TeV.json']))

if __name__ == '__main__':

    import pprint
    import sys
    mass = float(sys.argv[1])
    
    print 'mass', mass 
    #pprint.pprint( yrparser7TeV.get(mass) )
    print '7 TeV', yrparser7TeV.get(mass)['VBF']['sigma']
    #pprint.pprint( yrparser8TeV.get(mass) )
    print '8 TeV', yrparser8TeV.get(mass)['VBF']['sigma']
