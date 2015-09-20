import ROOT

from CMGTools.VVResonances.plotting.TreePlotter import TreePlotter
from CMGTools.VVResonances.plotting.MergedPlotter import MergedPlotter
from CMGTools.VVResonances.plotting.StackPlotter import StackPlotter
 

#create the W+jets plotters
wjPlotters=[]


WJets = TreePlotter('samples/WJetsToLNu_50ns.root','tree')
WJets.setupFromFile('samples/WJetsToLNu_50ns.pck')
WJets.addCorrectionFactor('xsec','xsec',0.0,'lnN')
WJets.addCorrectionFactor('genWeight','genWeight',0.0,'lnN')
WJets.setFillProperties(1001,ROOT.kAzure-9)



TTJets = TreePlotter('samples/TTJets_50ns.root','tree')
TTJets.setupFromFile('samples/TTJets_50ns.pck')
TTJets.addCorrectionFactor('xsec','xsec',0.0,'lnN')
TTJets.addCorrectionFactor('genWeight','genWeight',0.0,'lnN')
TTJets.setFillProperties(1001,ROOT.kGreen-5)



qcdPlotters=[]

for sample in [
"QCD_Pt1000to1400_50ns","QCD_Pt120to170_50ns","QCD_Pt1400to1800_50ns","QCD_Pt170to300_50ns","QCD_Pt1800to2400_50ns","QCD_Pt2400to3200_50ns","QCD_Pt300to470_50ns","QCD_Pt3200_50ns","QCD_Pt470to600_50ns","QCD_Pt600to800_50ns","QCD_Pt800to1000_50ns","QCD_Pt80to120_50ns"
    ]:
    qcdPlotters.append(TreePlotter('samples/'+sample+'.root','tree'))
    qcdPlotters[-1].setupFromFile('samples/'+sample+'.pck')
    qcdPlotters[-1].addCorrectionFactor('xsec','xsec',0.0,'lnN')
    qcdPlotters[-1].addCorrectionFactor('genWeight','genWeight',0.0,'lnN')

QCD = MergedPlotter(qcdPlotters)
QCD.setFillProperties(1001,ROOT.kAzure-9)

#QCD.setFillProperties(1001,ROOT.kGreen-5)




singleMu = TreePlotter('samples/SingleMuon_Run2015B.root','tree')
singleMu.addCorrectionFactor('hlt','(HLT_ISOMU_50ns==1||(HLT_MU==1))',0.0,'lnN')

singleEle = TreePlotter('samples/SingleElectron_Run2015B.root','tree')
singleEle.addCorrectionFactor('hlt','((HLT_ISOELE_50ns==1||(HLT_ELE==1))&&(!(HLT_ISOMU_50ns==1||(HLT_MU==1))))',0.0,'lnN')

jetHT = TreePlotter('samples/JetHT_Run2015B.root','tree')
#jetHT.addCorrectionFactor('hlt','((HLT_JJ==1||HLT_HT==1)&&(!(HLT_ISOMU_50ns==1||(HLT_MU==1)||(HLT_ISOELE_50ns==1||(HLT_ELE==1)))))',0.0,'lnN')


#dataPlotters=[singleMu,singleEle,jetHT]
dataPlotters=[jetHT]


data = MergedPlotter(dataPlotters)


#WWZ = TreePlotter('samples/WprimeToWZToWhadZhad_narrow_2000.pck','tree')
#WWZ.setupFromFile('samples/WprimeToWZToWhadZhad_narrow_2000.pck')
#WWZ.setFillProperties(0,ROOT.kWhite)
#WWZ.setLineProperties(1,ROOT.kOrange+12,3)


#RSGWWLNuQQ..addCorrectionFactor('xsec',0.001,0.0,'lnN')


#Stack
lnujStack = StackPlotter()
lnujStack.addPlotter(WJets,"W+jets","W+Jets","background")
lnujStack.addPlotter(TTJets,"TTJets","t#bar{t}+jets","background")
#vvStack.addPlotter(RSGWWLNuQQ,"RSG2000","RSGWW #rightarrow l#nu QQ","signal")
lnujStack.addPlotter(data,"data_obs","Data","data")

jjStack = StackPlotter()
#lnujStack.addPlotter(WJets,"W+jets","W+Jets","background")
jjStack.addPlotter(QCD,"QCD","QCD","background")
#vvStack.addPlotter(RSGWWLNuQQ,"RSG2000","RSGWW #rightarrow l#nu QQ","signal")
jjStack.addPlotter(data,"data_obs","Data","data")
