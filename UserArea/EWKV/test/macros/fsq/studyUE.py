#! /usr/bin/env python

# python studyUE.py inputFiles=file

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
outF=options.outputFile

#get the events branch
events = Events (options)

# create handles
genParticlesHandle = Handle ('std::vector<reco::GenParticle>')
genJetsHandle     = Handle('std::vector<reco::GenJet>')

# Create histograms, etc.
mon={}
mon['zpt']         = ROOT.TH1F('zpt',";p_{T} [GeV]; Events",100,0,1000)
mon['njets']       = ROOT.TH1F('njets',";Jet multiplicity; Events",10,0,10)
mon['mjj']         = ROOT.TH1F('mjj',";M_{jj} [GeV]; Events",100,0,3000)
mon['spt']         = ROOT.TH1F('spt',";S_{p_{T}} ; Events",100,0,1)
mon['detajj']      = ROOT.TH1F('detajj',";|#Delta #eta_{jj}|; Events",100,0,8)
mon['leadetaj']    = ROOT.TH1F('leadetaj',";max |#eta_{j}|; Events",100,0,6)
mon['traileretaj'] = ROOT.TH1F('traileretaj',";min |#eta_{j}|; Events",100,0,6)
mon['mjj_mpi']     = mon['mjj'].Clone('mjj_mpi')
mon['spt_mpi']     = mon['spt'].Clone('mjj_mpi')
mon['detajj_mpi']  = mon['detajj'].Clone('detajj_mpi')
for key, h in mon.iteritems():
    h.Sumw2()
    h.SetDirectory(0)

# loop over events
for event in events:

    #get objects from the event
    event.getByLabel ('genParticles', genParticlesHandle)
    event.getByLabel ('ak5GenJets',   genJetsHandle)

    #select leptons
    genParticles=genParticlesHandle.product()
    leptons=[]
    zpt=0
    for p in genParticles:
        if p.status()!=3 : continue
        if abs(p.pdgId())==23:
            zpt=p.pt()
            continue
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


    mon['njets'].Fill(nselJets)
    if(nselJets<2): continue

    if(zpt>0) : mon['zpt'].Fill(zpt)

    #analyse leading pT jets
    j1=selJets.pop()
    j1Orig=jetOrigin.pop()
    j2=selJets.pop()
    j2Orig=jetOrigin.pop()
    dijet=j1.p4()+j2.p4()
    spt=dijet.pt()/(j1.pt()+j2.pt())
    
    mon['mjj'].Fill(dijet.mass())
    mon['spt'].Fill(spt)
    mon['detajj'].Fill(abs(j1.eta()-j2.eta()))
    mon['leadetaj'].Fill(max(abs(j1.eta()),abs(j2.eta())))
    mon['traileretaj'].Fill(min(abs(j1.eta()),abs(j2.eta())))
                     
    if(j1Orig==0 or j2Orig==0) :
        mon['mjj_mpi'].Fill(dijet.mass())
        mon['detajj_mpi'].Fill(abs(j1.eta()-j2.eta()))
        mon['spt_mpi'].Fill(spt)


#save histos
fOut=ROOT.TFile(outF,"RECREATE")
fOut.cd()
for key, h in mon.iteritems():
    h.Write()
fOut.Close()
