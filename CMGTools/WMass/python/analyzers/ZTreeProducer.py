
from CMGTools.WMass.analyzers.CoreTreeProducer import *
#from ZAnalyzer import testLegID

# def bookJetCollections( tree, pName ):
    # var(tree, '{pName}_number'.format(pName=pName),int)
    # tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[10]/D' )

# def bookLeptonCollections( tree, pName ):
    # var(tree, '{pName}_number'.format(pName=pName),int)
   
    # tree.vars['{pName}_charge'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_charge'.format(pName=pName),tree.vars['{pName}_charge'.format(pName=pName)] ,'{pName}_charge'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_ID'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_ID'.format(pName=pName),tree.vars['{pName}_ID'.format(pName=pName)] ,'{pName}_ID'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_ID_8TeV'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_ID_8TeV'.format(pName=pName),tree.vars['{pName}_ID_8TeV'.format(pName=pName)] ,'{pName}_ID_8TeV'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_Iso'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_Iso'.format(pName=pName),tree.vars['{pName}_Iso'.format(pName=pName)] ,'{pName}_Iso'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_IsPromt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_IsPromt'.format(pName=pName),tree.vars['{pName}_IsPromt'.format(pName=pName)] ,'{pName}_IsPromt'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_IsTrig'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_IsTrig'.format(pName=pName),tree.vars['{pName}_IsTrig'.format(pName=pName)] ,'{pName}_IsTrig'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[10]/D' )
    # #  tree.vars['{pName}_Zmatch'.format(pName=pName)]= my_n.zeros(10, dtype=int)
    # #  tree.tree.Branch('{pName}_Zmatch'.format(pName=pName),tree.vars['{pName}_Zmatch'.format(pName=pName)] ,'{pName}_Zmatch'.format(pName=pName)+'[10]/I' )


# def bookElectrons( tree, pName ):
    # var(tree, '{pName}_number'.format(pName=pName),int)
    # tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_TightID'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_TightID'.format(pName=pName),tree.vars['{pName}_TightID'.format(pName=pName)] ,'{pName}_TightID'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_TightIso'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_TightIso'.format(pName=pName),tree.vars['{pName}_TightIso'.format(pName=pName)] ,'{pName}_TightIso'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_charge'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_charge'.format(pName=pName),tree.vars['{pName}_charge'.format(pName=pName)] ,'{pName}_charge'.format(pName=pName)+'[10]/D' )
    # tree.vars['{pName}_IsPromt'.format(pName=pName)]= my_n.zeros(10, dtype=float)
    # tree.tree.Branch('{pName}_IsPromt'.format(pName=pName),tree.vars['{pName}_IsPromt'.format(pName=pName)] ,'{pName}_IsPromt'.format(pName=pName)+'[10]/D' )

# def bookcmgPFcands( tree, pName ):
    # var(tree, '{pName}_number'.format(pName=pName),int)
    # tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    # tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[5000]/D' )
    # tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    # tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[5000]/D' )
    # tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    # tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[5000]/D' )
    # tree.vars['{pName}_pdgId'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    # tree.tree.Branch('{pName}_pdgId'.format(pName=pName),tree.vars['{pName}_pdgId'.format(pName=pName)] ,'{pName}_pdgId'.format(pName=pName)+'[5000]/D' )
    # tree.vars['{pName}_fromPV'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    # tree.tree.Branch('{pName}_fromPV'.format(pName=pName),tree.vars['{pName}_fromPV'.format(pName=pName)] ,'{pName}_fromPV'.format(pName=pName)+'[5000]/D' )
    # tree.vars['{pName}_dZfromPV'.format(pName=pName)]= my_n.zeros(5000, dtype=float)
    # tree.tree.Branch('{pName}_dZfromPV'.format(pName=pName),tree.vars['{pName}_dZfromPV'.format(pName=pName)] ,'{pName}_dZfromPV'.format(pName=pName)+'[5000]/D' )

# def bookPFJets( tree, pName ):
    # var(tree, '{pName}_number'.format(pName=pName),int)
    # tree.vars['{pName}_pt'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # tree.tree.Branch('{pName}_pt'.format(pName=pName),tree.vars['{pName}_pt'.format(pName=pName)] ,'{pName}_pt'.format(pName=pName)+'[100]/D' )
    # tree.vars['{pName}_eta'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # tree.tree.Branch('{pName}_eta'.format(pName=pName),tree.vars['{pName}_eta'.format(pName=pName)] ,'{pName}_eta'.format(pName=pName)+'[100]/D' )
    # tree.vars['{pName}_phi'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # tree.tree.Branch('{pName}_phi'.format(pName=pName),tree.vars['{pName}_phi'.format(pName=pName)] ,'{pName}_phi'.format(pName=pName)+'[100]/D' )
    # tree.vars['{pName}_pdgId'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # # tree.tree.Branch('{pName}_pdgId'.format(pName=pName),tree.vars['{pName}_pdgId'.format(pName=pName)] ,'{pName}_pdgId'.format(pName=pName)+'[100]/D' )
    # # tree.vars['{pName}_fromPV'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # # tree.tree.Branch('{pName}_fromPV'.format(pName=pName),tree.vars['{pName}_fromPV'.format(pName=pName)] ,'{pName}_fromPV'.format(pName=pName)+'[100]/D' )
    # # tree.vars['{pName}_dZfromPV'.format(pName=pName)]= my_n.zeros(100, dtype=float)
    # # tree.tree.Branch('{pName}_dZfromPV'.format(pName=pName),tree.vars['{pName}_dZfromPV'.format(pName=pName)] ,'{pName}_dZfromPV'.format(pName=pName)+'[100]/D' )

# def fillJets( tree, pName, particles ):
    # fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    # for i in range(0, min(len(particles),10)):
        # tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        # tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        # tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()

# def fillMuons( tree, pName, particles , event):
    # fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    # for i in range(0, min(len(particles),10)):
        
        # tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        # tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        # tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        # tree.vars['{pName}_charge'.format(pName=pName)][i] = float(particles[i].charge())
        # tree.vars['{pName}_ID'.format(pName=pName)][i] = event.ZallMuonsID[i]
        # tree.vars['{pName}_ID_8TeV'.format(pName=pName)][i] = event.ZallMuonsID_8TeV[i]
        # tree.vars['{pName}_Iso'.format(pName=pName)][i] = particles[i].relIso(0.5)
        # tree.vars['{pName}_IsTrig'.format(pName=pName)][i] = int(event.ZallMuonsTrig[i])

# def fillMuonsGen( tree, pName, particles , event):
    # for i in range(0, min(len(particles),10)):
        # #print i, ' len ', len(particles)
        # tree.vars['{pName}_IsPromt'.format(pName=pName)][i] = event.ZallMuonsMatched[i]

# def fillElectrons( tree, pName, particles,event ):
    # fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    # for i in range(0, min(len(particles),10)):
        # tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        # tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        # tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        # tree.vars['{pName}_TightID'.format(pName=pName)][i] = event.ZElTightID[i]
        # tree.vars['{pName}_TightIso'.format(pName=pName)][i] = event.ZElTightIso[i]
        # tree.vars['{pName}_charge'.format(pName=pName)][i] = particles[i].charge()

# def fillcmgPFcands( tree, pName, particles,vertex,event ):
    # fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    # for i in range(0, min(len(particles),2000)):
        # # print particles[i].pdgId(), math.fabs(particles[i].vertex().z()-vertex.z())
        # tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        # tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        # tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        # tree.vars['{pName}_pdgId'.format(pName=pName)][i] = particles[i].pdgId()
        # tree.vars['{pName}_fromPV'.format(pName=pName)][i] = particles[i].fromPV()
        # tree.vars['{pName}_dZfromPV'.format(pName=pName)][i] = math.fabs(particles[i].vertex().z()-vertex.z())

# def fillPFJets( tree, pName, particles,event ):
    # fill(tree, '{pName}_number'.format(pName=pName),len(particles))
    # for i in range(0, min(len(particles),100)):
        # # print particles[i].pdgId(), math.fabs(particles[i].vertex().z()-vertex.z())
        # tree.vars['{pName}_pt'.format(pName=pName)][i] = particles[i].pt()
        # tree.vars['{pName}_eta'.format(pName=pName)][i] = particles[i].eta()
        # tree.vars['{pName}_phi'.format(pName=pName)][i] = particles[i].phi()
        # # print 'particles[',i,'].mva() ', particles[i].mva()
        # # tree.vars['{pName}_pdgId'.format(pName=pName)][i] = particles[i].pdgId()
        # # tree.vars['{pName}_fromPV'.format(pName=pName)][i] = particles[i].fromPV()
        # # tree.vars['{pName}_dZfromPV'.format(pName=pName)][i] = math.fabs(particles[i].vertex().z()-vertex.z())

# def fillNeutralcmgPFcands( tree, pName, particles,vertex,event ):
    # jneu=0
    # for i in range(0, min(len(particles),2500)):
          # # case 211: return h;
          # # case 11: return e;
          # # case 13: return mu;
          # # case 22: return gamma;
          # # case 130: return h0;
          # # case 1: return h_HF;
          # # case 2: return egamma_HF;
          # # case 0: return X;  
        # if(math.fabs(particles[i].pdgId())==130 or math.fabs(particles[i].pdgId())==1 \
           # or math.fabs(particles[i].pdgId())==2 or math.fabs(particles[i].pdgId())==22):
            # tree.vars['{pName}_pt'.format(pName=pName)][jneu] = particles[i].pt()
            # tree.vars['{pName}_eta'.format(pName=pName)][jneu] = particles[i].eta()
            # tree.vars['{pName}_phi'.format(pName=pName)][jneu] = particles[i].phi()
            # tree.vars['{pName}_pdgId'.format(pName=pName)][jneu] = particles[i].pdgId()
            # tree.vars['{pName}_fromPV'.format(pName=pName)][jneu] = particles[i].fromPV()
            # tree.vars['{pName}_dZfromPV'.format(pName=pName)][i] = math.fabs(particles[i].vertex().z()-vertex.z())
            # jneu = jneu+1
    
    # fill(tree, '{pName}_number'.format(pName=pName),jneu)
        
# def fillElectronsGen(tree, pName, particles,event ):
    # for i in range(0, min(len(particles),10)):
        # tree.vars['{pName}_IsPromt'.format(pName=pName)][i] = event.ZElIsPromt[i] 

class ZTreeProducer( CoreTreeProducer ):
    
    MuonClass = Muon 
        
    def declareHandles(self):
      super(ZTreeProducer, self).declareHandles()
      self.declareCoreHandles()
    
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
      
      # if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
        # self.handles['cmgCandidates'] = AutoHandle('cmgCandidates','std::vector<cmg::Candidate>')
        # # self.handles['kt6PFJets'] = AutoHandle('kt6PFJets','std::vector<reco::PFJet>')
              
    def declareVariables(self):
      tr = self.tree

      self.declareCoreVariables(self.cfg_comp.isMC)
     
      var( tr, 'njets', int)
      var( tr, 'evtHasTrg', int)
      var( tr, 'evtZSel', int)
      
      var( tr, 'nMuons', int)
      var( tr, 'nTrgMuons', int)
      # var( tr, 'nNoTrgMuons', int)
      var( tr, 'noTrgExtraMuonsLeadingPt', int)

     ###--------------------------- BOOK OTHER MET infos ------------------------------
      
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
            
      ###--------------------------- BOOK Z and muon infos ------------------------------
        
      bookZ(tr, 'Z')
      var(tr, 'Z_mt')
      var(tr, 'pt_vis')
      var(tr, 'phi_vis')
      if (self.cfg_comp.isMC):
        if not hasattr(self.cfg_ana,'storeSlimGenInfo'):
          bookZ(tr, 'ZGen')      
        bookZ(tr, 'ZGen_PostFSR')      
        var(tr, 'ZGen_mt')      
      # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # var( tr, 'u1')
        # var( tr, 'u2')
      
      bookMuon(tr, 'MuPos')
      var(tr, 'MuPos_dxy')
      var(tr, 'MuPos_dz')
      var(tr, 'MuPosTrg', int)
      # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # var(tr, 'MuPosIsTightAndIso', int)
      var(tr, 'MuPosIsTight', int)
      # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # var(tr, 'MuPosMatchCMGmuon', int)
      if (self.cfg_comp.isMC):
        bookParticle(tr, 'MuPosGen')
        bookParticle(tr, 'MuPosGenStatus1')
        var(tr, 'FSRWeight')

      # var(tr, 'MuPosGen_pdgId', int)
      bookMuon(tr, 'MuNeg')
      var(tr, 'MuNeg_dxy')
      var(tr, 'MuNeg_dz')
      var(tr, 'MuNegTrg', int)
      # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # var(tr, 'MuNegIsTightAndIso', int)
      var(tr, 'MuNegIsTight', int)
      # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # var(tr, 'MuNegMatchCMGmuon', int)
      if (self.cfg_comp.isMC):
        bookParticle(tr, 'MuNegGen')
        bookParticle(tr, 'MuNegGenStatus1')
        # var(tr, 'MuNegGen_pdgId', int)
        if not hasattr(self.cfg_ana,'storeSlimGenInfo'):
          var(tr, 'MuPosDRGenP')
          var(tr, 'MuNegDRGenP')
      bookMuonCovMatrix(tr,'MuPos' )
      bookMuonCovMatrix(tr,'MuNeg' )

      bookJet(tr, 'Jet_leading')
      # #print 'booking stuff'
      # # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
        # # bookJetCollections(tr,'cmgjets' )
        # # bookLeptonCollections(tr,'cmgmuons' )
        # # bookElectrons(tr,'cmgelectrons' )
      # if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
        # bookcmgPFcands(tr,'cmgCandidates' )
        # # bookPFJets(tr,'kt6PFJets' )
     # # print 'booked stuff'

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
        
        self.fillCoreVariables(tr, iEvent, event, self.cfg_comp.isMC)
        
        # this contain at least 1VTX, 2 muons, trigger
        fill( tr, 'evtZSel', event.ZGoodEvent)
 
        fill( tr, 'evtHasTrg', True)
        fill( tr, 'njets', len(event.ZselJets))
        if len(event.ZselJets)>0:
            fillJet(tr, 'Jet_leading', event.ZselJets[0])
         

        ###--------------------------- FILL Z and muon infos ------------------------------
         
        # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
          # fillElectronsGen( tr,'cmgelectrons' ,event.ZElIsPromt ,event) 
          # fillMuonsGen (tr, 'cmgmuons', event.ZallMuonsMatched, event)       
         
        if (event.savegenpZ and self.cfg_comp.isMC):
             
          fill(tr, 'FSRWeight',event.fsrWeight)
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
                                
          # if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
            # cmgPFcands = self.handles['cmgCandidates'].product()
            # # # kt6PFJets = self.handles['kt6PFJets'].product()
            # # # fillPFJets(tr, 'kt6PFJets', kt6PFJets, event)
          # # if hasattr(self.cfg_ana,'storeCMGcandidates'):
            # # fillcmgPFcands( tr,'cmgCandidates' ,cmgPFcands, event.goodVertices[0], event)
          # # if hasattr(self.cfg_ana,'storeNeutralCMGcandidates'):
            # # fillNeutralcmgPFcands( tr,'cmgCandidates' ,cmgPFcands ,event.goodVertices[0], event)
            
          fillZ(tr, 'Z', event.Z4V)
          fill(tr, 'pt_vis', event.Z4V.Pt())
          fill(tr, 'phi_vis', event.Z4V.Phi())
          fill(tr,  'Z_mt', event.Z4V_mt)
          # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            # fill(tr, 'u1', event.Zu1)
            # fill(tr, 'u2', event.Zu2)
            # fillW(tr, 'WlikePos', event.Wpos4VfromZ)
            # fill(tr,  'WlikePos_mt', event.Wpos4VfromZ_mt)
            # fillMET(tr, 'pfmetWlikePos', event.ZpfmetWpos)
            # fillW(tr, 'WlikeNeg', event.Wneg4VfromZ)
            # fill(tr,  'WlikeNeg_mt', event.Wneg4VfromZ_mt)
            # fillMET(tr, 'pfmetWlikeNeg', event.ZpfmetWneg)
          
          fillMuon(tr, 'MuPos', event.BestZPosMuon)
          if ( event.BestZPosMuon.isGlobalMuon() or event.BestZPosMuon.isTrackerMuon() ) and event.passedVertexAnalyzer:
            fill(tr, 'MuPos_dxy', math.fabs(event.BestZPosMuon.dxy()))
            fill(tr, 'MuPos_dz',math.fabs(event.BestZPosMuon.dz()))
          fill(tr, 'MuPosTrg', event.BestZPosMuonHasTriggered)
          # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            # fill(tr, 'MuPosIsTightAndIso',event.BestZPosMuonIsTightAndIso)
          fill(tr, 'MuPosIsTight',event.BestZPosMuonIsTight)
          # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            # fill(tr, 'MuPosMatchCMGmuon',event.BestZPosMatchIndex)

          fillMuon(tr, 'MuNeg', event.BestZNegMuon)
          if ( event.BestZNegMuon.isGlobalMuon() or event.BestZNegMuon.isTrackerMuon() ) and event.passedVertexAnalyzer:
            fill(tr, 'MuNeg_dxy', math.fabs(event.BestZNegMuon.dxy()))
            fill(tr, 'MuNeg_dz',math.fabs(event.BestZNegMuon.dz()))
          fill(tr, 'MuNegTrg', event.BestZNegMuonHasTriggered)
          # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            # fill(tr, 'MuNegIsTightAndIso',event.BestZNegMuonIsTightAndIso)
          fill(tr, 'MuNegIsTight',event.BestZNegMuonIsTight)
          # if not hasattr(self.cfg_ana,'storeSlimRecoInfo'):
            # fill(tr, 'MuNegMatchCMGmuon',event.BestZNegMatchIndex)
          
          fillMuonCovMatrix( tr,'MuPos' ,event.covMatrixPosMuon ,event)
          fillMuonCovMatrix( tr,'MuNeg' ,event.covMatrixNegMuon ,event)

        ###--------------------------- FILL extra infos ------------------------------
        if (event.savegenpZ and self.cfg_comp.isMC) or event.ZGoodEvent:
          
          fill( tr, 'nMuons', len(event.ZallMuons))
          fill( tr, 'nTrgMuons', len(event.ZselTriggeredMuons))
          # fill( tr, 'nNoTrgMuons', len(event.ZselNoTriggeredMuons))
          if(len(event.ZselNoTriggeredExtraMuonsLeadingPt)>0):
            fill( tr, 'noTrgExtraMuonsLeadingPt', event.ZselNoTriggeredExtraMuonsLeadingPt[0].pt())

          ###--------------------------- FILL OTHER MET ------------------------------

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
                
            
        #print 'filling'  
        self.tree.tree.Fill()
       
