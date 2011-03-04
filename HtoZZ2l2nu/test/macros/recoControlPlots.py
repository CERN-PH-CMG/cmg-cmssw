import sys,os
import commands
import math
from math import sqrt,pow,sin
import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import setStyle, getNewCanvas, formatPlot, formatForCmsPublic, fixExtremities, showPlots


"""
loops over files in directory and fills control histograms
"""
def getControlPlots(url) :

    from DataFormats.FWLite import Events, Handle
    ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
    from genLevelUtils import getDecayTreeFrom, getDecayCounters

    #objects of interest
    jetHandle  = Handle ('std::vector<cmg::BaseJet>')
    jetLabel = ('cmgPFBaseJetSel')

    hcandHandle  = Handle ('std::vector<cmg::DiObject<cmg::DiObject<cmg::Muon,cmg::Muon>,cmg::BaseMET> >')
    hcandLabel = ('cmgDiMuonPlusMET')
       
    #book histograms
    results={}
    results['cutflow']          = formatPlot( ROOT.TH1F("cutflow", ";Cut flow; Events", 6, 0.,6.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    cutflowsteps=['reco','p_{T}>20','fid.trigger','ID','RelIso<0.15','M_{ll}>50']
    for i in xrange(0,len(cutflowsteps)): results['cutflow'].GetXaxis().SetBinLabel(i+1,cutflowsteps[i])
    results['muon_pt']      = formatPlot( ROOT.TH1F("muon_pt", ";p_{T} [GeV/c]; Muons", 100, 0.,200.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['muon_reliso']      = formatPlot( ROOT.TH1F("muon_reliso", ";Relative Isolation; Muons", 100, 0.,2.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_dz']        = formatPlot( ROOT.TH1F("dimuon_dz", ";#Delta z(#mu,#mu) [cm]; Events", 100, -0.02,0.02), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_mass']      = formatPlot( ROOT.TH1F("dimuon_mass", ";Invariant Mass(#mu,#mu) [GeV/c^{2}]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_pt']        = formatPlot( ROOT.TH1F("dimuon_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_dphi']      = formatPlot( ROOT.TH1F("dimuon_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['met2muon_mindphi'] = formatPlot( ROOT.TH1F("met2muon_mindphi", ";min #Delta #phi(#slash{E}_{T},#mu) [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['met']              = formatPlot( ROOT.TH1F("met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['st']               = formatPlot( ROOT.TH1F("st", ";S_{T} [GeV/c]; Events", 100,  0.,1000.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon2met_dphi']  = formatPlot( ROOT.TH1F("dimuon2met_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['projmet']          = formatPlot( ROOT.TH1F("projmet", ";projected #slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['mT']               = formatPlot( ROOT.TH1F("mT",";Transverse mass [GeV/c^{2}]; Events",100,0,1000), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['njets']            = formatPlot( ROOT.TH1F("njets",";Jet multiplicity; Events",4,0,4), 1, 1, 1, 20, 0, True, True, 1,1,1)
#    results['bmult']            = formatPlot( ROOT.TH1F("bmult",";b tag multiplicity; Events",4,0,4), 1, 1, 1, 20, 0, True, True, 1,1,1)
    for ibin in xrange(0,results['njets'].GetXaxis().GetNbins()) :
        ilabel=str(ibin)
        if(ibin==results['njets'].GetXaxis().GetNbins()-1) : ilabel='#geq'+ilabel
        results['njets'].GetXaxis().SetBinLabel(ibin+1,ilabel)
#        results['bmult'].GetXaxis().SetBinLabel(ibin+1,ilabel)
#    results['btags']           = formatPlot( ROOT.TH1F("btags",";b tags (SSVHE); Jets",100,-0.5,2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['jetpt']           = formatPlot( ROOT.TH1F("jetpt",";p_{T} [GeV/c]; Jets",100,0,200), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['jet2zcand_mindphi']  = formatPlot( ROOT.TH1F("jet2zcand_mindphi", ";min #Delta #phi(jet,#slash{E}_{T} or #mu#mu) [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
     
    #build the list of files
    import os
    dirList=[url]
    if(url.find('castor')>0):
        sc, flist = commands.getstatusoutput('nsls %s' % (url,) )
        flist = flist.split()
        for f in flist:
            if(f.find('cmgTuple')<0): continue
            dirList.append(f)
    else :
            dirList=os.listdir(url)

    #loop over results in files
    for fname in dirList:
        if(fname.find(".root")<0) : continue

        absUrl=url + '/' + fname
        if(url.find('castor')>0) : absUrl = 'rfio://' + url + '/' + fname
        file = ROOT.TFile.Open(absUrl)
        if(file==None) :
            continue
        if(file.IsZombie()):
            file.Close()
            continue
        events = Events (file)
        
        # loop over events
        for event in events:
            
            #get the dimuon+met level
            event.getByLabel(hcandLabel,hcandHandle)
            hcands=hcandHandle.product()
            if(hcands.size()==0): continue

            #get the dimuon+met candidate
            muP=[]
            recodimu=ROOT.TLorentzVector(0,0,0,0)
            recomet=ROOT.TLorentzVector(0,0,0,0)
            mu1=hcands[0].leg1().leg1()
            muP.append(ROOT.TLorentzVector(mu1.px(),mu1.py(),mu1.pz(),mu1.energy()))
            mu2=hcands[0].leg1().leg2()
            muP.append(ROOT.TLorentzVector(mu2.px(),mu2.py(),mu2.pz(),mu2.energy()))
            recodimu=muP[0]+muP[1]
            themet=hcands[0].leg2()
            recomet=ROOT.TLorentzVector(-themet.px(),-themet.py(),-themet.pz(),themet.energy())
      
            dphimin = min( [ abs(recomet.DeltaPhi(muP[0])) , abs(recomet.DeltaPhi(muP[1])) ] )
            projmet=recomet.Pt()
            if(dphimin<math.pi/2) : projmet = projmet*sin(dphimin)

            #reco
            results['cutflow'].Fill(0)

            #basic kinematics
            results['muon_pt'].Fill(mu1.pt())
            results['muon_pt'].Fill(mu2.pt())
            if(mu1.pt()<20 or mu2.pt()<20 or abs(mu1.eta())>2.4 or abs(mu2.eta())>2.4) : continue
            results['cutflow'].Fill(1)
            
            #one muon trigger fiducial region
            if(abs(mu1.eta())>2.1 and abs(mu2.eta())>2.1) : continue
            results['cutflow'].Fill(2)

            #id cuts
            if( not mu1.isGlobal() or not mu1.isTracker() ) : continue
            if( not mu2.isGlobal() or not mu2.isTracker() ) : continue
            if( mu1.numberOfValidTrackerHits()<10 or mu2.numberOfValidTrackerHits()<10 ): continue
            if( not mu1.muonID() or not mu2.muonID() ) : continue
            results['cutflow'].Fill(3)

            results['muon_reliso'].Fill(mu1.relIso())
            results['muon_reliso'].Fill(mu2.relIso())
            results['dimuon_dz'].Fill(mu1.dz()-mu2.dz())
            if(mu1.relIso()>0.15 or mu2.relIso()>0.15) : continue
            results['cutflow'].Fill(4)
            
            #veto low mass dileptons
            if(recodimu.M()<50) : continue
            results['cutflow'].Fill(5)

            #compute the transverse mass
            transvSum=recodimu+recomet
            transvMass=pow(sqrt(pow(recodimu.Pt(),2)+pow(recodimu.M(),2))+sqrt(pow(recomet.Pt(),2)+pow(recodimu.M(),2)),2)
            transvMass-=pow(transvSum.Pt(),2)
            transvMass=sqrt(transvMass)

            #fill the histograms
            results['dimuon_mass'].Fill(recodimu.M())
            results['dimuon_pt'].Fill(recodimu.Pt())
            results['dimuon_dphi'].Fill(abs(muP[0].DeltaPhi(muP[1])))
            results['met'].Fill(recomet.Pt())
            results['st'].Fill(recomet.Pt()+muP[0].Pt()+muP[1].Pt())
            results['dimuon2met_dphi'].Fill(abs(recodimu.DeltaPhi(recomet)))
            results['met2muon_mindphi'].Fill(dphimin)
            results['projmet'].Fill(projmet)
            results['mT'].Fill(transvMass)

            event.getByLabel(jetLabel,jetHandle)
            njets=0
            nbjets=0
            for j in jetHandle.product() :
                if (j.pt()<30 or abs(j.eta())>2.4) : continue
                njets+=1
#                btag=j.btag()
#                if(btag>1.7) : nbjets+=1 #medium point
 #               results['btags'].Fill(btag)
                results['jetpt'].Fill(j.pt())
                jetMom = ROOT.TLorentzVector(j.px(),j.py(),j.pz(),j.energy())
                jet2zcand_mindphi = min( [ abs(jetMom.DeltaPhi(recodimu)), abs(jetMom.DeltaPhi(recomet)) ] )
                results['jet2zcand_mindphi'].Fill(jet2zcand_mindphi)
            results['njets'].Fill(njets)
#            results['bmult'].Fill(nbjets)

        file.Close()

    #add overflow+underflow
    for r in results.items() : fixExtremities(r[1],True,True)
    return results

"""
shows the control plots
"""
def showControlPlots(stackplots=None,spimposeplots=None,dataplots=None,generalLabel='CMS preliminary',outdir='plots') :

    if(len(stackplots)==0 and len(spimposeplots)==0 and len(dataplots)==0) : return

    #convert to lists
    stackLists=[]
    for p in stackplots:
        for i in xrange(0,len(p)):
            if(len(stackLists)<=i):
                newlist=ROOT.TList()
                stackLists.append(newlist)
            stackLists[i].Add(p[i])
            
    spimposeLists=[]
    for p in spimposeplots:
        for i in xrange(0,len(p)):
            if(len(spimposeLists)<=i):
                newlist=ROOT.TList()
                spimposeLists.append(newlist)
            spimposeLists[i].Add(p[i])

    dataLists=[]
    for p in dataplots:
        for i in xrange(0,len(p)):
            if(len(dataLists)<=i):
                newlist=ROOT.TList()
                dataLists.append(newlist)
            dataLists[i].Add(p[i])

    #draw result
    nplots=max([len(stackLists), len(dataLists), len(spimposeLists)])
    if(nplots==0) : return
    
    setStyle()
    c = getNewCanvas("recolevelc","recolevelc",False)
    c.SetWindowSize(600,600)
    for iplot in xrange(0,nplots):
        stack=ROOT.TList()
        if(len(stackLists)>iplot): stack=stackLists[iplot]
        spimpose=ROOT.TList()
        if(len(spimposeLists)>iplot): spimpose=spimposeLists[iplot]
        data=ROOT.TList()
        if(len(dataLists)>iplot): data=dataLists[iplot]
        leg=showPlots(c,stack,spimpose,data)
        formatForCmsPublic(c,leg,generalLabel,nplots)
        raw_input('Any key to continue...')
        c.SaveAs('plots/'+stack.At(0).GetName()+'.png')
        c.SaveAs('plots/'+stack.At(0).GetName()+'.C')

        
    
    
