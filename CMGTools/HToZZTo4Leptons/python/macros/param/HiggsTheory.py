import ROOT
import json
import array

ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")


class HiggsTheory(object):
    def __init__(self,jsonFile):
        self.productions=['GGH','VBF','WH','ZH','TTH']
        self.prodProperties=['sigma','pdfUp','pdfDown','scaleUp','scaleDown']

        self.decays=['H4F']
        self.decayProperties=['llllt','eeee','eemumu']
        self.decays2=['H2B']
        self.decayProperties2=['ZZ']

        f=open(jsonFile)
        self.dict=json.load(f)

        self.param=dict() 
        self.makeHZZParam()
        
    def getSpline(self,name,var,arg1,arg2 = None):
        inp =self.fillArray(arg1,arg2)
        x=array.array('d')
        y=array.array('d')
        for i,j in zip(inp[0],inp[1]):
            x.append(i)
            y.append(j)


        spline=ROOT.RooSpline1D(name, name, var, len(x), x, y)
        return spline

        

    def makeHZZParam(self):
        self.MH = ROOT.RooRealVar("MH","MH",125,100,800)
        for production in self.productions:
            self.param[production] = dict()
            for property in self.prodProperties:
                self.param[production][property] = self.getSpline(production+'_'+property,self.MH,production,property)
        for decay in self.decays:
            self.param[decay] = dict()
            for property in self.decayProperties:
                self.param[decay][property] = self.getSpline(decay+'_'+property,self.MH,decay,property)
        for decay in self.decays2:
            self.param[decay] = dict()
            for property in self.decayProperties2:
                self.param[decay][property] = self.getSpline(decay+'_'+property,self.MH,decay,property)
                
        self.param['width'] = self.getSpline('width',self.MH,'width')

    def getParam(self,arg1,arg2 = None):
        if arg2 is None:
            return self.param[arg1]
        else:
            return self.para,[arg2]


        
    def getInterp(self,mass):
        out=dict()
        out['mH']=mass
        self.MH.setVal(mass)

        for production in self.productions:
            out[production] = dict()
            for property in self.prodProperties:
                out[production][property] = self.param[production][property].getVal()
        for decay in self.decays:
            out[decay] = dict()
            for property in self.decayProperties:
                out[decay][property] = self.param[decay][property].getVal()
                
        out['width'] = self.param['width'].getVal()
        return out


    def fillArray(self,arg1,arg2 = None):
        out=dict()
        masses=[]
        for item in self.dict:
            if arg2 is None:
                if arg1 in item:
                    masses.append(item['mH'])
                    out[item['mH']]=item[arg1]
            else:    
                if arg1 in item:
                    if arg2 in item[arg1]:
                        out[item['mH']]=item[arg1][arg2]
                        masses.append(item['mH'])
        masses.sort()
        values=[]
        for m in masses:
            values.append(out[m])
        return [masses,values]    
                        

        
    def get(self,mass):
      return  (item for item in self.dict if item["mH"] == mass).next()
