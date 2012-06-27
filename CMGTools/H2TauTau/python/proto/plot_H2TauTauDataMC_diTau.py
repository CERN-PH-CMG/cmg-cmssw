import os
import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.Style import formatPad,Style
from QCDEstimation_diTau import *
from DYEstimate_diTau import *
from SaveHistograms_diTau import *
from PrepareDictionaries_diTau import *
import math
import copy

from os import path
if path.exists('/afs/cern.ch/work/m/manzoni/diTau2012/CMGTools/CMSSW_5_2_5/src/CMGTools/H2TauTau/python/proto/rootlogon2.C'):
    gROOT.Macro('/afs/cern.ch/work/m/manzoni/diTau2012/CMGTools/CMSSW_5_2_5/src/CMGTools/H2TauTau/python/proto/rootlogon2.C')  # Run ROOT logon script


run2012=True

import ROOT
ROOT.gROOT.SetBatch(True)


#def weightQCD(nVert) : 
#    vertSig[]


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
    options.embed=True
    crossCheckZee=True

    if run2012:
      # 1.616/fb
      selComps['data_Run2012A_PromptReco_v1'].intLumi = 920.039
      weights['data_Run2012A_PromptReco_v1'].intLumi = 920.039
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 696.0
      weights['data_Run2012B_PromptReco_v1'].intLumi = 696.0
      # 2.4/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 1500.
      weights['data_Run2012B_PromptReco_v1'].intLumi = 1500.
      # 2.86/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 1940.
      weights['data_Run2012B_PromptReco_v1'].intLumi = 1940.
      # 3.93/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 3010.
      weights['data_Run2012B_PromptReco_v1'].intLumi = 3010.
      # 5.1/fb
      selComps['data_Run2012B_PromptReco_v1'].intLumi = 4180.
      weights['data_Run2012B_PromptReco_v1'].intLumi = 4180.
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

    selCompsDataMass, weightsDataMass = componentsWithData(selComps,weights)
    #selCompsMCMass, weightsMCMass     = componentsWithOutData(selComps,weights)
    selCompsNoSignal, weightsNoSignal = componentsWithOutSignal(selComps,weights)

    if options.embed:
        embeddedScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight)

    if crossCheckZee:
        zeeScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, options.embed)

    tauScale='0.03'
    shiftedMet = 'sqrt(pow(mex+'+tauScale+'*l1Px+'+tauScale+'*l2Px,2)+pow(mey+'+tauScale+'*l1Py+'+tauScale+'*l2Py,2))' 

    baseline           =  'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0'
    baseline           += ' && l2MVAEle>0.5'
    #baseline           += ' && l1MVAEle>0.5 && l2MVAEle<0.5'
    #antiEleNegato      =  ' && l1MVAEle<0.5 && l2MVAEle<0.5'
    l1Pt35l2Pt35       =  ' && l1Pt>35 && l2Pt>35'
    l1Pt40l2Pt40       =  ' && l1Pt>40 && l2Pt>40'
    l1Pt45l2Pt40       =  ' && l1Pt>45 && l2Pt>40'
    l1Pt45l2Pt45       =  ' && l1Pt>45 && l2Pt>45'
    isolationL         =  ' && (l1RawMVAIso>0.795 || l2RawMVAIso>0.795)'
    isolationMLL       =  ' && ((l1MedMVAIso>0.5 && l2RawMVAIso>0.5) || (l1RawMVAIso>0.5 && l2MedMVAIso>0.5))'

    isolationMNM       =  ' && l1MedMVAIso>0.5 && l2MedMVAIso<0.5'

    isolationM         =  ' && (l1MedMVAIso>0.5 || l2MedMVAIso>0.5)'
    isolationLL        =  ' && l1RawMVAIso>0.795 && l2RawMVAIso>0.795'
    isolationML        =  ' && ((l1MedMVAIso>0.5 && l2RawMVAIso>0.795) || (l1RawMVAIso>0.795 && l2MedMVAIso>0.5))'
    isolationMM        =  ' && l1MedMVAIso>0.5 && l2MedMVAIso>0.5'
    isolationTM        =  ' && ((l1MedMVAIso>0.5 && l2TigMVAIso>0.5) || (l1TigMVAIso>0.5 && l2MedMVAIso>0.5))'
    isolationTT        =  ' && l1TigMVAIso>0.5 && l2TigMVAIso>0.5'
    Jet0               =  ' && jet1Pt<50'
    BOOSTED            =  ' && jet1Pt>50'
    VBF                =  ' &&  jet1Pt>30 && jet2Pt>30 && abs(jet1Eta - jet2Eta)>2.5 && (jet1Eta*jet2Eta)<0 && mjj>250 && nCentralJets==0 '
    NOVBF              =  ' && (jet1Pt<30 || jet2Pt<30 || abs(jet1Eta - jet2Eta)<2.5 || (jet1Eta*jet2Eta)>0 || mjj<250 || nCentralJets >0)'
    VBFtight           =  ' &&  jet1Pt>30 && jet2Pt>30 && abs(jet1Eta - jet2Eta)>4.0 && (jet1Eta*jet2Eta)<0 && mjj>400 && nCentralJets==0'
    NOVBFtight         =  ' && (jet1Pt<30 || jet2Pt<30 || abs(jet1Eta - jet2Eta)<4.0 || (jet1Eta*jet2Eta)>0 || mjj<400 || nCentralJets >0)'

    cuts=[
##### Andreas ###############

        ("CMS_test2012_5_1_embed_zee_oldsvfit_l40_j50_dR20_tt_Met00_BOOSTED_noDR",baseline+l1Pt40l2Pt40+BOOSTED+NOVBF,'',isolationMM,5),
        ("CMS_test2012_5_1_embed_zee_oldsvfit_l40_j50_dR20_tt_Met00_BOOSTED",baseline+l1Pt40l2Pt40+BOOSTED+NOVBF,' && dRtt<2.0',isolationMM,5),
        ("CMS_test2012_5_1_embed_zee_oldsvfit_l40_j50_dR20_tt_Met00_VBF",baseline+l1Pt40l2Pt40+VBF,' && dRtt<2.0',isolationMM,5),
        ("CMS_test2012_5_1_embed_zee_oldsvfit_l45_j50_dR20_tt_Met00_BOOSTED",baseline+l1Pt45l2Pt45+BOOSTED+NOVBF,' && dRtt<2.0',isolationMM,5),

#####  Riccardo  ############################################################################################################################### 

        #("CMS_2012_3_fb_l40_met00_mm_dRtt20_VertReweight_loose_BOOSTED"   , baseline + BOOSTED + l1Pt40l2Pt40 + NOVBF     , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_3_fb_l40_met00_mm_dRtt20_VertReweight_tight_BOOSTED"   , baseline + BOOSTED + l1Pt40l2Pt40 + NOVBFtight, ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_3_fb_l40_met00_mm_dRtt20_VertReweight_loose_VBF"       , baseline + VBF      + l1Pt40l2Pt40 + VBF      , ' && dRtt<2.0' , isolationMM , 5 ),
        #("CMS_2012_3_fb_l40_met00_mm_dRtt20_noReweight_tight_VBF"       , baseline + VBFtight + l1Pt40l2Pt40 + VBFtight , ' && dRtt<2.0' , isolationMM , 5 ),


        #("CMS_2012_June1v3_fb_l35_j50_met00_mm_dRtt20_met20_Reweight_BOOSTED"  , baseline + BOOSTED , ' && met < 20' , isolationMM , 5 ),
        #("CMS_2012_June1v3_fb_l40_j50_met00_mm_dRtt20_noReweightl2Pt_BOOSTED" , baseline + BOOSTED + l1Pt40l2Pt40 , ' && dRtt<2.0' , isolationMM , 5 ),
        
        #("CMS_2012_June1v3_fb_controlRegion2_BOOSTED"                   , baseline + BOOSTED , ' && dRtt>2' , isolationMM , 5 ),
        
        #("CMS_test2012_2_86_embed_zee_l40_j50_dR20_mm_Met00_BOOSTED"    , baseline + l1Pt40l2Pt40 + BOOSTED + NOVBF ,' && dRtt<2.0',isolationMM,5),
        #("CMS_test2012_2_86_embed_zee_l40_j50_dR20_mm_Met00_VBF"        , baseline + l1Pt40l2Pt40 + VBF             ,' && dRtt<2.0',isolationMM,5),
        #("CMS_test2012_2_86_embed_zee_l45_j50_dR20_mm_Met00_BOOSTED"    , baseline + l1Pt45l2Pt45 + BOOSTED + NOVBF ,' && dRtt<2.0',isolationMM,5),

	  ]
        
    for prefix,cut,antiqcdcut,isocut,qcdEstimate in cuts:
     if str(prefix).find("VBF") > 0 :
       rebin = 2	
     else :
       rebin = 1	
     for var, nx, xmin, xmax in [
        ('svfitMass'        ,int(30/rebin), 0 , 300    ), 
        ('svfitMass*1.03'   ,int(30/rebin), 0 , 300    ),
        ('svfitMass*0.97'   ,int(30/rebin), 0 , 300    ),
        ('met'              ,int(40/rebin), 0 , 200    ),
        ('l1Pt'             ,int(50/rebin), 0 , 250    ),   # was 75 bins
        ('l2Pt'             ,int(50/rebin), 0 , 250    ),   # was 75 bins
        ('jet1Pt'           ,int(50/rebin), 0 , 500    ),
        ('jet2Pt'           ,int(50/rebin), 0 , 500    ),
        ('visMass'          ,int(30/rebin), 0 , 300    ),
        #('visMass*1.03'     ,int(30/rebin), 0 , 300    ),
        #('visMass*0.97'     ,int(30/rebin), 0 , 300    ),
        ('nVert'            ,int(25/rebin), 0 , 50     ),
        ('l1Eta'            ,int(20/rebin), -3, 3      ),   # was 40 bins
        ('l2Eta'            ,int(20/rebin), -3, 3      ),   # was 40 bins
        ('jet1Eta'          ,int(20/rebin), -5, 5      ),
        ('jet2Eta'          ,int(20/rebin), -5, 5      ),
        ('mjj'              ,int(30/rebin), 0 , 800    ),
        ('nJets'            ,10           , 0 , 10     ),
        ('dRtt'             ,int(40/rebin), 0 , 5      ),
        ('dPhitt'           ,int(40/rebin), 0 , 3.15   ),
        ('mt'               ,int(40/rebin), 0 , 200    ),
        ('pThiggs'          ,int(40/rebin), 0 , 300    ),
        ('diTauPt'          ,int(40/rebin), 0 , 300    ),
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
      
      prefix1 = os.getcwd()+"/"+prefix+"/diTau_2012_"
      dirList = os.listdir(os.getcwd())
      exists = False
      for fname in dirList:
        if fname == prefix :
          exists = True
      if not(exists) :
        os.mkdir(os.getcwd()+"/"+prefix)

      #if var in ["met","jet1Pt","jet2Pt","l1Pt","l2Pt"]:
      #    log=True
      #else:
      log=False

      #looseisocut=" && !(1 "+isocut+")"
      #semilooseisocut=isolationM+" && !(1 "+isocut+")"

      looseisocut=isolationM+" && !(1 "+isocut+")"
      #looseisocut=isolationMNM+" && !(1 "+isocut+")"

      semilooseisocut=isolationMLL+" && !(1 "+isocut+")"
      
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
        pass

      cutSS=cut.replace("diTauCharge==0","diTauCharge!=0")
      
      plotVarDataSS = H2TauTauDataMC(var,                           \
                                     anaDir,                        \
                                     selCompsNoSignal,              \
                                     weightsNoSignal,               \
     			                     nx,                            \
     			                     xmin,                          \
     			                     xmax,                          \
     			                     cut = cutSS+isocut+antiqcdcut, \
     			                     weight=weight,                 \
     			                     embed=options.embed)
      
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

      plotVarDataLooseIsoOS = H2TauTauDataMC(var,                              \
                                             anaDir,                           \
                                             selCompsNoSignal,                 \
                                             weightsNoSignal,                  \
     			                             nx,                               \
     			                             xmin,                             \
     			                             xmax,                             \
     			                             cut = cut+looseisocut+antiqcdcut, \
     			                             #weight=weight+"*weightQCD_nVert(nVert)",\
     			                             #weight=weight+"*weightQCD_l2Pt(l2Pt)*weightQCD_nVert(nVert)",\
     			                             weight=weight,                    \
     			                             embed=options.embed)

      if qcdEstimate==4 or qcdEstimate==5:			    
       plotVarDataSemiLooseIsoSS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
     			    nx, xmin, xmax,
     			    cut = cutSS+semilooseisocut+antiqcdcut, weight=weight,
     			    embed=options.embed)
       plotVarDataSemiLooseIsoOS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
     			    nx, xmin, xmax,
     			    cut = cut+semilooseisocut+antiqcdcut, weight=weight,
     			    embed=options.embed)
			    
      if qcdEstimate==0 or qcdEstimate==1 or qcdEstimate==3:
       plotVarDataLowControlOS = 0
       plotVarDataLowControlSS = 0
       plotVarDataAverageControlOS = 0
       plotVarDataAverageControlSS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
       plotVarDataHighControlOS = 0
       plotVarDataHighControlSS = 0
      if qcdEstimate==0 or qcdEstimate==1:
       plotVarDataLowControlLooseIsoOS = 0
       plotVarDataAverageControlLooseIsoOS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
     			    nx, xmin, xmax,
     			    cut = cut+looseisocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
       plotVarDataHighControlLooseIsoOS = 0
      if qcdEstimate==0 or qcdEstimate==1 or qcdEstimate==3:
       plotVarDataLowControlLooseIsoSS = 0
       plotVarDataAverageControlLooseIsoSS = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal,
     			    nx, xmin, xmax,
     			    cut = cutSS+looseisocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
       plotVarDataHighControlLooseIsoSS = 0

      if qcdEstimate==1:
       QCDShape, QCDScale = QCDEstimate1(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                       plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS, plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,
                                       plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS, plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS, plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,log)

      if qcdEstimate==2:
        QCDShape, QCDScale, QCDlooseSS, QCDtightSS = QCDEstimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, log)

      if qcdEstimate==6:
        QCDShape, QCDScale, QCDlooseSS, QCDtightSS = QCD_WJets_Estimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, log)

      if qcdEstimate==3:
       QCDShape, QCDScale = QCDEstimate3(prefix,prefix1,xmin,xmax,plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataAverageControlSS, plotVarDataAverageControlLooseIsoSS, log)

      if qcdEstimate==4:
       QCDShape, QCDScale = QCDEstimate4(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataSemiLooseIsoSS, log)

      if qcdEstimate==5:
       QCDShape    , QCDScale    , QCDlooseSS    , QCDtightSS     = QCDEstimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, log)
       QCDShapeSemi, QCDScaleSemi, QCDlooseSSSemi, QCDtightSSSemi = QCDEstimate2(prefix,prefix1,var,xmin,xmax,plotVarDataSS, plotVarDataSemiLooseIsoOS, plotVarDataSemiLooseIsoSS, log)

       QCDShapePlot=copy.deepcopy(QCDShape)
       QCDShapePlot.SetStyle( sRedLine )
       QCDShapePlot.weighted.Scale(QCDScale)
        
       QCDShapeSemiPlot=copy.deepcopy(QCDShapeSemi)
       QCDShapeSemiPlot.SetStyle( sBlueLine )
       QCDShapeSemiPlot.weighted.Scale(QCDScaleSemi)

       if True:
          print "tight SS"
          print "Data:"                    , plotVarDataSS.Hist("Data").Integral()
          print "TTJets:"                  , plotVarDataSS.Hist("TTJets").Integral()
          print "DYJets:"                  , plotVarDataSS.Hist("DYJets").Integral()#+plotVarDataSS.Hist("DYJets_Photon").Integral()
          print "DYJets_Electron:"         , plotVarDataSS.Hist("DYJets_Electron").Integral()
          print "DYJets_Fakes:"            , plotVarDataSS.Hist("DYJets_Fakes").Integral()
          print "WJets:"                   , plotVarDataSS.Hist("WJets").Integral()+plotVarDataSS.Hist("WJets_Fakes").Integral()
          print "DiBoson:"                 , plotVarDataSS.Hist("WW").Integral()+plotVarDataSS.Hist("WZ").Integral()+plotVarDataSS.Hist("ZZ").Integral()

       if True:
          print "loose OS"
          print "Data:"                    , plotVarDataLooseIsoOS.Hist("Data").Integral()
          print "TTJets:"                  , plotVarDataLooseIsoOS.Hist("TTJets").Integral()
          print "DYJets:"                  , plotVarDataLooseIsoOS.Hist("DYJets").Integral()#+plotVarDataLooseIsoOS.Hist("DYJets_Photon").Integral()
          print "DYJets_Electron:"         , plotVarDataLooseIsoOS.Hist("DYJets_Electron").Integral()
          print "DYJets_Fakes:"            , plotVarDataLooseIsoOS.Hist("DYJets_Fakes").Integral()
          print "WJets:"                   , plotVarDataLooseIsoOS.Hist("WJets").Integral()+plotVarDataLooseIsoOS.Hist("WJets_Fakes").Integral()
          print "DiBoson:"                 , plotVarDataLooseIsoOS.Hist("WW").Integral()+plotVarDataLooseIsoOS.Hist("WZ").Integral()+plotVarDataLooseIsoOS.Hist("ZZ").Integral()

       if True:
          print "loose SS"
          print "Data:"                    , plotVarDataLooseIsoSS.Hist("Data").Integral()
          print "TTJets:"                  , plotVarDataLooseIsoSS.Hist("TTJets").Integral()
          print "DYJets:"                  , plotVarDataLooseIsoSS.Hist("DYJets").Integral()#+plotVarDataLooseIsoSS.Hist("DYJets_Photon").Integral()
          print "DYJets_Electron:"         , plotVarDataLooseIsoSS.Hist("DYJets_Electron").Integral()
          print "DYJets_Fakes:"            , plotVarDataLooseIsoSS.Hist("DYJets_Fakes").Integral()
          print "WJets:"                   , plotVarDataLooseIsoSS.Hist("WJets").Integral()+plotVarDataLooseIsoSS.Hist("WJets_Fakes").Integral()
          print "DiBoson:"                 , plotVarDataLooseIsoSS.Hist("WW").Integral()+plotVarDataLooseIsoSS.Hist("WZ").Integral()+plotVarDataLooseIsoSS.Hist("ZZ").Integral()

       ymax = max(QCDShapePlot.GetMaximum(),QCDShapeSemiPlot.GetMaximum())*1.5
        
       QCDShapePlot.weighted.Draw("HISTe")
       if log:
           QCDShapePlot.GetYaxis().SetRangeUser(0.1,ymax)
       else:
           QCDShapePlot.GetYaxis().SetRangeUser(0,ymax)
       QCDShapeSemiPlot.weighted.Draw("HISTeSAME")
     
       gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_QCDcheck.png")
       gPad.WaitPrimitive()
      
      #######################################################
      ################# PLOTTING DATA/MC ####################
      #######################################################
      yields = False
      if ( var == "svfitMass" or var == "svfitMass*1.03" or var == "svfitMass*0.97" ):
        #massesRange = [110,115,120,125,130,135,140,145]
        massesRange = [125]
        print 'I\'m plotting mass distribution for masses in [110,115,120,125,130,135,140,145] GeV'
        if var == "svfitMass" :
          yields = True
      else :
        massesRange = [125]
        print 'I\'m plotting distribution just for mass 125 GeV'
        
      for mIndex in massesRange :
        
        plotVarDataOS = H2TauTauDataMC(var, anaDir, selCompsDataMass[mIndex], weightsDataMass[mIndex],
     			    nx, xmin, xmax,
     			    cut = cut+isocut+antiqcdcut, weight=weight,
     			    embed=options.embed)

        
        ### WJets estimate for low stat 2012 MC samples ###
        WJetsShape = copy.deepcopy(plotVarDataLooseIsoOS.Hist("WJets"))
        #WJetsScale = plotVarDataOS.Hist("WJets").Integral()/plotVarDataLooseIsoOS.Hist("WJets").Integral()
        WJetsScale = 1.5*plotVarDataOS.Hist("WJets").Integral()/plotVarDataLooseIsoOS.Hist("WJets").Integral()
        WJetsFakesShape = copy.deepcopy(plotVarDataLooseIsoOS.Hist("WJets_Fakes"))
        #WJetsFakesScale = plotVarDataOS.Hist("WJets_Fakes").Integral()/plotVarDataLooseIsoOS.Hist("WJets_Fakes").Integral()
        WJetsFakesScale = 1.5*plotVarDataOS.Hist("WJets_Fakes").Integral()/plotVarDataLooseIsoOS.Hist("WJets_Fakes").Integral()
        plotVarDataOS.Hist("WJets").obj = WJetsShape.obj
        plotVarDataOS.Hist("WJets").weighted = WJetsShape.weighted
        plotVarDataOS.Hist("WJets").Scale(WJetsScale)
        plotVarDataOS.Hist("WJets_Fakes").obj = WJetsFakesShape.obj
        plotVarDataOS.Hist("WJets_Fakes").weighted = WJetsFakesShape.weighted
        plotVarDataOS.Hist("WJets_Fakes").Scale(WJetsFakesScale)

        plotVarDataOS.AddHistogram("QCDdata",QCDShape.weighted)
        plotVarDataOS.Hist('QCDdata').stack = True
        plotVarDataOS.Hist('QCDdata').SetStyle( sBlack )
        plotVarDataOS.Hist('QCDdata').layer = 0.99
        plotVarDataOS.Hist('QCDdata').Scale(QCDScale)
	      
        #####################################################
        ###        SAVE ROOT FILE FOR PLOTTING QCD        ###
        #####################################################

        #saveQCD(copy.deepcopy(QCDShape),copy.deepcopy(QCDlooseSS),copy.deepcopy(QCDtightSS),var,prefix,mIndex)

        if yields == True :
          Yields_dump = open(os.getcwd()+"/"+prefix+"/Yields_"+var+"_mH"+str(mIndex)+".txt","w")
          print >> Yields_dump, "Yields for MC and Data Higgs Mass = "+str(mIndex)+" GeV"
          print >> Yields_dump, "Data: \t\t\t"                    , plotVarDataOS.Hist("Data").Integral()
          print >> Yields_dump, "TTJets: \t\t"                    , plotVarDataOS.Hist("TTJets").Integral()
          print >> Yields_dump, "DYJets: \t\t"                    , plotVarDataOS.Hist("DYJets").Integral()
          print >> Yields_dump, "DYJets_Electron: \t"             , plotVarDataOS.Hist("DYJets_Electron").Integral()
          print >> Yields_dump, "DYJets_Fakes: \t\t"              , plotVarDataOS.Hist("DYJets_Fakes").Integral()
          print >> Yields_dump, "WJets: \t\t\t"                   , plotVarDataOS.Hist("WJets").Integral()+plotVarDataOS.Hist("WJets_Fakes").Integral()
          print >> Yields_dump, "DiBoson: \t\t"                   , plotVarDataOS.Hist("WW").Integral()+plotVarDataOS.Hist("WZ").Integral()+plotVarDataOS.Hist("ZZ").Integral()
          print >> Yields_dump, "QCDdata: \t\t"                   , plotVarDataOS.Hist("QCDdata").Integral()
          print >> Yields_dump, str('Higgsgg'+str(mIndex)+":\t\t") , plotVarDataOS.Hist(str('Higgsgg'+str(mIndex))).Integral()
          print >> Yields_dump, str('HiggsVBF'+str(mIndex)+":\t\t"), plotVarDataOS.Hist(str('HiggsVBF'+str(mIndex))).Integral()
          Yields_dump.close()
          
        #####################################################
        ###     SAVE ROOT FILES FOR LIMIT COMPUTATION     ###
        #####################################################
        
        if str(prefix).find("VBF") > 0 :
          if var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"svfitMass","SM2")
          if var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"visMass","SM2")
        elif str(prefix).find("BOOSTED") > 0 :
          if var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"svfitMass","SM1")
          if var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03":
            saveForLimit(copy.deepcopy(plotVarDataOS),prefix,mIndex,"visMass","SM1")
      
        #####################################################
        ###   BOOSTING THE SIGNAL FOR PICTORIAL RESULTS   ###
        #####################################################

        plotVarDataOS.Hist(str('Higgsgg'+str(mIndex))).Scale(5)
        plotVarDataOS.Hist(str('HiggsVBF'+str(mIndex))).Scale(5)

        #####################################################
        ###            BLINDING DATA ABOVE Z PEAK         ###
        #####################################################
        if ( var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03" ) and (str(prefix).find("BOOSTED") > 0 or str(prefix).find("VBF") > 0):
         for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
           if ( plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>100 and plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)<140 ):
              plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)

        #if (var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03" ) and (str(prefix).find("BOOSTED") > 0 or str(prefix).find("VBF") > 0):
        # for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
        #   if ( plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>80 and plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)<120 ):
        #      plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)

        #if var=="dRtt" and (str(prefix).find("BOOSTED") > 0 or str(prefix).find("VBF") > 0):
        # for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
        #   if plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>1:
        #      plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)
      
        ymax = max( plotVarDataOS.Hist("Data").GetMaximum(),
	              plotVarDataOS.Hist("TTJets").GetMaximum()+
		      plotVarDataOS.Hist("DYJets").GetMaximum()+
		      plotVarDataOS.Hist("DYJets_Electron").GetMaximum()+
		      plotVarDataOS.Hist("DYJets_Fakes").GetMaximum()+
		      plotVarDataOS.Hist("WJets").GetMaximum()+
		      plotVarDataOS.Hist("WJets_Fakes").GetMaximum()+
		      plotVarDataOS.Hist("WW").GetMaximum()+
		      plotVarDataOS.Hist("WZ").GetMaximum()+
		      plotVarDataOS.Hist("ZZ").GetMaximum()+
		      plotVarDataOS.Hist("QCDdata").GetMaximum() )*1.5
            
        if log:
            plotVarDataOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
        else:
            plotVarDataOS.DrawStack("HIST",xmin,xmax,0,ymax)
      
        gPad.SaveAs(prefix1+prefix+'_'+plotVarDataOS.varName+"_mH"+str(mIndex)+"_data.png")
        gPad.WaitPrimitive()

        #####################################################
        ###  SAVE ROOT FILE FOR PLOTTING PRETTIER HISTOS  ###
        #####################################################
        saveForPlotting(copy.deepcopy(plotVarDataOS),prefix,mIndex)
        
