#!/usr/bin/env python

MYTREEDIR="/data1/p/peruzzi/TREES_74X_191115_MiniIso_v6"
MYLUMI="2.11"

EXE="python mcPlots.py"
WDIR="susy-multilepton/for-pu-rew"
COMMOPT='--s2v --tree treeProducerSusyMultilepton --noErrorBandOnRatio  --rspam "%(lumi) (13 TeV)  " --lspam "#bf{CMS} #it{Preliminary}" --legendBorder=0 --legendFontSize 0.055 --legendWidth=0.35 --showRatio --maxRatioRange 0 2 --showRatio --poisson -j 8 -f --sp ".*"'
MCA = 'mca_13tev.txt'

print "%s %s/%s %s/zjets-4-nvtx.txt %s/zjets-4-nvtx_plots.txt %s --xp 'ZZTo4L' -P %s -l %s --pdir %s/pu_plots"%(EXE,WDIR,MCA,WDIR,WDIR,COMMOPT,MYTREEDIR,MYLUMI,WDIR)

print "%s %s/%s %s/alwaystrue.txt %s/zjets-4-nvtx_plots_true.txt %s -p 'ZZTo4L' -P %s -l %s --pdir %s/pu_plots"%(EXE,WDIR,MCA,WDIR,WDIR,COMMOPT,MYTREEDIR,MYLUMI,WDIR)

