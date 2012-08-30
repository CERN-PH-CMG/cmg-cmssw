import os
import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.Style import formatPad,Style
from QCDEstimation_diTau import *
from DYEstimate_diTau import *
from SaveHistograms_diTau import *
from PrepareDictionaries_diTau import *
from array import array
import math
import copy

'''This provides a lineno() function to make it easy to grab the line
number that we're on.

Danny Yoo (dyoo@hkn.eecs.berkeley.edu)
'''

import inspect

def lineno():
    '''Returns the current line number in our program.'''
    return inspect.currentframe().f_back.f_lineno

########## rootlogon5 for loose iso M          ##### don't forget to change weight at line ~401!
########## rootlogon6 for loose iso LL4        ##### don't forget to change weight at line ~401!
########## rootlogon7 for loose iso and dR<3.5 ##### don't forget to change weight at line ~401!
########## rootlogon8 for loose iso M          ##### don't forget to change weight at line ~401!
########## rootlogon9 for loose iso M   and Tau pT calculated post dR+nVert RW       ##### don't forget to change weight at line ~401!
########## rootlogon10 for loose iso LL4 POST BUG! RW                                ##### don't forget to change weight at line ~401!
from os import path
if path.exists('../proto/rootlogon11.C'):
    gROOT.Macro('../proto/rootlogon11.C')  # Run ROOT logon script

run2012=True

import ROOT
ROOT.gROOT.SetBatch(True)


#def weightQCD(nVert) : 
#    vertSig[]

def integralAndError(hist):
    error=array('d',[0])
    integral=hist.IntegralAndError(1, hist.GetNbinsX(),error)
    return integral, error[0]

def prepareComponents(dir, config):
    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
    return selComps, weights

NBINS = 100
XMIN  = 0
XMAX  = 200

if __name__ == '__main__':

    import copy
    from optparse import OptionParser
    from CMGTools.RootTools.RootInit import *
    gROOT.SetStyle("Plain")

    parser = OptionParser()
    parser.usage = '''
    %prog <anaDir> <cfgFile>

    cfgFile: analysis configuration file, see CMGTools.H2TauTau.macros.MultiLoop
    anaDir: analysis directory containing all components, see CMGTools.H2TauTau.macros.MultiLoop.

    hist: histogram you want to plot
    '''
    parser.add_option("-B", "--box", 
                      dest="box", 
                      help="box. Default is Inclusive",
                      default='Inclusive')
    parser.add_option("-M", "--mtregion", 
                      dest="mtregion", 
                      help="mT region. Default is LowMT",
                      default='LowMT')
    parser.add_option("-H", "--histlist", 
                      dest="histlist", 
                      help="histogram list",
                      default=None)
    parser.add_option("-C", "--cut", 
                      dest="cut", 
                      help="cut to apply in TTree::Draw",
                      default='1')
    parser.add_option("-G", "--histgroup", 
                      dest="histgroup", 
                      help="histogram group",
                      default=None)
    parser.add_option("-R", "--rebin", 
                      dest="rebin", 
                      help="rebinning factor",
                      default=None)
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedd samples.",
                      action="store_true",
                      default=False)
    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)
    #print lineno()

    dataName = 'Data'
    weight='weight'

    anaDir = args[0]
    hists = histogramSet( options )
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
    #print [co for co in selComps]

    # SWITCH ON/OFF embedded samples
    options.embed = True
    crossCheckZee = True

    if run2012:
      # 1.616/fb
      selComps['data_Run2012A_PromptReco_v1'].intLumi = 920.039
      weights[ 'data_Run2012A_PromptReco_v1'].intLumi = 920.039
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 696.0
      weights[ 'data_Run2012B_PromptReco_v1'].intLumi = 696.0
      # 2.4/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 1500.
      weights[ 'data_Run2012B_PromptReco_v1'].intLumi = 1500.
      # 2.86/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 1940.
      weights[ 'data_Run2012B_PromptReco_v1'].intLumi = 1940.
      # 3.93/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 3010.
      weights[ 'data_Run2012B_PromptReco_v1'].intLumi = 3010.
      # 5.1/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 4180.
      weights[ 'data_Run2012B_PromptReco_v1'].intLumi = 4180.
    else:
      # 1fb
      selComps['data_Run2011A_05Aug2011_v1'].intLumi = 31.9
      weights['data_Run2011A_05Aug2011_v1'].intLumi = 31.9
      #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
      #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.
      
      # IsoTau20
      #selComps['data_Run2011A_PromptReco_v4'].intLumi = 31.5
      #weights['data_Run2011A_PromptReco_v4'].intLumi = 31.5

      # IsoTau25
      #selComps['data_Run2011A_May10ReReco_v1'].intLumi = 0.+31.9
      #weights['data_Run2011A_May10ReReco_v1'].intLumi = 0.+31.9
      #selComps['data_Run2011A_PromptReco_v4'].intLumi = 139.+31.5
      #weights['data_Run2011A_PromptReco_v4'].intLumi = 139.+31.5
      #selComps['data_Run2011A_05Aug2011_v1'].intLumi = 0.
      #weights['data_Run2011A_05Aug2011_v1'].intLumi = 0.
      #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
      #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.

      # IsoTau35
      #selComps['data_Run2011A_May10ReReco_v1'].intLumi = 0.
      #weights['data_Run2011A_May10ReReco_v1'].intLumi = 0.
      #selComps['data_Run2011A_PromptReco_v4'].intLumi = 759.2
      #weights['data_Run2011A_PromptReco_v4'].intLumi = 759.2
      #selComps['data_Run2011A_05Aug2011_v1'].intLumi = 31.9
      #weights['data_Run2011A_05Aug2011_v1'].intLumi = 31.9
      #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
      #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.
    #print lineno()

    selCompsDataMass, weightsDataMass = componentsWithData(selComps,weights)
    #selCompsMCMass, weightsMCMass     = componentsWithOutData(selComps,weights)
    selCompsNoSignal, weightsNoSignal = componentsWithOutSignal(selComps,weights)
    #print lineno()

    if options.embed:
        embeddedScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight)

    if crossCheckZee:
        zeeScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, options.embed)

    tauScale='0.03'
    shiftedMet = 'sqrt(pow(mex+'+tauScale+'*l1Px+'+tauScale+'*l2Px,2)+pow(mey+'+tauScale+'*l1Py+'+tauScale+'*l2Py,2))' 

    baseline           =  'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && abs(jet1Eta) < 3.0'
    baselineSS         =  'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge!=0 && abs(jet1Eta) < 3.0'
    baseline           += ' && l2MVAEle>0.5'
    baselineSS         += ' && l2MVAEle>0.5'
    l1Pt35l2Pt35       =  ' && l1Pt>35 && l2Pt>35'
    
    l1Pt40l2Pt40_down  =  ' && l1Pt>40*0.97 && l2Pt>40*0.97'
    l1Pt40l2Pt40_up    =  ' && l1Pt>40*1.03 && l2Pt>40*1.03'
    l1Pt40l2Pt40       =  ' && l1Pt>40 && l2Pt>40'
    
    l1Pt45l2Pt40       =  ' && l1Pt>45 && l2Pt>40'
    l1Pt45l2Pt45       =  ' && l1Pt>45 && l2Pt>45'
    l1Pt50l2Pt50       =  ' && l1Pt>50 && l2Pt>50'
    isolationL         =  ' && (l1RawMVAIso>0.795 || l2RawMVAIso>0.795)'
    isolationMLL       =  ' && ((l1MedMVAIso>0.5 && l2RawMVAIso>0.5) || (l1RawMVAIso>0.5 && l2MedMVAIso>0.5))'

    isolationMNM       =  ' && ((l1MedMVAIso>0.5 && l2MedMVAIso<0.5) || (l1MedMVAIso<0.5 && l2MedMVAIso>0.5))'

    isolationM         =  ' && (l1MedMVAIso>0.5 || l2MedMVAIso>0.5)'
    isolationLL        =  ' && l1RawMVAIso>0.795 && l2RawMVAIso>0.795'
    isolationLL2       =  ' && l1RawMVAIso>0.7 && l2RawMVAIso>0.7'
    isolationLL3       =  ' && l1RawMVAIso>0.3 && l2RawMVAIso>0.3'
    isolationLL4       =  ' && l1RawMVAIso>0.5 && l2RawMVAIso>0.5'
    isolationLL1       =  ' && l1RawMVAIso>0.1 && l2RawMVAIso>0.1'
    isolationLL4old    =  ' && l1LooIso>0.5 && l2LooIso>0.5'
    isolationML        =  ' && ((l1MedMVAIso>0.5 && l2RawMVAIso>0.795) || (l1RawMVAIso>0.795 && l2MedMVAIso>0.5))'
    isolationMM        =  ' && l1MedMVAIso>0.5 && l2MedMVAIso>0.5'
    isolationMMold     =  ' && l1MedIso>0.5 && l2MedIso>0.5'
    isolationTM        =  ' && ((l1MedMVAIso>0.5 && l2TigMVAIso>0.5) || (l1TigMVAIso>0.5 && l2MedMVAIso>0.5))'
    isolationTT        =  ' && l1TigMVAIso>0.5 && l2TigMVAIso>0.5'
    isolationTTold     =  ' && l1TigIso>0.5 && l2TigIso>0.5'
    Jet0               =  ' && jet1Pt<50'
    BOOSTED            =  ' && jet1Pt>50'
    VBF                =  ' &&  jet1Pt>50 && jet2Pt>30 && abs(jet1Eta - jet2Eta)>2.5 && (jet1Eta*jet2Eta)<0 && mjj>250 && nCentralJets==0'
    VBFlooser          =  ' &&  jet1Pt>50 && jet2Pt>30 && abs(jet1Eta - jet2Eta)>2.5 && (jet1Eta*jet2Eta)<0 && mjj>150 && nCentralJets==0'
    VBFtight           =  ' &&  jet1Pt>50 && jet2Pt>30 && abs(jet1Eta - jet2Eta)>4.0 && (jet1Eta*jet2Eta)<0 && mjj>400 && nCentralJets==0 '
    VBFmedium          =  ' &&  jet1Pt>50 && jet2Pt>30 && abs(jet1Eta - jet2Eta)>2.5 && (jet1Eta*jet2Eta)<0 && mjj>500 && nCentralJets==0 '
    NOVBF              =  ' && (jet1Pt<50 || jet2Pt<30 || abs(jet1Eta - jet2Eta)<2.5 || (jet1Eta*jet2Eta)>0 || mjj<250 || nCentralJets >0)'
    NOVBFmedium        =  ' && (jet1Pt<50 || jet2Pt<30 || abs(jet1Eta - jet2Eta)<2.5 || (jet1Eta*jet2Eta)>0 || mjj<500 || nCentralJets >0)'
    NOVBFtight         =  ' && (jet1Pt<50 || jet2Pt<30 || abs(jet1Eta - jet2Eta)<4.0 || (jet1Eta*jet2Eta)>0 || mjj<400 || nCentralJets >0)'
    #print lineno()
    
    cuts=[
#### New default for HCP ####

        ("CMS_2012_5_fb_forHCP_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
    
#### Andreas ####    
        #("CMS_2012_5_fb_l40_mm_dRtt20_qcd5M_dRRW_loose_BOOSTED" , baseline + l1Pt40l2Pt40 + BOOSTED + NOVBF , ' && dRtt<200.0'   , isolationMM , 5 ),
        #("CMS_2012_5_fb_qcd5LL4_forRW_loose_BOOSTED"     , baselineSS + BOOSTED, ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb_qcd5M_Closure_loose_BOOSTED"     , baselineSS + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_IsoTest_forRW_no_loose_BOOSTED"  , baselineSS  + BOOSTED, ' && dRtt<100.' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_forRW_no_dR3_loose_BOOSTED"  , baselineSS  + BOOSTED, ' && dRtt<3.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_forRW_no_dR2_loose_BOOSTED"  , baselineSS  + BOOSTED, ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_forRW_no_iso04_loose_BOOSTED"  , baselineSS  + BOOSTED, ' && dRtt<100.' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_forRW_no_iso06_loose_BOOSTED"  , baselineSS  + BOOSTED, ' && dRtt<100.' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_dR3_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_dR2_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_iso04_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_iso06_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_dR3_VBF"      , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_dR2_VBF"      , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_iso04_VBF"    , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_no_iso06_VBF"    , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_test_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_test_VBF"      , baselineSS  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_test_VBFwider"      , baselineSS  + VBFlooser             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_test_VBFiso06"      , baselineSS  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_sep_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_loose_sep_VBF"      , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_nodR_loose_sep_BOOSTED"  , baseline  + BOOSTED + NOVBF , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_IsoTest_SS_loose_BOOSTED"  , baseline  + BOOSTED + NOVBF, ' && dRtt<2.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb_qcd5LL4_loose_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_VBF"      , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_isoChooseDiLepton_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_isoChooseDiLepton_forRW_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_isoChooseDiLepton_relaxed_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_isoChooseDiLepton_relaxed_forRW_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_isoChoosDiLepton_relaxe_SS_DR_FINE_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_isoChoosDiLepton_relaxe_SS_NODR_FINE_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_relaxed_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_relaxed_forRW_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_QCD_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationLL4 , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_QCD_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationLL4 , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_higgs_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_oldLIso_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_btag_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_newfit_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_plateauup_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_plateaudown_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_higgs_FINE_VBF"      , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_QCD_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF +' && weight<1e-4' , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_loose_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationLL4 , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_looseRef_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_stdMedium_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMMold , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_stdTight_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationTTold , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_oldloose_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationLL4old , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_default_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_oldDefault_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMMold , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_firstTight_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_scaleDY2_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_tightE_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF + ' && l1MVAEle>0.5' , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_btag_ldR2_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt>2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_btag_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_newfit_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_oldLIso_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_oneProng_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode<3 && l2DecayMode<3' , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_onePiProng_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode==0 && l2DecayMode==0' , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_twoPiProng_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode==1 && l2DecayMode==1' , ' && dRtt<200.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_threeProng_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode>=3 && l2DecayMode>=3' , ' && dRtt<200.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_forRW_Closure_FINE_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<200.0' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOWEIGHT_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOWEIGHT_VBF"      , baseline  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_SS_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_WIDE_SS_VBF"      , baselineSS  + VBF             , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NODR_NOWEIGHT_BOOSTED"  , baseline  + BOOSTED + NOVBF , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_oneProng_FINE_NODR_NOWEIGHT_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode<3 && l2DecayMode<3' , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_onePiProng_FINE_NODR_NOWEIGHT_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode==0 && l2DecayMode==0' , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_twoPiProng_FINE_NODR_NOWEIGHT_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode==1 && l2DecayMode==1' , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_threeProng_FINE_NODR_NOWEIGHT_BOOSTED"  , baseline  + BOOSTED + NOVBF+' && l1DecayMode>=3 && l2DecayMode>=3' , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NODR_VBF"      , baseline  + VBF             , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_SS_NODR_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_ptDRWeight9_SS_NODR_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , '' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_firstRelaxed_SS_BOOSTED"  , baselineSS  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_WIDE_SS_NODR_VBF"      , baselineSS  + VBF             , '' , isolationMM , 5 ),

        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_TRIGUP_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_TRIGDOWN_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_TRIGSLOPEUP_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_TRIGSLOPEDOWN_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_TRIGPLATEAUUP_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_5_fb__qcd5LL4_PREAPP_FINE_NOEMBED_TRIGPLATEAUUP_BOOSTED"  , baseline  + BOOSTED + NOVBF , ' && dRtt<2.0' , isolationMM , 5 ),
	  ]
    
    for prefix,cut,antiqcdcut,isocut,qcdEstimate in cuts:
     bareCut = cut
     if str(prefix).find("VBF") > 0 :
       rebin = 2
     else :
       rebin = 1	
     for var, nx, xmin, xmax in [
        #('svfitMass'        ,int(300/rebin), 0 , 300    ),
        ('svfitMass'        ,int(30/rebin), 0 , 300    ), 
        #('svfitMass'        ,int(20/rebin), 0 , 300    ), 
        #('svfitMass'        ,int(15/rebin), 0 , 300    ), 
        #('svfitMass*1.03'   ,int(300/rebin), 0 , 300    ),
        ('svfitMass*1.03'   ,int(30/rebin), 0 , 300    ),
        #('svfitMass*0.97'   ,int(300/rebin), 0 , 300    ),
        ('svfitMass*0.97'   ,int(30/rebin), 0 , 300    ),
        ('visMass'          ,int(30/rebin), 0 , 300    ),
        #('visMass*1.03'     ,int(30/rebin), 0 , 300    ),
        #('visMass*0.97'     ,int(30/rebin), 0 , 300    ),
        ('dRtt'             ,int(15/rebin), 0 , 5      ),
        ('jet1Pt'           ,int(20/rebin), 0 , 600    ),
        ('nVert'            ,int(25/rebin), 0 , 50     ),
        ('l1Pt'             ,int(15/rebin), 0 , 300    ),   # was 75 bins
        ('l2Pt'             ,int(20/rebin), 0 , 200    ),   # was 75 bins
        ('jet2Pt'           ,int(25/rebin), 0 , 500    ),
        ('met'              ,int(40/rebin), 0 , 200    ),
        ('l1Eta'            ,int(20/rebin), -3, 3      ),   # was 40 bins
        ('l2Eta'            ,int(20/rebin), -3, 3      ),   # was 40 bins
        ('jet1Eta'          ,int(20/rebin), -5, 5      ),
        ('jet2Eta'          ,int(20/rebin), -5, 5      ),
        ('mjj'              ,int(30/rebin), 0 , 800    ),
        ('nJets'            ,10           , 0 , 10     ),
        ('dPhitt'           ,int(40/rebin), 0 , 3.15   ),
        ('mt'               ,int(40/rebin), 0 , 200    ),
        ('pThiggs'          ,int(40/rebin), 0 , 300    ),
        ('diTauPt'          ,int(40/rebin), 0 , 300    ),
        ('l1jetPt'          ,15           , 0 , 300    ),
        ('l2jetPt'          ,20           , 0 , 200    ),
        ('l1jetMass'        ,30           , 0 , 100    ),
        ('l2jetMass'        ,30           , 0 , 100    ),
        ('l1jetWidth'       ,50           , 0 , 0.05   ),
        ('l2jetWidth'       ,50           , 0 , 0.05   ),
        ('l1jetBtag'        ,50           , -1 , 1     ),
        ('l2jetBtag'        ,50           , -1 , 1     ),
        #('l1RawMVAIso'      ,int(30/rebin) , 0 , 1.00001),
        #('l1DecayMode'      ,12           , 0 , 12     ),
        #('l2DecayMode'      ,12           , 0 , 12     ),
        #('mttj'             ,int(40/rebin), 0 , 1000   ),
        #('diTauCharge'      ,7            , -3, 3      ),
        #('l1LooIso'         ,2            , 0,  2      ),
        #('l2LooIso'         ,2            , 0,  2      ),
        #('l1MedIso'         ,2            , 0,  2      ),
        #('l2MedIso'         ,2            , 0,  2      ),
        #('l1TigIso'         ,2            , 0,  2      ),
        #('l2TigIso'         ,2            , 0,  2      ),
        #('l1RawMVAIso'      ,100          , 0 , 1.00001),
        #('l1MedMVAIso'      ,2            , 0 , 2      ),
        #('l1TigMVAIso'      ,2            , 0 , 2      ),
        #('l1LooseEle'       ,2            , 0 , 2      ),
        #('l1MVAEle'         ,2            , 0 , 2      ),
        #('l1LooseMu'        ,2            , 0 , 2      ),
        #('l2RawMVAIso'      ,100          , 0 , 1.00001),
        #('l2MedMVAIso'      ,2            , 0 , 2      ),
        #('l2TigMVAIso'      ,2            , 0 , 2      ),
        #('l2LooseEle'       ,2            , 0 , 2      ),
        #('l2MVAEle'         ,2            , 0 , 2      ),
        #('l2LooseMu'        ,2            , 0 , 2      ),
      ]:
      
      cut     = bareCut
      print 'I\'ve stripped the old pt cut\n', cut
      if   ( var == 'svfitMass*1.03' or var == 'visMass*1.03' ):
        cut += l1Pt40l2Pt40_up
      elif ( var == 'svfitMass*0.97' or var == 'visMass*0.97' ):
        cut += l1Pt40l2Pt40_down 
      else :
        cut += l1Pt40l2Pt40
      
      print 'I\'m using this cut string\n',cut+isocut+antiqcdcut    
      
      prefix1 = os.getcwd()+"/"+prefix+"/diTau_2012_"
      dirList = os.listdir(os.getcwd())
      exists = False
      for fname in dirList:
        if fname == prefix :
          exists = True
      if not(exists) :
        os.mkdir(os.getcwd()+"/"+prefix)
      #print lineno()
      
      #if var in ["met","jet1Pt","jet2Pt","l1Pt","l2Pt"]:
      #  log=True
      #else:
      #  log=False

      log=False


      #looseisocut = isolationM + " && !(1 "+isocut+")"
      #semilooseisocut=isolationM+" && !(1 "+isocut+")"

#       if prefix.find('VBF') > 0 :
#         looseisocut = isolationM+" && !(1 "+isocut+")"
#       else :
#         looseisocut = isolationLL4+" && !(1 "+isocut+")"


      #########################################################
      ############## change here the loose iso ################
      #########################################################
      #looseisocut = isolationM + " && !(1 "+isocut+")"
      looseisocut = isolationLL4+" && !(1 "+isocut+")"
      #looseisocut = isolationLL4old +" && !(1 "+isocut+")"
      #looseisocut = ""
      #looseisocut = ' && l1RawMVAIso>0.5 && l2MedMVAIso>0.5'
      #########################################################
      
      semilooseisocut=isolationLL3+" && !(1 "+isocut+")"
      
      if qcdEstimate==0:
        # MET based QCD estimation
        lowcontrolcut=" && met<10"
        averagecontrolcut=" && met<20"
        highcontrolcut=" && met>10 && met<20"
      elif qcdEstimate==1:
        # dR based QCD estimation
        lowcontrolcut=" && dRtt>3.0"
        averagecontrolcut=" && dRtt>2.0"
        highcontrolcut=" && dRtt>2.0 && dRtt<3.0"
      elif qcdEstimate==2:
        # Loose based QCD estimation
        pass
      elif qcdEstimate==3:
        # OS/SS from loose. tight/loose from dR SS
        lowcontrolcut=" && dRtt>3.0"
        averagecontrolcut=" && dRtt>2.0"
        highcontrolcut=" && dRtt>2.0 && dRtt<3.0"
      elif qcdEstimate==6:
        # Loose based QCD estimation
        # shape from SS tight, scale from OS loose / SS loose
        pass

      cutSS=cut.replace("diTauCharge==0","diTauCharge!=0")
      #print lineno()

      plotVarDataSS = H2TauTauDataMC(var,                           \
                                     anaDir,                        \
                                     selCompsNoSignal,              \
                                     weightsNoSignal,               \
     			                     nx,                            \
     			                     xmin,                          \
     			                     xmax,                          \
     			                     cut = cutSS+isocut+antiqcdcut, \
#                                             weight=weight+"*weightQCD_dR(dRtt)*weightQCD_nVert(nVert)",\
     			                     weight=weight,                 \
     			                     embed=options.embed)
      #print lineno()
      
      plotVarDataLooseIsoSS = H2TauTauDataMC(var,                               \
                                             anaDir,                            \
                                             selCompsNoSignal,                  \
                                             weightsNoSignal,                   \
     			                             nx,                                \
     			                             xmin,                              \
     			                             xmax,                              \
     			                             cut = cutSS+looseisocut+antiqcdcut,\
     			                             weight=weight,                     \
     			                             embed=options.embed)

      #### NO NEED TO APPLY WEIGHTS HERE UNLESS YOU WANT TO COMPUTE CORRELATIONS (and uncomment the lines at the bottom of this file)
      plotVarDataLooseIsoSSWeighted = H2TauTauDataMC(var,                       \
                                             anaDir,                            \
                                             selCompsNoSignal,                  \
                                             weightsNoSignal,                   \
     			                             nx,                                \
     			                             xmin,                              \
     			                             xmax,                              \
     			                             cut = cutSS+looseisocut+antiqcdcut,\
                                             #weight = weight+"*weightQCD_dR(dRtt)",    \
                                             #weight = weight+"*weightQCD_l1Pt(l1Pt)",  \
                                             #weight = weight+"*weightQCD_l2Pt(l2Pt)",  \
                                             #weight = weight+"*weightQCD_nVert(nVert)",\
                                             #weight = weight+"*weightQCD_dR(dRtt)*weightQCD_nVert(nVert)",\
                                             #weight = weight+"*weightQCD_dR(dRtt)*weightQCD_l2Pt(l2Pt)",\
                                             #weight = weight+"*weightQCD_dR(dRtt)*weightQCD_l2Pt(l2Pt)*weightQCD_l1Pt(l1Pt)*weightQCD_nVert(nVert)",\
                                             #weight = weight+"*weightQCD_dR(dRtt)*weightQCD_l2Pt(l2Pt)*weightQCD_nVert(nVert)",\
     			                             weight=weight,                     \
     			                             embed=options.embed)
      #print lineno()
      
      # for the QCD estimation MC contributions are subtracted in the consistent regio NO NEED TO APPLY THE WEIGHT ON THEM
      plotVarDataLooseIsoOSMC = H2TauTauDataMC(var,                            \
                                             anaDir,                           \
                                             selCompsNoSignal,                 \
                                             weightsNoSignal,                  \
                                             nx,                               \
                                             xmin,                             \
                                             xmax,                             \
                                             cut = cut+looseisocut+antiqcdcut, \
                                             weight = weight,                  \
                                             embed=options.embed)

      plotVarDataLooseIsoOS = H2TauTauDataMC(var,                              \
                                             anaDir,                           \
                                             selCompsNoSignal,                 \
                                             weightsNoSignal,                  \
                                             nx,                               \
                                             xmin,                             \
                                             xmax,                             \
                                             cut = cut+looseisocut+antiqcdcut, \
                                             #weight=weight+"*weightQCD_nVert(nVert)",\
                                             #weight=weight+"*weightQCD_dR(dRtt)*weightQCD_nVert(nVert)",\
                                             #weight=weight+"*weightQCD_l1Pt(l1Pt)*weightQCD_l2Pt(l2Pt)*weightQCD_nVert(nVert)*weightQCD_jet1Pt(jet1Pt)",\
                                             #weight=weight+"*weightQCD_l1Pt(l1Pt)*weightQCD_l2Pt(l2Pt)*weightQCD_nVert(nVert)",\
                                             #weight=weight+"*weightQCD_dR(dRtt)*weightQCD_l1Pt(l1Pt)*weightQCD_l2Pt(l2Pt)*weightQCD_nVert(nVert)",\
                                             #weight=weight+"*weightQCD_dR(dRtt)*weightQCD_l1Pt(l1Pt)*weightQCD_l2Pt(l2Pt)*weightQCD_nVert(nVert)*weightQCD_jet1Pt(jet1Pt)",\
                                             #weight=weight+"*weightQCD_dR(dRtt)*weightQCD_nVert(nVert)",\
                                             weight=weight,                    \
                                             embed=options.embed)
      #print lineno()
      
      WJets_looseisocut = isolationMNM + " && !(1 "+isocut+")"
      if prefix.find('VBF')     > 0 : weightForWJets = 'weightWJets2011_nVert(nVert)'
      if prefix.find('BOOSTED') > 0 : weightForWJets = 'weightW3Jets2011_nVert(nVert)'
      plotVarDataLooseIsoOS_WJets = H2TauTauDataMC(var,                                    \
                                                   anaDir,                                 \
                                                   selCompsNoSignal,                       \
                                                   weightsNoSignal,                        \
     			                                   nx,                                     \
     			                                   xmin,                                   \
     			                                   xmax,                                   \
     			                                   cut = cut+WJets_looseisocut+antiqcdcut, \
     			                                   weight=weight+"*"+weightForWJets,       \
     			                                   embed=options.embed)

      WJets_looseisocut = isolationMNM + " && !(1 "+isocut+")"
      if prefix.find('VBF')     > 0 : weightForWJets = 'weightWJets2011_nVert(nVert)'
      if prefix.find('BOOSTED') > 0 : weightForWJets = 'weightW3Jets2011_nVert(nVert)'
      plotVarDataLooseIsoSS_WJets = H2TauTauDataMC(var,                                      \
                                                   anaDir,                                   \
                                                   selCompsNoSignal,                         \
                                                   weightsNoSignal,                          \
     			                                   nx,                                       \
     			                                   xmin,                                     \
     			                                   xmax,                                     \
     			                                   cut = cutSS+WJets_looseisocut+antiqcdcut, \
     			                                   weight=weight+"*"+weightForWJets,         \
     			                                   embed=options.embed)

      
      #######################################################
      ################# PLOTTING DATA/MC ####################
      #######################################################
      yields = False
      if ( var == "svfitMass" or var == "svfitMass*1.03" or var == "svfitMass*0.97" ):
        massesRange = [110,115,120,125,130,135,140,145]
        #massesRange = [125]
        print 'I\'m plotting mass distribution for masses in [110,115,120,125,130,135,140,145] GeV'
        if var == "svfitMass" :
          yields = True
      else :
        massesRange = [125]
        print 'I\'m plotting distribution just for mass 125 GeV'
        
      for mIndex in massesRange :    
        plotVarDataOS = H2TauTauDataMC(var, anaDir, selCompsDataMass[mIndex], weightsDataMass[mIndex],
     			    nx, xmin, xmax,
     			    ####### for eta dependent trigger Turn On
     			    #cut = cut+isocut+antiqcdcut, weight=weight+'*weightZTT_Eta(l1Eta)*weightZTT_Eta(l2Eta)',
     			    cut = cut+isocut+antiqcdcut, weight=weight,
     			    embed=options.embed)

        #print lineno()

        #####################################################
        ###            WJets / W3Jets Estimation          ###
        #####################################################
        
        if prefix.find('VBF')     > 0 : 
          WJets_sample    = 'W3Jets'
          WJets_ToBePut_0 = ['WJets','WJets_Fakes']
          if   prefix.find('loose') or prefix.find('medium')  > 0 : 
            scaleFromMuTau  = 8.56  # for VBF loose
          elif prefix.find('tight')   > 0 : 
            scaleFromMuTau  = 9.96  # for VBF tight
        
        if prefix.find('BOOSTED') > 0 : 
          WJets_sample    = 'WJets'
          WJets_ToBePut_0 = ['W3Jets']
          scaleFromMuTau  = 0.64

          WJetsFakesShape   = copy.deepcopy(plotVarDataLooseIsoOS_WJets.Hist("WJets_Fakes"))
          WJetsFakesShapeSS = copy.deepcopy(plotVarDataLooseIsoSS_WJets.Hist("WJets_Fakes"))
        
          if plotVarDataLooseIsoOS.Hist(WJets_sample).Integral() == 0 : WJetsFakesScale = 0
          else :                                                        WJetsFakesScale = scaleFromMuTau * plotVarDataOS.Hist("WJets_Fakes").Integral()/plotVarDataLooseIsoOS_WJets.Hist("WJets_Fakes").Integral()

          if plotVarDataLooseIsoSS.Hist(WJets_sample).Integral() == 0 : WJetsFakesScaleSS = 0
          else :                                                        WJetsFakesScaleSS = scaleFromMuTau * plotVarDataSS.Hist("WJets_Fakes").Integral()/plotVarDataLooseIsoSS_WJets.Hist("WJets_Fakes").Integral()

          plotVarDataOS.Hist("WJets_Fakes").obj      = WJetsFakesShape.obj
          plotVarDataOS.Hist("WJets_Fakes").weighted = WJetsFakesShape.weighted
          plotVarDataOS.Hist("WJets_Fakes").Scale(WJetsFakesScale)

          plotVarDataSS.Hist("WJets_Fakes").obj      = WJetsFakesShapeSS.obj
          plotVarDataSS.Hist("WJets_Fakes").weighted = WJetsFakesShapeSS.weighted
          plotVarDataSS.Hist("WJets_Fakes").Scale(WJetsFakesScaleSS)

        WJetsShape   = copy.deepcopy(plotVarDataLooseIsoOS_WJets.Hist(WJets_sample))
        WJetsShapeSS = copy.deepcopy(plotVarDataLooseIsoSS_WJets.Hist(WJets_sample))
        
        if plotVarDataLooseIsoOS.Hist(WJets_sample).Integral() == 0 : WJetsScale = 0
        else :                                                        WJetsScale = scaleFromMuTau * plotVarDataOS.Hist(WJets_sample).Integral()/plotVarDataLooseIsoOS_WJets.Hist(WJets_sample).Integral()

        if plotVarDataLooseIsoSS.Hist(WJets_sample).Integral() == 0 : WJetsScaleSS = 0
        else :                                                        WJetsScaleSS = scaleFromMuTau * plotVarDataSS.Hist(WJets_sample).Integral()/plotVarDataLooseIsoSS_WJets.Hist(WJets_sample).Integral()
        	
        plotVarDataOS.Hist(WJets_sample).obj       = WJetsShape.obj
        plotVarDataOS.Hist(WJets_sample).weighted  = WJetsShape.weighted
        plotVarDataOS.Hist(WJets_sample).Scale(WJetsScale)

        plotVarDataSS.Hist(WJets_sample).obj       = WJetsShapeSS.obj
        plotVarDataSS.Hist(WJets_sample).weighted  = WJetsShapeSS.weighted
        plotVarDataSS.Hist(WJets_sample).Scale(WJetsScaleSS)

        for zero in WJets_ToBePut_0 :
          plotVarDataOS.Hist(zero).Scale(0)

        for zero in WJets_ToBePut_0 :
          plotVarDataSS.Hist(zero).Scale(0)

        #####################################################
        ###                 QCD ESTIMATION                ###
        #####################################################

        if qcdEstimate == 4 or qcdEstimate == 5:			    
         plotVarDataSemiLooseIsoSS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
       			    nx, xmin, xmax,
       			    cut = cutSS+semilooseisocut+antiqcdcut, weight=weight,
       			    embed=options.embed)
         plotVarDataSemiLooseIsoOS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
       			    nx, xmin, xmax,
       			    cut = cut+semilooseisocut+antiqcdcut, weight=weight,
       			    embed=options.embed)
        if qcdEstimate == 0 or qcdEstimate == 1 or qcdEstimate == 3:
         plotVarDataLowControlOS = 0
         plotVarDataLowControlSS = 0
         plotVarDataAverageControlOS = 0
         plotVarDataAverageControlSS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
       			    nx, xmin, xmax,
       			    cut = cutSS+isocut+averagecontrolcut, weight=weight,
       			    embed=options.embed)
         plotVarDataHighControlOS = 0
         plotVarDataHighControlSS = 0
        if qcdEstimate == 0 or qcdEstimate == 1:
         plotVarDataLowControlLooseIsoOS = 0
         plotVarDataAverageControlLooseIsoOS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
       			    nx, xmin, xmax,
       			    cut = cut+looseisocut+averagecontrolcut, weight=weight,
       			    embed=options.embed)
         plotVarDataHighControlLooseIsoOS = 0
        if qcdEstimate == 0 or qcdEstimate == 1 or qcdEstimate == 3:
         plotVarDataLowControlLooseIsoSS = 0
         plotVarDataAverageControlLooseIsoSS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
       			    nx, xmin, xmax,
       			    cut = cutSS+looseisocut+averagecontrolcut, weight=weight,
       			    embed=options.embed)
         plotVarDataHighControlLooseIsoSS = 0
        if qcdEstimate == 0:
         QCDShape, QCDScale = QCDEstimate1(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                         plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS, plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,
                                         plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS, plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS, plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,log)
        if qcdEstimate == 1:
         QCDShape, QCDScale = QCDEstimate1(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                         plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS, plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,
                                         plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS, plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS, plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,log)
        if qcdEstimate == 2:
         QCDShape    , QCDScale    , QCDlooseSS    , QCDtightSS     = QCDEstimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, log)
        if qcdEstimate == 6:
          QCDShape, QCDScale, QCDlooseSS, QCDtightSS = QCDEstimate6(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, log)
        if qcdEstimate == 3:
         QCDShape, QCDScale = QCDEstimate3(prefix,prefix1,xmin,xmax,plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataAverageControlSS, plotVarDataAverageControlLooseIsoSS, log)
        if qcdEstimate == 4:
         QCDShape, QCDScale = QCDEstimate4(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataSemiLooseIsoSS, log)
        if qcdEstimate == 5:
   	 QCDShapeSemi, QCDScaleSemi, QCDlooseSSSemi, QCDtightSSSemi , tightLooseErrSemi = QCDEstimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataSemiLooseIsoOS, plotVarDataSemiLooseIsoOS, plotVarDataSemiLooseIsoSS, log)
   	 QCDShape    , QCDScale    , QCDlooseSS    , QCDtightSS     , tightLooseErr	= QCDEstimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoOSMC, plotVarDataLooseIsoSS, log)
   
   	if False:
   	 plotVarDataLooseIsoSS_sys1 = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
   			      nx, xmin, xmax,
   			      cut = cutSS+looseisocut+antiqcdcut+" && l1Pt>70", weight=weight,
   			      embed=options.embed)
   	 plotVarDataLooseIsoSS_sys2 = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
   			      nx, xmin, xmax,
   			      cut = cutSS+looseisocut+antiqcdcut+" && l2Pt>50", weight=weight,
   			      embed=options.embed)
   	 plotVarDataLooseIsoSS_sys3 = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
   			      nx, xmin, xmax,
   			      cut = cutSS+looseisocut+antiqcdcut+" && jet1Pt>100", weight=weight,
   			      embed=options.embed)
   	 plotVarDataSS_sys1 = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
   			      nx, xmin, xmax,
   			      cut = cutSS+isocut+antiqcdcut+" && l1Pt>70", weight=weight,
   			      embed=options.embed)
   	 plotVarDataSS_sys2 = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
   			      nx, xmin, xmax,
   			      cut = cutSS+isocut+antiqcdcut+" && l2Pt>50", weight=weight,
   			      embed=options.embed)
   	 plotVarDataSS_sys3 = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
   			      nx, xmin, xmax,
   			      cut = cutSS+isocut+antiqcdcut+" && jet1Pt>100", weight=weight,
   			      embed=options.embed)
 	  		      
   	 tightLooseErr,tightLooseErrSys = QCDYieldError(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoSS, 
   			     plotVarDataSS_sys1, plotVarDataLooseIsoSS_sys1,
   			     plotVarDataSS_sys2, plotVarDataLooseIsoSS_sys2,
   			     plotVarDataSS_sys3, plotVarDataLooseIsoSS_sys3,
   	  		     log)
   	else:
   	 # if QCD yield sys is not evaluated inflate by hand
   	 #if prefix.find('BOOSTED') > 0 : 
   	 #    tightLooseErr=0.25
   	 #if prefix.find('VBF') > 0 : 
   	 #    tightLooseErr=0.45
   	 if prefix.find('BOOSTED') > 0 : 
   	     tightLooseErr=0.056
   	 if prefix.find('VBF') > 0 : 
   	     tightLooseErr=0.22
   
        if qcdEstimate==5:
   	 print "QCD yield uncertainty:", tightLooseErr
   	 if prefix.find('BOOSTED') > 0 : 
             f=TFile.Open("BOOSTED_shape.root")
   	 if prefix.find('VBF') > 0 : 
             f=TFile.Open("VBF_shape.root")
         uncup=f.Get("UncDown")
   	 QCDShapeErrorPlot=copy.deepcopy(QCDShape)
   	 QCDShapeErrorPlot.SetStyle( sBlue )
   	 QCDShapeErrorPlot.weighted.Scale(QCDScale)
   	 for b in range(QCDShapeErrorPlot.weighted.GetNbinsX()):
   	     QCDShapeErrorPlot.weighted.SetBinError(b+1,uncup.GetBinContent(uncup.FindBin(QCDShapeErrorPlot.weighted.GetBinCenter(b+1)))*QCDShapeErrorPlot.weighted.GetBinContent(b+1))
   	 QCDShapeErrorPlot.weighted.SetMarkerSize(0)
   
   	 QCDYieldErrorPlot=copy.deepcopy(QCDShape)
   	 QCDYieldErrorPlot.SetStyle( sOrange )
   	 QCDYieldErrorPlot.weighted.Scale(QCDScale)
   	 for b in range(QCDYieldErrorPlot.weighted.GetNbinsX()):
   	     QCDYieldErrorPlot.weighted.SetBinError(b+1,tightLooseErr*QCDYieldErrorPlot.weighted.GetBinContent(b+1))
   	 QCDYieldErrorPlot.weighted.SetMarkerSize(0)
     
         if str(prefix).find("SS") > 0 :
   	   for b in range(QCDYieldErrorPlot.weighted.GetNbinsX()):
   	     QCDYieldErrorPlot.weighted.SetBinError(b+1,math.sqrt(pow(QCDYieldErrorPlot.weighted.GetBinError(b+1),2)+pow(QCDShapeErrorPlot.weighted.GetBinError(b+1),2)))
   	  
   	 QCDShapePlot=copy.deepcopy(QCDShape)
   	 QCDShapePlot.SetStyle( sRedLine )
   	 QCDShapePlot.weighted.Scale(QCDScale)

         if str(prefix).find("SS") > 0 :
   	   for b in range(QCDShapePlot.weighted.GetNbinsX()):
   	      QCDShapePlot.weighted.SetBinError(b+1,math.sqrt(pow(QCDShapePlot.weighted.GetBinError(b+1),2)+pow(QCDShapeErrorPlot.weighted.GetBinError(b+1),2)+pow(QCDYieldErrorPlot.weighted.GetBinError(b+1),2)))
   	  
   	 QCDShapeSemiPlot=copy.deepcopy(QCDShapeSemi)
   	 QCDShapeSemiPlot.SetStyle( sBlueLine )
   	 QCDShapeSemiPlot.weighted.Scale(QCDShapePlot.weighted.Integral()/QCDShapeSemiPlot.weighted.Integral())
   
   	 QCDShapeSSPlot=copy.deepcopy(QCDtightSS)
   	 QCDShapeSSPlot.SetStyle( sGreenLine )
   	 for b in range(QCDShapeErrorPlot.weighted.GetNbinsX()):
           if QCDlooseSSSemi.weighted.GetBinContent(b+1)>0:
   	     QCDShapeSSPlot.weighted.SetBinContent(b+1,QCDShapeSSPlot.weighted.GetBinContent(b+1)*QCDShapeSemi.weighted.GetBinContent(b+1)/QCDlooseSSSemi.weighted.GetBinContent(b+1))
   	     #QCDShapeSSPlot.weighted.SetBinContent(b+1,QCDShapeSemi.weighted.GetBinContent(b+1)/QCDlooseSSSemi.weighted.GetBinContent(b+1))
   	     #QCDShapeSSPlot.weighted.SetBinError(b+1,QCDShapeSemi.weighted.GetBinError(b+1)/QCDlooseSSSemi.weighted.GetBinContent(b+1))
	   else:
   	     QCDShapeSSPlot.weighted.SetBinContent(b+1,0)
	 if QCDShapeSSPlot.weighted.Integral()>0:
             QCDShapeSSPlot.weighted.Scale(QCDShapePlot.weighted.Integral()/QCDShapeSSPlot.weighted.Integral())
   
   	 if True:
   	    print "tight SS"
   	    print "Data:"		     , plotVarDataSS.Hist("Data").Integral()
   	    print "TTJets:"		     , plotVarDataSS.Hist("TTJets").Integral()
   	    print "DYJets:"		     , plotVarDataSS.Hist("DYJets").Integral()#+plotVarDataSS.Hist("DYJets_Photon").Integral()
   	    print "DYJets_Electron:"	     , plotVarDataSS.Hist("DYJets_Electron").Integral()
   	    print "DYJets_Fakes:"	     , plotVarDataSS.Hist("DYJets_Fakes").Integral()
   	    print "WJets:"		     , plotVarDataSS.Hist("WJets").Integral()+plotVarDataSS.Hist("WJets_Fakes").Integral()
     	    print "DiBoson:"		     , plotVarDataSS.Hist("WW").Integral()+plotVarDataSS.Hist("WZ").Integral()+plotVarDataSS.Hist("ZZ").Integral()

         if True:
            print "loose OS"
            print "Data:"		     , plotVarDataLooseIsoOS.Hist("Data").Integral()
            print "TTJets:"		     , plotVarDataLooseIsoOS.Hist("TTJets").Integral()
            print "DYJets:"		     , plotVarDataLooseIsoOS.Hist("DYJets").Integral()#+plotVarDataLooseIsoOS.Hist("DYJets_Photon").Integral()
            print "DYJets_Electron:"	     , plotVarDataLooseIsoOS.Hist("DYJets_Electron").Integral()
            print "DYJets_Fakes:"	     , plotVarDataLooseIsoOS.Hist("DYJets_Fakes").Integral()
            print "WJets:"		     , plotVarDataLooseIsoOS.Hist("WJets").Integral()+plotVarDataLooseIsoOS.Hist("WJets_Fakes").Integral()
            print "DiBoson:"		     , plotVarDataLooseIsoOS.Hist("WW").Integral()+plotVarDataLooseIsoOS.Hist("WZ").Integral()+plotVarDataLooseIsoOS.Hist("ZZ").Integral()
       
         if True:
            print "loose SS"
            print "Data:"		     , plotVarDataLooseIsoSS.Hist("Data").Integral()
            print "TTJets:"		     , plotVarDataLooseIsoSS.Hist("TTJets").Integral()
            print "DYJets:"		     , plotVarDataLooseIsoSS.Hist("DYJets").Integral()#+plotVarDataLooseIsoSS.Hist("DYJets_Photon").Integral()
            print "DYJets_Electron:"	     , plotVarDataLooseIsoSS.Hist("DYJets_Electron").Integral()
            print "DYJets_Fakes:"	     , plotVarDataLooseIsoSS.Hist("DYJets_Fakes").Integral()
            print "WJets:"		     , plotVarDataLooseIsoSS.Hist("WJets").Integral()+plotVarDataLooseIsoSS.Hist("WJets_Fakes").Integral()
            print "DiBoson:"		     , plotVarDataLooseIsoSS.Hist("WW").Integral()+plotVarDataLooseIsoSS.Hist("WZ").Integral()+plotVarDataLooseIsoSS.Hist("ZZ").Integral()

         ymax = max(QCDShapePlot.GetMaximum(),QCDShapeSemiPlot.GetMaximum())*1.5
          
         QCDShapePlot.weighted.Draw("HISTe")
         #QCDShapePlot.weighted.Draw("HIST")
         if log:
             QCDShapePlot.GetYaxis().SetRangeUser(0.1,ymax)
         else:
             QCDShapePlot.GetYaxis().SetRangeUser(0,ymax)
         #QCDShapeSemiPlot.weighted.Draw("HISTeSAME")
         if str(prefix).find("SS") > 0 :
             QCDShapeSSPlot.weighted.Draw("HISTeSAME")
         QCDYieldErrorPlot.weighted.Draw("e2SAME")
	 QCDShapeErrorPlot.weighted.Draw("e2SAME")
         QCDShapePlot.weighted.Draw("esame")
         l=TLegend(0.45,0.65,0.9,0.9)
         if str(prefix).find("SS") > 0 :
             l.AddEntry(QCDShapePlot.weighted,"Estimated SS QCD shape","p")
	 else:
             l.AddEntry(QCDShapePlot.weighted,"QCD shape","p")
         l.AddEntry(QCDShapeErrorPlot.weighted,"Shape uncertainty","f")
         if str(prefix).find("SS") > 0 :
             l.AddEntry(QCDYieldErrorPlot.weighted,"Yield(+shape) uncertainty","f")
             l.AddEntry(QCDShapePlot.weighted,"Stat(+yield+shape) uncertainty","le")
             l.AddEntry(QCDShapeSSPlot.weighted,"True SS QCD shape","lep")
	 else:
             l.AddEntry(QCDYieldErrorPlot.weighted,"Yield uncertainty","f")
             l.AddEntry(QCDShapePlot.weighted,"Stat uncertainty","le")
         l.SetTextSize(0.035)
         l.SetFillStyle(0)
         l.Draw("same")
         
         gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDcheck.png")
         gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDcheck.pdf")
         gPad.WaitPrimitive()
         QCDShapeSSPlot.weighted.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDcheck.root")
	
      	 QCDOSSSratio=copy.deepcopy(QCDShape)
	 QCDOSSSratio.weighted.Divide(QCDOSSSratio.weighted,copy.deepcopy(QCDlooseSS).weighted,1,1)
         #for b in range(QCDOSSSratio.weighted.GetNbinsX()):
   	 #    QCDOSSSratio.weighted.SetBinError(b+1,QCDOSSSratio.weighted.GetBinContent(b+1)*math.sqrt(1e-4/QCDlooseSS.weighted.GetBinContent(b+1)))
   	 QCDOSSSratio.SetStyle( sBlueLine )

         QCDOSSSratio.weighted.Draw("HISTe")
         QCDOSSSratio.GetYaxis().SetRangeUser(0,3)

         gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDosss.png")
         gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDosss.pdf")
         gPad.WaitPrimitive()
         QCDOSSSratio.weighted.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDosss.root")

        plotVarDataOS.AddHistogram("QCDdata",QCDShape.weighted)
        plotVarDataOS.Hist('QCDdata').stack = True
        plotVarDataOS.Hist('QCDdata').SetStyle( sGreen )
        plotVarDataOS.Hist('QCDdata').layer = 0.99
        plotVarDataOS.Hist('QCDdata').Scale(QCDScale)
	      
        #####################################################
        ###        SAVE ROOT FILE FOR PLOTTING QCD        ###
        #####################################################

        saveQCD(copy.deepcopy(QCDShape),copy.deepcopy(QCDlooseSS),copy.deepcopy(QCDtightSS),var,prefix,mIndex)

        if yields == True :
          Yields_dump = open(os.getcwd()+"/"+prefix+"/Yields_"+var+"_mH"+str(mIndex)+".txt","w")
          print >> Yields_dump, "Yields for MC and Data Higgs Mass = "+str(mIndex)+" GeV"
	  print 
          print >> Yields_dump, "Data: \t\t\t"                     , plotVarDataOS.Hist("Data").weighted.Integral(), "+-", integralAndError(plotVarDataOS.Hist("Data").weighted)[1]
          print >> Yields_dump, "TTJets: \t\t"                     , plotVarDataOS.Hist("TTJets").Integral(), "+-", integralAndError(plotVarDataOS.Hist("TTJets").weighted)[1]
          print >> Yields_dump, "DYJets: \t\t"                     , plotVarDataOS.Hist("DYJets").Integral(), "+-", integralAndError(plotVarDataOS.Hist("DYJets").weighted)[1]
          print >> Yields_dump, "DYJets_Electron: \t"              , plotVarDataOS.Hist("DYJets_Electron").Integral(), "+-", integralAndError(plotVarDataOS.Hist("DYJets_Electron").weighted)[1]
          print >> Yields_dump, "DYJets_Fakes: \t\t"               , plotVarDataOS.Hist("DYJets_Fakes").Integral(), "+-", integralAndError(plotVarDataOS.Hist("DYJets_Fakes").weighted)[1]
          print >> Yields_dump, "WJets: \t\t\t"                    , plotVarDataOS.Hist("WJets").Integral()+plotVarDataOS.Hist("WJets_Fakes").Integral(),"+-",\
	           math.sqrt(pow(integralAndError(plotVarDataOS.Hist("WJets").weighted)[1],2)+pow(integralAndError(plotVarDataOS.Hist("WJets_Fakes").weighted)[1],2))
          print >> Yields_dump, "W3Jets: \t\t\t"                   , plotVarDataOS.Hist("W3Jets").Integral(), "+-", integralAndError(plotVarDataOS.Hist("W3Jets").weighted)[1]
          print >> Yields_dump, "DiBoson: \t\t"                    , plotVarDataOS.Hist("WW").Integral()+plotVarDataOS.Hist("WZ").Integral()+plotVarDataOS.Hist("ZZ").Integral(),"+-",\
	           math.sqrt(pow(integralAndError(plotVarDataOS.Hist("WW").weighted)[1],2)+pow(integralAndError(plotVarDataOS.Hist("WZ").weighted)[1],2)+pow(integralAndError(plotVarDataOS.Hist("ZZ").weighted)[1],2))
          print >> Yields_dump, "QCDdata: \t\t"                    , plotVarDataOS.Hist("QCDdata").Integral(), "+-", integralAndError(plotVarDataOS.Hist("QCDdata").weighted)[1]
          print >> Yields_dump, str('Higgsgg' +str(mIndex)+":\t\t"), plotVarDataOS.Hist(str('Higgsgg' +str(mIndex))).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('Higgsgg' +str(mIndex))).weighted)[1]
          print >> Yields_dump, str('HiggsVBF'+str(mIndex)+":\t\t"), plotVarDataOS.Hist(str('HiggsVBF'+str(mIndex))).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('HiggsVBF'+str(mIndex))).weighted)[1]
          print >> Yields_dump, str('HiggsVH' +str(mIndex)+":\t\t"), plotVarDataOS.Hist(str('HiggsVH' +str(mIndex))).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('HiggsVH' +str(mIndex))).weighted)[1]
          #print >> Yields_dump, str('GluGluToHToWWTo2LAndTau2Nu_M_125'+":\t\t"), plotVarDataOS.Hist(str('GluGluToHToWWTo2LAndTau2Nu_M_125')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('GluGluToHToWWTo2LAndTau2Nu_M_125')).weighted)[1]
          #print >> Yields_dump, str('VBF_HToWWTo2LAndTau2Nu_M_125'+":\t\t"), plotVarDataOS.Hist(str('VBF_HToWWTo2LAndTau2Nu_M_125')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('VBF_HToWWTo2LAndTau2Nu_M_125')).weighted)[1]
          #print >> Yields_dump, str('GluGluToHToWWTo2Tau2Nu_M_120'+":\t\t"), plotVarDataOS.Hist(str('GluGluToHToWWTo2Tau2Nu_M_120')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('GluGluToHToWWTo2Tau2Nu_M_120')).weighted)[1]
          #print >> Yields_dump, str('GluGluToHToWWToTauNuQQ_M_120'+":\t\t"), plotVarDataOS.Hist(str('GluGluToHToWWToTauNuQQ_M_120')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('GluGluToHToWWToTauNuQQ_M_120')).weighted)[1]
          #print >> Yields_dump, str('VBF_HToWWToTauNuQQ_M_120'+":\t\t"), plotVarDataOS.Hist(str('VBF_HToWWToTauNuQQ_M_120')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('VBF_HToWWToTauNuQQ_M_120')).weighted)[1]
          #print >> Yields_dump, str('VBF_HToWWTo2Tau2Nu_M_120'+":\t\t"), plotVarDataOS.Hist(str('VBF_HToWWTo2Tau2Nu_M_120')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('VBF_HToWWTo2Tau2Nu_M_120')).weighted)[1]
          #print >> Yields_dump, str('GluGluToHToZZTo2L2Nu_M_125'+":\t\t"), plotVarDataOS.Hist(str('GluGluToHToZZTo2L2Nu_M_125')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('GluGluToHToZZTo2L2Nu_M_125')).weighted)[1]
          #print >> Yields_dump, str('GluGluToHToZZTo2L2Q_M_125'+":\t\t"), plotVarDataOS.Hist(str('GluGluToHToZZTo2L2Q_M_125')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('GluGluToHToZZTo2L2Q_M_125')).weighted)[1]
          #print >> Yields_dump, str('VBF_ToHToZZTo2l2q_M_130'+":\t\t"), plotVarDataOS.Hist(str('VBF_ToHToZZTo2l2q_M_130')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('VBF_ToHToZZTo2l2q_M_130')).weighted)[1]
          #print >> Yields_dump, str('VBF_ToHToZZTo2L2NU_M_125'+":\t\t"), plotVarDataOS.Hist(str('VBF_ToHToZZTo2L2NU_M_125')).Integral(), "+-", integralAndError(plotVarDataOS.Hist(str('VBF_ToHToZZTo2L2NU_M_125')).weighted)[1]
          Yields_dump.close()
          
        #####################################################
        ###     SAVE ROOT FILES FOR LIMIT COMPUTATION     ###
        #####################################################
        
        if str(prefix).find("VBF") > 0 :
	  if "svfitMass" in var and nx>100:
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"svfitMass","SM2_for_smoothing")
          elif var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"svfitMass","SM2")
          if var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"visMass","SM2")
        elif str(prefix).find("BOOSTED") > 0 :
          if var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"svfitMass","SM1")
          if var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"visMass","SM1")
        #print lineno()
        
        #####################################################
        ###        SAVE ROOT FILES FOR REWEIGHTING        ###
        #####################################################
        #saveForWeighting(plotVarDataOS, plotVarDataSS, plotVarDataLooseIsoSS, plotVarDataLooseIsoOS, prefix, mIndex)

        #####################################################
        ###   BOOSTING THE SIGNAL FOR PICTORIAL RESULTS   ###
        #####################################################

        plotVarDataOS.Hist(str('Higgsgg' +str(mIndex))).Scale(5)
        plotVarDataOS.Hist(str('HiggsVBF'+str(mIndex))).Scale(5)
        plotVarDataOS.Hist(str('HiggsVH' +str(mIndex))).Scale(5)

        #####################################################
        ###            BLINDING DATA ABOVE Z PEAK         ###
        #####################################################
        #if ( var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03" ) and (str(prefix).find("BOOSTED") > 0 or str(prefix).find("VBF") > 0):
        # for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
        #   if ( plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>100 and plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)<140 ):
        #      plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)

        #if (var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03" ) and (str(prefix).find("BOOSTED") > 0 or str(prefix).find("VBF") > 0):
        # for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
        #   if ( plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>80 and plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)<120 ):
        #      plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)

        #if var=="dRtt" and (str(prefix).find("BOOSTED") > 0 or str(prefix).find("VBF") > 0):
        # for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
        #   if plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>1:
        #      plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)
      
        ymax = max( plotVarDataOS.Hist("Data").GetMaximum()            ,
	                plotVarDataOS.Hist("TTJets").GetMaximum()          +
		            plotVarDataOS.Hist("DYJets").GetMaximum()          +
		            plotVarDataOS.Hist("DYJets_Electron").GetMaximum() +
		            plotVarDataOS.Hist("DYJets_Fakes").GetMaximum()    +
		            plotVarDataOS.Hist("WJets").GetMaximum()           +
		            plotVarDataOS.Hist("WJets_Fakes").GetMaximum()     +
		            plotVarDataOS.Hist("WW").GetMaximum()              +
		            plotVarDataOS.Hist("WZ").GetMaximum()              +
		            plotVarDataOS.Hist("ZZ").GetMaximum()              +
		            plotVarDataOS.Hist("QCDdata").GetMaximum() )*1.5
            
        if log:
            plotVarDataOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
        else:
            plotVarDataOS.DrawStack("HIST",xmin,xmax,0,ymax)
      
        gPad.SaveAs(prefix1+prefix+'_'+plotVarDataOS.varName+"_mH"+str(mIndex)+"_data.png")
        gPad.SaveAs(prefix1+prefix+'_'+plotVarDataOS.varName+"_mH"+str(mIndex)+"_data.pdf")
        gPad.WaitPrimitive()

        #####################################################
        ###  SAVE ROOT FILE FOR PLOTTING PRETTIER HISTOS  ###
        #####################################################
	if not "svfitMass" in var or nx<100:
            saveForPlotting(copy.deepcopy(plotVarDataOS),prefix,mIndex)
        
        #####################################################
        ###        SAVE ROOT FILE FOR CORRELATIONS        ###
        #####################################################

        #saveQCD(copy.deepcopy(QCDShape),copy.deepcopy(QCDlooseSSWeighted),copy.deepcopy(QCDtightSS),var,prefix,mIndex,True)


