from ROOT import TTree, TH1F, TH2F, TProfile, TLegend, gDirectory

from CMGTools.H2TauTau.macros.Histograms import Histograms


class WeightHistograms(Histograms):
    def __init__(self, name):

        self.h_trigger = TH1F(name+'_h_trigger', ';trigger weight', 100, 0,5)
        self.h_vertex = TH1F(name+'_h_vertex', ';pile-up weight', 100, 0, 5)
        self.h_event = TH1F(name+'_h_full', ';event weight', 100, 0, 5)
         
        super(WeightHistograms, self).__init__(name)

    def FillWeight(self, event ):
        self.h_trigger.Fill( event.triggerWeight )
        self.h_vertex.Fill( event.vertexWeight )
        self.h_event.Fill( event.eventWeight )
        
 
