import CMGTools.RootTools.fwlite.Config as cfg

## # Backgrounds
## all            ZZTo4L                    1   0.09505       1 # Pythia; qq only; dyn K to be applied [IP 26/10]
## all            ZZTo4L_tau                1   0.09505       1 #  "
## all            ZZTo4mu                   1   0.01290       1 # 1/2 of ZZTo2e2mu
## all            ZZTo4e                    1   0.01290       1 #  "
## all            ZZTo4tau                  1   0.01290       1 #  "
## all            ZZTo2e2mu                 1   0.02442       1 # Buggy sample - Scaled down by 1/1.2556; 
## all            ZZTo2e2tau                1   0.02442       1 #  " 
## all            ZZTo2mu2tau               1   0.02442       1 #  "
## all            ggZZ4l                    1   0.00174       1 # [IP 13/11]
## all            ggZZ2l2l                  1   0.00348       1 #  "

# x sections etc are here: 
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Torino/ZZAnalysis/AnalysisStep/test/Plotter/Xsection_v1.txt?revision=1.12&view=markup



ZZTo4mu = cfg.MCComponent(
    name = 'ZZTo4mu',
    files = [],
    xSection = 0.01290, 
    nGenEvents = 1, 
    triggers = [],
    effCorrFactor = 1 )

ZZTo4e = cfg.MCComponent(
    name = 'ZZTo4e',
    files = [],
    xSection = 0.01290, 
    nGenEvents = 1, 
    triggers = [],
    effCorrFactor = 1 )

ZZTo4tau = cfg.MCComponent(
    name = 'ZZTo4tau',
    files = [],
    xSection = 0.01290, 
    nGenEvents = 1, 
    triggers = [],
    effCorrFactor = 1 )

ZZTo2e2mu = cfg.MCComponent(
    name = 'ZZTo2e2mu',
    files = [],
    xSection = 0.02442, 
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

ZZTo2e2tau = cfg.MCComponent(
    name = 'ZZTo2e2tau',
    files = [],
    xSection = 0.02442, 
    nGenEvents = 1, 
    triggers = [],
    effCorrFactor = 1 )

ZZTo2mu2tau = cfg.MCComponent(
    name = 'ZZTo2mu2tau',
    files = [],
    xSection = 0.02442, 
    nGenEvents = 1, 
    triggers = [],
    effCorrFactor = 1 )

mc_zz = [
    ZZTo4mu, ZZTo4e, ZZTo4tau,
    ZZTo2e2mu, ZZTo2e2tau, ZZTo2mu2tau
    ]

