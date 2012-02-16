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


prefix="plots_Qstar_4677pbm1/limit"

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

    qstar_newcut=[0.7005E+02, 0.4039E+02, 0.2394E+02,  0.1452E+02, 0.8982E+01,  0.5645E+01,  0.3596E+01,  0.2317E+01, 0.1507E+01, 0.9889E+00, 0.6531E+00, 0.4338E+00, 0.2896E+00, 0.1940E+00 ,  0.1304E+00, 0.8782E-01, 0.5925E-01,  0.4002E-01,  0.2704E-01, 0.1828E-01, 0.1234E-01, 0.8329E-02, 0.5613E-02,  0.3776E-02, 0.2535E-02, 0.1698E-02,  0.1135E-02, 0.7559E-03, 0.5021E-03, 0.3325E-03, 0.2195E-03]

#twosigma_lower = 0.0533168 ;  onesigma_lower = 0.0907331 ;  median = 0.114855 ;  onesigma_upper = 0.157456 ;  twosigma_upper = 0.212495

        
    for mass in masses:
        print "mass = ",mass
        print "start to look inside the fit output"
        os.system("tar -xvf out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_Qstar_ak5_fat30.txt.tar.gz")
        f_fit=file("out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_Qstar_ak5_fat30.txt")
        print "we look inside the fit output"
        for line in f_fit.readlines():
            if "twosigma_lower" in line:
                twosigma_lowers+=[float(line.split("=")[3].split(";")[0])]
                onesigma_lowers+=[float(line.split("=")[4].split(";")[0])]
                medians+=[float(line.split("=")[5].split(";")[0])]
                onesigma_uppers+=[float(line.split("=")[6].split(";")[0])]               
                twosigma_uppers+=[float(line.split("=")[7])]               

        f_fit.close()
        os.system("rm out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_Qstar_ak5_fat30.txt")

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

    qstarXsec = TH1F("qstarXsec","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-4):
        qstarXsec.SetBinContent(i+1,qstar_newcut[i])
    qstarXsec.SetLineColor(kRed)
    qstarXsec.SetLineStyle(2)
    qstarXsec.Draw("LSAME")


    legend.AddEntry(qstarXsec,"Q* -> qg","l")

    
    legend.Draw()
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_expectedlimit50_'+str(statlevel)+'.root', "RECREATE")
    qstarXsec.Write()
    htwosigma_lowers.Write()
    honesigma_lowers.Write()
    hmedians.Write()
    honesigma_uppers.Write()
    htwosigma_uppers.Write()
    fileRoot.Close()
    os.system("ghostview "+prefix + '_expectedlimit50_'+str(statlevel)+'.eps&')
