import ROOT

from CMGTools.VVResonances.plotting.TreePlotter import TreePlotter
from CMGTools.VVResonances.plotting.StackPlotter import StackPlotter
 
ROOT.gROOT.ProcessLine('.x tdrstyle.C')



w = TreePlotter('samples/W.root','tree')
w.addCorrectionFactor('xsec','xsec',0.0,'lnN')
w.addCorrectionFactor('mc','l_mcPt>0',0.0,'lnN')
w.setFillProperties(0,ROOT.kWhite)
w.setLineProperties(1,ROOT.kOrange+10,3)

qcd = TreePlotter('samples/W.root','tree')
qcd.addCorrectionFactor('xsec','xsec',0.0,'lnN')
qcd.addCorrectionFactor('mc','l_mcPt==0',0.0,'lnN')
qcd.setFillProperties(3003,ROOT.kBlue)
qcd.setLineProperties(1,ROOT.kBlue,3)




#Stack
lStack = StackPlotter()
lStack.addPlotter(qcd,"QCD","Non prompt","background")
lStack.addPlotter(w,"Prompt_leptons","Prompt","signal")
