#!/usr/bin/env python

import ROOT as rt

def getBinContent(histo, x, y):
    
    xaxis = histo.GetXaxis()
    yaxis = histo.GetYaxis()
    
    xbin = xaxis.FindBin(x)
    ybin = yaxis.FindBin(y)
    
    bin = histo.GetBin(xbin,ybin)
    
    return histo.GetBinContent(bin)
    
def setBinContent(histo, x, y, val):
    
    xaxis = histo.GetXaxis()
    yaxis = histo.GetYaxis()
    
    xbin = xaxis.FindBin(x)
    ybin = yaxis.FindBin(y)
    
    bin = histo.GetBin(xbin,ybin)
    
    histo.SetBinContent(bin, val)
    

if __name__ == '__main__':
    
    model = 'T1tttt'

    #pkl = "SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_290612-BPOG-ByPoint.pkl"
    pkl = "SMS-T1tttt_Mgluino-450to1200_mLSP-50to800_7TeV-Pythia6Z-Summer11-PU_START42_V11_FSIM-v2-wreece_290612-BPOG-ByPoint.pkl"
    import pickle
    norms = pickle.load(file(pkl))    
    
    fitregion = "((((( (MR >= 500.000000) && (MR < 800.000000) ) && ( (RSQ >= 0.030000) && (RSQ < 0.090000) )) || (( (MR >= 500.000000) && (MR < 650.000000) ) && ( (RSQ >= 0.090000) && (RSQ < 0.200000) ))) || (( (MR >= 500.000000) && (MR < 600.000000) ) && ( (RSQ >= 0.200000) && (RSQ < 0.300000) ))) || (( (MR >= 500.000000) && (MR < 550.000000) ) && ( (RSQ >= 0.300000) && (RSQ < 0.500000) ))) || (( (MR >= 800.000000) && (MR < 3000.000000) ) && ( (RSQ >= 0.030000) && (RSQ < 0.037500) ))"

    binningHistos = rt.TFile.Open("/afs/cern.ch/user/w/wreece/work/LimitSetting/RazorMultiJet2011/sms-binning.root")
    binning = binningHistos.Get("wHisto_BJet_0")
    
    sigTrees = rt.TFile.Open(pkl.replace('.pkl', '.root'))

    result = {}

    for point in norms:
        

        tree = sigTrees.Get('RMRTree_%d_%d' % point)

        #nominal
        sigHist = binning.Clone('sig_%d_%d' % point)
        tree.Project('sig_%d_%d' % point, "RSQ:MR", 'LEP_W*W_EFF*(!%s)' % fitregion)
        c = sigHist.Integral()

        sigHistISR = binning.Clone('sig_%d_%d_isr' % point)
        tree.Project('sig_%d_%d_isr' % point, "RSQ:MR", 'ISR_W*LEP_W*W_EFF*(!%s)' % fitregion)
        i = sigHistISR.Integral()
        
        diff = abs(c-i)/c
        result[point] = diff
        print point , result[point],diff < 0.5
    
    pickle.dump(result,file('ISRWeights_%s_ByPoint.pkl' % model,'wb'))
    print result    
        
        
