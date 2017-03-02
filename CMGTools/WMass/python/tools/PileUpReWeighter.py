import os.path
from ROOT import TFile, TH1F

class PileUpReWeighter:

    def __init__(self,puFileData, puFileMC, run):
        self.isdata = True if run > 1 else False
        self.puFileData = puFileData
        self.puFileMC = puFileMC

        assert( os.path.isfile(os.path.expandvars(self.puFileMC)) )
        assert( os.path.isfile(os.path.expandvars(self.puFileData)) )

        self.mcfile = TFile( self.puFileMC )
        self.mchist = self.mcfile.Get('nvtx')
        self.mchist.Scale( 1 / self.mchist.Integral() )
        
        self.datafile = TFile( self.puFileData )
        self.datahist = self.datafile.Get('nvtx')
        self.datahist.Scale( 1 / self.datahist.Integral() )

        if self.mchist.GetNbinsX() != self.datahist.GetNbinsX():
            raise ValueError('data and mc histograms must have the same number of bins')
        if self.mchist.GetXaxis().GetXmin() != self.datahist.GetXaxis().GetXmin():
            raise ValueError('data and mc histograms must have the same xmin')
        if self.mchist.GetXaxis().GetXmax() != self.datahist.GetXaxis().GetXmax():
            raise ValueError('data and mc histograms must have the same xmax')


    def getWeight(self,nvtx):
        if self.isdata: return 1.0
        vertexWeight = 1
        bin = self.datahist.FindBin(nvtx)
        if bin<1 or bin>self.datahist.GetNbinsX():
            vertexWeight = 0
        else:
            data = self.datahist.GetBinContent(bin)
            mc = self.mchist.GetBinContent(bin)
            #Protect 0 division!!!!
            if mc !=0.0:
                vertexWeight = data/mc
            else:
                vertexWeight = 1

        return vertexWeight
        
