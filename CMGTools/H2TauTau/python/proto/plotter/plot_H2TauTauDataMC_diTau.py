import numpy
import math
import inspect
import os
import imp
from ROOT import gROOT, gStyle
from numpy import array
from os import path
from copy import deepcopy
from CMGTools.RootTools.Style import formatPad,Style
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.cuts_diTau                import *
from CMGTools.H2TauTau.proto.plotter.titles_diTau              import xtitles
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC_diTau      import *
from CMGTools.H2TauTau.proto.plotter.QCDEstimation_diTau       import *
from CMGTools.H2TauTau.proto.plotter.DYEstimate_diTau          import *
from CMGTools.H2TauTau.proto.plotter.SaveHistograms_diTau      import *
from CMGTools.H2TauTau.proto.plotter.PrepareDictionaries_diTau import *

def bah():
  print '*'*150
  print len(gROOT.GetListOfFiles())

def blind(var, varToblind, plot, min, max ) :
  if var==varToblind :
   for bin in range(plot.Hist("Data").weighted.GetNbinsX()):
     if ( plot.Hist("Data").weighted.GetBinCenter(bin+1)>min and plot.Hist("Data").weighted.GetBinCenter(bin+1)<max ):
        plot.Hist("Data").weighted.SetBinContent(bin+1,-1)

def printYields(region, plots, mass=-1, fileout=None, qcd=False, susy=False) :
  if fileout != None : print >> fileout, 'Yields in '+region
  else               : print             'Yields in '+region
  contributions = ["Data" ,"TTJets" ,"DYJets" ,"DYJets_Electron" ,"DYJets_Fakes", "WJets", "WW", 'ZZ', 'WZ'] ## , "WJets_Fakes"
  if qcd != False : contributions.extend(['QCDdata'])
  if mass!= -1 and not susy : 
    contributions.extend(['HiggsGGH' +str(mass),'HiggsVBF'+str(mass),'HiggsVH' +str(mass)])
  if mass!= -1 and susy : 
    contributions.extend(['HiggsSUSYBB' +str(mass),'HiggsSUSYGluGlu'+str(mass)])
  for c in contributions :
    if fileout != None : print >> fileout, c+':'+' '*(15-len(c)),"{0:.2f}".format(round(integralAndError(plots.Hist(c).weighted)[0],2)) ,' '*(9-len("{0:.2f}".format(round(plots.Hist(c).Integral(),2))))+"+-","{0:.2f}".format(round(integralAndError(plots.Hist(c).weighted)[1],2))
    else               : print             c+':'+' '*(15-len(c)),"{0:.2f}".format(round(integralAndError(plots.Hist(c).weighted)[0],2)) ,' '*(9-len("{0:.2f}".format(round(plots.Hist(c).Integral(),2))))+"+-","{0:.2f}".format(round(integralAndError(plots.Hist(c).weighted)[1],2))

def removekey(dictionary, key):
  r = dict(dictionary)
  del r[key]
  return r

def shrinkEmbed(selComps, weights, abcd='') :
  '''
  pass the name of the sample to be dropped (useful for ABC vs D comparison)
  A ==> remove embed_Run2012A_13Jul2012_v1 and embed_Run2012A_recover_06Aug2012_v1
  B ==> remove embed_Run2012B_13Jul2012_v4
  C ==> remove embed_Run2012C_24Aug2012_v1 and embed_Run2012C_PromptReco_v2
  D ==> remove embed_2012D_PromptReco_v1
  '''

  dic = { 'A':['embed_Run2012A_13Jul2012_v1','embed_Run2012A_recover_06Aug2012_v1'],
          'B':['embed_Run2012B_13Jul2012_v4'],
          'C':['embed_Run2012C_24Aug2012_v1','embed_Run2012C_PromptReco_v2'],
          'D':['embed_2012D_PromptReco_v1'] }

  if abcd == '' : 
    return selComps, weights
  else :
    for i in dic.keys() :
      if i in abcd :
        for l in dic[i] :
          selComps = removekey(selComps, l)
          weights  = removekey(weights , l)
    return selComps, weights

def lineno():
  '''Returns the current line number in our program.'''
  return inspect.currentframe().f_back.f_lineno

def integralAndError(hist):
  error    = numpy.array([0.])
  integral = hist.IntegralAndError(1, hist.GetNbinsX(),error)
  return integral, error[0]

def prepareComponents(dir, config):
  # list of components from configuration file
  selComps = dict( [ (comp.name, comp) for comp in config.components ])
  weights  = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
  return selComps, weights

###### load weights, if needed, can be commented out
#if path.exists( os.environ.get('CMSSW_BASE')+'/src/CMGTools/H2TauTau/python/proto/rootlogonMoriond.C'):
#    gROOT.Macro(os.environ.get('CMSSW_BASE')+'/src/CMGTools/H2TauTau/python/proto/rootlogonMoriond.C')  # Run ROOT logon script
#print 'path exists?: ', path.exists(os.environ.get('CMSSW_BASE')+'/src/CMGTools/H2TauTau/python/proto/rootlogonMoriond.C')  # Run ROOT logon script

### some switches
run2012  = True
blinding = True
just125  = True
embZtt   = True
datacard = 'svfitMass'  ## 'visMass'
logVars  = []           ## ["met","jet1Pt","jet2Pt","l1Pt","l2Pt"]

if __name__ == '__main__':

  from optparse import OptionParser
  from CMGTools.RootTools.RootInit import *

  parser = OptionParser()
  parser.usage = '''
  %prog <anaDir> <cfgFile>

  cfgFile: analysis configuration file, see CMGTools.H2TauTau.macros.MultiLoop
  anaDir: analysis directory containing all components, see CMGTools.H2TauTau.macros.MultiLoop.

  hist: histogram you want to plot
  '''
  parser.add_option("-S", "--shift"    , dest = "shift"     ,  help = "TES nom,up,down. Default is nom" , default = 'nom'      )
  parser.add_option("-P", "--prong"    , dest = "prong"     ,  help = "diTau 1 prong. Default False"    , default = False      )
  parser.add_option("-B", "--box"      , dest = "box"       ,  help = "box. Default is Inclusive"       , default = 'Inclusive')
  parser.add_option("-M", "--mtregion" , dest = "mtregion"  ,  help = "mT region. Default is LowMT"     , default = 'LowMT'    )
  parser.add_option("-H", "--histlist" , dest = "histlist"  ,  help = "histogram list"                  , default = None       )
  parser.add_option("-C", "--cut"      , dest = "cut"       ,  help = "cut to apply in TTree::Draw"     , default = '1'        )
  parser.add_option("-G", "--histgroup", dest = "histgroup" ,  help = "histogram group"                 , default = None       )
  parser.add_option("-R", "--rebin"    , dest = "rebin"     ,  help = "rebinning factor"                , default = None       )
  parser.add_option("-T", "--susy"     , dest = "susy"      ,  help = "run on susy"                     , default = False      )
  parser.add_option("-E", "--embed"    , dest = "embed"     ,  help = "Use embedd samples."             , default = False      , action="store_true")
      
  (options,args) = parser.parse_args()
  if len(args) != 2:
      parser.print_help() 
      sys.exit(1)

  dataName = 'Data'
  weight   = 'weight'
  ##################################################
  ##       APPLY TRIGGER ON MC NOT ON DATA        ##
  ##################################################
  weight  += "*( (triggerWeight<=0 || embedWeight!=1) + (triggerWeight>0 && embedWeight==1)/triggerWeight )"
  
  anaDir      = args[0]
  hists       = histogramSet( options )
  cfgFileName = args[1]
  file        = open( cfgFileName, 'r' )
  #print cfgFileName
  #print cfgFileName.replace('_gold_cfg.py','_'+options.shift+'_gold_cfg.py')
  cfg         = imp.load_source( 'cfg', cfgFileName, file)
  
  selComps, weights = prepareComponents(anaDir, cfg.config)
  
  ##################################################
  ##                  SWITCH SUSY                 ##
  ##################################################
  susy = options.susy 

  ##################################################
  ##                SWITCH EMBEDDED               ##
  ##################################################
  options.embed = embZtt
  crossCheckZee = embZtt
  
  ##################################################
  ##         PASS THE PERIOD(S) TO DROP           ##
  ##################################################
  if embZtt : selComps, weights = shrinkEmbed(selComps, weights, '')          
  
  ##################################################
  ##  READS THE TREES AND CREATES THE COMPONENTS  ##
  ##################################################
  selCompsDataMass, weightsDataMass = componentsWithData( selComps, weights, susy )
  selCompsNoSignal, weightsNoSignal = componentsWithOutSignal( selComps, weights, susy )      
  
  if options.embed:
      embeddedScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, options.prong)
  if crossCheckZee:
      zeeScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, options.embed)
  
  if options.prong :
    baseline   += ' && abs(jet1Eta)<4.7 && jet1Pt>30. '  ### only for 1p
    baseline   += ' && l1DecayMode<3 && l2DecayMode<3 '  ### only for 1p
  else :  
    baseline   += ' && abs(jet1Eta)<3.0 && jet1Pt>50. '
    #baseline   += ' && (l1DecayMode>2 || l2DecayMode>2) '  ### only for !(1p)
  
  #baseline  += ' && l1DecayMode<3 && l2DecayMode<3 ' 
  baselineSS = baseline.replace('diTauCharge==0','diTauCharge!=0')

  cuts=[  
      ("CMS_2012_nobtag_BOOSTED" , baseline + ' & nbJets==0', '45', ' & pThiggs>140.' , isolationTM , 5 ),
      ("CMS_2012_btag_BOOSTED"   , baseline + ' & nbJets> 0', '45', ' & pThiggs>140.' , isolationTM , 5 ),


      #("CMS_2012_1pr_VBF"     , baseline + VBFmedium  , '45', ' && pThiggs> 70.' , isolationMM , 5 ),
      #("CMS_2012_1pr_BOOSTED" , baseline + NOVBFmedium, '45', ' && pThiggs>110.' , isolationTT , 5 ),
      ]
  
  ##################################################
  ##      LOOP OVER THE DIFFERENT SELECTIONS      ##
  ##################################################
  for prefix,cut,ptcut,antiqcdcut,isocut,qcdEstimate in cuts:
        
    prefix += '_'+options.shift
    
    ptCut              = ptcut
    ptCut_up           = ptCut+'*0.97'
    ptCut_down         = ptCut+'*1.03'
 
    ptCutString        =  ' && l1Pt>'+ptCut      +' && l2Pt>'+ptCut
    ptCutString_up     =  ' && l1Pt>'+ptCut_up   +' && l2Pt>'+ptCut_up
    ptCutString_down   =  ' && l1Pt>'+ptCut_down +' && l2Pt>'+ptCut_down
 
    bareCut = cut

    if str(prefix).find("VBF") > 0 :
      rebin  = 2
      mjjMin = 100
      mjjMax = 1000
      mjjBin = 30
    else :
      rebin = 1
      mjjMin = 0
      mjjMax = 800
      mjjBin = 20
    
    #####################################################
    ##histos you want to produce, defined in cuts_diTau##
    #####################################################
    variables = hists_pref( rebin, mjjMin, mjjMax, mjjBin, susy )
 
    #####################################################
    ###               SAVE LIST OF CUTS               ###
    #####################################################
    Cutlist = open(os.getcwd()+"/list_of_Cuts.txt","a")
    print >> Cutlist, prefix
    print >> Cutlist, cut+antiqcdcut+isocut, 'Taus pT', ptCutString,  'QCD estimate', qcdEstimate
    print >> Cutlist, '\n\n' 

    for varLoop in variables:
 
      if len(varLoop) == 4 :
        var  = varLoop[0]
        nx   = varLoop[1] 
        xmin = varLoop[2]
        xmax = varLoop[3]
      else :
        var  = varLoop[0]
        nx   = varLoop[2]
        xmin = None
        xmax = None
                                       
      cut     = bareCut
      print 'I\'ve stripped the old pt cut\n', cut
      if   ( var == 'svfitMass*1.03' or var == 'visMass*1.03' or var == 'l1Pt*1.03' ):
        cut += ptCutString_up
      elif ( var == 'svfitMass*0.97' or var == 'visMass*0.97' or var == 'l1Pt*0.97' ):
        cut += ptCutString_down 
      else :
        cut += ptCutString
      
      print 'I\'m using this cut string\n',cut+isocut+antiqcdcut    
      
      prefix1 = os.getcwd()+"/"+prefix+"/diTau_2012_"
      dirList = os.listdir(os.getcwd())
      exists = False
      for fname in dirList:
        if fname == prefix :
          exists = True
      if not(exists) :
        os.mkdir(os.getcwd()+"/"+prefix)
          
      if var in logVars or susy:
        log = True
      else:
        log = False
  
      #########################################################
      ##         DEFINITION OF LOOSE ISO CUT FOR QCD         ##
      #########################################################
      looseisocut     = isolationLL4+" && !(1 "+isocut+")"
                
      cutSS = cut.replace("diTauCharge==0","diTauCharge!=0")
                                  
      #########################################################
      ##            RELAXING VBF SELECTION FOR QCD           ##
      #########################################################

      if 'VBF' in prefix : 
         cutLooseOS      = baseline   + ptCutString + antiqcdcut + isolationLL4 + ' && (l1MedMVAIso<0.5 || l2MedMVAIso<0.5) ' + ' && jet1Pt>50 && jet2Pt>30 && abs(jet2Eta)<4.7 ' 
         cutLooseSS      = baselineSS + ptCutString + antiqcdcut + isolationLL4 + ' && (l1MedMVAIso<0.5 || l2MedMVAIso<0.5) ' + ' && jet1Pt>50 && jet2Pt>30 && abs(jet2Eta)<4.7 ' 
      else :
         cutLooseOS      = baseline   + ptCutString + looseisocut + antiqcdcut
         cutLooseSS      = baselineSS + ptCutString + looseisocut + antiqcdcut
              
      #########################################################
      ##         CREATE THE THREE QCD CONTROL REGIONS        ##
      #########################################################

      TightIsoSS   = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal, nx, xmin, xmax,  
                                    cut    = cutSS+isocut+antiqcdcut, 
                                    weight = weight,                 
                                    embed  = options.embed)
                 
      LooseIsoSS   = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal, nx, xmin, xmax,                              
                                    cut    = cutLooseSS,
                                    weight = weight,#+"*weightQCD_nVert(nVert)*weightQCD_HpT(pThiggs)",                     
                                    embed  = options.embed) 
                  
      # for the QCD estimation MC contributions are subtracted in the consistent region NO NEED TO APPLY THE WEIGHT ON THEM
      LooseIsoOSMC = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal, nx, xmin, xmax,                             
                                    cut    = cutLooseOS, 
                                    weight = weight,                  
                                    embed  = options.embed)
      
      LooseIsoOS   = H2TauTauDataMC(var, anaDir, selCompsNoSignal, weightsNoSignal, nx, xmin, xmax,                             
                                    cut    = cutLooseOS, 
                                    #weight=weight+"*weightQCD_nVert(nVert)",                   
                                    #weight=weight+"*weightQCD_nVert(nVert)*weightQCD_l1jetPt(l1jetPt)",                   
                                    weight = weight,                   
                                    embed  = options.embed)
      
      #######################################################
      ###               PLOTTING DATA/MC                  ###
      #######################################################
      yields = False
      if 'svfitMass' in var or 'visMass' in var or 'l1Pt' in var :
        if just125 : massesRange = [125]
        else       : massesRange = [125,110,115,120,130,135,140,145]
        if susy    : massesRange = [80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,1000]
        print 'I\'m plotting mass distribution for masses in',massesRange,' GeV'
        if (var == "svfitMass" or var == "visMass") :
          yields = True
      else :
        massesRange = [125]
        if susy : 
          massesRange = [160,600]
        print 'I\'m plotting distribution just for mass 125 GeV'

      #####################################################
      ###                 QCD ESTIMATION                ###
      #####################################################
      QCDShape, QCDScale, QCDlooseSS, QCDtightSS, tightLooseErr = QCDEstimate2( prefix, prefix1, var, xmin, xmax,
                                                                                TightIsoSS, 
                                                                                LooseIsoOS, 
                                                                                LooseIsoOSMC, 
                                                                                LooseIsoSS, 
                                                                                log )
        
      print "QCD yield uncertainty:", tightLooseErr
      print "QCDShape yield", QCDShape.Integral()
       
      #####################################################
      ###       PRINT YIELDS in QCD CONTROL REGION      ###
      #####################################################
      if yields:
        Yields_dump = open(os.getcwd()+"/"+prefix+"/Yields_OSloose"+var+".txt","w")
        printYields('OS loose', LooseIsoOS, fileout=Yields_dump, susy=susy)       
        Yields_dump = open(os.getcwd()+"/"+prefix+"/Yields_SStight"+var+".txt","w")
        printYields('SS tight', TightIsoSS, fileout=Yields_dump, susy=susy)       
        Yields_dump = open(os.getcwd()+"/"+prefix+"/Yields_SSloose"+var+".txt","w")
        printYields('SS loose', LooseIsoSS, fileout=Yields_dump, susy=susy)            

      #####################################################
      ###        LOOP OVER DIFFERENT HIGGS MASSES       ###
      #####################################################

      for mIndex in massesRange :    
        TightIsoOS = H2TauTauDataMC(var, anaDir, selCompsDataMass[mIndex], weightsDataMass[mIndex], nx, xmin, xmax,
                                    cut = cut+isocut+antiqcdcut, 
                                    weight=weight,#+'*( (genMass>0.)*weightZTT_dRtt(dRtt) + (genMass<=0.) )',
                                    embed=options.embed)

        #####################################################
        ###           ADD QCD TO OS TIGHT STACK           ###
        #####################################################
        qcdshape = deepcopy(QCDShape.weighted)
        qcdshape.Scale(QCDScale)

        TightIsoOS.AddHistogram("QCDdata",qcdshape)
        TightIsoOS.Hist('QCDdata').stack = True
        TightIsoOS.Hist('QCDdata').SetStyle( sHTT_QCD )
        TightIsoOS.Hist('QCDdata').layer = 0.99

        #####################################################
        ###              WJets Estimation                 ###
        #####################################################
            
        if 'VBF'     in prefix : scaleFromMuTau  = 1.06  # for 2012D by Jose          
        if 'BOOSTED' in prefix : scaleFromMuTau  = 0.65  # for 2012D by Jose
  
        WJetsShape        = deepcopy(LooseIsoOSMC.Hist("WJets"))
        WJetsShapeSS      = deepcopy(LooseIsoSS.Hist("WJets"))
        
        WJetsScale        = scaleFromMuTau * TightIsoOS.Hist('WJets').Integral() / LooseIsoOSMC.Hist('WJets').Integral()
        WJetsScaleSS      = scaleFromMuTau * TightIsoSS.Hist('WJets').Integral() / LooseIsoSS.Hist('WJets').Integral()

        TightIsoOS.Hist("WJets").obj      = WJetsShape.obj
        TightIsoOS.Hist("WJets").weighted = WJetsShape.weighted
        TightIsoOS.Hist("WJets").Scale(WJetsScale)

        TightIsoSS.Hist('WJets').obj      = WJetsShapeSS.obj
        TightIsoSS.Hist('WJets').weighted = WJetsShapeSS.weighted
        TightIsoSS.Hist('WJets').Scale(WJetsScaleSS)

        #####################################################
        ###         PRINT YIELDS in SIGNAL REGION         ###
        #####################################################
        
        if yields :                                        
          Yields_dump = open(os.getcwd()+"/"+prefix+"/Yields_"+var+"_mH"+str(mIndex)+".txt","w")
          printYields('OS tight', TightIsoOS, mass=mIndex, fileout=Yields_dump, qcd=True, susy=susy)            
                             
        #####################################################
        ###     SAVE ROOT FILES FOR LIMIT COMPUTATION     ###
        #####################################################
	
        if datacard in var and isinstance(nx, numpy.ndarray) and 'VBF'     in prefix :
          saveForLimit(deepcopy(TightIsoOS),prefix,mIndex,datacard,"vbf"  ,susy)
        if datacard in var and isinstance(nx, numpy.ndarray) and 'BOOSTED' in prefix :
          saveForLimit(deepcopy(TightIsoOS),prefix,mIndex,datacard,"boost",susy)
                
        #####################################################
        ###   BOOSTING THE SIGNAL FOR PICTORIAL RESULTS   ###
        #####################################################
        if susy :
          TightIsoOS.Hist(str('HiggsSUSYBB'    +str(mIndex))).Scale(5)
          TightIsoOS.Hist(str('HiggsSUSYGluGlu'+str(mIndex))).Scale(5)
        else :
          TightIsoOS.Hist(str('HiggsGGH'+str(mIndex))).Scale(5)
          TightIsoOS.Hist(str('HiggsVBF'+str(mIndex))).Scale(5)
          TightIsoOS.Hist(str('HiggsVH' +str(mIndex))).Scale(5)
        
        #####################################################
        ###                 GROUPING BKGS                 ###
        #####################################################
                  
        TightIsoOS.Group('electroweak'                                   , ['WJets', 'DYJets_Electron', 'DYJets_Fakes','ZZ','WZ','WW'])#,'WJets_Fakes'])
        TightIsoOS.Group('t#bar{t}'                                      , ['TTJets','Tbar_tW','T_tW'])
        TightIsoOS.Group('Fakes'                                         , ['QCDdata'])
        TightIsoOS.Group('Z#rightarrow#tau#tau'                          , ['DYJets'])
        if susy :
          TightIsoOS.Group('5x#phi('+str(mIndex)+' GeV)#rightarrow#tau#tau, tan#beta=20'  , ['HiggsSUSYBB'+str(mIndex), 'HiggsSUSYGluGlu'+str(mIndex)])         
        else :
          TightIsoOS.Group('5xH('   +str(mIndex)+' GeV)#rightarrow#tau#tau'  , ['HiggsVBF'+str(mIndex), 'HiggsGGH'+str(mIndex), 'HiggsVH'+str(mIndex)]) 

        #for h in ['electroweak','t#bar{t}','Z#rightarrow#tau#tau','5x H('+str(mIndex)+' GeV)#rightarrow#tau#tau','Data','Fakes'] :
        for h in ['electroweak','t#bar{t}','Z#rightarrow#tau#tau','Data','Fakes'] :
          TightIsoOS.Hist(h).weighted.SetTitle('CMS preliminary 2012, #sqrt{s} = 8TeV, L = 19.4 fb^{-1}        #tau_{h}#tau_{h}')
          TightIsoOS.Hist(h).GetXaxis().SetTitle(xtitles[var])
          if TightIsoOS.Hist(h).Integral()<0.1 :
            TightIsoOS.Group('Fakes', ['QCDdata',h])
          if isinstance(nx, numpy.ndarray):
            TightIsoOS.Hist(h).GetYaxis().SetTitle('dN/dm_{#tau#tau} [1/GeV]')
          else :
            TightIsoOS.Hist(h).GetYaxis().SetTitle('Events')
          
        #####################################################
        ###           BLINDING DATA ABOVE Z PEAK          ###
        #####################################################
        
        if blinding and not susy:
          blind(var, 'svfitMass', TightIsoOS, 100., 150. )
          blind(var, 'visMass'  , TightIsoOS,  80., 120. )
          #blind(var, 'dRtt'     , TightIsoOS,   0.,   2. )
        if blinding and susy:
          blind(var, 'svfitMass', TightIsoOS, 100., 1500. )
          blind(var, 'visMass'  , TightIsoOS,  80., 1500. )

        #####################################################
        ###                    PLOTTING                   ###
        #####################################################         
        gPad.SetCanvasSize(700,700)
        gPad.SetLeftMargin(0.15)
        gPad.SetBottomMargin(0.15)
        gPad.SetFrameLineWidth(3)
        
        ## normalizing to bin width if variable binning 
        if isinstance(nx, numpy.ndarray) :
          TightIsoOS.NormalizeToBinWidth()
        
        TightIsoOS.DrawStack("HIST")
        
        gPad.SaveAs(prefix1+prefix+'_'+TightIsoOS.varName+"_mH"+str(mIndex)+"_data.pdf")
                  
        #####################################################
        ### SAVE ROOT FILE TO ACCESS THE TEMPLATES EASILY ###
        #####################################################
        
        if not datacard in var or ((len(nx) == 1 and nx<100000) or isinstance(nx, numpy.ndarray)) :
          saveForPlotting(deepcopy(TightIsoOS),prefix,mIndex,susy)
        
        