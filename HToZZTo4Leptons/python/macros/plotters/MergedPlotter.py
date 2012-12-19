import ROOT
import sys
from array import array
import pickle
from CMGTools.HToZZTo4Leptons.macros.plotters.PlotterBase import PlotterBase
class MergedPlotter(PlotterBase):

    def __init__(self,plotters):
        super(MergedPlotter,self).__init__()
        self.plotters=plotters
        self.corrFactors=plotters[0].corrFactors

    def applySmoothing(self):
        for plotter in self.plotters:
            plotter.applySmoothing()


    def drawTH1(self,var,cuts,lumi,bins,min,max,titlex = "",units = "",drawStyle = "HIST"):
        h=None
        for plotter in self.plotters:
            if h is None:
                h=plotter.drawTH1(var,cuts,lumi,bins,min,max,titlex,units,drawStyle)
            else:
                h.Add(plotter.drawTH1(var,cuts,lumi,bins,min,max,titlex,units,drawStyle))
        h.SetLineColor(self.linecolor)
        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.SetMarkerStyle(self.markerstyle)
        h.GetXaxis().SetTitle(titlex+ " ["+units+"]")
        return h

    def drawTH2(self,var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,titlex = "",unitsx = "",titley = "",unitsy = "",drawStyle = "COLZ"):
        h=None
        for plotter in self.plotters:
            if h is None:
                h=plotter.drawTH2(var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,titlex,unitsx,titley,unitsy,drawStyle)
            else:
                h.Add(plotter.drawTH2(var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,titlex,unitsx,titley,unitsy,drawStyle))

#        h.SetLineStyle(self.linestyle)
#        h.SetLineColor(self.linecolor)
#        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.SetMarkerStyle(self.markerstyle)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
        h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")
        return h

    def drawTH2Binned(self,var,cuts,lumi,binningx,binningy,titlex = "",unitsx = "",titley = "",unitsy = "",drawStyle = "COLZ"):
        h=None
        for plotter in self.plotters:
            if h is None:
                h=plotter.drawTH2Binned(var,cuts,lumi,binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle)
            else:
                h.Add(plotter.drawTH2Binned(var,cuts,lumi,binningx,binningy,titlex,unitsx,titley,unitsy,drawStyle))

#        h.SetLineStyle(self.linestyle)
#        h.SetLineColor(self.linecolor)
#        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.SetMarkerStyle(self.markerstyle)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
        h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")
        return h

    def drawTH1Binned(self,var,cuts,lumi,binningx,titlex = "",unitsx = "",drawStyle = "COLZ"):
        h=None
        for plotter in self.plotters:
            if h is None:
                h=plotter.drawTH1Binned(var,cuts,lumi,binningx,titlex,unitsx,drawStyle)
            else:
                h.Add(plotter.drawTH1Binned(var,cuts,lumi,binningx,titlex,unitsx,drawStyle))
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.SetMarkerStyle(self.markerstyle)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
        return h


    
    def makeDataSetOld(self,var,cuts,mini = 0,maxi = 1.0e+10,miny=0,maxy=1e+10,name = "data",entries=-1):
        dataset=None
        observables=[]
        for i,plotter in enumerate(self.plotters):
            if dataset is None:
                dataset,observables = plotter.makeDataSet(var,cuts,mini,maxi,miny,maxy,name,entries)
            else:
                dataNew,observablesNew = plotter.makeDataSet(var,cuts,mini,maxi,miny,maxy,name+str(i),entries)
                dataset.append(dataNew)
        
        return dataset,observables    

    def makeDataSet(self,var,cuts,mini,maxi,name = "data",entries=-1):
        dataset=None
        observables=[]
        for i,plotter in enumerate(self.plotters):
            if dataset is None:
                dataset,observables = plotter.makeDataSet(var,cuts,mini,maxi,name,entries)
            else:
                dataNew,observablesNew = plotter.makeDataSet(var,cuts,mini,maxi,name+str(i),entries)
                dataset.append(dataNew)
        
        return dataset,observables    
