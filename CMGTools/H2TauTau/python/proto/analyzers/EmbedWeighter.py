from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

class EmbedWeighter( Analyzer ):
    '''Gets lepton efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EmbedWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

            
    def beginLoop(self):
        print self, self.__class__
        super(EmbedWeighter,self).beginLoop()
        self.averages.add('weight', Average('weight') )


    def declareHandles(self):
        super(EmbedWeighter,self).declareHandles()
        if self.cfg_comp.isEmbed:
            # import pdb; pdb.set_trace()
            if cmsswIs52X():
                self.embhandles['minVisPtFilter'] = AutoHandle(
                    ('generator', 'minVisPtFilter'),
                    'GenFilterInfo'
                    )
                self.embhandles['TauSpinnerReco'] = AutoHandle(
                    ('TauSpinnerReco', 'TauSpinnerWT'),
                    'double'
                    )
                self.embhandles['ZmumuEvtSelEffCorrWeightProducer'] = AutoHandle(
                    ('ZmumuEvtSelEffCorrWeightProducer', 'weight'),
                    'double'
                    )
                self.embhandles['muonRadiationCorrWeightProducer'] = AutoHandle(
                    ('muonRadiationCorrWeightProducer', 'weight'),
                    'double'
                    )
                self.embhandles['genTau2PtVsGenTau1Pt'] = AutoHandle(
                    ('embeddingKineReweightRECembedding', 'genTau2PtVsGenTau1Pt'),
                    'double'
                    )
                self.embhandles['genTau2EtaVsGenTau1Eta'] = AutoHandle(
                    ('embeddingKineReweightRECembedding', 'genTau2EtaVsGenTau1Eta'),
                    'double'
                    )
                self.embhandles['genDiTauMassVsGenDiTauPt'] = AutoHandle(
                    ('embeddingKineReweightRECembedding', 'genDiTauMassVsGenDiTauPt'),
                    'double'
                    )
            else:
                self.embhandles['minVisPtFilter'] = AutoHandle(
                    ('generator', 'weight'),
                    'double'
                    )

                

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.weight = 1

        if self.cfg_comp.isEmbed:
            try: 
                genfilter = self.embhandles['minVisPtFilter'].product()
                tauspin = self.embhandles['TauSpinnerReco'].product()
                zmumusel = self.embhandles['ZmumuEvtSelEffCorrWeightProducer'].product()
                muradcorr = self.embhandles['muonRadiationCorrWeightProducer'].product()
                genTau2PtVsGenTau1Pt = self.embhandles['genTau2PtVsGenTau1Pt'].product()
                genTau2EtaVsGenTau1Eta = self.embhandles['genTau2EtaVsGenTau1Eta'].product()
                genDiTauMassVsGenDiTauPt = self.embhandles['genDiTauMassVsGenDiTauPt'].product()
            except RuntimeError:
                print 'WARNING EmbedWeighter, cannot find the weight in the event'
                return False
            if cmsswIs52X():
                self.weight = genfilter.filterEfficiency()
                self.weight *= tauspin[0]
                self.weight *= zmumusel[0]
                self.weight *= muradcorr[0]
                self.weight *= genTau2PtVsGenTau1Pt[0]
                self.weight *= genTau2EtaVsGenTau1Eta[0]
                self.weight *= genDiTauMassVsGenDiTauPt[0]
            else: 
                self.weight = genfilter[0]
        if self.cfg_ana.verbose:
            print self.name, 'efficiency =', self.weight
        event.eventWeight *= self.weight
        event.embedWeight = self.weight
        self.averages['weight'].add( self.weight )
        return True
                
