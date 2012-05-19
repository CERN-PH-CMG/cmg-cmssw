import os
import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.Style import formatPad,Style
from QCDEstimation_diTau import *
from SaveHistograms_diTau import *
from PrepareDictionaries_diTau import *
import math

import ROOT
ROOT.gROOT.SetBatch(True)

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
    selCompsMCMass, weightsMCMass     = componentsWithOutData(selComps,weights)

    tauScale='0.03'
    shiftedMet = 'sqrt(pow(mex+'+tauScale+'*l1Px+'+tauScale+'*l2Px,2)+pow(mey+'+tauScale+'*l1Py+'+tauScale+'*l2Py,2))' 

    baseline='l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>50'
    baseline+=' && l1LooseEle>0.5 && l2LooseEle>0.5 && (l1MVAEle>0.5 || l2MVAEle>0.5)'
    #isolation=' && l1TigIso>0.5 && l2TigIso>0.5'
    #isolation=' &&((l1TigIso>0.5 && l2MedIso>0.5) || (l1MedIso>0.5 && l2TigIso>0.5))'
    isolation=' && l1TigMVAIso>0.5 && l2TigMVAIso>0.5'
    #isolation=' &&((l1TigMVAIso>0.5 && l2MedMVAIso>0.5) || (l1MedMVAIso>0.5 && l2TigMVAIso>0.5))'
    #isolation=' && l1RawMVAIso>0.93 && l2RawMVAIso>0.93'

    cuts=[
        ("CMS_l45_j50_dR20_tt_Met00_BOOSTED",baseline,' && dRtt<2.0',isolation,2),
	  ]
        
    for prefix,cut,antiqcdcut,isocut,qcdEstimate in cuts:
     if str(prefix).find("VBF") > 0 :
       rebin = 1	
     else :
       rebin = 1
     for var, nx, xmin, xmax in [
        ('svfitMass'        ,int(15/rebin), 0 , 300    ), 
        ('svfitMass*1.03'   ,int(15/rebin), 0 , 300    ),
        ('svfitMass*0.97'   ,int(15/rebin), 0 , 300    ),
        ('visMass'          ,int(15/rebin), 0 , 300    ),
        ('visMass*1.03'     ,int(15/rebin), 0 , 300    ),
        ('visMass*0.97'     ,int(15/rebin), 0 , 300    ),
        ('nVert'            ,int(50/rebin), 0 , 50     ),
        ('mt'               ,int(20/rebin), 0 , 200    ),
        ('pThiggs'          ,int(20/rebin), 0 , 300    ),
        ('diTauPt'          ,int(20/rebin), 0 , 300    ),
        ('l1Pt'             ,int(75/rebin), 0 , 250    ),   # was 25 bins
        ('l2Pt'             ,int(75/rebin), 0 , 250    ),   # was 25 bins
        ('l1Eta'            ,int(40/rebin), -3, 3      ),   # was 20 bins
        ('l2Eta'            ,int(40/rebin), -3, 3      ),   # was 20 bins
        ('jet1Eta'          ,int(20/rebin), -5, 5      ),
        ('jet1Pt'           ,int(20/rebin), 0 , 500    ),
        ('jet2Eta'          ,int(20/rebin), -5, 5      ),
        ('jet2Pt'           ,int(20/rebin), 0 , 500    ),
        ('mjj'              ,int(32/rebin), 0 , 800    ),
        ('dRtt'             ,int(20/rebin), 0 , 5      ),
        ('dPhitt'           ,int(20/rebin), 0 , 3.15   ),
        ('mttj'             ,int(20/rebin), 0 , 1000   ),
        ('met'              ,int(20/rebin), 0 , 200    ),
        ('diTauCharge'      ,7            , -3, 3      ),
        ('l1LooIso'         ,2            , 0,  2      ),
        ('l2LooIso'         ,2            , 0,  2      ),
        ('l1MedIso'         ,2            , 0,  2      ),
        ('l2MedIso'         ,2            , 0,  2      ),
        ('l1TigIso'         ,2            , 0,  2      ),
        ('l2TigIso'         ,2            , 0,  2      ),
        ('l1DecayMode'      ,12           , 0 , 12     ),
        ('l2DecayMode'      ,12           , 0 , 12     ),
        ('l1RawMVAIso'      ,100          , 0 , 1.00001),
        ('l1MedMVAIso'      ,2            , 0 , 2      ),
        ('l1TigMVAIso'      ,2            , 0 , 2      ),
        #('l1LooseEle'       ,2            , 0 , 2      ),
        ('l1MVAEle'         ,2            , 0 , 2      ),
        #('l1LooseMu'        ,2            , 0 , 2      ),
        ('l2RawMVAIso'      ,100          , 0 , 1.00001),
        ('l2MedMVAIso'      ,2            , 0 , 2      ),
        ('l2TigMVAIso'      ,2            , 0 , 2      ),
        #('l2LooseEle'       ,2            , 0 , 2      ),
        ('l2MVAEle'         ,2            , 0 , 2      ),
        #('l2LooseMu'        ,2            , 0 , 2      ),
      ]:
      
      prefix1 = os.getcwd()+"/"+prefix+"/IsoTauTight1fb_"
      dirList = os.listdir(os.getcwd())
      exists = False
      for fname in dirList:
        if fname == prefix :
          exists = True
      if not(exists) :
        os.mkdir(os.getcwd()+"/"+prefix)

      if var in ["met","jet1Pt","jet2Pt","l1Pt","l2Pt"]:
          log=True
      else:
          log=False

      if qcdEstimate==0:
        # MET based QCD estimation
        looseisocut=""#" && l1LooIso>0.5 && l2LooIso>0.5"
        lowcontrolcut=" && met<10"
        averagecontrolcut=" && met<20"
        highcontrolcut=" && met>10 && met<20"
      elif qcdEstimate==1:
        # dR based QCD estimation
        looseisocut=""#" && l1LooIso>0.5 && l2LooIso>0.5"
        lowcontrolcut=" && dRtt>3.0"
        averagecontrolcut=" && dRtt>2.0"
        highcontrolcut=" && dRtt>2.0 && dRtt<3.0"
      elif qcdEstimate==2:
        # Loose based QCD estimation
        looseisocut=""#" && l1LooIso<0.5 && l2LooIso<0.5"
      elif qcdEstimate==3:
        # OS/SS from loose. tight/loose from dR SS
        looseisocut=""#" && l1LooIso>0.5 && l2LooIso>0.5"
        lowcontrolcut=" && dRtt>3.0"
        averagecontrolcut=" && dRtt>2.0"
        highcontrolcut=" && dRtt>2.0 && dRtt<3.0"

      cutSS=cut.replace("diTauCharge==0","diTauCharge!=0")
      
      plotVarDataSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+antiqcdcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+looseisocut+antiqcdcut, weight=weight,
     			    embed=options.embed)
			    
      if qcdEstimate==2 or qcdEstimate==3:
       plotVarDataLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+looseisocut+antiqcdcut, weight=weight,
     			    embed=options.embed)

      if qcdEstimate==0 or qcdEstimate==3:
       plotVarDataLowControlOS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cut+isocut+lowcontrolcut, weight=weight,
     	#		    embed=options.embed)
       plotVarDataLowControlSS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cutSS+isocut+lowcontrolcut, weight=weight,
     	#		    embed=options.embed)
       plotVarDataAverageControlOS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cut+isocut+averagecontrolcut, weight=weight,
     	#		    embed=options.embed)
       plotVarDataAverageControlSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
       plotVarDataHighControlOS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cut+isocut+highcontrolcut, weight=weight,
     	#		    embed=options.embed)
       plotVarDataHighControlSS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cutSS+isocut+highcontrolcut, weight=weight,
     	#		    embed=options.embed)
      if qcdEstimate==0 or qcdEstimate==1:
       plotVarDataLowControlLooseIsoOS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cut+looseisocut+lowcontrolcut, weight=weight,
     	#		    embed=options.embed)
       plotVarDataAverageControlLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+looseisocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
       plotVarDataHighControlLooseIsoOS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cut+looseisocut+highcontrolcut, weight=weight,
     	#		    embed=options.embed)
      if qcdEstimate==0 or qcdEstimate==1 or qcdEstimate==3:
       plotVarDataLowControlLooseIsoSS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
      	#		    nx, xmin, xmax,
     	#		    cut = cutSS+looseisocut+lowcontrolcut, weight=weight,
     	#		    embed=options.embed)
       plotVarDataAverageControlLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+looseisocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
       plotVarDataHighControlLooseIsoSS = 0#H2TauTauDataMC(var, anaDir, selComps, weights,
     	#		    nx, xmin, xmax,
     	#		    cut = cutSS+looseisocut+highcontrolcut, weight=weight,
     	#		    embed=options.embed)

      if qcdEstimate==1:
       QCDShape, QCDScale = QCDEstimate1(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                       plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS, plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,
                                       plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS, plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS, plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,
       			       log)

      if qcdEstimate==2:
       QCDShape, QCDScale = QCDEstimate2(prefix,prefix1,xmin,xmax,plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, log)

      if qcdEstimate==3:
       QCDShape, QCDScale = QCDEstimate3(prefix,prefix1,xmin,xmax,plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataAverageControlSS, plotVarDataAverageControlLooseIsoSS, log)


      #######################################################
      ################# PLOTTING DATA/MC ####################
      #######################################################

      for mIndex in [125]:#[110,115,120,125,130,135] :

        plotVarDataOS = H2TauTauDataMC(var, anaDir, selCompsDataMass[mIndex], weightsDataMass[mIndex],
     			    nx, xmin, xmax,
     			    cut = cut+isocut+antiqcdcut, weight=weight,
     			    embed=options.embed)

        plotVarDataOS.AddHistogram("QCDdata",QCDShape.weighted)
        plotVarDataOS.Hist('QCDdata').stack = True
        plotVarDataOS.Hist('QCDdata').SetStyle( sBlack )
        plotVarDataOS.Hist('QCDdata').layer = 5
        plotVarDataOS.Hist('QCDdata').Scale(QCDScale)

        print "Yields for MC and Data Higgs Mass = "+str(mIndex)+" GeV"
        print "Data:"                    , plotVarDataOS.Hist("Data").Integral()
        print "TTJets:"                  , plotVarDataOS.Hist("TTJets").Integral()
        print "DYJets:"                  , plotVarDataOS.Hist("DYJets").Integral()+plotVarDataOS.Hist("DYJets_Photon").Integral()
        print "DYJets_Electron:"         , plotVarDataOS.Hist("DYJets_Electron").Integral()
        print "DYJets_Fakes:"            , plotVarDataOS.Hist("DYJets_Fakes").Integral()
        print "WJets:"                   , plotVarDataOS.Hist("WJets").Integral()+plotVarDataOS.Hist("WJets_Fakes").Integral()
        print "DiBoson:"                 , plotVarDataOS.Hist("WW").Integral()+plotVarDataOS.Hist("WZ").Integral()+plotVarDataOS.Hist("ZZ").Integral()
        print "QCDdata:"                 , plotVarDataOS.Hist("QCDdata").Integral()
        print str('Higgsgg'+str(mIndex)) , plotVarDataOS.Hist(str('Higgsgg'+str(mIndex))).Integral()
        print str('HiggsVBF'+str(mIndex)), plotVarDataOS.Hist(str('HiggsVBF'+str(mIndex))).Integral()

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
        if ( var=="svfitMass" or var=="svfitMass*0.97" or var=="svfitMass*1.03" ):
         for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
           if plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>100:
              plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)

        if (var=="visMass" or var=="visMass*0.97" or var=="visMass*1.03" ):
         for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
           if plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)>80:
              plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)

        if var=="dRtt":
         for bin in range(plotVarDataOS.Hist("Data").weighted.GetNbinsX()):
           if plotVarDataOS.Hist("Data").weighted.GetBinCenter(bin+1)<2:
              plotVarDataOS.Hist("Data").weighted.SetBinContent(bin+1,-1)
      
        ymax = plotVarDataOS.Hist("Data").GetMaximum()*1.5
            
        if log:
            plotVarDataOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
        else:
            plotVarDataOS.DrawStack("HIST",xmin,xmax,0,ymax)
      
        gPad.SaveAs(prefix1+prefix+'_'+plotVarDataOS.varName+"_mH"+str(mIndex)+"_data.png")
        gPad.WaitPrimitive()

        #####################################################
        ###  SAVE ROOT FILE FOR PLOTTING PRETTIER HISTOS  ###
        #####################################################
        #saveForPlotting(copy.deepcopy(plotVarDataOS),prefix,mIndex)
        
'''
        #######################################################
        ################# PLOTTING MC ONLY ####################
        #######################################################

        plotMass = H2TauTauDataMC(var, anaDir, selCompsMCMass[mIndex], weightsMCMass[mIndex],
				nx, xmin, xmax,
				cut = cut+isocut+antiqcdcut, weight=weight,
				embed=options.embed)

        print "MC lumi:", plotMass.intLumi
        plotMass.AddHistogram("QCDdata",QCDShape.weighted)
        plotMass.Hist('QCDdata').stack = True
        plotMass.Hist('QCDdata').SetStyle( sBlack )
        plotMass.Hist('QCDdata').layer = 5
        plotMass.Hist('QCDdata').Scale(plotMass.intLumi/plotVarDataOS.intLumi)
        plotMass.Hist('QCDdata').Scale(QCDScale)
        plotMass.Hist(str('Higgsgg'+str(mIndex))).Scale(5)
        plotMass.Hist(str('HiggsVBF'+str(mIndex))).Scale(5)
        ymax = (plotMass.Hist("QCDdata").GetMaximum()+
        plotMass.Hist("TTJets").GetMaximum()+
        plotMass.Hist("WW").GetMaximum()+
        plotMass.Hist("WZ").GetMaximum()+
        plotMass.Hist("ZZ").GetMaximum()+
        plotMass.Hist("DYJets").GetMaximum()+
        plotMass.Hist("DYJets_Fakes").GetMaximum()+
        plotMass.Hist("DYJets_Photon").GetMaximum()+
        plotMass.Hist("DYJets_Electron").GetMaximum()+
        plotMass.Hist("WJets").GetMaximum()+
        plotMass.Hist("WJets_Fakes").GetMaximum()+
        plotMass.Hist("WW").GetMaximum()+
        plotMass.Hist("WZ").GetMaximum()+
        plotMass.Hist("ZZ").GetMaximum())*1.5

        if log:
          plotMass.DrawStack("HIST",xmin,xmax,0.1,ymax)
        else:
          plotMass.DrawStack("HIST",xmin,xmax,0,ymax)
        
        gPad.SaveAs(prefix1+prefix+'_'+plotMass.varName+"_mH"+str(mIndex)+"_mc.png")
        gPad.WaitPrimitive()
        
#         print "Yields for Higgs Mass = "+str(mIndex)+" GeV"
#         print "TTJets:"                  , plotVar.Hist("TTJets").Integral()
#         print "DYJets:"                  , plotVar.Hist("DYJets").Integral()+plotVar.Hist("DYJets_Photon").Integral()
#         print "DYJets_Electron:"         , plotVar.Hist("DYJets_Electron").Integral()
#         print "DYJets_Fakes:"            , plotVar.Hist("DYJets_Fakes").Integral()
#         print "WJets:"                   , plotVar.Hist("WJets").Integral()+plotVar.Hist("WJets_Fakes").Integral()
#         print "DiBoson:"                 , plotVar.Hist("WW").Integral()+plotVar.Hist("WZ").Integral()+plotVar.Hist("ZZ").Integral()
#         print "QCDdata:"                 , plotVar.Hist("QCDdata").Integral()
#         print str('Higgsgg'+str(mIndex)) , plotVar.Hist(str('Higgsgg'+str(mIndex))).Integral()
#         print str('HiggsVBF'+str(mIndex)), plotVar.Hist(str('HiggsVBF'+str(mIndex))).Integral()
'''
	