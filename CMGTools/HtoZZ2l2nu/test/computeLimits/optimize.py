#!/usr/bin/env python

import os,sys
#import json
#import getopt
import commands
import ROOT
from ROOT import TFile, TH1, TGraph, TCanvas, TF1

def findCutIndex(cut1, hcut1, cut2, hcut2, cut3, hcut3):
   for i in range(1, hcut1.GetXaxis().GetNbins()):
      if(hcut1.GetBinContent(i)<cut1-1):continue;
      if(hcut2.GetBinContent(i)<cut2-5):continue;
      if(hcut3.GetBinContent(i)<cut3-5):continue;
      return i;   
   return hcut1.GetXaxis().GetNbins();
  



if (len(sys.argv) < 1):
	print 'please give in argument the running mode'
	print '1: for phase1 --> submit landS jobs for all selection point'
        print '2: for phase2 --> check the logs to find the optimal selection point --> you can then look at the optimal points yourself and make your choice --> edit phase3 of optimize.py with your optimal points'
        print '3: for phase3 --> after you have written by hand the best signal point for your signal key points, run the full limit plot including systematics'
	print '4: for phase4 --> once all the final limit jobs have been run, use this phase to build the brezilian plot'
	sys.exit(0)

shapeBased = '1'
if(len(sys.argv) > 2):
	shapeBased = sys.argv[2]

file = ROOT.TFile('../plotter2011.root')
cuts1   = file.Get('WW/optim_cut1_met')
cuts2   = file.Get('WW/optim_cut1_mtmin')
cuts3   = file.Get('WW/optim_cut1_mtmax')

CWD = os.getcwd()
OUT = CWD+'/JOBS/'
if(shapeBased=='1'):	OUT+='SHAPE/'
else:		        OUT+='COUNT/'	

os.system('mkdir -p ' + OUT)

MASS = [200,300,400,500,600]
SUBMASS = [200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650];



if( sys.argv[1] == '1' ):
	FILE = open("LIST.txt","w")
	for i in range(1,cuts1.GetXaxis().GetNbins()):
		if(shapeBased=='1' and cuts3.GetBinContent(i)<780):continue
		FILE.writelines("index="+str(i).rjust(5) + " --> met>" + str(cuts1.GetBinContent(i)).rjust(5) + " " + str(cuts2.GetBinContent(i)).rjust(5) + "<mt<"+str(cuts3.GetBinContent(i)).rjust(5) + "\n")

                SCRIPT = open(OUT+'script_'+str(i)+'.sh',"w")
		SCRIPT.writelines('echo "TESTING SELECTION : ' + str(i).rjust(5) + ' --> met>' + str(cuts1.GetBinContent(i)).rjust(5) + ' ' + str(cuts2.GetBinContent(i)).rjust(5) + '<mt<'+str(cuts3.GetBinContent(i)).rjust(5)+'";\n')
	        SCRIPT.writelines('cd ' + CWD + ';\n')
                SCRIPT.writelines("export SCRAM_ARCH=slc5_amd64_gcc434;\n")
                SCRIPT.writelines('echo "TESTC";\n')
		SCRIPT.writelines("eval `scram r -sh`;\n")
		SCRIPT.writelines('cd /tmp/;')
		for m in MASS:
			SCRIPT.writelines("runLandS "+str(m)+" mt_shapes $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root 0 "+shapeBased+" "+str(i)+";\n")
			if(shapeBased=='1'):       SCRIPT.writelines('cat H' +str(m)+'_shape_'+str(i)+'/*.log &> ' +OUT+str(m)+'_'+str(i)+'.log;\n')
			else:                      SCRIPT.writelines('cat H' +str(m)+'_count_'+str(i)+'/*.log &> ' +OUT+str(m)+'_'+str(i)+'.log;\n')
		SCRIPT.close()
      	        print("bsub -q 8nh -J optim"+str(i)+" 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")
		os.system("bsub -q 8nh 'sh " + OUT+"script_"+str(i)+".sh &> "+OUT+"script_"+str(i)+".log'")


	FILE.close()

elif(sys.argv[1] == '2' ):
	print("All selection point will be scanned... it may take several minutes\n")
	fileName = "OPTIM_"
	if(shapeBased=='1'):    fileName+='SHAPE'
	else:			fileName+='COUNT' 	
        FILE = open(fileName+".txt","w")
	for m in MASS:
	        FILE.writelines("mH="+str(m)+":\n")
        	FILE.writelines("------------------------------------------------------------------------------------\n")
		BestLimit = []
		fileList = commands.getstatusoutput("ls " + OUT + str(m)+"_*.log")[1].split();
		for f in fileList:
			exp = commands.getstatusoutput("cat " + f + " | grep BANDS")[1];
			if(len(exp)<=0):continue
			median = exp.split()[6]
			index = int(f[f.rfind("_")+1:f.rfind(".log")])
			BestLimit.append( str(median).rjust(8) + " " + str(index).rjust(5) + " " + str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + " " + str(cuts3.GetBinContent(index)).rjust(5))
		BestLimit.sort()
		for s in BestLimit:
			FILE.writelines(s+"\n")
        FILE.close()
	print("file OPTIM.txt written: it contains all selection point ordered by exp limit")


elif(sys.argv[1] == '3' ):


   Gmet  = ROOT.TGraph(5);
   Gtmin = ROOT.TGraph(5);
   Gtmax = ROOT.TGraph(5);

   
   if(shapeBased=='1'): 
        Gmet .SetPoint(0, 200,  55);
        Gtmin.SetPoint(0, 200, 225);
        Gtmax.SetPoint(0, 200,3000);
        Gmet .SetPoint(1, 300,  55);
        Gtmin.SetPoint(1, 300, 225);
        Gtmax.SetPoint(1, 300,3000);
        Gmet .SetPoint(2, 400,  55);
        Gtmin.SetPoint(2, 400, 225);
        Gtmax.SetPoint(2, 400,3000);
        Gmet .SetPoint(3, 500,  55);
        Gtmin.SetPoint(3, 500, 225);
        Gtmax.SetPoint(3, 500,3000);
        Gmet .SetPoint(4, 600,  55);
        Gtmin.SetPoint(4, 600, 225);
        Gtmax.SetPoint(4, 600,3000);

   else:                  
	Gmet .SetPoint(0, 200,  55);
        Gtmin.SetPoint(0, 200, 225);
        Gtmax.SetPoint(0, 200, 325);
        Gmet .SetPoint(1, 300,  55);
        Gtmin.SetPoint(1, 300, 225);
        Gtmax.SetPoint(1, 300, 325);
        Gmet .SetPoint(2, 400,  55);
        Gtmin.SetPoint(2, 400, 225);
        Gtmax.SetPoint(2, 400, 325);
        Gmet .SetPoint(3, 500,  55);
        Gtmin.SetPoint(3, 500, 225);
        Gtmax.SetPoint(3, 500, 325);
        Gmet .SetPoint(4, 600,  55);
        Gtmin.SetPoint(4, 600, 225);
        Gtmax.SetPoint(4, 600, 325);

#	   Gmet .SetPoint(0, 250,  70);
#          Gtmin.SetPoint(0, 250, 222);
#          Gtmax.SetPoint(0, 250, 272);
#	   Gmet .SetPoint(1, 300,  79);
#          Gtmin.SetPoint(1, 300, 264);
#          Gtmax.SetPoint(1, 300, 331);
#	   Gmet .SetPoint(2, 350,  95);
#          Gtmin.SetPoint(2, 350, 298);
#          Gtmax.SetPoint(2, 350, 393);
#	   Gmet .SetPoint(3, 400, 115);
#          Gtmin.SetPoint(3, 400, 327);
#          Gtmax.SetPoint(3, 400, 460);
#	   Gmet .SetPoint(4, 450, 134);
#          Gtmin.SetPoint(4, 450, 354);
#          Gtmax.SetPoint(4, 450, 531);
#	   Gmet .SetPoint(5, 500, 150);
#          Gtmin.SetPoint(5, 500, 382);
#          Gtmax.SetPoint(5, 500, 605);
#	   Gmet .SetPoint(6, 550, 150);
#	   Gtmin.SetPoint(6, 550, 413);
#  	   Gtmax.SetPoint(6, 550, 684);
#	   Gmet .SetPoint(7, 600, 161);
#	   Gtmin.SetPoint(7, 600, 452);
# 	   Gtmax.SetPoint(7, 600, 767);



   c1 = ROOT.TCanvas("c1", "c1",900,300);
   c1.Divide(3);
   c1.cd(1);
   Gmet.SetMarkerStyle(20);
   Gmet.SetTitle("MET");
   Gmet.Draw("AP");
   Gmet.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gmet.GetYaxis().SetTitle("met cut");
   Fmet  = ROOT.TF1("Fmet", "pol1");
   Gmet.Fit("Fmet");

   c1.cd(2);
   Gtmin.SetMarkerStyle(20);
   Gtmin.SetTitle("MT min");
   Gtmin.Draw("AP");
   Gtmin.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gtmin.GetYaxis().SetTitle("mt_{min} cut");
   Ftmin  = ROOT.TF1("Ftmin", "pol1");
   Gtmin.Fit("Ftmin");

   c1.cd(3);
   Gtmax.SetMarkerStyle(20);
   Gtmax.SetTitle("MT max");
   Gtmax.Draw("AP");
   Gtmax.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gtmax.GetYaxis().SetTitle("mt_{max} cut");
   Ftmax  = ROOT.TF1("Ftmax", "pol1");
   Gtmax.Fit("Ftmax");
   c1.cd(0);
   c1.Update();


   for m in SUBMASS:
      	index = findCutIndex(Fmet.Eval(m), cuts1, Ftmin.Eval(m), cuts2,  Ftmax.Eval(m), cuts3);
	print("best mH="+str(m).rjust(3)+ " met>"+str(int(Fmet.Eval(m))).rjust(5) + " " + str(int(Ftmin.Eval(m))).rjust(5) + "<mt<"+str(int(Ftmax.Eval(m))).rjust(5) ) 
      	print("used mH="+str(m).rjust(3)+ " met>"+str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + "<mt<"+str(cuts3.GetBinContent(index)).rjust(5) )

   while True:
	ans = raw_input('Use this fit and compute final limits? (y or n)\n')
	if(ans=='y' or ans == 'Y'): break;
	else:			    sys.exit(0);
	
   print 'YES'
   list = open(OUT+'list.txt',"w")
   for m in SUBMASS:
        index = findCutIndex(Fmet.Eval(m), cuts1, Ftmin.Eval(m), cuts2,  Ftmax.Eval(m), cuts3);
        SCRIPT = open(OUT+'script_mass_'+str(m)+'.sh',"w")
	SCRIPT.writelines('cd ' + CWD + ';\n')
        SCRIPT.writelines("export SCRAM_ARCH=slc5_amd64_gcc434;\n")
	SCRIPT.writelines("eval `scram r -sh`;\n")
	SCRIPT.writelines("runLandS " + str(m) + " mt_shapes " + " $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root " + " 0 " + shapeBased + " " + str(index) + ";\n")
	SCRIPT.close()
	os.system("bsub -q 8nh 'sh " + OUT+"script_mass_"+str(m)+".sh'")

	if(shapeBased=='1'):   list.writelines('H'+str(m)+'_shape_'+str(index)+'\n'); 
	else:                  list.writelines('H'+str(m)+'_count_'+str(index)+'\n');
   list.close();

elif(sys.argv[1] == '4' ):
   list = open(OUT+'list.txt',"r")
   files = ""
   for m in SUBMASS:   
	line = list.readline().split()[0]
	line+="/*m2lnQ.root"
	print line
	files += " " + commands.getstatusoutput("ls " + line)[1];
   list.close();
   os.system("root -l -b -q plotLimit.C++'(\""+files+"\")'") 
   os.system("rm *.gif");

else:
	print "unknown running mode"


	
