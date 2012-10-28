import ROOT
import json
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath


class LineShapeWeights(object):
    def __init__(self,comp):
        f=open(getFullPath('data/lineshapeWeights.json'))
        data=json.load(f)
        name=comp.name
        self.method=lambda x: 1


        
        if comp.isMC: 
            if comp.dataset.find('7TeV')>-1:
                period='7TeV'
            else:    
                period='8TeV'
            mass=''
            for char in comp.name:
                if char in ['1','2','3','4','5','6','7','8','9','0']:
                    mass+=char
            if len(mass)>2:        
                if int(mass)>399 and mass in data[period]:
                    dictionary =data[period][mass]
                    self.g=ROOT.TGraph()
                    i=0
                    for key in sorted(dictionary,lambda x,y:int(float(x))):
                        self.g.SetPoint(i,float(key),float(dictionary[key]['final']))
                        i=i+1
                    self.method =self.getWeight
                    
                   
            
    def getWeight(self,x):
        return self.g.Eval(x)


    def applyWeight(self,event):
        if hasattr(event,'higgsGen'):
            event.eventWeight = event.eventWeight*self.method(event.higgsGen.mass())
