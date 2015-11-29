import ROOT


class RooPlotter(object):
    def __init__(self,filename):
        self.fIN=ROOT.TFile(filename)
        self.w=self.fIN.Get("w")
        self.contributions=[]


    def fix(self,var,val):
        self.w.var(var).setVal(val)
        self.w.var(var).setConstant(1)

    def prefit(self,model="s",weighted=False):
        self.fitResult = self.w.pdf("model_"+model).fitTo(self.w.data("data_obs"),ROOT.RooFit.NumCPU(8),ROOT.RooFit.SumW2Error(weighted))
    

        
    def addContribution(self,contrib,signal,description,linewidth,lineStyle,lineColor,fillStyle,fillColor):
        self.contributions.append({'name':contrib,'signal':signal,'description':description,'linewidth':linewidth,'linestyle':lineStyle,'linecolor':lineColor,'fillstyle':fillStyle,'fillcolor':fillColor}) 


    def drawStack(self,var,varDesc,cat,suffix,cut="",visualizeError = False):
        #make frame
        self.frame=self.w.var(var+"_"+suffix).frame()
        #plot data 
        if cut !="":
            dataset=self.w.data("data_obs").reduce(cut)
        else:
            dataset=self.w.data("data_obs")

        dataset.plotOn(self.frame,ROOT.RooFit.Cut("CMS_channel==CMS_channel::"+cat))
        #OK now stack for each curve add all the others
        
        errorVisualized=False

        for i in range(0,len(self.contributions)):
            data = self.contributions[i]
            names=[]
            for j in range(i,len(self.contributions)):
                if self.contributions[j]['signal']:
                    names.append('shapeSig_'+self.contributions[j]['name']+"_"+cat)
                else:
                    names.append('shapeBkg_'+self.contributions[j]['name']+"_"+cat)

            self.w.pdf("model_s").plotOn(self.frame,ROOT.RooFit.Slice(self.w.cat("CMS_channel"),cat),ROOT.RooFit.Components(",".join(names)),ROOT.RooFit.ProjWData(dataset),ROOT.RooFit.Name(data['name']+"1"),ROOT.RooFit.LineStyle(data['linestyle']),\
                                             ROOT.RooFit.LineColor(data['linecolor']),ROOT.RooFit.FillStyle(data['fillstyle']),ROOT.RooFit.FillColor(data['fillcolor']),ROOT.RooFit.DrawOption("F"))
           
            self.w.pdf("model_s").plotOn(self.frame,ROOT.RooFit.Slice(self.w.cat("CMS_channel"),cat),ROOT.RooFit.Components(",".join(names)),ROOT.RooFit.ProjWData(dataset),ROOT.RooFit.Name(data['name']+"2"),ROOT.RooFit.LineStyle(data['linestyle']),\
                                             ROOT.RooFit.LineColor(data['linecolor']),ROOT.RooFit.FillStyle(data['fillstyle']),ROOT.RooFit.FillColor(data['fillcolor']),ROOT.RooFit.DrawOption("L"))

            if (not data['signal']) and (not errorVisualized):
                self.w.pdf("model_s").plotOn(self.frame,ROOT.RooFit.Slice(self.w.cat("CMS_channel"),cat),ROOT.RooFit.Components(",".join(names)),ROOT.RooFit.ProjWData(dataset),ROOT.RooFit.Name(data['name']+"3"),ROOT.RooFit.LineStyle(data['linestyle']),\
                                                 ROOT.RooFit.LineColor(data['linecolor']),ROOT.RooFit.FillStyle(1),ROOT.RooFit.FillColor(ROOT.kOrange),ROOT.RooFit.VisualizeError(self.fitResult,1))
                errorVisualized=True

        dataset.plotOn(self.frame,ROOT.RooFit.Cut("CMS_channel==CMS_channel::"+cat))
        
        self.frame.SetXTitle(varDesc)
        self.frame.SetYTitle("Events")

        #legend
        self.legend = ROOT.TLegend(0.62,0.6,0.92,0.90,"","brNDC")
	self.legend.SetBorderSize(0)
	self.legend.SetLineColor(1)
	self.legend.SetLineStyle(1)
	self.legend.SetLineWidth(1)
	self.legend.SetFillColor(0)
	self.legend.SetFillStyle(0)
	self.legend.SetTextFont(42)

        for c in self.contributions:
            name=c['name']
            desc=c['description']
            curve=self.frame.getCurve(name+"1")
            self.legend.AddEntry(curve,desc,"lf")
            
            


        self.frame.SetTitle("CMS Preliminary, #sqrt{s} = 13 TeV, L = 1.3 fb^{-1}")    
        self.frame.SetLabelSize(0.05,"X")    
        self.frame.SetLabelSize(0.05,"Y")    
        self.frame.SetTitleSize(0.05,"X")    
        self.frame.SetTitleSize(0.05,"Y")    
        self.frame.SetTitleOffset(0.90,"X")    
        self.frame.SetTitleOffset(0.93,"Y")    

        self.frame.Draw()


        self.legend.Draw()    

	pt =ROOT.TPaveText(0.1577181,0.9562937,0.9580537,0.9947552,"brNDC")
	pt.SetBorderSize(0)
	pt.SetTextAlign(12)
	pt.SetFillStyle(0)
	pt.SetTextFont(42)
	pt.SetTextSize(0.03)
	text = pt.AddText(0.01,0.3,"CMS Preliminary")
	text = pt.AddText(0.25,0.3,"")
	pt.Draw()   



    
    

        
        
        
