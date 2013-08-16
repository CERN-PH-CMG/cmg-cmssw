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

statl=[6]

for statlevel in statl:


    print "statlevel ", statlevel

#    if __name__ == '__main__':
#    Limits stat only
#    limits = [0.84578500000000001, 0.705538, 0.64213600000000004, 0.46875, 0.29454599999999997, 0.24405099999999999, 0.20947099999999999, 0.19139, 0.13278400000000001, 0.119934, 0.133213, 0.11694599999999999, 0.104599, 0.108589, 0.104201, 0.089100899999999997, 0.064506900000000006, 0.048416899999999999, 0.042190699999999998, 0.034851699999999999, 0.0256902, 0.018396800000000001, 0.0130501, 0.010163500000000001, 0.0089183600000000002, 0.0082032400000000005, 0.0078152199999999995, 0.00753311, 0.0072137499999999997, 0.0067167099999999999, 0.0062106699999999997]

#    Limits sys
    limits = [0.93722099999999997, 0.75419599999999998, 0.64448899999999998, 0.48904999999999998, 0.31066500000000002, 0.251888, 0.21739700000000001, 0.18430199999999999, 0.13472300000000001, 0.12872900000000001, 0.137852, 0.12853100000000001, 0.113815, 0.107624, 0.101768, 0.091490600000000005, 0.074874099999999999, 0.0576623, 0.044042999999999999, 0.033631899999999999, 0.0256902, 0.019648200000000001, 0.0150659, 0.0116925, 0.0096806099999999992, 0.0084744299999999998, 0.0077605699999999996, 0.0073944600000000003, 0.0071200600000000001, 0.00678256, 0.0063770199999999997]

    eff=[0.752614, 0.829898, 0.87992, 0.91429, 0.936309, 0.951325, 0.962299, 0.971395, 0.975795, 0.9811, 0.985793, 0.988608, 0.988827, 0.992177, 0.996037, 0.99614, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]
#    masses =[1800, 1800]
    
    qstar_newcut=[0.7005E+02, 0.4039E+02, 0.2394E+02,  0.1452E+02, 0.8982E+01,  0.5645E+01,  0.3596E+01,  0.2317E+01, 0.1507E+01, 0.9889E+00, 0.6531E+00, 0.4338E+00, 0.2896E+00, 0.1940E+00 ,  0.1304E+00, 0.8782E-01, 0.5925E-01,  0.4002E-01,  0.2704E-01, 0.1828E-01, 0.1234E-01, 0.8329E-02, 0.5613E-02,  0.3776E-02, 0.2535E-02, 0.1698E-02,  0.1135E-02, 0.7559E-03, 0.5021E-03, 0.3325E-03, 0.2195E-03]
        
#    qstar_newcut=[0.1507E+01, 0.9889E+00]
        
     
    canvas = TCanvas("canvas","canvas",0,0,150,150)
    canvas.Divide(1,1)
    canvas.cd(1)
    canvas.GetPad(1).SetLogy()
    legend=TLegend(0.5,0.7,0.9,0.9,"")
    h = TH1F("limit","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)):
        h.SetBinContent(i+1,limits[i]/eff[i])
        
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
