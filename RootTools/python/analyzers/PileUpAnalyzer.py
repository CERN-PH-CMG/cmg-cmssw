import os 
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.physicsobjects.PileUpSummaryInfo import PileUpSummaryInfo
from ROOT import TFile, TH1F

class PileUpAnalyzer( Analyzer ):
    '''Computes pile-up weights for MC from the pile up histograms for MC and data.
    These histograms should be set on the components as
    puFileData, puFileMC attributes, as is done here:
    
    http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/H2TauTau/Colin/test_tauMu_2012_cfg.py?view=markup

    THESE HISTOGRAMS MUST BE CONSISTENT, SEE
    https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsPileUpReweighting#Generating_pile_up_distributions
    
    Additionally, this analyzer writes in the output an histogram containing the unweighting MC
    pile-up distribution, to be used in input of the weighting for a later pass. 
    
    Example of use: 
    
    puAna = cfg.Analyzer(
      "PileUpAnalyzer",
      # build unweighted pu distribution using number of pile up interactions if False
      # otherwise, use fill the distribution using number of true interactions
      true = True
      )
    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(PileUpAnalyzer, self).__init__(cfg_ana, cfg_comp, looperName)

        if self.cfg_comp.isMC:
            self.rawmcfile = TFile( '/'.join([self.dirName,
                                              'rawMCPU.root']), 'recreate')
            self.rawmcpileup = TH1F('pileup','raw mc pile-up', 70, 0, 70)
            
            assert( os.path.isfile(self.cfg_comp.puFileMC) )
            assert( os.path.isfile(self.cfg_comp.puFileData) )
            
            self.mcfile = TFile( self.cfg_comp.puFileMC )
            self.mchist = self.mcfile.Get('pileup')
            self.mchist.Scale( 1 / self.mchist.Integral() )
            
            self.datafile = TFile( self.cfg_comp.puFileData )
            self.datahist = self.datafile.Get('pileup')
            self.datahist.Scale( 1 / self.datahist.Integral() )
            # import pdb; pdb.set_trace()
            if self.mchist.GetNbinsX() != self.datahist.GetNbinsX():
                raise ValueError('data and mc histograms must have the same number of bins')
            if self.mchist.GetXaxis().GetXmin() != self.datahist.GetXaxis().GetXmin():
                raise ValueError('data and mc histograms must have the same xmin')
            if self.mchist.GetXaxis().GetXmax() != self.datahist.GetXaxis().GetXmax():
                raise ValueError('data and mc histograms must have the same xmax')

    def declareHandles(self):
        super(PileUpAnalyzer, self).declareHandles()
        self.mchandles['pusi'] =  AutoHandle(
            'addPileupInfo',
            'std::vector<PileupSummaryInfo>' 
            ) 

    def beginLoop(self):
        super(PileUpAnalyzer,self).beginLoop()
        self.averages.add('vertexWeight', Average('vertexWeight') )


    def process(self, iEvent, event):
        self.readCollections( iEvent )

        event.vertexWeight = 1 
        if not self.cfg_comp.isMC:
            return True 

        event.pileUpInfo = map( PileUpSummaryInfo,
                                self.mchandles['pusi'].product() )

        nPU = None
        for puInfo in event.pileUpInfo:
            if puInfo.getBunchCrossing()==0:
                # import pdb; pdb.set_trace()
                if self.cfg_ana.true is False:
                    nPU = puInfo.nPU()
                else:
                    nPU = puInfo.nTrueInteractions()
                self.rawmcpileup.Fill( nPU )

        if nPU is None:
            raise ValueError('nPU cannot be None! means that no pu info has been found for bunch crossing 0.')

        bin = self.datahist.FindBin(nPU)
        if bin<1 or bin>self.datahist.GetNbinsX():
            event.vertexWeight = 0
        else:
            data = self.datahist.GetBinContent(bin)
            mc = self.mchist.GetBinContent(bin)
            #Protect 0 division!!!!
            if mc !=0.0:
                event.vertexWeight = data/mc
            else:
                event.vertexWeight = 1
                
        event.eventWeight *= event.vertexWeight
        self.averages['vertexWeight'].add( event.vertexWeight )

        
    def write(self):
        super(PileUpAnalyzer, self).write()
        if self.cfg_comp.isMC:
            self.rawmcfile.Write()
