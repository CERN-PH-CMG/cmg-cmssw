import ROOT
import random
from math import ceil

h_temp = None

def evalEff(tree,expr,cut,maxN):
    global h_temp
    h_temp.Reset()
    tree.Draw("(%s)>>dummy"%expr, str(cut), "GOFF", int(maxN))
    return h_temp.GetBinContent(2)/(h_temp.GetBinContent(1)+h_temp.GetBinContent(2))

def optimize_Eff_ND(sigTree, bkgTree, plotexpr, targetEff, params, scut="1", bcut="1", algo="RandomSampling", stopThr = 1e-3, maxN=1e9, maxS=1e9, scanPointsND=1000):

    # plotexpr: like "LepGood_miniRelIso<0.2"
    # targetEff: number, like 0.9
    # params: list of (name,min_val,max_val)
    # scut: like "LepGood_mcMatchId!=0"
    # bcut: like "LepGood_mcMatchId==0"
    # algo: "RandomSampling" or "Scan"
    # stopThr: stop after the relative improvement in background efficiency reaches this value
    # maxN: number of events to read in the tree
    # maxS: max number of samplings in RandomSampling algo
    # scanPointsND: points in the n-dim. space to sample in Scan algo

    nparams = len(params)
    step = 0
    vals = []
    minBkgEff = 1.0
    minBkgEff_vals = vals
    minBkgEff_sigEff = 0
    maxsteps = 0
    if algo=="RandomSampling": maxsteps = int(maxS)
    elif algo=="Scan":
        points1d = ceil(pow(scanPointsND,1.0/nparams))
        npoints = pow(points1d,nparams)
        xstep = [0]*nparams
        xstepwidth = [(params[i][2]-params[i][1])/points1d for i in xrange(nparams)]
        maxsteps = int(npoints)
    else: raise RuntimeError, 'Unknown algo'
    algo_ = 0 if algo=="RandomSampling" else 1
    reports = maxsteps/100
    while step<maxsteps:
        step = step+1
        if algo_==1:
            count = step
            for i in xrange(nparams):                
                xstep[i] = count%points1d
                count/=points1d
        vals = [random.uniform(params[i][1],params[i][2]) for i in xrange(nparams)] if algo_==0 else [params[i][1]+xstepwidth[i]*(xstep[i]+0.5) for i in xrange(nparams)]
        expr = plotexpr
        for i in xrange(nparams):
            expr=expr.replace(params[i][0],str(vals[i]))
        sigEff = evalEff(sigTree,expr,scut,maxN)
        bkgEff = evalEff(bkgTree,expr,bcut,maxN)
        if (step%reports==0): print 'Ongoing: step',step,vals
        if (sigEff<targetEff): continue
        if (bkgEff<minBkgEff):
            if (bkgEff/minBkgEff>1-stopThr):
                print 'Will stop here!'
                step=maxsteps+1
            minBkgEff = bkgEff
            minBkgEff_vals = vals
            minBkgEff_sigEff = sigEff
            print 'Found better WP!','Cut values:',[("%s: %f"%(params[i][0],minBkgEff_vals[i])) for i in xrange(nparams)],'Signal efficiency:',minBkgEff_sigEff,'Background efficiency:',minBkgEff
    print 'Best WP:','Cut values:',[("%s: %f"%(params[i][0],minBkgEff_vals[i])) for i in xrange(nparams)],'Signal efficiency:',minBkgEff_sigEff,'Background efficiency:',minBkgEff



if __name__=="__main__":
    
   dataset = "TT_pow"

   f = ROOT.TFile("/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/%s/treeProducerSusyMultilepton/tree.root"%dataset)
   t = f.tree
   t.AddFriend("sf/t","/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/2_multiIsoMVAtraining2015_v1/evVarFriend_%s.root"%dataset)

   h_temp=ROOT.TH1F("dummy","dummy",2,-0.5,1.5)

   params = [("miniIsoCut",0.05,0.2),("ptRatioCut",0.6,0.8),("ptRelCut",5,15)]

   targetEff = 0.9

   import sys
   if sys.argv[1]=="1":
       print 'training 2015'
       mIso = "LepGood_miniRelIso<miniIsoCut && (LepGood_jetPtRatio>ptRatioCut || LepGood_jetPtRel>ptRelCut)"
       optimize_Eff_ND(t,t,mIso,targetEff,params,"genWeight*(LepGood_mcMatchId!=0)","genWeight*(LepGood_mcMatchId==0)",algo="Scan",scanPointsND=1000)
   elif sys.argv[1]=="2":
       print 'training 2015v2'
       mIso = "LepGood_miniRelIso<miniIsoCut && (LepGood_jetPtRatio_LepAwareJECv2>ptRatioCut || LepGood_jetPtRelv2>ptRelCut)"
       optimize_Eff_ND(t,t,mIso,targetEff,params,"genWeight*(LepGood_mcMatchId!=0)","genWeight*(LepGood_mcMatchId==0)",algo="Scan",scanPointsND=1000)
