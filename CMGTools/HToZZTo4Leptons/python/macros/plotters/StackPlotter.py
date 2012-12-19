import ROOT
from TreePlotter import TreePlotter




def convertToPoisson(h):
    graph = ROOT.TGraphAsymmErrors()
    q = (1-0.6827)/2.

    for i in range(1,h.GetNbinsX()+1):
        x=h.GetXaxis().GetBinCenter(i)
        xLow =h.GetXaxis().GetBinLowEdge(i) 
        xHigh =h.GetXaxis().GetBinUpEdge(i) 
        y=h.GetBinContent(i)
        yLow=0
        yHigh=0
        if y !=0.0:
            yLow = y-ROOT.Math.chisquared_quantile_c(1-q,2*y)/2.
            yHigh = ROOT.Math.chisquared_quantile_c(q,2*(y+1))/2.-y
            graph.SetPoint(i-1,x,y)
            graph.SetPointEYlow(i-1,yLow)
            graph.SetPointEYhigh(i-1,yHigh)
            graph.SetPointEXlow(i-1,x-xLow)
            graph.SetPointEXhigh(i-1,xHigh-x)


    graph.SetMarkerStyle(20)
    graph.SetLineWidth(2)
    graph.SetMarkerSize(1.)
    graph.SetMarkerColor(ROOT.kBlack)
    

    return graph    

class StackPlotter(object):
    def __init__(self):
        self.plotters = []
        self.types    = []
        self.labels   = []
        self.names    = []
    def addPlotter(self,plotter,name="",label = "label",typeP = "background"):
        self.plotters.append(plotter)
        self.types.append(typeP)
        self.labels.append(label)
        self.names.append(name)

    def drawStack(self,var,cut,lumi,bins,mini,maxi,titlex = "", units = ""):
        canvas = ROOT.TCanvas("canvas","")
        canvas.cd()
        hists=[]
        stack = ROOT.THStack("stack","")
        
        signal=0
        background=0
        
        data=None
        for (plotter,typeP,label,name) in zip(self.plotters,self.types,self.labels,self.names):
            if typeP == "signal" or typeP =="background":
                hist = plotter.drawTH1(var,cut,lumi,bins,mini,maxi,titlex,units)
                hist.SetName(name)
                stack.Add(hist)
                hists.append(hist)
                print label+" : %f\n" % hist.Integral()
 
                if typeP == "signal" :
                    signal+=hist.Integral()
                if typeP == "background" :
                    background+=hist.Integral()

       
            if typeP =="data":
                hist = plotter.drawTH1(var,cut,"1",bins,mini,maxi,titlex,units)
                hist.SetName(hist.GetName()+label)
                hists.append(hist)
                data=hist
                dataG=convertToPoisson(hist)
                print label+" : %f\n" % hist.Integral()

       
        #if data not found plot stack only
        if data is not None:                  
            datamax = ROOT.Math.chisquared_quantile_c((1-0.6827)/2.,2*(data.GetMaximum()+1))/2.

            frame = canvas.DrawFrame(mini,0.0,maxi,max(stack.GetMaximum(),datamax)*1.20)
            if len(units)>0:
                frame.GetXaxis().SetTitle(titlex + " [" +units+"]")
            else:    
                frame.GetXaxis().SetTitle(titlex)

            frame.GetYaxis().SetTitle("Events")

            frame.Draw()
            stack.Draw("A,HIST,SAME")
            dataG.Draw("Psame")              
#            ROOT.SetOwnership(data,False)

        else:
            stack.Draw()
        #make legend
        legend = ROOT.TLegend(0.6,0.6,0.8,0.9)
        legend.SetFillColor(ROOT.kWhite)
        for (histo,label,typeP) in reversed(zip(hists,self.labels,self.types)):
            if typeP != "data":
                legend.AddEntry(histo,label,"f")
            else:
                legend.AddEntry(histo,label,"p")
 #       ROOT.SetOwnership(legend,False)

        legend.Draw()
        canvas.SetLeftMargin(canvas.GetLeftMargin()*1.15)
        canvas.Update()



        canvas.RedrawAxis()
        canvas.Update()

        print"---------------------------"
        print "Signal = %f" %(signal)
        print "Bkg    = %f" %(background)
        if data is not None:
            print "Observed = %f"%(data.Integral())
        

        latex1 = ROOT.TLatex(frame.GetXaxis().GetXmin()*1.01,frame.GetYaxis().GetXmax()*1.01,'CMS Preliminary 2011-2012, #sqrt{s} = 7+8 TeV')
        latex1.SetTextSize(0.037)
        latex1.Draw()

        plot={'canvas':canvas,'stack':stack,'legend':legend,'data':data,'dataG':dataG,'latex1':latex1}


        return plot



    def drawComp(self,var,cut,bins,mini,maxi,titlex = "", units = ""):
        canvas = ROOT.TCanvas("canvas","")
        ROOT.SetOwnership(canvas,False)
        canvas.cd()
        hists=[]
        stack = ROOT.THStack("stack","")
        ROOT.SetOwnership(stack,False)
        

        for (plotter,typeP,label) in zip(self.plotters,self.types,self.labels):
                hist = plotter.drawTH1(var,cut,"1",bins,mini,maxi,titlex,units)
#                hist.SetFillStyle(0)
                hist.SetName(hist.GetName()+label)
                hist.Scale(1./hist.Integral())
                stack.Add(hist)
                hists.append(hist)


        stack.Draw("HIST,NOSTACK")
        canvas.SetLeftMargin(canvas.GetLeftMargin()*1.15)

        if len(units):
            stack.GetXaxis().SetTitle(titlex + " [" +units+"]")
        else:
            stack.GetXaxis().SetTitle(titlex)
    
        stack.GetYaxis().SetTitle("a.u")
        stack.GetYaxis().SetTitleOffset(1.2)



        legend = ROOT.TLegend(0.6,0.6,0.9,0.9)
        legend.SetFillColor(ROOT.kWhite)
        for (histo,label,typeP) in zip(hists,self.labels,self.types):
                legend.AddEntry(histo,label,"lf")
        ROOT.SetOwnership(legend,False)
        legend.Draw()

        canvas.Update()

        return canvas

        
        

