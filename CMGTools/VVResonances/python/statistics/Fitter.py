import ROOT


class Fitter(object):
    def __init__(self,poi = ['x']):
        self.w=ROOT.RooWorkspace("w","w")
        self.dimensions = len(poi)
        self.poi=poi
        for v in poi:
            self.w.factory(v+"[0,1]")


    def factory(self,expr):
        self.w.factory(expr)

    def function(self,name,function,dependents):
        self.w.factory("expr::"+name+"('"+function+"',"+','.join(dependents)+")")


    def bernstein(self,name = 'model',poi='x',order=1):
        cList = ROOT.RooArgList()
        for i in range(0,order):
            self.w.factory("c_"+str(i)+"[0.1,0,1000]")
            cList.add(self.w.var("c_"+str(i)))
        bernsteinPDF = ROOT.RooBernstein(name,name,self.w.var(poi),cList)
        getattr(self.w,'import')(bernsteinPDF,ROOT.RooFit.Rename(name))



        
    def importBinnedData(self,histogram,poi = ["x"],name = "data"):
        cList = ROOT.RooArgList()
        for i,p in enumerate(poi):
            cList.add(self.w.var(p))
            if i==0:
                axis=histogram.GetXaxis()
            elif i==1:
                axis=histogram.GetYaxis()
            elif i==2:
                axis=histogram.GetZaxis()
            else:
                print 'Asking for more than 3 D . ROOT doesnt support that, use unbinned data instead'
                return
            mini=axis.GetXmin()
            maxi=axis.GetXmax()
            bins=axis.GetNbins()
            self.w.var(p).setMin(mini)
            self.w.var(p).setMax(maxi)
            self.w.var(p).setBins(bins)
        dataHist=ROOT.RooDataHist(name,name,cList,histogram)
        getattr(self.w,'import')(dataHist,ROOT.RooFit.Rename(name))

    def fit(self,model = "model",data="data"):
        self.w.pdf(model).fitTo(self.w.data("data"))


    def fetch(self,var):
        return (self.w.var(var).getVal(), self.w.var(var).getError())

    def projection(self,model = "model",data="data",poi="x",filename="fit.root"):
        
        frame=self.w.var(poi).frame()
        self.w.data(data).plotOn(frame)
        self.w.pdf(model).plotOn(frame)
        c=ROOT.TCanvas("c","c")
        c.cd()
        frame.Draw()
        c.SaveAs(filename)
        
        
        
                                

        
        
