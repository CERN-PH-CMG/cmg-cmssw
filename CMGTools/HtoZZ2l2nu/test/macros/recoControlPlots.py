import sys,os
import commands
import math
from math import sqrt,pow,sin,cos
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

    mumuHandle  = Handle ('std::vector<cmg::DiObject<cmg::Muon,cmg::Muon> >')
    mumuLabel = ('cmgDiMuon')

    eeHandle  = Handle ('std::vector<cmg::DiObject<cmg::Electron,cmg::Electron> >')
    eeLabel = ('cmgDiElectron')

    metHandle  = Handle ('std::vector<cmg::BaseMET>')
    metLabel = ('cmgMETPFCandidates')
       
    #book histograms
    results={}
    streams=['e','mu']
    for istream in streams :
        results[istream+'_cutflow']          = formatPlot( ROOT.TH1F(istream+"_cutflow", ";Cut flow; Events", 6, 0.,6.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        cutflowsteps=['reco','p_{T}>20','fid.trigger','ID','Isolation','M_{ll}>50']
        for i in xrange(0,len(cutflowsteps)): results[istream+'_cutflow'].GetXaxis().SetBinLabel(i+1,cutflowsteps[i])
        results[istream+'_pt']                = formatPlot( ROOT.TH1F(istream+"_pt", ";p_{T} [GeV/c]; Leptons", 100, 0.,200.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_reliso']            = formatPlot( ROOT.TH1F(istream+"_reliso", ";Relative Isolation; Leptons", 100, 0.,2.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_dilepton_dz']       = formatPlot( ROOT.TH1F(istream+"_dilepton_dz", ";#Delta z(l,l') [cm]; Events", 100, -0.02,0.02), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_dilepton_mass']     = formatPlot( ROOT.TH1F(istream+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_dilepton_pt']       = formatPlot( ROOT.TH1F(istream+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_dilepton_dphi']     = formatPlot( ROOT.TH1F(istream+"_dilepton_dphi", ";#Delta #phi(l,l') [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results['met2'+istream+'_mindphi']    = formatPlot( ROOT.TH1F("met2'+istream+'_mindphi", ";min #Delta #phi(#slash{E}_{T},l) [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_met']               = formatPlot( ROOT.TH1F(istream+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_st']                = formatPlot( ROOT.TH1F(istream+"_st", ";S_{T} [GeV/c]; Events", 100,  0.,1000.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_dilepton2met_dphi'] = formatPlot( ROOT.TH1F(istream+"dilepton2met_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_projmet']           = formatPlot( ROOT.TH1F(istream+"_projmet", ";projected #slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_mT']                = formatPlot( ROOT.TH1F(istream+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",100,0,1000), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_mT_individual']     = formatPlot( ROOT.TH1F(istream+"_mT_individual",";Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_mT_individualsum']  = formatPlot( ROOT.TH1F(istream+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_projmT_individual']     = formatPlot( ROOT.TH1F(istream+"_projmT_individual",";Transverse mass(lepton,projected MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_projmT_individualsum']  = formatPlot( ROOT.TH1F(istream+"_projmT_individualsum",";#Sigma Transverse mass(lepton,projected MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_njets']             = formatPlot( ROOT.TH1F(istream+"_njets",";Jet multiplicity; Events",4,0,4), 1, 1, 1, 20, 0, True, True, 1,1,1)
        #    results['bmult']            = formatPlot( ROOT.TH1F("bmult",";b tag multiplicity; Events",4,0,4), 1, 1, 1, 20, 0, True, True, 1,1,1)
        for ibin in xrange(0,results[istream+'_njets'].GetXaxis().GetNbins()) :
            ilabel=str(ibin)
            if(ibin==results[istream+'_njets'].GetXaxis().GetNbins()-1) : ilabel='#geq'+ilabel
            results[istream+'_njets'].GetXaxis().SetBinLabel(ibin+1,ilabel)
            #        results['bmult'].GetXaxis().SetBinLabel(ibin+1,ilabel)
        results['btags']                    = formatPlot( ROOT.TH1F("btags",";b tags (SSVHE); Jets",100,-0.5,2), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_jetpt']           = formatPlot( ROOT.TH1F(istream+"_jetpt",";p_{T} [GeV/c]; Jets",100,0,200), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[istream+'_jet2zll_mindphi'] = formatPlot( ROOT.TH1F(istream+"_jet2zcand_mindphi", ";min #Delta #phi(jet,#slash{E}_{T} or ll) [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
     
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
            
            #get the dimuon
            event.getByLabel(eeLabel,eeHandle)
            event.getByLabel(mumuLabel,mumuHandle)
            event.getByLabel(metLabel,metHandle)

            """
            this requires work: first select leptons carefully,
            then loop over the dilepton candidates,
            for now, i'm just taking the leading one...
            """
            dilCand=None
            istream=''
            if(mumuHandle.product().size()>0):
                dilCand = mumuHandle.product()[0]
                istream='mu'
            elif(eeHandle.product().size()>0):
                dilCand=eeHandle.product()[0]
                istream='e'
            if(dilCand is None): continue
            if(metHandle.product().size()==0): continue
            
            #analyze the dilepton+met candidate
            dilP=[]
            recodil=ROOT.TLorentzVector(0,0,0,0)
            recomet=ROOT.TLorentzVector(0,0,0,0)
            lepton1=dilCand.leg1()
            dilP.append(ROOT.TLorentzVector(lepton1.px(),lepton1.py(),lepton1.pz(),lepton1.energy()))
            lepton2=dilCand.leg2()
            dilP.append(ROOT.TLorentzVector(lepton2.px(),lepton2.py(),lepton2.pz(),lepton2.energy()))
            recodil=dilP[0]+dilP[1]
            themet=metHandle.product()[0]
            recomet=ROOT.TLorentzVector(-themet.px(),-themet.py(),-themet.pz(),themet.energy())

            dphil2met=[ abs(recomet.DeltaPhi(dilP[0])) , abs(recomet.DeltaPhi(dilP[1])) ]
            mTlmet=[sqrt(2*recomet.Pt()*dilP[0].Pt()*(1-cos(dphil2met[0]))),
                    sqrt(2*recomet.Pt()*dilP[1].Pt()*(1-cos(dphil2met[1])))]
            dphimin = min( dphil2met )
            projmet=recomet.Pt()
#            recoprojmet=recomet
            if(dphimin<math.pi/2) :
                projmet = projmet*sin(dphimin)
#            dphil2projmet=[ abs(recoprojmet.DeltaPhi(dilP[0])) , abs(recoprojmet.DeltaPhi(dilP[1])) ]
#            mTlprojmet=[sqrt(2*recoprojmet.Pt()*dilP[0].Pt()*(1-cos(dphil2projmet[0]))),
#                        sqrt(2*recoprojmet.Pt()*dilP[1].Pt()*(1-cos(dphil2projmet[1])))]
         
            #reco
            results[istream+'_cutflow'].Fill(0)

            #basic kinematics
            results[istream+'_pt'].Fill(lepton1.pt())
            results[istream+'_pt'].Fill(lepton2.pt())
            if(lepton1.pt()<20 or lepton2.pt()<20 or abs(lepton1.eta())>2.4 or abs(lepton2.eta())>2.4) : continue
            results[istream+'_cutflow'].Fill(1)
            
            #one muon trigger fiducial region
            if(istream=='mu') :
                if(abs(lepton1.eta())>2.1 and abs(lepton2.eta())>2.1) : continue
            results[istream+'_cutflow'].Fill(2)

            #id cuts
            if(istream=='mu') :
                if( not lepton1.isGlobal() or not lepton1.isTracker() ) : continue
                if( not lepton2.isGlobal() or not lepton2.isTracker() ) : continue
                if( lepton1.numberOfValidTrackerHits()<10 or lepton2.numberOfValidTrackerHits()<10 ): continue
                if( not lepton1.muonID() or not lepton2.muonID() ) : continue
            else :
                #if( not lepton1.electronID() or not electron2.muonID() ) : continue
                a=1
            results[istream+'_cutflow'].Fill(3)

            results[istream+'_reliso'].Fill(lepton1.relIso())
            results[istream+'_reliso'].Fill(lepton2.relIso())
            results[istream+'_dilepton_dz'].Fill(lepton1.dz()-lepton2.dz())
            if(lepton1.relIso()>0.25 or lepton2.relIso()>0.25) : continue
            results[istream+'_cutflow'].Fill(4)
            
            #veto low mass dileptons
            if(recodil.M()<50) : continue
            results[istream+'_cutflow'].Fill(5)

            #compute the transverse mass
            transvSum=recodil+recomet
            transvMass=pow(sqrt(pow(recodil.Pt(),2)+pow(recodil.M(),2))+sqrt(pow(recomet.Pt(),2)+pow(recodil.M(),2)),2)
            transvMass-=pow(transvSum.Pt(),2)
            transvMass=sqrt(transvMass)

            #fill the histograms
            results[istream+'_dilepton_mass'].Fill(recodil.M())
            results[istream+'_dilepton_pt'].Fill(recodil.Pt())
            results[istream+'_dilepton_dphi'].Fill(abs(dilP[0].DeltaPhi(dilP[1])))
            results[istream+'_met'].Fill(recomet.Pt())
            results[istream+'_st'].Fill(recomet.Pt()+dilP[0].Pt()+dilP[1].Pt())
            results[istream+'_dilepton2met_dphi'].Fill(abs(recodil.DeltaPhi(recomet)))
            results['met2'+istream+'_mindphi'].Fill(dphimin)
            results[istream+'_projmet'].Fill(projmet)
            results[istream+'_mT'].Fill(transvMass)
            results[istream+'_mT_individual'].Fill(mTlmet[0])
            results[istream+'_mT_individual'].Fill(mTlmet[1])
            results[istream+'_mT_individualsum'].Fill(mTlmet[0]+mTlmet[1])

            event.getByLabel(jetLabel,jetHandle)
            njets=0
            nbjets=0
            for j in jetHandle.product() :
                if (j.pt()<30 or abs(j.eta())>2.4) : continue
                njets+=1
#                btag=j.btag()
#                if(btag>1.7) : nbjets+=1 #medium point
 #               results['btags'].Fill(btag)
                results[istream+'_jetpt'].Fill(j.pt())
                jetMom = ROOT.TLorentzVector(j.px(),j.py(),j.pz(),j.energy())
                jet2zcand_mindphi = min( [ abs(jetMom.DeltaPhi(recodil)), abs(jetMom.DeltaPhi(recomet)) ] )
                results[istream+'_jet2zll_mindphi'].Fill(jet2zcand_mindphi)
            results[istream+'_njets'].Fill(njets)
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
        pname=''
        if(stack.At(0) is not None) :      pname=stack.At(0).GetName()
        elif(spimpose.At(0) is not None) : pname=spimpose.At(0).GetName()
        elif(data.At(0) is not None) :     pname=data.At(0).GetName()
        if(len(pname)<=0): continue
        raw_input('Any key to continue...')
        c.SaveAs('plots/'+pname+'.png')
        c.SaveAs('plots/'+pname+'.C')

        
    
    
