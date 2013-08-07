#!/usr/bin/env python
import os,string,sys,commands,time,ConfigParser
from ROOT import *
from array import array
import numpy
# legenda:
# step 0--> all
# step 1--> after njet cut
# step 2--> after cut on energy
# step 3--> after 2 tau candidate
# step 4--> after testID
# step 5--> after findhz

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
#latest ok (no pftau)
#mclist=[
#    ["htt/Hig125_3/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",200.,"HZqqtt","HZ(qq#tau#tau)"],
#    ["htt/Hig125_3/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",200.,"HZothers","HZ(others)"],
#    ["htt/ZZ_1/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",1446.,"ZZ","ZZ"],
#    ["htt/WW_1/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",14080.,"WW","WW"],
#    ["htt/QQ_1/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",50000.,"QQ","QQ"]
#    ]
#treename="htttreeproducer_httanalyzer"

#pftau version 1 (loose ele,muons)
#latest ok
#mclist=[
#    ["htt/Hig125_83/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",200.,"HZqqtt","HZ(qq#tau#tau)"],
#    ["htt/Hig125_83/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",200.,"HZothers","HZ(others)"],
#    ["htt/ZZ_2/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",1446.,"ZZ","ZZ"],
#    ["htt/WW_3/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",14080.,"WW","WW"],
#    ["htt/QQ_4/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",50000.,"QQ","QQ"]
#    ]
#treename="htttreeproducerpftau_httanalyzerpftau"

#pftau version 2 (tight ele muons)
mclist=[
    ["htt/Hig125_85/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",200.,"HZqqtt","HZ(qq#tau#tau)"],
    ["htt/Hig125_85/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",200.,"HZothers","HZ(others)"],
    ["htt/ZZ_3/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",1446.,"ZZ","ZZ"],
    ["htt/WW_4/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",14080.,"WW","WW"],
    ["htt/QQ_5/htttreeproducerpftau_httanalyzerpftau/htttreeproducerpftau_httanalyzerpftau_tree.root",50000.,"QQ","QQ"]
    ]
treename="htttreeproducerpftau_httanalyzerpftau"

# luminosity to normalize (in fb-1)
lumi=500

# step at which the plot should be made
#stepplot=6

# define special histograms
step_h=[]
genrec_s3_t1=[]
genrec_s3_t2=[]
mzh_h2=[]
dgen1_vs_iso_h2=[]
dgen2_vs_iso_h2=[]

# Define all others
# syntax: name, variable, nibn,xmin,xmax,visualize, tag (no strange character here),title,xlabel,ylabel 

step_label=["all","njet>4","ejet>10","2 taucand","2 good taucand","jet sele","mzmh cut","btag"]

def_plot=true
h1_list=[
    ["Hmass_" ,"event.recHMass" ,80,40,200,True, "", "", "", ""],
    ["Zmass_" ,"event.recZMass" ,30,60,120,def_plot, "", "", "", ""],
    ["mvis_"  ,"event.mvis"     ,100,0,400,def_plot, "", "", "", ""],
    ["evis_"  ,"event.evis"     ,100,0,400,def_plot, "", "", "", ""],
#    ["ntrt1_" ,"event.t1recNtrk", 10,0, 10,def_plot, "", "", "", ""],
#    ["ntrt2_" ,"event.t2recNtrk", 10,0, 10,def_plot, "", "", "", ""],
#    ["ntrt_"  ,"event.t2recNtrk+event.t1recNtrk", 10,0, 10,def_plot, "", "", "", ""],
    ["pz_"    ,"event.pz"  ,  50 ,-100, 100,def_plot, "", "", "", ""],
    ["chi2_"  ,"event.chi2", 1000, 0,10000, def_plot, "", "", "", ""],
    ["wwmin_" ,"event.wwMin",  25, 0,  100, def_plot, "", "", "", ""],
    ["zzmin_" ,"event.zzMin",  25, 0,  100, def_plot, "", "", "", ""],
    ["btag_tt","event.btag_tt",20, 0,   4., def_plot, "", "", "", ""],
    ["btag_jj","event.btag_jj",20, 0,   4., def_plot, "", "", "", ""],
    ["pz+emiss","240.-event.evis+abs(event.pz)", 200,-400,400,def_plot, "", "", "", ""],
    ["ttacoll","event.tt_acoll", 100,-1,1,def_plot, "", "", "", ""],
    ["jjacoll","event.jj_acoll", 100,-1,1,def_plot, "", "", "", ""],
    ["leadingmu_e","event.leadingMuonEnergy", 100,0,200,def_plot, "", "", "", ""],
    ["leadinge_e","event.leadingElectronEnergy", 100,0,200,def_plot, "", "", "", ""],
    ["subleadingmu_e","event.subleadingMuonEnergy", 100,0,200,def_plot, "", "", "", ""],
    ["subleadinge_e","event.subleadingElectronEnergy", 100,0,200,def_plot, "", "", "", ""],
#    ["nhfraction_t1","event.t1recNHFraction", 100,0,1,def_plot, "", "", "", ""],
#    ["nhfraction_t2","event.t2recNHFraction", 100,0,1,def_plot, "", "", "", ""],
#    ["chfraction_t1","event.t1recChFraction", 100,0,1,def_plot, "", "", "", ""],
#    ["chfraction_t2","event.t2recChFraction", 100,0,1,def_plot, "", "", "", ""],
#    ["pfraction_t1","event.t1recPFraction", 100,0,1,def_plot, "", "", "", ""],
#    ["pfraction_t2","event.t2recPFraction", 100,0,1,def_plot, "", "", "", ""],
    ["mass_t1","event.t1recMass", 100,0,30,def_plot, "", "", "", ""],
    ["mass_t2","event.t2recMass", 100,0,30,def_plot, "", "", "", ""],
    ["acopl","event.ttmet_acopl", 100,-1,1,def_plot, "", "", "", ""],
    ["constrained_mass","event.recZMass + event.recHMass - 91.2", 80,40,200,def_plot,"hqqtt", "H#rightarrow#tau#tau with Z#rightarrow q#bar{q}", "Higgs mass (GeV)", "Events/2 GeV"],
    ["t1candiso","event.t1candiso",100,0.,1.,True, "", "", "", ""],
    ["t2candiso","event.t2candiso",100,0.,1.,True, "", "", "", ""]

    ]
    
    
linecolors=[2, 3, 4, 1, 6]
fillcolors=[0, 0, 4, 4, 4]
fillstyles=[0, 0, 3013, 3013, 3013]
smooth=[False, False, True, True, True]

h1glob=[]
for index in range(0,len(mclist)):
    mc=mclist[index]
    tag=mc[2]

    step_h.append(TH1F("step_"+tag,"step_"+tag,10,0,10))
    step_h[index].SetLineColor(linecolors[index])
    step_h[index].SetLineWidth(2)
    step_h[index].SetMarkerColor(linecolors[index])
    step_h[index].SetFillColor(fillcolors[index])
    step_h[index].SetFillStyle(fillstyles[index])
    for bin in range(1,len(step_label)+1):
        step_h[index].GetXaxis().SetBinLabel(bin,step_label[bin-1])    
#        step_h[index].LabelsOption("v","X")
        
    mzh_h2.append(TH2F("mzvsmh_"+tag,"mzvsmh_"+tag,200,0,400,200,0,400))
    mzh_h2[index].SetLineColor(linecolors[index])
    mzh_h2[index].SetMarkerColor(linecolors[index])
#    mzh_h2[index].SetFillColor(linecolors[index])
    dgen1_vs_iso_h2.append(TH2F("dgen1vsiso_"+tag,"dgen1vsiso_"+tag,100,-1,1,100,-1,1))
    dgen1_vs_iso_h2[index].SetLineColor(linecolors[index])
    dgen1_vs_iso_h2[index].SetMarkerColor(linecolors[index])
    dgen2_vs_iso_h2.append(TH2F("dgen2vsiso_"+tag,"dgen2vsiso_"+tag,100,-1,1,100,-1,1))
    dgen2_vs_iso_h2[index].SetLineColor(linecolors[index])
    dgen2_vs_iso_h2[index].SetMarkerColor(linecolors[index])

    genrec_s3_t1.append(TH1F("genrec_s3_t1_"+tag,"genrec_s3_t1_"+tag,100,-1,1))
    genrec_s3_t2.append(TH1F("genrec_s3_t2_"+tag,"genrec_s3_t2_"+tag,100,-1,1))

    h1loc=[]
    for h1 in range(0,len(h1_list)):
        param=h1_list[h1]
        h1loc.append(TH1F(param[0]+tag,param[0]+tag,param[2],param[3],param[4]))  
        h1loc[len(h1loc)-1].SetLineColor(linecolors[index])
        h1loc[len(h1loc)-1].SetLineWidth(2)
        h1loc[len(h1loc)-1].SetMarkerColor(linecolors[index])
        #if index != 0:
        h1loc[len(h1loc)-1].SetFillStyle(fillstyles[index]);
        h1loc[len(h1loc)-1].SetFillColor(fillcolors[index])
    h1glob.append(h1loc)            

#maxevent=100000000
# now loop on tree and project
nhtt=0
nhtt_sel=0
nhbbtt=0
nhbbtt_sel=0

for index,mc in enumerate(mclist):
    rootfile=mc[0]
    xsec=mc[1]
    tag=mc[2]
    treefile=TFile.Open(rootfile)
    print "opening ",rootfile
    tree=treefile.Get(treename)
    nevents=tree.GetEntries()
    nevents=min(nevents,maxevent)
    # loop on tree entries
    weight=xsec*lumi/nevents
    
    print weight,nevents
    h1loc=h1glob[index]
    read=0
    for event  in tree:
        if read>=nevents:
            break
        read+=1
        if read % 10000 ==1:
            print "Reading event:",read,'/',nevents
        

        istt=event.g_ishtt==1 and event.g_isHZqq==1

        addcut = True
        if index==0:
            addcut= addcut and istt
        if index==1:
            addcut= addcut and not istt
            

        addcut = addcut and event.mvis<210.
#        addcut = addcut and event.tt_acoll<-0.5
        addcut = addcut and event.jj_acoll<-0.3
        if "pftau" in treename:
            addcut = addcut and event.leadingMuon_en<50.
            addcut = addcut and event.leadingElectron_en<50.
        else:
            addcut = addcut and event.leadingMuonEnergy<50.
            addcut = addcut and event.leadingElectronEnergy<50.
            addcut = addcut and event.t1recMass < 7 and event.t2recMass < 7
            addcut = addcut and event.t1recChFraction > 0.06


        if index==0:
            if event.g_ishtt==1 and event.g_isHZqq==1:
                nhtt+=1
            if event.g_ishtt==1 and event.g_isHZbb==1:
                nhbbtt+=1
                
        for bin in range(0,int(event.step)+1):
            if index==0:
                if event.g_ishtt==1 and event.g_isHZqq==1:
                    step_h[index].Fill(bin)
            else:
                step_h[index].Fill(bin)
                
        if event.step>=stepplot and addcut:
            # here we can put all plots after selection
            if index==0:
                if event.g_ishtt==1 and event.g_isHZbb==1:
                    nhbbtt_sel+=1
                if event.g_ishtt==1 and event.g_isHZqq==1:
                    nhtt_sel+=1
#                    print 'ok'
                    genrec_s3_t1[index].Fill(event.genRecDistance1)
                    genrec_s3_t2[index].Fill(event.genRecDistance2)
            mzh_h2[index].Fill(event.recHMass,event.recZMass)

            for i,h1 in enumerate(h1loc):
                param=h1_list[i]
#                print param[1]
                h1.Fill(eval(param[1]),weight)
            
    treefile.Close()
    # renormalize step_h histo for efficiencies
    norm=step_h[index].GetBinContent(1)
    print norm
    if norm==0: norm=-1
    for bin in range(0,step_h[index].GetNbinsX()):
        step_h[index].SetBinContent(bin,step_h[index].GetBinContent(bin)/norm)
        
    
# now we can plot them


# first prepare legenda
#yheaderstart=.95-.023*len(mclist)
leg_hist = TLegend(0.15,0.55,0.35,0.85);
leg_hist.SetFillColor(0)# Have a white background
leg_hist.SetTextSize(0.03)
#text_lumi = TText(60,320,"L = 500 fb-1");

c1=TCanvas("step","step")
c1.SetFillColor(kWhite)
c1.SetFillStyle(1001)
#gStyle.SetOptStat(0)
 
first=True
# legenda

#c1.Divide(1,2)
for index in range(0,len(mclist)):
    opt="same"
    mc=mclist[index]
    if (first):
        first=False
        opt=""
#    print index,opt
    step_h[index].Draw(opt)
    if index == 1:
      leg_hist.AddEntry( step_h[index], "All backgrounds", "lf");
    leg_hist.AddEntry(step_h[index],mc[3],"l")

leg_hist.Draw() 
c1.Print(plot_dire+"cut_chain.png")
c1.Print(plot_dire+"cut_chain.C")

c2=TCanvas("matched_s4","matched_s4")
c2.SetFillColor(kWhite)
c2.SetFillStyle(1001)
c2.Divide(1,2)
c2.cd(1)
genrec_s3_t1[0].Draw()
c2.cd(2)
genrec_s3_t2[0].Draw()
c2.Print(plot_dire+"matched.png")
c2.Print(plot_dire+"matched.C")

c3=TCanvas("dgenvsiso","dgenvsiso")
c3.SetFillColor(kWhite)
c3.SetFillStyle(1001)
c3.Divide(1,2)
c3.cd(1)
dgen1_vs_iso_h2[0].Draw()
c3.cd(2)
dgen2_vs_iso_h2[0].Draw()
c3.Print(plot_dire+"dgenvsiso.png")
c3.Print(plot_dire+"dgenvsiso.C")





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
    for index in range(len(mclist)-1,-1,-1):
        h1loc=h1glob[index]
        print index
        if smooth[index]:
          h1loc[i].Smooth(1, "R")
        stackh_h.Add(h1loc[i])
    stackh_h.Draw()
    if h1[8] != "":    
      stackh_h.GetXaxis().SetTitle(h1[8])    
    if h1[9] != "":
      stackh_h.GetYaxis().SetTitle(h1[9])   
    leg_hist.Draw()
    #text_lumi.Draw()
    canv[len(canv)-1].Print(plot_dire+"/"+tag+".png")
    canv[len(canv)-1].Print(plot_dire+"/"+tag+".C")




#stackmzh_h2 = THStack("mzmh", "mzmh")
#for index in range(0,len(mclist)):
#    stackmzh_h2.Add(mzh_h2[index])
#stackmzh_h2.Draw("box")


print "Efficiency on htt=",float(nhtt_sel)/nhtt,"(",nhtt_sel,"/",nhtt,")"
print "Efficiency on hbbtt=",float(nhbbtt_sel)/nhbbtt,"(",nhbbtt_sel,"/",nhbbtt,")"

a=raw_input("hit a key to exit...")
