#! /usr/bin/env python
import os
import sys
import time

nPE = 50

masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]
#masses = []

for mass in masses:
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " Qstar_ak5_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " Qstar_ak5_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " RSGraviton_ak5_QQtoQQ_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " RSGraviton_ak5_QQtoQQ_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " RSGraviton_ak5_GGtoGG_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " RSGraviton_ak5_GGtoGG_fat30 0 1nw")


masses =[600., 700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.]


for mass in masses:
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " Qstar_HLT_ak5_pf 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " Qstar_HLT_ak5_pf 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " RSGraviton_HLT_ak5_QQtoQQ_pf 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " RSGraviton_HLT_ak5_QQtoQQ_pf 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " RSGraviton_HLT_ak5_GGtoGG_pf 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " RSGraviton_HLT_ak5_GGtoGG_pf 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " Qstar_HLT_ak5_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " Qstar_HLT_ak5_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " RSGraviton_HLT_ak5_QQtoQQ_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " RSGraviton_HLT_ak5_QQtoQQ_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 " + str(nPE) + " RSGraviton_HLT_ak5_GGtoGG_fat30 0 1nw")
    os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 " + str(nPE) + " RSGraviton_HLT_ak5_GGtoGG_fat30 0 1nw")




