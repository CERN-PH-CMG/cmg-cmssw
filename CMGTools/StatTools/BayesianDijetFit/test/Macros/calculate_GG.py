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


prefix="plots_GG/limit"

statl=[6]

for statlevel in statl:


    print "statlevel ", statlevel

#    if __name__ == '__main__':
    limits=[]
        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]
#    masses =[1800, 1800]
    

    rsg_newcut=[0.1053E+01, 0.5905E+00, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01, 0.4911E-01, 0.3145E-01, 0.2036E-01, 0.1330E-01, 0.8743E-02, 0.5781E-02, 0.3840E-02, 0.2559E-02, 0.1708E-02, 0.1142E-02, 0.7635E-03, 0.5101E-03, 0.3402E-03, 0.2264E-03, 0.1501E-03, 0.9913E-04, 0.6512E-04, 0.4253E-04, 0.2759E-04, 0.1775E-04]
        
#    qstar_newcut=[0.1507E+01, 0.9889E+00]
        
    for mass in masses:
        print "mass = ",mass
        if mass > 3801:
            os.system("dijetStatsNewBackground "+str(mass)+" " + str(statlevel) + " 0 RSGraviton_ak5_GGtoGG_fat30 -b -q &> plots_GG/masslimit"+str(mass)+"_"+str(statlevel)+".txt")
        print "start to look inside the fit output"
        f_fit=file("plots_GG/masslimit"+str(mass)+"_"+str(statlevel)+".txt")
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
    rsgXsec = TH1F("rsgXsec","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-5):
        rsgXsec.SetBinContent(i+1,rsg_newcut[i])

    rsgXsec.SetLineColor(kRed)
    rsgXsec.SetLineStyle(2)
    rsgXsec.Draw("LSAME")
    legend.AddEntry(rsgXsec,"RS -> gg","l")
    legend.Draw()
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_limit_'+str(statlevel)+'.root', "RECREATE")
    rsgXsec.Write()
    h.Write()
    fileRoot.Close()
#    os.system("ghostview "+prefix + '_limit_'+str(statlevel)+'.eps&')
