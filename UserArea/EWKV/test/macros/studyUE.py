#! /usr/bin/env python

import ROOT
import sys
from DataFormats.FWLite import Events, Handle

def traceHistory(p):
    m=p.mother(0)
    mid=m.pdgId()
    while mid!=0 and mid != 2212:
        mid=traceHistory(m)
    return mid
    
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.parseArguments()

# use Varparsing object
events = Events (options)

# create handles
genParticlesHandle = Handle ('std::vector<reco::GenParticle>')
genJetsHandle     = Handle('std::vector<reco::GenJet>')

# Create histograms, etc.
ROOT.gROOT.SetBatch()        # don't pop up canvases
ROOT.gROOT.SetStyle('Plain') # white background

njets_h  = ROOT.TH1F('njets',";Jet multiplicity; Events",10,0,10)
mjj_h    = ROOT.TH1F('mjj',";M_{jj} [GeV]; Events",100,0,3000)
detajj_h = ROOT.TH1F('detajj',";|#Delta #eta_{jj}|; Events",100,0,8)

mjj_mpi_h = mjj_h.Clone('mjj_mpi')
detajj_mpi_h = detajj_h.Clone('detajj_mpi')

# loop over events
for event in events:

    #get objects from the event
    event.getByLabel ('genParticles', genParticlesHandle)
    event.getByLabel ('ak5GenJets',   genJetsHandle)

    #select leptons
    genParticles=genParticlesHandle.product()
    leptons=[]
    for p in genParticles:
        if p.status()!=3 : continue
        if abs(p.pdgId()) not in [11,13]: continue
        if p.pt()<20 or abs(p.eta())>2.5: continue
        leptons.append(p)
    if len(leptons)<2 : continue

    #select jets
    genJets = genJetsHandle.product()
    selJets=[]
    for j in genJets:

        j4=ROOT.TLorentzVector(j.px(),j.py(),j.pz(),j.energy())
        if(j4.Pt()<30 or abs(j4.Eta())>4.7): continue
        
        minDR=9999
        for l in leptons :
            l4=ROOT.TLorentzVector(l.px(),l.py(),l.pz(),l.energy())
            dR=l4.DeltaR(j4)
            if(dR<minDR) : minDR=dR
        if minDR<0.3 : continue
       
        selJets.append(j)
    selJets.sort(key=lambda x: x.pt(), reverse=True)
    nselJets=len(selJets)

    jetOrigin=[]
    for j in selJets:
        n=0
        nFromHP=0
        nFromMPI=0
        while 1:
            if j.getGenConstituent(n) :
                type=traceHistory(j.getGenConstituent(n))
                if(type==2212) : nFromHP=nFromHP+1
                if(type==0)    : nFromMPI=nFromMPI+1
                n=n+1
                break
            else:
                break
        if nFromMPI>nFromHP : jetOrigin.append(0)
        else : jetOrigin.append(1)
        #print '%d hp=%f mpi=%f'%(n,nFromHP/n,nFromMPI/n)

    njets_h.Fill(nselJets)
    if(nselJets<2): continue

    #analyse leading pT jets
    j1=selJets.pop()
    j1Orig=jetOrigin.pop()
    j2=selJets.pop()
    j2Orig=jetOrigin.pop()
    
    dijet=j1.p4()+j2.p4()
    mjj_h.Fill(dijet.mass())
    detajj_h.Fill(abs(j1.eta()-j2.eta()))
    if(j1Orig==1 or j2Orig==1) :
        mjj_mpi_h.Fill(dijet.mass())
        detajj_mpi_h.Fill(abs(j1.eta()-j2.eta()))
    
        
# make a canvas, draw, and save it
c=ROOT.TCanvas()
c.SetWindowSize(1200,400)
c.Divide(3,1)
c.cd(1)
njets_h.Draw()
c.cd(2)
mjj_h.Draw()
mjj_mpi_h.Draw()
c.cd(3)
detajj_h.Draw()
detajj_mpi_h.Draw()
c.Print ('njets.png')
