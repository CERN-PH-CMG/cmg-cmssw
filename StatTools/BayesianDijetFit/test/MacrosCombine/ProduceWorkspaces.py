import os

masses =[1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900]

for mass in masses:
  outputname = "submit_VV_"+str(mass)+".src"
  logname = "submit_VV_"+str(mass)+".log"
  outputfile = open(outputname,'w')
  outputfile.write('#!/bin/bash\n')
  outputfile.write("cd ${CMSSW_BASE}/src/CMGTools/StatTools/MacrosCombine; eval `scramv1 run -sh`\n")
  outputfile.write("root -b -q 'R2JJFitter.cc("+str(mass)+","+'""'+",0)'\n")
  outputfile.close
  
  command="rm "+logname
  print command
  os.system(command)
  command="bsub -q 1nh -o "+logname+" source "+outputname
  print command
  os.system(command)

masses =[1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000]

for mass in masses:
  outputname = "submit_qV_"+str(mass)+".src"
  logname = "submit_qV_"+str(mass)+".log"
  outputfile = open(outputname,'w')
  outputfile.write('#!/bin/bash\n')
  outputfile.write("cd ${CMSSW_BASE}/src/CMGTools/StatTools/MacrosCombine; eval `scramv1 run -sh`\n")
  outputfile.write("root -b -q 'R2JJFitter.cc("+str(mass)+","+'""'+",1)'\n")
  outputfile.close
  
  command="rm "+logname
  print command
  os.system(command)
  command="bsub -q 1nh -o "+logname+" source "+outputname
  print command
  os.system(command)
