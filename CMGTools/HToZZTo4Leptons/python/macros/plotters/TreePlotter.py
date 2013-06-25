import ROOT
import sys
from array import array
import pickle
from CMGTools.HToZZTo4Leptons.macros.plotters.PlotterBase import PlotterBase
from array import array

class TreePlotter(PlotterBase):

    def __init__(self,file,tree,weight = "1"):
        self.file = ROOT.TFile(file)
        self.tree = self.file.Get(tree)
        if tree ==0:
            print "Tree not found\n"
            sys.exit()
        self.weight=weight
        self.smooth=False
        super(TreePlotter,self).__init__()

    def setupFromFile(self,filename,finalstate = 'All',prod  = 'All'):
        f=open(filename)
        data=pickle.load(f)
        weightinv = float(data['events'])
        self.addCorrectionFactor("mcWeight",1./weightinv,0.0,'lnN')



            
    def applySmoothing(self):
        self.smooth=True

    def drawTH1(self,var,cuts,lumi,bins,min,max,titlex = "",units = "",drawStyle = "HIST"):
        h = ROOT.TH1D("tmpTH1","",bins,min,max)
        h.Sumw2()
        h.SetLineStyle(self.linestyle)
        h.SetLineColor(self.linecolor)
        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.SetMarkerStyle(self.markerstyle)
        h.GetXaxis().SetTitle(titlex+ " ["+units+"]")

        #Apply correction factors
        corrString='1'
        for corr in self.corrFactors:
                corrString = corrString+"*("+str(corr['value'])+")" 
        self.tree.Draw(var+">>tmpTH1","("+cuts+")*"+lumi+"*"+self.weight+"*("+corrString+")","goff")

        if self.smooth:
           integral=h.Integral()
           h=self.drawSmoothedTH1(var,cuts,bins,min,max)
           h.Scale(integral/h.Integral())
           h.SetLineStyle(self.linestyle)
           h.SetLineColor(self.linecolor)
           h.SetLineWidth(self.linewidth)
           h.SetFillStyle(self.fillstyle)
           h.SetFillColor(self.fillcolor)
           h.SetMarkerStyle(self.markerstyle)
           h.GetXaxis().SetTitle(titlex+ " ["+units+"]")



#        h.Draw(drawStyle)
        return h


    def drawTH2(self,var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,titlex = "",unitsx = "",titley="",unitsy="", drawStyle = "COLZ"):
        h = ROOT.TH2D("tmpTH2","",binsx,minx,maxx,binsy,miny,maxy)
        h.Sumw2()
#        h.SetLineStyle(self.linestyle)
#        h.SetLineColor(self.linecolor)
#        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
        h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")

        #Apply correction factors
        corrString='1'
        for corr in self.corrFactors:
                corrString = corrString+"*"+str(corr['value']) 
        self.tree.Draw(var+">>tmpTH2","("+cuts+")*"+lumi+"*"+self.weight+"*("+corrString+")","goff")

        if self.smooth:
           integral=h.Integral()
           h=self.drawSmoothedTH1(var,cuts,binsx,minx,maxx,binsy)
           h.Scale(integral/h.Integral())
           h.SetLineStyle(self.linestyle)
           h.SetLineColor(self.linecolor)
           h.SetLineWidth(self.linewidth)
           h.SetFillStyle(self.fillstyle)
           h.SetFillColor(self.fillcolor)
           h.SetMarkerStyle(self.markerstyle)
           h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
           h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")

        return h



    def drawTH3(self,var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,binsz,minz,maxz,titlex = "",unitsx = "",titley="",unitsy="", drawStyle = "COLZ"):
        h = ROOT.TH3D("tmpTH3","",binsx,minx,maxx,binsy,miny,maxy,binsz,minz,maxz)
        h.Sumw2()
#        h.SetLineStyle(self.linestyle)
#        h.SetLineColor(self.linecolor)
#        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
        h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")

        #Apply correction factors
        corrString='1'
        for corr in self.corrFactors:
                corrString = corrString+"*"+str(corr['value']) 
        self.tree.Draw(var+">>tmpTH3","("+cuts+")*"+lumi+"*"+self.weight+"*("+corrString+")","goff")
        return h



    def drawTH2Binned(self,var,cuts,lumi,binningx,binningy,titlex = "",unitsx = "",titley="",unitsy="", drawStyle = "COLZ"):
        h = ROOT.TH2D("tmpTH2","",len(binningx)-1,array('f',binningx),len(binningy)-1,array('f',binningy))
        h.Sumw2()
#        h.SetLineStyle(self.linestyle)
#        h.SetLineColor(self.linecolor)
#        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
        h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")

        #Apply correction factors
        corrString='1'
        for corr in self.corrFactors:
                corrString = corrString+"*"+str(corr['value']) 
        self.tree.Draw(var+">>tmpTH2","("+cuts+")*"+lumi+"*"+self.weight+"*("+corrString+")","goff")

        if self.smooth:
           integral=h.Integral()
           h=self.drawSmoothedTH1(var,cuts,binsx,minx,maxx,binsy)
           h.Scale(integral/h.Integral())
           h.SetLineStyle(self.linestyle)
           h.SetLineColor(self.linecolor)
           h.SetLineWidth(self.linewidth)
           h.SetFillStyle(self.fillstyle)
           h.SetFillColor(self.fillcolor)
           h.SetMarkerStyle(self.markerstyle)
           h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
           h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")

        return h

    def drawTH1Binned(self,var,cuts,lumi,binningx,titlex = "",unitsx = "", drawStyle = "COLZ"):
        h = ROOT.TH1D("tmpTH1","",len(binningx)-1,array('f',binningx))
        h.Sumw2()
#        h.SetLineStyle(self.linestyle)
#        h.SetLineColor(self.linecolor)
#        h.SetLineWidth(self.linewidth)
        h.SetFillStyle(self.fillstyle)
        h.SetFillColor(self.fillcolor)
        h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")


        #Apply correction factors
        corrString='1'
        for corr in self.corrFactors:
                corrString = corrString+"*"+str(corr['value']) 
        self.tree.Draw(var+">>tmpTH1","("+cuts+")*"+lumi+"*"+self.weight+"*("+corrString+")","goff")

        if self.smooth:
           integral=h.Integral()
           h=self.drawSmoothedTH1(var,cuts,binsx,minx,maxx,binsy)
           h.Scale(integral/h.Integral())
           h.SetLineStyle(self.linestyle)
           h.SetLineColor(self.linecolor)
           h.SetLineWidth(self.linewidth)
           h.SetFillStyle(self.fillstyle)
           h.SetFillColor(self.fillcolor)
           h.SetMarkerStyle(self.markerstyle)
           h.GetXaxis().SetTitle(titlex+ " ["+unitsx+"]")
           h.GetYaxis().SetTitle(titley+ " ["+unitsy+"]")

        return h



    def makeDataSetOld(self,var,cuts,mini = -1e+10,maxi = 1.0e+10,miny=-1e+10,maxy=1e+10,name='data',entries=-1):

        #Check if it is 2D
        variables = var.split(':')
        is2D = False
        if len(variables) == 2:
            is2D= True

        tmp = ROOT.TFile("___tmp___.root","recreate")
        tmp.cd()
        if entries>0:
            newTree = self.tree.CopyTree(cuts,'',entries)
        else:
            newTree = self.tree.CopyTree(cuts)

        weightVar = ROOT.RooRealVar('weight','weight',0.,1e+10)

        makeWeighted= len(self.corrFactors)>0
        


        if is2D:
            rooVar1 = ROOT.RooRealVar(variables[1],variables[1],mini,maxi)
            rooVar2 = ROOT.RooRealVar(variables[0],variables[0],miny,maxy)
            if makeWeighted:
                dataset = ROOT.RooDataSet(name,"data set",ROOT.RooArgSet(rooVar1,rooVar2,weightVar),"weight")
            else:
                dataset = ROOT.RooDataSet(name,"data set",ROOT.RooArgSet(rooVar1,rooVar2))
    
        else:
            rooVar = ROOT.RooRealVar(var,var,mini,maxi)

            if makeWeighted:
                dataset = ROOT.RooDataSet(name,"data set",ROOT.RooArgSet(rooVar,weightVar),"weight")
            else:    
                dataset = ROOT.RooDataSet(name,"data set",ROOT.RooArgSet(rooVar))
    
        for event in newTree:
            weight=1
            for w in self.corrFactors:
                if (type(w['value']) is float) or (type(w['value']) is int):
                    weight=weight*w['value']
                elif type(w['value']) is str:
                    weight=weight*getattr(event,w['value'])
            weightVar.setVal(weight)        
            if is2D:
                val1=getattr(event,variables[1])
                val2=getattr(event,variables[0])
                if val1>mini and val1<maxi and val2>miny and val2<maxy: 
                    rooVar1.setVal(val1)
                    rooVar2.setVal(val2)
                    if makeWeighted:
                        dataset.add(ROOT.RooArgSet(rooVar1,rooVar2,weightVar),weight)               
                    else:
                        dataset.add(ROOT.RooArgSet(rooVar1,rooVar2))
            else:    
                val=getattr(event,var)
                if val>mini and val<maxi:
                    rooVar.setVal(val)
                    if makeWeighted:
                        dataset.add(ROOT.RooArgSet(rooVar,weightVar),weight)
                    else:    
                        dataset.add(ROOT.RooArgSet(rooVar))

        tmp.Close()    
        observables=[]
        if is2D:
            observables=[rooVar1,rooVar2]
        else:    
            observables=[rooVar]
        return dataset,observables    



    def makeDataSet(self,var,cuts,mini=[],maxi=[],name='data',entries=-1):

        #Check if it is 2D
        variables = (var.split(':'))
        variables.reverse()
        
        tmp = ROOT.TFile("___tmp___.root","recreate")
        tmp.cd()
        if entries>0:
            newTree = self.tree.CopyTree(cuts,'',entries)
        else:
            newTree = self.tree.CopyTree(cuts)

        weightVar = ROOT.RooRealVar('weight','weight',0.,1e+10)

        makeWeighted= len(self.corrFactors)>0
        roovars=[]
        varSet = ROOT.RooArgSet('set')
        for var,minel,maxel in zip(variables,mini,maxi):
            roovars.append(ROOT.RooRealVar(var,var,minel,maxel))
        for v in roovars:
            varSet.add(v)
            
        if makeWeighted:
            dataset = ROOT.RooDataSet(name,"data set",varSet,"weight")
        else:
            dataset = ROOT.RooDataSet(name,"data set",varSet)


        for event in newTree:
            weight=1

            for w in self.corrFactors:
                if (type(w['value']) is float) or (type(w['value']) is int):
                    weight=weight*w['value']
                elif type(w['value']) is str:
                    weight=weight*getattr(event,w['value'])
            weightVar.setVal(weight)        

            fill=True
            for var,roovar,minel,maxel in zip(variables,roovars,mini,maxi):
                val=getattr(event,var)
                roovar.setVal(val)
                if val<minel or val>maxel:
                    fill=False
            if fill:        
                if makeWeighted:
                    dataset.add(varSet,weight)               
                else:
                    dataset.add(varSet)
        tmp.Close()    
        return dataset,roovars


    
    def drawEff1D(self,hD,hN,var,denom,num,titlex = "", units = ""):
        self.tree.Draw(var+">>denom","("+denom+")*"+self.weight,"goff")
        self.tree.Draw(var+">>num","("+denom+"&&"+num+")*"+self.weight,"goff")
        graph = ROOT.TGraphAsymmErrors();
        graph.Divide(hN,hD);
        graph.SetName('efficiency')
        graph.SetLineStyle(self.linestyle)
        graph.SetLineColor(self.linecolor)
        graph.SetLineWidth(self.linewidth)
        graph.SetFillStyle(self.fillstyle)
        graph.SetFillColor(self.fillcolor)
        graph.SetMarkerStyle(self.markerstyle)
        if len(units) >0 :
            graph.GetXaxis().SetTitle(titlex+ " ["+units+"]")
        else:
            graph.GetXaxis().SetTitle(titlex)
    
        graph.Draw("AP");
        return graph


    def drawEff2D(self,hD,hN,var,denom,num,titlex = "", unitsx = "",titley = "", unitsy = ""):
        self.tree.Draw(var+">>denom","("+denom+")*"+self.weight,"goff")
        self.tree.Draw(var+">>num","("+denom+"&&"+num+")*"+self.weight,"goff")

        hErrU = hD.Clone()
        hErrU.SetName("hErr")
        hErrD = hD.Clone()
        hErrD.SetName("hErr")

        hEff = hD.Clone()
        hEff.SetName("hEff")

        for i in range(1,hD.GetNbinsX() + 1):
            for j in range(1,hD.GetNbinsY() + 1):
                n=hN.GetBinContent(i,j)
                d=hD.GetBinContent(i,j)
                if n>0. and d>0.:
                    eff=n/d
                    errUp = ROOT.TEfficiency.ClopperPearson(int(d),int(n),0.68,True)
                    errDwn=ROOT.TEfficiency.ClopperPearson(int(d),int(n),0.68,False)

                    hEff.SetBinContent(i,j,eff)
                    hErrU.SetBinContent(i,j,errUp)
                    hErrD.SetBinContent(i,j,errDwn)
        hEff.Draw("COLZ");        
        hEff.GetXaxis().SetTitle(titlex+" ["+unitsx+"]")
        hErrU.GetXaxis().SetTitle(titlex+" ["+unitsx+"]")
        hErrD.GetXaxis().SetTitle(titlex+" ["+unitsx+"]")
        hEff.GetYaxis().SetTitle(titley+" ["+unitsy+"]")
        hErrU.GetYaxis().SetTitle(titley+" ["+unitsy+"]")
        hErrD.GetYaxis().SetTitle(titley+" ["+unitsy+"]")

        out=dict()
        out['eff']=hEff
        out['effUp']=hErrU
        out['effDwn']=hErrD
        return out
        



    def drawEfficiency(self,var,denom,num,bins,mini,maxi,titlex = "", units = ""):
        hD = ROOT.TH1D("denom","",bins,mini,maxi)
        hN = ROOT.TH1D("num","",bins,mini,maxi)
        hD.Sumw2()
        hN.Sumw2()
        return self.drawEff1D(hD,hN,var,denom,num,titlex,units)


    def drawEfficiency2D(self,var,denom,num,bins1,mini1,maxi1,bins2,mini2,maxi2,titlex = "", unitsx = "",titley = "", unitsy = ""):
        hD = ROOT.TH2D("denom","",bins1,mini1,maxi1,bins2,mini2,maxi2)
        hN = ROOT.TH2D("num","",bins1,mini1,maxi1,bins2,mini2,maxi2)
        hD.Sumw2()
        hN.Sumw2()
        return self.drawEff2D(hD,hN,var,denom,num,titlex,unitsx,titley,unitsy)


    def drawEfficiencyB(self,var,denom,num,binning,titlex = "", units = ""):
        hD = ROOT.TH1D("denom","",len(binning)-1,array('d',binning))
        hN = ROOT.TH1D("num","",len(binning)-1,array('d',binning))
        hD.Sumw2()
        hN.Sumw2()
        return self.drawEff1D(hD,hN,var,denom,num,titlex,units)

    def drawEfficiency2DB(self,var,denom,num,binning1,binning2,titlex = "", unitsx = "",titley = "", unitsy=""):
        hD = ROOT.TH2D("denom","",len(binning1)-1,array('d',binning1),len(binning2)-1,array('d',binning2))
        hN = ROOT.TH2D("num","",len(binning1)-1,array('d',binning1),len(binning2)-1,array('d',binning2))

        hD.Sumw2()
        hN.Sumw2()
        return self.drawEff2D(hD,hN,var,denom,num,titlex,unitsx,titley,unitsy)




def saveEfficiency(filename,dic):
    output=ROOT.TFile(filename,'recreate')
    output.cd()
    dic['eff'].SetName('eff')
    dic['effUp'].SetName('effUp')
    dic['effDwn'].SetName('effDwn')
    dic['eff'].Write()
    dic['effUp'].Write()
    dic['effDwn'].Write()
    output.Close()

        
