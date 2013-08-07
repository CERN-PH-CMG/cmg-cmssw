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

#    Limits stat only
#    limits = [1.0019899999999999, 0.86975800000000003, 0.75268699999999999, 0.60716000000000003, 0.41617399999999999, 0.33137299999999997, 0.28306900000000002, 0.25164199999999998, 0.18803, 0.17030600000000001, 0.182919, 0.17597099999999999, 0.15390400000000001, 0.14430299999999999, 0.13623099999999999, 0.123239, 0.10108, 0.078099500000000002, 0.0600961, 0.045481399999999998, 0.034844100000000003, 0.026781699999999999, 0.020901300000000001, 0.016279600000000002, 0.0136443, 0.0117286, 0.010493199999999999, 0.0095665799999999999, 0.0091342899999999998, 0.0084288100000000001, 0.0079851399999999999]

# Limits with SYS
    limits = [0.92960100000000001, 0.827183, 0.73622200000000004, 0.58686000000000005, 0.40005499999999999, 0.321297, 0.27061400000000002, 0.25755, 0.18706100000000001, 0.16071099999999999, 0.17827999999999999, 0.16328300000000001, 0.14100199999999999, 0.144785, 0.14069100000000001, 0.123581, 0.089561000000000002, 0.065934499999999993, 0.056185699999999998, 0.046526999999999999, 0.035139400000000001, 0.025530199999999999, 0.018567199999999999, 0.0142109, 0.0123485, 0.0113896, 0.010493199999999999, 0.0096590100000000009, 0.0092279800000000002, 0.0083629599999999991, 0.0077633299999999997] 

    eff = [0.541388, 0.633169, 0.697597, 0.756154, 0.802959, 0.838591, 0.869259, 0.897656, 0.909851, 0.928901, 0.947016, 0.961476, 0.964754, 0.977442, 0.98903, 0.987961, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]

#    if __name__ == '__main__':
#    Limits stat only
    
        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]
#    masses =[1800, 1800]
    
    rsg_newcut=[0.1053E+01, 0.5905E+00, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01, 0.4911E-01, 0.3145E-01, 0.2036E-01, 0.1330E-01, 0.8743E-02, 0.5781E-02, 0.3840E-02, 0.2559E-02, 0.1708E-02, 0.1142E-02, 0.7635E-03, 0.5101E-03, 0.3402E-03, 0.2264E-03, 0.1501E-03, 0.9913E-04, 0.6512E-04, 0.4253E-04, 0.2759E-04, 0.1775E-04]   
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
    rsgXsec = TH1F("rsgXsec","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-5):
        rsgXsec.SetBinContent(i+1,rsg_newcut[i])

    rsgXsec.SetLineColor(kRed)
    rsgXsec.SetLineStyle(2)
    rsgXsec.Draw("LSAME")
    legend.AddEntry(rsgXsec,"RSG -> gg","l")
    legend.Draw()
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_limit_'+str(statlevel)+'.root', "RECREATE")
    rsgXsec.Write()
    h.Write()
    fileRoot.Close()
#    os.system("ghostview "+prefix + '_limit_'+str(statlevel)+'.eps&')
