#! /usr/bin/env python
import os
import sys
import time

nPE = 0
queue = "1nh"

#nPE = 50
#queue = "1nw"

#sys = [0,6]
sys = [0, 1, 2, 3, 4, 6]


masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]

massesLow =[600., 700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.]

massesJetTag = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]


massesQBH = [3000., 3200., 3400., 3600., 3800., 4000., 4200., 4400., 4600., 4800., 5000., 5200., 5400.]

masses = []
massesLow = []
#massesJetTag = []
massesQBH = []

shapes=["RSGraviton_ak5_GGtoGG_fat30", "RSGraviton_ak5_QQtoQQ_fat30", "Qstar_ak5_fat30"]


for mass in masses:
   for sShapes in shapes:
       for iSys in sys:
           os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)
    



shapes=["RSGraviton_HLT_ak5_GGtoGG_fat30", "RSGraviton_HLT_ak5_QQtoQQ_fat30", "Qstar_HLT_ak5_fat30", "RSGraviton_HLT_ak5_GGtoGG_pf", "RSGraviton_HLT_ak5_QQtoQQ_pf", "Qstar_HLT_ak5_pf"]


for mass in massesLow:
    for sShapes in shapes:
       for iSys in sys:
           os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass)  + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)



shapes=["Qstar_qW_ak5_fat", "Qstar_qZ_ak5_fat", "RSGraviton_WW_ak5_fat", "RSGraviton_WZ_ak5_fat", "RSGraviton_ZZ_ak5_fat"]




for mass in massesJetTag:
    for sShapes in shapes:
        for iSys in sys:
            os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)


shapes=["QBH_ak5_fat"]



for mass in massesQBH:
    for sShapes in shapes:
        for iSys in sys:
            os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)


