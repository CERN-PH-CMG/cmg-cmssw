import numpy as my_n
import math
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.analyzers.ntuple import *
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton, Muon, Electron
from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.RootTools.physicsobjects.PileUpSummaryInfo import PileUpSummaryInfo
#from ZAnalyzer import testLegID


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

def bookJetCollections( tree, pName ):
    var(tree, '{pName}_number'.format(pName=pName),int)
    tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[10]/D' )

def bookLeptonCollections( tree, pName ):
    var(tree, '{pName}_number'.format(pName=pName),int)
   
    tree.vars['{pName}_charge'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_charge'.format(pName=pName),tree.vars['{pName}_charge'.format(pName=pName)] ,'{pName}_charge'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_ID'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_ID'.format(pName=pName),tree.vars['{pName}_ID'.format(pName=pName)] ,'{pName}_ID'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_ID_8TeV'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_ID_8TeV'.format(pName=pName),tree.vars['{pName}_ID_8TeV'.format(pName=pName)] ,'{pName}_ID_8TeV'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_Iso'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_Iso'.format(pName=pName),tree.vars['{pName}_Iso'.format(pName=pName)] ,'{pName}_Iso'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_IsPromt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_IsPromt'.format(pName=pName),tree.vars['{pName}_IsPromt'.format(pName=pName)] ,'{pName}_IsPromt'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_IsTrig'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_IsTrig'.format(pName=pName),tree.vars['{pName}_IsTrig'.format(pName=pName)] ,'{pName}_IsTrig'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[10]/D' )
    #  tree.vars['{pName}_Zmatch'.format(pName=pName)]= my_n.zeros(10, dtype=int)
    #  tree.tree.Branch('{pName}_Zmatch'.format(pName=pName),tree.vars['{pName}_Zmatch'.format(pName=pName)] ,'{pName}_Zmatch'.format(pName=pName)+'[10]/I' )


def bookElectrons( tree, pName ):
    var(tree, '{pName}_number'.format(pName=pName),int)
    tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_TightID'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_TightID'.format(pName=pName),tree.vars['{pName}_TightID'.format(pName=pName)] ,'{pName}_TightID'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_TightIso'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_TightIso'.format(pName=pName),tree.vars['{pName}_TightIso'.format(pName=pName)] ,'{pName}_TightIso'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_charge'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_charge'.format(pName=pName),tree.vars['{pName}_charge'.format(pName=pName)] ,'{pName}_charge'.format(pName=pName)+'[10]/D' )
    tree.vars['{pName}_IsPromt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    tree.tree.Branch('{pName}_IsPromt'.format(pName=pName),tree.vars['{pName}_IsPromt'.format(pName=pName)] ,'{pName}_IsPromt'.format(pName=pName)+'[10]/D' )

def bookcmgPFcands( tree, pName ):
    var(tree, '{pName}_number'.format(pName=pName),int)
    tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[5000]/D' )
    tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[5000]/D' )
    tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[5000]/D' )
    tree.vars['{pName}_pdgId'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    tree.tree.Branch('{pName}_pdgId'.format(pName=pName),tree.vars['{pName}_pdgId'.format(pName=pName)] ,'{pName}_pdgId'.format(pName=pName)+'[5000]/D' )
    tree.vars['{pName}_fromPV'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    tree.tree.Branch('{pName}_fromPV'.format(pName=pName),tree.vars['{pName}_fromPV'.format(pName=pName)] ,'{pName}_fromPV'.format(pName=pName)+'[5000]/D' )
    tree.vars['{pName}_dZfromPV'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    tree.tree.Branch('{pName}_dZfromPV'.format(pName=pName),tree.vars['{pName}_dZfromPV'.format(pName=pName)] ,'{pName}_dZfromPV'.format(pName=pName)+'[5000]/D' )

def bookPFJets( tree, pName ):
    var(tree, '{pName}_number'.format(pName=pName),int)
    tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[100]/D' )
    tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[100]/D' )
    tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[100]/D' )
    tree.vars['{pName}_pdgId'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # tree.tree.Branch('{pName}_pdgId'.format(pName=pName),tree.vars['{pName}_pdgId'.format(pName=pName)] ,'{pName}_pdgId'.format(pName=pName)+'[100]/D' )
    # tree.vars['{pName}_fromPV'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # tree.tree.Branch('{pName}_fromPV'.format(pName=pName),tree.vars['{pName}_fromPV'.format(pName=pName)] ,'{pName}_fromPV'.format(pName=pName)+'[100]/D' )
    # tree.vars['{pName}_dZfromPV'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # tree.tree.Branch('{pName}_dZfromPV'.format(pName=pName),tree.vars['{pName}_dZfromPV'.format(pName=pName)] ,'{pName}_dZfromPV'.format(pName=pName)+'[100]/D' )

def bookMuonCovMatrix( tree, pName ):
    tree.vars['{pName}CovMatrix'.format(pName=pName)]= my_n.zeros(9, dtype=float)
    tree.tree.Branch('{pName}CovMatrix'.format(pName=pName),tree.vars['{pName}CovMatrix'.format(pName=pName)] ,'{pName}CovMatrix'.format(pName=pName)+'[9]/D' )



def bookW( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def bookZ( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_rap'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))

def bookMET( tree, pName ):
    var(tree, '{pName}'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def bookCustomMET( tree, pName ):
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

def fillZ( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.Pt() )
    fill(tree, '{pName}_rap'.format(pName=pName), particle.Rapidity() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.Phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.M() )

def fillMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

def fillCustomMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle[0].Pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle[0].Phi() )
    fill(tree, '{pName}_sumEt'.format(pName=pName), particle[1] )

def fillJet( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

def fillJets( tree, pName, particles ):
    fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    for i in range(0, min(len(particles),10)):
        tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()

def fillMuons( tree, pName, particles , event):
    fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    for i in range(0, min(len(particles),10)):
        
        tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        tree.vars['{pName}_charge'.format(pName=pName)][i] = float(particles[i].charge())
        tree.vars['{pName}_ID'.format(pName=pName)][i] = event.ZallMuonsID[i]
        tree.vars['{pName}_ID_8TeV'.format(pName=pName)][i] = event.ZallMuonsID_8TeV[i]
        tree.vars['{pName}_Iso'.format(pName=pName)][i] = particles[i].relIso(0.5)
        tree.vars['{pName}_IsTrig'.format(pName=pName)][i] = int(event.ZallMuonsTrig[i])
def fillMuonsGen( tree, pName, particles , event):
    for i in range(0, min(len(particles),10)):
        #print i, ' len ', len(particles)
        tree.vars['{pName}_IsPromt'.format(pName=pName)][i] = event.ZallMuonsMatched[i]

def fillElectrons( tree, pName, particles,event ):
    fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    for i in range(0, min(len(particles),10)):
        tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        tree.vars['{pName}_TightID'.format(pName=pName)][i] = event.ZElTightID[i]
        tree.vars['{pName}_TightIso'.format(pName=pName)][i] = event.ZElTightIso[i]
        tree.vars['{pName}_charge'.format(pName=pName)][i] = particles[i].charge()

def fillcmgPFcands( tree, pName, particles,vertex,event ):
    fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    for i in range(0, min(len(particles),2000)):
        # print particles[i].pdgId(), math.fabs(particles[i].vertex().z()-vertex.z())
        tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        tree.vars['{pName}_pdgId'.format(pName=pName)][i] = particles[i].pdgId()
        tree.vars['{pName}_fromPV'.format(pName=pName)][i] = particles[i].fromPV()
        tree.vars['{pName}_dZfromPV'.format(pName=pName)][i] = math.fabs(particles[i].vertex().z()-vertex.z())

def fillPFJets( tree, pName, particles,event ):
    fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    for i in range(0, min(len(particles),100)):
        # print particles[i].pdgId(), math.fabs(particles[i].vertex().z()-vertex.z())
        tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        # print 'particles[',i,'].mva() ', particles[i].mva()
        # tree.vars['{pName}_pdgId'.format(pName=pName)][i] = particles[i].pdgId()
        # tree.vars['{pName}_fromPV'.format(pName=pName)][i] = particles[i].fromPV()
        # tree.vars['{pName}_dZfromPV'.format(pName=pName)][i] = math.fabs(particles[i].vertex().z()-vertex.z())

def fillNeutralcmgPFcands( tree, pName, particles,vertex,event ):
    jneu=0
    for i in range(0, min(len(particles),2500)):
          # case 211: return h;
          # case 11: return e;
          # case 13: return mu;
          # case 22: return gamma;
          # case 130: return h0;
          # case 1: return h_HF;
          # case 2: return egamma_HF;
          # case 0: return X;  
        if(math.fabs(particles[i].pdgId())==130 or math.fabs(particles[i].pdgId())==1 \
           or math.fabs(particles[i].pdgId())==2 or math.fabs(particles[i].pdgId())==22):
            tree.vars['{pName}_pt'.format(pName=pName)][jneu] = particles[i].pt()
            tree.vars['{pName}_eta'.format(pName=pName)][jneu] = particles[i].eta()
            tree.vars['{pName}_phi'.format(pName=pName)][jneu] = particles[i].phi()
            tree.vars['{pName}_pdgId'.format(pName=pName)][jneu] = particles[i].pdgId()
            tree.vars['{pName}_fromPV'.format(pName=pName)][jneu] = particles[i].fromPV()
            tree.vars['{pName}_dZfromPV'.format(pName=pName)][i] = math.fabs(particles[i].vertex().z()-vertex.z())
            jneu = jneu+1
    
    fill(tree, '{pName}_number'.format(pName=pName),jneu)
        
def fillMuonCovMatrix( tree, pName, covMatrix,event ):
    for i in range(0,9):
        tree.vars['{pName}CovMatrix'.format(pName=pName)][i] = covMatrix[i]

def fillElectronsGen(tree, pName, particles,event ):
    for i in range(0, min(len(particles),10)):
        tree.vars['{pName}_IsPromt'.format(pName=pName)][i] = event.ZElIsPromt[i] 

class ZTreeProducer( TreeAnalyzerNumpy ):
    
    MuonClass = Muon 
        
    def declareHandles(self):
      super(ZTreeProducer, self).declareHandles()
    
      self.handles['pfMet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>')
      self.handles['pfMetraw'] = AutoHandle('cmgPFMETRaw','std::vector<cmg::BaseMET>')
      self.handles['pfMetSignificance'] = AutoHandle('pfMetSignificance','cmg::METSignificance')
      self.handles['nopuMet'] = AutoHandle('nopuMet','std::vector<reco::PFMET>')
      self.handles['pucMet'] = AutoHandle('pcMet','std::vector<reco::PFMET>')
      self.handles['pfMetForRegression'] = AutoHandle('pfMetForRegression','std::vector<reco::PFMET>')
      self.handles['puMet'] = AutoHandle('puMet','std::vector<reco::PFMET>')
      self.handles['tkMet'] = AutoHandle('tkMet','std::vector<reco::PFMET>')

      customMetFlavor_str = [ 'tkmethemu' , 'h0','gamma','hegammaHF' ]
      customMetEtaBinEdge_str = ['m5p0','m3p0','m2p4','m2p1','m1p4','1p4','2p1','2p4','3p0','5p0']
      customMetPtBinEdge_str = ['0p0','0p5','1p0','1p5','2p0','3p0','5p0','10p0']

      if hasattr(self.cfg_ana,'storeMyCustomMets'):
        for particleId in xrange(len(customMetFlavor_str)):
          for EtaBinEdge in xrange(len(customMetEtaBinEdge_str)-1):
            for PtBinEdge in xrange(len(customMetPtBinEdge_str)):
              if( ( ('gamma' is not customMetFlavor_str[particleId]) and ('h0' not in customMetFlavor_str[particleId]) ) and (PtBinEdge > 4) ): continue
              if( ('h0' not in customMetFlavor_str[particleId]) and (PtBinEdge > 5) ): continue
              # print 'my%sPFmetPt0p0EtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])
              self.handles['my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])] = AutoHandle(
                   'my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1]),
                   'std::vector<reco::PFMET>' )
      
      if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
        self.handles['cmgCandidates'] = AutoHandle('cmgCandidates','std::vector<cmg::Candidate>')
        # self.handles['kt6PFJets'] = AutoHandle('kt6PFJets','std::vector<reco::PFJet>')
              
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
      self.mchandles['generator'] = AutoHandle(
          'generator','GenEventInfoProduct' 
          )

    
    def declareVariables(self):
      tr = self.tree

      if not hasattr(self.cfg_ana,'storeSlimGenInfo'):
        var(tr, 'scalePDF')
        var(tr, 'parton1_pdgId')
        var(tr, 'parton1_x')
        var(tr, 'parton2_pdgId')
        var(tr, 'parton2_x')
        var(tr, 'FSRWeight')
      
      var( tr, 'run', int)
      var( tr, 'lumi', int)
      var( tr, 'evt', int)
      var( tr, 'nvtx', int)
      var( tr, 'njets', int)
      var( tr, 'npu', int)
      var( tr, 'evtHasGoodVtx', int)
      var( tr, 'Vtx_ndof', int)
      # var( tr, 'firstVtxIsGood', int)
      var( tr, 'evtHasTrg', int)
      var( tr, 'evtZSel', int)
      
      var( tr, 'nMuons', int)
      var( tr, 'nTrgMuons', int)
      var( tr, 'nNoTrgMuons', int)
      var( tr, 'noTrgExtraMuonsLeadingPt', int)

      # # customMetFlavor_str =    [ 'h' ,  'h0','gamma','hf' ,'ele','mu']
      # # customMetPtMin_str = ['0p0','0p5','1p0','1p5','2p0']
      # # customMetEtaMin_str = ['0p0','1p4','2p1','2p5','3p0']
      # # customMetEtaMax_str = ['1p4','2p1','2p5','3p0','5p0']
      # customMetFlavor_str =    ['h0','gamma','hf']
      # customMetPtMin_str = ['0p0','1p0','2p0']
      # customMetEtaMin_str = ['0p0','1p4','2p5','3p0']
      # customMetEtaMax_str = ['1p4','2p5','3p0','5p0']

      # for iFlavor in xrange(len(customMetFlavor_str)):
        # for iPtMin in xrange(len(customMetPtMin_str)):
          # for jEtaMax in xrange(len(customMetEtaMax_str)):
            # bookCustomMET(tr, \
                              # 'met'+customMetFlavor_str[iFlavor]+'Pt'+customMetPtMin_str[iPtMin]+\
                              # 'EtaMin'+customMetEtaMin_str[jEtaMax]+'EtaMax'+customMetEtaMax_str[jEtaMax] )
      
      customMetFlavor_str = [ 'tkmethemu' , 'h0','gamma','hegammaHF' ]
      customMetEtaBinEdge_str = ['m5p0','m3p0','m2p4','m2p1','m1p4','1p4','2p1','2p4','3p0','5p0']
      customMetPtBinEdge_str = ['0p0','0p5','1p0','1p5','2p0','3p0','5p0','10p0']
      if hasattr(self.cfg_ana,'storeMyCustomMets'):
        for particleId in xrange(len(customMetFlavor_str)):
          for EtaBinEdge in xrange(len(customMetEtaBinEdge_str)-1):
            for PtBinEdge in xrange(len(customMetPtBinEdge_str)):            
              if( ( ('gamma' is not customMetFlavor_str[particleId]) and ('h0' not in customMetFlavor_str[particleId]) ) and (PtBinEdge > 4) ): continue
              if( ('h0' not in customMetFlavor_str[particleId]) and (PtBinEdge > 5) ): continue

              # print 'my%sPFmetPt0p0EtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])
              bookMET(tr, 'my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1]) )
              var(tr, 'my%sPFmetPt%sEtaMin%sEtaMax%s_sumEt'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1]) )
            
      bookMET(tr, 'pfmet')
      # bookMET(tr, 'pfmet2')
      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        bookMET(tr, 'pfmetraw')
      bookMET(tr, 'nopumet')
      bookMET(tr, 'pucmet')
      bookMET(tr, 'pfMetForRegression')
      bookMET(tr, 'pumet')
      bookMET(tr, 'tkmet')
      var(tr, 'pfmet_sumEt')
      var(tr, 'pfmetraw_sumEt')
      var(tr, 'nopumet_sumEt')
      var(tr, 'pucmet_sumEt')
      var(tr, 'pfMetForRegression_sumEt')
      var(tr, 'pumet_sumEt')
      var(tr, 'tkmet_sumEt')

      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        var( tr, 'pfmetcov00')
        var( tr, 'pfmetcov01')
        var( tr, 'pfmetcov10')
        var( tr, 'pfmetcov11')

      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        bookMET(tr, 'pfmetWlikeNeg')
        bookMET(tr, 'pfmetWlikePos')
        bookW(tr, 'WlikePos')
        var(tr, 'WlikePos_mt')
        bookW(tr, 'WlikeNeg')
        var(tr, 'WlikeNeg_mt')
        
      bookZ(tr, 'Z')
      var(tr, 'Z_mt')
      var(tr, 'pt_vis')
      var(tr, 'phi_vis')
      if not hasattr(self.cfg_ana,'storeSlimGenInfo'):
        bookZ(tr, 'ZGen')      
      bookZ(tr, 'ZGen_PostFSR')      
      var(tr, 'ZGen_mt')      
      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        var( tr, 'u1')
        var( tr, 'u2')
      
      bookParticle(tr, 'MuPos')
      var(tr, 'MuPos_dxy')
      var(tr, 'MuPosRelIso')
      var(tr, 'MuPosTrg', int)
      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        var(tr, 'MuPosIsTightAndIso', int)
      var(tr, 'MuPosIsTight', int)
      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        var(tr, 'MuPosMatchCMGmuon', int)
      var(tr, 'MuPos_dz')
      bookParticle(tr, 'MuPosGen')
      bookParticle(tr, 'MuPosGenStatus1')
      # var(tr, 'MuPosGen_pdgId', int)
      bookParticle(tr, 'MuNeg')
      var(tr, 'MuNeg_dxy')
      var(tr, 'MuNegRelIso')
      var(tr, 'MuNegTrg', int)
      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        var(tr, 'MuNegIsTightAndIso', int)
      var(tr, 'MuNegIsTight', int)
      if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        var(tr, 'MuNegMatchCMGmuon', int)
      var(tr, 'MuNeg_dz')
      bookParticle(tr, 'MuNegGen')
      bookParticle(tr, 'MuNegGenStatus1')
      # var(tr, 'MuNegGen_pdgId', int)
      if not hasattr(self.cfg_ana,'storeSlimGenInfo'):
        var(tr, 'MuPosDRGenP')
        var(tr, 'MuNegDRGenP')
      bookMuonCovMatrix(tr,'MuPos' )
      bookMuonCovMatrix(tr,'MuNeg' )
      bookJet(tr, 'Jet_leading')
      #print 'booking stuff'
      # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # bookJetCollections(tr,'cmgjets' )
        # bookLeptonCollections(tr,'cmgmuons' )
        # bookElectrons(tr,'cmgelectrons' )
      if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
        bookcmgPFcands(tr,'cmgCandidates' )
        # bookPFJets(tr,'kt6PFJets' )
     # print 'booked stuff'

    def process(self, iEvent, event):

        self.readCollections( iEvent )
        tr = self.tree
        tr.reset()
       # print 'ola'   
        # print 'event.savegenpZ= ',event.savegenpZ,' self.cfg_comp.isMC= ',self.cfg_comp.isMC,' event.ZGoodEvent= ',event.ZGoodEvent

        # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
          # fillMuons(tr, 'cmgmuons', event.ZallMuons, event)
          # fillElectrons( tr,'cmgelectrons' ,event.ZselElectrons ,event)
          # fillJets(tr, 'cmgjets', event.ZselJets)
        
        fill( tr, 'run', event.run) 
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)
        fill( tr, 'nvtx', len(self.handles['vertices'].product()))
        fill( tr, 'evtHasGoodVtx', event.passedVertexAnalyzer)
        fill( tr, 'evtHasTrg', event.passedTriggerAnalyzer)

        
        if (self.cfg_comp.isMC and event.savegenpZ) and not hasattr(self.cfg_ana,'storeSlimGenInfo'):
            fill(tr, 'FSRWeight',event.fsrWeight)
            event.pileUpInfo = map( PileUpSummaryInfo,
                                    self.mchandles['pusi'].product() )
            for puInfo in event.pileUpInfo:
                if puInfo.getBunchCrossing()==0:
                    fill( tr, 'npu', puInfo.nTrueInteractions())
                # print 'puInfo.nTrueInteractions()= ',puInfo.nTrueInteractions()
            event.generator = self.mchandles['generator'].product()
            # print 'ZTreeProducer.py: ',event.generator.pdf().scalePDF,' ',event.generator.pdf().id.first,' ',event.generator.pdf().x.first,' ',event.generator.pdf().id.second,' ',event.generator.pdf().x.second
            fill(tr, 'scalePDF',event.generator.pdf().scalePDF)
            fill(tr, 'parton1_pdgId',event.generator.pdf().id.first)
            fill(tr, 'parton1_x',event.generator.pdf().x.first)
            fill(tr, 'parton2_pdgId',event.generator.pdf().id.second)
            fill(tr, 'parton2_x',event.generator.pdf().x.second)
        
        # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
          # fillElectronsGen( tr,'cmgelectrons' ,event.ZElIsPromt ,event) 
          # fillMuonsGen (tr, 'cmgmuons', event.ZallMuonsMatched, event)       
        
        if (event.savegenpZ and self.cfg_comp.isMC):
          fillZ(tr, 'ZGen_PostFSR', event.genZ_PostFSR)
          fill(tr, 'ZGen_mt', event.genZ_mt)
          fillParticle(tr, 'MuPosGenStatus1', event.genMuPosStatus1[0])      
          fillParticle(tr, 'MuNegGenStatus1', event.genMuNegStatus1[0])          
          if not hasattr(self.cfg_ana,'storeSlimGenInfo'):
            fillZ(tr, 'ZGen', event.genZ[0].p4())
            fillParticle(tr, 'MuPosGen', event.genMuPos[0])
            fillParticle(tr, 'MuNegGen', event.genMuNeg[0])
            fill(tr, 'MuPosDRGenP', event.muPosGenDeltaRgenP)              
            fill(tr, 'MuNegDRGenP', event.muNegGenDeltaRgenP)
         
        if event.ZGoodEvent == True :
                                
          if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
            cmgPFcands = self.handles['cmgCandidates'].product()
            # kt6PFJets = self.handles['kt6PFJets'].product()
            # fillPFJets(tr, 'kt6PFJets', kt6PFJets, event)
          if hasattr(self.cfg_ana,'storeCMGcandidates'):
            fillcmgPFcands( tr,'cmgCandidates' ,cmgPFcands, event.goodVertices[0], event)
          if hasattr(self.cfg_ana,'storeNeutralCMGcandidates'):
            fillNeutralcmgPFcands( tr,'cmgCandidates' ,cmgPFcands ,event.goodVertices[0], event)
            
          fillZ(tr, 'Z', event.Z4V)
          fill(tr, 'pt_vis', event.Z4V.Pt())
          fill(tr, 'phi_vis', event.Z4V.Phi())
          fill(tr,  'Z_mt', event.Z4V_mt)
          if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            fill(tr, 'u1', event.Zu1)
            fill(tr, 'u2', event.Zu2)
            fillW(tr, 'WlikePos', event.Wpos4VfromZ)
            fill(tr,  'WlikePos_mt', event.Wpos4VfromZ_mt)
            fillMET(tr, 'pfmetWlikePos', event.ZpfmetWpos)
            fillW(tr, 'WlikeNeg', event.Wneg4VfromZ)
            fill(tr,  'WlikeNeg_mt', event.Wneg4VfromZ_mt)
            fillMET(tr, 'pfmetWlikeNeg', event.ZpfmetWneg)
          
          fillParticle(tr, 'MuPos', event.BestZPosMuon)
          if ( event.BestZPosMuon.isGlobalMuon() or event.BestZPosMuon.isTrackerMuon() ) and event.passedVertexAnalyzer:
            fill(tr, 'MuPos_dxy', math.fabs(event.BestZPosMuon.dxy()))
            fill(tr, 'MuPos_dz',math.fabs(event.BestZPosMuon.dz()))
          fill(tr, 'MuPosRelIso', event.BestZPosMuon.relIso(0.5))
          fill(tr, 'MuPosTrg', event.BestZPosMuonHasTriggered)
          if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            fill(tr, 'MuPosIsTightAndIso',event.BestZPosMuonIsTightAndIso)
          fill(tr, 'MuPosIsTight',event.BestZPosMuonIsTight)
          if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            fill(tr, 'MuPosMatchCMGmuon',event.BestZPosMatchIndex)

          fillParticle(tr, 'MuNeg', event.BestZNegMuon)
          if ( event.BestZNegMuon.isGlobalMuon() or event.BestZNegMuon.isTrackerMuon() ) and event.passedVertexAnalyzer:
            fill(tr, 'MuNeg_dxy', math.fabs(event.BestZNegMuon.dxy()))
            fill(tr, 'MuNeg_dz',math.fabs(event.BestZNegMuon.dz()))
          fill(tr, 'MuNegRelIso', event.BestZNegMuon.relIso(0.5))
          fill(tr, 'MuNegTrg', event.BestZNegMuonHasTriggered)
          if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            fill(tr, 'MuNegIsTightAndIso',event.BestZNegMuonIsTightAndIso)
          fill(tr, 'MuNegIsTight',event.BestZNegMuonIsTight)
          # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            # fill(tr, 'MuNegMatchCMGmuon',event.BestZNegMatchIndex)
          
          fillMuonCovMatrix( tr,'MuPos' ,event.covMatrixPosMuon ,event)
          fillMuonCovMatrix( tr,'MuNeg' ,event.covMatrixNegMuon ,event)
          
        if (event.savegenpZ and self.cfg_comp.isMC) or event.ZGoodEvent:
          
          fill( tr, 'njets', len(event.ZselJets))
          if(event.passedVertexAnalyzer):
            fill( tr, 'Vtx_ndof', event.goodVertices[0].ndof())
          # fill( tr, 'firstVtxIsGood', event.firstVtxIsGoodVertices) # REQUIRES DEFINITION IN CMGTools/RootTools/python/analyzers/VertexAnalyzer.py
          fill( tr, 'nMuons', len(event.ZallMuons))
          fill( tr, 'nTrgMuons', len(event.ZselTriggeredMuons))
          fill( tr, 'nNoTrgMuons', len(event.ZselNoTriggeredMuons))
          if(len(event.ZselNoTriggeredExtraMuonsLeadingPt)>0):
            fill( tr, 'noTrgExtraMuonsLeadingPt', event.ZselNoTriggeredExtraMuonsLeadingPt[0].pt())
 
          fill( tr, 'evtZSel', event.ZGoodEvent)
          fill(tr, 'pfmet_sumEt', event.pfmet.sumEt())
          fillMET(tr, 'pfmet', event.ZpfmetNoMu)
          # event.pfmet2 = self.handles['pfMet'].product()[0]
          event.pfmetraw = self.handles['pfMetraw'].product()[0]
          if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            pfMetSignificance = self.handles['pfMetSignificance'].product().significance()
            fill( tr, 'pfmetcov00', pfMetSignificance(0,0))
            fill( tr, 'pfmetcov01', pfMetSignificance(0,1))
            fill( tr, 'pfmetcov10', pfMetSignificance(1,0))
            fill( tr, 'pfmetcov11', pfMetSignificance(1,1))

          event.nopumet = self.handles['nopuMet'].product()[0]
          event.pucmet = self.handles['pucMet'].product()[0]
          event.pfMetForRegression = self.handles['pfMetForRegression'].product()[0]
          event.pumet = self.handles['puMet'].product()[0]
          event.tkmet = self.handles['tkMet'].product()[0]
          # fillMET(tr, 'pfmet2', event.pfmet2.p4())
          if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            fillMET(tr, 'pfmetraw', event.pfmetraw.p4())
            fill(tr, 'pfmetraw_sumEt', event.pfmetraw.sumEt())
          fillMET(tr, 'nopumet', event.nopumet.p4())
          fill(tr, 'nopumet_sumEt', event.nopumet.sumEt())
          fillMET(tr, 'pucmet', event.pucmet.p4())
          fill(tr, 'pucmet_sumEt', event.pucmet.sumEt())
          fillMET(tr, 'pfMetForRegression', event.pfMetForRegression.p4())
          fill(tr, 'pfMetForRegression_sumEt', event.pfMetForRegression.sumEt())
          fillMET(tr, 'pumet', event.pumet.p4())
          fill(tr, 'pumet_sumEt', event.pumet.sumEt())
          fillMET(tr, 'tkmet', event.tkmet.p4())
          fill(tr, 'tkmet_sumEt', event.tkmet.sumEt())
          
          customMetFlavor_str = [ 'tkmethemu' , 'h0','gamma','hegammaHF' ]
          customMetEtaBinEdge_str = ['m5p0','m3p0','m2p4','m2p1','m1p4','1p4','2p1','2p4','3p0','5p0']
          customMetPtBinEdge_str = ['0p0','0p5','1p0','1p5','2p0','3p0','5p0','10p0']
          
          if hasattr(self.cfg_ana,'storeMyCustomMets'):
            for particleId in xrange(len(customMetFlavor_str)):
              for EtaBinEdge in xrange(len(customMetEtaBinEdge_str)-1):
                for PtBinEdge in xrange(len(customMetPtBinEdge_str)):
                  if( ( ('gamma' is not customMetFlavor_str[particleId]) and ('h0' not in customMetFlavor_str[particleId]) ) and (PtBinEdge > 4) ): continue
                  if( ('h0' not in customMetFlavor_str[particleId]) and (PtBinEdge > 5) ): continue

                  # print 'my%sPFmetPt0p0EtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])
                  fillMET(tr, 'my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1]),
                              self.handles['my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])].product()[0].p4()
                          )
                  fill(tr, 'my%sPFmetPt%sEtaMin%sEtaMax%s_sumEt'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1]), 
                              self.handles['my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])].product()[0].sumEt())


                            
                # if('gamma' in customMetFlavor_str[particleId]):
                  # if('0p5' in customMetPtBinEdge_str[PtBinEdge]):
                    # print 'my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1]),\
                    # self.handles['my%sPFmetPt%sEtaMin%sEtaMax%s'%(customMetFlavor_str[particleId],customMetPtBinEdge_str[PtBinEdge],customMetEtaBinEdge_str[EtaBinEdge],customMetEtaBinEdge_str[EtaBinEdge+1])].product()[0].sumEt()
                
          # print 'raw',event.pfmetraw.pt(), event.pfmetraw.phi(), event.pfmetraw.sumEt(), \
                # 'pfMetForRegression',event.pfMetForRegression.pt(),event.pfMetForRegression.phi(),event.pfMetForRegression.sumEt()

          # event.customMetFlavor =        [[211], [130],  [22] ,[1,2],[11] ,[13]]
          # event.customMetPtMin =     [0.0,  0.5,  1.0,  1.5,  2.0]
          # event.customMetEtaMax =     [2.1,  2.5,  3.0,  5.0,  10.0]
          
          # for iFlavor in xrange(len(event.customMetFlavor)):
            # for iPtMin in xrange(len(event.customMetPtMin)):
              # for jEtaMax in xrange(len(event.customMetEtaMax)):
                # fillCustomMET(tr, \
                              # 'met'+event.customMetFlavor_str[iFlavor]+'Pt'+event.customMetPtMin_str[iPtMin]+\
                              # 'EtaMin'+event.customMetEtaMin_str[jEtaMax]+'EtaMax'+event.customMetEtaMax_str[jEtaMax], \
                              # event.customMet[iFlavor][iPtMin][jEtaMax] )
          
          
          
          # customMetFlavor_str = [ 'tkmethemu' , 'h0','gamma','egammaHF' ]
          # customMetPtMin_str = ['m5p0','m3p0','m2p4','m1p4','1p4','2p4','3p0','5p0']
          # customMetEtaMin_str = ['0p0','1p4','2p1','2p5','3p0']
          # customMetEtaMax_str = ['1p4','2p1','2p5','3p0','5p0']
          
          # mygammaPFmetPt0p0EtaMin1p4EtaMax2p1
                
            
          if len(event.ZselJets)>0:
              fillJet(tr, 'Jet_leading', event.ZselJets[0])
        #print 'filling'  
        self.tree.tree.Fill()
       
