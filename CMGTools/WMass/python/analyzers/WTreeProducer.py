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

def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))

def bookW( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def bookMET( tree, pName ):
    var(tree, '{pName}'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_sumEt'.format(pName=pName))

def bookJet( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )

def fillW( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.Pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.Phi() )

def fillMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_sumEt'.format(pName=pName), particle.sumEt() )
    
def fillJet( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )



class WTreeProducer( TreeAnalyzerNumpy ):
    
    MuonClass = Muon 

    
    def declareHandles(self):
      super(WTreeProducer, self).declareHandles()
    
      self.handles['pfmet'] = AutoHandle(
        'cmgPFMET',
        'std::vector<cmg::BaseMET>' 
        )
      self.handles['muons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )
      self.handles['electrons'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
      self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
            'std::vector<reco::GenParticle>' )
    
      self.handles['vertices'] =  AutoHandle(
          'offlinePrimaryVertices',
          'std::vector<reco::Vertex>'
          )
      self.mchandles['pusi'] =  AutoHandle(
          'addPileupInfo',
          'std::vector<PileupSummaryInfo>' 
          ) 
    
    def declareVariables(self):
      tr = self.tree

      var( tr, 'run', int)
      var( tr, 'lumi', int)
      var( tr, 'evt', int)
      var( tr, 'nvtx', int)
      var( tr, 'njets', int)
      var( tr, 'npu', int)
      var( tr, 'evtHasGoodVtx', int)
      var( tr, 'evtHasTrg', int)
      var( tr, 'evtWSel', int)
      
      var( tr, 'nMuons', int)
      var( tr, 'nTrgMuons', int)
      var( tr, 'noTrgMuonsLeadingPt', int)

      bookMET( tr, 'pfmet')
      
      bookW( tr, 'W')
      var( tr, 'W_mt')
      bookW( tr, 'WGen')
      var( tr, 'WGen_rap')
      var( tr, 'WGen_m')
      var( tr, 'WGen_mt')
      var( tr, 'u1')
      var( tr, 'u2')

      bookParticle(tr, 'Mu')
      var(tr, 'Mu_dxy')
      var(tr, 'MuIsTightAndIso', int)
      var(tr, 'MuIsTight', int)
      var(tr, 'MuRelIso')
      bookParticle(tr, 'MuGen')
      bookParticle(tr, 'MuGenStatus1')
      var(tr, 'MuDRGenP')
      
      bookParticle(tr, 'NuGen')
      
      bookJet(tr, 'Jet_leading')

       
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        tr = self.tree
        tr.reset()
          
        if (event.savegenpW and self.cfg_comp.isMC):
        
          fillW(tr,'WGen', event.genW[0].p4())
          fill(tr, 'WGen_rap', event.genW[0].p4().Rapidity())
          fill(tr, 'WGen_m', event.genW[0].p4().M())
          fill(tr, 'WGen_mt', event.genW_mt)
          fillParticle(tr, 'MuGen',event.genMu[0])
          fillParticle(tr, 'MuGenStatus1', event.genMuStatus1[0])      
          fill(tr, 'MuDRGenP',event.muGenDeltaRgenP)
          fillParticle(tr, 'NuGen', event.genNu[0])

        if event.WGoodEvent == True :
                      
          fillW( tr, 'W',event.W4V)
          fill(tr, 'W_mt', event.W4V_mt)
          fill(tr, 'u1', event.u1)
          fill(tr, 'u2', event.u2)

          fillParticle(tr, 'Mu', event.selMuons[0])
          if ( event.selMuons[0].isGlobalMuon() or event.selMuons[0].isTrackerMuon() ) and event.passedVertexAnalyzer:
            fill(tr, 'Mu_dxy', math.fabs(event.selMuons[0].dxy()))
          fill(tr, 'MuIsTightAndIso', event.selMuonIsTightAndIso)
          fill(tr, 'MuIsTight', event.selMuonIsTight)
          fill(tr, 'MuRelIso', event.selMuons[0].relIso(0.5))

          
        if (event.savegenpW and self.cfg_comp.isMC) or event.WGoodEvent:
          fill( tr, 'run', event.run) 
          fill( tr, 'lumi',event.lumi)
          fill( tr, 'evt', event.eventId)
          fill( tr, 'nvtx', len(self.handles['vertices'].product()))          
          fill( tr, 'njets', len(event.selJets))
          if (self.cfg_comp.isMC) :
            event.pileUpInfo = map( PileUpSummaryInfo,
                                    self.mchandles['pusi'].product() )
            for puInfo in event.pileUpInfo:
              if puInfo.getBunchCrossing()==0:
                fill( tr, 'npu', puInfo.nTrueInteractions())
                # print 'puInfo.nTrueInteractions()= ',puInfo.nTrueInteractions()
              # else:
                # print 'NO INFO FOR puInfo.getBunchCrossing()==0 !!!!'
            
          fill( tr, 'nMuons', event.nMuons)
          fill( tr, 'nTrgMuons', len(event.selMuons))
          if len(event.NoTriggeredMuonsLeadingPt) > 0 :
            fill( tr, 'noTrgMuonsLeadingPt', event.NoTriggeredMuonsLeadingPt[0].pt())
          fill( tr, 'evtHasGoodVtx', event.passedVertexAnalyzer)
          fill( tr, 'evtHasTrg', event.passedTriggerAnalyzer)
          fill( tr, 'evtWSel', event.WGoodEvent)
          fillMET(tr, 'pfmet', event.pfmet)
          if len(event.selJets)>0:
            fillJet(tr, 'Jet_leading', event.selJets[0])
            
          self.tree.tree.Fill()
       
