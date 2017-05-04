#!/usr/bin/env python
from shutil import copyfile
import re, sys, os, os.path, subprocess

FASTTEST=''
#FASTTEST='--max-entries 1000 '
T='/data1/emanuele/wmass/TREES_1LEP_53X_V2_WSKIM_V1/'
J=4
MCA='wmass_e/mca-53X-wenu.txt'
CUTFILE='wmass_e/wenu.txt'
SYSTFILE='wmass_e/systsEnv.txt'
VAR="w_mt 90,20,110"
NPDFSYSTS=53 # for CT10

def writePdfSystsToMCA(sample,syst,dataset,xsec,vec_weight,filename):
    mcafile = open(filename, "a")
    for i in range(1,NPDFSYSTS):
        pdfvar=str((i-1)/2+1)
        direction="Up" if i%2 else "Dn"
        mcafile.write(sample+"_"+str(syst)+pdfvar+"_"+direction+"+   : "+dataset+" :  "+str(xsec)+" : "+vec_weight+"["+str(i)+"]/"+vec_weight+"[0]; SkipMe=True \n")
    print "written ",vec_weight," systematics into ",filename

def writePdfSystsToSystFile(sample,syst,channel,filename):
    systfile=open(filename,"a")
    for i in range(NPDFSYSTS/2):
        systfile.write(channel+"_pdf"+str(i+1)+"  : "+sample+" : .* : pdf"+str(i+1)+" : templates\n")
    print "written pdf syst configuration to ",filename
        
from optparse import OptionParser
parser = OptionParser(usage="%prog testname ")
parser.add_option("--etaBins", dest="etaBins", action="append", default=[], help="Give a list of lepton eta bins to make fit categories")
(options, args) = parser.parse_args()

outdir="cards/"+args[0]

# write systematic variations to be considered in the MCA file
MCASYSTS=('.').join(MCA.split('.')[:-1])+"-systs.txt"
copyfile(MCA,MCASYSTS)
writePdfSystsToMCA("W","pdf","WJets",37509.0,"pdfWeight_CT10",MCASYSTS)

# write the complete systematics file
SYSTFILEALL=('.').join(SYSTFILE.split('.')[:-1])+"-all.txt"
copyfile(SYSTFILE,SYSTFILEALL)
writePdfSystsToSystFile("W","pdf","CMS_We",SYSTFILEALL)

ARGS=' '.join([MCASYSTS,CUTFILE,VAR,SYSTFILEALL])
OPTIONS=" -P "+T+" --s2v -j "+str(J)+" -l 19.7 -f "+FASTTEST
if not os.path.exists(outdir): os.makedirs(outdir)
OPTIONS+=' -F mjvars/t "'+T+'/friends/evVarFriend_{cname}.root" '

masses = [ 10,20,40 ]
#masses = [20]
mass_offs = 0

etaBins=options.etaBins if options.etaBins!=[] else ['0','5']
for ieta in range(len(etaBins)-1):
    subdir = "eta_"+str(ieta) if options.etaBins!=[] else 'etaIncl'
    etacut=" -A alwaystrue eta%d 'abs(LepGood1_eta)>%s && abs(LepGood1_eta)<%s' " % (ieta,etaBins[ieta],etaBins[ieta+1])
    myout = outdir + "/" + subdir
    if not os.path.exists(myout): os.mkdir(myout)
    for mass in masses:
        smass=str(mass-mass_offs)
        W=" -W 'puWeight*mwWeight["+str(mass)+"]' "
        POS=" -A alwaystrue positive 'LepGood1_charge>0' "
        NEG=" -A alwaystrue negative 'LepGood1_charge<0' "
        charges=[POS,NEG]
        for c in charges: 
            dcname="wenu_mass"+smass+("_pos_" if "positive" in c else "_neg_")+subdir
            BIN_OPTS=OPTIONS+W+" -o "+dcname+" --od "+myout+" --floatProcesses W --groupSystematics pdfUncertainties pdf"
            cmd = "python makeShapeCards.py "+ARGS+" "+BIN_OPTS+c+etacut
            #print cmd
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
            out, err = p.communicate() 
            result = out.split('\n')
            for lin in result:
                if not lin.startswith('#'):
                    print(lin)
