#! /usr/bin/env python
import os
import sys
import time

masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]


for mass in masses:
    os.system("python ${CMSSW_BASE}/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 30 Qstar_ak5_fat30 0 1nd")
    os.system("python ${CMSSW_BASE}/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 30 Qstar_ak5_fat30 0 1nd")
    os.system("python ${CMSSW_BASE}/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 30 RSGraviton_ak5_QQtoQQ_fat30 0 1nd")
    os.system("python ${CMSSW_BASE}/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 30 RSGraviton_ak5_QQtoQQ_fat30 0 1nd")
    os.system("python ${CMSSW_BASE}/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 0 30 RSGraviton_ak5_GGtoGG_fat30 0 1nd")
    os.system("python ${CMSSW_BASE}/StatTools/BayesianDijetFit/scripts/submit.py " + str(mass) + " 6 30 RSGraviton_ak5_GGtoGG_fat30 0 1nd")
