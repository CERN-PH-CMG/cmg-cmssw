from ROOT import *
import ROOT
import array, math
import os

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.03)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(1.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)


prefix="plots_Qstar/limit"

statl=[0,1,2,3,4,6]

for statlevel in statl:


    print "statlevel ", statlevel

#    if __name__ == '__main__':
    limits=[]
        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]
#    masses =[1800, 1800]
    
    qstar_newcut=[0.7005E+02, 0.4039E+02, 0.2394E+02,  0.1452E+02, 0.8982E+01,  0.5645E+01,  0.3596E+01,  0.2317E+01, 0.1507E+01, 0.9889E+00, 0.6531E+00, 0.4338E+00, 0.2896E+00, 0.1940E+00 ,  0.1304E+00, 0.8782E-01, 0.5925E-01,  0.4002E-01,  0.2704E-01, 0.1828E-01, 0.1234E-01, 0.8329E-02, 0.5613E-02,  0.3776E-02, 0.2535E-02, 0.1698E-02,  0.1135E-02, 0.7559E-03, 0.5021E-03, 0.3325E-03, 0.2195E-03]
        
#    qstar_newcut=[0.1507E+01, 0.9889E+00]
        
    for mass in masses:
        print "mass = ",mass
        os.system("dijetStatsNewBackground "+str(mass)+" " + str(statlevel) + " 0 Qstar_ak5_fat30 -b -q &> plots_Qstar/masslimit"+str(mass)+"_"+str(statlevel)+".txt")
        print "start to look inside the fit output"
        f_fit=file("plots_Qstar/masslimit"+str(mass)+"_"+str(statlevel)+".txt")
        print "we look inside the fit output"
        for line in f_fit.readlines():
            if "upperlimit" in line:
                limits+=[float(line.split("=")[5])]
                print "limits ", float(line.split("=")[5])
        f_fit.close()

    print "limits",[(masses[i],limits[i]) for i in range(len(limits))]
    s = "limits",[(masses[i],limits[i]) for i in range(len(limits))]
    
    f1 = open(prefix+"_"+ str(statlevel)+".txt", "w")
    f1.write(str(s))
    f1.close()
    
    canvas = TCanvas("canvas","canvas",0,0,150,150)
    canvas.Divide(1,1)
    canvas.cd(1)
    canvas.GetPad(1).SetLogy()
    legend=TLegend(0.5,0.7,0.9,0.9,"")
    h = TH1F("limit","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)):
        h.SetBinContent(i+1,limits[i])
        
    h.SetLineColor(1)
    h.GetXaxis().SetTitle("dijet mass [GeV]")
    h.GetXaxis().SetRangeUser(masses[0],masses[-1])
    h.GetYaxis().SetTitle("xsec * BR * acc [pb]")
    h.GetYaxis().SetRangeUser(1e-5,1e2)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.4)
    h.Draw("L")
    legend.AddEntry(h,"Q* limit","l")
    qstarXsec = TH1F("qstarXsec","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)):
        qstarXsec.SetBinContent(i+1,qstar_newcut[i])

    qstarXsec.SetLineColor(kRed)
    qstarXsec.SetLineStyle(2)
    qstarXsec.Draw("LSAME")
    legend.AddEntry(qstarXsec,"Q* -> qg","l")
    legend.Draw()
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_limit_'+str(statlevel)+'.root', "RECREATE")
    qstarXsec.Write()
    h.Write()
    fileRoot.Close()
#    os.system("ghostview "+prefix + '_limit_'+str(statlevel)+'.eps&')
