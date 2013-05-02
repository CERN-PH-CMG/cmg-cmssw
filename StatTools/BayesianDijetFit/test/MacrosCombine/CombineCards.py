import os

masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0]

for mass in masses:
  command="combineCards.py ch0=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel0.txt ch1=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel1.txt ch2=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel2.txt > datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel012.txt"
  print command
  os.system(command)
  command="combineCards.py ch0=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel0.txt ch1=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel1.txt ch2=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel2.txt > datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel012.txt"
  print command
  os.system(command)
  #command="combineCards.py ch0=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel0.txt ch1=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel1.txt ch2=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel2.txt ch3=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel3.txt ch4=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel4.txt ch5=datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel5.txt > datacards/Xvv.mX"+str(mass)+"_WW_8TeV_channel012345.txt"
  #print command
  #os.system(command)
  #command="combineCards.py ch0=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel0.txt ch1=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel1.txt ch2=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel2.txt ch3=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel3.txt ch4=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel4.txt ch5=datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel5.txt > datacards/Xvv.mX"+str(mass)+"_ZZ_8TeV_channel012345.txt"
  #print command
  #os.system(command)
