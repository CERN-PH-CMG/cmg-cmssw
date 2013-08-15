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

#statl=[2000]
statl=[0]#, 11, 12, 100, 101, 102, 103, 104, 105, 1002, 1003, 1004]

#shapes=["RSGraviton_ak5_GGtoGG_fat30", "RSGraviton_ak5_QQtoQQ_fat30",
shapes = ["RSGraviton_ak5_GGtoGG_fat30", "RSGraviton_ak5_QQtoQQ_fat30", "Qstar_ak5_fat30"]


for statlevel in statl:


    for sShapes in shapes:

        print "statlevel ", statlevel

        norm=[]
        p1=[]
        p2=[]
        p3=[]
 
        masses =[1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300.]
        
        
        for mass in masses:
            print "mass = ",mass
            print "start to look inside the fit output"
            os.system("tar -xvf out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_0_" + sShapes + ".txt.tar.gz")
            f_fit=file("out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_0_" + sShapes + ".txt")
            print "we look inside the fit output"
            for line in f_fit.readlines():
                if "nbkg=" in line:
                    norm+=[float(line.split("=")[1].split("+")[0])]
                    print "---------------------------- nbkg ", float(line.split("=")[1].split("+")[0])
                if "p1=" in line:
                    p1+=[float(line.split("=")[1].split("+")[0])]
                    print "---------------------------- p1 ", float(line.split("=")[1].split("+")[0])
                if "p2=" in line:
                    p2+=[float(line.split("=")[1].split("+")[0])]
                    print "---------------------------- p2 ", float(line.split("=")[1].split("+")[0])
                if "p3=" in line:
                    p3+=[float(line.split("=")[1].split("+")[0])]
                    print "---------------------------- p3 ", float(line.split("=")[1].split("+")[0])


            f_fit.close()

            os.system("rm out/res/masslimit_"+str(mass)+"_" + str(statlevel) + "_0_" + sShapes + ".txt")


        print "we look the bkg only"
        os.system("tar -xvf out/res_B/masslimit_1000.0_" + str(statlevel) + "_0_" + sShapes + ".txt.tar.gz")
        f_fit=file("out/res/masslimit_1000.0_" + str(statlevel) + "_0_" + sShapes + ".txt")
        print "we look inside the fit output"
        for line in f_fit.readlines():
            if "nbkg=" in line:
                normB=float(line.split("=")[1].split("+")[0])
                print "---------------------------- nbkg B ", float(line.split("=")[1].split("+")[0])
            if "p1=" in line:
                p1B=float(line.split("=")[1].split("+")[0])
                print "---------------------------- p1 B ", float(line.split("=")[1].split("+")[0])
            if "p2=" in line:
                p2B=float(line.split("=")[1].split("+")[0])
                print "---------------------------- p2 B ", float(line.split("=")[1].split("+")[0])
            if "p3=" in line:
                p3B=float(line.split("=")[1].split("+")[0])
                print "---------------------------- p3 B ", float(line.split("=")[1].split("+")[0])
        
        f_fit.close()

        os.system("rm out/res/masslimit_1000.0_" + str(statlevel) + "_0_" + sShapes + ".txt")


        canvas = TCanvas("canvas","canvas",0,0,500,500)
        canvas.Divide(2,2)

        bkg=TF1("bkg", "[0]*TMath::Power((1-x/[4]),[1])/TMath::Power(x/[4],[2]+[3]*log(x/[4]))")
        bkg.SetParameters(1, p1B, p2B, p3B, 7000.0);
        normB = normB/bkg.Integral(890, 4300)
        print "normB = ",normB

        bkg0=TF1("bkg0", "[0]*TMath::Power((1-x/[4]),[1])/TMath::Power(x/[4],[2]+[3]*log(x/[4]))")

        bkg_Rate0=TF1("bkg_Rate0", "[0]*pow((1-x/[4]),[1])/pow(x/[4],[2]+[3]*log(x/[4]))/([5]*pow((1-x/[4]),[6])/pow(x/[4],[7]+[8]*log(x/[4])))", 890., 4300.)
        
 
        PLOTTER=TH1F("PLOTTER", "PLOTTER", 1, 890., 4300.)
        PLOTTER.SetMaximum(1.2);
        PLOTTER.SetMinimum(0.75);
        for j in range(0,4):
            canvas.cd(j+1)

            if j < 3:
                pTitle=sShapes + " Res Masses from "+str((j+1)*1000)+" to "+str((j+1)*1000+900)+"; M; S+B/B       "
                PLOTTER.SetTitle(pTitle)
                PLOTTER.DrawCopy()
                for i in range(0,10):
                    bkg0.SetParameters(1, p1[j*10+i], p2[j*10+i], p3[j*10+i], 7000.); 
                    normBS=norm[j*10+i]/bkg0.Integral(890, 4300)
                    bkg_Rate0.SetParameters(normBS, p1[j*10+i], p2[j*10+i], p3[j*10+i], 7000., normB, p1B, p2B, p3B); 
                    bkg_Rate0.SetLineColor(i+1)
                    if i==9:
                        bkg_Rate0.SetLineColor(11)
                    bkg_Rate0.SetLineStyle(i+1)
                    bkg_Rate0.DrawCopy("SAME")
            else:
                pTitle=sShapes + "Res Masses from "+str((j+1)*1000)+" to "+str((j+1)*1000+300)+"; M; S+B/B       "
                PLOTTER.SetTitle(pTitle)
                PLOTTER.DrawCopy()
                for i in range(0,4):
                    bkg0.SetParameters(1, p1[j*10+i], p2[j*10+i], p3[j*10+i], 7000.); 
                    normBS=norm[j*10+i]/bkg0.Integral(890, 4300)
                    bkg_Rate0.SetParameters(normBS, p1[j*10+i], p2[j*10+i], p3[j*10+i], 7000., normB, p1B, p2B, p3B); 
                    bkg_Rate0.SetLineColor(i+1)
                    if i==9:
                        bkg_Rate0.SetLineColor(11)
                    bkg_Rate0.SetLineStyle(i+1)
                    bkg_Rate0.DrawCopy("SAME") 
   
        

        canvas.SaveAs(prefix + '_MassFits_' + sShapes + '_' + str(statlevel)+'.png')
        canvas.SaveAs(prefix + '_MassFits_' + sShapes + '_' + str(statlevel)+'.eps')
        canvas.SaveAs(prefix + '_MassFits_' + sShapes + '_' + str(statlevel)+'.pdf')       
##         h = TH1F("limit","",len(masses),masses[0]-(masses[1]-masses[0])/2.0,masses[-1]+(masses[-1]-masses[-2])/2.0)

##         for i in range(len(masses)):
##             h.SetBinContent(i+1,limits[i])
        
    
##         h.Draw("")

##         canvas.Update()


## #        legend.Draw()

##         fileRoot = TFile(prefix + '_limit_' + sShapes + '_'+str(statlevel)+'.root', "RECREATE")
##         h.Write()
##         fileRoot.Close()
## #        canvas.SaveAs(prefix + '_limit_' + sShapes + '_' + str(statlevel)+'.root')
##         canvas.SaveAs(prefix + '_limit_' + sShapes + '_' + str(statlevel)+'.png')
##         canvas.SaveAs(prefix + '_limit_' + sShapes + '_' + str(statlevel)+'.eps')
## #        os.system("display "+prefix + '_limit_'+ sShapes + '_' + str(statlevel)+'.png&')


    

