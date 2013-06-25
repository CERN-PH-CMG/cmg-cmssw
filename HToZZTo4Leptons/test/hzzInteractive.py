import ROOT
from CMGTools.HToZZTo4Leptons.macros.plotterSetup import *


periods=['7TeV','8TeV']
#periods=['7TeV']



mass=126

zzPlotters=[]
fakePlotters=[]
dataPlotters=[]
ggPlotters=[]
qqPlotters=[]
whPlotters=[]
zhPlotters=[]
tthPlotters=[]

for period in periods:
    zzPlotters = zzPlotters+plotterSetup.getQQToZZPlotter(period)['deps']\
                 +plotterSetup.getGluGluToZZPlotter(period)['deps']
    fakePlotters = fakePlotters+plotterSetup.getFakePlotter(period,
                                                            'HLoose_Z2_Charge!=0&&abs(HLoose_Z2_leg1_SIP3D)<4&&abs(HLoose_Z2_leg2_SIP3D)<4',
                                                            None,
                                                            1)['deps']
    dataPlotters = dataPlotters+plotterSetup.getDataPlotter(period)['deps']

    ggPlotters = ggPlotters+plotterSetup.getHiggsPlotter(['GluGluToH'],period,mass)['deps']
    qqPlotters = qqPlotters+plotterSetup.getHiggsPlotter(['VBF'],period,mass)['deps']
    whPlotters = whPlotters+plotterSetup.getHiggsPlotter(['WH'],period,mass)['deps']
    zhPlotters = zhPlotters+plotterSetup.getHiggsPlotter(['ZH'],period,mass)['deps']
    tthPlotters = tthPlotters+plotterSetup.getHiggsPlotter(['TTbarH'],period,mass)['deps']




#create Merged Plotters for the stack
zz = MergedPlotter(zzPlotters)
zz.setFillProperties(1001,ROOT.kAzure-9)


higgsRF = MergedPlotter(ggPlotters+tthPlotters)
higgsRV = MergedPlotter(qqPlotters+whPlotters+zhPlotters)
higgsGGH = MergedPlotter(ggPlotters)
higgsQQH = MergedPlotter(qqPlotters)
higgsVH = MergedPlotter(whPlotters+zhPlotters)
higgsttH = MergedPlotter(tthPlotters)
higgs = MergedPlotter(ggPlotters+qqPlotters+whPlotters+zhPlotters+tthPlotters)



higgsRF.setFillProperties(0,ROOT.kWhite)
higgsRF.setLineProperties(1,ROOT.kOrange+10,3)
higgsRV.setFillProperties(1001,ROOT.kOrange+10)
higgsRV.setLineProperties(1,ROOT.kOrange+10,3)

higgsGGH.setFillProperties(0,ROOT.kWhite)
higgsGGH.setLineProperties(1,ROOT.kRed,3)

higgsQQH.setFillProperties(3004,ROOT.kOrange+10)
higgsQQH.setLineProperties(1,ROOT.kOrange+10,3)


higgsVH.setFillProperties(1001,ROOT.kOrange+10)
higgsVH.setLineProperties(1,ROOT.kOrange+10,3)


higgsttH.setFillProperties(0,ROOT.kWhite)
higgsttH.setLineProperties(1,ROOT.kYellow,3)

higgs.setFillProperties(0,ROOT.kWhite)
higgs.setLineProperties(1,ROOT.kOrange+10,3)



#for plotter in fakePlotters:
#    plotter.setFactorizedRegion('&&H_NJets==2&&H_DEtaAbs>3&&H_MJJ>300')
#    plotter.applyFactorization(3.17e-3)

#    plotter.setFactorizedRegion('&&H_NJets==2&&H_MVAFisher>0')
#    plotter.applyFactorization(3.72e-3)
#    plotter.setFactorizedRegion('&&H_NJets>=2')
#    plotter.applyFactorization(0.3)
#    plotter.setFactorizedRegion('&&H_lepton_Pt>10')
#    plotter.applyFactorization(4.48e-3)

fakes = MergedPlotter(fakePlotters)
fakes.setFillProperties(1001,ROOT.kGreen-5)

data = MergedPlotter(dataPlotters)

#Stack
zzStack = StackPlotter()
zzStack.addPlotter(fakes,"FAKES","Z+X","background")
zzStack.addPlotter(zz,"qq/gg ZZ","ZZ , Z#gamma^{*}","background")
zzStack.addPlotter(higgsRV,"RVH","qqH+VH (126 GeV)","signal")
zzStack.addPlotter(higgsRF,"RFH","ggH+ttH (126 GeV)","signal")
#zzStack.addPlotter(higgsGGH,"RV1","ggH (126 GeV)","signal")
#zzStack.addPlotter(higgsQQH,"RV2","qqH (126 GeV)","signal")
#zzStack.addPlotter(higgsVH,"RV3","VH (126 GeV)","signal")
#zzStack.addPlotter(higgs,"RV3","m_{H} = 126 GeV","signal")

zzStack.addPlotter(data,"data_obs","Data","data")



