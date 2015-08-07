import sys
OUTDIR='FR/closure'

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


cuts["isee"]="abs(LepGood1_pdgId) == 11 && abs(LepGood2_pdgId) == 11"
cuts["ismm"]="abs(LepGood1_pdgId) == 13 && abs(LepGood2_pdgId) == 13"
cuts["isem"]="(abs(LepGood1_pdgId) == 11 || abs(LepGood1_pdgId) == 13) && (abs(LepGood2_pdgId) == 11 || abs(LepGood2_pdgId) == 13)"
cuts["isinclflav"]="((abs(LepGood1_pdgId) == 11 || abs(LepGood1_pdgId) == 13) && (abs(LepGood2_pdgId) == 11 || abs(LepGood2_pdgId) == 13))"
cuts["pt1LT25"]="LepGood1_ConePt<25"
cuts["pt1GT25"]="LepGood1_ConePt>=25"
cuts["pt2LT25"]="LepGood2_ConePt<25"
cuts["pt2GT25"]="LepGood2_ConePt>=25"
cuts["bas0"]="nBJetMedium25==0"
cuts["bas1"]="nBJetMedium25==1"
cuts["bas2"]="nBJetMedium25==2"
cuts["bas3"]="nBJetMedium25>=3"
cuts["pt_ll"]="LepGood1_ConePt<25 && LepGood2_ConePt<25"
cuts["pt_lh"]="(LepGood1_ConePt<25 && LepGood2_ConePt>=25) || (LepGood1_ConePt>=25 && LepGood2_ConePt<25)"
cuts["pt_hh"]="LepGood1_ConePt>=25 && LepGood2_ConePt>=25"
cuts["pt_inclpt"]="1"
cuts["fakeismu"] = "( (LepGood1_mcMatchId!=0 || abs(LepGood1_pdgId)==13) && (LepGood2_mcMatchId!=0 || abs(LepGood2_pdgId)==13) )"
cuts["fakeisel"] = "( (LepGood1_mcMatchId!=0 || abs(LepGood1_pdgId)==11) && (LepGood2_mcMatchId!=0 || abs(LepGood2_pdgId)==11) )"
cuts["isfake"] = "(LepGood1_mcMatchId!=0 || abs(LepGood1_pt/LepGood_pt-1)<0.001) && (LepGood2_mcMatchId!=0 || abs(LepGood2_pt/LepGood_pt-1)<0.001)"

runs=[]
#[NAME,CUTS_TXT_FILE,SELECTION_CUTS,REMOVED_CUTS,REPLACED_CUTS,DATASETS,NUM_FOR_FR_STUDY(doeff==1 + define in sels.txt),XVAR_FOR_FR_STUDY(doeff==1 + define in xvars.txt)]
#for xvar in ["eta_pt","eta_conept","eta_jetpt"]:
for xvar in ["eta_conept"]:
    for ptreg in ["inclpt","ll","lh","hh"]:
#        for lepflav in ["inclflav","ee","em","mm"]:
#            for baselineregion in [-1,0,1,2,3]:
#    for ptreg in ["inclpt"]:
#        for lepflav in ["inclflav"]:
#        for lepflav in ["inclflav","ee","em","mm"]:
        lepflav="inclflav"
        for w in ['mu','el']:
            for baselineregion in [-1]:
                app=[]
                app.append("isfake")
                app.append("is"+lepflav)
                app.append("pt_"+ptreg)
                app.append("fakeis"+w)
                br="_incl"
                if baselineregion >= 0:
                    app.append("bas%d" % (baselineregion,))
                    br="_b%d" % (baselineregion,)
#                runs.append(["Application_"+xvar+"_"+lepflav+"_"+ptreg+br+"_"+w,"susy-multilepton/fake_rate/apply/susy_2lss_fake_rate_applreg.txt",app,[],[],"-p TT,TT_red_FO1_%s,TT_red_FO2_%s,TT_red_FO3_%s,TT_red_FO4_%s,TT_red_FO1_%s_insitu,TT_red_FO2_%s_insitu,TT_red_FO3_%s_insitu,TT_red_FO4_%s_insitu" % (xvar,xvar,xvar,xvar,xvar,xvar,xvar,xvar)])
                if w=='mu':
                    runs.append(["Application_"+xvar+"_"+lepflav+"_"+ptreg+br+"_"+w,"susy-multilepton/fake_rate/apply/susy_2lss_fake_rate_applreg.txt",app,[],[],"-p TT,TT_red_FO1_%s,TT_red_FO4_%s_insitu" % (xvar,xvar)])
                else:
                    runs.append(["Application_"+xvar+"_"+lepflav+"_"+ptreg+br+"_"+w,"susy-multilepton/fake_rate/apply/susy_2lss_fake_rate_applreg.txt",app,[],[],"-p TT,TT_red_FO2_%s,TT_red_FO4_%s_insitu" % (xvar,xvar)])

isplot = 'table' not in sys.argv[1:]

for run in runs:
    PATH="-P /data1/p/peruzzi/TREES_72X_210515_MiniIsoRelaxDxy -F sf/t {P}/1_lepJetReClean_Susy_v4/evVarFriend_{cname}.root -F sf/t {P}/3_QCDVarsSusy_FakeRateFO_v9/evVarFriend_{cname}.root --mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_looseveto_lepchoice.txt %s"
    MYPATH = PATH % ("--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_coneptchoice.txt" if "conept" in run[0] else "--mcc susy-multilepton/fake_rate/susy_2lss_fake_rate_defaultptchoice.txt")
    RUN="python %s -j 8 -l 0.01 --s2v --tree treeProducerSusyMultilepton" % ("mcPlots.py -e -f --plotmode nostack --print 'pdf'" if isplot else "mcAnalysis.py")
    B0=' '.join([RUN,MYPATH,"susy-multilepton/fake_rate/susy_2lss_fake_rate_mca_sync.txt",run[1],"susy-multilepton/fake_rate/apply/susy_2lss_fake_rate_plots.txt" if isplot else ""])
    B0 += ' '.join([' ',add_cuts(prepare_cuts(run[2],run[3],run[4])),"--pdir "+OUTDIR+'_'+run[0] if isplot else "",run[5]])
    print B0
