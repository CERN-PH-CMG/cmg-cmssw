#!/usr/bin/env python

ODIR="~/www/25ns_run2015d_upto4391"
MYTREEDIR="~/work/ra5trees/TREES_74X_230915_MiniIso_7_4_12"
MYLUMI="0.12163"
#run 256843 up to lumi ~ 370

EXE="python mcPlots.py"
MYMCC="--mcc susy-multilepton/first-data-new/susy_jetPtRatioL1Corr.txt"
WDIR="susy-multilepton/first-data-new"
COMMOPT='--s2v --tree treeProducerSusyMultilepton --FMC sf/t {P}/0_vtxWeight_v1/evVarFriend_{cname}.root -W vtxWeight --noErrorBandOnRatio  --rspam "%(lumi) (13 TeV)  " --lspam "#bf{CMS} #it{Preliminary}" --legendBorder=0 --legendFontSize 0.055 --legendWidth=0.35 --showRatio --maxRatioRange 0 2 --showRatio --poisson -j 8 -f --sp ".*"'

SELECTIONS=["ZtoEE","ZtoMuMu","ttbar","Wl","Zl","ttbar_semiLeptonic"]

for SEL in SELECTIONS:
    print '#'+SEL
    MCA = 'mca_13tev_%s.txt'%SEL if SEL in ['Wl','Zl','ttbar_semiLeptonic'] else 'mca_13tev.txt'
    print '%s %s/%s %s/cuts_%s.txt %s/plots_%s.txt %s -P %s -l %s %s --scaleSigToData --pdir %s/%s/ScaleSigToData'%(EXE,WDIR,MCA,WDIR,SEL,WDIR,SEL,COMMOPT,MYTREEDIR,MYLUMI,MYMCC,ODIR,SEL)
    print '%s %s/%s %s/cuts_%s.txt %s/plots_%s.txt %s -P %s -l %s %s --pdir %s/%s/ScaleToLumi'%(EXE,WDIR,MCA,WDIR,SEL,WDIR,SEL,COMMOPT,MYTREEDIR,MYLUMI,MYMCC,ODIR,SEL)

