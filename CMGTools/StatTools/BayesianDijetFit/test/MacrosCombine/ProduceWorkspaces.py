import os

masses =[1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300]

for mass in masses:
  outputname = "submit_"+str(mass)+".src"
  logname = "submit_"+str(mass)+".log"
  outputfile = open(outputname,'w')
  outputfile.write('#!/bin/bash\n')
  outputfile.write("cd ${CMSSW_BASE}/src/CMGTools/StatTools/MacrosCombine; eval `scramv1 run -sh`\n")
  outputfile.write("root -b -q 'R2JJFitter.cc("+str(mass)+")'\n")
  outputfile.close
  
  command="rm "+logname
  print command
  os.system(command)
  command="bsub -q 1nh -o "+logname+" source "+outputname
  print command
  os.system(command)
