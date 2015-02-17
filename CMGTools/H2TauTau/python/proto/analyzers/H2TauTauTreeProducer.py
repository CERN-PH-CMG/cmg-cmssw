from PhysicsTools.Heppy.analyzers.core.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.tauIDs            import tauIDs
from PhysicsTools.HeppyCore.utils.deltar                 import deltaR, deltaPhi
from PhysicsTools.Heppy.analyzers.core.AutoHandle        import AutoHandle


class H2TauTauTreeProducer( TreeAnalyzerNumpy ):
    '''
       Base H->tautau tree producer.
       Books and fills the branches that are common to
       all four channels (mt, et, tt, em).
       The following branches are booked and filled:
       _ event ID
       _ di-tau pair variables (including MET)
       _ raw pf MET
       _ isSignal (both legs pass tight ID and isolation)
       _ third lepton veto
       _ lepton accept
       _ VBF variables
       _ generator information variables (including NUP)
       _ jet1 and jet2 variables (sorted by pt)
       _ bjet1 and bjet2 variables (sorted by pt)
       _ event weight
       _ vertex weight
       _ embed weight
       _ gen parent boson H, W, Z (if exists)
       _ NJetWeight
       _ event rho
       _ HqT weights
       _ hard scattering quarks and gluons (up to 4, can be set by self.maxNGenJets)
       Signal lepton-specific variables need to be booked
       and filled in the channel-specific child producers.
    '''

    def var( self, tree, varName, type=float ):
        tree.var(varName, type)
    
    def fill( self, tree, varName, value ):
        tree.fill( varName, value )

    def declareHandles(self):
        super(H2TauTauTreeProducer, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
                'slimmedMETs',
                'std::vector<pat::MET>' 
                )
    
    def declareVariables( self, setup ):
        
        self.bookEvtID      (self.tree)
        self.bookDiLepton   (self.tree)          
        self.bookGenInfo    (self.tree)
        self.bookVBF        (self.tree, 'ditau')

        self.var(self.tree, 'isSignal'       )
        self.var(self.tree, 'leptonAccept'   )
        self.var(self.tree, 'thirdLeptonVeto')

        self.var            (self.tree, 'nJets'  , int) # pt>30 GeV
        self.var            (self.tree, 'nJets20', int) # pt>20 GeV
        self.bookJet        (self.tree, 'jet1'        )
        self.bookJet        (self.tree, 'jet2'        )
   
        self.var            (self.tree, 'nCSVLJets', int)
        self.var            (self.tree, 'nbJets'   , int)
        self.bookJet        (self.tree, 'bjet1'         )
        self.bookJet        (self.tree, 'bjet2'         )
           
        self.var            (self.tree, 'nVert'       , int)
        self.var            (self.tree, 'rho'              )
        self.var            (self.tree, 'weight'           )
        self.var            (self.tree, 'vertexWeight'     )
        self.var            (self.tree, 'embedWeight'      )
        self.var            (self.tree, 'NJetWeight'       )
   
        self.var            (self.tree, 'hqtWeight'    )
        self.var            (self.tree, 'hqtWeightUp'  )
        self.var            (self.tree, 'hqtWeightDown')

        self.bookParticle   (self.tree, 'pfmet')

        self.bookGenParticle(self.tree, 'genBoson')
        
        self.maxNGenJets = 4

    def process( self, event ):
    
        # needed when doing handle.product(), goes back to PhysicsTools.Heppy.analyzers.core.Analyzer
        self.readCollections( event.input ) 

        self.tree.reset()
     
        self.fillEvtID   (self.tree, event         )
        self.fillDiLepton(self.tree, event.diLepton)          
        self.fillGenInfo (self.tree, event         )
        if hasattr(event, 'vbf') :
            self.fillVBF     (self.tree, 'ditau', event.vbf)

        self.fill(self.tree, 'isSignal'       , event.isSignal       )
        self.fill(self.tree, 'leptonAccept'   , event.leptonAccept   )
        self.fill(self.tree, 'thirdLeptonVeto', event.thirdLeptonVeto)
        
        nJets   = len(event.cleanJets30)
        nJets20 = len(event.cleanJets  )
        self.fill(self.tree, 'nJets'  , nJets  )
        self.fill(self.tree, 'nJets20', nJets20)
        for i, jet in enumerate(event.cleanJets[:2]) : 
            self.fillJet(self.tree, 'jet{NUM}'.format(NUM=str(i+1)), jet)
        
        nbJets = len(event.cleanBJets)
        self.fill(self.tree, 'nbJets', nbJets)
        for i, jet in enumerate(event.cleanBJets[:2]) : 
            self.fillJet(self.tree, 'bjet{NUM}'.format(NUM=str(i+1)), jet)
        # JAN - directly count CSVL jets as done in
        # other groups. May apply SFs as for CSVM jets
        # after rewriting BTagSF module in the future
        nCSVLJets = 0
        for jet in event.cleanJets:
            if jet.btag('combinedSecondaryVertexBJetTags') > 0.244:
                    nCSVLJets += 1
        self.fill(self.tree, 'nCSVLJets', nCSVLJets)
        
        self.fill(self.tree, 'nVert'       , len(event.vertices)) 
        self.fill(self.tree, 'rho'         , event.rho          ) 
        self.fill(self.tree, 'weight'      , event.eventWeight  )
        self.fill(self.tree, 'vertexWeight', event.vertexWeight )
        self.fill(self.tree, 'embedWeight' , event.embedWeight  )
        self.fill(self.tree, 'NJetWeight'  , event.NJetWeight   )

        self.fill(self.tree, 'hqtWeight'    , event.higgsPtWeight    ) 
        self.fill(self.tree, 'hqtWeightUp'  , event.higgsPtWeightUp  ) 
        self.fill(self.tree, 'hqtWeightDown', event.higgsPtWeightDown) 

        if hasattr(event, 'parentBoson') :
            self.fillGenParticle(self.tree, 'genBoson', event.parentBoson)

        pfmet = self.handles['pfmetraw'].product()[0]
        self.fillParticle(self.tree, 'pfmet', pfmet)
        
        self.tree.tree.Fill()


    # event ID
    def bookEvtID( self, tree ):
        self.var(tree, 'run'  ,int)
        self.var(tree, 'lumi' ,int)
        self.var(tree, 'event',int)

    def fillEvtID( self, tree, event ):
        self.fill(tree, 'run'  , event.run    )
        self.fill(tree, 'lumi' , event.lumi   )
        self.fill(tree, 'event', event.eventId)

    
    # simple particle
    def bookParticle( self, tree, pName ):
        self.var(tree, '{pName}_pt'    .format(pName=pName))
        self.var(tree, '{pName}_eta'   .format(pName=pName))
        self.var(tree, '{pName}_phi'   .format(pName=pName))
        self.var(tree, '{pName}_charge'.format(pName=pName))
        self.var(tree, '{pName}_mass'  .format(pName=pName))
    
    def fillParticle( self, tree, pName, particle ):
        self.fill(tree, '{pName}_pt'    .format(pName=pName), particle.pt()     )
        self.fill(tree, '{pName}_eta'   .format(pName=pName), particle.eta()    )
        self.fill(tree, '{pName}_phi'   .format(pName=pName), particle.phi()    )
        self.fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )
        self.fill(tree, '{pName}_mass'  .format(pName=pName), particle.mass()   )
    
    
    # simple gen particle
    def bookGenParticle( self, tree, pName ):
        self.bookParticle(tree, pName)
        self.var(tree, '{pName}_mass' .format(pName=pName))
        self.var(tree, '{pName}_pdgId'.format(pName=pName))
        
    def fillGenParticle( self, tree, pName, particle ):
        self.fillParticle(tree, pName, particle)
        self.fill(tree, '{pName}_mass' .format(pName=pName), particle.mass() )
        self.fill(tree, '{pName}_pdgId'.format(pName=pName), particle.pdgId())
    
    
    # di-tau 
    def bookDiLepton( self, tree ):
        self.var( tree, 'visMass'      )
        self.var( tree, 'svfitMass'    )
        self.var( tree, 'pZetaMET'     )
        self.var( tree, 'pZetaVis'     )
        self.var( tree, 'pZetaDisc'    )
        self.var( tree, 'mtleg2'       )
        self.var( tree, 'mtleg1'       )
        self.var( tree, 'mt'           )
        self.var( tree, 'pthiggs'      )
        self.var( tree, 'deltaPhiL1L2' )
        self.var( tree, 'deltaEtaL1L2' )
        self.var( tree, 'deltaRL1L2'   )
        self.var( tree, 'deltaPhiL1MET')
        self.var( tree, 'deltaPhiL2MET')
        self.var( tree, 'metcov00'     )
        self.var( tree, 'metcov01'     )
        self.var( tree, 'metcov10'     )
        self.var( tree, 'metcov11'     )
        self.var( tree, 'metphi'       )
        self.var( tree, 'mex'          )
        self.var( tree, 'mey'          )
        self.var( tree, 'met'          )
    
    def fillDiLepton( self, tree, diLepton):
        self.fill(tree, 'visMass'  , diLepton.mass()     )
        self.fill(tree, 'svfitMass', diLepton.svfitMass())
        self.fill(tree, 'pZetaMET' , diLepton.pZetaMET() )
        self.fill(tree, 'pZetaVis' , diLepton.pZetaVis() )
        self.fill(tree, 'pZetaDisc', diLepton.pZetaDisc())
        self.fill(tree, 'mt'       , diLepton.mTLeg2()   )
        self.fill(tree, 'mtleg2'   , diLepton.mTLeg2()   ) # RIC: handy for tt, redundant but non destructive
        self.fill(tree, 'mtleg1'   , diLepton.mTLeg1()   )
        # RIC: can't find cov matrix... will look it up later
        #self.fill(tree, 'metcov00', diLepton.metSig().significance()(0,0))
        #self.fill(tree, 'metcov01', diLepton.metSig().significance()(0,1))
        #self.fill(tree, 'metcov10', diLepton.metSig().significance()(1,0))
        #self.fill(tree, 'metcov11', diLepton.metSig().significance()(1,1))
        self.fill(tree, 'metphi'  , diLepton.met().phi())
        self.fill(tree, 'mex'     , diLepton.met().px() )
        self.fill(tree, 'mey'     , diLepton.met().py() )
        self.fill(tree, 'met'     , diLepton.met().pt() )
    
        pthiggs = (diLepton.leg1().p4()+diLepton.leg2().p4()+diLepton.met().p4()).pt()
        self.fill(tree, 'pthiggs', pthiggs)
        
        l1eta  = diLepton.leg1().eta()
        l2eta  = diLepton.leg2().eta()
        l1phi  = diLepton.leg1().phi()
        l2phi  = diLepton.leg2().phi()
        metphi = diLepton.met().phi()
        self.fill(tree, 'deltaPhiL1L2' , deltaPhi(l1phi, l2phi)            )
        self.fill(tree, 'deltaEtaL1L2' , abs(l1eta-l2eta)                  )
        self.fill(tree, 'deltaRL1L2'   , deltaR(l1eta, l1phi, l2eta, l2phi))
        self.fill(tree, 'deltaPhiL1MET', deltaPhi(l1phi, metphi)           )
        self.fill(tree, 'deltaPhiL2MET', deltaPhi(l2phi, metphi)           )
        
    
    # lepton
    def bookLepton( self, tree, pName ):
        self.bookParticle(tree, pName        )
        self.bookParticle(tree, pName+'_jet' )
        self.var(tree, '{pName}_relIso05'      .format(pName=pName))
        self.var(tree, '{pName}_dxy'           .format(pName=pName))
        self.var(tree, '{pName}_dz'            .format(pName=pName))
        self.var(tree, '{pName}_weight'        .format(pName=pName))
        self.var(tree, '{pName}_triggerWeight' .format(pName=pName))
        self.var(tree, '{pName}_triggerEffData'.format(pName=pName))
        self.var(tree, '{pName}_triggerEffMC'  .format(pName=pName))
        self.var(tree, '{pName}_recEffWeight'  .format(pName=pName))
    
    def fillLepton( self, tree, pName, lepton ):
        self.fillParticle(tree, pName       , lepton     )
        self.fillParticle(tree, pName+'_jet', lepton.jet )
        self.fill(tree, '{pName}_relIso05'      .format(pName=pName), lepton.relIsoAllChargedDB05() )
        self.fill(tree, '{pName}_dxy'           .format(pName=pName), lepton.dxy()                  )
        self.fill(tree, '{pName}_dz'            .format(pName=pName), lepton.dz()                   )
        self.fill(tree, '{pName}_weight'        .format(pName=pName), lepton.weight                 )
        self.fill(tree, '{pName}_triggerWeight' .format(pName=pName), lepton.triggerWeight          )
        self.fill(tree, '{pName}_triggerEffData'.format(pName=pName), lepton.triggerEffData         )
        self.fill(tree, '{pName}_triggerEffMC'  .format(pName=pName), lepton.triggerEffMC           )
        self.fill(tree, '{pName}_recEffWeight'  .format(pName=pName), lepton.recEffWeight           )
    
    
    # muon
    def bookMuon( self, tree, pName ):
        self.bookLepton(tree, pName )
        # JAN FIXME - do we need the MVA iso and does it exist?
        # var(tree, '{pName}_mvaIso'.format(pName=pName))
        self.var(tree, '{pName}_looseId'.format(pName=pName))
        self.var(tree, '{pName}_tightId'.format(pName=pName))
    
    def fillMuon( self, tree, pName, muon ):
        self.fillLepton(tree, pName, muon)
        # JAN FIXME - do we need the MVA iso and does it exist?
        # fill(tree, '{pName}_mvaIso'.format(pName=pName), muon.mvaIso() )
        self.fill(tree, '{pName}_looseId'.format(pName=pName), muon.looseId() )
        self.fill(tree, '{pName}_tightId'.format(pName=pName), muon.tightId() )
    
    
    # electron
    def bookEle( self, tree, pName ):
        self.bookLepton(tree, pName )
        #self.var(tree, '{pName}_mvaIso'             .format(pName=pName))
        self.var(tree, '{pName}_mvaTrigV0'          .format(pName=pName))
        self.var(tree, '{pName}_mvaNonTrigV0'       .format(pName=pName))
        self.var(tree, '{pName}_looseId'            .format(pName=pName))
        self.var(tree, '{pName}_tightId'            .format(pName=pName))
        self.var(tree, '{pName}_numberOfMissingHits'.format(pName=pName))
        self.var(tree, '{pName}_passConversionVeto' .format(pName=pName))
        
    def fillEle( self, tree, pName, ele ):
        self.fillLepton(tree, pName, ele)
        # JAN FIXME - do we need the MVA iso and does it exist?
        # RIC: 16/2/15 this is all messed up, commented out until electron obj is fixed
        # self.fill(tree, '{pName}_mvaIso'             .format(pName=pName), ele.mvaIso()                   )
        # self.fill(tree, '{pName}_mvaTrigV0'          .format(pName=pName), ele.mvaTrigV0()                )
        # self.fill(tree, '{pName}_mvaNonTrigV0'       .format(pName=pName), ele.mvaNonTrigV0()             )
        # self.fill(tree, '{pName}_looseId'            .format(pName=pName), ele.looseIdForEleTau()         )
        # self.fill(tree, '{pName}_tightId'            .format(pName=pName), ele.tightIdForEleTau()         )
        # self.fill(tree, '{pName}_numberOfMissingHits'.format(pName=pName), ele.lostInner()                )
        # self.fill(tree, '{pName}_passConversionVeto' .format(pName=pName), ele.passConversionVeto()       )
    
    
    # tau   
    def bookTau( self, tree, pName ):
        self.bookLepton(tree, pName )
        for tauID in tauIDs:
            self.var(tree, '{pName}_{tauID}'.format(pName=pName, tauID=tauID))
        self.var(tree, '{pName}_EOverp'   .format(pName=pName))
        self.var(tree, '{pName}_decayMode'.format(pName=pName))
        self.var(tree, '{pName}_zImpact'  .format(pName=pName))
    
    def fillTau( self, tree, pName, tau ):
        self.fillLepton(tree, pName, tau)
        for tauID in tauIDs:
            self.fill(tree, '{pName}_{tauID}'.format(pName=pName, tauID=tauID), tau.tauID(tauID))
        #self.fill(tree, '{pName}_EOverp'   .format(pName=pName), tau.calcEOverP())
        self.fill(tree, '{pName}_decayMode'.format(pName=pName), tau.decayMode()  )
        self.fill(tree, '{pName}_zImpact'  .format(pName=pName), tau.zImpact()    )
    
    
    # jet
    def bookJet( self, tree, pName ):
        self.bookParticle(tree, pName)
        self.var(tree, '{pName}_puMva'        .format(pName=pName))
        self.var(tree, '{pName}_looseJetId'   .format(pName=pName))
        self.var(tree, '{pName}_btagMVA'      .format(pName=pName))
        self.var(tree, '{pName}_area'         .format(pName=pName))
        self.var(tree, '{pName}_genJetPt'     .format(pName=pName))
        self.var(tree, '{pName}_partonFlavour'.format(pName=pName))
        self.var(tree, '{pName}_csv'          .format(pName=pName))
        self.var(tree, '{pName}_Bmatch'       .format(pName=pName))
    
    def fillJet( self, tree, pName, jet ):
        self.fillParticle(tree, pName, jet )
        # JAN - only one PU mva working point, but we may want to add more
        # run in our skimming step
        # (for which Jet.py would have to be touched again)
        self.fill(tree, '{pName}_puMva'        .format(pName=pName), jet.puMva('pileupJetIdFull:full53xDiscriminant') )
        self.fill(tree, '{pName}_looseJetId'   .format(pName=pName), jet.looseJetId()                                 )
        self.fill(tree, '{pName}_btagMVA'      .format(pName=pName), jet.btagMVA                                      )
        self.fill(tree, '{pName}_area'         .format(pName=pName), jet.jetArea()                                    )
        self.fill(tree, '{pName}_partonFlavour'.format(pName=pName), jet.partonFlavour()                              )
        self.fill(tree, '{pName}_csv'          .format(pName=pName), jet.btag('combinedSecondaryVertexBJetTags')      )
        self.fill(tree, '{pName}_Bmatch'       .format(pName=pName), jet.matchGenParton                               )
        if hasattr(jet, 'matchedGenJet') and jet.matchedGenJet:
            self.fill(tree, '{pName}_genJetPt'.format(pName=pName), jet.matchedGenJet.pt())
    
    
    # vbf  
    def bookVBF( self, tree, pName ):
        self.var(tree, '{pName}_mjj'      .format(pName=pName))
        self.var(tree, '{pName}_deta'     .format(pName=pName))
        self.var(tree, '{pName}_nCentral' .format(pName=pName))
        self.var(tree, '{pName}_mva'      .format(pName=pName))
        self.var(tree, '{pName}_jdphi'    .format(pName=pName))
        self.var(tree, '{pName}_dijetpt'  .format(pName=pName))
        self.var(tree, '{pName}_dijetphi' .format(pName=pName))
        self.var(tree, '{pName}_hdijetphi'.format(pName=pName))
        self.var(tree, '{pName}_visjeteta'.format(pName=pName))
        self.var(tree, '{pName}_ptvis'    .format(pName=pName))
        
    def fillVBF( self, tree, pName, vbf ):
        self.fill(tree, '{pName}_mjj'      .format(pName=pName), vbf.mjj              )
        self.fill(tree, '{pName}_deta'     .format(pName=pName), vbf.deta             )
        self.fill(tree, '{pName}_nCentral' .format(pName=pName), len(vbf.centralJets) )
        self.fill(tree, '{pName}_mva'      .format(pName=pName), vbf.mva              )
        self.fill(tree, '{pName}_jdphi'    .format(pName=pName), vbf.dphi             )
        self.fill(tree, '{pName}_dijetpt'  .format(pName=pName), vbf.dijetpt          )
        self.fill(tree, '{pName}_dijetphi' .format(pName=pName), vbf.dijetphi         )
        self.fill(tree, '{pName}_hdijetphi'.format(pName=pName), vbf.dphidijethiggs   )
        self.fill(tree, '{pName}_visjeteta'.format(pName=pName), vbf.visjeteta        )
        self.fill(tree, '{pName}_ptvis'    .format(pName=pName), vbf.ptvis            )
    
    
    # generator information 
    def bookGenInfo( self, tree ):
        self.var(tree, 'genMatched', int)
        self.var(tree, 'NUP'      , int)
        self.var(tree, 'isZtt'    , int)
        self.var(tree, 'isZmt'    , int)
        self.var(tree, 'isZet'    , int)
        self.var(tree, 'isZee'    , int)
        self.var(tree, 'isZmm'    , int)
        self.var(tree, 'isZem'    , int)
        self.var(tree, 'isZEE'    , int)
        self.var(tree, 'isZMM'    , int)
        self.var(tree, 'isZLL'    , int)
        self.var(tree, 'isFake'   , int)
        self.var(tree, 'hasW'     , int)
        self.var(tree, 'hasZ'     , int)
        self.var(tree, 'genMass'       )
        self.var(tree, 'genMet'        )
        self.var(tree, 'genMex'        )
        self.var(tree, 'genMey'        )
        self.var(tree, 'genMetPhi'     )
    
    def fillGenInfo( self, tree, event ):
        #self.fill(tree, 'NUP'      , event.NUP       )
        self.fill(tree, 'isZtt'    , event.isZtt     )
        self.fill(tree, 'isZmt'    , event.isZmt     )
        self.fill(tree, 'isZet'    , event.isZet     )
        self.fill(tree, 'isZee'    , event.isZee     )
        self.fill(tree, 'isZmm'    , event.isZmm     )
        self.fill(tree, 'isZem'    , event.isZem     )
        self.fill(tree, 'isZEE'    , event.isZEE     )
        self.fill(tree, 'isZMM'    , event.isZMM     )
        self.fill(tree, 'isZLL'    , event.isZLL     )
        self.fill(tree, 'isFake'   , event.isFake    )
        self.fill(tree, 'hasW'     , event.hasW      )
        self.fill(tree, 'hasZ'     , event.hasZ      )
        self.fill(tree, 'genMass'  , event.genMass   )
        self.fill(tree, 'genMet'   , event.genMet    )
        self.fill(tree, 'genMex'   , event.genMex    )
        self.fill(tree, 'genMey'   , event.genMey    )
        self.fill(tree, 'genMetPhi', event.genMetPhi )

    # quark and gluons
    def bookQG( self, tree ):    
        for i in range(0, self.maxNGenJets):
            self.bookGenParticle(self.tree, 'genQG_{i}'.format(i=i))

    def fillQG( self, tree, event):
        # Fill hard quarks/gluons
        quarksGluons = [p for p in event.genParticles if abs(p.pdgId()) in (1, 2, 3, 4, 5, 21)                  and \
                                                         p.status() == 3                                        and \
                                                         (p.numberOfDaughters() == 0 or p.daughter(0).status() != 3)]
        quarksGluons.sort(key=lambda x: -x.pt())
        for i in range(0, min(self.maxNGenJets, len(quarksGluons))):
            self.fillGenParticle(tr, 'genQG_{i}'.format(i=i), quarksGluons[i])
