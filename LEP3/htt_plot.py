#!/usr/bin/env python
import os,string,sys,commands,time,ConfigParser
from ROOT import *
from array import array

# legenda:
# step 0--> all
# step 1--> after njet cut
# step 2--> after cut on energy
# step 3--> after 2 tau candidate
# step 4--> after testID
# step 5--> after findhz


# format: file,xsec(fb),tag for legenda
#latest ok
#mclist=[
#    ["htt/Hig125_134/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",200.,"HZ"],
#    ["htt/ZZ_2/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",1446.,"ZZ"],
#    ["htt/WW_8/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",14080.,"WW"],
#    ["htt/QQ_1/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",50000.,"QQ"]
#    ]

mclist=[
    ["htt/Hig125_139/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",200.,"HZ"],
    ["htt/ZZ_7/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",1446.,"ZZ"],
    ["htt/WW_10/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",14080.,"WW"],
    ["htt/QQ_2/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",50000.,"QQ"]
    ]


# luminosity to normalize (in fb-1)
lumi=500

# step at which the plot should be made
stepplot=6

# define special histograms
step_h=[]
genrec_s3_t1=[]
genrec_s3_t2=[]
mzh_h2=[]

# Define all others
# syntax: name, variable, nibn,xmin,xmax,visualize

step_label=["all","njet>4","ejet>10","2 taucand","2 good taucand","jet sele","mzmh cut","btag"]

h1_list=[
    ["Hmass_" ,"event.recHMass" ,200,0,400,True],
    ["Zmass_" ,"event.recZMass" ,200,0,400,True],
    ["mvis_"  ,"event.mvis"     ,200,0,400,True],
    ["evis_"  ,"event.evis"     ,200,0,400,True],
    ["ntrt1_" ,"event.t1recNtrk", 10,0, 10,True],
    ["ntrt2_" ,"event.t2recNtrk", 10,0, 10,True],
    ["ntrt_"  ,"event.t2recNtrk+event.t1recNtrk", 10,0, 10,True],
    ["pz_"    ,"event.pz"  ,  100,-100, 100,True],
    ["chi2_"  ,"event.chi2", 1000, 0,10000, True],
    ["wwmin_" ,"event.wwMin",  50, 0,  100, True],
    ["zzmin_" ,"event.zzMin",  50, 0,  100, True],
    ["btag_tt","event.btag_tt",50, -10, 10, True],
    ["btag_jj","event.btag_jj",50, -10, 10, True]
    ]
    
    


h1glob=[]
for index in range(0,len(mclist)):
    mc=mclist[index]
    tag=mc[2]

    step_h.append(TH1F("step_"+tag,"step_"+tag,10,0,10))
    step_h[index].SetLineColor(index+2)
    step_h[index].SetLineWidth(2)
    step_h[index].SetMarkerColor(index+2)
    for bin in range(1,len(step_label)+1):
        step_h[index].GetXaxis().SetBinLabel(bin,step_label[bin-1])    
#        step_h[index].LabelsOption("v","X")
        
    mzh_h2.append(TH2F("mzvsmh_"+tag,"mzvsmh_"+tag,200,0,400,200,0,400))
    mzh_h2[index].SetLineColor(index+2)
    mzh_h2[index].SetMarkerColor(index+2)
#    mzh_h2[index].SetFillColor(index+2)

    genrec_s3_t1.append(TH1F("genrec_s3_t1_"+tag,"genrec_s3_t1_"+tag,100,-1,1))
    genrec_s3_t2.append(TH1F("genrec_s3_t2_"+tag,"genrec_s3_t2_"+tag,100,-1,1))

    h1loc=[]
    for h1 in range(0,len(h1_list)):
        param=h1_list[h1]
        h1loc.append(TH1F(param[0]+tag,param[0]+tag,param[2],param[3],param[4]))  
        h1loc[len(h1loc)-1].SetLineColor(index+2)
        h1loc[len(h1loc)-1].SetLineWidth(2)
        h1loc[len(h1loc)-1].SetMarkerColor(index+2)
    h1glob.append(h1loc)            

# now loop on tree and project
for index,mc in enumerate(mclist):
    rootfile=mc[0]
    xsec=mc[1]
    tag=mc[2]
    treefile=TFile.Open(rootfile)
    print "opening ",rootfile
    tree=treefile.Get("htttreeproducer_httanalyzer")
    nevents=tree.GetEntries()
    nevents=min(nevents,100000)
    # loop on tree entries
    weight=xsec*lumi/nevents
    
    print weight,nevents
    h1loc=h1glob[index]
    read=0
    for event  in tree:
        if read>=nevents:
            break
        read+=1
        for bin in range(0,int(event.step)+1):
            if index==0:
                if event.g_ishtt==1 and event.g_isHZqq==1:
                    step_h[index].Fill(bin)
            else:
                step_h[index].Fill(bin)
                
        if event.step>=stepplot:
            # here we can put all plots after selection
            if index==0:
                if event.g_ishtt==1 and event.g_isHZqq==1:
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
yheaderstart=.95-.023*len(mclist)
leg_hist = TLegend(0.7,yheaderstart,.98,.98);
leg_hist.SetFillColor(0)# Have a white background


c1=TCanvas("c1","c1",800,600)
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
    print index,opt
    step_h[index].Draw(opt)
    leg_hist.AddEntry(step_h[index],mc[2],"l")
#    c1.cd(1)
#    genrec_s3_t1[index].Draw(opt)
#    c1.cd(2)
#    genrec_s3_t2[index].Draw(opt)

leg_hist.Draw() 
c1.Print("cut_chain.png")

canv=[]
for i,h1 in enumerate(h1_list):
    plot=h1[5]
    if not plot:
        continue
    tag=h1[0]
    canv.append(TCanvas("c_"+tag,"c_"+tag,800,600))
    stackh_h = THStack(tag, tag)
    for index in range(len(mclist)-1,-1,-1):
        h1loc=h1glob[index]
        print index
        stackh_h.Add(h1loc[i])
    stackh_h.Draw()
    leg_hist.Draw()
    canv[len(canv)-1].Print(tag+".png")




#stackmzh_h2 = THStack("mzmh", "mzmh")
#for index in range(0,len(mclist)):
#    stackmzh_h2.Add(mzh_h2[index])
#stackmzh_h2.Draw("box")



a=raw_input("hit a key to exit...")
