import ROOT as rt
from ROOT import *

withAcceptance=False

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

def Plot(files, label, obs):

    radmasses = []
    for f in files:
        radmasses.append(float(f.replace("Xvv.mX","").split("_")[0]))
    print radmasses

    efficiencies={}
    for mass in radmasses:
        # old version with 20 events in limit code
	#efficiencies[mass]=20.0/19600.0*1.0/0.1
        #if mass>2000:
	#    efficiencies[mass]/=(1-0.4*(mass-2000.0)/1000.0)
	#if not withAcceptance:
        #    efficiencies[mass]/=0.3
	efficiencies[mass]=30000.*0.005/19600.0
	#print mass,efficiencies[mass]

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
    mg.Add(grmean,"L")#.Draw("same,AC*")
    mg.Add(gr1down)#.Draw("same,AC*")
    mg.Add(gr2down)#.Draw("same,AC*")
    if obs: mg.Add(grobs,"L,P")#.Draw("AC*")
 
    c1.SetLogy(1)
    mg.SetTitle("")
    mg.Draw("AP")
    mg.GetXaxis().SetTitle("resonance mass (GeV)")
    if withAcceptance:
        mg.GetYaxis().SetTitle("#sigma #times BR(X #rightarrow "+label.split("_")[0]+") #times A (pb)")
    else:
        mg.GetYaxis().SetTitle("#sigma #times BR(X #rightarrow "+label.split("_")[0]+") (pb)")
    mg.GetYaxis().SetTitleSize(0.06)
    mg.GetXaxis().SetTitleSize(0.06)
    mg.GetXaxis().SetLabelSize(0.045)
    mg.GetYaxis().SetLabelSize(0.045)
    mg.GetYaxis().SetRangeUser(max(0.001,yobs[0]/300.),max(3,yobs[0]*3.))
    mg.GetYaxis().SetTitleOffset(1.4)
    mg.GetYaxis().CenterTitle(True)
    mg.GetXaxis().SetTitleOffset(1.1)
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

    grmean.Draw("L")
    if obs: grobs.Draw("L,P,E")

    leg = rt.TLegend(0.60,0.65,0.95,0.89)
    leg.SetFillColor(rt.kWhite)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.04)
    leg.SetBorderSize(0)

    if obs: leg.AddEntry(grobs, "Observed", "L,P")
    leg.AddEntry(grmean, "Expected", "L")
    leg.AddEntry(grgreen, "98%", "f")
    leg.AddEntry(gryellow, "68%", "f")
    leg.SetHeader("X #rightarrow %s" %label.split("_")[0])

    leg.Draw()

    if withAcceptance:
        c1.SaveAs("brazilianFlag_acc_%s.root" %label)
        c1.SaveAs("brazilianFlag_acc_%s.pdf" %label)
    else:
        c1.SaveAs("brazilianFlag_%s.root" %label)
        c1.SaveAs("brazilianFlag_%s.pdf" %label)


if __name__ == '__main__':
    """
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
          "WW_m2mg2", True)

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
          "ZZ_m2mg2", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel1_m2mg1.root"],
          "WW_m2mg1", True)

    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel1_m2mg1.root"],
          "ZZ_m2mg1", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel0_m2mg0.root"],
          "WW_m2mg0", True)

    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel0_m2mg0.root"],
          "ZZ_m2mg0", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel4_m1mg1.root"],
          "WW_m1mg1", True)

    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel4_m1mg1.root"],
          "ZZ_m1mg1", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel3_m1mg0.root"],
          "WW_m1mg0", True)

    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel3_m1mg0.root"],
          "ZZ_m1mg0", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel5_m0.root"],
          "WW_0m", True)

    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel5_m0.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel5_m0.root"],
          "ZZ_0m", True)

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
    """
    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel0.root"],
          "WW_high_purity", True)
    
    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel0.root"],
          "ZZ_high_purity", True)

    Plot(["Xvv.mX1000.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1100.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1200.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1300.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1400.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1500.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1600.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1700.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1800.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX1900.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2000.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2100.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2200.0_WZ_Asymptotic_8TeV_channel0.root",
          "Xvv.mX2300.0_WZ_Asymptotic_8TeV_channel0.root"],
          "WZ_high_purity", True)
    
    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel1.root"],
          "WW_medium_purity", True)
    
    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel1.root"],
          "ZZ_medium_purity", True)

    Plot(["Xvv.mX1000.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1100.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1200.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1300.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1400.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1500.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1600.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1700.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1800.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX1900.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2000.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2100.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2200.0_WZ_Asymptotic_8TeV_channel1.root",
          "Xvv.mX2300.0_WZ_Asymptotic_8TeV_channel1.root"],
          "WZ_medium_purity", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel2.root"],
          "WW_low_purity", True)
    
    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel2.root"],
          "ZZ_low_purity", True)

    Plot(["Xvv.mX1000.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1100.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1200.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1300.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1400.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1500.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1600.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1700.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1800.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX1900.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2000.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2100.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2200.0_WZ_Asymptotic_8TeV_channel2.root",
          "Xvv.mX2300.0_WZ_Asymptotic_8TeV_channel2.root"],
          "WZ_low_purity", True)

    Plot(["Xvv.mX1000.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1100.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1200.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1300.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1400.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1500.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1600.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1700.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1800.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1900.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2000.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2100.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2200.0_WW_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2300.0_WW_Asymptotic_8TeV_channel012.root"],
          "WW_combined", True)
    
    Plot(["Xvv.mX1000.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1100.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1200.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1300.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1400.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1500.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1600.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1700.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1800.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1900.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2000.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2100.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2200.0_ZZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2300.0_ZZ_Asymptotic_8TeV_channel012.root"],
          "ZZ_combined", True)

    Plot(["Xvv.mX1000.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1100.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1200.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1300.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1400.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1500.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1600.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1700.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1800.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX1900.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2000.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2100.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2200.0_WZ_Asymptotic_8TeV_channel012.root",
          "Xvv.mX2300.0_WZ_Asymptotic_8TeV_channel012.root"],
          "WZ_combined", True)

