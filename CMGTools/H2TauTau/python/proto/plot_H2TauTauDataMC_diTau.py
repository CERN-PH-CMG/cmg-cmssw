import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import H2TauTauDataMC
from CMGTools.RootTools.Style import formatPad
from ROOT import kPink

def prepareComponents(dir, config):
    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
    return selComps, weights
    
    
def DYScale( svfitMassplot, dataName ):
    
    # DYs_data = data - WJets - TTbar 
    DY = copy.deepcopy(svfitMassplot.Hist(dataName))
    DY.Add(svfitMassplot.Hist('WJets'), -1)
    DY.Add(svfitMassplot.Hist('TTJets'), -1)

    # adding the DYs_data estimation to the stack
    svfitMassplot.AddHistogram( 'Data - WJets - TT', DY.weighted, 1010)
    svfitMassplot.Hist('Data - WJets - TT').stack = False
    # with a nice pink color
    pink = kPink+7
    sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
    svfitMassplot.Hist('Data - WJets - TT').SetStyle( sPinkHollow )

    # determine scaling factor for the DY MC
    svfitMassmin, svfitMassmax = 0, 100
    # scale = DYs_data / DYs 
    scale_DYs = svfitMassplot.Hist('Data - WJets - TT').Integral(True, svfitMassmin, svfitMassmax) \
                  / svfitMassplot.Hist('DYJets_Fakes').Integral(True, svfitMassmin, svfitMassmax)

    # hide the DYs_data component from the svfitMassplot. can be set to True interactively
    svfitMassplot.Hist('Data - WJets - TT').on = True

    return scale_DYs

NBINS = 100
XMIN  = 0
XMAX  = 200

if __name__ == '__main__':

    import copy
    from optparse import OptionParser
    from CMGTools.RootTools.RootInit import *

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
    selCompsMC = {}
    selCompsMC['DYJets']=copy.deepcopy(selComps['DYJets'])
    selCompsMC['WJets']=copy.deepcopy(selComps['WJets'])
    selCompsMC['TTJets']=copy.deepcopy(selComps['TTJets'])
    selCompsMC['ggHTT125']=copy.deepcopy(selComps['ggHTT125'])
    selCompsMC['VBFHTT125']=copy.deepcopy(selComps['VBFHTT125'])
    weightsMC = {}
    weightsMC['DYJets']=copy.deepcopy(weights['DYJets'])
    weightsMC['WJets']=copy.deepcopy(weights['WJets'])
    weightsMC['TTJets']=copy.deepcopy(weights['TTJets'])
    weightsMC['ggHTT125']=copy.deepcopy(weights['ggHTT125'])
    weightsMC['VBFHTT125']=copy.deepcopy(weights['VBFHTT125'])
    
    # 1fb
    selComps['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    weights['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
    weights['data_Run2011A_PromptReco_v6'].intLumi = 0.
    
    # IsoTau20
    #selComps['data_Run2011A_PromptReco_v4'].intLumi = 31.5
    #weights['data_Run2011A_PromptReco_v4'].intLumi = 31.5

    # IsoTau25
    #selComps['data_Run2011A_May10ReReco_v1'].intLumi = 0.
    #weights['data_Run2011A_May10ReReco_v1'].intLumi = 0.
    #selComps['data_Run2011A_PromptReco_v4'].intLumi = 139.
    #weights['data_Run2011A_PromptReco_v4'].intLumi = 139.
    #selComps['data_Run2011A_05Aug2011_v1'].intLumi = 0.
    #weights['data_Run2011A_05Aug2011_v1'].intLumi = 0.
    #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
    #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.

    # IsoTau35
    #selComps['data_Run2011A_May10ReReco_v1'].intLumi = 0.
    #weights['data_Run2011A_May10ReReco_v1'].intLumi = 0.
    #selComps['data_Run2011A_PromptReco_v4'].intLumi = 759.2
    #weights['data_Run2011A_PromptReco_v4'].intLumi = 759.2
    #selComps['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    #weights['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
    #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.

    cuts_control_Z2 = 'l1Pt>45 && l2Pt>45 && diTauCharge==0 && svfitMass<100'

    cuts=[#("controlZ45",cuts_control_Z2),
	  #("ATLAS45",cuts_analysis2),
          #("ATLAS",'l1Pt>35 && l2Pt>25 && diTauCharge==0 && jet1Pt>40 && met>20 && dRtt<2.2 && mttj>225 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl25met30",'l1Pt>25 && l2Pt>25 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt<30 && met>30 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl25met30j30",'l1Pt>25 && l2Pt>25 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && met>30 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl35met30",'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt<30 && met>30 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl35met30j30",'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && met>30 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl45met30",'l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt<30 && met>30 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl45met30j30",'l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && met>30 && l1MedIso>0.5 && l2MedIso>0.5'),
	  ("CMSl45met20j30dR",'l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && met>20 && l1MedIso>0.5 && l2MedIso>0.5 && dRtt<2.2'),
	  ("CMSl45j30j30dR",'l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && jet2Pt>30 && (jet1Eta*jet2Eta)<0 && l1TigIso>0.5 && l2TigIso>0.5 && dRtt<2.2'),
	  #("CMSl45met20j30",'l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && met>20 && l1MedIso>0.5 && l2MedIso>0.5'),
	  #("CMSl45met20j30tight",'l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>30 && met>20 && l1TigIso>0.5 && l2TigIso>0.5'),
	  ]

    # get DY scaling factor for opposite sign
    #svfitMassOS2 = H2TauTauDataMC('svfitMass', anaDir, selComps, weights,
    #                      NBINS, XMIN, XMAX, 
    #                      cut = cuts_control_Z2, weight=weight,
    #                      embed=options.embed)
    #DYScaleOS2 = DYScale( svfitMassOS2, dataName)

    prefix1="~/workspace/ditau/plots/IsoTau1fb_"

    for prefix,cut in cuts:
     for var, nx, xmin, xmax in [
         ('mt',20, 0, 200),
         ('visMass',30, 0, 300),
         ('svfitMass',30, 0, 300),
         ('l1Pt',25, 0, 250),
         ('l2Pt',25, 0, 250),
         ('l1Eta',20, -3, 3),
         ('l2Eta',20, -3, 3),
         ('jet1Eta',20, -5, 5),
         ('jet1Pt',20, 0, 500),
         ('dRtt',20, 0, 5),
         ('mttj',20, 0, 1000),
         ('met',20, 0, 200),
         ('diTauCharge',7, -3, 3),
         ('l1MedIso',4, -1, 2),
         ('l2MedIso',4, -1, 2),
         ('l1TigIso',4, -1, 2),
         ('l2TigIso',4, -1, 2),
      ]:
      if "45" in cut and "Mass" in var:
          nx/=2
      plotVarDataSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0"), weight=weight,
     			    embed=options.embed)
      plotVarDataLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace(" && l1MedIso>0.5 && l2MedIso>0.5","").replace(" && l1TigIso>0.5 && l2TigIso>0.5",""), weight=weight, #.replace("met>20","met>10").replace("met>30","met>15")
     			    embed=options.embed)
      plotVarDataLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace(" && l1MedIso>0.5 && l2MedIso>0.5","").replace(" && l1TigIso>0.5 && l2TigIso>0.5","").replace("diTauCharge==0","diTauCharge!=0"), weight=weight, #.replace("met>20","met>10").replace("met>30","met>15")
     			    embed=options.embed)
      plotVarDataSS.DrawStack("HIST",xmin,xmax,0,plotVarDataSS.Hist("Data").GetMaximum()*2.0)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_qcd.png")
      #gPad.WaitPrimitive()
      print "QCD lumi:", plotVarDataSS.intLumi
      print "QCD in SS, met>30:", plotVarDataSS.Hist("Data").Integral()
      print "QCD in LooseIso SS, met>30:", plotVarDataLooseIsoSS.Hist("Data").Integral()
      print "DYJets in LooseIso SS, met>30:", plotVarDataLooseIsoSS.Hist("DYJets").Integral()
      print "WJets in LooseIso SS, met>30:", plotVarDataLooseIsoSS.Hist("WJets").Integral()
      print "TTJets in LooseIso SS, met>30:", plotVarDataLooseIsoSS.Hist("TTJets").Integral()
      QCDdata=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      QCDdata.Add(plotVarDataLooseIsoSS.Hist("DYJets"),-1)
      QCDdata.Add(plotVarDataLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDdata.Add(plotVarDataLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDdata.Add(plotVarDataLooseIsoSS.Hist("WJets"),-1)
      QCDdata.Add(plotVarDataLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDdata.Add(plotVarDataLooseIsoSS.Hist("TTJets"),-1)
      print "QCD estimate LooseIso:", QCDdata.Integral()
      QCDscale=plotVarDataSS.Hist("Data").Integral()/QCDdata.Integral()
      print "QCD / QCDLooseIso", QCDscale

      plotVarDataOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut, weight=weight,
     			    embed=options.embed)
      plotVarDataOS.AddHistogram("QCDdata",QCDdata.weighted)
      plotVarDataOS.Hist('QCDdata').stack = True
      plotVarDataOS.Hist('QCDdata').SetStyle( sBlack )
      plotVarDataOS.Hist('QCDdata').layer = 5
      plotVarDataOS.Hist('QCDdata').Scale(QCDscale)
      plotVarDataOS.Hist('ggHTT125').Scale(5)
      plotVarDataOS.Hist('VBFHTT125').Scale(5)
      plotVarDataOS.DrawStack("HIST",xmin,xmax,0,plotVarDataOS.Hist("Data").GetMaximum()*2.0)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataOS.varName+"_data.png")
      #gPad.WaitPrimitive()

      plotVar = H2TauTauDataMC(var, anaDir, selCompsMC, weightsMC,
     			    nx, xmin, xmax,
     			    cut = cut, weight=weight,
     			    embed=options.embed)
      print "MC lumi:", plotVar.intLumi
      plotVar.AddHistogram("QCDdata",QCDdata.weighted)
      plotVar.Hist('QCDdata').stack = True
      plotVar.Hist('QCDdata').SetStyle( sBlack )
      plotVar.Hist('QCDdata').layer = 5
      plotVar.Hist('QCDdata').Scale(plotVar.intLumi/plotVarDataOS.intLumi)
      plotVar.Hist('QCDdata').Scale(QCDscale)
      plotVar.Hist('ggHTT125').Scale(5)
      plotVar.Hist('VBFHTT125').Scale(5)
      plotVar.DrawStack("HIST",xmin,xmax,0,
         (plotVar.Hist("QCDdata").GetMaximum()+
	  plotVar.Hist("DYJets").GetMaximum()+
	  plotVar.Hist("DYJets_Fakes").GetMaximum()+
	  plotVar.Hist("DYJets_Photon").GetMaximum()+
	  plotVar.Hist("WJets").GetMaximum()+
	  plotVar.Hist("WJets_Fakes").GetMaximum())*1.5)
      gPad.SaveAs(prefix1+prefix+'_'+plotVar.varName+"_mc.png")
      #gPad.WaitPrimitive()
