import imp
from CMGTools.HToZZTo4Leptons.HToZZDataMC import HToZZDataMC
from CMGTools.RootTools.Style import formatPad
from ROOT import kPink

def prepareComponents(dir, config):
    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
    return selComps, weights
    
    

NBINS = 100
XMIN  = 100
XMAX  = 600
YMIN  = 0.001

def savePlot(name):
    if gPad is None:
        print 'no active canvas'
        return
    fileName = '%s/%s' % (anaDir, name)
    print 'pad', gPad.GetName(), 'saved to', fileName    
    gPad.SaveAs( fileName )   


#TODO:
# include ZZ - OK 
# group ZZ 
# include data
# group data
# get Higgs signals out of the stack
# set up data driven background estimation (relax charge, flavour, sip, etc)


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
    parser.add_option("-X", "--xmax", 
                      dest="xmax", 
                      help="xmax",
                      default=XMAX)
    parser.add_option("-x", "--xmin", 
                      dest="xmin", 
                      help="xmin",
                      default=XMIN)
    parser.add_option("-Y", "--ymax", 
                      dest="ymax", 
                      help="ymax",
                      default=None)
    parser.add_option("-y", "--ymin", 
                      dest="ymin", 
                      help="ymin",
                      default=YMIN)
    parser.add_option("-H", "--hist", 
                      dest="hist", 
                      help="histogram",
                      default='HMass')
    parser.add_option("-C", "--cut", 
                      dest="cut", 
                      help="cut to apply in TTree::Draw",
                      default='HMass>0')
    parser.add_option("-G", "--histgroup", 
                      dest="histgroup", 
                      help="histogram group",
                      default=None)
    parser.add_option("-R", "--rebin", 
                      dest="rebin", 
                      help="rebinning factor",
                      default=None)
    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)

    dataName = 'Data'
    weight='1'

    anaDir = args[0]
    # hists = histogramSet( options )
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
        
    
    # get WJet scaling factor for same sign
    nbins = NBINS
    xmin = float( options.xmin )
    xmax = float( options.xmax )
    plot = HToZZDataMC(options.hist, anaDir, selComps, weights,
                         nbins,xmin,xmax,
                         cut = options.cut, weight=weight)

    ymax = None
    if options.ymax is not None:
        ymax = float(options.ymax)
    plot.DrawStack('HIST',ymin=float(options.ymin), ymax=ymax)
    # gPad.SetLogy()
