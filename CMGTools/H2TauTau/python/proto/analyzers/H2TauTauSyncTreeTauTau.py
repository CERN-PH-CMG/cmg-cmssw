from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.analyzers.ntuple import *

class H2TauTauSyncTreeTauTau( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

        tr = self.tree
        var( tr, 'NUP' , int)
        
        var( tr, 'run' , int)
        var( tr, 'lumi', int)
        var( tr, 'evt' , int)
        
        var( tr, 'npv', int)
        var( tr, 'npu', int)
        var( tr, 'rho')
        
        var( tr, 'mcweight')
        var( tr, 'puweight')
        var( tr, 'effweight')
        var( tr, 'decaymodeweight')
        var( tr, 'weight')
        
        var( tr, 'm_sv')
        var( tr, 'mvis')
        var( tr, 'm_sv_Up')
        var( tr, 'm_sv_Down')
        
        var( tr, 'gen_pt_1')
        var( tr, 'pt_1')
        var( tr, 'phi_1')
        var( tr, 'eta_1')
        var( tr, 'm_1')
        var( tr, 'q_1')
        var( tr, 'iso_1')
        #var( tr, 'mva_1')
        var( tr, 'antiele_1')
        var( tr, 'd0_1')
        var( tr, 'dZ_1')
        var( tr, 'passid_1', int)
        var( tr, 'passiso_1', int)
        var( tr, 'mt_1')
        var( tr, 'trigweight_1')
        var( tr, 'idweight_1')
        var( tr, 'isoweight_1')
        var( tr, 'byCombinedIsolationDeltaBetaCorrRaw3Hits_1')
        var( tr, 'againstElectronMVA3raw_1')
        var( tr, 'againstElectronMVA3category_1')
        var( tr, 'byIsolationMVA2raw_1')
        var( tr, 'againstMuonLoose_1')
        var( tr, 'againstMuonLoose2_1')
        var( tr, 'againstMuonMedium2_1')
        var( tr, 'againstMuonTight2_1')
        var( tr, 'againstElectronLooseMVA3_1')
        var( tr, 'againstElectronLoose_1')
        var( tr, 'againstElectronNewLooseMVA3_1')

        
        var( tr, 'gen_pt_2')
        var( tr, 'pt_2')
        var( tr, 'phi_2')
        var( tr, 'eta_2')
        var( tr, 'm_2')
        var( tr, 'q_2')
        var( tr, 'iso_2')
        #var( tr, 'mva_2')
        var( tr, 'antiele_2')
        var( tr, 'passid_2', int)
        var( tr, 'passiso_2', int)
        var( tr, 'mt_2')
        var( tr, 'trigweight_2')
        var( tr, 'idweight_2')
        var( tr, 'isoweight_2')
        var( tr, 'byCombinedIsolationDeltaBetaCorrRaw3Hits_2')
        var( tr, 'againstElectronMVA3raw_2')
        var( tr, 'againstElectronMVA3category_2')
        var( tr, 'byIsolationMVA2raw_2')
        var( tr, 'againstMuonLoose2_2')
        var( tr, 'againstMuonLoose_2')
        var( tr, 'againstMuonMedium2_2')
        var( tr, 'againstMuonTight2_2')
        var( tr, 'againstElectronLooseMVA3_2')
        var( tr, 'againstElectronLoose_2')
        var( tr, 'againstElectronNewLooseMVA3_2')
        
        var( tr, 'met')
        var( tr, 'metphi')
        var( tr, 'mvamet')
        var( tr, 'mvametphi')
        var( tr, 'pzetavis')
        var( tr, 'pzetamiss')

        var( tr, 'metcov00')
        var( tr, 'metcov01')
        var( tr, 'metcov10')
        var( tr, 'metcov11')
        
        var( tr, 'mvacov00')
        var( tr, 'mvacov01')
        var( tr, 'mvacov10')
        var( tr, 'mvacov11')
      
        var( tr, 'jpt_1')
        var( tr, 'jeta_1')
        var( tr, 'jphi_1')
        var( tr, 'jptraw_1')
        var( tr, 'jptunc_1')
        var( tr, 'jmva_1')
        var( tr, 'jpass_1', int)
        
        var( tr, 'jpt_2')
        var( tr, 'jeta_2')
        var( tr, 'jphi_2')
        var( tr, 'jptraw_2')
        var( tr, 'jptunc_2')
        var( tr, 'jmva_2')
        var( tr, 'jpass_2', int)
        
        var( tr, 'bpt')
        var( tr, 'beta')
        var( tr, 'bphi')
        
        var( tr, 'mjj')
        var( tr, 'jdeta')
        var( tr, 'njetingap', int)
        var( tr, 'mva')
        
        var( tr, 'jdphi')
        var( tr, 'dijetpt')
        var( tr, 'dijetphi')
        var( tr, 'hdijetphi')
        var( tr, 'visjeteta')
        var( tr, 'ptvis')
        
        var( tr, 'nbtag', int)
        var( tr, 'njets', int)
        var( tr, 'njetspt20', int)

        var( tr, 'l1TrigMatched_diTau', int)
        var( tr, 'l2TrigMatched_diTau', int)

        var( tr, 'l1TrigMatched_diTauJet' , int)
        var( tr, 'l2TrigMatched_diTauJet' , int)
        var( tr, 'jetTrigMatched_diTauJet', int)

        ####### TRIGGER WEIGHTS
        var( tr, 'triggerWeight_diTauJet' )
        var( tr, 'triggerEffMC_diTauJet'  )
        var( tr, 'triggerEffData_diTauJet')

        var( tr, 'triggerWeight_diTau' )
        var( tr, 'triggerEffMC_diTau'  )
        var( tr, 'triggerEffData_diTau')

        ####### LEPTON VETO 
        var( tr, 'muon1Pt'    )
        var( tr, 'electron1Pt')

        ####### EMBEDDED WEIGHTS
        var( tr, 'genfilter')
        var( tr, 'tauspin'  )
        var( tr, 'zmumusel' )
        var( tr, 'muradcorr')
        var( tr, 'genTau2PtVsGenTau1Pt'    )
        var( tr, 'genTau2EtaVsGenTau1Eta'  )
        var( tr, 'genDiTauMassVsGenDiTauPt')



    def declareHandles(self):
        super(H2TauTauSyncTreeTauTau, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
            'cmgPFMETRaw',
            'std::vector<cmg::BaseMET>' 
            )        
        self.handles['pfmetsig'] = AutoHandle(
            'pfMetSignificance',
            'cmg::METSignificance' 
            )                

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        
        tr = self.tree
        tr.reset()

        #import pdb ; pdb.set_trace()
        if hasattr(event,"run"):
            fill( tr,'run', event.run)
        if hasattr(event,"lumi"):
            fill( tr,'lumi', event.lumi)
        if hasattr(event,"eventId"):
            fill( tr,'evt', event.eventId)
                        
        fill( tr, 'npv', len(event.goodVertices)) 
        nPU = -1
        if hasattr(event, 'pileUpInfo'):
            nPU = event.pileUpInfo[1].nTrueInteractions()
            fill( tr, 'npu', nPU) 
        fill( tr, 'rho', event.rho)
        fill( tr, 'NUP', event.NUP )
        
        #fill( tr, 'mcweight', event.eventWeight) 
        #fill( tr, 'puweight', event.vertexWeight) #OK
        # no need to invert in tau tau case! 
        
        ### taus pt hierarchy can be swapped after TauES correction
        if event.diLepton.leg1().pt() >= event.diLepton.leg2().pt() :   
          leg1 = event.diLepton.leg1()
          leg2 = event.diLepton.leg2()
          fill( tr, 'mt_1', event.diLepton.mTLeg1())
          fill( tr, 'mt_2', event.diLepton.mTLeg2())
        if event.diLepton.leg1().pt() < event.diLepton.leg2().pt() :   
          leg2 = event.diLepton.leg1()
          leg1 = event.diLepton.leg2()
          fill( tr, 'mt_2', event.diLepton.mTLeg1())
          fill( tr, 'mt_1', event.diLepton.mTLeg2())
        
        #fill( tr, 'effweight', leg1.weight*leg2.weight) 
        fill( tr, 'weight', event.eventWeight ) 
        
        fill( tr, 'mvis', event.diLepton.mass() )
        fill( tr, 'm_sv', event.diLepton.massSVFit() ) 
        fill( tr, 'm_sv_Up'  , -1) #? tau up 3%
        fill( tr, 'm_sv_Down', -1) #? tau down 3%
        
        fill( tr, 'gen_pt_1', leg1.physObj.genJetp4().pt())
        fill( tr, 'pt_1', leg1.pt())
        fill( tr, 'phi_1', leg1.phi())
        fill( tr, 'eta_1', leg1.eta())
        fill( tr, 'm_1', leg1.mass())
        fill( tr, 'q_1', leg1.charge())
        fill( tr, 'iso_1', leg1.tauID("byRawIsoMVA"))        
        fill( tr, 'antiele_1', leg2.tauID("againstElectronMVA")) # we probably need the mva output here. I guess this is a working point
        #fill( tr, 'mva_1', leg1.tauID("againstElectronMVA")) # we probably need the mva output here. I guess this is a working point
        fill( tr, 'passid_1',  1)
        fill( tr, 'passiso_1', 1)
        fill( tr, 'trigweight_1', leg1.triggerWeight)
        fill( tr, 'idweight_1'  , leg1.idWeight)
        fill( tr, 'isoweight_1' , leg1.isoWeight)
        fill( tr, 'byCombinedIsolationDeltaBetaCorrRaw3Hits_1', leg1.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits"))
        fill( tr, 'againstElectronMVA3raw_1'     , leg1.tauID("againstElectronMVA3raw"))
        fill( tr, 'againstElectronMVA3category_1', leg1.tauID("againstElectronMVA3category"))
        fill( tr, 'byIsolationMVA2raw_1'         , leg1.tauID("byIsolationMVA2raw"))
        fill( tr, 'againstMuonLoose2_1'          , leg1.tauID("againstMuonLoose2"))
        fill( tr, 'againstMuonLoose_1'           , leg1.tauID("againstMuonLoose"))
        fill( tr, 'againstMuonMedium2_1'         , leg1.tauID("againstMuonMedium2"))
        fill( tr, 'againstMuonTight2_1'          , leg1.tauID("againstMuonTight2"))
        fill( tr, 'againstElectronLooseMVA3_1'   , leg1.tauID("againstElectronLooseMVA3"))
        fill( tr, 'againstElectronLoose_1'       , leg1.tauID("againstElectronLoose"))
        fill( tr, 'againstElectronNewLooseMVA3_1', leg1.NewLooseAntiEleMVA3)
        
        fill( tr, 'gen_pt_2' , leg2.physObj.genJetp4().pt())
        fill( tr, 'pt_2'     , leg2.pt())
        fill( tr, 'phi_2'    , leg2.phi())
        fill( tr, 'eta_2'    , leg2.eta())
        fill( tr, 'm_2'      , leg2.mass())
        fill( tr, 'q_2'      , leg2.charge())
        fill( tr, 'iso_2'    , leg2.tauID("byRawIsoMVA"))
        fill( tr, 'antiele_2', leg2.tauID("againstElectronMVA")) # we probably need the mva output here. I guess this is a working point
        fill( tr, 'passid_2' , 1)
        fill( tr, 'passiso_2', 1)
        fill( tr, 'trigweight_2', leg2.triggerWeight)
        fill( tr, 'idweight_2'  , leg2.idWeight)
        fill( tr, 'isoweight_2' , leg2.isoWeight)
        fill( tr, 'byCombinedIsolationDeltaBetaCorrRaw3Hits_2', leg2.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits"))
        fill( tr, 'againstElectronMVA3raw_2'     , leg2.tauID("againstElectronMVA3raw"))
        fill( tr, 'againstElectronMVA3category_2', leg2.tauID("againstElectronMVA3category"))
        fill( tr, 'byIsolationMVA2raw_2'         , leg2.tauID("byIsolationMVA2raw"))
        fill( tr, 'againstMuonLoose2_2'          , leg2.tauID("againstMuonLoose2"))
        fill( tr, 'againstMuonLoose_2'           , leg2.tauID("againstMuonLoose"))
        fill( tr, 'againstMuonMedium2_2'         , leg2.tauID("againstMuonMedium2"))
        fill( tr, 'againstMuonTight2_2'          , leg2.tauID("againstMuonTight2"))
        fill( tr, 'againstElectronLooseMVA3_2'   , leg2.tauID("againstElectronLooseMVA3"))
        fill( tr, 'againstElectronLoose_2'       , leg2.tauID("againstElectronLoose"))
        fill( tr, 'againstElectronNewLooseMVA3_2', leg2.NewLooseAntiEleMVA3)
        
        met = self.handles['pfmetraw'].product()[0]
        fill( tr, 'met', met.pt()) # raw 
        fill( tr, 'metphi', met.phi()) # raw
        fill( tr, 'mvamet', event.diLepton.met().pt()) 
        fill( tr, 'mvametphi', event.diLepton.met().phi())
        fill( tr, 'pzetavis', event.diLepton.pZetaVis())
        fill( tr, 'pzetamiss', event.diLepton.pZetaMET())

        fill( tr, 'mvacov00', event.diLepton.metSig().significance()(0,0))
        fill( tr, 'mvacov01', event.diLepton.metSig().significance()(0,1))
        fill( tr, 'mvacov10', event.diLepton.metSig().significance()(1,0))
        fill( tr, 'mvacov11', event.diLepton.metSig().significance()(1,1))

        fill( tr, 'metcov00', event.metsig.significance()(0,0))
        fill( tr, 'metcov01', event.metsig.significance()(0,1))
        fill( tr, 'metcov10', event.metsig.significance()(1,0))
        fill( tr, 'metcov11', event.metsig.significance()(1,1))

#         metsig = self.handles['pfmetsig'].product().significance()
#         fill( tr, 'metcov00', metsig(0,0))
#         fill( tr, 'metcov01', metsig(0,1))
#         fill( tr, 'metcov10', metsig(1,0))
#         fill( tr, 'metcov11', metsig(1,1))
#         
#         
#         mvametsig = self.handles['recoilcorrectedmvamet'].product().significance()
#         fill( tr, 'mvacov00', mvametsig(0,0))
#         fill( tr, 'mvacov01', mvametsig(0,1))
#         fill( tr, 'mvacov10', mvametsig(1,0))
#         fill( tr, 'mvacov11', mvametsig(1,1))

#         if hasattr( event.diLepton, 'mvaMetSig'):
#           mvametsig = event.diLepton.mvaMetSig.significance()
#           fill( tr, 'mvacov00', mvametsig(0,0))
#           fill( tr, 'mvacov01', mvametsig(0,1))
#           fill( tr, 'mvacov10', mvametsig(1,0))
#           fill( tr, 'mvacov11', mvametsig(1,1))

        nJets = len(event.cleanJets30)
        nJetsPt20 = len(event.cleanJets)
        jets = event.cleanJets30
        if self.cfg_ana.pt20:
            jets = event.cleanJets
        if nJets>=1:
            j1 = jets[0]
            fill( tr, 'jpt_1'   , j1.pt())
            fill( tr, 'jeta_1'  , j1.eta())
            fill( tr, 'jphi_1'  , j1.phi())
            fill( tr, 'jptraw_1', j1.pt()*j1.rawFactor())
            fill( tr, 'jptunc_1', j1.uncOnFourVectorScale())
            fill( tr, 'jmva_1'  , j1.puMva("full53x"))
            # 2 is the loose working point
            fill( tr, 'jpass_1' , j1.passPuJetId("full53x", 2))

        if nJets>=2:
            j2 = jets[1]
            fill( tr, 'jpt_2'   , j2.pt())
            fill( tr, 'jeta_2'  , j2.eta())
            fill( tr, 'jphi_2'  , j2.phi())
            fill( tr, 'jptraw_2', j2.pt()*j2.rawFactor())
            fill( tr, 'jptunc_2', j2.uncOnFourVectorScale())
            fill( tr, 'jmva_2'  , j2.puMva("full53x"))
            # 2 is the loose working point
            fill( tr, 'jpass_2'  , j2.passPuJetId("full53x", 2))

        if len(event.cleanBJets)>0:
            fill( tr, 'bpt' , event.cleanBJets[0].pt())
            fill( tr, 'beta', event.cleanBJets[0].eta())
            fill( tr, 'bphi', event.cleanBJets[0].phi())

        if hasattr( event, 'vbf'):
            vbf = event.vbf
            # import pdb; pdb.set_trace()
            fill( tr, 'mjj', vbf.mjj)
            fill( tr, 'jdeta', abs(vbf.deta))
            fill( tr, 'njetingap', len(vbf.centralJets))
            fill( tr, 'mva', vbf.mva)
            
            # add the following variables to the vbf object in VBFAnalyzer
            fill( tr, 'jdphi'    , abs(vbf.dphi))
            fill( tr, 'dijetpt'  , vbf.dijetpt)
            fill( tr, 'dijetphi' , vbf.dijetphi)
            fill( tr, 'hdijetphi', abs(vbf.dphidijethiggs)) # higgs p = p1 + p2 + met
            fill( tr, 'visjeteta', vbf.visjeteta)
            fill( tr, 'ptvis'    , vbf.ptvis)

        # trigger matching for diTau trigger present in parked dataset 
	if hasattr(event,"l1TrigMatched_diTau") and hasattr(event,"l2TrigMatched_diTau") :
          fill( tr, 'l1TrigMatched_diTau' , event.l1TrigMatched_diTau )
          fill( tr, 'l2TrigMatched_diTau' , event.l2TrigMatched_diTau )
	else:
          fill( tr, 'l1TrigMatched_diTau' , -1)
          fill( tr, 'l2TrigMatched_diTau' , -1)

        # trigger matching for diTauJet trigger 
	if hasattr(event,"l1TrigMatched_diTauJet") and hasattr(event,"l2TrigMatched_diTauJet") and hasattr(event,"jetTrigMatched_diTauJet"):
          fill( tr, 'l1TrigMatched_diTauJet' , event.l1TrigMatched_diTauJet )
          fill( tr, 'l2TrigMatched_diTauJet' , event.l2TrigMatched_diTauJet )
          fill( tr, 'jetTrigMatched_diTauJet', event.jetTrigMatched_diTauJet)
	else:
          fill( tr, 'l1TrigMatched_diTauJet' , -1)
          fill( tr, 'l2TrigMatched_diTauJet' , -1)
          fill( tr, 'jetTrigMatched_diTauJet', -1)


	if hasattr(event,"triggerWeight_diTauJet") and hasattr(event,"triggerEffMC_diTauJet") and hasattr(event,"triggerEffData_diTauJet"):
          fill( tr, 'triggerWeight_diTauJet' ,event.triggerWeight_diTauJet )
          fill( tr, 'triggerEffMC_diTauJet'  ,event.triggerEffMC_diTauJet  )
          fill( tr, 'triggerEffData_diTauJet',event.triggerEffData_diTauJet)
        else :
          fill( tr, 'triggerWeight_diTauJet' ,1)
          fill( tr, 'triggerEffMC_diTauJet'  ,1)
          fill( tr, 'triggerEffData_diTauJet',1)


	if hasattr(event,"triggerWeight_diTau") and hasattr(event,"triggerEffMC_diTau") and hasattr(event,"triggerEffData_diTau"):
          fill( tr, 'triggerWeight_diTau' ,event.triggerWeight_diTau )
          fill( tr, 'triggerEffMC_diTau'  ,event.triggerEffMC_diTau  )
          fill( tr, 'triggerEffData_diTau',event.triggerEffData_diTau)
        else :
          fill( tr, 'triggerWeight_diTau' ,1)
          fill( tr, 'triggerEffMC_diTau'  ,1)
          fill( tr, 'triggerEffData_diTau',1)


        fill( tr, 'nbtag', len(event.cleanBJets))
        fill( tr, 'njets', nJets)
        fill( tr, 'njetspt20', nJetsPt20)

        #import pdb ; pdb.set_trace()         
        if len(event.muons)>0:
            #import pdb ; pdb.set_trace()
            fill( tr, 'muon1Pt' , event.muons[0].pt()  )
	else:
            fill( tr, 'muon1Pt' , -1 )

        if len(event.electrons)>0:
            #import pdb ; pdb.set_trace()
            fill( tr, 'electron1Pt' , event.electrons[0].pt()  )
	else:
            fill( tr, 'electron1Pt' , -1 )

        ####### EMBEDDED WEIGHTS
        fill( tr, 'genfilter'               , event.genfilter                )
        fill( tr, 'tauspin'                 , event.tauspin                  )
        fill( tr, 'zmumusel'                , event.zmumusel                 )
        fill( tr, 'muradcorr'               , event.muradcorr                )
        fill( tr, 'genTau2PtVsGenTau1Pt'    , event.genTau2PtVsGenTau1Pt     )
        fill( tr, 'genTau2EtaVsGenTau1Eta'  , event.genTau2EtaVsGenTau1Eta   )
        fill( tr, 'genDiTauMassVsGenDiTauPt', event.genDiTauMassVsGenDiTauPt )
        
        ### MY WEIGTHS
        fill( tr, 'effweight'       , event.triggerWeight_diTau )
        fill( tr, 'puweight'        , event.vertexWeight        )
        fill( tr, 'mcweight'        , 1./event.ntot             )  
        DMweight = 1.
        if leg1.decayMode()==0 :
          DMweight *= 0.88 
        if leg2.decayMode()==0 :
          DMweight *= 0.88 
        fill( tr, 'decaymodeweight' , DMweight)

        self.tree.tree.Fill()
        return True
