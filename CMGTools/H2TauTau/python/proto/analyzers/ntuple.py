#!/bin/env python
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi


tauIDs = ['againstElectronLoose', 'againstElectronLooseMVA5', 'againstElectronMVA5category', 'againstElectronMVA5raw', 'againstElectronMedium', 'againstElectronMediumMVA5', 'againstElectronTight', 'againstElectronTightMVA5', 'againstElectronVLooseMVA5', 'againstElectronVTightMVA5', 'againstMuonLoose', 'againstMuonLoose2', 'againstMuonLoose3', 'againstMuonLooseMVA', 'againstMuonMVAraw', 'againstMuonMedium', 'againstMuonMedium2', 'againstMuonMediumMVA', 'againstMuonTight', 'againstMuonTight2', 'againstMuonTight3', 'againstMuonTightMVA', 'byCombinedIsolationDeltaBetaCorrRaw3Hits', 'byIsolationMVA3newDMwLTraw', 'byIsolationMVA3newDMwoLTraw', 'byIsolationMVA3oldDMwLTraw', 'byIsolationMVA3oldDMwoLTraw', 'byLooseCombinedIsolationDeltaBetaCorr3Hits', 'byLooseIsolationMVA3newDMwLT', 'byLooseIsolationMVA3newDMwoLT', 'byLooseIsolationMVA3oldDMwLT', 'byLooseIsolationMVA3oldDMwoLT', 'byMediumCombinedIsolationDeltaBetaCorr3Hits', 'byMediumIsolationMVA3newDMwLT', 'byMediumIsolationMVA3newDMwoLT', 'byMediumIsolationMVA3oldDMwLT', 'byMediumIsolationMVA3oldDMwoLT', 'byTightCombinedIsolationDeltaBetaCorr3Hits', 'byTightIsolationMVA3newDMwLT', 'byTightIsolationMVA3newDMwoLT', 'byTightIsolationMVA3oldDMwLT', 'byTightIsolationMVA3oldDMwoLT', 'byVLooseIsolationMVA3newDMwLT', 'byVLooseIsolationMVA3newDMwoLT', 'byVLooseIsolationMVA3oldDMwLT', 'byVLooseIsolationMVA3oldDMwoLT', 'byVTightIsolationMVA3newDMwLT', 'byVTightIsolationMVA3newDMwoLT', 'byVTightIsolationMVA3oldDMwLT', 'byVTightIsolationMVA3oldDMwoLT', 'byVVTightIsolationMVA3newDMwLT', 'byVVTightIsolationMVA3newDMwoLT', 'byVVTightIsolationMVA3oldDMwLT', 'byVVTightIsolationMVA3oldDMwoLT', 'chargedIsoPtSum', 'decayModeFinding', 'decayModeFindingNewDMs', 'neutralIsoPtSum', 'puCorrPtSum']

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

# simple particle

def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )

def bookGenParticle(tree, pName):
    bookParticle(tree, pName)
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_pdgId'.format(pName=pName))
    
def fillGenParticle( tree, pName, particle ):
    fillParticle( tree, pName, particle )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_pdgId'.format(pName=pName), particle.pdgId() )

# di-tau

def bookDiLepton(tree):
    var( tree, 'visMass')
    var( tree, 'svfitMass')
    var( tree, 'pZetaMET')
    var( tree, 'pZetaVis')
    var( tree, 'pZetaDisc')
    var( tree, 'mt')
    var( tree, 'mtleg1')
    var( tree, 'met')
    var( tree, 'metphi')
    var( tree, 'pthiggs')
    var( tree, 'deltaPhiL1L2')
    var( tree, 'deltaEtaL1L2')
    var( tree, 'deltaRL1L2')
    var( tree, 'deltaPhiL1MET')
    var( tree, 'deltaPhiL2MET')


def fillDiLepton(tree, diLepton):
    fill(tree, 'visMass', diLepton.mass())
    fill(tree, 'svfitMass', diLepton.svfitMass())
    fill(tree, 'pZetaMET', diLepton.pZetaMET())
    fill(tree, 'pZetaVis', diLepton.pZetaVis())
    fill(tree, 'pZetaDisc', diLepton.pZetaDisc())
    fill(tree, 'mt', diLepton.mTLeg2())
    fill(tree, 'mtleg1', diLepton.mTLeg1())
    fill(tree, 'met', diLepton.met().pt())
    fill(tree, 'metphi', diLepton.met().phi())

    pthiggs = (diLepton.leg1().p4()+diLepton.leg2().p4()+diLepton.met().p4()).pt()
    fill(tree, 'pthiggs', pthiggs)
    
    l1eta = diLepton.leg1().eta()
    l2eta = diLepton.leg2().eta()
    l1phi = diLepton.leg1().phi()
    l2phi = diLepton.leg2().phi()
    metphi = diLepton.met().phi()
    fill(tree, 'deltaPhiL1L2', deltaPhi(l1phi, l2phi))
    fill(tree, 'deltaEtaL1L2', abs(l1eta-l2eta))
    fill(tree, 'deltaRL1L2', deltaR(l1eta, l1phi, l2eta, l2phi))
    fill(tree, 'deltaPhiL1MET', deltaPhi(l1phi, metphi))
    fill(tree, 'deltaPhiL2MET', deltaPhi(l2phi, metphi))
    
# lepton

def bookLepton( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_relIso05'.format(pName=pName))
    var(tree, '{pName}_dxy'.format(pName=pName))
    var(tree, '{pName}_dz'.format(pName=pName))
    var(tree, '{pName}_weight'.format(pName=pName))
    var(tree, '{pName}_triggerWeight'.format(pName=pName))
    var(tree, '{pName}_triggerEffData'.format(pName=pName))
    var(tree, '{pName}_triggerEffMC'.format(pName=pName))
    var(tree, '{pName}_recEffWeight'.format(pName=pName))

def fillLepton( tree, pName, lepton ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_relIso05'.format(pName=pName), lepton.relIsoAllChargedDB05() )
    fill(tree, '{pName}_dxy'.format(pName=pName), lepton.dxy() )
    fill(tree, '{pName}_dz'.format(pName=pName), lepton.dz() )
    fill(tree, '{pName}_weight'.format(pName=pName), lepton.weight )
    fill(tree, '{pName}_triggerWeight'.format(pName=pName), lepton.triggerWeight )
    fill(tree, '{pName}_triggerEffData'.format(pName=pName), lepton.triggerEffData )
    fill(tree, '{pName}_triggerEffMC'.format(pName=pName), lepton.triggerEffMC )
    fill(tree, '{pName}_recEffWeight'.format(pName=pName), lepton.recEffWeight )


# muon


def bookMuon( tree, pName ):
    bookLepton(tree, pName )
    # JAN FIXME - do we need the MVA iso and does it exist?
    # var(tree, '{pName}_mvaIso'.format(pName=pName))
    var(tree, '{pName}_looseId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))

def fillMuon( tree, pName, muon ):
    fillLepton(tree, pName, muon)
    # JAN FIXME - do we need the MVA iso and does it exist?
    # fill(tree, '{pName}_mvaIso'.format(pName=pName), muon.mvaIso() )
    fill(tree, '{pName}_looseId'.format(pName=pName), muon.looseId() )
    fill(tree, '{pName}_tightId'.format(pName=pName), muon.tightId() )


# electron


def bookEle( tree, pName ):
    bookLepton(tree, pName )
    # var(tree, '{pName}_mvaIso'.format(pName=pName))
    var(tree, '{pName}_mvaTrigV0'.format(pName=pName))
    var(tree, '{pName}_mvaNonTrigV0'.format(pName=pName))
    var(tree, '{pName}_looseId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))
    var(tree, '{pName}_numberOfMissingHits'.format(pName=pName))
    var(tree, '{pName}_passConversionVeto'.format(pName=pName))
    

def fillEle( tree, pName, ele ):
    fillLepton(tree, pName, ele)
    # JAN FIXME - do we need the MVA iso and does it exist?
    # fill(tree, '{pName}_mvaIso'.format(pName=pName), ele.mvaIso() )
    # fill(tree, '{pName}_mvaTrigV0'.format(pName=pName), ele.electronID("mvaTrigV0") )
    # fill(tree, '{pName}_mvaNonTrigV0'.format(pName=pName), ele.electronID("mvaNonTrigV0") )
    fill(tree, '{pName}_mvaTrigV0'.format(pName=pName), ele.mvaTrigV0() )
    fill(tree, '{pName}_mvaNonTrigV0'.format(pName=pName), ele.mvaNonTrigV0() )
    fill(tree, '{pName}_looseId'.format(pName=pName), ele.looseIdForEleTau() )
    fill(tree, '{pName}_tightId'.format(pName=pName), ele.tightIdForEleTau() )
    fill(tree, '{pName}_numberOfMissingHits'.format(pName=pName), ele.lostInner() )
    fill(tree, '{pName}_passConversionVeto'.format(pName=pName), ele.passConversionVeto() )

# tau 

def bookTau( tree, pName ):
    bookLepton(tree, pName )
    for tauID in tauIDs:
        var(tree, '{pName}_{tauID}'.format(pName=pName, tauID=tauID))
   
    var(tree, '{pName}_EOverp'.format(pName=pName))
    var(tree, '{pName}_decayMode'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_zImpact'.format(pName=pName))

def fillTau( tree, pName, tau ):
    fillLepton(tree, pName, tau)
    for tauID in tauIDs:
        fill(tree, '{pName}_{tauID}'.format(pName=pName, tauID=tauID),
            tau.tauID(tauID))
    # fill(tree, '{pName}_EOverp'.format(pName=pName),
    #      tau.calcEOverP())
    fill(tree, '{pName}_decayMode'.format(pName=pName),
         tau.decayMode())
    fill(tree, '{pName}_mass'.format(pName=pName),
         tau.mass())
    fill(tree, '{pName}_zImpact'.format(pName=pName),
         tau.zImpact())

# jet

def bookJet( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_puMva'.format(pName=pName))
    var(tree, '{pName}_looseJetId'.format(pName=pName))
    var(tree, '{pName}_btagMVA'.format(pName=pName))
    var(tree, '{pName}_area'.format(pName=pName))
    var(tree, '{pName}_genJetPt'.format(pName=pName))
    var(tree, '{pName}_partonFlavour'.format(pName=pName))

def fillJet( tree, pName, jet ):
    fillParticle(tree, pName, jet )
    # JAN - only one PU mva working point, but we may want to add more
    # run in our skimming step
    # (for which Jet.py would have to be touched again)
    fill(tree, '{pName}_puMva'.format(pName=pName), jet.puMva('pileupJetIdFull:full53xDiscriminant') )
    fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.looseJetId())
    fill(tree, '{pName}_btagMVA'.format(pName=pName), jet.btagMVA)
    fill(tree, '{pName}_area'.format(pName=pName), jet.jetArea())
    if hasattr(jet, 'matchedGenJet') and jet.matchedGenJet:
        fill(tree, '{pName}_genJetPt'.format(pName=pName), jet.matchedGenJet.pt())
    fill(tree, '{pName}_partonFlavour'.format(pName=pName), jet.partonFlavour())

# vbf

def bookVBF( tree, pName ):
    var(tree, '{pName}_mjj'.format(pName=pName))
    var(tree, '{pName}_deta'.format(pName=pName))
    var(tree, '{pName}_nCentral'.format(pName=pName))
    var(tree, '{pName}_mva'.format(pName=pName))
    var(tree, '{pName}_jdphi'.format(pName=pName))
    var(tree, '{pName}_dijetpt'.format(pName=pName))
    var(tree, '{pName}_dijetphi'.format(pName=pName))
    var(tree, '{pName}_hdijetphi'.format(pName=pName))
    var(tree, '{pName}_visjeteta'.format(pName=pName))
    var(tree, '{pName}_ptvis'.format(pName=pName))
    
def fillVBF( tree, pName, vbf ):
    fill(tree, '{pName}_mjj'.format(pName=pName), vbf.mjj )
    fill(tree, '{pName}_deta'.format(pName=pName), vbf.deta )
    fill(tree, '{pName}_nCentral'.format(pName=pName), len(vbf.centralJets) )
    fill(tree, '{pName}_mva'.format(pName=pName), vbf.mva )
    fill(tree, '{pName}_jdphi'.format(pName=pName), vbf.dphi )
    fill(tree, '{pName}_dijetpt'.format(pName=pName), vbf.dijetpt )
    fill(tree, '{pName}_dijetphi'.format(pName=pName), vbf.dijetphi )
    fill(tree, '{pName}_hdijetphi'.format(pName=pName), vbf.dphidijethiggs )
    fill(tree, '{pName}_visjeteta'.format(pName=pName), vbf.visjeteta )
    fill(tree, '{pName}_ptvis'.format(pName=pName), vbf.ptvis )
    
