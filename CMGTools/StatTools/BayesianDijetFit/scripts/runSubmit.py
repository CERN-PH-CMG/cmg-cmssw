#! /usr/bin/env python
import os
import sys
import time

normal_limits=False

if normal_limits:
  nPE = 0
  queue = "1nh"
  sys = [0, 1, 2, 3, 4, 6, 11, 12, 100, 101, 102, 103, 104, 105]
else:
  nPE = 200
  queue = "1nw"
  sys = [6]


masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]

massesLow =[600., 700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.]

masses1VTag = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300.]
#masses1VTag = []

masses2VTag = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100.]
#masses2VTag = []

massesQBH = [3000., 3200., 3400., 3600., 3800., 4000., 4200., 4400., 4600., 4800., 5000., 5200., 5400.]

masses = []
massesLow = []
#masses1VTag = []
#masses2VTag = []
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



shapes=["Qstar_qW_ak5_fat", "Qstar_qZ_ak5_fat"]




for mass in masses1VTag:
    for sShapes in shapes:
        for iSys in sys:
          if not normal_limits:
            for n in range(nPE/40):
              os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(40+n) + " " + sShapes + " 0 " + queue)
	      time.sleep(30)
          else:
              os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)
              time.sleep(10)

shapes=["RSGraviton_WW_ak5_fat", "RSGraviton_WZ_ak5_fat", "RSGraviton_ZZ_ak5_fat"]




for mass in masses2VTag:
    for sShapes in shapes:
        for iSys in sys:
          if not normal_limits:
            for n in range(nPE/40):
              os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(40+n) + " " + sShapes + " 0 " + queue)
	      time.sleep(30)
          else:
              os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)
              time.sleep(10)


shapes=["QBH_ak5_fat"]



for mass in massesQBH:
    for sShapes in shapes:
        for iSys in sys:
            os.system("python ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " " + str(iSys) + " " + str(nPE) + " " + sShapes + " 0 " + queue)


