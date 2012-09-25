#!/usr/bin/env python
import os,string,sys,commands,time,ConfigParser
from ROOT import *
from array import array
import numpy,math
# legenda:
# step 0--> all
# step 1--> after njet cut
# step 2--> after cut on energy
# step 3--> after 2 tau candidate
# step 4--> after testID
# step 5--> after findhz



def CrystalBall(x,par):
    #Crystal ball function for signal, parameters are 0:alpha,1:n,2:mean,3:sigma,4:normalization;
      t = (x[0]-par[2])/par[3]

      if par[0] < 0:
          t = -t
      absAlpha = math.fabs(par[0])
    
      if t >=-absAlpha :
          return par[4]*math.exp(-0.5*t*t) + par[5] + t*par[6]+t*t*par[7] + t*t*t*par[8] 
      else:
          a =  pow(par[1]/absAlpha,par[1])*math.exp(-0.5*absAlpha*absAlpha)
          b= par[1]/absAlpha - absAlpha
    
          return par[4]*(a/math.pow(b - t, par[1])) + par[5] + t*par[6] + t*t*par[7] + t*t*t*par[8]
    

#here starts the main

#cut1=TCut("Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<6&&(abs(mMiss)>10||(Tau1NEle+Tau2NEle<1&&Tau1NMu+Tau2NMu<2&&Tau1Ntrk+Tau2Ntrk<7))&&ZRMass+HRMass-91.2-sqrt(240**2-2*240*ZEnergy+ZMass**2)>-10")
#cut2=TCut("Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<6&&(abs(mMiss)>10||(Tau1NEle+Tau2NEle<1&&Tau1NMu+Tau2NMu<2&&Tau1Ntrk+Tau2Ntrk<7))&&ZRMass+HRMass-91.2-sqrt(240**2-2*240*ZEnergy+ZMass**2)>-10&&tautaull==0")

cut1="Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<6&&(abs(mMiss)>10||(Tau1NEle+Tau2NEle<1&&Tau1NMu+Tau2NMu<2&&Tau1Ntrk+Tau2Ntrk<7))&&ZRMass+HRMass-91.2-sqrt(240**2-2*240*ZEnergy+ZMass**2)>-10"


cut2="Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<6&&(abs(mMiss)>10||(Tau1NEle+Tau2NEle<1&&Tau1NMu+Tau2NMu<2&&Tau1Ntrk+Tau2Ntrk<7))&&ZRMass+HRMass-91.2-sqrt(240**2-2*240*ZEnergy+ZMass**2)>-10&&tautaull==0"

if len(sys.argv)!=5:
    print "Usage:",sys.argv[0]+" <plot directory> <step> <maxevent> <batch 1=yes>"
    sys.exit(1)

plot_dire=sys.argv[1]+"/"
stepplot=int(sys.argv[2])
maxevent=int(sys.argv[3])
batch=int(sys.argv[4])
if batch==1:
    print "working in batch mode, no plot will be shown"
    gROOT.SetBatch(True)
    

print "Parameters:",plot_dire,stepplot,maxevent,batch

os.system("mkdir -p "+plot_dire)



# format: file,xsec(fb),tag for internal use (no strange char there), tag for legenda
mclist=[
    ["httll/Hig125/TauTauLLTreeProducer_TauTauLLAnalyzer/TauTauLLTreeProducer_TauTauLLAnalyzer_tree.root",200.,"hzttll","HZ(#tau#taull)",97000],
    ["httll/Hig125/TauTauLLTreeProducer_TauTauLLAnalyzer/TauTauLLTreeProducer_TauTauLLAnalyzer_tree.root",200.,"HZothers","HZ(others)",97000],
    ["httll/ZZ_2/TauTauLLTreeProducer_TauTauLLAnalyzer/TauTauLLTreeProducer_TauTauLLAnalyzer_tree.root",1446.,"ZZ","ZZ",992000],
    ["httll/WW_2/TauTauLLTreeProducer_TauTauLLAnalyzer/TauTauLLTreeProducer_TauTauLLAnalyzer_tree.root",14080.,"WW","WW",1960000],
    ["httll/QQ_1/TauTauLLTreeProducer_TauTauLLAnalyzer/TauTauLLTreeProducer_TauTauLLAnalyzer_tree.root",50000.,"QQ","QQ",5998000]
    ]
treename="TauTauLLTreeProducer_TauTauLLAnalyzer"

# luminosity to normalize (in fb-1)
lumi=500

# step at which the plot should be made
#stepplot=6


# Define all others
# syntax: name, variable, nibn,xmin,xmax,visualize, tag (no strange character here),title,xlabel,ylabel 

#step_label=["all","njet>4","ejet>10","2 taucand","2 good taucand","jet sele","mzmh cut","btag"]
step_label=["all"]

def_plot=true

h1_list=[
    ["cmass","sqrt(240**2-2*240*ZEnergy+ZMass**2)", 50,50,150,def_plot,"httll", "H#rightarrow#tau#tau with Z#rightarrow ll", "Higgs mass (GeV)", "Events/2 GeV"]
    ]
    
    
linecolors=[2, 3, 4, 1, 6]
fillcolors=[0, 0, 4, 4, 4]
fillstyles=[0, 0, 3013, 3013, 3013]
#smooth=[False, False, True, True, True]
smooth=[False, False, False, False, False]



h1glob=[]
gDirectory.cd()
for index,mc in enumerate(mclist):
    rootfile=mc[0]
    xsec=mc[1]
    tag=mc[2]
    nev=mc[4]
    
    h1loc=[]
    # first create histos in memory
    for h1 in range(0,len(h1_list)):
        param=h1_list[h1]
        h1loc.append(TH1F(param[0]+tag,param[0]+tag,param[2],param[3],param[4]))

    h1glob.append(h1loc)

print "before",h1glob
    
    # here we have separate pointers for each histogram (tested)
    # then go to trees
    # we CANNOT go in the same loop because othewise the pointer is overwritten
    
for index,mc in enumerate(mclist):
    rootfile=mc[0]
    xsec=mc[1]
    tag=mc[2]
    nev=mc[4]

    treefile=TFile.Open(rootfile)
    print "opening ",rootfile
    tree=treefile.Get(treename)
    nevents=nev
    weight=xsec*lumi/nevents
    print weight,nevents

    h1loc=h1glob[index]
    for hindex in range(0,len(h1_list)):
        param=h1_list[hindex]

        h1=h1loc[hindex]
        h1.SetLineColor(linecolors[index])
        h1.SetLineWidth(2)
        h1.SetMarkerColor(linecolors[index])
        #if index != 0:
        h1.SetFillStyle(fillstyles[index]);
        h1.SetFillColor(fillcolors[index])
        # this is need to put it back in memory

        tmphisto=h1.Clone("tmp")
        if index!=1:
            tree.Project("tmp",param[1],cut1)
        else:
            tree.Project("tmp",param[1],cut2)
        tmphisto.SetName(h1.GetName())
        tmphisto.Copy(h1)
        h1.Scale(weight)
        h1.Draw()
        c1.Update()

#    a=raw_input("hit a key to continue...")
#    treefile.Close()
     
# now we can plot them
#
print "after",h1glob

# first prepare legenda
#yheaderstart=.95-.023*len(mclist)
leg_hist = TLegend(0.15,0.55,0.35,0.85);
leg_hist.SetFillColor(0)# Have a white background
leg_hist.SetTextSize(0.03)

#text_lumi = TText(60,320,"L = 500 fb-1");

canv=[]
for i,h1 in enumerate(h1_list):
    plot=h1[5]
    if not plot:
        continue
    tag=h1[0]
    canv.append(TCanvas("c_"+tag,"c_"+tag))

    if h1[6] != "":
      stackh_h = THStack(h1[6], h1[7])
    else:  
      stackh_h = THStack(tag, tag)

    first=True
    for index in range(len(mclist)-1,-1,-1):
        mc=mclist[index]
        print "Stacking mc ",index
        h1loc=h1glob[index]
#        print h1loc
#        print index
        if smooth[index]:
          h1loc[i].Smooth(1, "R")
        stackh_h.Add(h1loc[i])
        if first:
            summedh=h1loc[i].Clone("summedh")
        else:
            summedh.Add(h1loc[i])
            
        if index == 1:
            leg_hist.AddEntry( h1loc[i], "All backgrounds", "lf");
        leg_hist.AddEntry( h1loc[i],mc[3],"l")
        first=False
        
    stackh_h.Draw()
    if h1[8] != "":    
      stackh_h.GetXaxis().SetTitle(h1[8])    
    if h1[9] != "":
      stackh_h.GetYaxis().SetTitle(h1[9])   
    
    leg_hist.Draw()

    # let's fit
    fh = TF1("fh",CrystalBall,100,150,9)
    fh.SetParameters(-1.5,1.0,125.,3.0,85.,-4.,4.,0.,0.)
    fh.FixParameter(1,0.)
    summedh.Fit("fh","","0Same",105,150)  
    summedh.Fit("fh","E","0Same",105,150)  

    #text_lumi.Draw()
    canv[len(canv)-1].Print(plot_dire+"/"+tag+".png")
    canv[len(canv)-1].Print(plot_dire+"/"+tag+".C")




#stackmzh_h2 = THStack("mzmh", "mzmh")
#for index in range(0,len(mclist)):
#    stackmzh_h2.Add(mzh_h2[index])
#stackmzh_h2.Draw("box")


#print "Efficiency on htt=",float(nhtt_sel)/nhtt,"(",nhtt_sel,"/",nhtt,")"
#print "Efficiency on hbbtt=",float(nhbbtt_sel)/nhbbtt,"(",nhbbtt_sel,"/",nhbbtt,")"

a=raw_input("hit a key to exit...")
