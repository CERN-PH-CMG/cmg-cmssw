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


prefix="plots_QQ/limit"

statl=[6]

for statlevel in statl:


    print "statlevel ", statlevel

#    if __name__ == '__main__':
#    Limits stat only
#    limits = [0.80006699999999997, 0.62951000000000001, 0.60920600000000003, 0.43184, 0.25351499999999999, 0.21382499999999999, 0.18909000000000001, 0.172487, 0.11049200000000001, 0.10394200000000001, 0.115982, 0.094329300000000005, 0.086167800000000003, 0.090249599999999999, 0.088388099999999997, 0.074079999999999993, 0.050972000000000003, 0.0389281, 0.035399100000000003, 0.029449699999999999, 0.0212608, 0.014517199999999999, 0.0098671200000000001, 0.0077350700000000001, 0.0068602799999999999, 0.0064405599999999997, 0.0062303200000000001, 0.00605421,0.0058084699999999996, 0.0053997100000000003, 0.0049352600000000003] 

    eff = [0.815421, 0.877213, 0.912979, 0.938578, 0.953967, 0.96473, 0.972428, 0.978418, 0.980751, 0.984678, 0.988485, 0.991648, 0.991917, 0.99461, 0.997463, 0.997536, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
     
# Limits with SYS
    limits=[0.89531300000000003, 0.68120899999999995, 0.60450199999999998, 0.45583099999999999, 0.27109899999999998, 0.22278100000000001, 0.195884, 0.164217, 0.11243, 0.111138, 0.119958, 0.107017, 0.092618900000000004, 0.088801699999999997, 0.085144499999999998, 0.076469700000000002, 0.061915100000000001, 0.046956999999999999, 0.035604900000000002, 0.0273586, 0.020965500000000001, 0.0157687, 0.0117769, 0.0090842000000000006, 0.0074700799999999996, 0.0065083600000000004, 0.0061210099999999996, 0.0059155700000000002, 0.00571479, 0.0054655600000000004, 0.0051570699999999997]
        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000.]
#    masses =[1800, 1800]



    axigluon_newcut = [0.3967E+02, 0.2400E+02, 0.1486E+02, 0.9370E+01, 0.5998E+01, 0.3887E+01, 0.2544E+01, 0.1678E+01, 0.1115E+01, 0.7442E+00, 0.4988E+00, 0.3354E+00, 0.2260E+00, 0.1525E+00, 0.1030E+00, 0.6949E-01, 0.4684E-01, 0.3152E-01, 0.2116E-01, 0.1415E-01, 0.9428E-02, 0.6250E-02, 0.4119E-02, 0.2698E-02, 0.1754E-02, 0.1131E-02, 0.7222E-03, 0.4568E-03, 0.2858E-03, 0.1767E-03, 0.1079E-03, 0.6500E-04, 0.3863E-04, 0.2263E-04, 0.1307E-04, 0.7436E-05]

#    diquark_newcut  = [0.2393E+02, 0.1648E+02, 0.1154E+02, 0.8194E+01, 0.5877E+01, 0.4249E+01, 0.3090E+01, 0.2258E+01, 0.1656E+01, 0.1217E+01, 0.8953E+00, 0.6591E+00, 0.4852E+00, 0.3569E+00, 0.2622E+00, 0.1922E+00];

    string_newcut   = [1391.71, 787.875, 473.13, 290.1, 177.6, 111.91, 72.12, 47.07, 31.05, 20.60, 13.82, 9.117, 6.244, 4.238, 2.881, 1.973, 1.367, 0.93418, 0.644897, 0.445018, 0.304044, 0.212039, 0.143917, 0.099195, 0.066999, 0.0462355, 0.0313552, 0.021399, 0.0141453, 0.0095593, 0.0064255];
 
    zprime_newcut   = [0.1172E+01, 0.7171E+00, 0.4486E+00, 0.2857E+00, 0.1845E+00, 0.1206E+00, 0.7961E-01, 0.5295E-01, 0.3545E-01, 0.2386E-01, 0.1611E-01, 0.1092E-01, 0.7413E-02, 0.5039E-02, 0.3426E-02, 0.2329E-02, 0.1580E-02, 0.1070E-02, 0.7231E-03, 0.4867E-03, 0.3261E-03, 0.2174E-03, 0.1440E-03, 0.9477E-04, 0.6190E-04, 0.4007E-04]; 

    wprime_newcut   = [0.2212E+01, 0.1372E+01, 0.8673E+00, 0.5568E+00, 0.3616E+00, 0.2369E+00, 0.1562E+00, 0.1034E+00, 0.6872E-01, 0.4572E-01, 0.3043E-01, 0.2023E-01, 0.1342E-01, 0.8884E-02, 0.5859E-02, 0.3847E-02, 0.2513E-02, 0.1632E-02, 0.1053E-02, 0.6744E-03, 0.4287E-03, 0.2702E-03, 0.1688E-03, 0.1044E-03, 0.6403E-04, 0.3886E-04];

        
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
        h.SetBinContent(i+1,limits[i]/eff[i])
        
    h.SetLineColor(1)
    h.GetXaxis().SetTitle("dijet mass [GeV]")
    h.GetXaxis().SetRangeUser(masses[0],masses[-1])
    h.GetYaxis().SetTitle("xsec * BR * acc [pb]")
    h.GetYaxis().SetRangeUser(1e-5,1e2)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.4)
    h.Draw("L")

    axigluon = TH1F("axigluon","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)):
        axigluon.SetBinContent(i+1,axigluon_newcut[i])
    axigluon.SetLineColor(kRed)
    axigluon.SetLineStyle(2)
    axigluon.Draw("LSAME")


    zprime = TH1F("zprime","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-6):
        zprime.SetBinContent(i+1,zprime_newcut[i])
    zprime.SetLineColor(kMagenta)
    zprime.SetLineStyle(3)
    zprime.Draw("LSAME")

    wprime = TH1F("wprime","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-6):
        wprime.SetBinContent(i+1,wprime_newcut[i])
    wprime.SetLineColor(kGreen)
    wprime.SetLineStyle(4)
    wprime.Draw("LSAME")

    legend.AddEntry(axigluon,"A -> qq","l")
    legend.AddEntry(wprime,"W' -> qq","l")
    legend.AddEntry(zprime,"Z' -> qq","l")

    
    legend.Draw()
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_limit_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_limit_'+str(statlevel)+'.root', "RECREATE")
    axigluon.Write()
    zprime.Write()
    wprime.Write()
    h.Write()
    fileRoot.Close()
#    os.system("ghostview "+prefix + '_limit_'+str(statlevel)+'.eps&')
