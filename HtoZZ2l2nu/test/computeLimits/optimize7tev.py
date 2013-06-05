#!/usr/bin/env python

import os,sys
#import json
import getopt
import commands
import ROOT
from ROOT import TFile, TGraph, TCanvas, TF1, TH1

#default values
shapeName='mt_shapes --histoVBF met_shapes'
#inUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root'
inUrl='~psilva/public/HtoZZ/plotter_29April_2011.root'
CWD=os.getcwd()
phase=-1
jsonUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2011.json'
CMSSW_BASE=os.environ.get('CMSSW_BASE')


MASS = [200,250, 300, 350, 400, 450, 500, 550, 600, 650, 750, 800, 900, 950, 1000] #700 and 850 is missing
#SUBMASS = [200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262, 264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286, 288, 290, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 360, 370, 380, 390, 400, 420, 440, 460, 480, 500, 520, 540, 560, 580, 600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 1000]
SUBMASS = [200,250, 300,350,400,450,500,550,600, 650, 700, 750, 800, 850, 900, 950, 1000]



LandSArgCommonOptions=" --indexvbf 9 --subNRB --subDY $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/computeLimits_LHCP_13_04_02/gamma_half_2011_5fb_Moriond.root --interf --BackExtrapol "
signalSuffixVec = []
OUTName         = []
LandSArgOptions = []

###################  #7TeV cut&count combined, GG, QQ
signalSuffixVec += ["", "_cp0.10_brn0.00", "_cp0.20_brn0.00", "_cp0.30_brn0.00", "_cp0.40_brn0.00", "_cp0.50_brn0.00", "_cp0.60_brn0.00", "_cp0.80_brn0.00", "_cp1.00_brn0.00", "", "_cp0.10_brn0.00", "_cp0.20_brn0.00", "_cp0.30_brn0.00", "_cp0.40_brn0.00", "_cp0.50_brn0.00", "_cp0.60_brn0.00", "_cp0.80_brn0.00", "_cp1.00_brn0.00", "", "_cp0.10_brn0.00", "_cp0.20_brn0.00", "_cp0.30_brn0.00", "_cp0.40_brn0.00", "_cp0.50_brn0.00", "_cp0.60_brn0.00", "_cp0.80_brn0.00", "_cp1.00_brn0.00"];
OUTName         += ["CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_GG", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ", "CC7TeV_QQ"]
LandSArgOptions += [" --bins eq0jets,geq1jets,vbf       --systpostfix _7TeV " for i in range(9)]
LandSArgOptions += [" --bins eq0jets,geq1jets --skipQQH --systpostfix _7TeV " for i in range(9)]
LandSArgOptions += [" --bins vbf              --skipGGH --systpostfix _7TeV " for i in range(9)]

###################  #7TeV shape combined, GG, QQ
signalSuffixVec += ["", "_cp0.10_brn0.00", "_cp0.20_brn0.00", "_cp0.30_brn0.00", "_cp0.40_brn0.00", "_cp0.50_brn0.00", "_cp0.60_brn0.00", "_cp0.80_brn0.00", "_cp1.00_brn0.00", "", "_cp0.10_brn0.00", "_cp0.20_brn0.00", "_cp0.30_brn0.00", "_cp0.40_brn0.00", "_cp0.50_brn0.00", "_cp0.60_brn0.00", "_cp0.80_brn0.00", "_cp1.00_brn0.00", "", "_cp0.10_brn0.00", "_cp0.20_brn0.00", "_cp0.30_brn0.00", "_cp0.40_brn0.00", "_cp0.50_brn0.00", "_cp0.60_brn0.00", "_cp0.80_brn0.00", "_cp1.00_brn0.00"];
OUTName         += ["SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_GG", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ", "SB7TeV_QQ"]
LandSArgOptions += [" --bins eq0jets,geq1jets,vbf       --systpostfix _7TeV --shape " for i in range(9)]
LandSArgOptions += [" --bins eq0jets,geq1jets --skipQQH --systpostfix _7TeV --shape " for i in range(9)]
LandSArgOptions += [" --bins vbf              --skipGGH --systpostfix _7TeV --shape " for i in range(9)]


signalSuffixVec += ["", "", ""];
OUTName         += ["CC7TeV_CAT1", "CC7TeV_CAT2", "CC7TeV_CAT3"]
LandSArgOptions += [" --bins eq0jets       --systpostfix _7TeV "]
LandSArgOptions += [" --bins geq1jets      --systpostfix _7TeV "]
LandSArgOptions += [" --bins vbf           --systpostfix _7TeV "]


signalSuffixVec += ["", "", ""];
OUTName         += ["SB7TeV_CAT1", "SB7TeV_CAT2", "SB7TeV_CAT3"]
LandSArgOptions += [" --bins eq0jets       --systpostfix _7TeV --shape "]
LandSArgOptions += [" --bins geq1jets      --systpostfix _7TeV --shape "]
LandSArgOptions += [" --bins vbf           --systpostfix _7TeV --shape "]




#signalSuffixVec = [""]
#OUTName         = ["CC7TeV"]
#LandSArgOptions = [" --bins eq0jets,geq1jets,vbf       --systpostfix _7TeV "]


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
   shortopts  = "p:f:m:i:s:j:o:h?"
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
   elif o in('-i'): inUrl = a
   elif o in('-p'): phase = int(a)
   elif o in('-o'): CWD=a
   elif o in('-j'): jsonUrl=a
   elif o in('-s'): shapeName=a
   elif o in('-f'): cutList=a
      
if(phase<0 or len(CMSSW_BASE)==0):
   help()
   sys.exit(1)


#auxiliary function
def findCutIndex(cut1, hcut1, cut2, hcut2, cut3, hcut3):
   for i in range(1, hcut1.GetXaxis().GetNbins()):
      if(hcut1.GetBinContent(i)<cut1-1):continue;
#      if(hcut2.GetBinContent(i)<cut2-5):continue;
#      if(hcut3.GetBinContent(i)<cut3-5):continue;
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

#Loop over all configurations
iConf = -1
for signalSuffix in signalSuffixVec :
   iConf+=1;

   LandSArg = LandSArgCommonOptions + ' ' + LandSArgOptions[iConf];
   if(signalSuffix != ""):LandSArg+=' --signalSufix \"' + signalSuffix +'\" '
   DataCardsDir='cards_'+OUTName[iConf]+signalSuffix


   #prepare the output
   OUT = CWD+'/JOBS/'+OUTName[iConf]+signalSuffix+'/'
   os.system('mkdir -p ' + OUT)

   #get the cuts
   file = ROOT.TFile(inUrl)
   cuts1   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_met') 
   cuts2   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_mtmin') 
   cuts3   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_mtmax') 


   ######################################################################

   if( phase == 1 ):
      print '# RUN LIMITS FOR ALL POSSIBLE CUTS  for ' + DataCardsDir + '#\n'
      commandToRun = []

      FILE = open(OUT+"/LIST.txt","w")
      for i in range(1,cuts1.GetNbinsX()):
         #only run the optimization in step of 10GeV in MET (needed to reduce the number of points!)
         if(cuts1.GetBinContent(i)%10!=0): continue;

         #X axis of the 2D histograms does not contains the MT variation anymore -> not a big deal we can anyway cut on it via the --shapeMin/--shapeMax arguments of runLandS.
         for mtmin in range(125,550,25):
            mtmin_ = mtmin
            if(mtmin_<=125):mtmin_=0
            if(mtmin_>350 and mtmin_%50!=0):continue;
   #         if(mtmin_<450):continue #ONLY FOR HIGH MASS !!!!!!!!!!!!!!!!!
            for mtmax in range(mtmin+50,mtmin+500,25):
               mtmax_ = mtmax
               if(mtmax_>=mtmin_+455):mtmax_=9000 
               if(mtmin_==0 and mtmax_!=9000):continue;
               if(mtmin_>350 and mtmax_%100!=0):continue;
               if(mtmax_-mtmin_>200 and mtmax_%50!=0):continue;
               #if(mtmax_!=9000):continue; #ONLY FOR HIGH MASS !!!!!!!!!!!!!!!!!
               FILE.writelines("index="+str(i).rjust(5) + " --> met>" + str(cuts1.GetBinContent(i)).rjust(5) + " " + str(mtmin_).rjust(5) + '<mt<'+str(mtmax_).rjust(5) + "\n")

               #create wrappper script for each set of cuts ans submit it
               SCRIPT = open(OUT+'script_'+str(i)+'_'+str(mtmin_)+'_'+str(mtmax_)+'.sh',"w")
               SCRIPT.writelines('echo "TESTING SELECTION : ' + str(i).rjust(5) + ' --> met>' + str(cuts1.GetBinContent(i)).rjust(5) + ' ' + str(mtmin_) + '<mt<'+str(mtmax_)+'";\n')
               SCRIPT.writelines('cd ' + CMSSW_BASE + '/src;\n')
               SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc434")+";\n")
               SCRIPT.writelines("eval `scram r -sh`;\n")
               SCRIPT.writelines('cd /tmp/;\n')
               for m in MASS:
                  shapeBasedOpt=''
                  cardsdir = 'H'+ str(m);
                  SCRIPT.writelines('mkdir -p ' + cardsdir+';\ncd ' + cardsdir+';\n')
                  SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " --syst " + shapeBasedOpt + " --index " + str(i)     + " --json " + jsonUrl +" --fast " + " --shapeMin " + str(mtmin_) + " --shapeMax " + str(mtmax_) + " " + LandSArg + " ;\n")
                  SCRIPT.writelines("sh combineCards.sh;\n")
                  SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " --run expected card_combined.dat > COMB.log;\n")
                  SCRIPT.writelines('tail -n 100 COMB.log > ' +OUT+str(m)+'_'+str(i)+'_'+str(mtmin_)+'_'+str(mtmax_)+'.log;\n')
                  SCRIPT.writelines('cd ..;\n\n')
               SCRIPT.close()
               commandToRun.append("bsub -G u_zh -q 8nh -J optim"+str(i)+'_'+str(mtmin_)+'_'+str(mtmax_) + " 'sh " + OUT+"script_"+str(i)+'_'+str(mtmin_)+'_'+str(mtmax_)+".sh &> "+OUT+"script_"+str(i)+'_'+str(mtmin_)+'_'+str(mtmax_)+".log'")
      FILE.close()

      for c in commandToRun:
         print(c)
         os.system(c);

         
   ######################################################################
   elif(phase == 2):
      print '# SCANNING ALL SETS OF CUTS  for ' + DataCardsDir + ' (you may want to go for a coffee...)#\n'
      
      fileName = OUT + "/OPTIM" + signalSuffix
      FILE = open(fileName+".txt","w")
      for m in MASS:
         print 'Starting mass ' + str(m)
         FILE.writelines("------------------------------------------------------------------------------------\n")
         BestLimit = []
         fileList = commands.getstatusoutput("ls " + OUT + str(m)+"_*.log")[1].split();           
         for f in fileList:
            exp = commands.getstatusoutput("cat " + f + " | grep \"Expected 50.0%\"")[1];
            if(len(exp)<=0):continue
            median = exp.split()[4]
            if(float(median)<=0.0):continue
            f = f.replace(".log","")
            fields = f.split('_')
            N = len(fields)
            index = fields[N-3] 
            BestLimit.append("mH="+str(m)+ " --> " + ('%07.3f' % float(median)) + " " + str(cuts1.GetBinContent(int(index))).rjust(5) + " " + str(fields[N-2]).rjust(5) + " " + str(fields[N-1]).rjust(5))

         #sort the limits for this mass
         BestLimit.sort()
         for s in BestLimit:
            FILE.writelines(s+"\n")

      #all done
      FILE.close()
      print("file "+fileName+".txt is written: it contains all selection points ordered by exp limit")

   ######################################################################

   elif(phase == 3 ):

      print '# FINAL LIMITS  for ' + DataCardsDir + '#\n'
      Gmet  = ROOT.TGraph(len(SUBMASS));
      Gtmin = ROOT.TGraph(len(SUBMASS));
      Gtmax = ROOT.TGraph(len(SUBMASS));


      if(cutList=='') :
         fileName = OUT+"/OPTIM"+signalSuffix
         fileName+=".txt"
         
         mi=0
         for m in MASS:
            #if you want to display more than 3 options edit -m3 field
            cut_lines=commands.getstatusoutput("cat " + fileName + " | grep 'mH="+str(m)+"' -m20")[1].split('\n')
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

         Gmet.Set(mi);
         Gtmin.Set(mi);
         Gtmax.Set(mi);

         #run limits for the cuts chosen (for intermediate masses use spline interpolation)
         for m in SUBMASS:
              index = findCutIndex(Gmet.Eval(m,0,""), cuts1, Gtmin.Eval(m,0,""), cuts2,  Gtmax.Eval(m,0,""), cuts3);
      #        print("mH="+str(m).rjust(3)+ " met>"+str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + "<mt<"+str(cuts3.GetBinContent(index)).rjust(5) )

         while True:
              ans = raw_input('Use this fit and compute final limits? (y or n)\n')
              if(ans=='y' or ans == 'Y'): break;
              else:			    sys.exit(0);           
         print 'YES'

      else :
         mi=0
         f= open(cutList,'r')
         for line in f :
            vals=line.split(' ')
            Gmet .SetPoint(mi, float(vals[0]), float(vals[1]));
            Gtmin.SetPoint(mi, float(vals[0]), float(vals[2]));
            Gtmax.SetPoint(mi, float(vals[0]), float(vals[3]));
            mi+=1
         f.close()
         Gmet.Set(mi);
         Gtmin.Set(mi);
         Gtmax.Set(mi);



      list = open(OUT+'list.txt',"w")
      listcuts = open(OUT+'cuts.txt',"w")
      for m in SUBMASS:
           index = findCutIndex(Gmet.Eval(m,0,""), cuts1, Gtmin.Eval(m,0,""), cuts2,  Gtmax.Eval(m,0,""), cuts3);
           SCRIPT = open(OUT+'/script_mass_'+str(m)+'.sh',"w")
           SCRIPT.writelines('cd ' + CMSSW_BASE + ';\n')
           SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc434")+";\n")
           SCRIPT.writelines("eval `scram r -sh`;\n")
           SCRIPT.writelines('cd ' + CWD + ';\n')
           shapeBasedOpt=''

           SideMassesArgs = ' '
           SideMasses = findSideMassPoint(m)
           if(not (SideMasses[0]==SideMasses[1])):
              #print "Side Mass for mass " + str(m) + " are " + str(SideMasses[0]) + " and " + str(SideMasses[1])
              Lindex = findCutIndex(Gmet.Eval(SideMasses[0],0,""), cuts1, Gtmin.Eval(SideMasses[0],0,""), cuts2,  Gtmax.Eval(SideMasses[0],0,""), cuts3);
              Rindex = findCutIndex(Gmet.Eval(SideMasses[1],0,""), cuts1, Gtmin.Eval(SideMasses[1],0,""), cuts2,  Gtmax.Eval(SideMasses[1],0,""), cuts3);
              #print "cutIndex for sideBand are " + str(Lindex) + " and " + str(Rindex) 
              SideMassesArgs += "--mL " + str(SideMasses[0]) + " --mR " + str(SideMasses[1]) + " --indexL " + str(Lindex) +  " --indexR " + str(Rindex) + " "


           cutStr = " --rebin 2 --shapeMin " + str(Gtmin.Eval(m,0,"")) +" --shapeMax " + str(Gtmax.Eval(m,0,""));
           if(OUTName[iConf].find("SB")>=0):cutStr = " --rebin 8 "; #not cuts on the shape itself

           cardsdir=DataCardsDir+"/"+('%04.0f' % float(m));
           SCRIPT.writelines('mkdir -p ' + cardsdir+';\ncd ' + cardsdir+';\n')
           SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " " + " --syst " + shapeBasedOpt + " --index " + str(index) + " --json " + jsonUrl + " " + SideMassesArgs + " " + LandSArg + cutStr  +" &> LOG;\n")
           SCRIPT.writelines("sh combineCards.sh;\n")
           SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + "  card_combined.dat > COMB.log;\n") 
           SCRIPT.writelines("combine -M MaxLikelihoodFit -m " +  str(m) + " --saveNormalizations card_combined.dat;\n")
           SCRIPT.writelines("extractFitNormalization.py mlfit.root hzz2l2v_"+str(m)+"_?TeV.root > fit.txt;\n")
           SCRIPT.writelines('cd ..;\n\n') 
           SCRIPT.close()
           os.system("bsub -G u_zh -q 8nh 'sh " + OUT+"script_mass_"+str(m)+".sh'")
           list.writelines('H'+str(m)+'_'+str(index)+'\n');
           listcuts.writelines(str(m)+' ' + str(Gmet.Eval(m,0,"")) + ' ' + str(Gtmin.Eval(m,0,""))+' '+ str(Gtmax.Eval(m,0,"")) +'\n');
      list.close();
      listcuts.close();

   ######################################################################

   elif(phase == 4 ):
      print '# FINAL PLOT for ' + DataCardsDir + '#\n'
      os.system("hadd -f "+DataCardsDir+"/LimitTree.root "+DataCardsDir+"/*/higgsCombineTest.Asymptotic.*.root > /dev/null")
      if(LandSArg.find('skip')<0):
         os.system("root -l -b -q plotLimit.C+'(\""+DataCardsDir+"/Stength_\",\""+DataCardsDir+"/LimitTree.root\",\"\",  true, false, 7 , 5.035 )'")
      else:
         os.system("getXSec "+DataCardsDir+"/XSecs.txt "+DataCardsDir+"/*/Efficiency.tex")
         os.system("root -l -b -q plotLimit.C+'(\""+DataCardsDir+"/Stength_\",\""+DataCardsDir+"/LimitTree.root\",\""+DataCardsDir+"/XSecs.txt\",  true, false, 7 , 5.035 )'")
         os.system("root -l -b -q plotLimit.C+'(\""+DataCardsDir+"/XSec_\",\""+DataCardsDir+"/LimitTree.root\",\""+DataCardsDir+"/XSecs.txt\",  false, false, 7 , 5.035 )'")
#         os.system("root -l -b -q plotLimit.C+'(\""+DataCardsDir+"\",\""+DataCardsDir+"/LimitTree.root\",\"\",  true, 8 , 19.6 )'")
   ######################################################################

   else:

      help()


	
