import subprocess
import os
import time

processes = set()
max_processes = 4

doSM = True
doMSSM = True

catsSM = []
catsMSSM = []

if doSM:
    catsSM = [
    'Xcat_IncX && mt<30 && Xcat_J0_lowX && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_J0_mediumX && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_J0_highX && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_J1_mediumX && met>30 && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_J1_high_mediumhiggsX && met>30 && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_J1_high_lowhiggsX && met>30 && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_VBF_looseX && l1_decayMode==1',
    'Xcat_IncX && mt<30 && Xcat_VBF_tightX && l1_decayMode==1',
]

if doMSSM:
    catsMSSM = [
    'Xcat_IncX && mt<30 && Xcat_J1BX',
    'Xcat_IncX && mt<30 && Xcat_0BX'
]


dirUp = '/data/steggema/Aug08TauEle_Up/' #NOTE: Must end with Up
dirDown = '/data/steggema/Aug08TauEle_Down/' #NOTE: Must end with Down
dirNom = '/data/steggema/Aug08TauEle/'

dirGGH = '/data/steggema/Aug08TauEle/'


embedded = True

embOpt = ''
if embedded:
    embOpt = '-E'

mssmBinnings = ['2013', 'fine', 'default']

allArgs = []
for cat in catsSM:
    args = ['python', 'plot_H2TauTauDataMC_TauEle_All.py', dirNom, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', embOpt]


    argsUp = ['python', 'plot_H2TauTauMC.py', dirUp, 'tauEle_2012_up_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', embOpt, '-f', 'Higgs;Ztt', '-c', 'TauEle']
    argsDown = ['python', 'plot_H2TauTauMC.py', dirDown, 'tauEle_2012_down_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', embOpt, '-f', 'Higgs;Ztt', '-c', 'TauEle']


    argsGGHUp = ['python', 'plot_H2TauTauMC.py', dirGGH, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', '-f', 'HiggsGGH', '-w', 'weight*hqtWeightUp/hqtWeight', '-s', 'QCDscale_ggH1inUp', '-c', 'TauEle']
    argsGGHDown = ['python', 'plot_H2TauTauMC.py', dirGGH, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', '-f', 'HiggsGGH', '-w', 'weight*hqtWeightDown/hqtWeight', '-s', 'QCDscale_ggH1inDown', '-c', 'TauEle']

    argsZLUp = ['python', 'plot_H2TauTauMC.py', dirNom, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass*1.02', '-b', '-f', 'Ztt', '-s', 'CMS_htt_ZLScale_etau_8TeVUp', '-c', 'TauEle']
    argsZLDown = ['python', 'plot_H2TauTauMC.py', dirNom, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass*0.98', '-b', '-f', 'Ztt', '-s', 'CMS_htt_ZLScale_etau_8TeVDown', '-c', 'TauEle']

    allArgs += [args, argsUp, argsDown, argsGGHUp, argsGGHDown, argsZLUp, argsZLDown]

for cat in catsMSSM:
    for mssmBinning in mssmBinnings:
            args = ['python', 'plot_H2TauTauDataMC_TauEle_All.py', dirNom, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', embOpt, '-k', mssmBinning, '-p', mssmBinning, '-g', '125']

            argsUp = ['python', 'plot_H2TauTauMC.py', dirUp, 'tauEle_2012_up_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', embOpt, '-f', 'SUSY;Higgs;Ztt', '-k', mssmBinning, '-p', mssmBinning, '-c', 'TauEle', '-g', '125']
            argsDown = ['python', 'plot_H2TauTauMC.py', dirDown, 'tauEle_2012_down_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', embOpt, '-f', 'SUSY;Higgs;Ztt', '-k', mssmBinning, '-p', mssmBinning, '-c', 'TauEle', '-g', '125']


            argsGGHUp = ['python', 'plot_H2TauTauMC.py', dirGGH, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', '-f', 'HiggsGGH', '-w', 'weight*hqtWeightUp/hqtWeight', '-s', 'QCDscale_ggH1inUp', '-k', mssmBinning, '-p', mssmBinning, '-c', 'TauEle', '-g', '125']
            argsGGHDown = ['python', 'plot_H2TauTauMC.py', dirGGH, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass', '-b', '-f', 'HiggsGGH', '-w', 'weight*hqtWeightDown/hqtWeight', '-s', 'QCDscale_ggH1inDown', '-k', mssmBinning, '-p', mssmBinning, '-c', 'TauEle', '-g', '125']

            argsZLUp = ['python', 'plot_H2TauTauMC.py', dirNom, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass*1.02', '-b', '-f', 'Ztt', '-s', 'CMS_htt_ZLScale_etau_8TeVUp', '-k', mssmBinning, '-p', mssmBinning, '-c', 'TauEle', '-g', '125']
            argsZLDown = ['python', 'plot_H2TauTauMC.py', dirNom, 'tauEle_2012_cfg.py', '-C', cat, '-H', 'svfitMass*0.98', '-b', '-f', 'Ztt', '-s', 'CMS_htt_ZLScale_etau_8TeVDown', '-k', mssmBinning, '-p', mssmBinning, '-c', 'TauEle', '-g', '125']

            allArgs += [args, argsUp, argsDown, argsGGHUp, argsGGHDown, argsZLUp, argsZLDown]


for args in allArgs:
    processes.add(subprocess.Popen(args))
    if len(processes) >= max_processes:
        os.wait()
        processes.difference_update(
            p for p in processes if p.poll() is not None)

#Check if all the child processes were closed
for p in processes:
    if p.poll() is None:
        p.wait()
        # os.wait()
