from PhysicsTools.Heppy.analyzers.core.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

from CMGTools.H2TauTau.proto.analyzers.tauIDs import tauIDs
from CMGTools.H2TauTau.proto.analyzers.varsDictionary import vars

from math import sqrt


class Variable():
    def __init__(self, name, function=None, type=float):
        self.name = name
        self.function = function
        if function is None:
            # Note: works for attributes, not member functions
            self.function = lambda x : getattr(x, self.name) 
        self.type = float

# di-tau object variables

ditau_vars = [
    Variable('mvis', lambda dil : dil.mass()),
    Variable('svfit_mass', lambda dil : dil.svfitMass()),
    Variable('svfit_mass_error', lambda dil : dil.svfitMassError() if hasattr(dil, 'svfitMassError') else -999.),
    Variable('svfit_pt', lambda dil : dil.svfitPt() if hasattr(dil, 'svfitPt') else -999.),
    Variable('svfit_pt_error', lambda dil : dil.svfitPtError() if hasattr(dil, 'svfitPtError') else -999.),
    Variable('svfit_eta', lambda dil : dil.svfitEta() if hasattr(dil, 'svfitEta') else -999.),
    Variable('svfit_phi', lambda dil : dil.svfitPhi() if hasattr(dil, 'svfitPhi') else -999.),
    Variable('svfit_met_pt', lambda dil : dil.svfitMET().Rho() if hasattr(dil, 'svfitMET') else -999.),
    Variable('svfit_met_e', lambda dil : dil.svfitMET().mag2() if hasattr(dil, 'svfitMET') else -999.),
    Variable('svfit_met_phi', lambda dil : dil.svfitMET().phi() if hasattr(dil, 'svfitMET') else -999.),
    Variable('svfit_met_eta', lambda dil : dil.svfitMET().eta() if hasattr(dil, 'svfitMET') else -999.),
    Variable('pzeta_met', lambda dil : dil.pZetaMET()),
    Variable('pzeta_vis', lambda dil : dil.pZetaVis()),
    Variable('pzeta_disc', lambda dil : dil.pZetaDisc()),
    Variable('mt', lambda dil : dil.mTLeg2()),
    Variable('mtleg2', lambda dil : dil.mTLeg2()),
    Variable('mtleg1', lambda dil : dil.mTLeg1()),
    Variable('met_cov00', lambda dil : dil.mvaMetSig(0, 0)),
    Variable('met_cov01', lambda dil : dil.mvaMetSig(0, 1)),
    Variable('met_cov10', lambda dil : dil.mvaMetSig(1, 0)),
    Variable('met_cov11', lambda dil : dil.mvaMetSig(1, 1)),
    Variable('met_phi', lambda dil : dil.met().phi()),
    Variable('met_px', lambda dil : dil.met().px()),
    Variable('met_py', lambda dil : dil.met().py()),
    Variable('met_pt', lambda dil : dil.met().pt()),
    Variable('pthiggs', lambda dil : (dil.leg1().p4() + dil.leg2().p4() + dil.met().p4()).pt()),
    Variable('deltaPhiL1L2', lambda dil : deltaPhi(dil.leg1().phi(), dil.leg2().phi())),
    Variable('deltaEtaL1L2', lambda dil : abs(dil.leg1().eta() - dil.leg2().eta())),
    Variable('deltaRL1L2', lambda dil : deltaR(dil.leg1().eta(), dil.leg1().phi(), dil.leg2().eta(), dil.leg2().phi())),
    Variable('deltaPhiL1MET', lambda dil : deltaPhi(dil.leg1().phi(), dil.met().phi())),
    Variable('deltaPhiL2MET', lambda dil : deltaPhi(dil.leg2().phi(), dil.met().phi())),
]

# generic particle
particle_vars = [
    Variable('pt', lambda p: p.pt()),
    Variable('eta', lambda p: p.eta()),
    Variable('phi', lambda p: p.phi()),
    Variable('charge', lambda p: p.charge()), # charge may be non-integer for gen particles
    Variable('mass', lambda p: p.mass()),
]


# electron
electron_vars = [
    Variable('eid_nontrigmva_loose', lambda ele: ele.mvaIDRun2("NonTrigPhys14", "Loose")),
    Variable('eid_nontrigmva_tight', lambda ele: ele.mvaIDRun2("NonTrigPhys14", "Tight")),
    Variable('eid_veto', lambda ele: ele.cutBasedId('POG_PHYS14_25ns_v1_Veto')),
    Variable('eid_loose', lambda ele: ele.cutBasedId('POG_PHYS14_25ns_v1_Loose')),
    Variable('eid_medium', lambda ele: ele.cutBasedId('POG_PHYS14_25ns_v1_Medium')),
    Variable('eid_tight', lambda ele: ele.cutBasedId('POG_PHYS14_25ns_v1_Tight')),
    Variable('nhits_missing', lambda ele: ele.physObj.gsfTrack().hitPattern().numberOfHits(1), int),
    Variable('pass_conv_veto', lambda ele: ele.passConversionVeto()),
]

jet_vars = [
    Variable('mva_pu', lambda jet : jet.puMva('pileupJetIdFull:full53xDiscriminant')),
    Variable('id_loose', lambda jet : jet.looseJetId()),
    Variable('id_pu', lambda jet : jet.puJetId()),
    Variable('mva_btag', lambda jet : jet.btagMVA),
    Variable('area', lambda jet : jet.jetArea()),
    Variable('flavour_parton', lambda jet : jet.partonFlavour()),
    Variable('csv', lambda jet : jet.btag('combinedSecondaryVertexBJetTags')),
    Variable('Bmatch', lambda jet : jet.matchGenParton),
    Variable('rawFactor', lambda jet : jet.rawFactor()),

        if hasattr(jet, 'matchedGenJet') and jet.matchedGenJet:
            self.fill(tree, '{p_name}_genJetPt'.format(p_name=p_name), jet.matchedGenJet.pt())
]

# gen info
geninfo_vars = [
    # Variable('geninfo_nup', type=int),
    Variable('geninfo_tt', type=int),
    Variable('geninfo_mt', type=int),
    Variable('geninfo_et', type=int),
    Variable('geninfo_ee', type=int),
    Variable('geninfo_mm', type=int),
    Variable('geninfo_em', type=int),
    Variable('geninfo_EE', type=int),
    Variable('geninfo_MM', type=int),
    Variable('geninfo_TT', type=int),
    Variable('geninfo_LL', type=int),
    Variable('geninfo_fakeid', type=int),
    Variable('geninfo_has_w', type=int),
    Variable('geninfo_has_z', type=int),
    Variable('geninfo_mass'),
    Variable('genmet_pt'),
    Variable('genmet_eta'),
    Variable('genmet_e'),
    Variable('genmet_px'),
    Variable('genmet_py'),
    Variable('genmet_phi'),
]

vbf_vars = [
    Variable('mjj'),
    Variable('deta'),
    Variable('n_central', lambda vbf : len(vbf.centralJets), int),
    Variable('jdphi', lambda vbf : vbf.dphi),
    Variable('dijetpt'),
    Variable('dijetphi'),
    Variable('dphidijethiggs'),
    Variable('mindetajetvis', lambda vbf : vbf.visjeteta),
]



class H2TauTauTreeProducer(TreeAnalyzerNumpy):

    '''
       Base H->tautau tree producer.
       Books and fills the branches that are common to
       all four channels (mt, et, tt, em).
       The following branches are booked and filled:
       _ event ID
       _ di-tau pair variables (including MET)
       _ raw pf MET
       _ pass_leptons (both legs pass tight ID and isolation)
       _ third lepton veto
       _ dilepton veto
       _ VBF variables
       _ generator information variables (including NUP)
       _ jet1 and jet2 variables (sorted by pt)
       _ bjet1 and bjet2 variables (sorted by pt)
       _ event weight
       _ vertex weight
       _ embed weight
       _ gen parent boson H, W, Z (if exists)
       _ weight_njet
       _ event rho
       _ HqT weights
       _ hard scattering quarks and gluons (up to 4, can be set by self.maxNGenJets)
       Signal lepton-specific variables need to be booked
       and filled in the channel-specific child producers.

       The branch names can be changed by means of a dictionary.
    '''

    def __init__(self, *args):
        super(H2TauTauTreeProducer, self).__init__(*args)
        self.varStyle = 'std'
        self.varDict = vars
        self.skimFunction = 'True'
        if hasattr(self.cfg_ana, 'varStyle'):
            self.varStyle = self.cfg_ana.varStyle
        if hasattr(self.cfg_ana, 'varDict'):
            self.varDict = self.cfg_ana.varDict
        if hasattr(self.cfg_ana, 'skimFunction'):
            self.skimFunction = self.cfg_ana.skimFunction

    def var(self, tree, varName, type=float):
        tree.var(self.varName(varName), type)

    def fill(self, tree, varName, value):
        tree.fill(self.varName(varName), value)

    def declareHandles(self):
        super(H2TauTauTreeProducer, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
            'slimmedMETs',
            'std::vector<pat::MET>'
        )

    def varName(self, name):
        try:
            return self.varDict[name][self.varStyle]
        except:
            if self.verbose:
                print 'WARNING: self.varDict[{NAME}][{VARSTYLE}] does not exist'.format(NAME=name, VARSTYLE=self.varStyle)
                print '         using {NAME}'.format(NAME=name)
            return name

    def fillTree(self, event):
        if eval(self.skimFunction):
            self.tree.tree.Fill()

    def bookGeneric(self, tree, var_list, obj_name=None):
        for var in var_list:
            names = [obj_name, var.name] if obj_name else [var.name]
            self.var(tree, '_'.join(names), var.type)

    def fillGeneric(self, tree, var_list, obj, obj_name=None):
        for var in var_list:
            names = [obj_name, var.name] if obj_name else [var.name]
            self.fill(tree, '_'.join(names), var.function(obj))


    def declareVariables(self, setup):

        self.bookEvtID(self.tree)
        self.bookDiLepton(self.tree)
        self.bookGenInfo(self.tree)
        self.bookVBF(self.tree, 'vbf')

        self.var(self.tree, 'pass_leptons')
        self.var(self.tree, 'veto_dilepton')
        self.var(self.tree, 'veto_thirdlepton')

        self.var(self.tree, 'n_jets', int)  # pt>30 GeV
        self.var(self.tree, 'n_jets_20', int)  # pt>20 GeV
        self.bookJet(self.tree, 'jet1')
        self.bookJet(self.tree, 'jet2')

        self.var(self.tree, 'n_jets_csvl', int)
        self.var(self.tree, 'n_bjets', int)
        self.bookJet(self.tree, 'bjet1')
        self.bookJet(self.tree, 'bjet2')

        self.var(self.tree, 'n_vertices', int)
        self.var(self.tree, 'rho')
        self.var(self.tree, 'weight')
        self.var(self.tree, 'weight_vertex')
        self.var(self.tree, 'weight_embed')
        self.var(self.tree, 'weight_njet')

        self.var(self.tree, 'weight_hqt')
        self.var(self.tree, 'weight_hqt_up')
        self.var(self.tree, 'weight_hqt_down')

        self.bookParticle(self.tree, 'pfmet')

        self.bookGenParticle(self.tree, 'genboson')

        self.maxNGenJets = 4

    def process(self, event):

        # needed when doing handle.product(), goes back to
        # PhysicsTools.Heppy.analyzers.core.Analyzer
        self.readCollections(event.input)

        self.tree.reset()

        if not eval(self.skimFunction):
            return False

        self.fillEvtID(self.tree, event)
        self.fillDiLepton(self.tree, event.diLepton)
        self.fillGenInfo(self.tree, event)
        if hasattr(event, 'vbf'):
            self.fillVBF(self.tree, 'vbf', event.vbf)

        self.fill(self.tree, 'pass_leptons', event.isSignal)
        self.fill(self.tree, 'veto_dilepton', event.leptonAccept)
        self.fill(self.tree, 'veto_thirdlepton', event.thirdLeptonVeto)

        nJets = len(event.cleanJets30)
        nJets20 = len(event.cleanJets)
        self.fill(self.tree, 'n_jets', nJets)
        self.fill(self.tree, 'n_jets_20', nJets20)
        for i, jet in enumerate(event.cleanJets[:2]):
            self.fillJet(self.tree, 'jet{NUM}'.format(NUM=str(i + 1)), jet)

        n_bjets = len(event.cleanBJets)
        self.fill(self.tree, 'n_bjets', n_bjets)
        for i, jet in enumerate(event.cleanBJets[:2]):
            self.fillJet(self.tree, 'bjet{NUM}'.format(NUM=str(i + 1)), jet)
        # JAN - directly count CSVL jets as done in
        # other groups. May apply SFs as for CSVM jets
        # after rewriting BTagSF module in the future
        n_jets_csvl = sum(1 for jet in event.cleanJets if jet.btag('combinedSecondaryVertexBJetTags') > 0.244)
        self.fill(self.tree, 'n_jets_csvl', n_jets_csvl)

        self.fill(self.tree, 'n_vertices', len(event.vertices))
        self.fill(self.tree, 'rho', event.rho)
        self.fill(self.tree, 'weight', event.eventWeight)
        self.fill(self.tree, 'weight_vertex', event.vertexWeight)
        self.fill(self.tree, 'weight_embed', event.embedWeight)
        self.fill(self.tree, 'weight_njet', event.NJetWeight)

        self.fill(self.tree, 'weight_hqt', event.higgsPtWeight)
        self.fill(self.tree, 'weight_hqt_up', event.higgsPtWeightUp)
        self.fill(self.tree, 'weight_hqt_down', event.higgsPtWeightDown)

        if hasattr(event, 'parentBoson'):
            self.fillGenParticle(self.tree, 'genboson', event.parentBoson)

        pfmet = self.handles['pfmetraw'].product()[0]
        self.fillParticle(self.tree, 'pfmet', pfmet)

        if type(self) is H2TauTauTreeProducer:
            self.fillTree(event)

    # event ID
    def bookEvtID(self, tree):
        self.var(tree, 'run', int)
        self.var(tree, 'lumi', int)
        self.var(tree, 'event', int)

    def fillEvtID(self, tree, event):
        self.fill(tree, 'run', event.run)
        self.fill(tree, 'lumi', event.lumi)
        self.fill(tree, 'event', event.eventId)

    # simple particle
    def bookParticle(self, tree, p_name):
        for var in particle_vars:
            self.var(tree, '{p_name}_{v_name}'.format(p_name=p_name, v_name=var.name))

    def fillParticle(self, tree, p_name, particle):
        for var in particle_vars:
            self.fill(tree, '{p_name}_{v_name}'.format(p_name=p_name, v_name=var.name), var.function(particle))

    # simple gen particle
    def bookGenParticle(self, tree, p_name):
        self.bookParticle(tree, p_name)
        self.var(tree, '{p_name}_pdgId'.format(p_name=p_name))

    def fillGenParticle(self, tree, p_name, particle):
        self.fillParticle(tree, p_name, particle)
        self.fill(tree, '{p_name}_pdgId'.format(p_name=p_name), particle.pdgId())

    # di-tau
    def bookDiLepton(self, tree):
        # RIC: to add
        # svfit 'fittedDiTauSystem', 'fittedMET', 'fittedTauLeptons'
        self.bookGeneric(tree, ditau_vars)
        self.bookParticle(tree, 'svfit_l1')
        self.bookParticle(tree, 'svfit_l2')

    def fillDiLepton(self, tree, diLepton):
        self.fillGeneric(tree, ditau_vars, diLepton)
        if hasattr(diLepton, 'svfit_Taus'):
            for i, tau in enumerate(diLepton.svfitTaus()):
                self.fillParticle(tree, 'svfit_l' + str(i + 1), tau)

    # lepton
    def bookLepton(self, tree, p_name):
        self.bookParticle(tree, p_name)
        self.bookParticle(tree, p_name + '_jet')
        self.var(tree, '{p_name}_reliso05'      .format(p_name=p_name))
        self.var(tree, '{p_name}_dxy'           .format(p_name=p_name))
        self.var(tree, '{p_name}_dz'            .format(p_name=p_name))
        self.var(tree, '{p_name}_weight'        .format(p_name=p_name))
        self.var(tree, '{p_name}_weight_trigger' .format(p_name=p_name))
        self.var(tree, '{p_name}_eff_trigger_data'.format(p_name=p_name))
        self.var(tree, '{p_name}_eff_trigger_mc'  .format(p_name=p_name))
        self.var(tree, '{p_name}_weight_rec_eff'  .format(p_name=p_name))

    def fillLepton(self, tree, p_name, lepton):
        self.fillParticle(tree, p_name, lepton)
        self.fillParticle(tree, p_name + '_jet', lepton.jet)
        self.fill(tree, '{p_name}_reliso05'      .format(
            p_name=p_name), lepton.relIso(dBetaFactor=0.5, allCharged=0))
        try:
            self.fill(tree, '{p_name}_dxy'.format(p_name=p_name), lepton.dxy())
            self.fill(tree, '{p_name}_dz'.format(p_name=p_name), lepton.dz())
        except:
            self.fill(tree, '{p_name}_dxy'.format(p_name=p_name), 999.)
            self.fill(tree, '{p_name}_dz'.format(p_name=p_name), 999.)

        self.fill(
            tree, '{p_name}_weight'        .format(p_name=p_name), lepton.weight)
        self.fill(tree, '{p_name}_weight_trigger' .format(
            p_name=p_name), lepton.triggerWeight)
        self.fill(tree, '{p_name}_eff_trigger_data'.format(
            p_name=p_name), lepton.triggerEffData)
        self.fill(tree, '{p_name}_eff_trigger_mc'  .format(
            p_name=p_name), lepton.triggerEffMC)
        self.fill(tree, '{p_name}_weight_rec_eff'  .format(
            p_name=p_name), lepton.recEffWeight)

    # muon
    def bookMuon(self, tree, p_name):
        self.bookLepton(tree, p_name)
        # JAN FIXME - do we need the MVA iso and does it exist?
        # var(tree, '{p_name}_mvaIso'.format(p_name=p_name))
        self.var(tree, '{p_name}_muonid_loose'     .format(p_name=p_name))
        self.var(tree, '{p_name}_muonid_medium'    .format(p_name=p_name))
        self.var(tree, '{p_name}_muonid_tight'     .format(p_name=p_name))
        self.var(tree, '{p_name}_muonid_tightnovtx'.format(p_name=p_name))
        self.var(tree, '{p_name}_muonid_highpt'    .format(p_name=p_name))

    def fillMuon(self, tree, p_name, muon):
        self.fillLepton(tree, p_name, muon)
        # JAN FIXME - do we need the MVA iso and does it exist?
        # fill(tree, '{p_name}_mvaIso'.format(p_name=p_name), muon.mvaIso() )
        self.fill(tree, '{p_name}_muonid_loose'     .format(
            p_name=p_name), muon.muonID('POG_ID_Loose'))
        self.fill(tree, '{p_name}_muonid_medium'    .format(
            p_name=p_name), muon.muonID('POG_ID_Medium'))
        self.fill(tree, '{p_name}_muonid_tight'     .format(
            p_name=p_name), muon.muonID('POG_ID_Tight'))
        self.fill(tree, '{p_name}_muonid_tightnovtx'.format(
            p_name=p_name), muon.muonID('POG_ID_TightNoVtx'))
        self.fill(tree, '{p_name}_muonid_highpt'    .format(
            p_name=p_name), muon.muonID('POG_ID_HighPt'))


    def bookEle(self, tree, p_name):
        self.bookLepton(tree, p_name)
        for var in electron_vars:
            self.var(tree, '{p_name}_{v_name}'.format(p_name=p_name, v_name=var.name), var.type)

    def fillEle(self, tree, p_name, ele):
        self.fillLepton(tree, p_name, ele)
        for var in electron_vars:
            self.fill(tree, '{p_name}_{v_name}'.format(p_name=p_name, v_name=var.name), var.function(ele))



    # tau
    def bookTau(self, tree, p_name):
        self.bookLepton(tree, p_name)
        for tauID in tauIDs:
            self.var(tree, '{p_name}_{tauID}'.format(p_name=p_name, tauID=tauID))
        self.var(tree, '{p_name}_eoverp'.format(p_name=p_name))
        self.var(tree, '{p_name}_decayMode'.format(p_name=p_name))
        self.var(tree, '{p_name}_zImpact'.format(p_name=p_name))

    def fillTau(self, tree, p_name, tau):
        self.fillLepton(tree, p_name, tau)
        for tauID in tauIDs:
            self.fill(tree, '{p_name}_{tauID}'.format(
                p_name=p_name, tauID=tauID), tau.tauID(tauID))
        #self.fill(tree, '{p_name}_eoverp'   .format(p_name=p_name), tau.calcEOverP())
        self.fill(
            tree, '{p_name}_decayMode'.format(p_name=p_name), tau.decayMode())
        self.fill(tree, '{p_name}_zImpact'  .format(p_name=p_name), tau.zImpact())

    # jet
    def bookJet(self, tree, p_name):
        self.bookParticle(tree, p_name)
        self.var(tree, '{p_name}_puMva'.format(p_name=p_name))
        self.var(tree, '{p_name}_looseJetId'.format(p_name=p_name))
        self.var(tree, '{p_name}_puJetId'.format(p_name=p_name))
        self.var(tree, '{p_name}_btagMVA'.format(p_name=p_name))
        self.var(tree, '{p_name}_area'.format(p_name=p_name))
        self.var(tree, '{p_name}_genJetPt'.format(p_name=p_name))
        self.var(tree, '{p_name}_partonFlavour'.format(p_name=p_name))
        self.var(tree, '{p_name}_csv'.format(p_name=p_name))
        self.var(tree, '{p_name}_Bmatch'.format(p_name=p_name))
        self.var(tree, '{p_name}_rawFactor'.format(p_name=p_name))

    def fillJet(self, tree, p_name, jet):
        self.fillParticle(tree, p_name, jet)
        # JAN - only one PU mva working point, but we may want to add more
        # run in our skimming step
        # (for which Jet.py would have to be touched again)
        self.fill(tree, '{p_name}_puMva'.format(p_name=p_name), jet.puMva('pileupJetIdFull:full53xDiscriminant'))
        self.fill(tree, '{p_name}_looseJetId'.format(p_name=p_name), jet.looseJetId())
        self.fill(tree, '{p_name}_puJetId'.format(p_name=p_name), jet.puJetId())
        self.fill(tree, '{p_name}_btagMVA'.format(p_name=p_name), jet.btagMVA)
        self.fill(tree, '{p_name}_area'.format(p_name=p_name), jet.jetArea())
        self.fill(tree, '{p_name}_partonFlavour'.format(p_name=p_name), jet.partonFlavour())
        self.fill(tree, '{p_name}_csv'.format(p_name=p_name), jet.btag('combinedSecondaryVertexBJetTags'))
        self.fill(tree, '{p_name}_Bmatch'.format(p_name=p_name), jet.matchGenParton)
        self.fill(tree, '{p_name}_rawFactor'.format(p_name=p_name), jet.rawFactor())
        if hasattr(jet, 'matchedGenJet') and jet.matchedGenJet:
            self.fill(tree, '{p_name}_genJetPt'.format(p_name=p_name), jet.matchedGenJet.pt())

    # vbf
    def bookVBF(self, tree, p_name):
        self.bookGeneric(tree, vbf_vars, p_name)

    def fillVBF(self, tree, p_name, vbf):
        self.fillGeneric(tree, vbf_vars, vbf, p_name)

    # generator information
    def bookGenInfo(self, tree):
        self.bookGeneric(tree, geninfo_vars)

    def fillGenInfo(self, tree, event):
        self.fillGeneric(tree, geninfo_vars, event)


    # quark and gluons
    def bookQG(self, tree):
        for i in range(0, self.maxNGenJets):
            self.bookGenParticle(self.tree, 'genqg_{i}'.format(i=i))

    def fillQG(self, tree, event):
        # Fill hard quarks/gluons
        quarksGluons = [p for p in event.genParticles if abs(p.pdgId()) in (1, 2, 3, 4, 5, 21) and
                        p.status() == 3 and
                        (p.numberOfDaughters() == 0 or p.daughter(0).status() != 3)]
        quarksGluons.sort(key=lambda x: -x.pt())
        for i in range(0, min(self.maxNGenJets, len(quarksGluons))):
            self.fillGenParticle(
                tree, 'genqg_{i}'.format(i=i), quarksGluons[i])
