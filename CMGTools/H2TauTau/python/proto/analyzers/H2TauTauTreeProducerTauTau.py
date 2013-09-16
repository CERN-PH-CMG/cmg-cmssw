from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
from CMGTools.RootTools.utils.DeltaR import bestMatch
from math import *
import inspect

def lineno():
    '''Returns the current line number in our program.'''
    return inspect.currentframe().f_back.f_lineno



def deltaPhi(phi1, phi2):
   p=abs(phi1-phi2)
   if p<pi:
      return p
   else:
      return 2*pi-p

def deltaR(eta1, phi1, eta2, phi2) :
    deta = eta1-eta2
    dphi = deltaPhi(phi1,phi2)
    return sqrt(deta*deta + dphi*dphi)


class H2TauTauTreeProducerTauTau( TreeAnalyzer ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def varInt( varName ):
            self.tree.addVar('int', varName)

        def particleVars( pName ):
            var('{pName}Px'.format(pName=pName)                         )
            var('{pName}Py'.format(pName=pName)                         )
            var('{pName}Pz'.format(pName=pName)                         )
            var('{pName}E'.format(pName=pName)                          )
            var('{pName}Pt'.format(pName=pName)                         )
            var('{pName}Eta'.format(pName=pName)                        )
            var('{pName}Phi'.format(pName=pName)                        )
            var('{pName}Charge'.format(pName=pName)                     )
            var('{pName}Mass'.format(pName=pName)                       )            
            if not ('diTau' in pName or 'jet' in pName or 'L1' in pName):
              var('{pName}DecayMode'.format(pName=pName)                  )
              var('{pName}RawDB3HIso'.format(pName=pName)                 )
              var('{pName}LooseDB3HIso'.format(pName=pName)               )
              var('{pName}MediumDB3HIso'.format(pName=pName)              )
              var('{pName}TightDB3HIso'.format(pName=pName)               )
              var('{pName}LooseEle'.format(pName=pName)                   )
              var('{pName}MedEle'.format(pName=pName)                     )
              var('{pName}MVAEle'.format(pName=pName)                     )
              var('{pName}LooseMu'.format(pName=pName)                    )
              var('{pName}jetMass'.format(pName=pName)                    )
              var('{pName}jetPt'.format(pName=pName)                      )
              var('{pName}againstElectronMVA3raw'.format(pName=pName)     )
              #var('{pName}againstElectronLooseMVA3'.format(pName=pName)   )
              #var('{pName}againstElectronMediumMVA3'.format(pName=pName)  )
              #var('{pName}againstElectronTightMVA3'.format(pName=pName)   )
              var('{pName}againstElectronVTightMVA3'.format(pName=pName)  )
              var('{pName}againstElectronMVA3category'.format(pName=pName))
              var('{pName}againstElectronNewLooseMVA3'.format(pName=pName))
              var('{pName}againstMuonLoose2'.format(pName=pName)          )
              var('{pName}againstMuonMedium2'.format(pName=pName)         )
              var('{pName}againstMuonTight2'.format(pName=pName)          )
              var('{pName}VertexZ'.format(pName=pName)                    )
              var('{pName}GenPdgId'.format(pName=pName)                   )
              var('{pName}GenPt'.format(pName=pName)                      )
              var('{pName}GenEta'.format(pName=pName)                     )
              var('{pName}GenPhi'.format(pName=pName)                     )
              var('{pName}GenMass'.format(pName=pName)                    )
            
        varInt('run'  )
        varInt('lumi' )
        varInt('event')
	
	varInt('isRealTau')
	
        var('visMass'  )
        var('svfitMass')
        var('mt1')
        var('mt2')
        var('l1Radius')
        var('l2Radius')
	var('pThiggs')
        
        var('mvacov00')
        var('mvacov01')
        var('mvacov10')
        var('mvacov11')
	var('metPhi')
	var('mex')
	var('mey')
	var('met')
	var('mttj')

	var('rawMET')

	var('mjj')
	var('dEtajj')
	var('dPhijj')
# 	var('dPhittjj')
# 	var('dEtattjj')
	varInt('nCentralJets')
# 	var('vbfMVA')

        var('dRtt')
        var('dRttL1')
        var('dEtatt')
        var('dPhitt')

        particleVars('diTau')
        particleVars('l1')
        particleVars('l2')

#         var('l1jetMass')
#         var('l1jetPt')
#         var('l1jetWidth')
#         var('l1jetBtag')
# 
#         var('l2jetMass')
#         var('l2jetPt')
#         var('l2jetWidth')
#         var('l2jetBtag')
# 
#         var('l1match')
#         var('l2match')
# 
#         var('l1JetInvMass')
#         var('l2JetInvMass')

        ####### TRIGGER MATCHING
        varInt( 'l1TrigMatched_diTau'    )
        varInt( 'l2TrigMatched_diTau'    )
        varInt( 'l1TrigMatched_diTauJet' )
        varInt( 'l2TrigMatched_diTauJet' )
        varInt( 'jetTrigMatched_diTauJet')

        ####### TRIGGER WEIGHTS
        var( 'triggerWeight_diTauJet' )
        var( 'triggerEffMC_diTauJet'  )
        var( 'triggerEffData_diTauJet')

        var( 'triggerWeight_diTau' )
        var( 'triggerEffMC_diTau'  )
        var( 'triggerEffData_diTau')

        ####### JETS VARIABLES
        varInt('nJets')
        particleVars('jet1')
        particleVars('jet2')
        var('jet1Btag')
        var('jet2Btag')
        var('jet1Bmatch')
        var('jet2Bmatch')
        #var('sumJetPt')
        #var('allJetPt')
        varInt('nbJets')
        particleVars('bjet1')
        particleVars('bjet2')
        #var('sumbJetPt')
        #var('allbJetPt')

        ####### WEIGHTS AND VERTICES
        var('weight')
        var('vertexWeight')
        var('embedWeight')
        var('triggerWeight')
        var('triggerEffData')
        var('triggerEffMC')
        varInt('nVert')

        ####### L1 MATCHING PARTICLES
        var('l1L1Px'    )
        var('l1L1Py'    )
        var('l1L1Pz'    )
        var('l1L1E'     )
        var('l1L1Pt'    )
        var('l1L1Eta'   )
        var('l1L1Phi'   )
        var('l1L1Charge')
        var('l1L1Mass'  )
        
        var('l2L1Px'    )
        var('l2L1Py'    )
        var('l2L1Pz'    )
        var('l2L1E'     )
        var('l2L1Pt'    )
        var('l2L1Eta'   )
        var('l2L1Phi'   )
        var('l2L1Charge')
        var('l2L1Mass'  )
        
        ####### GENERATOR VARIABLES
        varInt('genMatched')
        varInt('isFake'    )
        varInt('isZtt'     )
        varInt('isZee'     )
        varInt('isZmm'     )
        varInt('isZj'      )
        varInt('isZttj'    )
        varInt('isZttll'   )
        varInt('isPhoton'  )
        varInt('isElectron')
        varInt('isMuon'    )
        varInt('hasW'      )
        varInt('hasZ'      )
        var('genMass'      )
        #var('genMassSmeared'  )
        #var('svfitMassSmeared')

        ####### THIRD LEPTON VETO
        var('electron1Pt' )
        var('electron1Eta')
        var('electron1Phi')
        var('muon1Pt' )
        var('muon1Eta')
        var('muon1Phi')
        
        ####### HIGGS PT WEIGHTS
        var('higgsPtWeightNom')
        var('higgsPtWeightUp')
        var('higgsPtWeightDown')

        ####### EMBEDDED WEIGHTS
        var('genfilter'               )
        var('tauspin'                 )
        var('zmumusel'                )
        var('muradcorr'               )
        var('genTau2PtVsGenTau1Pt'    )
        var('genTau2EtaVsGenTau1Eta'  )
        var('genDiTauMassVsGenDiTauPt')
        
        ####### NUM OF GEN JETS FOR STITCHING
        var('NUP')

	self.triggers=[
	 'HLT_LooseIsoPFTau35_Trk20_Prong1_v6',
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v2',
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_v2',
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6',
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_Prong1_v6',
         'HLT_Mu17_Mu8_v16',
         'HLT_Mu17_Mu8_v17',
         'HLT_Mu17_Mu8_v18',
         'HLT_Mu17_Mu8_v19',
         'HLT_Mu17_Mu8_v21',
         'HLT_Mu17_Mu8_v21',
         'HLT_Mu17_Mu8_v22',
         ]
        for trig in self.triggers:
            varInt(trig)
	
        self.tree.book()

    def process(self, iEvent, event):

        #if event.eventId in notPassed :
        #  print 'WEIRD!! I have this event'
        #  import pdb ; pdb.set_trace()

        scale=1.0

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fParticleVars( pName, particle ):
            fill('{pName}Px'.format(pName=pName)     , particle.px()*scale     )
            fill('{pName}Py'.format(pName=pName)     , particle.py()*scale     )
            fill('{pName}Pz'.format(pName=pName)     , particle.pz()*scale     )
            fill('{pName}E'.format(pName=pName)      , particle.energy()*scale )
            fill('{pName}Pt'.format(pName=pName)     , particle.pt()*scale     )
            fill('{pName}Eta'.format(pName=pName)    , particle.eta()          )
            fill('{pName}Phi'.format(pName=pName)    , particle.phi()          )
            fill('{pName}Charge'.format(pName=pName) , particle.charge()       )
            fill('{pName}Mass'.format(pName=pName)   , particle.mass()*scale   )
            if not ('diTau' in pName or 'jet' in pName or 'L1' in pName):
              fill('{pName}DecayMode'.format(pName=pName)                  , particle.decayMode()                                         )
              fill('{pName}RawDB3HIso'.format(pName=pName)                 , particle.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")   )
              fill('{pName}LooseDB3HIso'.format(pName=pName)               , particle.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") )
              fill('{pName}MediumDB3HIso'.format(pName=pName)              , particle.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"))
              fill('{pName}TightDB3HIso'.format(pName=pName)               , particle.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits") )
              fill('{pName}LooseEle'.format(pName=pName)                   , particle.tauID("againstElectronLoose")                       )
              fill('{pName}MedEle'.format(pName=pName)                     , particle.tauID("againstElectronMedium")                      )
              fill('{pName}MVAEle'.format(pName=pName)                     , particle.tauID("againstElectronMVA")                         )
              fill('{pName}LooseMu'.format(pName=pName)                    , particle.tauID("againstMuonLoose")                           )
              fill('{pName}jetMass'.format(pName=pName)                    , particle.jetRefp4().mass()                                   )
              fill('{pName}jetPt'.format(pName=pName)                      , particle.jetRefp4().pt()                                     )
              fill('{pName}againstElectronMVA3raw'.format(pName=pName)     , particle.tauID("againstElectronMVA3raw")                     )
              fill('{pName}againstElectronLooseMVA3'.format(pName=pName)   , particle.tauID("againstElectronLooseMVA3")                   )
              fill('{pName}againstElectronMediumMVA3'.format(pName=pName)  , particle.tauID("againstElectronMediumMVA3")                  )
              fill('{pName}againstElectronTightMVA3'.format(pName=pName)   , particle.tauID("againstElectronTightMVA3")                   )
              fill('{pName}againstElectronVTightMVA3'.format(pName=pName)  , particle.tauID("againstElectronVTightMVA3")                  )
              fill('{pName}againstElectronMVA3category'.format(pName=pName), particle.tauID("againstElectronMVA3category")                )
              fill('{pName}againstElectronNewLooseMVA3'.format(pName=pName), particle.NewLooseAntiEleMVA3                                 )
              fill('{pName}againstMuonLoose2'.format(pName=pName)          , particle.tauID("againstMuonLoose2")                          )
              fill('{pName}againstMuonMedium2'.format(pName=pName)         , particle.tauID("againstMuonMedium2")                         )
              fill('{pName}againstMuonTight2'.format(pName=pName)          , particle.tauID("againstMuonTight2")                          )
              fill('{pName}VertexZ'.format(pName=pName)                    , particle.tau.vz()                                            )

        if hasattr(event,"run"):
          fill('run', event.run)
        if hasattr(event,"lumi"):
          fill('lumi', event.lumi)
        if hasattr(event,"eventId"):
          fill('event', event.eventId)
        
        if hasattr(event,"isRealTau"):
          fill('isRealTau', event.isRealTau)
          
        fill('visMass'  , event.diLepton.mass()*scale)
        fill('svfitMass', event.diLepton.massSVFit()*scale)
        ### import pdb ; pdb.set_trace() ## SVFit pT is missingin tautau cmgtuples
        ### need to hack http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h?revision=1.11&view=markup
        #fill('svfitPt' , event.diLepton.massSVFit()*scale)
        fill('pThiggs'  , sqrt(pow(event.diLepton.met().px()+event.diLepton.leg1().px()+event.diLepton.leg2().px(),2)+pow(event.diLepton.met().py()+event.diLepton.leg1().py()+event.diLepton.leg2().py(),2))*scale)

        #mvametsig = event.diLepton.mvaMetSig.significance()
        fill('mvacov00', event.diLepton.metSig().significance()(0,0))
        fill('mvacov01', event.diLepton.metSig().significance()(0,1))
        fill('mvacov10', event.diLepton.metSig().significance()(1,0))
        fill('mvacov11', event.diLepton.metSig().significance()(1,1))
        fill('metPhi'  , event.diLepton.met().phi()*scale)
        fill('mex'     , event.diLepton.met().px()*scale)
        fill('mey'     , event.diLepton.met().py()*scale)
        fill('met'     , event.diLepton.met().pt()*scale)
        fill('rawMET'  , event.rawMET[0].pt())
        fill('dRtt'    , sqrt(pow(deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()),2)+pow(fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()),2)))
        fill('dPhitt'  , deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()))
        fill('dEtatt'  , fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()))

        if hasattr(event.leg1,"L1particle") and hasattr(event.leg2,"L1particle") :
          fill('dRttL1'  , sqrt(pow(deltaPhi(event.leg1.L1particle.phi(),event.leg2.L1particle.phi()),2)+pow(fabs(event.leg1.L1particle.eta()-event.leg2.L1particle.eta()),2)))

          fill('l1L1Px'    , event.leg1.L1particle.px()    )
          fill('l1L1Py'    , event.leg1.L1particle.py()    )
          fill('l1L1Pz'    , event.leg1.L1particle.pz()    )
          fill('l1L1E'     , event.leg1.L1particle.energy())
          fill('l1L1Pt'    , event.leg1.L1particle.pt()    )
          fill('l1L1Eta'   , event.leg1.L1particle.eta()   )
          fill('l1L1Phi'   , event.leg1.L1particle.phi()   )
          fill('l1L1Charge', event.leg1.L1particle.charge())
          fill('l1L1Mass'  , event.leg1.L1particle.mass()  )
          
          fill('l2L1Px'    , event.leg2.L1particle.px()    )
          fill('l2L1Py'    , event.leg2.L1particle.py()    )
          fill('l2L1Pz'    , event.leg2.L1particle.pz()    )
          fill('l2L1E'     , event.leg2.L1particle.energy())
          fill('l2L1Pt'    , event.leg2.L1particle.pt()    )
          fill('l2L1Eta'   , event.leg2.L1particle.eta()   )
          fill('l2L1Phi'   , event.leg2.L1particle.phi()   )
          fill('l2L1Charge', event.leg2.L1particle.charge())
          fill('l2L1Mass'  , event.leg2.L1particle.mass()  )
          #fParticleVars('l1L1', event.leg1.L1particle)
          #fParticleVars('l2L1', event.leg2.L1particle)

        ### taus pt hierarchy can be swapped after TauES correction
        if event.diLepton.leg1().pt() >= event.diLepton.leg2().pt() :   
          leg1 = event.diLepton.leg1()
          leg2 = event.diLepton.leg2()
          fill('mt1', event.diLepton.mTLeg1()*scale)
          fill('mt2', event.diLepton.mTLeg2()*scale)
        elif event.diLepton.leg1().pt() < event.diLepton.leg2().pt() :   
          leg2 = event.diLepton.leg1()
          leg1 = event.diLepton.leg2()
          fill('mt2', event.diLepton.mTLeg1()*scale)
          fill('mt1', event.diLepton.mTLeg2()*scale)
	
	## tau1 radius
	eta = []
	phi = []
	for sigCand in event.diLepton.leg1().physObj.signalPFCands() :
	  eta.append(sigCand.eta())
	  phi.append(sigCand.phi())
        dR = 0.
        for i in range(len(eta)) :
	  if deltaR(leg1.eta(), leg1.phi(), eta[i], phi[i]) > dR :
            dR = deltaR(leg1.eta(), leg1.phi(), eta[i], phi[i])
	fill('l1Radius', dR)

	## tau2 radius
	eta = []
	phi = []
	for sigCand in event.diLepton.leg2().physObj.signalPFCands() :
	  eta.append(sigCand.eta())
	  phi.append(sigCand.phi())
        dR = 0.
        for i in range(len(eta)) :
	  if deltaR(leg2.eta(), leg2.phi(), eta[i], phi[i]) > dR :
            dR = deltaR(leg2.eta(), leg2.phi(), eta[i], phi[i])
	fill('l2Radius', dR)

	
	
        fParticleVars('l1', leg1 )
        fParticleVars('l2', leg2 )

        l1jet = bestMatch( leg1, event.jets )[0]
        l2jet = bestMatch( leg2, event.jets )[0]
	
        if l1jet:
          fill('l1jetWidth', l1jet.rms() )
          fill('l1jetBtag' , l1jet.btag("combinedSecondaryVertexBJetTags") )
        
        if l2jet:
          fill('l2jetWidth', l2jet.rms() )
          fill('l2jetBtag' , l1jet.btag("combinedSecondaryVertexBJetTags") )

        if hasattr(event,"leg1DeltaR"):
            fill('l1match', event.leg1DeltaR )
            fill('l2match', event.leg2DeltaR )
	else:
            fill('l1match', -1 )
            fill('l2match', -1 )

	if hasattr(event,"genMass"):
            fill('genMass', event.genMass )
#	    from ROOT import TFile,TH1F
#            f = TFile("/afs/cern.ch/user/h/hinzmann/workspace/stable2012/CMGTools/CMSSW_5_2_5/src/CMGTools/H2TauTau/python/proto/embeddedWeights.root")
#            hEmbed = f.Get("hemb")
#            shift=hEmbed.GetRandom()/event.genMass
#            fill('genMassSmeared', event.genMass*shift )
#            fill('svfitMassSmeared', event.diLepton.massSVFit()*scale*shift )
	else:
            fill('genMass'         , -1 )
            fill('genMassSmeared'  , -1 )
            fill('svfitMassSmeared', -1 )

        # trigger matching for diTau trigger present in parked dataset 
	if hasattr(event,"l1TrigMatched_diTau") and hasattr(event,"l2TrigMatched_diTau") :
          fill( 'l1TrigMatched_diTau' , event.l1TrigMatched_diTau )
          fill( 'l2TrigMatched_diTau' , event.l2TrigMatched_diTau )
	else:
          fill( 'l1TrigMatched_diTau' , -1)
          fill( 'l2TrigMatched_diTau' , -1)

        # trigger matching for diTauJet trigger 
	if hasattr(event,"l1TrigMatched_diTauJet") and hasattr(event,"l2TrigMatched_diTauJet") and hasattr(event,"jetTrigMatched_diTauJet"):
          fill( 'l1TrigMatched_diTauJet' , event.l1TrigMatched_diTauJet )
          fill( 'l2TrigMatched_diTauJet' , event.l2TrigMatched_diTauJet )
          fill( 'jetTrigMatched_diTauJet', event.jetTrigMatched_diTauJet)
	else:
          fill( 'l1TrigMatched_diTauJet' , -1)
          fill( 'l2TrigMatched_diTauJet' , -1)
          fill( 'jetTrigMatched_diTauJet', -1)


	if hasattr(event,"triggerWeight_diTauJet") and hasattr(event,"triggerEffMC_diTauJet") and hasattr(event,"triggerEffData_diTauJet"):
          fill( 'triggerWeight_diTauJet' ,event.triggerWeight_diTauJet )
          fill( 'triggerEffMC_diTauJet'  ,event.triggerEffMC_diTauJet  )
          fill( 'triggerEffData_diTauJet',event.triggerEffData_diTauJet)
        else :
          fill( 'triggerWeight_diTauJet' ,1)
          fill( 'triggerEffMC_diTauJet'  ,1)
          fill( 'triggerEffData_diTauJet',1)


	if hasattr(event,"triggerWeight_diTau") and hasattr(event,"triggerEffMC_diTau") and hasattr(event,"triggerEffData_diTau"):
          fill( 'triggerWeight_diTau' ,event.triggerWeight_diTau )
          fill( 'triggerEffMC_diTau'  ,event.triggerEffMC_diTau  )
          fill( 'triggerEffData_diTau',event.triggerEffData_diTau)
        else :
          fill( 'triggerWeight_diTau' ,1)
          fill( 'triggerEffMC_diTau'  ,1)
          fill( 'triggerEffData_diTau',1)

        nJets = len(event.cleanJets)
        fill('nJets', nJets )
        if nJets>=1:
            fParticleVars('jet1', event.cleanJets[0] )
            fill('jet1Btag'     , event.cleanJets[0].btag("combinedSecondaryVertexBJetTags") )
            fill('jet1Bmatch'   , event.cleanJets[0].matchGenParton )
	    fill('mttj'         , sqrt(pow(event.diLepton.energy()+event.cleanJets[0].energy(),2) - pow(event.diLepton.px()+event.cleanJets[0].px(),2) - pow(event.diLepton.py()+event.cleanJets[0].py(),2) - pow(event.diLepton.pz()+event.cleanJets[0].pz(),2)))
	    fill('l1JetInvMass' , sqrt(pow(leg1.energy()+event.cleanJets[0].energy(),2) - pow(leg1.px()+event.cleanJets[0].px(),2) - pow(leg1.py()+event.cleanJets[0].py(),2) - pow(leg1.pz()+event.cleanJets[0].pz(),2)))
	    fill('l2JetInvMass' , sqrt(pow(leg2.energy()+event.cleanJets[0].energy(),2) - pow(leg2.px()+event.cleanJets[0].px(),2) - pow(leg2.py()+event.cleanJets[0].py(),2) - pow(leg2.pz()+event.cleanJets[0].pz(),2)))
        if nJets>=2:
            fParticleVars('jet2', event.cleanJets[1] )
            fill('jet2Btag'     , event.cleanJets[1].btag("combinedSecondaryVertexBJetTags") )
            fill('jet2Bmatch'   , event.cleanJets[1].matchGenParton )
	    fill('mjj'          , event.vbf.mjj)
	    fill('dEtajj'       , event.vbf.deta)
	    fill('dPhijj'       , deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi()))
	    fill('dEtattjj'     , fabs(event.cleanJets[0].eta()+event.cleanJets[1].eta()-event.diLepton.eta()))
	    fill('dPhittjj'     , deltaPhi(event.cleanJets[0].phi()+event.cleanJets[1].phi(),event.diLepton.phi()))
	    fill('nCentralJets' , len(event.vbf.centralJets))
	    fill('vbfMVA'       , event.vbf.mva)

        ## Riccardo
        sumJetPt = 0.
        for j in range(nJets) :
          sumJetPt += event.cleanJets[j].pt()
          fill('allJetPt', event.cleanJets[j].pt())
        if nJets == 0 : sumJetPt = -99.
        fill('sumJetPt', sumJetPt)

        nbJets = len(event.cleanBJets)
        fill('nbJets', nbJets )
        if nbJets>=1:
            fParticleVars('bjet1', event.cleanBJets[0] )
        if nbJets>=2:
            fParticleVars('bjet2', event.cleanBJets[1] )

        ## Riccardo
        sumbJetPt = 0.
        for j in range(nbJets) :
          sumbJetPt += event.cleanBJets[j].pt()
          fill('allbJetPt', event.cleanBJets[j].pt())
        if nbJets == 0 : sumbJetPt = -99.
        fill('sumbJetPt', sumbJetPt)

        fill('weight', event.eventWeight)
        if hasattr( event, 'vertexWeight'): 
            fill('vertexWeight', event.vertexWeight  )
            fill('nVert'       , len(event.vertices) ) 
        if hasattr( event, 'embedWeight'): 
            fill('embedWeight', event.embedWeight)
        if hasattr( event, 'triggerWeight'): 
            fill('triggerWeight', event.triggerWeight)

        if hasattr( event, 'triggerEffData'): 
            fill('triggerEffData', event.triggerEffData)
        if hasattr( event, 'triggerEffMC'): 
            fill('triggerEffMC', event.triggerEffMC)

        isFake = 1
        if hasattr( event, 'genMatched'): 
            if event.genMatched == 1:
                isFake = 0
        fill('isFake', isFake)
        
        if hasattr( event, 'isZtt') and hasattr( event, 'isZee') and hasattr( event, 'isZmm') and hasattr( event, 'isZj') and hasattr( event, 'isZttll') and hasattr( event, 'isZttj') : 
          fill('isZtt'  , event.isZtt   )
          fill('isZee'  , event.isZee   )
          fill('isZmm'  , event.isZmm   )
          fill('isZj'   , event.isZj    )
          fill('isZttll', event.isZttll )
          fill('isZttj' , event.isZttj  )
        else :
          fill('isZtt'  , -1 )
          fill('isZee'  , -1 )
          fill('isZmm'  , -1 )
          fill('isZj'   , -1 )
          fill('isZttll', -1 )
          fill('isZttj' , -1 )
        
        
        genMatched = 0
        if hasattr(event,'genMatched') and event.genMatched > 0: genMatched = event.genMatched
        fill('genMatched', genMatched)

        isPhoton = 0
        if hasattr(event,'isPhoton') and event.isPhoton == 1: isPhoton = 1
        fill('isPhoton', isPhoton)

        isElectron = 0
        if hasattr(event,'isElectron') and event.isElectron > 0: isElectron = event.isElectron
        fill('isElectron', isElectron)

        isMuon = 0
        if hasattr(event,'isMuon') and event.isMuon > 0: isMuon = event.isMuon
        fill('isMuon', isMuon)

        hasW = 0
        if hasattr(event,'hasW') and event.hasW == 1: hasW = 1
        fill('hasW', hasW)

        hasZ = 0
        if hasattr(event,'hasZ') and event.hasZ == 1: hasZ = 1
        fill('hasZ', hasZ)

        if hasattr(leg1,'genl') :
          fill('l1GenPdgId', leg1.genl.pdgId())
          fill('l1GenPt'   , leg1.genl.pt()   )
          fill('l1GenEta'  , leg1.genl.eta()  )
          fill('l1GenPhi'  , leg1.genl.phi()  )
          fill('l1GenMass' , leg1.genl.mass() )
        else :
          fill('l1GenPt'  , -1. )
          fill('l1GenEta' , -99.)
          fill('l1GenPhi' , -99.)
          fill('l1GenMass', -1. )
        
        if hasattr(leg2,'genl') :
          fill('l2GenPdgId', leg2.genl.pdgId())
          fill('l2GenPt'   , leg2.genl.pt()  )
          fill('l2GenEta'  , leg2.genl.eta() )
          fill('l2GenPhi'  , leg2.genl.phi() )
          fill('l2GenMass' , leg2.genl.mass())
        else :
          fill('l2GenPt'  , -1. )
          fill('l2GenEta' , -99.)
          fill('l2GenPhi' , -99.)
          fill('l2GenMass', -1. )

        if len(event.muons)>0:
            fill('muon1Pt' , event.muons[0].pt()  )
            fill('muon1Eta', event.muons[0].eta() )
            fill('muon1Phi', event.muons[0].phi() )
	else:
            fill('muon1Pt' , -1 )
            fill('muon1Eta', -1 )
            fill('muon1Phi', -1 )

        if len(event.electrons)>0:
            fill('electron1Pt' , event.electrons[0].pt()  )
            fill('electron1Eta', event.electrons[0].eta() )
            fill('electron1Phi', event.electrons[0].phi() )
	else:
            fill('electron1Pt' , -1 )
            fill('electron1Eta', -1 )
            fill('electron1Phi', -1 )

        fill('higgsPtWeightNom' ,event.higgsPtWeightNom )
        fill('higgsPtWeightUp'  ,event.higgsPtWeightUp  )
        fill('higgsPtWeightDown',event.higgsPtWeightDown)

        ####### EMBEDDED WEIGHTS
        fill( 'genfilter'               , event.genfilter                )
        fill( 'tauspin'                 , event.tauspin                  )
        fill( 'zmumusel'                , event.zmumusel                 )
        fill( 'muradcorr'               , event.muradcorr                )
        fill( 'genTau2PtVsGenTau1Pt'    , event.genTau2PtVsGenTau1Pt     )
        fill( 'genTau2EtaVsGenTau1Eta'  , event.genTau2EtaVsGenTau1Eta   )
        fill( 'genDiTauMassVsGenDiTauPt', event.genDiTauMassVsGenDiTauPt )

        fill('NUP', event.NUP )

        for trig in self.triggers:
            fill(trig, getattr(event,trig))

        self.tree.fill()
