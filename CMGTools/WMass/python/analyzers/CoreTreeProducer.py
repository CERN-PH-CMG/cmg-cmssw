import numpy as my_n
import math
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.analyzers.ntuple import *
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton, Muon, Electron
from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.RootTools.physicsobjects.PileUpSummaryInfo import PileUpSummaryInfo


def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

##--------------------------------------------------

def bookW( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def fillW( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.Pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.Phi() )

def bookZ( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_rap'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))

def fillZ( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.Pt() )
    fill(tree, '{pName}_rap'.format(pName=pName), particle.Rapidity() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.Phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.M() )

##--------------------------------------------------

def bookFourVector( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))

def fillFourVector( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    
def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )

def bookMuon( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))
    var(tree, '{pName}RelIso'.format(pName=pName))
    var(tree, '{pName}TkIso'.format(pName=pName))
    var(tree, '{pName}TkValidHits'.format(pName=pName))
    # var(tree, '{pName}TkAlgo'.format(pName=pName))
    # var(tree, '{pName}_ptError'.format(pName=pName))

def fillMuon( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )
    fill(tree, '{pName}RelIso'.format(pName=pName), particle.relIso(0.5) )
    fill(tree, '{pName}TkIso'.format(pName=pName), particle.sourcePtr().userIsolation( 7 ) )
    fill(tree, '{pName}TkValidHits'.format(pName=pName), particle.sourcePtr().innerTrack().numberOfValidHits() )
    # fill(tree, '{pName}TkAlgo'.format(pName=pName), particle.sourcePtr().innerTrack.TrackAlgorithm() )
    # fill(tree, '{pName}_ptError'.format(pName=pName), particle.sourceCandidatePtr(0).innerTrack().ptError() )


def bookJet( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def fillJet( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

##--------------------------------------------------
    
def bookLHE_weight( tree, pName ):
    # tree.vars['{pName}_weight'.format(pName=pName)]= my_n.zeros(400, dtype=float)
    # tree.tree.Branch('{pName}_weight'.format(pName=pName),tree.vars['{pName}_weight'.format(pName=pName)] ,'{pName}_weight'.format(pName=pName)+'[400]/D' )
    tree.vector('{pName}_weight'.format(pName=pName), 466)

def fillLHE_weight( tree, pName, LHE_weight,event ):
    # for i in range(0,min(len(LHE_weight),400)):
        # # print 'filling ',i,'with ',LHE_weight[i]
        # tree.vars['{pName}_weight'.format(pName=pName)][i] = LHE_weight[i]
    tree.vfill('{pName}_weight'.format(pName=pName), LHE_weight)

##--------------------------------------------------

def bookMuonCovMatrix( tree, pName ):
    # tree.vars['{pName}CovMatrix'.format(pName=pName)]= my_n.zeros(9, dtype=float)
    # tree.tree.Branch('{pName}CovMatrix'.format(pName=pName),tree.vars['{pName}CovMatrix'.format(pName=pName)] ,'{pName}CovMatrix'.format(pName=pName)+'[9]/D' )
    tree.vector('{pName}CovMatrix'.format(pName=pName), 9)

def fillMuonCovMatrix( tree, pName, covMatrix,event ):
    # vcovMatrix=[]
    # for i in range(0,9):
        # # tree.vars['{pName}CovMatrix'.format(pName=pName)][i] = covMatrix[i]
      # vcovMatrix.append(covMatrix[i])
    tree.vfill('{pName}CovMatrix'.format(pName=pName), covMatrix)

##--------------------------------------------------

def bookMET( tree, pName ):
    var(tree, '{pName}'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def bookCustomMET( tree, pName ):
    var(tree, '{pName}'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_sumEt'.format(pName=pName))

def fillMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

def fillCustomMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_sumEt'.format(pName=pName), particle.sumEt() )    ### this was in the W analyzer


class CoreTreeProducer( TreeAnalyzerNumpy ):

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(CoreTreeProducer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareCoreHandles(self):

      self.handles['pfMet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>')
      self.handles['pfMetraw'] = AutoHandle('cmgPFMETRaw','std::vector<cmg::BaseMET>')
      self.handles['pfMetSignificance'] = AutoHandle('pfMetSignificance','cmg::METSignificance')
      self.handles['nopuMet'] = AutoHandle('nopuMet','std::vector<reco::PFMET>')
      self.handles['pucMet'] = AutoHandle('pcMet','std::vector<reco::PFMET>')
      self.handles['pfMetForRegression'] = AutoHandle('pfMetForRegression','std::vector<reco::PFMET>')
      self.handles['puMet'] = AutoHandle('puMet','std::vector<reco::PFMET>')
      self.handles['tkMet'] = AutoHandle('tkMet','std::vector<reco::PFMET>')
      if( hasattr(self.cfg_ana,'additionalTKMet') and self.cfg_ana.additionalTKMet ):
        self.handles['tkmetABC'] = AutoHandle('tkmetABC','std::vector<reco::PFMET>')
        self.handles['tkmetABC050'] = AutoHandle('tkmetABC050','std::vector<reco::PFMET>')
        self.handles['tkmet050'] = AutoHandle('tkmet050','std::vector<reco::PFMET>')
        self.handles['tkmet075'] = AutoHandle('tkmet075','std::vector<reco::PFMET>')
        self.handles['tkmet100'] = AutoHandle('tkmet100','std::vector<reco::PFMET>')
        self.handles['tkmetAB'] = AutoHandle('tkmetAB','std::vector<reco::PFMET>')
        self.handles['tkmetAB050'] = AutoHandle('tkmetAB050','std::vector<reco::PFMET>')
        if(self.cfg_comp.isMC ):
          self.handles['gentkmet'] = AutoHandle('gentkmet','std::vector<reco::PFMET>')
          self.handles['genpfmet'] = AutoHandle('genpfmet','std::vector<reco::PFMET>')
        
      self.handles['muons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )

#      self.handles['electrons'] = AutoHandle(
#            'cmgElectronSel',
#            'std::vector<cmg::Electron>'        

#      self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
#            'std::vector<reco::GenParticle>' )

#      self.handles['vertices'] =  AutoHandle(
#          # 'offlinePrimaryVertices', # earlier than PATCMG_v5_18_0
#          'slimmedPrimaryVertices', # later than PATCMG_v5_18_0
#          'std::vector<reco::Vertex>'
#          )
      self.mchandles['pusi'] =  AutoHandle(
          'addPileupInfo',
          'std::vector<PileupSummaryInfo>' 
          ) 
      self.mchandles['generator'] = AutoHandle(
          'generator','GenEventInfoProduct' 
          )
        

    def declareHandles(self):
        super(CoreTreeProducer, self).declareHandles()
        self.declareCoreHandles()
     
    def declareCoreVariables(self, isMC):
        tr = self.tree

        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
            var( tr, 'run', int)
            var( tr, 'lumi', int)
            var( tr, 'evt', int)
            if isMC:
                var( tr, 'xsec', float)

        var( tr, 'nvtx', int)
        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
            var( tr, 'evtHasGoodVtx', int)
            var( tr, 'Vtx_ndof', int)
            # var( tr, 'firstVtxIsGood', int)

        ###--------------------------- BOOK MC infos ------------------------------
        
        if ( isMC and not hasattr(self.cfg_ana,'storeSlimGenInfo') ):
            var( tr, 'npu', int)
                  
            var(tr, 'scalePDF')
            var(tr, 'parton1_pdgId')
            var(tr, 'parton1_x')
            var(tr, 'parton2_pdgId')
            var(tr, 'parton2_x')        

            if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
                # print "booking tree"
                bookLHE_weight(tr,'LHE' )

        ###--------------------------- BOOK MET infos ------------------------------

        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          bookCustomMET( tr, 'pfmet')
        bookCustomMET( tr, 'tkmet')
        if(hasattr(self.cfg_ana,'additionalTKMet') and self.cfg_ana.additionalTKMet ):
          bookCustomMET( tr, 'tkmetABC')
          bookCustomMET( tr, 'tkmetABC050')
          bookCustomMET( tr, 'tkmet050')
          bookCustomMET( tr, 'tkmet075')
          bookCustomMET( tr, 'tkmet100')
          bookCustomMET( tr, 'tkmetAB')
          bookCustomMET( tr, 'tkmetAB050')
        if ( isMC ):
            bookCustomMET(tr, 'gentkmet')
            # var(tr, 'gentkmet_sumEt')
            bookCustomMET(tr, 'genpfmet')
            # var(tr, 'genpfmet_sumEt')            

        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          bookCustomMET( tr, 'nopumet')
          bookCustomMET( tr, 'pumet')
          bookCustomMET( tr, 'pucmet')
          bookCustomMET( tr, 'pfMetForRegression')
          bookCustomMET( tr, 'pfMetraw')

        # if ( not hasattr(self.cfg_ana,'storeSlimGenInfo') ):
            # bookCustomMET( tr, 'pfmetraw')

#        if ( not hasattr(self.cfg_ana,'storeSlimGenInfo') ):        
#            var( tr, 'pfmetcov00')
#            var( tr, 'pfmetcov01')
#            var( tr, 'pfmetcov10')
#            var( tr, 'pfmetcov11')
        

    def declareVariables(self):
        tr = self.tree
        self.declareCoreVariables()

    def fillCoreVariables(self, tr, iEvent, event, isMC):

        fill( tr, 'run', event.run)
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)
        if isMC:
            fill( tr, 'xsec',getattr(self.cfg_comp,'xSection',1.0))

        fill( tr, 'nvtx', len(event.goodVertices))
        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          fill( tr, 'evtHasGoodVtx', event.passedVertexAnalyzer)
          if(event.passedVertexAnalyzer):
              fill( tr, 'Vtx_ndof', event.goodVertices[0].ndof())
            # print 'Size goodVertices:',len(event.goodVertices),' allVertices',len(self.handles['vertices'].product())
            # fill( tr, 'firstVtxIsGood', event.firstVtxIsGoodVertices) # REQUIRES DEFINITION IN CMGTools/RootTools/python/analyzers/VertexAnalyzer.py


        ###------------------------------- FILL event MC weights ------------------------------

## MARIA: this was the conditions in the ZTreeProducer
##      if (self.cfg_comp.isMC and event.savegenpZ) and not hasattr(self.cfg_ana,'storeSlimGenInfo'):
        if ( isMC and not hasattr(self.cfg_ana,'storeSlimGenInfo')):

            if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
                # print "filling tree"
                fillLHE_weight( tr,'LHE' ,event.LHE_weights ,event)
            
            event.pileUpInfo = map( PileUpSummaryInfo,
                                    self.mchandles['pusi'].product() )
            for puInfo in event.pileUpInfo:
                if puInfo.getBunchCrossing()==0:
                    fill( tr, 'npu', puInfo.nTrueInteractions())
                # print 'puInfo.nTrueInteractions()= ',puInfo.nTrueInteractions()
                # else:
                # print 'NO INFO FOR puInfo.getBunchCrossing()==0 !!!!'
                
            
            event.generator = self.mchandles['generator'].product()
            # print 'WTreeProducer.py: ',event.generator.pdf().scalePDF,' ',event.generator.pdf().id.first,' ',event.generator.pdf().x.first,' ',event.generator.pdf().id.second,' ',event.generator.pdf().x.second
            fill(tr, 'scalePDF',float(event.generator.pdf().scalePDF))
            fill(tr, 'parton1_pdgId',float(event.generator.pdf().id.first))
            fill(tr, 'parton1_x',float(event.generator.pdf().x.first))
            fill(tr, 'parton2_pdgId',float(event.generator.pdf().id.second))
            fill(tr, 'parton2_x',float(event.generator.pdf().x.second))

        ###--------------------------- FILL MET ------------------------------

## MARIA: this was the conditions in the ZTreeProducer
## if (event.savegenpZ and self.cfg_comp.isMC) or event.ZGoodEvent:
## for now filling for all the events
            
        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          fillCustomMET(tr, 'pfmet', event.pfmet)
          if(hasattr(self.cfg_ana,'additionalTKMet') and self.cfg_ana.additionalTKMet ):
            event.tkmet = self.handles['tkmetABC'].product()[0]
            fillCustomMET(tr, 'tkmetABC', event.tkmet)
            event.tkmet = self.handles['tkmetABC050'].product()[0]
            fillCustomMET(tr, 'tkmetABC050', event.tkmet)
            event.tkmet = self.handles['tkmet050'].product()[0]
            fillCustomMET(tr, 'tkmet050', event.tkmet)
            event.tkmet = self.handles['tkmet075'].product()[0]
            fillCustomMET(tr, 'tkmet075', event.tkmet)
            event.tkmet = self.handles['tkmet100'].product()[0]
            fillCustomMET(tr, 'tkmet100', event.tkmet)
            event.tkmet = self.handles['tkmetAB'].product()[0]
            fillCustomMET(tr, 'tkmetAB', event.tkmet)
            event.tkmet = self.handles['tkmetAB050'].product()[0]
            fillCustomMET(tr, 'tkmetAB050', event.tkmet)
            if (isMC) :
                event.tkmet = self.handles['gentkmet'].product()[0]
                fillCustomMET(tr, 'gentkmet', event.tkmet)
                # fill( tr, 'gentkmet_sumEt', event.genTkSumEt)
                event.tkmet = self.handles['genpfmet'].product()[0]
                fillCustomMET(tr, 'genpfmet', event.tkmet)
                # fill( tr, 'genpfmet_sumEt', event.genPfSumEt)            
        event.tkmet = self.handles['tkMet'].product()[0]
        fillCustomMET(tr, 'tkmet', event.tkmet)


#        if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):        
#            pfMetSignificance = self.handles['pfMetSignificance'].product().significance()
#            fill( tr, 'pfmetcov00', pfMetSignificance(0,0))
#            fill( tr, 'pfmetcov01', pfMetSignificance(0,1))
#            fill( tr, 'pfmetcov10', pfMetSignificance(1,0))
#            fill( tr, 'pfmetcov11', pfMetSignificance(1,1))
            
            
        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          event.nopumet = self.handles['nopuMet'].product()[0]
          event.pucmet = self.handles['pucMet'].product()[0]
          event.pfMetForRegression = self.handles['pfMetForRegression'].product()[0]
          event.pumet = self.handles['puMet'].product()[0]
          event.pfMetraw = self.handles['pfMetraw'].product()[0]
          fillCustomMET(tr, 'nopumet', event.nopumet)
          fillCustomMET(tr, 'pucmet', event.pucmet)
          fillCustomMET(tr, 'pumet', event.pumet)
          fillCustomMET(tr, 'pfMetForRegression', event.pfMetForRegression)
          fillCustomMET(tr, 'pfMetraw', event.pfMetraw)
        
            
def process(self, iEvent, event):

        isMC = self.cfg_comp.isMC 
        self.readCollections( iEvent )
        tr = self.tree
        tr.reset()

        self.fillCoreVariables(self.tree, iEvent, event, isMC)

        
        
