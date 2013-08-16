import ROOT
import sys
from array import array
import pickle
import copy
from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import TreePlotter

class HZZFakeRatePlotter(TreePlotter):

    def __init__(self,file,tree,weight = "1"):
        super(HZZFakeRatePlotter,self).__init__(file,tree,weight)
        self.controlRegion='1'
        self.weight = "("+self.weight+")"
        self.factorizedRegion=''
        self.factor=1.0

    def setCR(self,cr):
        self.controlRegion = cr

    def setFactorizedRegion(self,region):
        self.factorizedRegion = region
    def applyFactorization(self,factor):
        self.factor = factor

    def setOSLS(self,mumu,elemu,muele,eleele):
        self.oslsMuMu = mumu
        self.oslsMuEle = muele
        self.oslsEleMu = muele
        self.oslsEleEle = eleele



    def setFakeRateVar(self,var,channel):
        self.addCorrectionFactor('CMS_hzz4l_'+channel+'_fakeRate',var,0.0,'lnN')
        self.fakeRateVar = var
        self.channel=channel

    def drawTH1(self,var,cuts,lumi,bins,mini,maxi,titlex = "",units = "",drawStyle = "HIST"):
        varReplaced=var.replace('H_','HLoose_') 

        factorizedRegion = self.factorizedRegion.replace('H_','HLoose_')

        cutsReplaced="("+cuts.replace('H_','HLoose_')+"&&"+self.controlRegion+")" 
        if self.factorizedRegion !='':
            cutsReplaced=cutsReplaced.replace(factorizedRegion,'') 


        #cache the smooth
        doSmooth = self.smooth

        self.smooth=False
        hControl = super(HZZFakeRatePlotter,self).drawTH1(varReplaced,'('+cutsReplaced+")/"+self.fakeRateVar,lumi,bins,mini,maxi,titlex,units,drawStyle)


        factor=self.factor
        if self.factorizedRegion != '':
            hFactorized = super(HZZFakeRatePlotter,self).drawTH1(varReplaced,'('+cutsReplaced+factorizedRegion+")/"+self.fakeRateVar,lumi,bins,mini,maxi,titlex,units,drawStyle)
#            factor = hFactorized.Integral()/hControl.Integral()
                
            print 'FACTOR is ',factor

        #Now add the OS/LS ratio
        cutsReplaced += "*((abs(HLoose_Z1_leg1_PdgId)==11&&abs(HLoose_Z2_leg1_PdgId)==11)*"+str(self.oslsEleEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)==11&&abs(HLoose_Z2_leg1_PdgId==13))*"+str(self.oslsEleMu) + \
            "+(abs(HLoose_Z1_leg1_PdgId)==13&&abs(HLoose_Z2_leg1_PdgId)==11)*"+str(self.oslsMuEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)==13&&abs(HLoose_Z2_leg1_PdgId)==13)*"+str(self.oslsMuMu)+ ")"

        hFakeUp =super(HZZFakeRatePlotter,self).drawTH1(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Up)')+"/"+self.fakeRateVar,lumi,bins,mini,maxi,titlex,units,drawStyle) 
        hFakeDown =super(HZZFakeRatePlotter,self).drawTH1(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Dwn)')+"/"+self.fakeRateVar,lumi,bins,mini,maxi,titlex,units,drawStyle) 


        self.smooth=doSmooth
        hFake =super(HZZFakeRatePlotter,self).drawTH1(varReplaced,cutsReplaced,"1",bins,mini,maxi,titlex,units,drawStyle) 

        extrapolationFactor = hFake.Integral()/hControl.Integral()
        extrapolationError = abs(hFakeUp.Integral()-hFakeDown.Integral())/hFake.Integral()
        #Set the values of the systematics
        for syst in self.corrFactors:
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRate':
                syst['error'] = extrapolationFactor

        hFake.Scale(self.factor)        
        return hFake


    def drawTH2(self,var,cuts,lumi,binsx,minix,maxix,binsy,miniy,maxiy,titlex = "",unitsx = "",titley = "",unitsy = "",drawStyle = "HIST"):
        varReplaced=var.replace('H_','HLoose_') 

        factorizedRegion = self.factorizedRegion.replace('H_','HLoose_')


        cutsReplaced="("+cuts.replace('H_','HLoose_')+"&&"+self.controlRegion+")" 

        if self.factorizedRegion !='':
            cutsReplaced=cutsReplaced.replace(factorizedRegion,'') 

        #cache the smooth
        doSmooth = self.smooth

        self.smooth=False
        hControl = super(HZZFakeRatePlotter,self).drawTH2(varReplaced,'('+cutsReplaced+")/"+self.fakeRateVar,"1",binsx,minix,maxix,binsy,miniy,maxiy,titlex,unitsx,titley,unitsy,drawStyle)


        factor = self.factor
        if self.factorizedRegion != '':
            hFactorized = super(HZZFakeRatePlotter,self).drawTH2(varReplaced,'('+cutsReplaced+factorizedRegion+")/"+self.fakeRateVar,'1',binsx,minix,maxix,binsy,miniy,maxiy,titlex,unitsx,titley,unitsy,drawStyle)
#            factor =hFactorized.Integral()/hControl.Integral() 

            print 'FACTOR is ',factor
            

        #Now add the OS/LS ratio
        cutsReplaced += "*((abs(HLoose_Z1_leg1_PdgId)==11&&abs(HLoose_Z2_leg1_PdgId)==11)*"+str(self.oslsEleEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)!=abs(HLoose_Z2_leg1_PdgId))*"+str(self.oslsMuEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)==13&&abs(HLoose_Z2_leg1_PdgId)==13)*"+str(self.oslsMuMu)+ ")"

        hFakeUp =super(HZZFakeRatePlotter,self).drawTH2(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Up)')+"/"+self.fakeRateVar,"1",binsx,minix,maxix,binsy,miniy,maxiy,titlex,unitsx,titley,unitsy,drawStyle) 
        hFakeDown =super(HZZFakeRatePlotter,self).drawTH2(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Dwn)')+"/"+self.fakeRateVar,"1",binsx,minix,maxix,binsy,miniy,maxiy,titlex,unitsx,titley,unitsy,drawStyle) 


        self.smooth=doSmooth
        hFake =super(HZZFakeRatePlotter,self).drawTH2(varReplaced,cutsReplaced,"1",binsx,minix,maxix,binsy,miniy,maxiy,titlex,unitsx,titley,unitsy,drawStyle) 

        extrapolationFactor = hFake.Integral()/hControl.Integral()
        extrapolationError = abs(hFakeUp.Integral()-hFakeDown.Integral())/hFake.Integral()
        #Set the values of the systematics
        for syst in self.corrFactors:
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRate':
                syst['error'] = 0.5
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRateStat':
                syst['count'] = hControl.Integral()
                syst['error'] = extrapolationFactor


        hFake.Scale(self.factor)        
        return hFake



    def drawTH3(self,var,cuts,lumi,binsx,minix,maxix,binsy,miniy,maxiy,binsz,minz,maxz,titlex = "",unitsx = "",titley = "",unitsy = "",drawStyle = "HIST"):
        varReplaced=var.replace('H_','HLoose_') 

        factorizedRegion = self.factorizedRegion.replace('H_','HLoose_')


        cutsReplaced="("+cuts.replace('H_','HLoose_')+"&&"+self.controlRegion+")" 

        if self.factorizedRegion !='':
            cutsReplaced=cutsReplaced.replace(factorizedRegion,'') 

        #cache the smooth
        doSmooth = self.smooth

        self.smooth=False
        hControl = super(HZZFakeRatePlotter,self).drawTH3(varReplaced,'('+cutsReplaced+")/"+self.fakeRateVar,"1",binsx,minix,maxix,binsy,miniy,maxiy,binsz,minz,maxz,titlex,unitsx,titley,unitsy,drawStyle)


        factor = self.factor
        if self.factorizedRegion != '':
            hFactorized = super(HZZFakeRatePlotter,self).drawTH3(varReplaced,'('+cutsReplaced+factorizedRegion+")/"+self.fakeRateVar,'1',binsx,minix,maxix,binsy,miniy,maxiy,binsz,minz,maxz,titlex,unitsx,titley,unitsy,drawStyle)
#            factor =hFactorized.Integral()/hControl.Integral() 

            print 'FACTOR is ',factor
            

        #Now add the OS/LS ratio
        cutsReplaced += "*((abs(HLoose_Z1_leg1_PdgId)==11&&abs(HLoose_Z2_leg1_PdgId)==11)*"+str(self.oslsEleEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)!=abs(HLoose_Z2_leg1_PdgId))*"+str(self.oslsMuEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)==13&&abs(HLoose_Z2_leg1_PdgId)==13)*"+str(self.oslsMuMu)+ ")"

        hFakeUp =super(HZZFakeRatePlotter,self).drawTH3(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Up)')+"/"+self.fakeRateVar,"1",binsx,minix,maxix,binsy,miniy,maxiy,binsz,minz,maxz,titlex,unitsx,titley,unitsy,drawStyle) 
        hFakeDown =super(HZZFakeRatePlotter,self).drawTH3(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Dwn)')+"/"+self.fakeRateVar,"1",binsx,minix,maxix,binsy,miniy,maxiy,binsz,minz,maxz,titlex,unitsx,titley,unitsy,drawStyle) 


        self.smooth=doSmooth
        hFake =super(HZZFakeRatePlotter,self).drawTH3(varReplaced,cutsReplaced,"1",binsx,minix,maxix,binsy,miniy,maxiy,binsz,minz,maxz,titlex,unitsx,titley,unitsy,drawStyle) 

        extrapolationFactor = hFake.Integral()/hControl.Integral()
        extrapolationError = abs(hFakeUp.Integral()-hFakeDown.Integral())/hFake.Integral()
        #Set the values of the systematics
        for syst in self.corrFactors:
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRate':
                syst['error'] = 0.5
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRateStat':
                syst['count'] = hControl.Integral()
                syst['error'] = extrapolationFactor


        hFake.Scale(self.factor)        
        return hFake




    def drawTH2Binned(self,var,cuts,lumi,binningx,binningy,titlex = "",unitsx = "",titley = "",unitsy = "",drawStyle = "HIST"):
        varReplaced=var.replace('H_','HLoose_') 

        factorizedRegion = self.factorizedRegion.replace('H_','HLoose_')


        cutsReplaced="("+cuts.replace('H_','HLoose_')+"&&"+self.controlRegion+")" 

        if self.factorizedRegion !='':
            cutsReplaced=cutsReplaced.replace(factorizedRegion,'') 

        #cache the smooth
        doSmooth = self.smooth

        self.smooth=False
        hControl = super(HZZFakeRatePlotter,self).drawTH2Binned(varReplaced,'('+cutsReplaced+")/"+self.fakeRateVar,"1",binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle)


        factor = self.factor
        if self.factorizedRegion != '':
            hFactorized = super(HZZFakeRatePlotter,self).drawTH2Binned(varReplaced,'('+cutsReplaced+factorizedRegion+")/"+self.fakeRateVar,'1',binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle)
#            factor =hFactorized.Integral()/hControl.Integral() 

            print 'FACTOR is ',factor
            

        #Now add the OS/LS ratio
        cutsReplaced += "*((abs(HLoose_Z1_leg1_PdgId)==11&&abs(HLoose_Z2_leg1_PdgId)==11)*"+str(self.oslsEleEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)!=abs(HLoose_Z2_leg1_PdgId))*"+str(self.oslsMuEle) + \
            "+(abs(HLoose_Z1_leg1_PdgId)==13&&abs(HLoose_Z2_leg1_PdgId)==13)*"+str(self.oslsMuMu)+ ")"

        hFakeUp =super(HZZFakeRatePlotter,self).drawTH2Binned(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Up)')+"/"+self.fakeRateVar,"1",binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle) 
        hFakeDown =super(HZZFakeRatePlotter,self).drawTH2Binned(varReplaced,'('+cutsReplaced+')*'+self.fakeRateVar.replace(')','Dwn)')+"/"+self.fakeRateVar,"1",binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle) 


        self.smooth=doSmooth
        hFake =super(HZZFakeRatePlotter,self).drawTH2Binned(varReplaced,cutsReplaced,"1",binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle) 

        extrapolationFactor = hFake.Integral()/hControl.Integral()
        extrapolationError = abs(hFakeUp.Integral()-hFakeDown.Integral())/hFake.Integral()
        #Set the values of the systematics
        for syst in self.corrFactors:
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRate':
                syst['error'] = 0.5
            if syst['name']=='CMS_hzz4l_'+self.channel+'_fakeRateStat':
                syst['count'] = hControl.Integral()
                syst['error'] = extrapolationFactor


        hFake.Scale(self.factor)        
        return hFake




    def makeDataSetOld(self,var,cuts,mini = 0,maxi = 1.0e+10,miny=0,maxy=1e+10,name='dataTmp',entries=-1):
        varReplaced=var.replace('H_','HLoose_') 
        cutsReplaced=cuts.replace('H_','HLoose_')+"&&"+self.controlRegion 

        return super(HZZFakeRatePlotter,self).makeDataSet(varReplaced,cutsReplaced,mini,maxi,miny,maxy,name,entries)

    def makeDataSet(self,var,cuts,mini,maxi,name='dataTmp',entries=-1):
        varReplaced=var.replace('H_','HLoose_') 
        cutsReplaced=cuts.replace('H_','HLoose_')+"&&"+self.controlRegion 

        return super(HZZFakeRatePlotter,self).makeDataSet(varReplaced,cutsReplaced,mini,maxi,name,entries)
