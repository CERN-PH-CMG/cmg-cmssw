import sys,os
import commands
from math import sqrt,pow
import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import setStyle, getNewCanvas, formatPlot, formatForCmsPublic


"""
loops over files in directory and fills control histograms
"""
def getControlPlots(url) :

    if( url.find('HToZZTo2L2Nu')<0): return None

    from DataFormats.FWLite import Events, Handle
    ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
    from genLevelUtils import getDecayTreeFrom, getDecayCounters

    #objects of interest

    #gen level
    hgenCandHandle  = Handle ('std::vector<reco::GenParticle>')
    hgenCandLabel = ('genHiggsFinalState')

    #reco level
    hcandHandle  = Handle ('std::vector<cmg::DiObject<cmg::DiObject<cmg::Muon,cmg::Muon>,cmg::BaseMET> >')
    hcandLabel = ('cmgDiMuonPlusMET')
       
    #book histograms
    results={}
    results['muon_pt']         = formatPlot( ROOT.TH1F("muon_pt", "reco level;p_{T} [GeV/c]; Muons", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['leadmuon_pt']     = formatPlot( ROOT.TH1F("leadmuon_pt", "reco level;p_{T} [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['muon_eta']        = formatPlot( ROOT.TH1F("muon_eta", "reco level;#eta; Muons", 100, -3.,3.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['leadmuon_eta']    = formatPlot( ROOT.TH1F("leadmuon_eta", "reco level;#eta; Events", 100, -3.,3.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['genmuon_pt']      = formatPlot( ROOT.TH1F("genmuon_pt", "gen level;p_{T} [GeV/c]; Muons", 100, 0.,300.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['genleadmuon_pt']  = formatPlot( ROOT.TH1F("genleadmuon_pt", "gen level;p_{T} [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['genmuon_eta']     = formatPlot( ROOT.TH1F("genmuon_eta", "reco level;#eta; Muons", 100, -3.,3.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['genleadmuon_eta'] = formatPlot( ROOT.TH1F("genleadmuon_eta", "reco level;#eta; Events", 100, -3.,3.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['dimuon_pt']       = formatPlot( ROOT.TH1F("dimuon_pt", "reco level;|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_eta']      = formatPlot( ROOT.TH1F("dimuon_eta", "reco level;#eta; Events", 100, -3.,3.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_dphi'] = formatPlot( ROOT.TH1F("dimuon_dphi", "reco level;#Delta #phi; Events", 100, -3.2,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['gendimuon_pt']    = formatPlot( ROOT.TH1F("gendimuon_pt", "gen level;|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['gendimuon_eta']   = formatPlot( ROOT.TH1F("gendimuon_eta", "gen level;#eta; Events", 100, -3.,3.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['gendimuon_dphi'] = formatPlot( ROOT.TH1F("gendimuon_dphi", "gen level;#Delta #phi; Events", 100, -3.2,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['met']             = formatPlot( ROOT.TH1F("met", "pfMET;#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['genmet']          = formatPlot( ROOT.TH1F("genmet", "#Sigma_{#nu} #vec{p}_{T};#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['st']             = formatPlot( ROOT.TH1F("st", "reco level;S_{T} [GeV/c]; Events", 100,  0.,1000.), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['genst']          = formatPlot( ROOT.TH1F("genst", "gen level;S_{T} [GeV/c]; Events", 100,  0.,1000.), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['dimuon2met_dphi']    = formatPlot( ROOT.TH1F("dimuon2met_dphi", "reco level;#Delta #phi; Events", 100, -3.2,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['gendimuon2met_dphi'] = formatPlot( ROOT.TH1F("gendimuon2met_dphi", "gen level;#Delta #phi; Events", 100, -3.2,3.2), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['dimuon_pt_res']   = formatPlot( ROOT.TH1F("dimuon_pt_res","Reco-Gen;|#vec{p}_{T}^{RECO}|-|#vec{p}_{T}^{GEN}|; Dimuons",100,-50,50), 1, 1, 1, 20, 3001, True, True, 1,1,1)
    results['dimuon_eta_res']  = formatPlot( ROOT.TH1F("dimuon_eta_res","Reco-Gen;#eta^{RECO}-#eta^{GEN}; Dimuons",100,-0.5,0.5), 1, 1, 1, 20, 3001, True, True, 1,1,1)
    results['met_res']         = formatPlot( ROOT.TH1F("met_res","Reco-Gen;#slash{E}_{T}^{RECO}-#Sigma_{#nu}#vec{p}_{T}^{GEN} [rad]; Events",100,-50,50), 1, 1, 1, 20, 3001, True, True, 1,1,1)
    results['met_phi_res']     = formatPlot( ROOT.TH1F("met_phi_res","Reco-Gen;#slash{#phi}^{RECO}-#slash#phi^{GEN}| [rad]; Events",100,-3.2,3.2), 1, 1, 1, 20, 3001, True, True, 1,1,1)
    results['hmT']             = formatPlot( ROOT.TH1F("hmT","H;Transverse mass [GeV/c^{2}]; Events",100,0,1000), 2, 1, 2, 20, 0, True, True, 1,1,1)
    results['hpz']             = formatPlot( ROOT.TH1F("hpz","H;Longitudinal balance [GeV/c]; Events",100,-1000,1000), 2, 1, 2, 20, 0, True, True, 1,1,1)
    results['mT']              = formatPlot( ROOT.TH1F("mT","reco level;Transverse mass [GeV/c^{2}]; Events",100,0,1000), 1, 1, 1, 20, 0, True, True, 1,1,1)
    results['genmT']           = formatPlot( ROOT.TH1F("genmT","gen level;Transverse mass [GeV/c^{2}]; Events",100,0,1000), 1, 1, 1, 1, 0, True, True, 1,1,1)
    results['mT_res']          = formatPlot( ROOT.TH1F("mT_res","Reco-Gen;#Delta Transverse mass [GeV/c^{2}]; Events",100,-100,100), 1, 1, 1, 20, 3001, True, True, 1,1,1)
   
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
    print dirList
    for fname in dirList:
        if(fname.find(".root")<0) : continue

        absUrl=url + '/' + fname
        if(url.find('castor')>0) : absUrl = 'rfio://' + url + '/' + fname
        file = ROOT.TFile.Open(absUrl)
        if(file.IsZombie()):
            file.Close()
            continue
        events = Events (file)
        runInfo = ROOT.getRunInfo(file)
        
        # loop over events
        for event in events:
            
            #reco level
            event.getByLabel(hcandLabel,hcandHandle)

            if(hcandHandle.product().size()==0): continue

            #fill the histos
            muP=[]
            recodimu=ROOT.TLorentzVector(0,0,0,0)
            recomet=ROOT.TLorentzVector(0,0,0,0)
            for hcand in hcandHandle.product() :
                mu=hcand.leg1().leg1()
                muP.append(ROOT.TLorentzVector(mu.px(),mu.py(),mu.pz(),mu.energy()))
                mu=hcand.leg1().leg2()
                muP.append(ROOT.TLorentzVector(mu.px(),mu.py(),mu.pz(),mu.energy()))
                recodimu=muP[0]+muP[1]
                results['muon_pt'].Fill( muP[0].Pt() )
                results['muon_pt'].Fill( muP[1].Pt() )
                results['muon_eta'].Fill( muP[0].Eta() )
                results['muon_eta'].Fill( muP[1].Eta() )
                results['leadmuon_pt'].Fill( muP[0].Pt() )
                results['leadmuon_eta'].Fill( muP[0].Eta() )
                results['dimuon_pt'].Fill( hcand.leg1().pt() )
                results['dimuon_eta'].Fill( hcand.leg1().eta() )
                results['dimuon_dphi'].Fill( muP[0].DeltaPhi(muP[1]) )
                themet=hcand.leg2()
                results['met'].Fill( themet.pt() )
                recomet=ROOT.TLorentzVector(-themet.px(),-themet.py(),-themet.pz(),themet.energy())
                results['dimuon2met_dphi'].Fill( recodimu.DeltaPhi(recomet) )
                results['st'].Fill(muP[0].Pt()+muP[1].Pt()+recomet.Pt())
                break

            transverseSum=recodimu+recomet
            mT=pow(sqrt(pow(recodimu.Pt(),2)+pow(recodimu.M(),2))+sqrt(pow(recomet.Pt(),2)+pow(recodimu.M(),2)),2)
            mT-=pow(transverseSum.Pt(),2)
            mT=sqrt(mT)
            results['mT'].Fill(mT)
            
            #gen level
            event.getByLabel(hgenCandLabel,hgenCandHandle)
            mothers,daughters,finalstates=getDecayTreeFrom(hgenCandHandle,25)
            nmuons,nelecs,ntaus,nneutrinos,njets=getDecayCounters(mothers,daughters,finalstates)
            genmuP=[]
            gennuP=[]
            leadgenmupt=ROOT.TLorentzVector(0,0,0,0)
            for fs in finalstates:
                for dec in fs :
                    for p in dec :
                        pdgid=abs(p.pdgId())
                        if(pdgid == 13 ) :
                            genmuP.append(ROOT.TLorentzVector(p.px(),p.py(),p.pz(),p.energy()))
                            if(p.pt()>leadgenmupt.Pt()) : leadgenmupt=ROOT.TLorentzVector(p.px(),p.py(),p.pz(),p.energy())
                        if(pdgid == 12 or pdgid==14 or pdgid==16 ) :
                            gennuP.append(ROOT.TLorentzVector(p.px(),p.py(),p.pz(),p.energy()))

            #MC truth control
            if(len(genmuP)>1 and len(gennuP)>1 ):
                dimu=genmuP[0]+genmuP[1]
                sumnu=gennuP[0]+gennuP[1]
                results['genmuon_pt'].Fill(genmuP[0].Pt())
                results['genmuon_pt'].Fill(genmuP[1].Pt())
                results['genmuon_eta'].Fill(genmuP[0].Eta())
                results['genmuon_eta'].Fill(genmuP[1].Eta())
                results['genleadmuon_pt'].Fill(leadgenmupt.Pt())
                results['genleadmuon_eta'].Fill(leadgenmupt.Eta())
                results['gendimuon_dphi'].Fill(genmuP[0].DeltaPhi(genmuP[1]))
                results['gendimuon_pt'].Fill(dimu.Pt())
                results['gendimuon_eta'].Fill(dimu.Eta())
                results['genmet'].Fill(sumnu.Pt())
                results['genst'].Fill(genmuP[0].Pt()+genmuP[1].Pt()+sumnu.Pt())
                results['gendimuon2met_dphi'].Fill(dimu.DeltaPhi(sumnu))

                #resolutions
                results['dimuon_pt_res'].Fill(recodimu.Pt()-dimu.Pt())
                results['dimuon_eta_res'].Fill(recodimu.Eta()-dimu.Eta())
                results['met_res'].Fill(recomet.Pt()-sumnu.Pt())
                results['met_phi_res'].Fill(recomet.DeltaPhi(sumnu))

                genTransverseSum=dimu+sumnu
                genmT=pow(sqrt(pow(dimu.Pt(),2)+pow(dimu.M(),2))+sqrt(pow(sumnu.Pt(),2)+pow(dimu.M(),2)),2)
                genmT-=pow(genTransverseSum.Pt(),2)
                genmT=sqrt(genmT)
                results['genmT'].Fill(genmT)
                results['mT_res'].Fill(mT-genmT)

                theX = ROOT.TLorentzVector(mothers[0].px(),mothers[0].py(),mothers[0].pz(),mothers[0].energy())
                theXdaughter1=ROOT.TLorentzVector(daughters[0][0].px(),daughters[0][0].py(),daughters[0][0].pz(),daughters[0][0].energy())
                theXdaughter2=ROOT.TLorentzVector(daughters[0][1].px(),daughters[0][1].py(),daughters[0][1].pz(),daughters[0][1].energy())
                xmt=sqrt(pow(theXdaughter1.Et()+theXdaughter2.Et(),2)-pow(theX.Pt(),2))
                results['hmT'].Fill(xmt)
                results['hpz'].Fill(theX.Pz())

        file.Close()
                
    return results

"""
shows the control plos
"""
def showControlPlots(stackplots,allspimposeplots,dataplots,generalLabel,outdir='plots') :

    genList={}
    recoList={}
    for p in allspimposeplots :
        if(p==None):continue
        if(len(p)==0): continue
        for ip in p:
            ipname=ip.GetName()
            if(ipname.find('gen')==0) : genList[ipname]=ip
            else : recoList[ipname]=ip
    
    setStyle()
    c = getNewCanvas("genlevelc","genlevelc",False)
    c.SetWindowSize(600,600)
    for p in recoList.items() :
        c.Clear()
        ipname = p[0]
        recoopt=''
        np=0
        try :
            genipname = 'gen'+p[0]
            genList[genipname].SetTitle("gen level")
            genList[genipname].DrawNormalized('hist')
            recoopt += 'same'
            np+=1
        except KeyError:
            recoopt='e1'
        p[1].SetTitle("reco level")
        p[1].SetMarkerStyle(20)
        p[1].DrawNormalized(recoopt)
        np+=1
        formatForCmsPublic(c,0,'CMS simulation',np)
        c.Modified()
        c.Update()
        raw_input('Any key to continue...')
        c.SaveAs(outdir+'/'+ipname+'.png')
        
