#!/usr/bin/env python

import os,sys
#import json
import getopt
import commands
import ROOT
from ROOT import TFile, TGraph, TCanvas, TF1, TH1

#default values
shapeBased='1'
shapeName='dijet_deta_shapes'
inUrl='/afs/cern.ch/user/p/psilva/www/FSQ-12-035/ll_19.6fbinv_ewkz.root'
CWD=os.getcwd()
phase=-1
jsonUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_vbfzmg_2012.json'
CMSSW_BASE=os.environ.get('CMSSW_BASE')
sqrts='8'
dyTemplates='/afs/cern.ch/user/p/psilva/www/FSQ-12-035/gamma_19.6fbinv_ewkz.root'
smXsec=0.504
blind=False

MASS = [0]
SUBMASS = [0]

cutList='' 
def help() :
   print '\n\033[92m optimize.py \033[0m \n'
   print ' -p phase (no default value is assigned)'
   print '\t 1 --> submit landS jobs for all selection point'
   print '\t 2 --> check the logs to find the optimal selection point'
   print '\t      from the ouptut of the logs you can search for the optimal points yourself ;)'
   print '\t      and edit phase3 of this script with your optimal points (note: change this to be given as input)'
   print '\t 3 --> you are prompted to choose the best cuts for the selection: the limits will be comptued for the list of cuts'
   print '\t       if -f LIST.txt is given the LIST of cuts passed will be used instead'
   print '\t 4 --> once all the final limit jobs have been run, use this phase to build the brazilian flag plot'
   print ' -m mode (default='+shapeBased+')'
   print '\t 0 --> cut and count based analysis'
   print '\t 1 --> shape based analysis'
   print ' -s shapename (default='+shapeName+')'
   print ' -i inputfile (default='+inUrl+')'
   print ' -o output (default='+CWD+')'
   print ' -j jsonfile (default='+jsonUrl+')'
   print ' -e cm energy (default='+sqrts+')'
   print ' -t dy templates from gamma (default='+dyTemplates+')' 
   print ' -x sm cross section (default='+str(smXsec)+')'
   print '\nUsage example: \033[92m python optimize.py -m 0 -i ~/work/plotter.root -o ~/work/ -p 1 \033[0m'
   print '\nNote: CMSSW_BASE must be set when launching optimize.py (current values is: ' + CMSSW_BASE + ')\n' 
   
#parse the options
try:
   # retrive command line options
   shortopts  = "p:f:m:i:s:j:o:e:t:b:x:h?"
   opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
   # print help information and exit:
   print "ERROR: unknown options in argument %s" % sys.argv[1:]
   help()
   sys.exit(1)

for o,a in opts:
   if o in("-?", "-h"):
      help()
      sys.exit(1)
   elif o in('-m'): shapeBased = a
   elif o in('-i'): inUrl = a
   elif o in('-p'): phase = int(a)
   elif o in('-o'): CWD=a
   elif o in('-j'): jsonUrl=a
   elif o in('-s'): shapeName=a
   elif o in('-f'): cutList=a
   elif o in('-e'): sqrts=a
   elif o in('-t'): dyTemplates=a
   elif o in('-x'):  smXsec=float(a)
   elif o in('-b'): blind=True 
      
if(phase<0 or len(CMSSW_BASE)==0):
   help()
   sys.exit(1)

#build the arguments for runLands
LandSArg=''
if(blind) : LandSArg='--blindWithSignal'
LandSArg+=' --systpostfix _%sTeV'%sqrts
if(len(dyTemplates)>0) : LandSArg+=' --subDY '+dyTemplates
LandSArg +=' --bins mjjq016,mjjq033,mjjq049,mjjq066,mjjq083,mjjq092,mjjq100'

DataCardsDir='cards%s'%(sqrts)

#auxiliary function
def findCutIndex(cut1, hcut1, cut2, hcut2, cut3, hcut3, cut4, hcut4):
   for i in range(1, hcut1.GetXaxis().GetNbins()):
      if(hcut1.GetBinContent(i)<cut1-0.1):continue;
      if(hcut2.GetBinContent(i)<cut2-0.1):continue;
      if(hcut3.GetBinContent(i)<cut3-0.1):continue;
      if(hcut4.GetBinContent(i)<cut4-0.1):continue;
      return i;   
   return hcut1.GetXaxis().GetNbins();

def findSideMassPoint(mass):
   global MASS
   LMass=0
   RMass=9999
   for m in MASS:
      if(m<=mass and m>=LMass):LMass=m
      if(m>=mass and m<=RMass):RMass=m
   return [LMass,RMass]

#######################

#prepare the output
OUT = CWD+'/JOBS'+sqrts+'/'
if(shapeBased=='1'): OUT+='SHAPE/'
else:		     OUT+='COUNT/'	
os.system('mkdir -p ' + OUT)

if(shapeBased=='1'): DataCardsDir+='Shape'

#get the cuts
file = ROOT.TFile(inUrl)
cuts   = file.Get('data/optim_cut2') 


######################################################################

if( phase == 1 ):

   print '#                                  #'
   print '# RUN LIMITS FOR ALL POSSIBLE CUTS #'
   print '#                                  #'

   commandToRun = []

   FILE = open(OUT+"/LIST.txt","w")
   print cuts
   for i in range(1,cuts.GetNbinsX()):
      #      if(shapeBased=='1' and cuts3.GetBinContent(i)<780):continue
      FILE.writelines("index="+str(i).rjust(5) + " --> jet1>" + str(cuts.GetBinContent(i,1)).rjust(5) + " jet2>" + str(cuts.GetBinContent(i,2)).rjust(5)+"\n")

      #create wrappper script for each set of cuts ans submit it
      SCRIPT = open(OUT+'script_'+str(i)+'.sh',"w")
      SCRIPT.writelines('echo "TESTING SELECTION : ' + str(i).rjust(5) + ' --> jet1>' + str(cuts.GetBinContent(i,1)).rjust(5) + ' jet2>' + str(cuts.GetBinContent(i,2)).rjust(5) + '";\n')
      SCRIPT.writelines('cd ' + CMSSW_BASE + '/src;\n')
      SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc462")+";\n")
      SCRIPT.writelines("eval `scram r -sh`;\n")
      SCRIPT.writelines('cd ' + CWD + ';\n')
      #      SCRIPT.writelines('cd /tmp/;\n')
      for m in MASS:
         shapeBasedOpt=''
         if(shapeBased=='1') : shapeBasedOpt='--shape'
         cardsdir = 'optim'+sqrts+'/H'+ str(m);
         if(shapeBased=='0'): cardsdir+='_count_'+str(i)
         if(shapeBased=='1'): cardsdir+='_shape_'+str(i)
         SCRIPT.writelines('mkdir -p ' + cardsdir+';\ncd ' + cardsdir+';\n')
         SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " --syst " + shapeBasedOpt + " --index " + str(i)     + " --json " + jsonUrl +" --fast " + LandSArg + " ;\n")
         SCRIPT.writelines("sh combineCards.sh;\n")
         #         SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " --run expected card_combined.dat > COMB.log;\n")
         SCRIPT.writelines("combine -M MaxLikelihoodFit -m " +  str(m) + " --saveNormalizations card_combined.dat > COMB.log;\n")
         SCRIPT.writelines("extractFitNormalization.py mlfit.root hzz2l2v__%sTeV.root > fit.txt;\n"%sqrts)
         SCRIPT.writelines('tail -n 100 COMB.log > ' +OUT+str(m)+'_'+str(i)+'.log;\n')
         SCRIPT.writelines('cd ..;\n\n')
         SCRIPT.close()
      commandToRun.append("bsub -G u_zh -q 8nh -J optim"+str(i)+" 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")
   FILE.close()

   for c in commandToRun:
      print(c)
      os.system(c);

      
######################################################################
elif(phase == 2):
   
   print '#                                      #'
   print '# SCANNING ALL SETS OF CUTS            #'
   print '# (you may want to go for a coffee...) #'
   print '#                                      #'
   
   fileName = OUT + "/OPTIM_"
   if(shapeBased=='1'):  fileName+='SHAPE'
   else:                 fileName+='COUNT' 	

   FILE = open(fileName+".txt","w")
   for m in MASS:
      print 'Starting mass ' + str(m)
      FILE.writelines("------------------------------------------------------------------------------------\n")
      BestXsec = []
      fileList = commands.getstatusoutput("ls " + OUT + str(m)+"_*.log")[1].split();           
      for f in fileList:
         exp = commands.getstatusoutput("cat " + f + " | grep \"Best fit r: \"")[1];
         if(len(exp)<=0):continue
         try:
            median = float(exp.split()[3])
            uncM = float(exp.split()[4].split('/')[0])
            uncP = float(exp.split()[4].split('/')[1])
            unc = (((median+uncP) + (median-uncM))/(2*median))-1
            if(float(median)<=0.0):continue
            index = int(f[f.rfind("_")+1:f.rfind(".log")])
            ixsecReport="%7.3f @ index=%d %7.3fpb [%7.3f/%7.3f] pt1=%f pt2=%f"%(float(100.0*unc),index,float(median*smXsec),100.0*uncM/median,100.0*uncP/median,cuts.GetBinContent(index,1),cuts.GetBinContent(index,2))
            BestXsec.append(ixsecReport)
         except:
            print 'Failed with %s'%exp
      #sort the limits for this mass
      BestXsec.sort()
      for s in BestXsec:
         FILE.writelines(s+'\n')
         
   #all done
   FILE.close()
   print("file "+fileName+".txt is written: it contains all selection points ordered by best cross section measurement")

######################################################################

elif(phase == 3 ):

   print '#              #'
   print '# FINAL XSEC   #'
   print '#              #'

   fileName = OUT+"/OPTIM_"
   if(shapeBased=='1'):
      fileName+='SHAPE'
   else:
      fileName+='COUNT'
   fileName+=".txt"
      
   #if you want to display more than 3 options edit -m3 field
   cut_lines=commands.getstatusoutput("cat " + fileName)[1].split('\n')
   print '\tOption \t dS/S \tjet 1\tjet 2'
   ictr=1
   for c in cut_lines:
      if(len(c.split())<2): continue
      try :
         print '%d)\t #%s \t %s \t %s \t %s'%(ictr,c.split()[2],c.split()[0],c.split()[6],c.split()[7])
      except:
         continue
      ictr+=1
   print "Which option you want to keep?"

   #save cut chosen
   opt = int(raw_input(">"))
   c=cut_lines[opt]
   pt1Cut=float(c.split()[6].split('=')[1])
   pt2Cut=float(c.split()[7].split('=')[1])
   optCuts="index="+str(opt).rjust(5) + " --> jet1>" + str(pt1Cut).rjust(5) + " jet2>" + str(pt2Cut).rjust(5)+"\n"
   print optCuts
   while True:
        ans = raw_input('Use this fit and compute final limits? (y or n)\n')
        if(ans=='y' or ans == 'Y'): break;
        else:			    sys.exit(0);           
   print 'YES'


   m=0
   list = open(OUT+'list.txt',"w")
   listcuts = open(OUT+'cuts.txt',"w")
   SCRIPT = open(OUT+'/script_mass_'+str(m)+'.sh',"w")
   SCRIPT.writelines('cd ' + CMSSW_BASE + ';\n')
   SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc462")+";\n")
   SCRIPT.writelines("eval `scram r -sh`;\n")
   SCRIPT.writelines('cd ' + CWD + ';\n')
   shapeBasedOpt=''
   if(shapeBased=='1') : shapeBasedOpt='--shape'

   cardsdir=DataCardsDir+"/"+str(m);
   SCRIPT.writelines('mkdir -p ' + cardsdir+';\ncd ' + cardsdir+';\n')
   SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " " + " --syst " + shapeBasedOpt + " --index " + str(opt) + " --json " + jsonUrl + " " + LandSArg +" ;\n")# " --shapeMin " + str(Gzpt.Eval(m,0,"")) +" --shapeMax " + str(Gzm.Eval(m,0,""))  +" ;\n")
   SCRIPT.writelines("sh combineCards.sh;\n")
   SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " card_combined.dat > COMB.log;\n") 
   SCRIPT.writelines("combine -M MaxLikelihoodFit -m " +  str(m) + " --saveNormalizations card_combined.dat > COMB.log;\n")
   SCRIPT.writelines("extractFitNormalization.py mlfit.root hzz2l2v__%sTeV.root > fit.txt;\n"%sqrts)
   SCRIPT.writelines('cd ..;\n\n') 
   SCRIPT.close()
   os.system("bsub -G u_zh -q 2nd 'sh " + OUT+"script_mass_"+str(m)+".sh'")
   if(shapeBased=='1'):   list.writelines('H'+str(m)+'_shape_'+str(opt)+'\n'); 
   else :                  list.writelines('H'+str(m)+'_count_'+str(opt)+'\n');

   listcuts.writelines(optCuts +'\n')
   list.close();
   listcuts.close();

######################################################################

elif(phase == 4 ):

   print '#            #'
   print '# FINAL PLOT #'
   print '#            #'
   ouputName = 'COUNT'
   if(shapeBased=='1'):ouputName='SHAPE'

   os.system("hadd -f "+ouputName+"_LimitTree.root "+DataCardsDir+"/*/higgsCombineTest.Asymptotic.*.root")
   os.system("root -l -b -q plotLimit.C++'(\""+ouputName+"\",\""+ouputName+"_LimitTree.root\", 7 , 5.035 )'")

######################################################################

else:

   help()


	
