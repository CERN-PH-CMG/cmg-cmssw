from ROOT import TFile, TH1, TCanvas, TLegend
import ROOT,os,copy

ROOT.gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

ROOT.gROOT.LoadMacro( 'cms35.C' )


dir = 'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/FullFilteringEffect/'

xmin = 0.25
xmax = xmin + 0.3

class Histograms:
    
    def setStyleBefore( self, histo ):
        setHisto( histo )
        histo.SetMarkerColor(1)
        histo.SetMarkerStyle(20)
        
    def setStyleAfter( self, histo ):
        setHisto( histo )
        histo.SetMarkerColor(4)
        histo.SetMarkerStyle(25)
        
    def __init__( self, isData, fileName ):
        
        self.isData_ = isData
        
        before = 'ht_geq300__geq3_j50__dphi12_geq0p5__dphi3_geq0p3__HT_triggered__mht'
        after = 'ht_geq300__geq3_j50__dphi12_geq0p5__dphi3_geq0p3__TP_and_BE_filtered__mht'
        self.file_ = TFile.Open( fileName )
        self.before_ = self.file_.Get( before )
        self.after_ = self.file_.Get( after )
        
        # scaling QCD histograms
        if not self.isData_:
            self.before_.Scale(0.36)
            self.after_.Scale(0.36)
        
        self.setStyleBefore(self.before_)
        self.setStyleAfter(self.after_)
        
        self.legend_ = TLegend(xmax+0.01,0.7, 0.89,0.89)
        self.legend_.AddEntry( self.before_ , "Before filters" )
        self.legend_.AddEntry( self.after_ , "After filters" )
        self.legend_.SetFillStyle(0)
        

def setCanvas( name ):
    canvas = ROOT.TCanvas(name, name, 500, 500)
    canvas.SetLogy()
    canvas.SetLeftMargin(0.15)
    ROOT.SetOwnership(canvas,False)
    return canvas

    
def setHisto( histo ):
    histo.Sumw2()
    histo.SetStats(0)
    histo.SetTitle(';#slashH_{T} (GeV)')



def plot( histograms, rebin=1, min=1e-8, max=2e5):
    
    # file = fQCD
    canvasName = 'QCD'
#    scaleFactor = 0.36
    if histograms.isData_:
#        scaleFactor = 1
        # file = fData
        canvasName = 'Data'
    
    canvas = setCanvas( canvasName) 
    
    hBefore = histograms.before_
    hAfter = histograms.after_ 
    
    
    print 'name: ', canvasName
    print 'before: ', hBefore.Integral()
    print 'after: ', hAfter.Integral()
    
    if rebin>1:
        hBefore.Rebin( rebin )
        hAfter.Rebin( rebin )
    
    hBefore.Draw()
    hAfter.Draw("same")
    histograms.legend_.Draw("same")

    if histograms.isData_:
        ROOT.cms35( xmin,0.75,xmax,0.88, "Data");
    else:
        ROOT.cms35( xmin,0.75,xmax,0.88, "Simulation");
    
    hBefore.GetYaxis().SetRangeUser(min, max)
    
#    setStyleBefore( hBefore )
#    setStyleAfter( hAfter )
    
    canvas.SaveAs('mht_' + canvasName + '.png')
    canvas.SaveAs('mht_' + canvasName + '.pdf')

QCD = Histograms(False, dir+'QCD-pythia6_data_test_cutflow.root')
Data = Histograms(True, dir+'JetData_data_test_cutflow.root')
    
plot( QCD, 4, 1e-8, 1e6)

plot( Data, 4, 1e-8, 1e6)
