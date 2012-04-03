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
inUrl='../plotter2011.root'
CWD=os.getcwd()
phase=-1
jsonUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samplesNoHWW.json'
CMSSW_BASE=os.environ.get('CMSSW_BASE')
 
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
         SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName  + " --in " + inUrl + " " + shapeBasedOpt + " --index " + str(i) + " --json " + jsonUrl +";\n")
         if(shapeBased=='1'):
            SCRIPT.writelines('cat H' +str(m)+'_shape_'+str(i)+'/combined/*.log &> ' +OUT+str(m)+'_'+str(i)+'.log;\n')
         else:
            SCRIPT.writelines('cat H' +str(m)+'_count_'+str(i)+'/combined/*.log &> ' +OUT+str(m)+'_'+str(i)+'.log;\n')
      SCRIPT.close()
      print("bsub -q 8nh -J optim"+str(i)+" 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")
      os.system("bsub -q 8nh 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")

   FILE.close()
      
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
   npts=len(SUBMASS)
   Gmet  = ROOT.TH1F('Gmet','min E_{T}^{miss}; m_{H} (GeV/c^{2}); min E_{T}^{miss}',npts,0,npts); GmetGr  = ROOT.TGraph(npts);
   Gtmin = ROOT.TH1F('Gmin','min E_{T}^{miss}; m_{H} (GeV/c^{2}); min E_{T}^{miss}',npts,0,npts); GtminGr = ROOT.TGraph(npts);
   Gtmax = ROOT.TH1F('Gmax','max E_{T}^{miss}; m_{H} (GeV/c^{2}); max E_{T}^{miss}',npts,0,npts); GtmaxGr = ROOT.TGraph(npts);
 
   fileName = OUT+"/OPTIM_"
   if(shapeBased=='1'):
      fileName+='SHAPE'
   else:
      fileName+='COUNT'
   fileName+=".txt"
      
   mi=0
   for m in MASS:

      #if you want to display more than 3 options edit -m3 field
      cut_lines=commands.getstatusoutput("cat " + fileName + " | grep 'mH="+str(m)+"' -m3")[1].split('\n')
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
      Gmet .SetBinContent(mi+1, metCut);   Gmet.GetXaxis().SetBinLabel(mi+1,str(m));  GmetGr.SetPoint(mi,m,metCut)
      Gtmin.SetBinContent(mi+1, mtMinCut); Gtmin.GetXaxis().SetBinLabel(mi+1,str(m)); GtminGr.SetPoint(mi,m,mtMinCut)
      Gtmax.SetBinContent(mi+1, mtMaxCut); Gtmax.GetXaxis().SetBinLabel(mi+1,str(m)); GtmaxGr.SetPoint(mi,m,mtMaxCut)
      mi+=1

   #display cuts chosen
   c1 = ROOT.TCanvas("c1", "c1",500,500);
   ROOT.gROOT.SetStyle('Plain')
   ROOT.gStyle.SetOptStat(False);
      
   Gtmin.SetMarkerStyle(21);
   Gtmin.SetMarkerColor(1);
   Gtmin.SetLineColor(1);
   Gtmin.SetLineStyle(1);
   Gtmin.Draw("hist");
   Gtmax.SetMarkerStyle(21);
   Gtmax.SetMarkerColor(1);
   Gtmax.SetLineColor(1);
   Gtmax.SetLineStyle(9);
   Gtmax.Draw("histsame");
   Gtmin.GetYaxis().SetRangeUser(0.95*Gtmin.GetMinimum(),1.05*Gtmax.GetMaximum())
   c1.Modified()
   c1.Update()
   
   Gmet.SetMarkerStyle(20);
   Gmet.SetMarkerColor(2);
   Gmet.SetLineColor(2);
   rightmax = 1.1*Gmet.GetMaximum();
   scale = ROOT.gPad.GetUymax()/rightmax;
   Gmet.Scale(scale);
   Gmet.Draw("histsame")
   
   axis =ROOT.TGaxis(ROOT.gPad.GetUxmax(),ROOT.gPad.GetUymin(),ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax(),0,rightmax,510,'+L');
   axis.SetTitle(Gmet.GetYaxis().GetTitle())
   axis.SetLineColor(2);
   axis.SetLabelColor(2);
   axis.Draw();

   c1.Modified()
   c1.Update();
   c1.SaveAs("OptimizedCuts.png")
   

   #run limits for the cuts chosen (for intermediate masses use spline interpolation)
   for m in SUBMASS:
      	index = findCutIndex(GmetGr.Eval(m,0,"S"), cuts1, GtminGr.Eval(m,0,"S"), cuts2,  GtmaxGr.Eval(m,0,"S"), cuts3);
	#print("best mH="+str(m).rjust(3)+ " met>"+str(int(GmetGr.Eval(m,0,"S"))).rjust(5) + " " + str(int(GtminGr.Eval(m,0,"S"))).rjust(5) + "<mt<"+str(int(GtmaxGr.Eval(m,0,"S"))).rjust(5) ) 
      	print("mH="+str(m).rjust(3)+ " met>"+str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + "<mt<"+str(cuts3.GetBinContent(index)).rjust(5) )

   while True:
	ans = raw_input('Use this fit and compute final limits? (y or n)\n')
	if(ans=='y' or ans == 'Y'): break;
	else:			    sys.exit(0);
	
   print 'YES'
   list = open(OUT+'list.txt',"w")
   for m in SUBMASS:
        index = findCutIndex(GmetGr.Eval(m,0,"S"), cuts1, GtminGr.Eval(m,0,"S"), cuts2,  GtmaxGr.Eval(m,0,"S"), cuts3);
        SCRIPT = open(OUT+'/script_mass_'+str(m)+'.sh',"w")
        SCRIPT.writelines('cd ' + CMSSW_BASE + '/src;\n')
        SCRIPT.writelines("export SCRAM_ARCH=slc5_amd64_gcc434;\n")
	SCRIPT.writelines("eval `scram r -sh`;\n")
        SCRIPT.writelines('cd ' + OUT + ';\n')
        shapeBasedOpt=''
        if(shapeBased=='1') : shapeBasedOpt='--shape'
	SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " --syst " + shapeBasedOpt + " --index " + str(index) + " --json " + jsonUrl + ";\n")
	SCRIPT.close()
	os.system("bsub -q 8nh 'sh " + OUT+"script_mass_"+str(m)+".sh'")
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
	line = OUT+'/'+list.readline().split()[0]+'/combined/*m2lnQ.root'
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


	
