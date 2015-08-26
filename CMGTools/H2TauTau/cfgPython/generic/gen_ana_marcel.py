from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools.samples.samples_13TeV_74X import QCD_Pt170to300
from PhysicsTools.HeppyCore.utils.deltar import deltaR


handle  = Handle ('std::vector<pat::PackedGenParticle>')
label = ("packedGenParticles")

j_handle  = Handle ('std::vector<pat::Jet>')
j_label = ("slimmedJets")

events = Events(QCD_Pt170to300.files[0])

for ev in events:
    ev.getByLabel(j_label, j_handle)
    jets = j_handle.product()
    jets30 = [j for j in jets if j.pt() > 30.]
    for j in jets30:
        ev.getByLabel(label, handle)
        gps = handle.product()
        stat1 = [p for p in gps if p.status()==1 and deltaR(p.eta(), p.phi(), j.eta(), j.phi()) < 0.4] 
        import pdb; pdb.set_trace()
        # print [p.pdgId() f
