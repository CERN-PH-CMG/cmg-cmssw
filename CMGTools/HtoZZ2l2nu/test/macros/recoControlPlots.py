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
    
    #objects of interest 
    evHandle  = Handle ('std::vector<pat::EventHypothesis>')
    evLabel = ('cleanEvent','selectedEvent')

    vertexHandle  = Handle ('std::vector<reco::Vertex>')
    vertexLabel = ('cleanEvent','selectedVertices')

    selInfo = Handle ('std::vector<int>')
    selInfoLabel = ('cleanEvent','selectionInfo')

    results={}
    streams=['ee','mumu','emu']
    jetmult=['eq0jets','eq1jets','geq2jets']
    for istream in streams :

        cat=istream

        #dilepton control
        results[cat+"_dilepton_mass"]     = formatPlot( ROOT.TH1F(cat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+"_dilepton_sumpt"]    = formatPlot( ROOT.TH1F(cat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
        results[cat+"_dilepton_pt"]       = formatPlot( ROOT.TH1F(cat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)

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
        results[cat+"_met"]               = formatPlot( ROOT.TH1F(cat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)

        #split the analysis according to the jet multiplicity
        for jstream in jetmult :
            subcat=istream+jstream
            results[subcat+"_dilepton_mass"]     = formatPlot( ROOT.TH1F(subcat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton_sumpt"]    = formatPlot( ROOT.TH1F(subcat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton_pt"]       = formatPlot( ROOT.TH1F(subcat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results["met2"+subcat+"_mindphi"]    = formatPlot( ROOT.TH1F("met2"+subcat+"_mindphi", ";min #Delta #phi(#slash{E}_{T},l) [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_met"]               = formatPlot( ROOT.TH1F(subcat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_dilepton2met_dphi"] = formatPlot( ROOT.TH1F(subcat+"dilepton2met_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_relmet"]            = formatPlot( ROOT.TH1F(subcat+"_relmet", ";#slash{E}_{T,rel} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT"]                = formatPlot( ROOT.TH1F(subcat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",100,0,1000), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT_individual"]     = formatPlot( ROOT.TH1F(subcat+"_mT_individual",";Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT_corr"]           = formatPlot( ROOT.TH2F(subcat+"_mT_corr",";Transverse mass(leading lepton,MET) [GeV/c^{2}];Transverse mass(trailer lepton,MET) [GeV/c^{2}]; Events",50,0,500,50,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)
            results[subcat+"_mT_individualsum"]  = formatPlot( ROOT.TH1F(subcat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500), 1, 1, 1, 20, 0, True, True, 1,1,1)

    #build the list of files
    import os
    dirList=[url]
    if(url.find('castor')>0):
        sc, flist = commands.getstatusoutput('nsls %s' % (url,) )
        flist = flist.split()
        for f in flist:
            dirList.append(f)
    else :
        dirList=os.listdir(url)

    #loop over results in files
    for fname in dirList:
        if(fname.find("root")<0) : continue
        absUrl='file:'+ url + '/' + fname
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
            event.getByLabel(evLabel,evHandle)
            event.getByLabel(vertexLabel,vertexHandle)
            event.getByLabel(selInfoLabel,selInfo)

            selPath = selInfo.product()[0]
            selStep = selInfo.product()[1]

            print str(selPath) + ' ' + str(selStep)
            
            #cutflow up to dilepton selection
            if(selPath==0 or selStep<3) : continue
            istream='mumu'
            if(selPath==2) :  istream='ee'
            if(selPath==3) :  istream='emu'

            evhyp = evHandle.product()[0]

            #jet multiplicity bin
            njets=0
            nbjets=0
            seljets= evhyp.all("jet")
            njets = seljets.size()
            for j in seljets :
                btag=j.bDiscriminator('trackCountingHighEffBJetTags')
                if(btag>1.74) : nbjets+=1 #loose point
                results[istream+'_btags'].Fill(btag)
                results[istream+'_jetpt'].Fill(j.pt())
            results[istream+'_njets'].Fill(njets)
            results[istream+'_bmult'].Fill(nbjets)
            jetbin=njets
            if(jetbin>2):jetbin=2

            #update the selection stream
            substream=istream
            if(njets==0) :    substream = substream+'eq0jets'
            elif(njets==1) :  substream = substream+'eq1jets'
            else :            substream = substream+'geq2jets'

            #basic dilepton kinematics
            lepton1 = evhyp["leg1"]
            lepton2 = evhyp["leg2"]
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
            themet=evhyp["met"]
            recomet=ROOT.TLorentzVector(-themet.px(),-themet.py(),0,themet.pt())
            results[istream+'_met'].Fill(recomet.Pt())
            
            #require MET back-to-back to dilepton
            dphiZ2met=ROOT.TVector2.Phi_mpi_pi(recoDil.Phi()-recomet.Phi())
            results[substream+'_met'].Fill(recomet.Pt())
            results[substream+'_dilepton2met_dphi'].Fill(abs(dphiZ2met))
            if(abs(dphiZ2met)<math.pi/2): continue
            
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
            relMET=recomet.Pt()
            if(abs(dphimin)<math.pi/2) : relMET = relMET*math.sin(dphimin)

            #dilepton vs MET kinematics
            transvSum=recoDil+recomet
            transverseMass=pow(sqrt(pow(recoDil.Pt(),2)+pow(recoDil.M(),2))+sqrt(pow(recomet.Pt(),2)+pow(recoDil.M(),2)),2)
            transverseMass-=pow(transvSum.Pt(),2)
            transverseMass=sqrt(transverseMass)
            
            #control histograms
            results['met2'+substream+'_mindphi'].Fill(dphimin)
            results[substream+"_relmet"].Fill(relMET)
            results[substream+'_mT_individual'].Fill(mTlmet[0])
            results[substream+'_mT_individual'].Fill(mTlmet[1])
            if(lepP[0].Pt()>lepP[1].Pt()): results[substream+'_mT_corr'].Fill(mTlmet[0],mTlmet[1])
            else : results[substream+'_mT_corr'].Fill(mTlmet[1],mTlmet[0])
            results[substream+'_mT_individualsum'].Fill(mTlmet[0]+mTlmet[1])
            results[substream+"_dilepton_mass"].Fill(recoDil.M())
            results[substream+"_dilepton_sumpt"].Fill(lepP[0].Pt()+lepP[1].Pt())
            results[substream+"_dilepton_pt"].Fill(recoDil.Pt())
            results[substream+"_mT"].Fill(transverseMass)

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
            if( p[i].IsA().InheritsFrom("TH1") ):
                if(p[i].GetEntries()==0) : continue
            stackLists[i].Add(p[i])
            
    spimposeLists=[]
    for p in spimposeplots:
        for i in xrange(0,len(p)):
            if(len(spimposeLists)<=i):
                newlist=ROOT.TList()
                spimposeLists.append(newlist)
            if( p[i].IsA().InheritsFrom("TH1") ):
                if(p[i].GetEntries()==0) : continue
            spimposeLists[i].Add(p[i])

    dataLists=[]
    for p in dataplots:
        for i in xrange(0,len(p)):
            if(len(dataLists)<=i):
                newlist=ROOT.TList()
                dataLists.append(newlist)
            if( p[i].IsA().InheritsFrom("TH1") ):
                print p[i].GetEntries()
                if(p[i].GetEntries()==0) : continue
            dataLists[i].Add(p[i])

    #draw result
    nplots=max([len(stackLists), len(dataLists), len(spimposeLists)])
    if(nplots==0) : return

    #output dir
    import getpass
    outdir='/tmp/'+getpass.getuser()+'/'
      
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
        formatForCmsPublic(c,leg,generalLabel,5)
        pname=''
        if(stack.At(0) is not None) :      pname=stack.At(0).GetName()
        elif(spimpose.At(0) is not None) : pname=spimpose.At(0).GetName()
        elif(data.At(0) is not None) :     pname=data.At(0).GetName()
        if(len(pname)<=0): continue
        c.SaveAs(outdir+'/'+pname+'.png')
        c.SaveAs(outdir+'/'+pname+'.C')
        #raw_input('Any key to continue...')

        
    
    
