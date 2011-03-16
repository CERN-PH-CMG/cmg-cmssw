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

    emuHandle  = Handle ('std::vector<cmg::DiObject<cmg::Electron,cmg::Muon> >')
    emuLabel = ('cmgEmuDilepton')

    metHandle  = Handle ('std::vector<cmg::BaseMET>')
    metLabel = ('cmgMETPFCandidates')
       
    #book histograms (...need to add emu)
    results={}
    streams=['e','mu']
    jetmult=['eq0jets','eq1jets','geq2jets']
    for istream in streams :

        cat=istream

        #cut flow
        cutflowsteps=['reco','p_{T}>20','fid.trigger','ID','Isolation','M_{ll}>50','=0 jets','=1 jets','#geq 2 jets']
        results[cat+'_cutflow']          = formatPlot( ROOT.TH1F(cat+"_cutflow", ";Cut flow; Events", len(cutflowsteps), 0.,len(cutflowsteps)), 1, 1, 1, 20, 0, True, True, 1,1,1)
        for i in xrange(0,len(cutflowsteps)): results[cat+'_cutflow'].GetXaxis().SetBinLabel(i+1,cutflowsteps[i])

        #lepton control
        results[cat+'_pt']                = formatPlot( ROOT.TH1F(cat+"_pt", ";p_{T} [GeV/c]; Leptons", 100, 0.,200.), 1, 1, 1, 20, 0, True, True, 1,1,1)

        if(istream=='e'):
            results[cat+'_sietaieta']   = formatPlot( ROOT.TH1F(cat+"_sietaieta", ";#sigma_{i#eta-i#eta}; Leptons", 100, 0.,1.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[cat+'_eoverp']     = formatPlot( ROOT.TH1F(cat+"_eoverp", ";E/p; Leptons", 100, 0.,5.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        if(istream=='mu'):
            results[cat+'_chi2']     = formatPlot( ROOT.TH1F(cat+"_chi2", ";#chi^{2}; Leptons", 100, 0.,20.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        
        results[cat+'_eta']               = formatPlot( ROOT.TH1F(cat+"_eta", ";#eta; Leptons", 100, -2.5, 2.5), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+'_reliso']            = formatPlot( ROOT.TH1F(cat+"_reliso", ";Relative Isolation; Leptons", 100, 0.,2.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+"_dilepton_mass"]     = formatPlot( ROOT.TH1F(cat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+"_dilepton_sumpt"]    = formatPlot( ROOT.TH1F(cat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+"_dilepton_pt"]       = formatPlot( ROOT.TH1F(cat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+"_dilepton_dphi"]     = formatPlot( ROOT.TH1F(cat+"_dilepton_dphi", ";#Delta #phi(l,l') [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)

        #jet multiplicity
        results[cat+'_jetpt']             = formatPlot( ROOT.TH1F(cat+"_jetpt",";p_{T} [GeV/c]; Jets",100,0,200), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+'_njets']             = formatPlot( ROOT.TH1F(cat+"_njets",";Jet multiplicity; Events",4,0,4), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+'_bmult']             = formatPlot( ROOT.TH1F(cat+"_bmult",";b tag multiplicity (TCHEL); Events",4,0,4), 1, 1, 1, 20, 0, True, True, 1,1,1)
        for ibin in xrange(0,results[cat+'_njets'].GetXaxis().GetNbins()) :
            ilabel=str(ibin)
            if(ibin==results[cat+'_njets'].GetXaxis().GetNbins()-1) : ilabel='#geq'+ilabel
            results[cat+'_njets'].GetXaxis().SetBinLabel(ibin+1,ilabel)
            results[cat+'_bmult'].GetXaxis().SetBinLabel(ibin+1,ilabel)
        results[cat+'_btags']             = formatPlot( ROOT.TH1F(cat+"_btags",";b tags (TCHE); Jets",100,-0.5,2), 1, 1, 1, 20, 0, True, True, 1,1,1)

        #split the analysis according to the jet multiplicity
        for jstream in jetmult :
            subcat=istream+jstream
            results[subcat+"_dilepton_mass"]     = formatPlot( ROOT.TH1F(subcat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton_sumpt"]    = formatPlot( ROOT.TH1F(subcat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton_pt"]       = formatPlot( ROOT.TH1F(subcat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton_dphi"]     = formatPlot( ROOT.TH1F(subcat+"_dilepton_dphi", ";#Delta #phi(l,l') [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results["met2"+subcat+"_mindphi"]    = formatPlot( ROOT.TH1F("met2"+subcat+"_mindphi", ";min #Delta #phi(#slash{E}_{T},l) [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_met"]               = formatPlot( ROOT.TH1F(subcat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton2met_dphi"] = formatPlot( ROOT.TH1F(subcat+"dilepton2met_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT"]                = formatPlot( ROOT.TH1F(subcat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",100,0,1000), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT_individual"]     = formatPlot( ROOT.TH1F(subcat+"_mT_individual",";Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT_individualsum"]  = formatPlot( ROOT.TH1F(subcat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_transvmet"]           = formatPlot( ROOT.TH1F(subcat+"_transvmet", ";projected #slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_transvmT_individual"] = formatPlot( ROOT.TH1F(subcat+"_transvmT_individual",";Transverse mass(lepton,projected MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_transvmT_individualsum"]  = formatPlot( ROOT.TH1F(subcat+"_transvmT_individualsum",";#Sigma Transverse mass(lepton,projected MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)

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
            
            #get the candidates
            event.getByLabel(eeLabel,eeHandle)
            event.getByLabel(mumuLabel,mumuHandle)
            event.getByLabel(emuLabel,emuHandle)
            event.getByLabel(jetLabel,jetHandle)            
            event.getByLabel(metLabel,metHandle)
            
            dilCand=None
            istream=''
            selStep=0
            
            # prefer di-muons
            if(mumuHandle.product().size()>0):
                istream='mu'
                for dil in mumuHandle.product() :

                    lepton1=dil.leg1()
                    lepton2=dil.leg2()

                    #kinematics
                    results['mu_pt'].Fill(lepton1.pt())
                    results['mu_pt'].Fill(lepton2.pt())
                    if(lepton1.pt()<20 or lepton2.pt()<20 or abs(lepton1.eta())>2.4 or abs(lepton2.eta())>2.4) : continue
                    selStep=selStep+1

                    #trigger fiducial
                    results['mu_pt'].Fill(lepton1.eta())
                    results['mu_pt'].Fill(lepton2.eta())
                    if(abs(lepton1.eta())>2.1 and abs(lepton2.eta())>2.1) : continue
                    selStep=selStep+1

                    #id
                    results['mu_chi2'].Fill(lepton1.normalizedChi2())
                    results['mu_chi2'].Fill(lepton2.normalizedChi2())
                    if( not lepton1.isGlobal() or not lepton1.isTracker() ) : continue
                    if( not lepton2.isGlobal() or not lepton2.isTracker() ) : continue
                    if( lepton1.numberOfValidTrackerHits()<10 or lepton2.numberOfValidTrackerHits()<10 ): continue
                    if( not lepton1.muonID() or not lepton2.muonID() ) : continue
                    selStep=selStep+1

                    #isolation
                    results['mu_reliso'].Fill(lepton1.relIso())
                    results['mu_reliso'].Fill(lepton2.relIso())
                    if(lepton1.relIso()>0.25 or lepton2.relIso()>0.25) : continue
                    selStep=selStep+1
                    
                    #mass cut
                    results['mu_dilepton_mass'].Fill(dil.mass())
                    if(dil.mass()<50) : continue
                    selStep=selStep+1

                    results['mu_dilepton_sumpt'].Fill(lepton1.pt()+lepton2.pt())
                    results['mu_dilepton_pt'].Fill(dil.pt())
                    results['mu_dilepton_dphi'].Fill( abs(ROOT.TVector2.Phi_mpi_pi(lepton1.phi()-lepton2.phi())) )
                    selStep=selStep+1

                    if(dilCand is None) : dilCand=dil
                    else :
                        curCandSumPt=dilCand.leg1().pt()+dilCand.leg2().pt()
                        candSumPt=lepton1.pt()+lepton2.pt()
                        if(curCandSumPt<candSumPt) : dilCand=dil

            #revert to di-electron if none found
            if(dilCand is None and eeHandle.product().size()>0) :

                istream='e'
                for dil in eeHandle.product() :

                    lepton1=dil.leg1()
                    lepton2=dil.leg2()

                    #kinematics
                    results['e_pt'].Fill(lepton1.pt())
                    results['e_pt'].Fill(lepton2.pt())
                    if(lepton1.pt()<20 or lepton2.pt()<20 or abs(lepton1.eta())>2.4 or abs(lepton2.eta())>2.4) : continue
                    selStep=selStep+1

                    #trigger fiducial
                    results['e_pt'].Fill(lepton1.eta())
                    results['e_pt'].Fill(lepton2.eta())
                    if(abs(lepton1.eta())>2.1 and abs(lepton2.eta())>2.1) : continue
                    selStep=selStep+1

                    #id
                    results['e_sietaieta'].Fill(lepton1.scSignaIetaIeta());
                    results['e_sietaieta'].Fill(lepton2.scSignaIetaIeta());
                    results['e_eoverp'].Fill(lepton1.eSuperClusterOverP());
                    results['e_eoverp'].Fill(lepton2.eSuperClusterOverP());
                    if( lepton1.numberOfLostHits()>1): continue
                    if( lepton1.electronID()<5 or lepton2.electronID()<5 ) : continue
                    selStep=selStep+1

                    #isolation
                    results['e_reliso'].Fill(lepton1.relIso())
                    results['e_reliso'].Fill(lepton2.relIso())
                    if(lepton1.relIso()>0.25 or lepton2.relIso()>0.25) : continue
                    selStep=selStep+1
                    
                    #mass cut
                    results['e_dilepton_mass'].Fill(dil.mass())
                    if(dil.mass()<50) : continue
                    selStep=selStep+1

                    results['e_dilepton_sumpt'].Fill(lepton1.pt()+lepton2.pt())
                    results['e_dilepton_pt'].Fill(dil.pt())
                    results['e_dilepton_dphi'].Fill( abs(ROOT.TVector2.Phi_mpi_pi(lepton1.phi()-lepton2.phi())) )

                    if(dilCand is None) : dilCand=dil
                    else :
                        curCandSumPt=dilCand.leg1().pt()+dilCand.leg2().pt()
                        candSumPt=lepton1.pt()+lepton2.pt()
                        if(curCandSumPt<candSumPt) : dilCand=dil

            #cutflow up to dilepton selection
            for iselstep in xrange(0,selStep) : results[istream+'_cutflow'].Fill(iselstep)
            if(dilCand is None): continue

            #jet multiplicity bin
            njets=0
            nbjets=0
            for j in jetHandle.product() :
                if (j.pt()<30 or abs(j.eta())>2.4) : continue
                njets+=1
                btag=j.btag()
                if(btag>1.74) : nbjets+=1 #loose point
                results[istream+'_btags'].Fill(btag)
                results[istream+'_jetpt'].Fill(j.pt())
            results[istream+'_njets'].Fill(njets)
            results[istream+'_cutflow'].Fill(selStep+njets)

            #update the selection stream
            if(njets==0) :  istream = istream+'eq0jets'
            elif(njets==1) :  istream = istream+'eq1jets'
            else : istream = istream+'geq2jets'

            #basic dilepton kinematics
            lepP=[
                ROOT.TLorentzVector(lepton1.px(),lepton1.py(),lepton1.pz(),lepton1.energy()) ,
                ROOT.TLorentzVector(lepton2.px(),lepton2.py(),lepton2.pz(),lepton2.energy())
                ]
            transvLepP=[
                ROOT.TLorentzVector(lepton1.px(),lepton1.py(),0,lepton1.pt()) ,
                ROOT.TLorentzVector(lepton2.px(),lepton2.py(),0,lepton2.pt())
                ]
            recoDil=lepP[0]+lepP[1]

            #base met kinematics
            themet=metHandle.product()[0]
            recomet=ROOT.TLorentzVector(-themet.px(),-themet.py(),0,themet.pt())

            #individual lepton vs MET kinematics
            dphil2met=[
                abs(recomet.DeltaPhi(lepP[0])) ,
                abs(recomet.DeltaPhi(lepP[1]))
                ]
            mTlmet=[
                sqrt(2*recomet.Pt()*lepP[0].Pt()*(1-cos(dphil2met[0]))) ,
                sqrt(2*recomet.Pt()*lepP[1].Pt()*(1-cos(dphil2met[1])))
                ]
            dphimin=min(dphil2met)

            #transverse MET kinematics (choose the closest direction to MET)
            dphiZ2met=ROOT.TVector2.Phi_mpi_pi(recoDil.Phi()-recomet.Phi())
            transvMETscale=recomet.Pt()*sin(dphiZ2met)/recoDil.Pt()
            transvMET_1=ROOT.TVector3(recoDil.Py()*transvMETscale,-recoDil.Px()*transvMETscale,0)
            transvMET_2=ROOT.TVector3(-recoDil.Py()*transvMETscale,recoDil.Px()*transvMETscale,0)
            transvMET=ROOT.TLorentzVector(transvMET_1,transvMET_1.Pt())
            if( abs(transvMET_1.DeltaPhi(recomet.Vect())) < abs(transvMET_2.DeltaPhi(recomet.Vect())) ) :
                transvMET = ROOT.TLorentzVector(transvMET_2,transvMET_2.Pt())
  
            #individual lepton vs projected MET kinematics
            dphil2transvmet=[
                abs(transvMET.DeltaPhi(lepP[0])) ,
                abs(transvMET.DeltaPhi(lepP[1]))
                ]
            mTltransvmet=[
                sqrt(2*transvMET.Pt()*lepP[0].Pt()*(1-cos(dphil2transvmet[0]))) ,
                sqrt(2*transvMET.Pt()*lepP[1].Pt()*(1-cos(dphil2transvmet[1])))
                ]

            #fill the histograms
            results[istream+'_met'].Fill(recomet.Pt())
            results['met2'+istream+'_mindphi'].Fill(dphimin)
            results[istream+'_dilepton2met_dphi'].Fill(abs(dphiZ2met))
            results[istream+'_mT_individual'].Fill(mTlmet[0])
            results[istream+'_mT_individual'].Fill(mTlmet[1])
            results[istream+'_mT_individualsum'].Fill(mTlmet[0]+mTlmet[1])
            results[istream+'_transvmet'].Fill(transvMET.Pt())
            results[istream+'_transvmT_individual'].Fill(mTltransvmet[0])
            results[istream+'_transvmT_individual'].Fill(mTltransvmet[1])
            results[istream+'_transvmT_individualsum'].Fill(mTltransvmet[0]+mTltransvmet[1])

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
        #raw_input('Any key to continue...')
        c.SaveAs('plots/'+pname+'.png')
        c.SaveAs('plots/'+pname+'.C')

        
    
    
