import ROOT
import math
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
            graph.SetPointEXlow(i-1,0.0)
            graph.SetPointEXhigh(i-1,0.0)


    graph.SetMarkerStyle(20)
    graph.SetLineWidth(2)
    graph.SetMarkerSize(1.)
    graph.SetMarkerColor(ROOT.kBlack)
    

    return graph    

def CMS_lumi(lumi,pad):
    H = pad.GetWh();
    W = pad.GetWw();
    l = pad.GetLeftMargin();
    t = pad.GetTopMargin();
    r = pad.GetRightMargin();
    b = pad.GetBottomMargin();

    cmsText     = "CMS"
    cmsTextFont   = 61  # default is helvetic-bold

    writeExtraText = True
#    extraText   = "Preliminary"
    extraText   = "Private"
    extraTextFont = 52  # default is helvetica-italics

    # text sizes and text offsets with respect to the top frame
    # in unit of the top margin size
    lumiTextSize     = 0.6
    lumiTextOffset   = 0.2
    cmsTextSize      = 0.75
    cmsTextOffset    = 0.1  # only used in outOfFrame version
    
    # ratio of "CMS" and extra text size
    extraOverCmsTextSize  = 0.76
    extraTextSize = extraOverCmsTextSize*cmsTextSize;

    
    if lumi > 1000.:
        lumi = float(lumi)/1000.
        print lumi

    lumiText = "#scale[0.85]{" + str(lumi) + " fb^{-1} (13 TeV) }"
#    print "lumiText: ", lumiText

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)


    posX_ =   1-r
    posY_ =   1-t+lumiTextOffset*t
    latex.SetTextFont(42)
    latex.SetTextAlign(31) 
    latex.SetTextSize(lumiTextSize*t)    
    latex.DrawLatex(posX_,posY_,lumiText)

    posX_ =   l+cmsTextOffset
    posY_ =   1-t+lumiTextOffset*t
    latex.SetTextFont(cmsTextFont)
    latex.SetTextAlign(11) 
    latex.SetTextSize(cmsTextSize*t)    
    latex.DrawLatex(posX_,posY_,cmsText)

    relPosX    = 0.15
    relPosY    = 0.035
    posX_ =   posX_ + relPosX*(1-r)
    posY_ =   1-t+lumiTextOffset*t
    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(extraTextSize*t)
    latex.SetTextAlign(11)
    latex.DrawLatex(posX_, posY_, extraText)

    return latex

def getPad4var():
    pad = ROOT.TPad("pad4var","Histos",0.,0.27,1.0,1.0)
    pad.SetBottomMargin(0.03)
    pad.SetLeftMargin(0.15)
    pad.SetRightMargin(0.08)
    pad.SetTopMargin(0.1)
    
    pad.SetFillColor(0)
    pad.SetBorderMode(0)
    pad.SetBorderSize(2)
    pad.SetTickx(1)
    pad.SetTicky(1)
    pad.SetFrameFillStyle(0)
    pad.SetFrameBorderMode(0)
    pad.SetFrameFillStyle(0)
    pad.SetFrameBorderMode(0)

    return pad

def getPad4ratio():
    pad = ROOT.TPad("pad4ratio","Ratio",0.,0.0,1.0,0.27)
    pad.SetBottomMargin(0.25)
    pad.SetTopMargin(0)
    pad.SetLeftMargin(0.15)
    pad.SetRightMargin(0.08)
    
    pad.SetFillColor(0)
    pad.SetBorderMode(0)
    pad.SetBorderSize(2)
    pad.SetTickx(1)
    pad.SetTicky(1)
    pad.SetFrameFillStyle(0)
    pad.SetFrameBorderMode(0)
    pad.SetFrameFillStyle(0)
    pad.SetFrameBorderMode(0)
    
    return pad


class StackPlotter(object):

    def __init__(self,defaultCut="1"):
        self.plotters = []
        self.types    = []
        self.labels   = []
        self.names    = []
        self.log=False
        self.defaultCut=defaultCut

    def setLog(self,doLog):
        self.log=doLog
    def addPlotter(self,plotter,name="",label = "label",typeP = "background"):
        self.plotters.append(plotter)
        self.types.append(typeP)
        self.labels.append(label)
        self.names.append(name)

    def drawStack(self,var,cut,lumi,bins,mini,maxi,titlex = "", units = ""):
        # handling input histograms
        hists=[]
        stack = ROOT.THStack("stack","")
        
        signal=0
        background=0
        backgroundErr=0
        
        data=None
        dataG=None
        error=ROOT.Double(0.0)

        cutL="("+self.defaultCut+")*("+cut+")"

        bkg = ROOT.TH1F("bkg","bkg",bins,mini,maxi)

        for (plotter,typeP,label,name) in zip(self.plotters,self.types,self.labels,self.names):
            if typeP == "signal" or typeP =="background":
                hist = plotter.drawTH1(var,cutL,lumi,bins,mini,maxi,titlex,units)
                hist.SetName(name)
                stack.Add(hist)
                hists.append(hist)
                print label+" : %f\n" % hist.Integral()
 
                if typeP == "signal" :
                    signal+=hist.Integral()
                if typeP == "background" :
                    background+=hist.IntegralAndError(1,hist.GetNbinsX(),error)
                    backgroundErr+=error*error
                    bkg.Add(hist)

            if typeP =="data":
                hist = plotter.drawTH1(var,cutL,"1",bins,mini,maxi,titlex,units)
                hist.SetName(hist.GetName()+label)
                hists.append(hist)
                data=hist
                dataG=convertToPoisson(hist)
                dataG.SetLineWidth(1)
                print label+" : %f\n" % hist.Integral()
                

        # plotting
        canvas = ROOT.TCanvas("canvas","",900,900)
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptTitle(0)
        ROOT.gStyle.SetOptTitle(0)
        ROOT.TGaxis.SetMaxDigits(4)
        canvas.Range(-68.75,-7.5,856.25,42.5)
        canvas.SetFillColor(0)
        canvas.SetBorderMode(0)
        canvas.SetBorderSize(2)
        canvas.SetTickx(1)
        canvas.SetTicky(1)
        canvas.SetLeftMargin(0.15)
        canvas.SetRightMargin(0.05)
        canvas.SetTopMargin(0.08)
        canvas.SetBottomMargin(0.15)
        canvas.SetFrameFillStyle(0)
        canvas.SetFrameBorderMode(0)
        
        canvas.cd()

        #if data not found plot stack only
        if data != None:                  
            datamax = ROOT.Math.chisquared_quantile_c((1-0.6827)/2.,2*(data.GetMaximum()+1))/2.
        else: 
            datamax = stack.GetMaximum()

        if not self.log:
            frame = canvas.DrawFrame(mini,0.0,maxi,max(stack.GetMaximum(),datamax)*1.20)
        else:    
            frame = canvas.DrawFrame(mini,0.1,maxi,max(stack.GetMaximum(),datamax)*100)

        frame.GetXaxis().SetLabelFont(42)
        frame.GetXaxis().SetLabelOffset(0.007)
        frame.GetXaxis().SetLabelSize(0.045)
        frame.GetXaxis().SetTitleSize(0.05)
        frame.GetXaxis().SetTitleOffset(1.15)
        frame.GetXaxis().SetTitleFont(42)
        frame.GetYaxis().SetLabelFont(42)
        frame.GetYaxis().SetLabelOffset(0.007)
        frame.GetYaxis().SetLabelSize(0.045)
        frame.GetYaxis().SetTitleSize(0.05)
        frame.GetYaxis().SetTitleOffset(1.4)
        frame.GetYaxis().SetTitleFont(42)
        frame.GetZaxis().SetLabelFont(42)
        frame.GetZaxis().SetLabelOffset(0.007)
        frame.GetZaxis().SetLabelSize(0.045)
        frame.GetZaxis().SetTitleSize(0.05)
        frame.GetZaxis().SetTitleFont(42)


        if len(units)>0:
            frame.GetXaxis().SetTitle(titlex + " (" +units+")")
            frame.GetYaxis().SetTitle("Events / "+str((maxi-mini)/bins)+ " "+units)
        else:    
            frame.GetXaxis().SetTitle(titlex)
            frame.GetYaxis().SetTitle("Events")

        frame.Draw()
        stack.Draw("A,HIST,SAME")
        if data !=None:
            dataG.Draw("Psame")              

        legend = ROOT.TLegend(0.58,0.55,0.88,0.85,"","brNDC")
	legend.SetBorderSize(0)
	legend.SetLineColor(1)
	legend.SetLineStyle(1)
	legend.SetLineWidth(1)
	legend.SetFillColor(0)
	legend.SetFillStyle(0)
	legend.SetTextFont(42)

        legend.SetFillColor(ROOT.kWhite)
        for (histo,label,typeP) in reversed(zip(hists,self.labels,self.types)):
            if typeP != "data" and typeP !='signal':
                legend.AddEntry(histo,label,"f")
            elif typeP == 'data':
                legend.AddEntry(histo,label,"p")

        for (histo,label,typeP) in reversed(zip(hists,self.labels,self.types)):
            if typeP == "signal":
                legend.AddEntry(histo,label,"f")

 #       ROOT.SetOwnership(legend,False)

        legend.Draw()

        if self.log:
            pad1.SetLogy()

        canvas.cd()
        canvas.SetLeftMargin(canvas.GetLeftMargin()*1.15)
        canvas.SetRightMargin(canvas.GetRightMargin()*2.)
        cmsText = CMS_lumi(lumi,canvas)

        canvas.Update()


        print"---------------------------"
        print "Signal = %f" %(signal)
        print "Bkg    = %f" %(background)
        if data is not None:
            print "Observed = %f"%(data.Integral())
            integral = data.IntegralAndError(1,data.GetNbinsX(),error)
            if background>0.0:
                print "Data/Bkg= {ratio} +- {err}".format(ratio=integral/background,err=math.sqrt(error*error/(background*background)+integral*integral*backgroundErr/(background*background*background*background)))


        plot={'canvas':canvas,'stack':stack,'legend':legend,'data':data,'dataG':dataG,'cmsText':cmsText}

        canvas.RedrawAxis()
        canvas.Update()


        return plot


    def drawStackAndRatio(self,var,cut,lumi,bins,mini,maxi,titlex = "", units = ""):
        # handling input histograms
        hists=[]
        stack = ROOT.THStack("stack","")
        
        signal=0
        background=0
        backgroundErr=0
        
        data=None
        dataG=None
        error=ROOT.Double(0.0)

        cutL="("+self.defaultCut+")*("+cut+")"

        bkg = ROOT.TH1F("bkg","bkg",bins,mini,maxi)

        for (plotter,typeP,label,name) in zip(self.plotters,self.types,self.labels,self.names):
            if typeP == "signal" or typeP =="background":
                hist = plotter.drawTH1(var,cutL,lumi,bins,mini,maxi,titlex,units)
                hist.SetName(name)
                stack.Add(hist)
                hists.append(hist)
                print label+" : %f\n" % hist.Integral()
 
                if typeP == "signal" :
                    signal+=hist.Integral()
                if typeP == "background" :
                    background+=hist.IntegralAndError(1,hist.GetNbinsX(),error)
                    backgroundErr+=error*error
                    bkg.Add(hist)

            if typeP =="data":
                hist = plotter.drawTH1(var,cutL,"1",bins,mini,maxi,titlex,units)
                hist.SetName(hist.GetName()+label)
                hists.append(hist)
                data=hist
                dataG=convertToPoisson(hist)
                dataG.SetLineWidth(1)
                print label+" : %f\n" % hist.Integral()
                

        # plotting
        canvas = ROOT.TCanvas("canvas","",900,900)
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptTitle(0)
        ROOT.TGaxis.SetMaxDigits(4)
        canvas.Range(-68.75,-7.5,856.25,42.5)
        canvas.SetFillColor(0)
        canvas.SetBorderMode(0)
        canvas.SetBorderSize(2)
        canvas.SetTickx(1)
        canvas.SetTicky(1)
        canvas.SetLeftMargin(0.15)
        canvas.SetRightMargin(0.05)
        canvas.SetTopMargin(0.08)
        canvas.SetBottomMargin(0.15)
        canvas.SetFrameFillStyle(0)
        canvas.SetFrameBorderMode(0)
        canvas.SetFrameFillStyle(0)
        canvas.SetFrameBorderMode(0)
        
        canvas.cd()
        ###########
        pad1 = getPad4var()
        pad1.Draw()

        pad2 = getPad4ratio()
        pad2.Draw()

        ###########

        if pad2 == None:
            pad2 = getPad4ratio()

        pad2.cd()
        ratio = data.Clone("ratio")
        ratio.GetYaxis().SetTitle("data/MC")
        ratio.Divide(bkg)
        
        
#        frame2 = pad2.DrawFrame(mini,0.0,maxi,2.0)
        ratio.GetXaxis().SetLabelFont(42)
        ratio.GetXaxis().SetLabelOffset(0.007)
        ratio.GetXaxis().SetLabelSize(0.1)
        ratio.GetXaxis().SetTitleSize(0.12)
        ratio.GetXaxis().SetTitleOffset(1.)
        ratio.GetXaxis().SetTitleFont(42)
        ratio.GetYaxis().SetLabelFont(42)
        ratio.GetYaxis().SetLabelOffset(0.007)
        ratio.GetYaxis().SetLabelSize(0.1)
        ratio.GetYaxis().SetTitleSize(0.12)
        ratio.GetYaxis().SetTitleOffset(1.4)
        ratio.GetYaxis().SetTitleFont(42)
        ratio.GetZaxis().SetLabelFont(42)
        ratio.GetZaxis().SetLabelOffset(0.007)
        ratio.GetZaxis().SetLabelSize(0.045)
        ratio.GetZaxis().SetTitleSize(0.05)
        ratio.GetZaxis().SetTitleFont(42)

        ratio.GetXaxis().SetTitle(titlex + " (" +units+")")
        ratio.GetYaxis().SetTitle("data/MC")
        
#        frame2.Draw()

        ratio.Draw()

        if pad1 == None:
            pad1 = getPad4var()
            
        pad1.cd()

        #if data not found plot stack only
        if data != None:                  
            datamax = ROOT.Math.chisquared_quantile_c((1-0.6827)/2.,2*(data.GetMaximum()+1))/2.
        else: 
            datamax = stack.GetMaximum()

        if not self.log:
            frame = pad1.DrawFrame(mini,0.0,maxi,max(stack.GetMaximum(),datamax)*1.20)
        else:    
            frame = pad1.DrawFrame(mini,0.1,maxi,max(stack.GetMaximum(),datamax)*100)

        frame.GetXaxis().SetLabelFont(0)
        frame.GetXaxis().SetLabelOffset(9999)
        frame.GetXaxis().SetLabelSize(0)
        frame.GetXaxis().SetTitleSize(0)
        frame.GetXaxis().SetTitleOffset(9999)
        frame.GetXaxis().SetTitleFont(0)
        frame.GetYaxis().SetLabelFont(42)
        frame.GetYaxis().SetLabelOffset(0.007)
        frame.GetYaxis().SetLabelSize(0.045)
        frame.GetYaxis().SetTitleSize(0.05)
        frame.GetYaxis().SetTitleOffset(1.4)
        frame.GetYaxis().SetTitleFont(42)
        frame.GetZaxis().SetLabelFont(42)
        frame.GetZaxis().SetLabelOffset(0.007)
        frame.GetZaxis().SetLabelSize(0.045)
        frame.GetZaxis().SetTitleSize(0.05)
        frame.GetZaxis().SetTitleFont(42)


        if len(units)>0:
            frame.GetXaxis().SetTitle(titlex + " (" +units+")")
            frame.GetYaxis().SetTitle("Events / "+str((maxi-mini)/bins)+ " "+units)
        else:    
            frame.GetXaxis().SetTitle(titlex)
            frame.GetYaxis().SetTitle("Events")

        frame.Draw()
        stack.Draw("A,HIST,SAME")
        if data !=None:
            dataG.Draw("Psame")              

        legend = ROOT.TLegend(0.62,0.55,0.92,0.85,"","brNDC")
	legend.SetBorderSize(0)
	legend.SetLineColor(1)
	legend.SetLineStyle(1)
	legend.SetLineWidth(1)
	legend.SetFillColor(0)
	legend.SetFillStyle(0)
	legend.SetTextFont(42)

        legend.SetFillColor(ROOT.kWhite)
        for (histo,label,typeP) in reversed(zip(hists,self.labels,self.types)):
            if typeP != "data" and typeP !='signal':
                legend.AddEntry(histo,label,"f")
            elif typeP == 'data':
                legend.AddEntry(histo,label,"p")

        for (histo,label,typeP) in reversed(zip(hists,self.labels,self.types)):
            if typeP == "signal":
                legend.AddEntry(histo,label,"f")

 #       ROOT.SetOwnership(legend,False)

        legend.Draw()

        if self.log:
            pad1.SetLogy()

#        cmsText = drawCMStext(lumi)
        cmsText = CMS_lumi(lumi,pad1)

        canvas.cd()
#        canvas.SetLeftMargin(canvas.GetLeftMargin()*1.15)
        canvas.Update()


        print"---------------------------"
        print "Signal = %f" %(signal)
        print "Bkg    = %f" %(background)
        if data is not None:
            print "Observed = %f"%(data.Integral())
            integral = data.IntegralAndError(1,data.GetNbinsX(),error)
            if background>0.0:
                print "Data/Bkg= {ratio} +- {err}".format(ratio=integral/background,err=math.sqrt(error*error/(background*background)+integral*integral*backgroundErr/(background*background*background*background)))


        plot={'canvas':canvas,'stack':stack,'legend':legend,'data':data,'dataG':dataG,'pad1':pad1,'pad2':pad2,'ratio':ratio,'cmsText':cmsText}

#        canvas.RedrawAxis()
        pad2.RedrawAxis()
        pad1.RedrawAxis()
        canvas.Update()


        return plot





    def drawComp(self,var,cut,bins,mini,maxi,titlex = "", units = ""):
        canvas = ROOT.TCanvas("canvas","")
        ROOT.SetOwnership(canvas,False)
        canvas.cd()
        hists=[]
        stack = ROOT.THStack("stack","")
        ROOT.SetOwnership(stack,False)

        canvas.Range(-68.75,-7.5,856.25,42.5)
        canvas.SetFillColor(0)
        canvas.SetBorderMode(0)
        canvas.SetBorderSize(2)
        canvas.SetTickx(1)
        canvas.SetTicky(1)
        canvas.SetLeftMargin(0.15)
        canvas.SetRightMargin(0.05)
        canvas.SetTopMargin(0.05)
        canvas.SetBottomMargin(0.15)
        canvas.SetFrameFillStyle(0)
        canvas.SetFrameBorderMode(0)
        canvas.SetFrameFillStyle(0)
        canvas.SetFrameBorderMode(0)


        for (plotter,typeP,label) in zip(self.plotters,self.types,self.labels):
                hist = plotter.drawTH1(var,cut,"1",bins,mini,maxi,titlex,units)
#                hist.SetFillStyle(0)
                hist.SetName(hist.GetName()+label)
                hist.Scale(1./hist.Integral())
                stack.Add(hist)
                hists.append(hist)


        stack.Draw("HIST,NOSTACK")
#        canvas.SetLeftMargin(canvas.GetLeftMargin()*1.15)

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


	pt =ROOT.TPaveText(0.1577181,0.9562937,0.9580537,0.9947552,"brNDC")
	pt.SetBorderSize(0)
	pt.SetTextAlign(12)
	pt.SetFillStyle(0)
	pt.SetTextFont(42)
	pt.SetTextSize(0.03)
	text = pt.AddText(0.01,0.5,"CMS simulation")
	pt.Draw()   


        canvas.Update()

        return canvas

        
        

