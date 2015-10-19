import ROOT
import random


class trainMVAIso:

    def __init__(self,nametags,tree=None,noAuto=False,cut="1"):
        self.name = '_'.join(nametags)
        self.nametags = nametags
        self.fOut = ROOT.TFile(self.name+".root","recreate")
        self.factory = ROOT.TMVA.Factory(self.name,self.fOut,"!V:!Color:Transformations=I")
        self.knownVars={}
        self.cut = "(%s)"%cut
        if not noAuto:
            if not tree: raise RuntimeError, "You must pass a tree to use the automatic training behaviour"
            self.setTree(tree)
            self.autoAddVariables()
            self.train()

    def setTree(self,tree,treetype="SB",w=1.0):
        if 'S' in treetype: self.factory.AddSignalTree(tree,w)
        if 'B' in treetype: self.factory.AddBackgroundTree(tree,w)

    def setWeight(self,wexpr):
        self.factory.SetWeightExpression(wexpr)

    def defineVariable(self,name,expr,vartype='D'):
        self.knownVars[name]=(expr,vartype)

    def addVariable(self,name):
        self.factory.AddVariable("%s := %s"%(name,self.knownVars[name][0]),self.knownVars[name][1])

    def autoAddVariables(self,):
        self.setWeight("genWeight")
        self.addCut("LepGood_pt>20")
        self.addCut("(LepGood_miniRelIso<0.4)")
        if 'mu' in self.nametags: self.addCut("abs(LepGood_pdgId) == 13 && LepGood_mediumMuonId == 1")
        if 'el' in self.nametags: self.addCut("abs(LepGood_pdgId) == 11")
        if '2015' in self.nametags:
            self.defineVariable("LepGood_miniRelIso","min(LepGood_miniRelIso,1.0)")
            self.defineVariable("LepGood_jetPtRatio","min(LepGood_jetPtRatio,1.5)")
            self.defineVariable("LepGood_jetPtRel","min(LepGood_jetPtRel, 30)")
            self.addVariable("LepGood_miniRelIso")
            self.addVariable("LepGood_jetPtRatio")
            self.addVariable("LepGood_jetPtRel")
        if '2015v2' in self.nametags:
            self.defineVariable("LepGood_miniRelIso","min(LepGood_miniRelIso,1.0)")
            self.defineVariable("LepGood_jetPtRatio_LepAwareJECv2","min(LepGood_jetPtRatio_LepAwareJECv2,1.5)")
            self.defineVariable("LepGood_jetPtRelv2","min(LepGood_jetPtRelv2, 30)")
            self.addVariable("LepGood_miniRelIso")
            self.addVariable("LepGood_jetPtRatio_LepAwareJECv2")
            self.addVariable("LepGood_jetPtRelv2")

    def joincuts(self,cut1,cut2): return "(%s) && (%s)"%(cut1,cut2)
    def addCut(self,string): self.cut = self.joincuts(self.cut,string)

    def train(self,isSigCut="LepGood_mcMatchId!=0", isBkgCut="LepGood_mcMatchId==0", trainOpt="GoodvsBad"):
        if trainOpt=="GoodvsBad":
            self.factory.PrepareTrainingAndTestTree(ROOT.TCut(self.joincuts(self.cut,isSigCut)),ROOT.TCut(self.joincuts(self.cut,isBkgCut)),'' if 'Boosted' not in self.nametags else 'nTrain_Signal=10000:nTrain_Background=50000:nTest_Signal=10000:nTest_Background=50000')
        else: raise RuntimeError, 'Unknown training requested'
        BDTGopt = "!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:MaxDepth=8:CreateMVAPdfs";
        self.factory.BookMethod(ROOT.TMVA.Types.kBDT, "BDTG", BDTGopt)
        self.factory.TrainAllMethods();
        self.factory.TestAllMethods();
        self.factory.EvaluateAllMethods();
        self.fOut.Close()



if __name__ == "__main__":

    dataset = "TT_pow"

    f = ROOT.TFile("/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/%s/treeProducerSusyMultilepton/tree.root"%dataset)
    t = f.tree
    t.AddFriend("sf/t","/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/1_lepAwareJECShifts_v1/evVarFriend_%s.root"%dataset)

    import sys
    if sys.argv[1]=="1": trainMVAIso(['2015'  , 'mu'],tree=t)
    if sys.argv[1]=="2": trainMVAIso(['2015'  , 'el'],tree=t)
    if sys.argv[1]=="3": trainMVAIso(['2015v2', 'mu'],tree=t)
    if sys.argv[1]=="4": trainMVAIso(['2015v2', 'el'],tree=t)

