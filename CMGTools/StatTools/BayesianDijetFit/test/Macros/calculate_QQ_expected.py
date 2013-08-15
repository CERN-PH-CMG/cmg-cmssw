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


prefix="plots_QQ_4677pbm1/limit"

statl=[0]

for statlevel in statl:


    print "statlevel ", statlevel

#    if __name__ == '__main__':
    twosigma_lowers=[]
    onesigma_lowers=[]
    medians=[]
    onesigma_uppers=[]
    twosigma_uppers=[]

        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]
#    masses =[1800, 1800]



    axigluon_newcut = [0.3967E+02, 0.2400E+02, 0.1486E+02, 0.9370E+01, 0.5998E+01, 0.3887E+01, 0.2544E+01, 0.1678E+01, 0.1115E+01, 0.7442E+00, 0.4988E+00, 0.3354E+00, 0.2260E+00, 0.1525E+00, 0.1030E+00, 0.6949E-01, 0.4684E-01, 0.3152E-01, 0.2116E-01, 0.1415E-01, 0.9428E-02, 0.6250E-02, 0.4119E-02, 0.2698E-02, 0.1754E-02, 0.1131E-02, 0.7222E-03, 0.4568E-03, 0.2858E-03, 0.1767E-03, 0.1079E-03, 0.6500E-04, 0.3863E-04, 0.2263E-04, 0.1307E-04, 0.7436E-05]

#    diquark_newcut  = [0.2393E+02, 0.1648E+02, 0.1154E+02, 0.8194E+01, 0.5877E+01, 0.4249E+01, 0.3090E+01, 0.2258E+01, 0.1656E+01, 0.1217E+01, 0.8953E+00, 0.6591E+00, 0.4852E+00, 0.3569E+00, 0.2622E+00, 0.1922E+00];

    string_newcut   = [1391.71, 787.875, 473.13, 290.1, 177.6, 111.91, 72.12, 47.07, 31.05, 20.60, 13.82, 9.117, 6.244, 4.238, 2.881, 1.973, 1.367, 0.93418, 0.644897, 0.445018, 0.304044, 0.212039, 0.143917, 0.099195, 0.066999, 0.0462355, 0.0313552, 0.021399, 0.0141453, 0.0095593, 0.0064255];
 
    zprime_newcut   = [0.1172E+01, 0.7171E+00, 0.4486E+00, 0.2857E+00, 0.1845E+00, 0.1206E+00, 0.7961E-01, 0.5295E-01, 0.3545E-01, 0.2386E-01, 0.1611E-01, 0.1092E-01, 0.7413E-02, 0.5039E-02, 0.3426E-02, 0.2329E-02, 0.1580E-02, 0.1070E-02, 0.7231E-03, 0.4867E-03, 0.3261E-03, 0.2174E-03, 0.1440E-03, 0.9477E-04, 0.6190E-04, 0.4007E-04]; 

    wprime_newcut   = [0.2212E+01, 0.1372E+01, 0.8673E+00, 0.5568E+00, 0.3616E+00, 0.2369E+00, 0.1562E+00, 0.1034E+00, 0.6872E-01, 0.4572E-01, 0.3043E-01, 0.2023E-01, 0.1342E-01, 0.8884E-02, 0.5859E-02, 0.3847E-02, 0.2513E-02, 0.1632E-02, 0.1053E-02, 0.6744E-03, 0.4287E-03, 0.2702E-03, 0.1688E-03, 0.1044E-03, 0.6403E-04, 0.3886E-04];

#twosigma_lower = 0.0533168 ;  onesigma_lower = 0.0907331 ;  median = 0.114855 ;  onesigma_upper = 0.157456 ;  twosigma_upper = 0.212495

        
    for mass in masses:
        print "mass = ",mass
        print "start to look inside the fit output"
        os.system("tar -xvf out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_RSGraviton_ak5_QQtoQQ_fat30.txt.tar.gz")
        f_fit=file("out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_RSGraviton_ak5_QQtoQQ_fat30.txt")
        print "we look inside the fit output"
        for line in f_fit.readlines():
            if "twosigma_lower" in line:
                twosigma_lowers+=[float(line.split("=")[3].split(";")[0])]
                onesigma_lowers+=[float(line.split("=")[4].split(";")[0])]
                medians+=[float(line.split("=")[5].split(";")[0])]
                onesigma_uppers+=[float(line.split("=")[6].split(";")[0])]               
                twosigma_uppers+=[float(line.split("=")[7])]               

        f_fit.close()
        os.system("rm out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_RSGraviton_ak5_QQtoQQ_fat30.txt")
        

    print "twosigma_lower",[(masses[i],twosigma_lowers[i]) for i in range(len(twosigma_lowers))]
    print "onesigma_lower",[(masses[i],onesigma_lowers[i]) for i in range(len(onesigma_lowers))]
    print "median",[(masses[i],medians[i]) for i in range(len(medians))]
    print "onesigma_upper",[(masses[i],onesigma_uppers[i]) for i in range(len(onesigma_uppers))]
    print "twosigma_upper",[(masses[i],twosigma_uppers[i]) for i in range(len(twosigma_uppers))]



    canvas = TCanvas("canvas","canvas",0,0,150,150)
    canvas.Divide(1,1)
    canvas.cd(1)
    canvas.GetPad(1).SetLogy()
    legend=TLegend(0.5,0.7,0.9,0.9,"")
    htwosigma_lowers = TH1F("htwosigma_lowers","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    honesigma_lowers = TH1F("honesigma_lowers","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    hmedians = TH1F("hmedians","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    honesigma_uppers = TH1F("honesigma_uppers","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    htwosigma_uppers = TH1F("htwosigma_uppers","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)


    for i in range(len(masses)):
        htwosigma_lowers.SetBinContent(i+1,twosigma_lowers[i])
    for i in range(len(masses)):
        honesigma_lowers.SetBinContent(i+1,onesigma_lowers[i])
    for i in range(len(masses)):
        hmedians.SetBinContent(i+1,medians[i])
    for i in range(len(masses)):
        honesigma_uppers.SetBinContent(i+1,onesigma_uppers[i])
    for i in range(len(masses)):
        htwosigma_uppers.SetBinContent(i+1,twosigma_uppers[i])
        
        
    hmedians.SetLineColor(1)
    hmedians.GetXaxis().SetTitle("dijet mass [GeV]")
    hmedians.GetXaxis().SetRangeUser(masses[0],masses[-1])
    hmedians.GetYaxis().SetTitle("xsec * BR * acc [pb]")
    hmedians.GetYaxis().SetRangeUser(1e-5,1e2)
    hmedians.GetXaxis().SetTitleOffset(1.0)
    hmedians.GetYaxis().SetTitleOffset(1.4)
    hmedians.Draw("L")


    htwosigma_lowers.SetFillColor(10);
    honesigma_lowers.SetFillColor(kGreen);
    honesigma_uppers.SetFillColor(kYellow);
    htwosigma_uppers.SetFillColor(kGreen);


    htwosigma_lowers.SetLineColor(kGreen);
    honesigma_lowers.SetLineColor(kYellow);
    honesigma_uppers.SetLineColor(kYellow);
    htwosigma_uppers.SetLineColor(kGreen);

    
    htwosigma_uppers.Draw("SAME")
    honesigma_uppers.Draw("SAME")
    hmedians.Draw("SAMEL")
    honesigma_lowers.Draw("SAME")
    htwosigma_lowers.Draw("SAME")

    axigluon = TH1F("axigluon","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)):
        axigluon.SetBinContent(i+1,axigluon_newcut[i])
    axigluon.SetLineColor(kRed)
    axigluon.SetLineStyle(2)
    axigluon.Draw("LSAME")


    zprime = TH1F("zprime","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-9):
        zprime.SetBinContent(i+1,zprime_newcut[i])
    zprime.SetLineColor(kMagenta)
    zprime.SetLineStyle(3)
    zprime.Draw("LSAME")

    wprime = TH1F("wprime","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-9):
        wprime.SetBinContent(i+1,wprime_newcut[i])
    wprime.SetLineColor(kGreen)
    wprime.SetLineStyle(4)
    wprime.Draw("LSAME")

    legend.AddEntry(axigluon,"A -> qq","l")
    legend.AddEntry(wprime,"W' -> qq","l")
    legend.AddEntry(zprime,"Z' -> qq","l")

    
    legend.Draw()
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_expectedlimit50_'+str(statlevel)+'.root', "RECREATE")
    axigluon.Write()
    zprime.Write()
    wprime.Write()
    htwosigma_lowers.Write()
    honesigma_lowers.Write()
    hmedians.Write()
    honesigma_uppers.Write()
    htwosigma_uppers.Write()
    fileRoot.Close()
    os.system("ghostview "+prefix + '_expectedlimit50_'+str(statlevel)+'.eps&')
