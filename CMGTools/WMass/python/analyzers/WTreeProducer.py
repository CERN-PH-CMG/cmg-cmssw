from CMGTools.WMass.analyzers.CoreTreeProducer import *

# def fillMuonCovMatrix( tree, pName, covMatrix,event ):
    # for i in range(0,9):
        # # tree.vars['{pName}CovMatrix'.format(pName=pName)][i] = covMatrix[i]
        # # if self.scalar:
            # # for i,w in enumerate(event.pdfWeights[pdf]):
                # # tr.fill('pdfWeight_%s_%d' % (pdf,i), w)
                # tree.fill('{pName}CovMatrix'.format(pName=pName), covMatrix[i])
        # # else:
            # # tr.vfill('pdfWeight_%s' % pdf, event.pdfWeights[pdf])

class WTreeProducer( CoreTreeProducer ):
    
    MuonClass = Muon 

    
    def declareHandles(self):
      super(WTreeProducer, self).declareHandles()
      self.declareCoreHandles()      

    def declareVariables(self):
      tr = self.tree

      self.declareCoreVariables(self.cfg_comp.isMC)

      var( tr, 'evtHasTrg', int)

      if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
        var( tr, 'njets', int)
        var( tr, 'evtWSel', int)
      
        var( tr, 'nMuons', int)
        var( tr, 'nTrgMuons', int)
        var( tr, 'noTrgMuonsLeadingPt', int)

      ###--------------------------- BOOK W and muon infos ------------------------------
      
      bookW( tr, 'W')
      var( tr, 'W_mt')
      if (self.cfg_comp.isMC):
        bookZ( tr, 'WGen')
        var( tr, 'WGen_mt')
        bookFourVector( tr, 'NuGen')        
          
      # var( tr, 'u1')
      # var( tr, 'u2')

      bookMuon(tr, 'Mu')
      var(tr, 'Mu_dxy')
      var(tr, 'Mu_dz')
      var(tr, 'MuIsTightAndIso', int)
      var(tr, 'MuIsTight', int)
      if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
        var(tr, 'pt_vis')
        var(tr, 'phi_vis')
      if (self.cfg_comp.isMC):
        bookParticle(tr, 'MuGen')
        bookParticle(tr, 'MuGenStatus1')
        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          var(tr, 'MuDRGenP')
        # bookParticle(tr, 'NuGen')
        var(tr, 'FSRWeight')
      if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
        bookMuonCovMatrix(tr,'Mu' )
        bookJet(tr, 'Jet_leading')
      
      if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
        if (self.cfg_comp.isMC):
          var(tr, 'genWLept')
       
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        tr = self.tree
        tr.reset()

        self.fillCoreVariables(tr, iEvent, event, self.cfg_comp.isMC)

        # this contain at least 1VTX, 1 muons, trigger 
        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          fill( tr, 'evtWSel', event.WGoodEvent)

          # fill( tr, 'evtHasTrg', event.passedTriggerAnalyzer)
          fill( tr, 'evtHasTrg', True)
        
        ###--------------------------- FILL W and muon infos ------------------------------

        if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
          if (self.cfg_comp.isMC):
            fill(tr, 'genWLept', len(event.genWLept))

        if (event.savegenpW and self.cfg_comp.isMC):
          
          fill(tr, 'FSRWeight',event.fsrWeight)
          fillZ(tr,'WGen', event.genW[0].p4())
          fill(tr, 'WGen_mt', event.genW_mt)
          fillParticle(tr, 'MuGen',event.genMu[0])
          fillParticle(tr, 'MuGenStatus1', event.genMuStatus1[0])      
          fillFourVector(tr, 'NuGen', event.genNu_p4)
          if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
            fill(tr, 'MuDRGenP',event.muGenDeltaRgenP)
          

        if event.WGoodEvent == True :
            
          fillW( tr, 'W',event.W4V)
          fill(tr, 'W_mt', event.W4V_mt)
          # fill(tr, 'u1', event.u1)
          # fill(tr, 'u2', event.u2)

          fillMuon(tr, 'Mu', event.selMuons[0])
          if ( event.selMuons[0].isGlobalMuon() or event.selMuons[0].isTrackerMuon() ) and event.passedVertexAnalyzer:
            fill(tr, 'Mu_dxy', math.fabs(event.selMuons[0].dxy()))
            fill(tr, 'Mu_dz', math.fabs(event.selMuons[0].dz()))
          fill(tr, 'MuIsTightAndIso', event.selMuonIsTightAndIso)
          fill(tr, 'MuIsTight', event.selMuonIsTight)
          if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
            fill(tr, 'pt_vis', event.selMuons[0].pt())
            fill(tr, 'phi_vis', event.selMuons[0].phi())
          
            fillMuonCovMatrix( tr,'Mu' ,event.covMatrixMuon ,event)

        ###--------------------------- FILL extra infos  ------------------------------

        if (event.savegenpW and self.cfg_comp.isMC) or event.WGoodEvent:

          if not (hasattr(self.cfg_ana,'superslimNtuples') and self.cfg_ana.superslimNtuples):
            fill( tr, 'nMuons', len(event.muons))
            fill( tr, 'nTrgMuons', len(event.selMuons))
            # if len(event.selMuons): print 'len(event.selMuons) ?!?!?'
            if len(event.NoTriggeredMuonsLeadingPt) > 0 :
              fill( tr, 'noTrgMuonsLeadingPt', event.NoTriggeredMuonsLeadingPt[0].pt())

            fill( tr, 'njets', len(event.selJets))
            if len(event.selJets)>0:
              fillJet(tr, 'Jet_leading', event.selJets[0])

            
          self.tree.tree.Fill()
       
