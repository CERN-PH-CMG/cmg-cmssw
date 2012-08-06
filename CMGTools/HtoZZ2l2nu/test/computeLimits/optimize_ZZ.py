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
inUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotterZZ2011.root'
CWD=os.getcwd()
phase=-1
jsonUrl='$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_ZZ_2011.json'
CMSSW_BASE=os.environ.get('CMSSW_BASE')
#LandSArg=' --indexvbf 78 '
#LandSArg=' --indexvbf 9 '
#LandSArg=' --bins eq0jets'
LandSArg=' --bins eq0jets'
LandSArg+=' --systpostfix _7TeV'
LandSArg+=' --subDY $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/gamma_zz_7TeV.root'
DataCardsDir='cards'

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
   elif o in('-m'): shapeBased = a
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
OUT = CWD+'/JOBS/'
if(shapeBased=='1'): OUT+='SHAPE/'
else:		     OUT+='COUNT/'	
os.system('mkdir -p ' + OUT)

if(shapeBased=='1'): DataCardsDir+='Shape'

#get the cuts
file = ROOT.TFile(inUrl)
cuts1   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_met') 
cuts2   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_zpt') 
cuts3   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_zm') 
cuts4   = file.Get('WW#rightarrow 2l2#nu/optim_cut1_jetthr')

######################################################################

if( phase == 1 ):

   print '#                                  #'
   print '# RUN LIMITS FOR ALL POSSIBLE CUTS #'
   print '#                                  #'

   commandToRun = []

   FILE = open(OUT+"/LIST.txt","w")
   for i in range(1,cuts1.GetNbinsX()):
#      if(shapeBased=='1' and cuts3.GetBinContent(i)<780):continue
      FILE.writelines("index="+str(i).rjust(5) + " --> met>" + str(cuts1.GetBinContent(i)).rjust(5) + " zpt>" + str(cuts2.GetBinContent(i)).rjust(5) + "  |zll-zmass|<"+str(cuts3.GetBinContent(i)).rjust(5) + ' redMetJetPt>'+str(cuts4.GetBinContent(i)).rjust(5)+"\n")

      #create wrappper script for each set of cuts ans submit it
      SCRIPT = open(OUT+'script_'+str(i)+'.sh',"w")
      SCRIPT.writelines('echo "TESTING SELECTION : ' + str(i).rjust(5) + ' --> met>' + str(cuts1.GetBinContent(i)).rjust(5) + ' zpt>' + str(cuts2.GetBinContent(i)).rjust(5) + ' |zll-zmass|<'+str(cuts3.GetBinContent(i)).rjust(5)+ ' redMetJetPt>'+str(cuts4.GetBinContent(i)).rjust(5)+'";\n')
      SCRIPT.writelines('cd ' + CMSSW_BASE + '/src;\n')
      SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc462")+";\n")
      SCRIPT.writelines("eval `scram r -sh`;\n")
      SCRIPT.writelines('cd ' + CWD + ';\n')
#      SCRIPT.writelines('cd /tmp/;\n')
      for m in MASS:
         shapeBasedOpt=''
         if(shapeBased=='1') : shapeBasedOpt='--shape'
         cardsdir = 'optim/H'+ str(m);
         if(shapeBased=='0'): cardsdir+='_count_'+str(i)
         if(shapeBased=='1'): cardsdir+='_shape_'+str(i)
         SCRIPT.writelines('mkdir -p ' + cardsdir+';\ncd ' + cardsdir+';\n')
         SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " --syst " + shapeBasedOpt + " --index " + str(i)     + " --json " + jsonUrl +" --fast " + LandSArg + " ;\n")# " --shapeMin " + str(Gzpt.Eval(m,0,"")) +" --shapeMax " + str(Gzm.Eval(m,0,""))  +" ;\n")
         SCRIPT.writelines("sh combineCards.sh;\n")
#         SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " --run expected card_combined.dat > COMB.log;\n")
         SCRIPT.writelines("combine -M MaxLikelihoodFit -m " +  str(m) + " --saveNormalizations card_combined.dat > COMB.log;\n")
         SCRIPT.writelines("extractFitNormalization.py mlfit.root hzz2l2v_"+str(m)+"_?TeV.root > fit.txt;\n")
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
      BestLimit = []
      fileList = commands.getstatusoutput("ls " + OUT + str(m)+"_*.log")[1].split();           
      for f in fileList:
         exp = commands.getstatusoutput("cat " + f + " | grep \"Best fit r: \"")[1];
         if(len(exp)<=0):continue
         median = float(exp.split()[3])
         uncM = float(exp.split()[4].split('/')[0])
         uncP = float(exp.split()[4].split('/')[1])
         unc = (((median+uncP) + (median-uncM))/(2*median))-1
         if(float(median)<=0.0):continue
         index = int(f[f.rfind("_")+1:f.rfind(".log")])
         BestLimit.append("mH="+str(m)+ " --> " + ('%07.3f%%' % float(100.0*unc)) + " " + ('%07.3fpb +- [%07.3f%%/%07.3f%%]' % (float(median*5.9) , 100.0*uncM/median , 100.0*uncP/median) ) + " " + str(index).rjust(5) + " " + str(cuts1.GetBinContent(index)).rjust(5) + " " + str(cuts2.GetBinContent(index)).rjust(5) + " " + str(cuts3.GetBinContent(index)).rjust(5) + " " + str(cuts4.GetBinContent(index)).rjust(5))

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
   Gzpt = ROOT.TGraph(len(SUBMASS));
   Gzm = ROOT.TGraph(len(SUBMASS));
   Gjt = ROOT.TGraph(len(SUBMASS));

   if(cutList=='') :
      fileName = OUT+"/OPTIM_"
      if(shapeBased=='1'):
         fileName+='SHAPE'
      else:
         fileName+='COUNT'
      fileName+=".txt"
      
      mi=0
      for m in MASS:

         #if you want to display more than 3 options edit -m3 field
         cut_lines=commands.getstatusoutput("cat " + fileName + " | grep 'mH="+str(m)+"' -m20")[1].split('\n')
         print 'mH='+str(m)+'\tOption \tR \tmin MET\tZPT\tZMass\tredMetJetPt' 
         ictr=1
         for c in cut_lines:
            print '\t #'+ str(ictr) + '\t' + c.split()[2] + '\t' + c.split()[4] + '\t' + c.split()[5] + '\t' + c.split()[6]+ '\t' + c.split()[7]
            ictr+=1
         print "Which option you want to keep?"
         opt = int(raw_input(">"))-1

         #save cut chosen
         metCut=float(cut_lines[opt].split()[4])
         zptCut=float(cut_lines[opt].split()[5])
         zmCut=float(cut_lines[opt].split()[6])
         jtCut=float(cut_lines[opt].split()[7])
         Gmet .SetPoint(mi, m, metCut);
         Gzpt.SetPoint(mi, m, zptCut);
         Gzm.SetPoint(mi, m, zmCut);
         Gjt.SetPoint(mi, m, jtCut);
         mi+=1
   else :
      mi=0
      f= open(cutList,'r')
      for line in f :
         vals=line.split(' ')
         Gmet .SetPoint(mi, float(vals[0]), float(vals[1]));
         Gzpt.SetPoint(mi, float(vals[0]), float(vals[2]));
         Gzm.SetPoint(mi, float(vals[0]), float(vals[3]));
         Gjt.SetPoint(mi, float(vals[0]), float(vals[4]));
         mi+=1
      f.close()

   Gmet.Set(mi);
   Gzpt.Set(mi);
   Gzm.Set(mi);




   #display cuts chosen
   ROOT.gROOT.SetStyle('Plain')
   ROOT.gStyle.SetOptStat(False);
   c1 = ROOT.TCanvas("c1", "c1",600,600);
   c1.Divide(2,2);
   c1.cd(1);
   Gmet.SetMarkerStyle(20);
   Gmet.SetTitle("MET");
   Gmet.Draw("APC");
   Gmet.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gmet.GetYaxis().SetTitle("met cut");

   c1.cd(2);
   Gzpt.SetMarkerStyle(20);
   Gzpt.SetTitle("ZPt");
   Gzpt.Draw("APC");
   Gzpt.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gzpt.GetYaxis().SetTitle("zpt cut");

   c1.cd(3);
   Gzm.SetMarkerStyle(20);
   Gzm.SetTitle("ZMass");
   Gzm.Draw("APC");
   Gzm.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gzm.GetYaxis().SetTitle("zmass cut");

   c1.cd(4);
   Gjt.SetMarkerStyle(20);
   Gjt.SetTitle("RedMet Jet Threshold");
   Gjt.Draw("APC");
   Gjt.GetXaxis().SetTitle("m_{H} (GeV/c^{2})");
   Gjt.GetYaxis().SetTitle("redMetJetThreshold cut");

   c1.cd(0);
   c1.Update();
   c1.SaveAs("OptimizedCuts.png")

   #run limits for the cuts chosen (for intermediate masses use spline interpolation)
   for m in SUBMASS:
        index = findCutIndex(Gmet.Eval(m,0,""), cuts1, Gzpt.Eval(m,0,""), cuts2,  Gzm.Eval(m,0,""), cuts3, Gjt.Eval(m,0,""), cuts4);
        print("mH="+str(m).rjust(3)+ " met>"+str(cuts1.GetBinContent(index)).rjust(5) + " zpt>" + str(cuts2.GetBinContent(index)).rjust(5) + "  |zll-zmass|<"+str(cuts3.GetBinContent(index)).rjust(5) + ' redMetJetPt>'+str(cuts4.GetBinContent(index)).rjust(5) )

   while True:
        ans = raw_input('Use this fit and compute final limits? (y or n)\n')
        if(ans=='y' or ans == 'Y'): break;
        else:			    sys.exit(0);           
   print 'YES'


   list = open(OUT+'list.txt',"w")
   listcuts = open(OUT+'cuts.txt',"w")
   for m in SUBMASS:
        index = findCutIndex(Gmet.Eval(m,0,""), cuts1, Gzpt.Eval(m,0,""), cuts2,  Gzm.Eval(m,0,""), cuts3, Gjt.Eval(m,0,""), cuts4);
        SCRIPT = open(OUT+'/script_mass_'+str(m)+'.sh',"w")
        SCRIPT.writelines('cd ' + CMSSW_BASE + ';\n')
        SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc462")+";\n")
        SCRIPT.writelines("eval `scram r -sh`;\n")
        SCRIPT.writelines('cd ' + CWD + ';\n')
        shapeBasedOpt=''
        if(shapeBased=='1') : shapeBasedOpt='--shape'

        SideMassesArgs = ' '
        SideMasses = findSideMassPoint(m)
        if(not (SideMasses[0]==SideMasses[1])):
           print "Side Mass for mass " + str(m) + " are " + str(SideMasses[0]) + " and " + str(SideMasses[1])
           Lindex = findCutIndex(Gmet.Eval(SideMasses[0],0,""), cuts1, Gzpt.Eval(SideMasses[0],0,""), cuts2,  Gzm.Eval(SideMasses[0],0,""), cuts3, Gjt.Eval(SideMasses[0],0,""), cuts4);
           Rindex = findCutIndex(Gmet.Eval(SideMasses[1],0,""), cuts1, Gzpt.Eval(SideMasses[1],0,""), cuts2,  Gzm.Eval(SideMasses[1],0,""), cuts3, Gjt.Eval(SideMasses[1],0,""), cuts4);
           print "cutIndex for sideBand are " + str(Lindex) + " and " + str(Rindex) 
           SideMassesArgs += "--mL " + str(SideMasses[0]) + " --mR " + str(SideMasses[1]) + " --indexL " + str(Lindex) +  " --indexR " + str(Rindex) + " "


        cardsdir=DataCardsDir+"/"+str(m);
        SCRIPT.writelines('mkdir -p ' + cardsdir+';\ncd ' + cardsdir+';\n')
        SCRIPT.writelines("runLandS --m " + str(m) + " --histo " + shapeName + " --in " + inUrl + " " + " --syst " + shapeBasedOpt + " --index " + str(index) + " --json " + jsonUrl + " " + SideMassesArgs + " " + LandSArg +" ;\n")# " --shapeMin " + str(Gzpt.Eval(m,0,"")) +" --shapeMax " + str(Gzm.Eval(m,0,""))  +" ;\n")
        SCRIPT.writelines("sh combineCards.sh;\n")
        SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " card_combined.dat > COMB.log;\n") 
        SCRIPT.writelines("combine -M MaxLikelihoodFit -m " +  str(m) + " --saveNormalizations card_combined.dat > COMB.log;\n")
        SCRIPT.writelines("extractFitNormalization.py mlfit.root hzz2l2v_*_?TeV.root > fit.txt;\n")
        SCRIPT.writelines('cd ..;\n\n') 
	SCRIPT.close()
        os.system("bsub -G u_zh -q 2nd 'sh " + OUT+"script_mass_"+str(m)+".sh'")
	if(shapeBased=='1'):   list.writelines('H'+str(m)+'_shape_'+str(index)+'\n'); 
	else:                  list.writelines('H'+str(m)+'_count_'+str(index)+'\n');
        listcuts.writelines(str(m)+' ' + str(Gmet.Eval(m,0,"")) + ' ' + str(Gzpt.Eval(m,0,""))+' '+ str(Gzm.Eval(m,0,"")) +'\n');
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


	
