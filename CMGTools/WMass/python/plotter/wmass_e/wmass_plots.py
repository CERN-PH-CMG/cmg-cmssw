#!/usr/bin/env python
# may use as: cat wmass_e/zee_catlist.txt | xargs -i python wmass_e/wmass_plots.py plots/testZskim {} > runplots.sh
import sys
import re
import os

ODIR=sys.argv[1]

dowhat = "plots" 
#dowhat = "dumps" 
#dowhat = "yields" 

TREES = "-F mjvars/t '{P}/friends/evVarFriend_{cname}.root' --FMC sf/t '{P}/friends/sfFriend_{cname}.root' "
TREESONLYSKIMW = "-P /data1/emanuele/wmass/TREES_1LEP_53X_V2_WSKIM_V3/"
TREESONLYSKIMZ = "-P /data1/emanuele/wmass/TREES_1LEP_53X_V2_ZEESKIM_V3/"
TREESONLYFULL = "-P /data1/emanuele/wmass/TREES_1LEP_53X_V2"

def base(selection):

    if selection=='wenu': TREESONLYSKIM=TREESONLYSKIMW
    elif selection=='zee': TREESONLYSKIM=TREESONLYSKIMZ
    else:
        raise RuntimeError, 'Unknown selection'

    CORE=' '.join([TREES,TREESONLYSKIMZ])
    if 'pccmsrm29' in os.environ['HOSTNAME']: CORE = CORE.replace('/data1/emanuele/wmass','/u2/emanuele')

    CORE+=" -f -j 2 -l 19.7 --s2v --tree treeProducerWMassEle "# --neg"
    if dowhat == "plots": CORE+=" --lspam '#bf{CMS} #it{Preliminary}' --legendWidth 0.20 --legendFontSize 0.035 --showRatio --maxRatioRange 0.8 1.2 --fixRatioRange  --showMCError"

    if selection=='wenu':
        GO="%s wmass_e/mca-53X-wenu.txt wmass_e/wenu.txt "%CORE
        GO="%s -W 'puWeight*SF_LepTight_1l'"%GO
        if dowhat in ["plots","ntuple"]: GO+=" wmass_e/wenu_plots.txt "
    elif selection=='zee':
        GO="%s wmass_e/mca-53X-zee.txt wmass_e/zee.txt "%CORE
        GO="%s -W 'puWeight*SF_LepTight_2l'"%GO
        if dowhat in ["plots","ntuple"]: GO+=" wmass_e/zee_plots.txt --xP 'mZ1' "
    else:
        raise RuntimeError, 'Unknown selection'

    return GO

def procs(GO,mylist):
    return GO+' '+" ".join([ '-p %s'%l for l in mylist ])
def sigprocs(GO,mylist):
    return procs(GO,mylist)+' --showIndivSigs --noStackSig'
def runIt(GO,name,plots=[],noplots=[]):
    if   dowhat == "plots":  print 'python mcPlots.py',"--pdir %s/%s"%(ODIR,name),GO,' '.join(['--sP %s'%p for p in plots]),' '.join(['--xP %s'%p for p in noplots]),' '.join(sys.argv[3:])
    elif dowhat == "yields": print 'echo %s; python mcAnalysis.py'%name,GO,' '.join(sys.argv[3:])
    elif dowhat == "dumps":  print 'echo %s; python mcDump.py'%name,GO,' '.join(sys.argv[3:])
    elif dowhat == "ntuple": print 'echo %s; python mcNtuple.py'%name,GO,' '.join(sys.argv[3:])
def add(GO,opt):
    return '%s %s'%(GO,opt)
def setwide(x):
    x2 = add(x,'--wide')
    x2 = x2.replace('--legendWidth 0.35','--legendWidth 0.20')
    return x2
def fulltrees(x,selection):
    if selection=='wenu': TREESONLYSKIM=TREESONLYSKIMW
    elif selection=='zee': TREESONLYSKIM=TREESONLYSKIMZ
    else:
        raise RuntimeError, 'Unknown selection'
    return x.replace(TREESONLYSKIM,TREESONLYFULL)

allow_unblinding = True

if __name__ == '__main__':

    torun = sys.argv[2]

    if (not allow_unblinding) and '_data' in torun and (not any([re.match(x.strip()+'$',torun) for x in ['.*_appl.*','cr_.*']])): raise RuntimeError, 'You are trying to unblind!'

    if 'zee_' in torun:
        x = base('zee')
        if '_ebeb' in torun: x = add(x,"-A alwaystrue ebeb 'max(abs(LepGood1_eta),abs(LepGood2_eta))<1.44'  --scaleSigToData")
        if '_notebeb' in torun: x = add(x,"-A alwaystrue notebeb 'max(abs(LepGood1_eta),abs(LepGood2_eta))>1.57' --scaleSigToData")
        if '_gg' in torun: x = add(x,"-A alwaystrue goldgold 'min(LepGood1_r9,LepGood2_r9)>0.94' --scaleSigToData")
        if '_notgg' in torun: x = add(x,"-A alwaystrue notgoldgold 'min(LepGood1_r9,LepGood2_r9)<0.94' --scaleSigToData")
        
        runIt(x,'%s'%torun)
