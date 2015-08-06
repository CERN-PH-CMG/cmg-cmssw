import sys
doeff = ("eff" in sys.argv[1:])
dotable = ("table" in sys.argv[1:])
dodump = ("dump" in sys.argv[1:])
doplot = ("plot" in sys.argv[1:])

PATH="-P /data1/p/peruzzi/TREES_72X_070615_MiniIso -F sf/t {P}/3_QCDVarsSusy_FakeRateFO_v10/evVarFriend_{cname}.root %s --mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_looseveto_lepchoice.txt %s"
#PATH="-P /data1/p/peruzzi/TREES_72X_070615_MiniIsoRelaxDxy  -F sf/t {P}/3_QCDVarsSusy_FakeRateFO_v10_dxy/evVarFriend_{cname}.root %s --mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_looseveto_lepchoice.txt %s"
FTREEQCD=" "
FTREETT="  "
OUTDIR='FR/derive'

cuts={}
def add_cuts(mylist):
    my = [x for (i,x) in enumerate(mylist) if x not in mylist[:i]]
    return ' '.join("-A alwaystrue "+cut+" '"+cuts[cut]+"'" for cut in my)
def remove_cuts(mylist):
    my = [x for (i,x) in enumerate(mylist) if x not in mylist[:i]]
    return ' '.join("-X "+cut for cut in my)
def replace_cuts(mylist):
    my = [x for (i,x) in enumerate(mylist) if x not in mylist[:i]]
    return ' '.join("-R "+cut+" "+newcut+" '"+cuts[newcut]+"'" for cut,newcut in mylist)
def prepare_cuts(add,remove,replace):
    my = [x for (i,x) in enumerate(add) if x not in add[:i]]
    my = [i for i in my if i not in remove]
    for k,l in replace:
        my = [l if k==x else x for x in my]
        my = [x for (i,x) in enumerate(my) if x not in my[:i]]
    return my


cuts["anylep"]="abs(LepGood_pdgId) > 0"
cuts["minireliso04"]="LepGood_miniRelIso<0.4"
cuts["dxy005"]="abs(LepGood_dxy)<0.05"
cuts["dz01"]="abs(LepGood_dz)<0.1"
cuts["sipLT4"]="LepGood_sip3d<4"
cuts["sipGT4"]="LepGood_sip3d>4"
cuts["pt5"]="LepGood_ConePt > 5"
cuts["pt7"]="LepGood_ConePt > 7"
cuts["pt10"]="LepGood_ConePt > 10"
cuts["etaLT2p4"]="abs(LepGood_eta) < 2.4"
cuts["etaLT2p5"]="abs(LepGood_eta) < 2.5"
cuts["etagap"]="abs(LepGood_pdgId)!=11 || abs(LepGood_eta)<1.4442 || abs(LepGood_eta)>1.566"
cuts["ismu"]="abs(LepGood_pdgId)==13"
cuts["isel"]="abs(LepGood_pdgId)==11"
cuts["muMediumID"]="LepGood_mediumMuonId > 0"
cuts["elMVAloose"]="LepGood_mvaIdPhys14 > -0.11+(-0.35+0.11)*(abs(LepGood_eta)>0.8)+(-0.55+0.35)*(abs(LepGood_eta)>1.479)"
cuts["elMVAtight"]="LepGood_mvaIdPhys14 > 0.73+(0.57-0.73)*(abs(LepGood_eta)>0.8)+(+0.05-0.57)*(abs(LepGood_eta)>1.479)"
cuts["losthitsLEQ1"]="LepGood_lostHits<=1"
cuts["losthitsEQ0"]="LepGood_lostHits==0"
cuts["elConvVeto"]="LepGood_convVeto"
cuts["tightcharge"]="LepGood_tightCharge > (abs(LepGood_pdgId) == 11)"
cuts["multiiso"]="multiIso_multiWP(LepGood_pdgId,LepGood_pt,LepGood_eta,LepGood_miniRelIso,LepGood_jetPtRatio,LepGood_jetPtRel,2) > 0"
cuts["multiiso_relaxed_miniiso"]="multiIso_multiWP(LepGood_pdgId,LepGood_pt,LepGood_eta,(LepGood_miniRelIso>=0.4),LepGood_jetPtRatio,LepGood_jetPtRel,2) > 0"
cuts["multiiso_relaxed_miniiso_conept"]="multiIso_multiWP(LepGood_pdgId,LepGood_pt,LepGood_eta,(LepGood_miniRelIso>=0.4),LepGood_jetPtRatio*LepGood_CorrConePt/LepGood_pt,LepGood_jetPtRel,2) > 0"
cuts["multiIso_singleWP_relaxFO4"]="multiIso_singleWP_relaxFO4(LepGood_pdgId,LepGood_pt,LepGood_eta,LepGood_miniRelIso,LepGood_jetPtRatio,LepGood_jetPtRel,2) > 0"
cuts["nFO1is1"]="nLepGood_FO1==1"
cuts["nFO2is1"]="nLepGood_FO2==1"
cuts["nFO3is1"]="nLepGood_FO3==1"
cuts["nFO4is1"]="nLepGood_FO4==1"
cuts["nFO1InSituis1"]="nLepGood_FO1InSitu==1"
cuts["nFO2InSituis1"]="nLepGood_FO2InSitu==1"
cuts["nFO3InSituis1"]="nLepGood_FO3InSitu==1"
cuts["nFO4InSituis1"]="nLepGood_FO4InSitu==1"
cuts["isfake"]="LepGood_mcMatchId==0"

LooseLepSel=["isfake","minireliso04","dxy005","dz01"]
LooseMuSel=LooseLepSel+["ismu","pt5","etaLT2p4"]
LooseElSel=LooseLepSel+["isel","pt7","pt10","etaLT2p5","etagap","elMVAloose","elConvVeto","losthitsLEQ1","losthitsEQ0"]

TightLepSel=["sipLT4","dz01","multiiso"]
TightMuSel=LooseMuSel+TightLepSel+["pt10","etaLT2p4","muMediumID","tightcharge"]
TightElSel=LooseElSel+TightLepSel+["pt10","etaLT2p5","elMVAtight","elConvVeto","tightcharge","losthitsEQ0"]


MuDsetsQCD=' -p QCD_Mu '
ElDsetsQCD=' -p QCD_El '
MuDsetsInSitu=' -p TT '
ElDsetsInSitu=' -p TT '



runs=[]
#[NAME,CUTS_TXT_FILE,SELECTION_CUTS,REMOVED_CUTS,REPLACED_CUTS,DATASETS,NUM_FOR_FR_STUDY(doeff==1 + define in sels.txt),XVAR_FOR_FR_STUDY(doeff==1 + define in xvars.txt)]

#runs.append(["FO1Mu"+"_"+"eta_conept","susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightMuSel+["nFO1is1"],[],[("multiiso","minireliso04")],MuDsetsQCD,"multiiso","eta_conept"])
#runs.append(["FO1MuNUM"+"_"+"eta_conept","susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightMuSel+["nFO1is1"],[],[],MuDsetsQCD,"multiiso","eta_conept"])

for xvar in ["eta_conept"]:
#for xvar in ["eta_pt"]:
#for xvar in ["eta_pt","eta_conept","eta_jetpt"]:
    True
    runs.append(["FO1Mu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightMuSel+["nFO1is1"],[],[("multiiso","minireliso04")]," -p QCD_Mu -p TT_derive -p WJets_derive ","multiiso",xvar])
    runs.append(["FO2El"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightElSel+["nFO2is1"],[],[("multiiso","minireliso04"),("elMVAtight","elMVAloose")]," -p QCD_El -p TT_derive -p WJets_derive ","multiiso_AND_elMVAtight",xvar])

#    runs.append(["FO1Mu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightMuSel+["nFO1is1"],[],[("multiiso","minireliso04")],MuDsetsQCD,"multiiso",xvar])
#    runs.append(["FO2El"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightElSel+["nFO2is1"],[],[("multiiso","minireliso04"),("elMVAtight","elMVAloose")],ElDsetsQCD,"multiiso_AND_elMVAtight",xvar])
#    runs.append(["FO4MuInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightMuSel+["nFO4InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","multiIso_singleWP_relaxFO4")],MuDsetsInSitu,"multiiso",xvar])
#    runs.append(["FO4ElInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightElSel+["nFO4InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","multiIso_singleWP_relaxFO4")],ElDsetsInSitu,"multiiso",xvar])

#    runs.append(["FO3Mu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightMuSel+["nFO3is1"],[],[("multiiso","multiiso_relaxed_miniiso_conept")],MuDsetsQCD,"multiiso",xvar])
#    runs.append(["FO3El"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightElSel+["nFO3is1"],[],[("multiiso","multiiso_relaxed_miniiso_conept")],ElDsetsQCD,"multiiso",xvar])
#    runs.append(["FO4Mu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightMuSel+["nFO4is1"],[],[("multiiso","multiIso_singleWP_relaxFO4")],MuDsetsQCD,"multiiso",xvar])
#    runs.append(["FO4El"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_perlep.txt",TightElSel+["nFO4is1"],[],[("multiiso","multiIso_singleWP_relaxFO4")],ElDsetsQCD,"multiiso",xvar])
#
#    runs.append(["FO1MuInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightMuSel+["nFO1InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","minireliso04")],MuDsetsInSitu,"multiiso",xvar])
#    runs.append(["FO1ElInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightElSel+["nFO1InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","minireliso04")],ElDsetsInSitu,"multiiso",xvar])
#    runs.append(["FO2ElInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightElSel+["nFO2InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","minireliso04"),("elMVAtight","elMVAloose")],ElDsetsInSitu,"multiiso_AND_elMVAtight",xvar])
#    runs.append(["FO3MuInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightMuSel+["nFO3InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","multiiso_relaxed_miniiso_conept")],MuDsetsInSitu,"multiiso",xvar])
#    runs.append(["FO3ElInSitu"+"_"+xvar,"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_insitu_measreg.txt",TightElSel+["nFO3InSituis1"],["dxy005"],[("sipLT4","sipGT4"),("multiiso","multiiso_relaxed_miniiso_conept")],ElDsetsInSitu,"multiiso",xvar])


if (dotable or dodump):
    for run in runs:
        RUN="python %s -l 1.0 --s2v --tree treeProducerSusyMultilepton" % ("mcAnalysis.py" if dotable else "mcDump.py")
        MYPATH=PATH
        MYPATH = MYPATH % (FTREEQCD if "QCD" in run[5] else FTREETT,"--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_coneptchoice.txt" if "conept" in run[0] else "--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_defaultptchoice.txt")
        B0=' '.join(['echo',run[0],';',RUN,MYPATH,"susy-multilepton/fake_rate/susy_2lss_fake_rate_mca_sync.txt",run[1]])
        run[0]=run[6]+'_ON_'+run[0]
        B0 += ' '.join([' ',add_cuts(prepare_cuts(run[2],run[3],run[4])),run[5]])
        if dodump:
            B0 += ' '+str(sys.argv[2])
        print B0

if doplot:
    for run in runs:
        RUN="python mcPlots.py -f -e --print pdf --plotmode nostack -l 1.0 --s2v --tree treeProducerSusyMultilepton"
        MYPATH=PATH
        MYPATH = MYPATH % (FTREEQCD if "QCD" in run[5] else FTREETT,"--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_coneptchoice.txt" if "conept" in run[0] else "--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_defaultptchoice.txt")
        B0=' '.join(['echo',run[0],';',RUN,MYPATH,"susy-multilepton/fake_rate/susy_2lss_fake_rate_mca_sync.txt",run[1],"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_plots_debug_sync.txt"])
        run[0]=run[6]+'_ON_'+run[0]
        B0 += ' '.join([' ',add_cuts(prepare_cuts(run[2],run[3],run[4])),run[5],"--pdir "+OUTDIR+'_'+run[0]])
        print B0

if doeff:
    for run in runs:
#        RUN="python mcEfficiencies.py --s2v -l 1.0 --tree treeProducerSusyMultilepton"
        RUN="python mcEfficiencies.py --yrange 0 0.4 --s2v -l 1.0 --tree treeProducerSusyMultilepton --normEffUncToLumi"
        MYPATH=PATH
        MYPATH = MYPATH % (FTREEQCD if "QCD" in run[5] else FTREETT,"--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_coneptchoice.txt" if "conept" in run[0] else "--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_defaultptchoice.txt")
        B0=' '.join([RUN,MYPATH,"susy-multilepton/fake_rate/susy_2lss_fake_rate_mca_sync.txt",run[1],"susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_sels_sync.txt","susy-multilepton/fake_rate/derive/susy_2lss_fake_rate_xvars_sync.txt"])
        run[0]=run[6]+'_ON_'+run[0]
        B0 += ' '.join([' ',"--legend=TL --ytitle 'Fake rate' --groupBy cut",'--sP '+run[6],"-o "+OUTDIR+'_'+run[0]+"/plots.root",' --sP '+run[7]])
        B0 += ' '.join([' ',add_cuts(prepare_cuts(run[2],run[3],run[4])),run[5]])
        print B0

