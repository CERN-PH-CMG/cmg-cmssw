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

statl=[0,1,2,3,4,6]

#shapes=["RSGraviton_ak5_GGtoGG_fat30", "RSGraviton_ak5_QQtoQQ_fat30", "Qstar_ak5_fat30", "RSGraviton_HLT_ak5_GGtoGG_fat30", "RSGraviton_HLT_ak5_QQtoQQ_fat30", "Qstar_HLT_ak5_fat30", "RSGraviton_HLT_ak5_GGtoGG_pf", "RSGraviton_HLT_ak5_QQtoQQ_pf", "Qstar_HLT_ak5_pf"]

shapes=["Qstar_qW_ak5_fat", "Qstar_qZ_ak5_fat"]
shapes+=["RSGraviton_WW_ak5_fat", "RSGraviton_WZ_ak5_fat", "RSGraviton_ZZ_ak5_fat"]
#shapes=["QBH_ak5_fat"]

for statlevel in statl:


    for sShapes in shapes:

        print "statlevel ", statlevel

        #    if __name__ == '__main__':
        limits=[]
 
        if "_HLT_" in sShapes:
            masses =[600., 700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.]
        elif ("qW" in sShapes) or ("qZ" in sShapes):
            masses = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300.]
        elif ("WW" in sShapes) or ("ZZ" in sShapes) or ("WZ" in sShapes):
            masses = [1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100.]
        elif ("QBH" in sShapes):
            masses = [3000., 3200., 3400., 3600., 3800., 4000., 4200., 4400., 4600., 4800., 5000.]
        else: 
            masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]
        
        
        for mass in masses:
            print "mass = ",mass
            print "start to look inside the fit output"
            os.system("tar -xvf out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_0_" + sShapes + ".txt.tar.gz")
            f_fit=file("out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_0_" + sShapes + ".txt")
            print "we look inside the fit output"
            for line in f_fit.readlines():
                if "upperlimit" in line:
                    limits+=[float(line.split("=")[5])]
                    print "limits ", float(line.split("=")[5])
            f_fit.close()

            os.system("rm out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_0_" + sShapes + ".txt")

        print "limits",[(masses[i],limits[i]) for i in range(len(limits))]

        canvas = TCanvas("canvas","canvas",0,0,500,500)
        canvas.Divide(1,1)


        canvas.cd(1)
        canvas.GetPad(1).SetLogy()
        legend=TLegend(0.5,0.7,0.9,0.9,"")
        h = TH1F("limit","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)

        for i in range(len(masses)):
            h.SetBinContent(i+1,limits[i])
        
    
        h.Draw("")

        canvas.Update()


#        legend.Draw()

        fileRoot = TFile(prefix + '_limit_' + sShapes + '_'+str(statlevel)+'.root', "RECREATE")
        h.Write()
        fileRoot.Close()
#        canvas.SaveAs(prefix + '_limit_' + sShapes + '_' + str(statlevel)+'.root')
        canvas.SaveAs(prefix + '_limit_' + sShapes + '_' + str(statlevel)+'.png')
        canvas.SaveAs(prefix + '_limit_' + sShapes + '_' + str(statlevel)+'.eps')
#        os.system("display "+prefix + '_limit_'+ sShapes + '_' + str(statlevel)+'.png&')


    

