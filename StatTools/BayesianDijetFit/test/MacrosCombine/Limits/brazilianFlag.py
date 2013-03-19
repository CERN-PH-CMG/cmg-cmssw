import ROOT as rt

withAcceptance=True

def Plot(files, label, obs):

    radmasses = []
    for f in files:
        radmasses.append(float(f.replace("Xvv.mX","").split("_%s" %label)[0]))
    print radmasses

    efficiencies={}
    for mass in radmasses:
	efficiencies[mass]=20.0/19600.0*1.0/0.1
        if mass>2000:
	    efficiencies[mass]/=(1-0.4*(mass-2000.0)/1000.0)
	#print mass,efficiencies[mass]
	if not withAcceptance:
            efficiencies[mass]/=0.3

    fChain = []
    for onefile in files:
        fileIN = rt.TFile.Open(onefile)
        fChain.append(fileIN.Get("limit;1"))  

        rt.gROOT.ProcessLine("struct limit_t {Double_t limit;};")
        from ROOT import limit_t
        limit_branch = limit_t()

        for j in range(0,len(fChain)):
            chain = fChain[j]
            chain.SetBranchAddress("limit", rt.AddressOf(limit_branch,'limit'))

    rad = []
    for j in range(0,len(fChain)):
        chain = fChain[j]
        thisrad = []
        for  i in range(0,6):
            chain.GetTree().GetEntry(i)
            thisrad.append(limit_branch.limit)
            #print "limit = %f" %limit_branch.limit
        print thisrad
        rad.append(thisrad)


    # we do a plot r*MR
    mg = rt.TMultiGraph()
    mg.SetTitle("X -> ZZ")
    c1 = rt.TCanvas("c1","A Simple Graph Example",200,10,600,600)
    x = []
    yobs = []
    y2up = []
    y1up = []
    y1down = []
    y2down = []
    ymean = []

    for i in range(0,len(fChain)):
        y2up.append(rad[i][0]*efficiencies[radmasses[j]])
        y1up.append(rad[i][1]*efficiencies[radmasses[j]])
        ymean.append(rad[i][2]*efficiencies[radmasses[j]])
        y1down.append(rad[i][3]*efficiencies[radmasses[j]])
        y2down.append(rad[i][4]*efficiencies[radmasses[j]])
        yobs.append(rad[i][5]*efficiencies[radmasses[j]])

    grobs = rt.TGraphErrors(1)
    grobs.SetMarkerStyle(rt.kFullDotLarge)
    grobs.SetLineColor(rt.kRed)
    grobs.SetLineWidth(3)
    gr2up = rt.TGraphErrors(1)
    gr1up = rt.TGraphErrors(1)
    grmean = rt.TGraphErrors(1)
    grmean.SetLineColor(1)
    grmean.SetLineWidth(2)
    grmean.SetLineStyle(3)
    gr1down = rt.TGraphErrors(1)
    gr2down = rt.TGraphErrors(1)
  
    for j in range(0,len(fChain)):
        grobs.SetPoint(j, radmasses[j], yobs[j])
        gr2up.SetPoint(j, radmasses[j], y2up[j])
        gr1up.SetPoint(j, radmasses[j], y1up[j])
        grmean.SetPoint(j, radmasses[j], ymean[j])
        gr1down.SetPoint(j, radmasses[j], y1down[j])    
        gr2down.SetPoint(j, radmasses[j], y2down[j])
        print " observed %f %f" %(radmasses[j],yobs[j])
    
    mg.Add(gr2up)#.Draw("same")
    mg.Add(gr1up)#.Draw("same")
    mg.Add(grmean,"L*")#.Draw("same,AC*")
    mg.Add(gr1down)#.Draw("same,AC*")
    mg.Add(gr2down)#.Draw("same,AC*")
    if obs: mg.Add(grobs,"L,P")#.Draw("AC*")
 
    c1.SetLogy(1)
    mg.SetTitle("")
    mg.Draw("AP")
    mg.GetXaxis().SetTitle("X #rightarrow %s mass (GeV)" %label)
    if withAcceptance:
        mg.GetYaxis().SetTitle("#sigma #times B #times A exclusion limits")
    else:
        mg.GetYaxis().SetTitle("Cross section exclusion limits")
    mg.GetYaxis().SetTitleSize(0.05)
    mg.GetXaxis().SetTitleSize(0.042)
    mg.GetYaxis().SetRangeUser(0.001,3)
    mg.GetYaxis().SetLabelSize(0.05)
    mg.GetYaxis().SetTitleOffset(1.05)
    mg.GetYaxis().CenterTitle(True)
    mg.GetXaxis().SetTitleOffset(1.01)
    mg.GetXaxis().CenterTitle(True)
    mg.GetXaxis().SetMoreLogLabels(True)

    # histo to shade
    n=len(fChain)

    grgreen = rt.TGraph(2*n)
    for i in range(0,n):
        grgreen.SetPoint(i,radmasses[i],y2up[i])
        grgreen.SetPoint(n+i,radmasses[n-i-1],y2down[n-i-1])

    grgreen.SetFillColor(rt.kGreen)
    grgreen.Draw("f") 


    gryellow = rt.TGraph(2*n)
    for i in range(0,n):
        gryellow.SetPoint(i,radmasses[i],y1up[i])
        gryellow.SetPoint(n+i,radmasses[n-i-1],y1down[n-i-1])

    gryellow.SetFillColor(rt.kYellow)
    gryellow.Draw("f,same") 

    grmean.Draw("L*")
    if obs: grobs.Draw("L,P,E")

    leg = rt.TLegend(0.70,0.71,0.95,0.89)
    leg.SetFillColor(rt.kWhite)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)

    leg.AddEntry(grmean, "Median", "L")
    if obs: leg.AddEntry(grobs, "Observed", "L,P")
    leg.AddEntry(grgreen, "98%", "f")
    leg.AddEntry(gryellow, "68%", "f")
    leg.SetHeader("X #rightarrow %s" %label)

    leg.Draw()

    if withAcceptance:
        c1.SaveAs("brazilianFlag_acc_%s.pdf" %label)
    else:
        c1.SaveAs("brazilianFlag_%s.pdf" %label)


if __name__ == '__main__':

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel2_m2mg2.root"],
          "WW", True)

    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel2_m2mg2.root"],
          "ZZ", True)

    Plot(["Xvv.mX1000.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1100.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1200.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1300.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1400.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1500.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1600.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1700.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1800.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1900.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2000.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2100.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2200.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2300.0_WW_MarkovChainMC_8TeV_channel2_m2mg2.root"],
          "WW_MarkovChainMC", True)

    Plot(["Xvv.mX1000.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1100.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1200.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1300.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1400.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1500.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1600.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1700.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1800.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX1900.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2000.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2100.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2200.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root",
          "Xvv.mX2300.0_ZZ_MarkovChainMC_8TeV_channel2_m2mg2.root"],
          "ZZ_MarkovChainMC", True)
