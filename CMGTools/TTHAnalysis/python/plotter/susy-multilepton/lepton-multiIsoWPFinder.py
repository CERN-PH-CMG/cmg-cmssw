import ROOT
import random, os, pprint
from math import ceil,log,exp

h_temp = None

def evalEff(tree,expr,cut,maxN):
    if type(tree) == list:
        return sum(evalEff(t,expr,cut,maxN) for t in tree)/len(tree)
    else:
        global h_temp
        h_temp.Reset()
        tree.Draw("(%s)>>dummy"%expr, str(cut), "GOFF", int(maxN))
        return h_temp.GetBinContent(2)/(h_temp.GetBinContent(1)+h_temp.GetBinContent(2))

def get_Eff_ND(sigTree, bkgTree, expr, scut="1", bcut="1", maxN=1e9):
    sigEff = evalEff(sigTree,expr,scut,maxN)
    bkgEff = evalEff(bkgTree,expr,bcut,maxN)
    print 'Signal efficiency:',sigEff,'Background efficiency:',bkgEff
    return (sigEff,bkgEff)

def optimize_Eff_ND(sigTree, bkgTree, plotexpr, targetEff, params, scut="1", bcut="1", algo="RandomSampling", stopThr = 1e-3, maxN=1e9, maxSteps=1000):

    # plotexpr: like "LepGood_miniRelIso<0.2"
    # targetEff: number, like 0.9
    # params: list of (name,min_val,max_val)
    # scut: like "LepGood_mcMatchId!=0"
    # bcut: like "LepGood_mcMatchId==0"
    # algo: "RandomSampling" or "Scan"
    # stopThr: stop after the relative improvement in background efficiency reaches this value
    # maxN: number of events to read in the tree
    # maxSteps: number of points to scan (in case of random sampling, it can stop earlier if accuracy is reached)

    nparams = len(params)
    step = 0
    vals = []
    minBkgEff = 1.0
    minBkgEff_vals = vals
    minBkgEff_sigEff = 0
    maxsteps = 0
    def crop(x,x1,x2): return min(max(x,x1),x2)
    if algo=="Scan":
        points1d = ceil(pow(maxSteps,1.0/nparams))
        npoints = pow(points1d,nparams)
        xstep = [0]*nparams
        xstepwidth = [(params[i][2]-params[i][1])/points1d for i in xrange(nparams)]
        maxSteps = int(npoints)
    elif algo=="RandomSampling":
        pass
    elif algo=="SGRandom":
        centers = [ (0.5*(x1+x2)) for (n,x1,x2,op) in params ]
        sigmas  = [ (0.5*(x2-x1)) for (n,x1,x2,op) in params ]
        maxN /= 5; stepthr = maxSteps/5
        sigmaexp = log(30)/maxSteps
    else: raise RuntimeError, 'Unknown algo'
    reports = maxSteps/20
    while step<maxSteps:
        step = step+1
        if algo == "Scan":
            count = step
            for i in xrange(nparams):                
                xstep[i] = count%points1d
                count/=points1d
            vals = [params[i][1]+xstepwidth[i]*(xstep[i]+0.5) for i in xrange(nparams)]
        elif algo == "RandomSampling":
            vals = [random.uniform(params[i][1],params[i][2]) for i in xrange(nparams)]
        elif algo == "SGRandom":
            if step <= stepthr:
                vals = [random.uniform(params[i][1],params[i][2]) for i in xrange(nparams)]
                if step == stepthr: maxN *= 5
            else:
                sigscale = exp(-step*sigmaexp)
                #if (step%reports==0): print 'Ongoing: sigscale',sigscale,'centers:',centers
                vals = [crop(random.gauss(centers[i],sigmas[i]*sigscale),params[i][1],params[i][2]) for i in xrange(nparams)]
        expr = plotexpr
        for i in xrange(nparams):
            expr=expr.replace(params[i][0],str(vals[i]))
        sigEff = evalEff(sigTree,expr,scut,maxN)
        bkgEff = evalEff(bkgTree,expr,bcut,maxN)
        #if (step%reports==0): print 'Ongoing: step',step,vals
        if (sigEff<targetEff): continue
        if (bkgEff<minBkgEff):
            if algo == "SGRandom":
                centers = vals
            if (bkgEff/minBkgEff>1-stopThr) and "Random" in algo:
                print 'Will stop here!'
                step=maxSteps+1
            minBkgEff = bkgEff
            minBkgEff_vals = vals
            minBkgEff_sigEff = sigEff
            print 'Found better WP!','Cut values:',[("%s: %f"%(params[i][0],minBkgEff_vals[i])) for i in xrange(nparams)],'Signal efficiency:',minBkgEff_sigEff,'Background efficiency:',minBkgEff,'Iter:',step
    print 'Best WP:','Cut values:',[("%s: %f"%(params[i][0],minBkgEff_vals[i])) for i in xrange(nparams)],'Signal efficiency:',minBkgEff_sigEff,'Background efficiency:',minBkgEff
    vals = minBkgEff_vals 
    expr = plotexpr
    for i in xrange(nparams):
        expr=expr.replace(params[i][0],str(vals[i]))
    return (expr, vals, minBkgEff_sigEff, minBkgEff)



if __name__=="__main__":
   ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/fakeRate.cc+" % os.environ['CMSSW_BASE'])
    
   dataset = "TT_pow"
   dataset2 = "T1tttt_mGo1500_mChi100"

   f = ROOT.TFile("/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/%s/treeProducerSusyMultilepton/tree.root"%dataset)
   t = f.tree
   if 1:
       f2 = ROOT.TFile("/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/%s/treeProducerSusyMultilepton/tree.root"%dataset2)
       t2 = [t,f2.tree] # average
   else:
       t2 = t
   #t.AddFriend("sf/t","/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12/1_lepAwareJECShifts_v1/evVarFriend_%s.root"%dataset)

   h_temp=ROOT.TH1F("dummy","dummy",2,-0.5,1.5)

   #cut = "abs(LepGood_pdgId) == 11 && LepGood_pt > 25 && LepGood_mediumMuonId >= 1"
   cut = "abs(LepGood_pdgId) == 13 && LepGood_pt > 20 && LepGood_mediumMuonId >= 1"
   scut, bcut = "(%s) && (LepGood_mcMatchId!=0)" % cut, "(%s) && (LepGood_mcMatchId==0)" % cut
   params  = [["miniIsoCut",0.05,0.25],["ptRatioCut",0.5,0.95],["ptRelCut",3,20]]
   params2 = [["miniIsoCut",0.05,0.20,'<'],["ptRatioCut",0.6,0.9,'>'],["ptRelCut",3,12,'>']]

   oldWPs = []
   if True:
       for (L,I) in reversed([("L", 1), ("M", 2), ("T",3), ("VT",4)]):
           (effs,effb) = get_Eff_ND(t2,t,"multiIso_singleWP(LepGood_miniRelIso,LepGood_jetPtRatio,LepGood_jetPtRel,%d)" % I,scut,bcut)
           oldWPs.append( (L, I, effs, effb) )
       print oldWPs

   newWPs = []
   for targetEff in [ effs-0.001 for (L,I,effs,effb) in oldWPs ]:
       #print 'training 2015'
       #mIso = "LepGood_miniRelIso<miniIsoCut && (LepGood_jetPtRatio>ptRatioCut || LepGood_jetPtRel>ptRelCut)"
       #optimize_Eff_ND(t,t,mIso,targetEff,params,scut,bcut,algo="Scan",scanPointsND=1000)

       print 'training 2015v2, target efficiency ',targetEff,"params:",params2
       mIso = "LepGood_miniRelIso<miniIsoCut && (LepGood_jetPtRatio_LepAwareJECv2>ptRatioCut || LepGood_jetPtRelv2>ptRelCut)"
       #optimize_Eff_ND(t,t,mIso,targetEff,params2,scut,bcut,maxN=1e4,maxSteps=1000) 
       #optimize_Eff_ND(t,t,mIso,targetEff,params2,scut,bcut,maxN=1e4,maxSteps=1000,algo="RandomSampling") 
       #optimize_Eff_ND(t,t,mIso,targetEff,params2,scut,bcut,maxN=1e4,maxSteps=1000,algo="RandomSampling") 
       (cut, bestparams, effs, effb) = optimize_Eff_ND(t2,t,mIso,targetEff,params2,scut,bcut,maxN=4e4,maxSteps=400,algo="SGRandom") 
       for i,b in enumerate(bestparams):
          if params2[i][3] == '<':
              params2[i][1] = b # increase minimum
          else:
              params2[i][2] = b # increase maximum
       newWPs.append ( (cut, bestparams, effs, effb) )
   pprint.pprint(oldWPs)
   pprint.pprint(newWPs)
