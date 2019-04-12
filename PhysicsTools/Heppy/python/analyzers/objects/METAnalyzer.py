from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Jet
from PhysicsTools.HeppyCore.utils.deltar import * 
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.physicsutils.JetReCalibrator import Type1METCorrector, setFakeRawMETOnOldMiniAODs
import PhysicsTools.HeppyCore.framework.config as cfg

import copy
import ROOT
from math import hypot

from copy import deepcopy

class METAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(METAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.recalibrateMET   = cfg_ana.recalibrate
        self.applyJetSmearing = cfg_comp.isMC and cfg_ana.applyJetSmearing
        self.old74XMiniAODs         = cfg_ana.old74XMiniAODs
        self.jetAnalyzerPostFix = getattr(cfg_ana, 'jetAnalyzerPostFix', '')
        if self.recalibrateMET in [ "type1", True ]:
            if self.recalibrateMET == "type1":
                self.type1METCorrector = Type1METCorrector(self.old74XMiniAODs)
        elif self.recalibrateMET != False:
            raise RuntimeError("Unsupported value %r for option 'recalibrate': allowed are True, False, 'type1'" % self.recalibrateMET)

    def declareHandles(self):
        super(METAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( self.cfg_ana.metCollection, 'std::vector<pat::MET>' )

        if self.cfg_ana.storePuppiExtra:
            self.handles['corX'] = AutoHandle( 'puppiMETEGCor:corX', 'float' )
            self.handles['corY'] = AutoHandle( 'puppiMETEGCor:corY', 'float' )
        if self.cfg_ana.doMetNoPU: 
            self.handles['nopumet'] = AutoHandle( self.cfg_ana.noPUMetCollection, 'std::vector<pat::MET>' )
        if self.cfg_ana.doTkMet or self.cfg_ana.doPuppiMet:
            self.handles['cmgCand'] = AutoHandle( self.cfg_ana.candidates, self.cfg_ana.candidatesTypes )
            #self.handles['vertices'] =  AutoHandle( "offlineSlimmedPrimaryVertices", 'std::vector<reco::Vertex>', fallbackLabel="offlinePrimaryVertices" )
            self.mchandles['packedGen'] = AutoHandle( 'packedGenParticles', 'std::vector<pat::PackedGenParticle>' )

    def beginLoop(self, setup):
        super(METAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    def applyDeltaMet(self, met, deltaMet):
        px,py = self.met.px()+deltaMet[0], self.met.py()+deltaMet[1]
        met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))

    def adduParaPerp(self, met, boson, postfix):

        upara = 0
        uperp = 0
        uX = - met.px() - boson.px()
        uY = - met.py() - boson.py()
        u1 = (uX*boson.px() + uY*boson.py())/boson.pt()
        u2 = (uX*boson.py() - uY*boson.px())/boson.pt()

        setattr(met, "upara"+postfix, u1)
        setattr(met, "uperp"+postfix, u2)


    def makePuppiMETs(self, event):
        chargedPuppi = []
        photonsPuppi = []
        neutralHadronsPuppi = []
        hfPuppi = []

        pfcands = self.handles['cmgCand'].product()

        for pfcand in pfcands:

            if (pfcand.charge()!=0):
                pxyw = pfcand.px(), pfcand.py(), pfcand.puppiWeightNoLep()
                chargedPuppi.append(pxyw)

            if (pfcand.pdgId()==22):
                pxyw = pfcand.px(), pfcand.py(), pfcand.puppiWeightNoLep()
                photonsPuppi.append(pxyw)

            if (pfcand.pdgId()==130):
                pxyw = pfcand.px(), pfcand.py(), pfcand.puppiWeightNoLep()
                neutralHadronsPuppi.append(pxyw)

            if (pfcand.pdgId()==1 or pfcand.pdgId()==2):
                pxyw = pfcand.px(), pfcand.py(), pfcand.puppiWeightNoLep()
                hfPuppi.append(pxyw)

        def sumXY(pxyws):
            px, py = sum(x[0]*x[2] for x in pxyws), sum(x[1]*x[2] for x in pxyws)
            return ROOT.reco.Particle.LorentzVector(-px, -py, 0, hypot(px,py))

        setattr(event, "puppiMetCh"+self.cfg_ana.collectionPostFix, sumXY(chargedPuppi))
        setattr(event, "puppiMetPh"+self.cfg_ana.collectionPostFix, sumXY(photonsPuppi))
        setattr(event, "puppiMetNh"+self.cfg_ana.collectionPostFix, sumXY(neutralHadronsPuppi))
        setattr(event, "puppiMetHF"+self.cfg_ana.collectionPostFix, sumXY(hfPuppi))

        getattr(event,"puppiMetCh"+self.cfg_ana.collectionPostFix).sumEt = sum([hypot(x[0]*x[2],x[1]*x[2]) for x in chargedPuppi])
        getattr(event,"puppiMetPh"+self.cfg_ana.collectionPostFix).sumEt = sum([hypot(x[0]*x[2],x[1]*x[2]) for x in photonsPuppi])
        getattr(event,"puppiMetNh"+self.cfg_ana.collectionPostFix).sumEt = sum([hypot(x[0]*x[2],x[1]*x[2]) for x in neutralHadronsPuppi])
        getattr(event,"puppiMetHF"+self.cfg_ana.collectionPostFix).sumEt = sum([hypot(x[0]*x[2],x[1]*x[2]) for x in hfPuppi])



    def makeTkMETs(self, event):
        charged = []
        chargedchs = []
        chargedPVLoose = []
        chargedPUPVLoose = []
        chargedPVTight = []
        chargedNoPV = []
        chargedPVUsedInFit = []
        nt = []
        ntcentral = []
        dochs=getattr(self.cfg_ana,"includeTkMetCHS",False)
        dotight=getattr(self.cfg_ana,"includeTkMetPVTight",False)
        doloose=getattr(self.cfg_ana,"includeTkMetPVLoose",False)
        doPVUsedInFit=getattr(self.cfg_ana,"includeTkMetPVUsedInFit",False)
        doNoPV=getattr(self.cfg_ana,"includeTkMetNoPV",False)
        useLeptonPV=getattr(self.cfg_ana,"useLeptonPV",False)
        doneutrals=getattr(self.cfg_ana,"includeNTMet",True)       

        pfcands = self.handles['cmgCand'].product()
        leadCharged,leadNeutral=None,None
        
        #set primary vertex to the leading pT lepton  
        ipv=0
        if useLeptonPV and len(event.selectedLeptons)>0:
            lpv=event.selectedLeptons[0].associatedVertex
            for ivtx in xrange(0,len(event.vertices)):
                if event.vertices[ivtx].position()!=lpv.position(): continue
                ipv=ivtx
                break

        for pfcand in pfcands:

            p = pfcand.p4()
            isCentral=True if abs(p.eta())<2.4 else False
            
            ## ===> require the Track Candidate charge and with a  minimum dz 
            if (pfcand.charge()==0):                 
                nt.append(p)
                if isCentral : 
                    ntcentral.append(p)
                    leadNeutral=p if not leadNeutral or leadNeutral.pt()<p.pt() else leadNeutral
                continue

            pvflag = pfcand.fromPV(ipv)

            if abs(pfcand.dz())<=self.cfg_ana.dzMax:
                charged.append(p)
                
            if pvflag>0:
                chargedchs.append(p)

            #close to PV and not in the fit of another PV
            if pvflag>1:
                chargedPVLoose.append(p)

                #clean wrt to selected leptons from the same PV 
                veto=False
                for l in event.selectedLeptons:
                    if l.associatedVertex.position()!=pfcand.vertex() : continue
                    if deltaR(l.p4(),p)>0.05 : continue
                    veto=True
                    break
                if not veto:
                    leadCharged=p if not leadCharged or leadCharged.pt()<p.pt() else leadCharged

            #close or used in the fit of another PV
            if pvflag<=1:
                chargedPUPVLoose.append(p)

            if pvflag>2:
                chargedPVTight.append(p)

            if doNoPV and pvflag >= 0:
                chargedNoPV.append(p)
            if doPVUsedInFit and pvflag >= 3:
                chargedPVUsedInFit.append(p)

        def sumP4(p4s):
            p4=ROOT.reco.Particle.LorentzVector(0.,0.,0.,0.)
            for p in p4s: p4 -= p
            return p4
        for coll,p4coll,doit in [('tkMet',            charged,            True),
                                 ('tkMetPVchs',       chargedchs,         dochs),
                                 ('tkMetPVLoose',     chargedPVLoose,     doloose),
                                 ('tkMetPUPVLoose',   chargedPUPVLoose,   doloose),
                                 ('tkMetPVUsedInFit', chargedPVUsedInFit, doPVUsedInFit),
                                 ('tkMetNoPV',        chargedNoPV,        doNoPV),
                                 ('tkMetPVTight',     chargedPVTight,     dotight),
                                 ('ntMet',            nt,                 doneutrals),
                                 ('ntCentralMet',     ntcentral,          doneutrals),]:
            if not doit: continue
            setattr(event,coll+self.cfg_ana.collectionPostFix, sumP4(p4coll))
            getattr(event,coll+self.cfg_ana.collectionPostFix).sumEt = sum(x.pt() for x in p4coll)
            setattr(event,coll+'_Count'+self.cfg_ana.collectionPostFix, len(p4coll))

        #add leading particles
        setattr(event,'leadCharged'+self.cfg_ana.collectionPostFix,leadCharged if leadCharged else ROOT.reco.Particle.LorentzVector(0.,0.,0.,0.))
        setattr(event,'leadNeutral'+self.cfg_ana.collectionPostFix,leadNeutral if leadNeutral else ROOT.reco.Particle.LorentzVector(0.,0.,0.,0.))

        if  hasattr(event,'zll_p4'):
            self.adduParaPerp(getattr(event,"tkMet"+self.cfg_ana.collectionPostFix), event.zll_p4,"_zll")
            self.adduParaPerp(getattr(event,"tkMetPVchs"+self.cfg_ana.collectionPostFix), event.zll_p4,"_zll")
            self.adduParaPerp(getattr(event,"tkMetPVLoose"+self.cfg_ana.collectionPostFix), event.zll_p4,"_zll")
            self.adduParaPerp(getattr(event,"tkMetPVTight"+self.cfg_ana.collectionPostFix), event.zll_p4,"_zll")


    def makeGenTkMet(self, event):
        genCharged,genChargedInc=[],[]
        for x in self.mchandles['packedGen'].product():
            if x.charge() == 0: continue
            if x.status() != 1: continue
            if abs(x.eta()) < 2.4 : genCharged.append( x.p4() )
            if abs(x.eta()) < 5   : genChargedInc.append( x.p4() )
        def sumP4(p4s):
            p4=ROOT.reco.Particle.LorentzVector(0.,0.,0.,0.)
            for p in p4s: p4 -= p
            return p4
        for coll,p4coll in [('tkGenMet',genCharged),
                             ('tkGenMetInc',genChargedInc)]:
            setattr(event,coll+self.cfg_ana.collectionPostFix, sumP4(p4coll))
            getattr(event,coll+self.cfg_ana.collectionPostFix).sumEt = sum(x.pt() for x in p4coll)

    def makeMETNoMu(self, event):
        self.metNoMu = copy.deepcopy(self.met)
        if self.cfg_ana.doMetNoPU: self.metNoMuNoPU = copy.deepcopy(self.metNoPU)

        mupx = 0
        mupy = 0
        #sum muon momentum
        for mu in event.selectedMuons:
            mupx += mu.px()
            mupy += mu.py()

        #subtract muon momentum and construct met
        px,py = self.metNoMu.px()+mupx, self.metNoMu.py()+mupy
        self.metNoMu.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
        setattr(event, "metNoMu"+self.cfg_ana.collectionPostFix, self.metNoMu)
        if self.cfg_ana.doMetNoPU:
            px,py = self.metNoMuNoPU.px()+mupx, self.metNoMuNoPU.py()+mupy
            self.metNoMuNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            setattr(event, "metNoMuNoPU"+self.cfg_ana.collectionPostFix, self.metNoMuNoPU)


    def makeMETNoEle(self, event):
        self.metNoEle = copy.deepcopy(self.met)
        if self.cfg_ana.doMetNoPU: self.metNoEleNoPU = copy.deepcopy(self.metNoPU)

        elepx = 0
        elepy = 0
        #sum electron momentum
        for ele in event.selectedElectrons:
            elepx += ele.px()
            elepy += ele.py()

        #subtract electron momentum and construct met
        px,py = self.metNoEle.px()+elepx, self.metNoEle.py()+elepy
        self.metNoEle.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
        setattr(event, "metNoEle"+self.cfg_ana.collectionPostFix, self.metNoEle)

        if self.cfg_ana.doMetNoPU:
            px,py = self.metNoEleNoPU.px()+elepx, self.metNoEleNoPU.py()+elepy
            self.metNoEleNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            setattr(event, "metNoEleNoPU"+self.cfg_ana.collectionPostFix, self.metNoEleNoPU)

    def makeMETNoPhoton(self, event):
        self.metNoPhoton = copy.deepcopy(self.met)

        phopx = 0
        phopy = 0
        #sum photon momentum
        for pho in event.selectedPhotons:
            phopx += pho.px()
            phopy += pho.py()

        #subtract photon momentum and construct met
        px,py = self.metNoPhoton.px()+phopx, self.metNoPhoton.py()+phopy
        self.metNoPhoton.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
        setattr(event, "metNoPhoton"+self.cfg_ana.collectionPostFix, self.metNoPhoton)
        if self.cfg_ana.doMetNoPU: 
          self.metNoPhotonNoPU = copy.deepcopy(self.metNoPU)
          px,py = self.metNoPhotonNoPU.px()+phopx, self.metNoPhotonNoPU.py()+phopy
          self.metNoPhotonNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
          setattr(event, "metNoPhotonNoPU"+self.cfg_ana.collectionPostFix, self.metNoPhotonNoPU)


    def makeMETs(self, event):
        import ROOT
        if self.cfg_ana.copyMETsByValue:
          self.met = ROOT.pat.MET(self.handles['met'].product()[0])
          if self.cfg_ana.doMetNoPU: self.metNoPU = ROOT.pat.MET(self.handles['nopumet'].product()[0])
        else:
          self.met = self.handles['met'].product()[0]
          if self.cfg_ana.doMetNoPU: self.metNoPU = self.handles['nopumet'].product()[0]

        if self.recalibrateMET == "type1":
          type1METCorr = getattr(event, 'type1METCorr'+self.jetAnalyzerPostFix)
          self.type1METCorrector.correct(self.met, type1METCorr)
        elif self.recalibrateMET == True:
          deltaMetJEC = getattr(event, 'deltaMetFromJEC'+self.jetAnalyzerPostFix)
          self.applyDeltaMet(self.met, deltaMetJEC)
        if self.applyJetSmearing:
          deltaMetSmear = getattr(event, 'deltaMetFromJetSmearing'+self.jetAnalyzerPostFix)
          self.applyDeltaMet(self.met, deltaMetSmear)

        if (not self.cfg_ana.copyMETsByValue) and getattr(self.cfg_ana, 'makeShiftedMETs', True):
          shifts = [] 
          for obj in 'JetEn', 'JetRes', 'MuonEn', 'ElectronEn', 'PhotonEn', 'TauEn', 'UnclusteredEn':
            for sh in 'Up', 'Down':
                shifts.append( (obj+sh, getattr(self.met,obj+sh)) )
          shifts.append( ('NoShift', self.met.NoShift) )
          for name,i in shifts:
               key = i
               m = ROOT.pat.MET(self.met)
               if self.old74XMiniAODs:
                    if key > 12:   key = 12
                    elif key <= 3: key = { 'JetEnUp':0, 'JetEnDown':1, 'JetResUp':2, 'JetResDown':3 }[name]
               m.setP4(self.met.shiftedP4(key))
               setattr(event, "met{0}_shifted_{1}".format(self.cfg_ana.collectionPostFix, i),m)
               setattr(event, "met{0}_shifted_{1}".format(self.cfg_ana.collectionPostFix, name),m)

        self.met_sig = self.met.metSignificance()
        self.met_sumet = self.met.sumEt()

        if self.cfg_ana.storePuppiExtra:
            self.met.EGCorX = self.handles['corX'].product()[0]
            self.met.EGCorY = self.handles['corY'].product()[0]
            setattr(event,"met_EGCorX"+self.cfg_ana.collectionPostFix, self.met.EGCorX)
            setattr(event,"met_EGCorY"+self.cfg_ana.collectionPostFix, self.met.EGCorY)


        if self.old74XMiniAODs and self.recalibrateMET != "type1":
           oldraw = self.met.shiftedP2_74x(12,0);
           setFakeRawMETOnOldMiniAODs( self.met, oldraw.px, oldraw.py, self.met.shiftedSumEt_74x(12,0) )
           px, py = oldraw.px, oldraw.py
        else:
           px, py = self.met.uncorPx(), self.met.uncorPy()
        self.met_raw = ROOT.reco.Particle.LorentzVector(px,py,0,hypot(px,py))

        if hasattr(event,'zll_p4'):
            self.adduParaPerp(self.met,event.zll_p4,"_zll")
            self.adduParaPerp(self.met_raw, event.zll_p4,"_zll")
            setattr(event,"met_raw"+self.cfg_ana.collectionPostFix, self.met_raw)
            setattr(event,"met_raw.upara_zll"+self.cfg_ana.collectionPostFix, self.met_raw.upara_zll)
            setattr(event,"met_raw.uperp_zll"+self.cfg_ana.collectionPostFix, self.met_raw.uperp_zll)

        if hasattr(event,'gamma_p4'):
            self.adduParaPerp(self.met,event.gamma_p4,"_gamma")
            self.adduParaPerp(self.met_raw, event.gamma_p4,"_gamma")
            setattr(event,"met_raw"+self.cfg_ana.collectionPostFix, self.met_raw)
            setattr(event,"met_raw.upara_gamma"+self.cfg_ana.collectionPostFix, self.met_raw.upara_gamma)
            setattr(event,"met_raw.uperp_gamma"+self.cfg_ana.collectionPostFix, self.met_raw.uperp_gamma)

        if hasattr(event,"met"+self.cfg_ana.collectionPostFix): raise RuntimeError("Event already contains met with the following postfix: "+self.cfg_ana.collectionPostFix)
        setattr(event, "met"+self.cfg_ana.collectionPostFix, self.met)
        if self.cfg_ana.doMetNoPU: setattr(event, "metNoPU"+self.cfg_ana.collectionPostFix, self.metNoPU)
        setattr(event, "met_sig"+self.cfg_ana.collectionPostFix, self.met_sig)
        setattr(event, "met_sumet"+self.cfg_ana.collectionPostFix, self.met_sumet)

        genMET = self.met.genMET()
        if genMET:
          setattr(event, "met_genPt"+self.cfg_ana.collectionPostFix, genMET.pt())
          setattr(event, "met_genPhi"+self.cfg_ana.collectionPostFix, genMET.phi())
        else:
          setattr(event, "met_genPt"+self.cfg_ana.collectionPostFix, float('nan'))
          setattr(event, "met_genPhi"+self.cfg_ana.collectionPostFix, float('nan'))

        if self.cfg_ana.doMetNoMu and hasattr(event, 'selectedMuons'):
            self.makeMETNoMu(event)

        if self.cfg_ana.doMetNoEle and hasattr(event, 'selectedElectrons'):
            self.makeMETNoEle(event)

        if self.cfg_ana.doMetNoPhoton and hasattr(event, 'selectedPhotons'):
            self.makeMETNoPhoton(event)

    def process(self, event):
        self.readCollections( event.input)
        self.counters.counter('events').inc('all events')

        self.makeMETs(event)

        if self.cfg_ana.doTkMet: 
            self.makeTkMETs(event);

        if self.cfg_ana.doPuppiMet:
            self.makePuppiMETs(event);

        if getattr(self.cfg_ana,"doTkGenMet",self.cfg_ana.doTkMet) and self.cfg_comp.isMC and hasattr(event, 'genParticles'):
            self.makeGenTkMet(event)

        return True


setattr(METAnalyzer,"defaultConfig", cfg.Analyzer(
    class_object = METAnalyzer,
    metCollection     = "slimmedMETs",
    noPUMetCollection = "slimmedMETs",
    copyMETsByValue = False,
    recalibrate = True,
    applyJetSmearing = True,
    jetAnalyzerPostFix = "",
    old74XMiniAODs = False, # need to set to True to get proper Raw MET on plain 74X MC produced with CMSSW <= 7_4_12
    makeShiftedMETs = True,
    doTkMet = False,
    doPuppiMet = False,
    ### more on tkMET
    includeTkMetCHS = False,
    includeTkMetPVLoose = False,
    includeTkMetPVTight = False,
    includeTkMetNoPV = False,
    includeTkMetPVUsedInFit = False,
    ###
    doMetNoPU = False, # Not existing in MiniAOD at the moment
    doMetNoMu = False,  
    doMetNoEle = False,  
    doMetNoPhoton = False,  
    storePuppiExtra = False,
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    dzMax = 0.1,
    collectionPostFix = "",
    )
)
