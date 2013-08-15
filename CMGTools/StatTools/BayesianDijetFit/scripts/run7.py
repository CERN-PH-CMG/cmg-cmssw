#! /usr/bin/env python

import os

#******************   definitions  **********************************
dataset="HT"

# 0 - stats only
# 1 - background only
# 2 - lumi only
# 3 - JES only
# 4 - JER only
# 6 - background , JES, JER, lumi all together.
# 9 - background, lumi.

parameters = "4"

# type of jet "calo" or "pf"

Jet = "pf"

# type of resonance decay "qg, gg, qq"

type = "qg"

# numPEs

numPEs = "0"


if (int(numPEs)==0):
    EVAL_PATH =          "/uscms_data/d2/chiyoung/Dijet/CMSSW_4_2_5_"+dataset+"_"+Jet+"_"+type+"/src"
else:
    EVAL_PATH =          "/uscms_data/d2/chiyoung/Dijet/CMSSW_4_2_5_"+dataset+"_"+Jet+"_"+type+"_PE/src"
WORK_PATH =          "/uscms_data/d2/chiyoung/work/dijet_4_2_5/7TeV/2011Jul01/limit/limit_john/1010_"+dataset+"_"+Jet+"_"+type+"_"+str(parameters)+"_"+numPEs+"/"
EMAIL =              "chiyoung@gmail.com"

#*********************************************************************

if (os.path.exists(WORK_PATH)):
    print "directory "+WORK_PATH+" exists"
else:
    os.mkdir(WORK_PATH)

controle_PE=100
if (int(numPEs)==0):
    controle_PE=0

start_sample=1000
end_sample=4100

if (int(numPEs)==0):
    PE1=0
    PE2=0
    PE3=0
    PE4=0
else:
    PE1=1
    PE2=1
    PE3=1
    PE4=1


while PE1<=controle_PE:
    sample1=start_sample
    while sample1<=end_sample: #Starting the loop for different samples
        
        #************* python file *********************
        py_body = "run"+"_"+str(sample1)+"_"+parameters+"_"+str(PE1)
        if not (os.path.exists(WORK_PATH+py_body)):
            os.mkdir(WORK_PATH+py_body)
        py_name = WORK_PATH+py_body+"/"+py_body+".py" 
        file = open(py_name,'w')
        file.write("#! /usr/bin/env python \n")
        file.write("import os \n")
        if (int(numPEs)==0):
            phrase="command = \"dijetStatsNewBackground "+str(sample1)+" "+parameters+" 0 "+"\" \n"
        else:
            phrase="command = \"dijetStatsNewBackground "+str(sample1)+" "+parameters+" "+str(int(numPEs)/controle_PE)+" "+"\" \n"
        file.write(phrase)
        phrase="print command \n"
        file.write(phrase)
        phrase="os.system(command) \n"
        file.write(phrase)
        phrase="print 'end' \n"
        file.write(phrase)
        file.close()
        if (PE1 == 1)or(PE1 == 0):
            print sample1
        #******************  creating the executable  *********************
        csh_name = WORK_PATH+py_body+"/"+py_body+".csh"
        file = open(csh_name,'w')
        file.write("#!/bin/tcsh \n")
        file.write("date \n")
        file.write("setenv SCRAM_ARCH slc5_amd64_gcc434 \n")
        file.write("source /uscmst1/prod/sw/cms/setup/cshrc prod \n")
        phrase = "cd "+EVAL_PATH+" \n"
        file.write(phrase)
        file.write("eval `scramv1 runtime -csh` \n")
        phrase = "cd "+WORK_PATH+py_body+" \n"
        file.write(phrase)
        phrase = "python "+py_name+" >&! "+"$_CONDOR_SCRATCH_DIR/log_"+str(sample1)+ "_" +parameters+ "_"+str(PE1)+".txt \n"
        file.write(phrase)
        file.write("date \n")
        file.close()
        command = "chmod a+x "+csh_name
        os.system(command)
        #******************  creating the condor job  *********************
        name = WORK_PATH+py_body+"/Condor_"+py_body
        file = open(name,'w')
        file.write("universe = vanilla \n")
        phrase = "Executable = "+csh_name+" \n"
        file.write(phrase)
        file.write("Requirements = Memory >= 199 &&OpSys == \"LINUX\"&& (Arch != \"DUMMY\" )&& Disk > 1000000 \n")
        file.write("Should_Transfer_Files = YES \n")
        file.write("WhenToTransferOutput = ON_EXIT \n")
#        phrase = "Output = "+py_body+"_$(Cluster).stdout \n"
#        file.write(phrase)
#        phrase = "Error = "+py_body+"_$(Cluster).stderr \n"
#        file.write(phrase)
#        phrase = "Log = "+py_body+"_$(Cluster).condorlog \n"
#        file.write(phrase)
        phrase = "notify_user = "+EMAIL+" \n"
        file.write(phrase)
        file.write("Queue 1 \n")
        file.close()
        sample1 += 100
    PE1 += 1




        
    #****************** crearting the controle file which submit all condor file ****************
name = WORK_PATH+"submit_all_condor.py"
file = open(name,'w')
file.write("#! /usr/bin/env python \n")
file.write("import os \n")
while PE2<=controle_PE:
    sample2=start_sample
    while sample2<=end_sample: #Starting the loop for sumbmit python file
        py_body = "run"+"_"+str(sample2)+"_"+parameters+"_"+str(PE2)
        phrase = "command = \"condor_submit "+py_body+"/Condor_"+py_body+"\" \n"
        file.write(phrase)
        phrase="os.system(command) \n"
        file.write(phrase)
        sample2 += 100
    PE2 += 1
file.close()

    #****************** crearting controle file to run python file locally****************

name = WORK_PATH+"submit_all_locally.py"
file = open(name,'w')
file.write("#! /usr/bin/env python \n")
file.write("import os \n")
while PE3<=controle_PE:
    sample3=start_sample
    while sample3<=end_sample: #Starting the loop for sumbmit python file
        py_body = "run"+"_"+str(sample3)+"_"+parameters+"_"+str(PE3)
        py_name = py_body+"/"+py_body+".py" 
        phrase = "command = \"python "+py_name+" >& "+py_body+"/log_"+str(sample3)+ "_" +parameters+ "_"+str(PE3)+".txt \" \n"
        file.write(phrase)
        phrase = "print command \n"
        file.write(phrase)
        phrase="os.system(command) \n"
        file.write(phrase)
        sample3 += 100
    PE3 += 1
file.close()

    #***************** creating the ourpur handling file**********************************

name = WORK_PATH+"output.py"

file = open(name,'w')
file.write("#! /usr/bin/env python \n")
file.write("import subprocess, shlex \n")
check = 0;
sample4=start_sample
if (int(numPEs)!=0):
    phrase = "up_2sigma=[]\n"
    file.write(phrase)
    phrase = "up_1sigma=[]\n"
    file.write(phrase)
    phrase = "down_1sigma=[]\n"
    file.write(phrase)
    phrase = "down_2sigma=[]\n"
    file.write(phrase) 
phrase = "median_list=[]\n"
file.write(phrase)
while sample4<=end_sample:
    if (int(numPEs)!=0):
        PE4=1
    else:
        PE4=0
    phrase = "upperlimit_"+str(sample4)+"=[]\n"
    file.write(phrase)
    while PE4<=controle_PE: #Staring the loop for output handling file
        #phrase = "command = \"tail log_"+str(sample4)+"_"+parameters+"_"+str(PE4)+".txt | grep \\\"upperlimit\\\" | cut -d \\\" \\\" -f 19 \" \n"
        #file.write(phrase)
        #phrase = "os.system(command) \n"
        #file.write(phrase)
        py_body = "run"+"_"+str(sample4)+"_"+parameters+"_"+str(PE4)
        phrase = "cmd1 = \'cat log_"+str(sample4)+"_"+parameters+"_"+str(PE4)+".txt\'\n"
        file.write(phrase)
        phrase = "cmd2 = \'grep  \\\"upperlimit\\\"\'\n"
        file.write(phrase)
        phrase = "cmd3 =   \'cut -d \\\" \\\" -f 19\'\n"
        file.write(phrase)
        phrase = "args1 = shlex.split(cmd1)\n"
        file.write(phrase)
        phrase = "args2 = shlex.split(cmd2)\n"
        file.write(phrase)
        phrase = "args3 = shlex.split(cmd3)\n"
        file.write(phrase)
        phrase = "cat = subprocess.Popen(args1,\n"
        file.write(phrase)
        phrase = "                        stdout=subprocess.PIPE\n"
        file.write(phrase)
        phrase = "                        )\n"
        file.write(phrase)
        phrase = "grep = subprocess.Popen(args2,\n"
        file.write(phrase)
        phrase = "                        stdin=cat.stdout,\n"
        file.write(phrase)        
        phrase = "                        stdout=subprocess.PIPE\n"
        file.write(phrase)
        phrase = "                        )\n"
        file.write(phrase)
        phrase = "cut = subprocess.Popen(args3,\n"
        file.write(phrase)
        phrase = "                        stdin=grep.stdout,\n"
        file.write(phrase)        
        phrase = "                        stdout=subprocess.PIPE\n"
        file.write(phrase)
        phrase = "                        )\n"
        file.write(phrase)
        phrase = "for line in iter(cut.stdout.readline,\'\'):\n"
        file.write(phrase)
        phrase = "\tupperlimit_"+str(sample4)+".append(float(line.strip()))\n"
        file.write(phrase)
        PE4 += 1

    if (int(numPEs)!=0):
        phrase = "upperlimit_"+str(sample4)+".sort()\n"
        file.write(phrase)
        phrase = "print \"length of +"+str(sample4)+"+= \"+str(len(upperlimit_"+str(sample4)+")) \n"
        file.write(phrase)
        phrase = "down2 = (len(upperlimit_"+str(sample4)+")-1)*0.0225\n"
        file.write(phrase)
        phrase = "down1 = (len(upperlimit_"+str(sample4)+")-1)*0.159\n"
        file.write(phrase)
        phrase = "median = (len(upperlimit_"+str(sample4)+")-1)*0.5\n"
        file.write(phrase)
        phrase = "upper1 = (len(upperlimit_"+str(sample4)+")-1)*0.841\n"
        file.write(phrase)
        phrase = "upper2 = (len(upperlimit_"+str(sample4)+")-1)*0.9975\n"
        file.write(phrase)
        phrase = "if (down2-int(down2)>0.5):\n"
        file.write(phrase)
        phrase = "\tdown2_index=int(down2)+1\n"
        file.write(phrase)
        phrase = "else:\n"
        file.write(phrase)
        phrase = "\tdown2_index=int(down2)\n"
        file.write(phrase)
        phrase = "if (down1-int(down1)>0.5):\n"
        file.write(phrase)
        phrase = "\tdown1_index=int(down1)+1\n"
        file.write(phrase)
        phrase = "else:\n"
        file.write(phrase)
        phrase = "\tdown1_index=int(down1)\n"
        file.write(phrase)
        phrase = "if (median-int(median)>0.5):\n"
        file.write(phrase)
        phrase = "\tmedian_index=int(median)+1\n"
        file.write(phrase)
        phrase = "else:\n"
        file.write(phrase)
        phrase = "\tmedian_index=int(median)\n"
        file.write(phrase)
        phrase = "if (upper1-int(upper1)>0.5):\n"
        file.write(phrase)
        phrase = "\tupper1_index=int(upper1)+1\n"
        file.write(phrase)
        phrase = "else:\n"
        file.write(phrase)
        phrase = "\tupper1_index=int(upper1)\n"
        file.write(phrase)
        phrase = "if (upper2-int(upper2)>0.5):\n"
        file.write(phrase)
        phrase = "\tupper2_index=int(upper2)+1\n"
        file.write(phrase)
        phrase = "else:\n"
        file.write(phrase)
        phrase = "\tupper2_index=int(upper2)\n"
        file.write(phrase)
        phrase = "down_2sigma.append(upperlimit_"+str(sample4)+"[down2_index])\n"
        file.write(phrase)
        phrase = "down_1sigma.append(upperlimit_"+str(sample4)+"[down1_index])\n"
        file.write(phrase)
        phrase = "median_list.append(upperlimit_"+str(sample4)+"[median_index])\n"
        file.write(phrase)
        phrase = "up_1sigma.append(upperlimit_"+str(sample4)+"[upper1_index])\n"
        file.write(phrase)
        phrase = "up_2sigma.append(upperlimit_"+str(sample4)+"[upper2_index])\n"
        file.write(phrase)
    else:
        phrase = "median_list.append(upperlimit_"+str(sample4)+"[0])\n"
        file.write(phrase)
    sample4 += 100

if (int(numPEs)!=0):
    phrase = "print \"lenth of down_2sigma is\", len(down_2sigma)\n"
    file.write(phrase)
    phrase = "print down_2sigma\n"
    file.write(phrase)
    phrase = "print \"lenth of down_1sigma is\", len(down_1sigma)\n"
    file.write(phrase)
    phrase = "print down_1sigma\n"
    file.write(phrase)
    phrase = "print \"lenth of up_1sigma is\", len(up_1sigma)\n"
    file.write(phrase)
    phrase = "print up_1sigma\n"
    file.write(phrase)
    phrase = "print \"lenth of up_2sigma is\", len(up_2sigma)\n"
    file.write(phrase)
    phrase = "print up_2sigma\n"
    file.write(phrase)
phrase = "print \"lenth of median is\", len(median_list)\n"
file.write(phrase)
phrase = "print median_list\n"
file.write(phrase)
file.close()

print "Done........" #End of sample loop               

