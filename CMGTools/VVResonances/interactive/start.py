import ROOT

from CMGTools.VVResonances.plotting.TreePlotter import TreePlotter
from CMGTools.VVResonances.plotting.MergedPlotter import MergedPlotter
from CMGTools.VVResonances.plotting.StackPlotter import StackPlotter
 

#create the W+jets plotters
wjPlotters=[]

for sample in ["WJetsToLNu_HT1200to2500","WJetsToLNu_HT2500toInf","WJetsToLNu_HT400to600","WJetsToLNu_HT600to800","WJetsToLNu_HT800to1200",'WJetsToLNu_HT100to200','WJetsToLNu_HT200to400']:
    wjPlotters.append(TreePlotter('samples/'+sample+'.root','tree'))
    wjPlotters[-1].setupFromFile('samples/'+sample+'.pck')
    wjPlotters[-1].addCorrectionFactor('xsec','tree')
    wjPlotters[-1].addCorrectionFactor('genWeight','tree')
    wjPlotters[-1].addCorrectionFactor('puWeight','tree')


WJets = MergedPlotter(wjPlotters)

tt=TreePlotter('samples/TTJets.root','tree')
tt.setupFromFile('samples/TTJets.pck')
tt.addCorrectionFactor('xsec','tree')
tt.addCorrectionFactor('genWeight','tree')
tt.addCorrectionFactor('puWeight','tree')




#create the Z+jets plotters

zPlotters=[]
for sample in ['DYJetsToLL_M50_HT100to200','DYJetsToLL_M50_HT200to400','DYJetsToLL_M50_HT400to600','DYJetsToLL_M50_HT600toInf']:  
    zPlotters.append(TreePlotter('samples/'+sample+'.root','tree'))
    zPlotters[-1].setupFromFile('samples/'+sample+'.pck')
    zPlotters[-1].addCorrectionFactor('xsec','tree')
    zPlotters[-1].addCorrectionFactor('genWeight','tree')
    zPlotters[-1].addCorrectionFactor('puWeight','tree')
ZJets = MergedPlotter(zPlotters)



gPlotters=[]
for sample in ['GJets_HT100to200','GJets_HT200to400','GJets_HT400to600','GJets_HT40to100','GJets_HT600toInf']:
    gPlotters.append(TreePlotter('samples/'+sample+'.root','tree'))
    gPlotters[-1].setupFromFile('samples/'+sample+'.pck')
    gPlotters[-1].addCorrectionFactor('xsec','tree')
    gPlotters[-1].addCorrectionFactor('genWeight','tree')
    gPlotters[-1].addCorrectionFactor('puWeight','tree')
GJets = MergedPlotter(gPlotters)



qcdPlotters=[]
for sample in ["QCD_HT1000to1500","QCD_HT1500to2000","QCD_HT2000toInf","QCD_HT200to300","QCD_HT300to500","QCD_HT500to700","QCD_HT700to1000"]:
    qcdPlotters.append(TreePlotter('samples/'+sample+'.root','tree'))
    qcdPlotters[-1].setupFromFile('samples/'+sample+'.pck')
    qcdPlotters[-1].addCorrectionFactor('xsec','tree')
    qcdPlotters[-1].addCorrectionFactor('genWeight','tree')
    qcdPlotters[-1].addCorrectionFactor('puWeight','tree')

QCD = MergedPlotter(qcdPlotters)






singleMuA = TreePlotter('samples/SingleMuon_Run2015D_05Oct.root','tree')
singleMuB = TreePlotter('samples/SingleMuon_Run2015D_v4.root','tree')
singleEleA = TreePlotter('samples/SingleElectron_Run2015D_05Oct.root','tree')
singleEleB = TreePlotter('samples/SingleElectron_Run2015D_v4.root','tree')
jetA = TreePlotter('samples/JetHT_Run2015D_05Oct.root','tree')
jetB = TreePlotter('samples/JetHT_Run2015D_v4.root','tree')
metA = TreePlotter('samples/MET_Run2015D_05Oct.root','tree')
metB = TreePlotter('samples/MET_Run2015D_v4.root','tree')

dataPlotters=[singleMuA,singleMuB,singleEleA,singleEleB,jetA,jetB]


dataEMUPlotters=[singleMuA,singleMuB,singleEleA,singleEleB]
dataJetPlotters=[jetA,jetB]
dataMETPlotters=[metA,metB]

#Fill properties
WJets.setFillProperties(1001,ROOT.kAzure-9)
tt.setFillProperties(1001,ROOT.kGreen-5)
ZJets.setFillProperties(1001,ROOT.kAzure+5)
GJets.setFillProperties(1001,ROOT.kYellow)
QCD.setFillProperties(1001,ROOT.kGray)



dataEMU = MergedPlotter(dataEMUPlotters)
dataJet = MergedPlotter(dataJetPlotters)


#Stack for lnu+J
lnujjStack = StackPlotter()
lnujjStack.addPlotter(QCD,"QCD","QCD multijet","background")
lnujjStack.addPlotter(GJets,"GJets","#gamma+Jets","background")
lnujjStack.addPlotter(tt,"tt","t#bar{t}","background")
lnujjStack.addPlotter(WJets,"WJets","W+Jets","background")
lnujjStack.addPlotter(dataEMU,"data_obs","Data","data")

#Stack for ll+J

lljjStack = StackPlotter()
lljjStack.addPlotter(tt,"tt","t#bar{t}","background")
lljjStack.addPlotter(ZJets,"ZJets","Z+Jets","background")
lljjStack.addPlotter(dataEMU,"data_obs","Data","data")

jjStack = StackPlotter()
jjStack.addPlotter(QCD,"QCD","QCD multijet","background")
jjStack.addPlotter(tt,"tt","t#bar{t}","background")
jjStack.addPlotter(dataJet,"data_obs","Data","data")


#ZNuNU not processed yet
#jjnunuStack = StackPlotter()
#jjnunuStack.addPlotter(QCD,"QCD","QCD multijet","background")
#jjnunuStack.addPlotter(tt,"tt","t#bar{t}","background")
#jjnunuStack.addPlotter(dataMET,"data_obs","Data","data")



