#!/usr/bin/env python

import os,sys
#import json
import getopt
import commands
import ROOT
from ROOT import TFile, TGraph, TCanvas, TF1, TH1

#default values
shapeBased='1'
shapeName='mt_shapes'
inUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root'
CWD=os.getcwd()
phase=-1
jsonUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json'
CMSSW_BASE=os.environ.get('CMSSW_BASE')
LandSArg=''
 
def help() :
   print '\n\033[92m optimize.py \033[0m \n'
   print ' -p phase (no default value is assigned)'
   print '\t 1 --> submit landS jobs for all selection point'
   print '\t 2 --> check the logs to find the optimal selection point'
   print '\t      from the ouptut of the logs you can search for the optimal points yourself ;)'
   print '\t      and edit phase3 of this script with your optimal points (note: change this to be given as input)'
   print '\t 3 --> after you have written by hand the best signal point for your signal key points'
   print '\t      run the full limit computation including systematics'
   print '\t 4 --> once all the final limit jobs have been run, use this phase to build the brazilian flag plot'
   print ' -m mode (default='+shapeBased+')'
   print '\t 0 --> cut and count based analysis'
   print '\t 1 --> shape based analysis'
   print ' -s shapename (default='+shapeName+')'
   print ' -i inputfile (default='+inUrl+')'
   print ' -o output (default='+CWD+')'
   print ' -j jsonfile (default='+jsonUrl+')'
   print '\nUsage example: \033[92m python optimize.py -m 0 -i ~/work/plotter.root -o ~/work/ -p 1 \033[0m'
   print '\nNote: CMSSW_BASE must be set when launching optimize.py (current values is: ' + CMSSW_BASE + ')\n' 
   
#parse the options
try:
   # retrive command line options
   shortopts  = "p:m:i:s:j:o:h?"
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
      
if(phase<0 or len(CMSSW_BASE)==0):
   help()
   sys.exit(1)


#auxiliary function
def findCutIndex(cut1, hcut1, cut2, hcut2, cut3, hcut3):
   for i in range(1, hcut1.GetXaxis().GetNbins()):
      if(hcut1.GetBinContent(i)<cut1-1):continue;
      if(hcut2.GetBinContent(i)<cut2-5):continue;
      if(hcut3.GetBinContent(i)<cut3-5):continue;
      return i;   
   return hcut1.GetXaxis().GetNbins();

#prepare the output
OUT = CWD+'/JOBS/'
if(shapeBased=='1'): OUT+='SHAPE/'
else:		     OUT+='COUNT/'	
os.system('mkdir -p ' + OUT)

#get the cuts
file = ROOT.TFile(inUrl)
cuts1   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_met') 
cuts2   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_mtmin') 
cuts3   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_mtmax') 

#MASS = [200,300,400,500,600]
#SUBMASS = [200,300,400,500,600]
MASS = [200,250, 300,350, 400,450, 500,550, 600]
SUBMASS = [200,250, 300,350, 400,450, 500,550, 600]
#SUBMASS = [200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650];


######################################################################

if( phase == 1 ):

   print '#                                  #'
   print '# RUN LIMITS FOR ALL POSSIBLE CUTS #'
   print '#                                  #'

   commandToRun = []

   FILE = open(OUT+"/LIST.txt","w")
   for i in range(1,cuts1.GetNbinsX()):
      if(shapeBased=='1' and cuts3.GetBinContent(i)<780):continue
      FILE.writelines("index="+str(i).rjust(5) + " --> met>" + str(cuts1.GetBinContent(i)).rjust(5) + " " + str(cuts2.GetBinContent(i)).rjust(5) + "<mt<"+str(cuts3.GetBinContent(i)).rjust(5) + "\n")

      #create wrappper script for each set of cuts ans submit it
      SCRIPT = open(OUT+'script_'+str(i)+'.sh',"w")
      SCRIPT.writelines('echo "TESTING SELECTION : ' + str(i).rjust(5) + ' --> met>' + str(cuts1.GetBinContent(i)).rjust(5) + ' ' + str(cuts2.GetBinContent(i)).rjust(5) + '<mt<'+str(cuts3.GetBinContent(i)).rjust(5)+'";\n')
      SCRIPT.writelines('cd ' + CMSSW_BASE + '/src;\n')
      SCRIPT.writelines("export SCRAM_ARCH=slc5_amd64_gcc434;\n")
      SCRIPT.writelines("eval `scram r -sh`;\n")
      SCRIPT.writelines('cd /tmp/;\n')
      for m in MASS:
         shapeBasedOpt=''
         if(shapeBased=='1') : shapeBasedOpt='--shape'
         SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName  + " --in " + inUrl + " " + shapeBasedOpt + " --index " + str(i) + " --json " + jsonUrl +" --fast " + LandSArg + " ;\n")
         if(shapeBased=='1'):
            SCRIPT.writelines('cat H' +str(m)+'_shape_'+str(i)+'/combined/*.log | grep BAND &> ' +OUT+str(m)+'_'+str(i)+'.log;\n')
         else:
            SCRIPT.writelines('cat H' +str(m)+'_count_'+str(i)+'/combined/*.log | grep BAND &> ' +OUT+str(m)+'_'+str(i)+'.log;\n')
      SCRIPT.close()
      commandToRun.append("bsub -q 8nh -J optim"+str(i)+" 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")
#      print("bsub -q 8nh -J optim"+str(i)+" 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")
#      os.system("bsub -q 8nh 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")

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
      BestLimit = []
      fileList = commands.getstatusoutput("ls " + OUT + str(m)+"_*.log")[1].split();		
      for f in fileList:
         exp = commands.getstatusoutput("cat " + f + " | grep BANDS")[1];
         if(len(exp)<=0):continue
         median = exp.split()[6]
         index = int(f[f.rfind("_")+1:f.rfind(".log")])
         BestLimit.append("mH="+str(m)+ " --> " + str(median).rjust(8) + " " + str(index).rjust(5) + " " + str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + " " + str(cuts3.GetBinContent(index)).rjust(5))

      #sort the limits for this mass
      BestLimit.sort()
      for s in BestLimit:
         FILE.writelines(s+"\n")

   #all done
   FILE.close()
   print("file "+fileName+".txt is written: it contains all selection points ordered by exp limit")

######################################################################

elif(phase == 3 ):

   print '#              #'
   print '# FINAL LIMITS #'
   print '#              #'
   Gmet  = ROOT.TGraph(len(SUBMASS));
   Gtmin = ROOT.TGraph(len(SUBMASS));
   Gtmax = ROOT.TGraph(len(SUBMASS));

 
   fileName = OUT+"/OPTIM_"
   if(shapeBased=='1'):
      fileName+='SHAPE'
   else:
      fileName+='COUNT'
   fileName+=".txt"
      
   mi=0
   for m in MASS:

      #if you want to display more than 3 options edit -m3 field
      cut_lines=commands.getstatusoutput("cat " + fileName + " | grep 'mH="+str(m)+"' -m10")[1].split('\n')
      print 'mH='+str(m)+'\tOption \tR \tmin MET\tMT range' 
      ictr=1
      for c in cut_lines:
         print '\t #'+ str(ictr) + '\t' + c.split()[2] + '\t' + c.split()[4] + '\t(' + c.split()[5] + '-' + c.split()[6]+ ')'
         ictr+=1
      print "Which option you want to keep?"
      opt = int(raw_input(">"))-1

      #save cut chosen
      metCut=float(cut_lines[opt].split()[4])
      mtMinCut=float(cut_lines[opt].split()[5])
      mtMaxCut=float(cut_lines[opt].split()[6])
      Gmet .SetPoint(mi, m, metCut);
      Gtmin.SetPoint(mi, m, mtMinCut);
      Gtmax.SetPoint(mi, m, mtMaxCut);
      mi+=1

   #display cuts chosen
   c1 = ROOT.TCanvas("c1", "c1",900,300);
   ROOT.gROOT.SetStyle('Plain')
   ROOT.gStyle.SetOptStat(False);

   c1 = ROOT.TCanvas("c1", "c1",900,300);
   c1.Divide(3);
   c1.cd(1);
   Gmet.SetMarkerStyle(20);
   Gmet.SetTitle("MET");
   Gmet.Draw("APC");
   Gmet.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gmet.GetYaxis().SetTitle("met cut");

   c1.cd(2);
   Gtmin.SetMarkerStyle(20);
   Gtmin.SetTitle("MT min");
   Gtmin.Draw("APC");
   Gtmin.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gtmin.GetYaxis().SetTitle("mt_{min} cut");

   c1.cd(3);
   Gtmax.SetMarkerStyle(20);
   Gtmax.SetTitle("MT max");
   Gtmax.Draw("APC");
   Gtmax.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gtmax.GetYaxis().SetTitle("mt_{max} cut");
   c1.cd(0);
   c1.Update();
   c1.SaveAs("OptimizedCuts.png")

   #run limits for the cuts chosen (for intermediate masses use spline interpolation)
   for m in SUBMASS:
      	index = findCutIndex(Gmet.Eval(m,0,"S"), cuts1, Gtmin.Eval(m,0,"S"), cuts2,  Gtmax.Eval(m,0,"S"), cuts3);
	#print("best mH="+str(m).rjust(3)+ " met>"+str(int(Gmet.Eval(m,0,"S"))).rjust(5) + " " + str(int(Gtmin.Eval(m,0,"S"))).rjust(5) + "<mt<"+str(int(Gtmax.Eval(m,0,"S"))).rjust(5) ) 
      	print("mH="+str(m).rjust(3)+ " met>"+str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + "<mt<"+str(cuts3.GetBinContent(index)).rjust(5) )

   while True:
	ans = raw_input('Use this fit and compute final limits? (y or n)\n')
	if(ans=='y' or ans == 'Y'): break;
	else:			    sys.exit(0);
	
   print 'YES'
   list = open(OUT+'list.txt',"w")
   for m in SUBMASS:
        index = findCutIndex(Gmet.Eval(m,0,"S"), cuts1, Gtmin.Eval(m,0,"S"), cuts2,  Gtmax.Eval(m,0,"S"), cuts3);
        SCRIPT = open(OUT+'/script_mass_'+str(m)+'.sh',"w")
        SCRIPT.writelines('cd ' + CMSSW_BASE + ';\n')
        SCRIPT.writelines("export SCRAM_ARCH=slc5_amd64_gcc434;\n")
        SCRIPT.writelines("eval `scram r -sh`;\n")
        SCRIPT.writelines('cd ' + CWD + ';\n')
        shapeBasedOpt=''
        if(shapeBased=='1') : shapeBasedOpt='--shape'
	SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " --syst " + shapeBasedOpt + " --index " + str(index) + " --json " + jsonUrl + " " + LandSArg + " ;\n")
	SCRIPT.close()
#	os.system("bsub -q 8nh 'sh " + OUT+"script_mass_"+str(m)+".sh'")
        os.system("bsub -q 2nd 'sh " + OUT+"script_mass_"+str(m)+".sh'")
	if(shapeBased=='1'):   list.writelines('H'+str(m)+'_shape_'+str(index)+'\n'); 
	else:                  list.writelines('H'+str(m)+'_count_'+str(index)+'\n');
   list.close();

######################################################################

elif(phase == 4 ):

   print '#            #'
   print '# FINAL PLOT #'
   print '#            #'

   list = open(OUT+'/list.txt',"r")
   files = ""
   for m in SUBMASS:   
	#line = CWD+'/'+list.readline().split()[0]+'/combined/*m2lnQ.root'
        line = list.readline().split()[0]+'/combined/*m2lnQ.root'
	print line
	out = commands.getstatusoutput("ls " + line)[1] 
	if(out.find("No such file or directory")>=0):continue
	files += " " + out;
   list.close();
   os.system("root -l -b -q plotLimit.C++'(\""+files+"\")'") 
   os.system("rm *.gif");

######################################################################

else:

   help()


	
