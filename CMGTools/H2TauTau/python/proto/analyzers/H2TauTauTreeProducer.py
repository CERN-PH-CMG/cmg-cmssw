from PhysicsTools.Heppy.analyzers.core.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

from CMGTools.H2TauTau.proto.analyzers.tauIDs import tauIDs
from CMGTools.H2TauTau.proto.analyzers.varsDictionary import vars

class Variable():
    def __init__(self, name, function=None, type=float):
        self.name = name
        self.function = function
        if function is None:
            # Note: works for attributes, not member functions
            self.function = lambda x : getattr(x, self.name) 
        self.type = float

# event variables
event_vars = [
    Variable('run', type=int),
    Variable('lumi', type=int),
    Variable('event', lambda ev : ev.eventId, type=int),
    Variable('pass_leptons', lambda ev : ev.isSignal, type=int),
    Variable('veto_dilepton', lambda ev : ev.leptonAccept, type=int),
    Variable('veto_thirdlepton', lambda ev : ev.thirdLeptonVeto, type=int),
    Variable('n_jets', lambda ev : len(ev.cleanJets30), type=int),
    Variable('n_jets_20', lambda ev : len(ev.cleanJets), type=int),
    Variable('n_bjets', lambda ev : len(ev.cleanBJets), type=int),
    Variable('n_jets_csvl', lambda ev : sum(1 for jet in ev.cleanJets if jet.btagWP('CSVv2IVFL')), type=int),
    Variable('n_vertices', lambda ev : len(ev.vertices), type=int),
    Variable('rho', lambda ev : ev.rho),
    Variable('weight', lambda ev : ev.eventWeight),
    Variable('weight_vertex', lambda ev : ev.vertexWeight),
    Variable('weight_embed', lambda ev : ev.embedWeight),
    Variable('weight_njet', lambda ev : ev.NJetWeight),
    Variable('weight_hqt', lambda ev : ev.higgsPtWeight),
    Variable('weight_hqt_up', lambda ev : ev.higgsPtWeightUp),
    Variable('weight_hqt_down', lambda ev : ev.higgsPtWeightDown),
]

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
    Variable('mt_leg2', lambda dil : dil.mTLeg2()),
    Variable('mt_leg1', lambda dil : dil.mTLeg1()),
    Variable('met_cov00', lambda dil : dil.mvaMetSig(0, 0)),
    Variable('met_cov01', lambda dil : dil.mvaMetSig(0, 1)),
    Variable('met_cov10', lambda dil : dil.mvaMetSig(1, 0)),
    Variable('met_cov11', lambda dil : dil.mvaMetSig(1, 1)),
    Variable('met_phi', lambda dil : dil.met().phi()),
    Variable('met_px', lambda dil : dil.met().px()),
    Variable('met_py', lambda dil : dil.met().py()),
    Variable('met_pt', lambda dil : dil.met().pt()),
    Variable('pthiggs', lambda dil : (dil.leg1().p4() + dil.leg2().p4() + dil.met().p4()).pt()),
    Variable('delta_phi_l1_l2', lambda dil : deltaPhi(dil.leg1().phi(), dil.leg2().phi())),
    Variable('delta_eta_l1_l2', lambda dil : abs(dil.leg1().eta() - dil.leg2().eta())),
    Variable('delta_r_l1_l2', lambda dil : deltaR(dil.leg1().eta(), dil.leg1().phi(), dil.leg2().eta(), dil.leg2().phi())),
    Variable('delta_phi_l1_met', lambda dil : deltaPhi(dil.leg1().phi(), dil.met().phi())),
    Variable('delta_phi_l2_met', lambda dil : deltaPhi(dil.leg2().phi(), dil.met().phi())),
]

# generic particle
particle_vars = [
    Variable('pt', lambda p: p.pt()),
    Variable('eta', lambda p: p.eta()),
    Variable('phi', lambda p: p.phi()),
    Variable('charge', lambda p: p.charge()), # charge may be non-integer for gen particles
    Variable('mass', lambda p: p.mass()),
]

# generic lepton
lepton_vars = [
    Variable('reliso05', lambda lep : lep.relIso(dBetaFactor=0.5, allCharged=0)),
    Variable('dxy', lambda lep : lep.dxy()),
    Variable('dz', lambda lep : lep.dz()),
    Variable('weight'),
    Variable('weight_trigger', lambda lep : lep.triggerWeight),
    Variable('eff_trigger_data', lambda lep : lep.triggerEffData),
    Variable('eff_trigger_mc', lambda lep : lep.triggerEffMC),
    Variable('weight_rec_eff', lambda lep : lep.recEffWeight),
]

# electron
electron_vars = [
    Variable('eid_nontrigmva_loose', lambda ele : ele.mvaIDRun2("NonTrigPhys14", "Loose")),
    Variable('eid_nontrigmva_tight', lambda ele : ele.mvaIDRun2("NonTrigPhys14", "Tight")),
    Variable('eid_veto', lambda ele : ele.cutBasedId('POG_PHYS14_25ns_v1_Veto')),
    Variable('eid_loose', lambda ele : ele.cutBasedId('POG_PHYS14_25ns_v1_Loose')),
    Variable('eid_medium', lambda ele : ele.cutBasedId('POG_PHYS14_25ns_v1_Medium')),
    Variable('eid_tight', lambda ele : ele.cutBasedId('POG_PHYS14_25ns_v1_Tight')),
    Variable('nhits_missing', lambda ele : ele.physObj.gsfTrack().hitPattern().numberOfHits(1), int),
    Variable('pass_conv_veto', lambda ele : ele.passConversionVeto()),
]

# muon
muon_vars = [
    Variable('muonid_loose', lambda muon : muon.muonID('POG_ID_Loose')),
    Variable('muonid_medium', lambda muon : muon.muonID('POG_ID_Medium')),
    Variable('muonid_tight', lambda muon : muon.muonID('POG_ID_Tight')),
    Variable('muonid_tightnovtx', lambda muon : muon.muonID('POG_ID_TightNoVtx')),
    Variable('muonid_highpt', lambda muon : muon.muonID('POG_ID_HighPt')),
]

# tau
tau_vars = [
    Variable('decayMode', lambda tau : tau.decayMode()),
    Variable('zImpact', lambda tau : tau.zImpact())
]
for tau_id in tauIDs:
    if type(tau_id) is str:
        # Need to use eval since functions are otherwise bound to local
        # variables
        tau_vars.append(Variable(tau_id, eval('lambda tau : tau.tauID("{id}")'.format(id=tau_id))))
    else:
        sum_id_str = ' + '.join('tau.tauID("{id}")'.format(id=tau_id[0].format(wp=wp)) for wp in tau_id[1])
        tau_vars.append(Variable(tau_id[0].format(wp=''), 
            eval('lambda tau : ' + sum_id_str), int))


# jet
jet_vars = [
    # JAN - only one PU mva working point, but we may want to add more
    # run in our skimming step
    # (for which Jet.py would have to be touched again)
    Variable('mva_pu', lambda jet : jet.puMva('pileupJetIdFull:full53xDiscriminant')),
    Variable('id_loose', lambda jet : jet.looseJetId()),
    Variable('id_pu', lambda jet : jet.puJetId()),
    Variable('mva_btag', lambda jet : jet.btagMVA),
    Variable('area', lambda jet : jet.jetArea()),
    Variable('flavour_parton', lambda jet : jet.partonFlavour()),
    Variable('csv', lambda jet : jet.btag('combinedInclusiveSecondaryVertexV2BJetTags')),
    Variable('rawfactor', lambda jet : jet.rawFactor()),
    Variable('genjet_pt', lambda jet : jet.matchedGenJet.pt() if hasattr(jet, 'matchedGenJet') and jet.matchedGenJet else -999.),
]

# gen info
geninfo_vars = [
    Variable('geninfo_nup', lambda ev : ev.NUP if hasattr(ev, 'NUP') else -1, type=int),
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

        self.bookEvent(self.tree)
        self.bookDiLepton(self.tree)
        self.bookGenInfo(self.tree)
        self.bookVBF(self.tree, 'vbf')

        self.bookJet(self.tree, 'jet1')
        self.bookJet(self.tree, 'jet2')

        self.bookJet(self.tree, 'bjet1')
        self.bookJet(self.tree, 'bjet2')

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

        self.fillEvent(self.tree, event)
        self.fillDiLepton(self.tree, event.diLepton)
        self.fillGenInfo(self.tree, event)
        if hasattr(event, 'vbf'):
            self.fillVBF(self.tree, 'vbf', event.vbf)

        for i, jet in enumerate(event.cleanJets[:2]):
            self.fillJet(self.tree, 'jet{n}'.format(n=str(i + 1)), jet)

        for i, jet in enumerate(event.cleanBJets[:2]):
            self.fillJet(self.tree, 'bjet{n}'.format(n=str(i + 1)), jet)

        if hasattr(event, 'parentBoson'):
            self.fillGenParticle(self.tree, 'genboson', event.parentBoson)

        pfmet = self.handles['pfmetraw'].product()[0]
        self.fillParticle(self.tree, 'pfmet', pfmet)

        if type(self) is H2TauTauTreeProducer:
            self.fillTree(event)

    # event
    def bookEvent(self, tree):
        self.bookGeneric(tree, event_vars)

    def fillEvent(self, tree, event):
        self.fillGeneric(tree, event_vars, event)

    # simple particle
    def bookParticle(self, tree, p_name):
        self.bookGeneric(tree, particle_vars, p_name)

    def fillParticle(self, tree, p_name, particle):
        self.fillGeneric(tree, particle_vars, particle, p_name)

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
        self.bookGeneric(tree, lepton_vars, p_name)

    def fillLepton(self, tree, p_name, lepton):
        self.fillParticle(tree, p_name, lepton)
        self.fillParticle(tree, p_name + '_jet', lepton.jet)
        self.fillGeneric(tree, lepton_vars, lepton, p_name)

    # muon
    def bookMuon(self, tree, p_name):
        self.bookLepton(tree, p_name)
        self.bookGeneric(tree, muon_vars, p_name)

    def fillMuon(self, tree, p_name, muon):
        self.fillLepton(tree, p_name, muon)
        self.fillGeneric(tree, muon_vars, muon, p_name)

    # ele
    def bookEle(self, tree, p_name):
        self.bookLepton(tree, p_name)
        self.bookGeneric(tree, electron_vars, p_name)

    def fillEle(self, tree, p_name, ele):
        self.fillLepton(tree, p_name, ele)
        self.fillGeneric(tree, electron_vars, ele, p_name)

    # tau
    def bookTau(self, tree, p_name):
        self.bookLepton(tree, p_name)
        self.bookGeneric(tree, tau_vars, p_name)

    def fillTau(self, tree, p_name, tau):
        self.fillLepton(tree, p_name, tau)
        self.fillGeneric(tree, tau_vars, tau, p_name)

    # jet
    def bookJet(self, tree, p_name):
        self.bookParticle(tree, p_name)
        self.bookGeneric(tree, jet_vars, p_name)
        
    def fillJet(self, tree, p_name, jet):
        self.fillParticle(tree, p_name, jet)
        self.fillGeneric(tree, jet_vars, jet, p_name)

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
