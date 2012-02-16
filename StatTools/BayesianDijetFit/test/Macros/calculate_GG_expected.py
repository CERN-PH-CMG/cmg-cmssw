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


prefix="plots_GG_4677pbm1/limit"

statl=[0,6]

for statlevel in statl:


    print "statlevel ", statlevel

#    if __name__ == '__main__':
    twosigma_lowers=[]
    onesigma_lowers=[]
    medians=[]
    onesigma_uppers=[]
    twosigma_uppers=[]

        
    masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]


    rsg_newcut=[0.1053E+01, 0.5905E+00, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01, 0.4911E-01, 0.3145E-01, 0.2036E-01, 0.1330E-01, 0.8743E-02, 0.5781E-02, 0.3840E-02, 0.2559E-02, 0.1708E-02, 0.1142E-02, 0.7635E-03, 0.5101E-03, 0.3402E-03, 0.2264E-03, 0.1501E-03, 0.9913E-04, 0.6512E-04, 0.4253E-04, 0.2759E-04, 0.1775E-04]

#twosigma_lower = 0.0533168 ;  onesigma_lower = 0.0907331 ;  median = 0.114855 ;  onesigma_upper = 0.157456 ;  twosigma_upper = 0.212495

        
    for mass in masses:
        print "mass = ",mass
        print "start to look inside the fit output"
        os.system("tar -xvf out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_RSGraviton_ak5_GGtoGG_fat30.txt.tar.gz")
        f_fit=file("out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_RSGraviton_ak5_GGtoGG_fat30.txt")
        print "we look inside the fit output"
        for line in f_fit.readlines():
            if "twosigma_lower" in line:
                twosigma_lowers+=[float(line.split("=")[3].split(";")[0])]
                onesigma_lowers+=[float(line.split("=")[4].split(";")[0])]
                medians+=[float(line.split("=")[5].split(";")[0])]
                onesigma_uppers+=[float(line.split("=")[6].split(";")[0])]               
                twosigma_uppers+=[float(line.split("=")[7])]               
                print float(line.split("=")[3].split(";")[0])

        f_fit.close()
        os.system("rm out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_RSGraviton_ak5_GGtoGG_fat30.txt")

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

    rsgXsec = TH1F("rsgXsec","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)
    for i in range(len(masses)-9):
        rsgXsec.SetBinContent(i+1,rsg_newcut[i])
    rsgXsec.SetLineColor(kRed)
    rsgXsec.SetLineStyle(2)
    rsgXsec.Draw("LSAME")


    legend.AddEntry(rsgXsec,"G -> gg","l")

    
    legend.Draw()
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.root')
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.pdf')
    canvas.SaveAs(prefix + '_expectedlimit50_'+str(statlevel)+'.eps')
    fileRoot = TFile(prefix + '_expectedlimit50_'+str(statlevel)+'.root', "RECREATE")
    rsgXsec.Write()
    htwosigma_lowers.Write()
    honesigma_lowers.Write()
    hmedians.Write()
    honesigma_uppers.Write()
    htwosigma_uppers.Write()
    fileRoot.Close()
    os.system("ghostview "+prefix + '_expectedlimit50_'+str(statlevel)+'.eps&')
