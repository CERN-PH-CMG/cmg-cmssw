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


prefix="plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit"

statl=[6]

#shapes=["RSGraviton_ak5_GGtoGG_fat30", "RSGraviton_ak5_QQtoQQ_fat30", "Qstar_ak5_fat30", "RSGraviton_HLT_ak5_GGtoGG_fat30", "RSGraviton_HLT_ak5_QQtoQQ_fat30", "Qstar_HLT_ak5_fat30", "RSGraviton_HLT_ak5_GGtoGG_pf", "RSGraviton_HLT_ak5_QQtoQQ_pf", "Qstar_HLT_ak5_pf"]

shapes=["Qstar_qW_ak5_fat", "Qstar_qZ_ak5_fat"]
shapes+=["RSGraviton_WW_ak5_fat", "RSGraviton_WZ_ak5_fat", "RSGraviton_ZZ_ak5_fat"]
#shapes=["Qstar_qZ_ak5_fat", "RSGraviton_WW_ak5_fat", "RSGraviton_WZ_ak5_fat", "RSGraviton_ZZ_ak5_fat"]


for statlevel in statl:


    for sShapes in shapes:

        print "statlevel ", statlevel

        #    if __name__ == '__main__':
        twosigma_lowers=[]
        onesigma_lowers=[]
        medians=[]
        onesigma_uppers=[]
        twosigma_uppers=[]

        if "_HLT_" in sShapes:
            masses =[600., 700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.]
        elif ("qW" in sShapes) or ("qZ" in sShapes):
            masses = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300.]
        elif ("WW" in sShapes) or ("ZZ" in sShapes) or ("WZ" in sShapes):
            masses = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100.]
        else: 
            masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]
        

        
        for mass in masses:
            print "mass = ",mass
            print "start to look inside the fit output"
            found=False
	    try:
                os.system("tar -xvf out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_" + sShapes + ".txt.tar.gz")
                f_fit=file("out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_" + sShapes + ".txt")
                print "we look inside the fit output"
                for line in f_fit.readlines():
                  if "twosigma_lower" in line:
                    twosigma_lowers+=[float(line.split("=")[3].split(";")[0])]
                    onesigma_lowers+=[float(line.split("=")[4].split(";")[0])]
                    medians+=[float(line.split("=")[5].split(";")[0])]
                    onesigma_uppers+=[float(line.split("=")[6].split(";")[0])]               
                    twosigma_uppers+=[float(line.split("=")[7])]               
                    print float(line.split("=")[3].split(";")[0])
		    found=True
                f_fit.close()
                os.system("rm out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_50_" + sShapes + ".txt")
	    except:
	       pass

            if not found:
	        print mass, "not found"
	    #    twosigma_lowers+=[twosigma_lowers[-1]]
	    #    onesigma_lowers+=[onesigma_lowers[-1]]
	    #    medians+=[medians[-1]]
	    #    onesigma_uppers+=[onesigma_uppers[-1]]
	    #    twosigma_uppers+=[twosigma_uppers[-1]]


        print "twosigma_lower",[(masses[i],twosigma_lowers[i]) for i in range(len(twosigma_lowers))]
        print "onesigma_lower",[(masses[i],onesigma_lowers[i]) for i in range(len(onesigma_lowers))]
        print "median",[(masses[i],medians[i]) for i in range(len(medians))]
        print "onesigma_upper",[(masses[i],onesigma_uppers[i]) for i in range(len(onesigma_uppers))]
        print "twosigma_upper",[(masses[i],twosigma_uppers[i]) for i in range(len(twosigma_uppers))]

        canvas = TCanvas("canvas","canvas",0,0,500,500)
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

        canvas.Update()


#        legend.Draw()

        fileRoot = TFile(prefix + '_expectedlimit50_' + sShapes + '_'+str(statlevel)+'.root', "RECREATE")
        htwosigma_lowers.Write()
        honesigma_lowers.Write()
        hmedians.Write()
        honesigma_uppers.Write()
        htwosigma_uppers.Write()
        fileRoot.Close()
 #       canvas.SaveAs(prefix + '_expectedlimit50_' + sShapes + '_' + str(statlevel)+'.root')
        canvas.SaveAs(prefix + '_expectedlimit50_' + sShapes + '_' + str(statlevel)+'.png')
        canvas.SaveAs(prefix + '_expectedlimit50_' + sShapes + '_' + str(statlevel)+'.eps')
 #       os.system("display "+prefix + '_expectedlimit50_'+ sShapes + '_' + str(statlevel)+'.png&')


    

